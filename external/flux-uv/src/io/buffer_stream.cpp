/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Stream Buffer implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/buffer/stream.hpp>
#include <flux/io/buffer/memread.hpp>
#include <flux/io/stream.hpp>
#include <flux/io/errors.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/stdexcept.hpp>

#include <flux/compat/byte.hpp>
#include <flux/report/badness.hpp>

#include <flux/util/narrow.hpp>
#include <flux/util/scope_exit.hpp>

#include <cstdlib>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	BufferSize gBufferMaxSizeLazy_ = 128*1024*1024;
	BufferSize gBufferMaxSizeEager_ = 512*1024*1024;
	
	class BufferStreamLazy_ final : public Buffer
	{
		public:	
			BufferStreamLazy_( Stream*, bool, StreamSize, StreamSize );
			~BufferStreamLazy_();

		public: /* virtuals from Stream */
			compat::span<compat::byte> map( EBufferMapRW, BufferSize, BufferSize ) override;
			compat::span<compat::byte const> map( EBufferMapRO, BufferSize, BufferSize ) override;

			void unmap( compat::span<compat::byte const> const& ) override;

			BufferCaps caps() const noexcept override;
			BufferSize size() const override;

			void flush( compat::span<compat::byte const> const& ) override;
			void resize( BufferSize, bool aKeepContents ) override;

			std::string description() const override;
			ftl::Any native_handle() const override;

		private:
			Stream* mStream;
			std::string mDesc;
			StreamSize mBegin;
			StreamSize mMaxSize;
			bool mDispose;
	};

	FLUX_REPORT_DEFINE_BADNESS_EX( BufferStream, cold, "BufferStream", "Synchronously reading stream into memory buffer in hot code." );

#	if FLUX_COMPILER == FLUX_COMPILER_GCC && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ <= 9))
#		define FLUXTMP_SUPERFLUOUS_MOVE_(x) std::move(x)
#	else
#		define FLUXTMP_SUPERFLUOUS_MOVE_(x) x
#	endif 
}

//--    buffer_create_stream()              ///{{{2///////////////////////////
unique_ptr<Buffer> buffer_create_stream( ftl::not_null<Stream*> aStream, StreamSize aMaxSize )
{
	auto const cc = aStream->caps();
	if( cc.readable && cc.remaining && cc.tell && cc.seekFwd && cc.seekRev )
		return buffer_create_stream_lazy( aStream, aMaxSize );

	return buffer_create_stream_eager( aStream, aMaxSize );
}
unique_ptr<Buffer> buffer_create_stream( unique_ptr<Stream> aStream, StreamSize aMaxSize )
{
	auto const cc = aStream->caps();
	if( cc.readable && cc.remaining && cc.tell && cc.seekFwd && cc.seekRev )
		return buffer_create_stream_lazy( std::move(aStream), aMaxSize );

	return buffer_create_stream_eager( std::move(aStream), aMaxSize );
}

