/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Memory-backed read-only Stream implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/stream/memread.hpp>
#include <flux/io/errors.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>

#include <flux/compat/byte.hpp>
#include <flux/util/narrow.hpp>

#include <algorithm>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	class StreamMemread_ final : public Stream
	{
		public:	
			StreamMemread_(
				void const*, 
				StreamSize,
				function<void(void const*)>
			);
			StreamMemread_(
				shared_ptr<void const>,
				StreamSize
			);

			~StreamMemread_();

		public: /* virtuals from Stream */
			StreamSize read( void*, StreamSize, EIoRequest ) override;
			StreamSize write( void const*, StreamSize, EIoRequest ) override;

			EStreamStatus status() const noexcept override;

			StreamCaps caps() const noexcept override;
			StreamSize size() const override;
			StreamSize tell() const override;
			StreamSize seek( StreamOffset, ESeekWhence ) override;
			StreamSize remaining() const override;

			void flush() override;

			std::string description() const override;
			ftl::Any native_handle() const override;

			unique_ptr<Stream> clone() const override;

		private:

		private:
			shared_ptr<void const> mBuffer;
			StreamSize mBufferSize;
			StreamSize mBufferPos;

			//unique_function<void(void const*)> mDeleter;
	};
}


//--    stream_create_memread()               ///{{{2///////////////////////////
unique_ptr<Stream> stream_create_memread( ftl::not_null<void const*> aBuffer, StreamSize aSize, bool aMakeCopy )
{
	if( aMakeCopy )
	{
		void* ptr = ::operator new( aSize );
		std::memcpy( ptr, aBuffer, aSize );

		return make_unique<StreamMemread_>(
			ptr, aSize, [] (void const* aPtr) { 
				::operator delete(const_cast<void*>(aPtr));
			}
		);
	}
	else
	{
		return make_unique<StreamMemread_>(
			aBuffer, aSize, [] (void const*) {}
		);
	}
}
unique_ptr<Stream> stream_create_memread_dealloc( ftl::not_null<void const*> aBuffer, StreamSize aSize, function<void(void const*)> aDeleter )
{
	return make_unique<StreamMemread_>(
		aBuffer, aSize, std::move(aDeleter)
	);
}

//--    $ StreamMemread_                      ///{{{2///////////////////////////
namespace
{
	StreamMemread_::StreamMemread_( void const* aBuffer, StreamSize aSize, function<void (void const*)> aDeleter )
		: mBuffer( aBuffer, std::move(aDeleter) )
		, mBufferSize( aSize )
		, mBufferPos( 0 )
	{}
	StreamMemread_::StreamMemread_( shared_ptr<void const> aBuffer, StreamSize aSize )
		: mBuffer( std::move(aBuffer) )
		, mBufferSize( aSize )
		, mBufferPos( 0 )
	{}

	StreamMemread_::~StreamMemread_()
	{}


	StreamSize StreamMemread_::read( void* aBuffer, StreamSize aSize, EIoRequest aReq )
	{
		FLUX_ASSERT( mBuffer && aBuffer );
		FLUX_ASSERT( mBufferSize >= mBufferPos );

		auto const size = std::min( util::narrow<std::size_t>(aSize), mBufferSize-mBufferPos );
		if( EIoRequest::mustComplete == aReq )
		{
			FLUX_THROW_IF( aSize != size, error::StreamIoIncomplete )
				<< einfo::StreamMode( EStreamMode::read )
				<< einfo::StreamDesc( description() )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("requested: {:d}, have: {:d}"), aSize, size )
			;
		}

		if( 0 == size )
			return 0;

		// copy data
		compat::byte const* buff = static_cast<compat::byte const*>(mBuffer.get());
		std::memcpy( aBuffer, buff+mBufferPos, size );
		mBufferPos += size;

		FLUX_ASSERT( mBufferSize >= mBufferPos );
		return size;
	}
	StreamSize StreamMemread_::write( void const*, StreamSize, EIoRequest )
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::write()" )
		;
	}

	EStreamStatus StreamMemread_::status() const noexcept
	{
		if( mBufferPos < mBufferSize )
			return EStreamStatus::readable;

		return EStreamStatus();
	}

	StreamCaps StreamMemread_::caps() const noexcept
	{
		StreamCaps caps;
		caps.readable = true;
		caps.writable = false;
		caps.size = true;
		caps.tell = true;
		caps.seekFwd = true;
		caps.seekRev = true;
		caps.remaining = true;
		caps.infinite = false;
		caps.clonable = true;
		return caps;
	}

	StreamSize StreamMemread_::size() const
	{
		return mBufferSize;
	}

	StreamSize StreamMemread_::tell() const
	{
		return mBufferPos;
	}
	StreamSize StreamMemread_::seek( StreamOffset aOffs, ESeekWhence aWhence )
	{
		std::size_t newPos = 0;
		std::size_t absOff = util::narrow<std::size_t>(std::abs(aOffs));

		switch( aWhence )
		{
			case ESeekWhence::set: {
				FLUX_THROW_IF( aOffs < 0, error::StreamInvalidSeek )
					<< einfo::StreamMode( EStreamMode::read )
					<< einfo::StreamDesc( description() )
					<< flux::einfo::ErrorDesc( "Stream::seek(set) with negative offset" )
				;
				newPos = absOff;
				break;
			}
			case ESeekWhence::cur: {
				if( aOffs > 0 )
				{
					FLUX_THROW_IF( mBufferSize-mBufferPos < absOff, error::StreamInvalidSeek )
						<< einfo::StreamMode( EStreamMode::read )
						<< einfo::StreamDesc( description() )
						<< flux::einfo::ErrorDesc( "Stream::seek(cur) offset out of bounds" )
					;

					newPos = mBufferPos + absOff;
				}
				else
				{
					FLUX_THROW_IF( absOff > mBufferPos, error::StreamInvalidSeek )
						<< einfo::StreamMode( EStreamMode::read )
						<< einfo::StreamDesc( description() )
						<< flux::einfo::ErrorDesc( "Stream::seek(cur) offset out of bounds" )
					;

					newPos = mBufferPos - absOff;
				}
				break;
			}
			case ESeekWhence::end: {
				FLUX_THROW_IF( aOffs < 0, error::StreamInvalidSeek )
					<< einfo::StreamMode( EStreamMode::read )
					<< einfo::StreamDesc( description() )
					<< flux::einfo::ErrorDesc( "Stream::seek(end) with positive offset" )
				;
				FLUX_THROW_IF( absOff > mBufferSize, error::StreamInvalidSeek )
					<< einfo::StreamMode( EStreamMode::read )
					<< einfo::StreamDesc( description() )
					<< flux::einfo::ErrorDesc( "Stream::seek(cur) offset out of bounds" )
				;

				newPos = mBufferSize - absOff;
				break;
			}
		}

		FLUX_ASSERT( newPos <= mBufferSize );
		mBufferPos = newPos;
		return mBufferPos;
	}
	StreamSize StreamMemread_::remaining() const
	{
		return mBufferSize-mBufferPos;
	}
						
	void StreamMemread_::flush()
	{}

	std::string StreamMemread_::description() const
	{
		return format( "memread@{:p}+{:d}", mBuffer.get(), mBufferSize );
	}
	ftl::Any StreamMemread_::native_handle() const
	{
		return ftl::Any( mBuffer );
	}

	unique_ptr<Stream> StreamMemread_::clone() const
	{
		return make_unique<StreamMemread_>( mBuffer, mBufferSize );
	}
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