//--    buffer_create_stream_eager()        ///{{{2///////////////////////////
unique_ptr<Buffer> buffer_create_stream_eager( ftl::not_null<Stream*> aStream, StreamSize aLength )
{
	FLUX_REPORT_DECLARED_BADNESS( BufferStream, 1.0 );
	
	// Try to figure out length
	auto const cc = aStream->caps();
	FLUX_THROW_IF( cc.infinite, error::BufferTooLarge )
		<< einfo::StreamDesc( aStream->description() )
		<< flux::einfo::ErrorDesc( "Refusing to buffer an infinite-size stream" )
	;
	
	if( ~StreamSize(0) == aLength && cc.remaining )
		aLength = aStream->remaining();

	// Buffer stream
	if( ~StreamSize(0) != aLength )
	{
		// OK, fixed size.
		FLUX_THROW_IF( aLength > gBufferMaxSizeEager_, error::BufferTooLarge )
			<< einfo::StreamDesc( aStream->description() )
			<< flux::einfo::ErrorDesc( "Size exceeds limit for eagerly buffered streams" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("size = {:d}, limit = {:d}"), aLength, gBufferMaxSizeEager_ )
		;

		void* ptr = ::operator new( aLength );
		FLUX_UTIL_ON_SCOPE_EXIT{ if( ptr ) ::operator delete( ptr ); };

		aStream->read(
			ptr,
			aLength,
			EIoRequest::mustComplete
		);

		auto ret = buffer_create_memread_dealloc(
			ptr,
			aLength,
			[] (void const* aPtr) { ::operator delete(const_cast<void*>(aPtr)); }
		);

		ptr = nullptr;
		return FLUXTMP_SUPERFLUOUS_MOVE_(ret);
	}
	else
	{
		// Don't know final size
		std::size_t bufferSize = 1024;
		auto* buffer = static_cast<compat::byte*>(::realloc( nullptr, bufferSize ));
		FLUX_UTIL_ON_SCOPE_EXIT{ ::free( buffer ); };

		StreamSize offset = 0;
		do
		{
			if( bufferSize == offset )
			{
				FLUX_THROW_IF( bufferSize >= gBufferMaxSizeEager_, error::BufferTooLarge )
					<< einfo::StreamDesc( aStream->description() )
					<< flux::einfo::ErrorDesc( "Size exceeds limit for eagerly buffered streams (dynamic size)" )
					<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("size = {:d}, limit = {:d}"), bufferSize, gBufferMaxSizeEager_ )
				;

				bufferSize = std::min( bufferSize + bufferSize / 2, gBufferMaxSizeEager_ );
				buffer = static_cast<compat::byte*>(::realloc( buffer, bufferSize ));
			}
			
			offset += aStream->read( buffer+offset, bufferSize-offset, EIoRequest::bestEffort );
		} while( !!(EStreamStatus::readable & aStream->status()) );

		auto ret = buffer_create_memread_dealloc(
			buffer,
			offset,
			[] (void const* aPtr) { ::free( const_cast<void*>(aPtr) ); }
		);

		buffer = nullptr;
		return FLUXTMP_SUPERFLUOUS_MOVE_(ret);
	}
}

unique_ptr<Buffer> buffer_create_stream_eager( unique_ptr<Stream> aStream, StreamSize aLength )
{
	return buffer_create_stream_eager( aStream.get(), aLength );
}

//--    buffer_create_stream_lazy()         ///{{{2///////////////////////////
unique_ptr<Buffer> buffer_create_stream_lazy( ftl::not_null<Stream*> aStream, StreamSize aMaxSize )
{
	auto const cc = aStream->caps();
	bool const reqs = cc.readable && cc.remaining && cc.tell && cc.seekFwd && cc.seekRev;
	FLUX_THROW_IF( !reqs, error::StreamOpNotSupported )
		<< einfo::StreamDesc( aStream->description() )
		<< flux::einfo::ErrorDesc( "buffer_create_stream_lazy() requires Stream to be readable, bidirectional seekable and have a known position and remaining size" )
		<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("caps = {}"), to_string(cc) )
	;

	auto const begin = aStream->tell();
	if( ~StreamSize(0) == aMaxSize )
		aMaxSize = aStream->remaining();

	return make_unique<BufferStreamLazy_>( static_cast<Stream*>(aStream), false, begin, aMaxSize );
}
unique_ptr<Buffer> buffer_create_stream_lazy( unique_ptr<Stream> aStream, StreamSize aMaxSize )
{
	auto const cc = aStream->caps();
	bool const reqs = cc.readable && cc.remaining && cc.tell && cc.seekFwd && cc.seekRev;
	FLUX_THROW_IF( !reqs, error::StreamOpNotSupported )
		<< einfo::StreamDesc( aStream->description() )
		<< flux::einfo::ErrorDesc( "buffer_create_stream_lazy() requires Stream to be readable, bidirectional seekable and have a known position and remaining size" )
		<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("caps = {}"), to_string(cc) )
	;

	auto const begin = aStream->tell();
	if( ~StreamSize(0) == aMaxSize )
		aMaxSize = aStream->remaining();

	unique_ptr<Buffer> ret = make_unique<BufferStreamLazy_>( aStream.get(), true, begin, aMaxSize );
	aStream.release();
	return FLUXTMP_SUPERFLUOUS_MOVE_(ret);
}


//--    buffer_stream_*_max_size*()         ///{{{2///////////////////////////
void buffer_stream_set_max_size_lazy( BufferSize aMaxSize ) noexcept
{
	gBufferMaxSizeLazy_ = aMaxSize;
}
void buffer_stream_set_max_size_eager( BufferSize aMaxSize ) noexcept
{
	gBufferMaxSizeEager_ = aMaxSize;
}

BufferSize buffer_stream_get_max_size_lazy() noexcept
{
	return gBufferMaxSizeLazy_;
}
BufferSize buffer_stream_get_max_size_eager() noexcept
{
	return gBufferMaxSizeEager_;
}

//--    $ BufferStreamLazy_                 ///{{{2///////////////////////////
namespace
{
	BufferStreamLazy_::BufferStreamLazy_( Stream* aStream, bool aDispose, StreamSize aBegin, StreamSize aMaxSize )
		: mStream( aStream )
		, mBegin( aBegin )
		, mMaxSize( aMaxSize )
		, mDispose( aDispose )
	{
		FLUX_ASSERT( mStream );
		FLUX_ASSERT( mStream->caps().seekFwd && mStream->caps().seekRev );
		
		mDesc = format( "streambuf:{}+{:d}", mStream->description(), aMaxSize );
	}

	BufferStreamLazy_::~BufferStreamLazy_()
	{
		if( mDispose ) delete mStream;
	}

	compat::span<compat::byte> BufferStreamLazy_::map( EBufferMapRW, BufferSize, BufferSize )
	{
		FLUX_THROW( io::error::BufferOpNotSupported )
			<< einfo::BufferMode( EBufferMode::read )
			<< einfo::BufferDesc( description() )
			<< flux::einfo::ErrorMeta( "Buffer::map(EBufferMapRW)" )
		;
	}
	compat::span<compat::byte const> BufferStreamLazy_::map( EBufferMapRO, BufferSize aBegin, BufferSize aLength )
	{
		FLUX_REPORT_DECLARED_BADNESS( BufferStream, 1.0 );

		FLUX_THROW_IF( aBegin >= mMaxSize, io::error::BufferOutOfRange )
			<< einfo::BufferMode( EBufferMode::read )
			<< einfo::BufferDesc( description() )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("size {:d}, requested offset {:d}"), mMaxSize, aBegin )
		;

		if( ~BufferSize(0) == aLength )
			aLength = mMaxSize-aBegin;

		FLUX_THROW_IF( aLength > mMaxSize-aBegin, io::error::BufferOutOfRange )
			<< einfo::BufferMode( EBufferMode::read )
			<< einfo::BufferDesc( description() )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("size {:d}, requested range {:d}+{:d}"), mMaxSize, aBegin, aLength )
		;
		FLUX_THROW_IF( aLength > gBufferMaxSizeLazy_, io::error::BufferTooLarge )
			<< einfo::BufferMode( EBufferMode::read )
			<< einfo::BufferDesc( description() )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("requested {:d} bytes, but the limit for lazy stream buffers is {:d}"), aLength, gBufferMaxSizeLazy_ )
		;

		auto* ptr = static_cast<compat::byte*>(::realloc( nullptr, aLength ));
		FLUX_ASSERT( ptr );

		mStream->seek( mBegin+aBegin, ESeekWhence::set );
		mStream->read( ptr, aLength, EIoRequest::mustComplete );
		return compat::make_span( ptr, ptr+aLength );
	}

	void BufferStreamLazy_::unmap( compat::span<compat::byte const> const& aMap )
	{
		::free( const_cast<compat::byte*>(aMap.data()) );
	}

	BufferCaps BufferStreamLazy_::caps() const noexcept
	{
		BufferCaps caps;
		caps.minOffsetAlignment = 1;
		
		caps.readable = true;
		caps.writable = false;

		caps.partialMap = true;
		caps.partialUnmap = false;
		caps.multiMap = true;

		caps.resizable = false;
		return caps;
	}
	BufferSize BufferStreamLazy_::size() const
	{
		return mMaxSize;
	}

	void BufferStreamLazy_::flush( compat::span<compat::byte const> const& )
	{}
	void BufferStreamLazy_::resize( BufferSize, bool )
	{
		FLUX_THROW( io::error::BufferOpNotSupported )
			<< einfo::BufferMode( EBufferMode::read )
			<< einfo::BufferDesc( description() )
			<< flux::einfo::ErrorMeta( "Buffer::resize()" )
		;
	}

	std::string BufferStreamLazy_::description() const
	{
		return mDesc;
	}
	ftl::Any BufferStreamLazy_::native_handle() const
	{
		return ftl::Any( mStream );
	}
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
