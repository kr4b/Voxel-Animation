/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Read-only memory-backed Buffer implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/buffer/memread.hpp>
#include <flux/io/errors.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/stdexcept.hpp>

#include <flux/util/narrow.hpp>
#include <flux/util/scope_exit.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	class BufferMemread_ final : public Buffer
	{
		public:	
			BufferMemread_( void const*, BufferSize, unique_function<void(void const*)> );
			~BufferMemread_();

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
			void const* mBuffer;
			BufferSize mBufferSize;

			unique_function<void(void const*)> mDeleter;
	};
}

//--    buffer_create_memread()             ///{{{2///////////////////////////
unique_ptr<Buffer> buffer_create_memread( ftl::not_null<void const*> aBuffer, BufferSize aBufferSize, bool aMakeCopy )
{
	if( aMakeCopy )
	{
		void* ptr = ::operator new( aBufferSize );
		std::memcpy( ptr, aBuffer, aBufferSize );

		return make_unique<BufferMemread_>(
			ptr, aBufferSize, [] (void const* aPtr) { 
				::operator delete(const_cast<void*>(aPtr));
			}
		);
	}
	else
	{
		return make_unique<BufferMemread_>(
			aBuffer, aBufferSize, [] (void const*) {}
		);
	}
}
unique_ptr<Buffer> buffer_create_memread_dealloc( ftl::not_null<void const*> aBuffer, BufferSize aBufferSize, unique_function<void(void const*)> aDeleter )
{
	return make_unique<BufferMemread_>(
		aBuffer, aBufferSize, std::move(aDeleter)
	);
}

//--    $ BufferMemread_                    ///{{{2///////////////////////////
namespace
{
	BufferMemread_::BufferMemread_( void const* aBuffer, BufferSize aBufferSize, unique_function<void (void const*)> aDeleter )
		: mBuffer( aBuffer )
		, mBufferSize( aBufferSize )
		, mDeleter( std::move(aDeleter) )
	{}

	BufferMemread_::~BufferMemread_()
	{
		mDeleter( mBuffer );
	}

	compat::span<compat::byte> BufferMemread_::map( EBufferMapRW, BufferSize, BufferSize )
	{
		FLUX_THROW( io::error::BufferOpNotSupported )
			<< einfo::BufferMode( EBufferMode::read )
			<< einfo::BufferDesc( description() )
			<< flux::einfo::ErrorMeta( "Buffer::map(EBufferMapRW)" )
		;
	}
	compat::span<compat::byte const> BufferMemread_::map( EBufferMapRO, BufferSize aBegin, BufferSize aLength )
	{
		FLUX_THROW_IF( aBegin >= mBufferSize, io::error::BufferOutOfRange )
			<< einfo::BufferMode( EBufferMode::read )
			<< einfo::BufferDesc( description() )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("size {:d}, requested offset {:d}"), mBufferSize, aBegin )
		;

		if( ~BufferSize(0) == aLength )
			aLength = mBufferSize-aBegin;
		
		FLUX_THROW_IF( aLength > mBufferSize-aBegin, io::error::BufferOutOfRange )
			<< einfo::BufferMode( EBufferMode::read )
			<< einfo::BufferDesc( description() )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("size {:d}, requested range {:d}+{:d}"), mBufferSize, aBegin, aLength )
		;

		auto* ptr = static_cast<compat::byte const*>(mBuffer);
		return compat::make_span( ptr+aBegin, ptr+aBegin+aLength );
	}

	void BufferMemread_::unmap( compat::span<compat::byte const> const& )
	{}

	BufferCaps BufferMemread_::caps() const noexcept
	{
		BufferCaps caps;
		caps.minOffsetAlignment = 1;
		
		caps.readable = true;
		caps.writable = false;

		caps.partialMap = true;
		caps.partialUnmap = true;
		caps.multiMap = true;

		caps.resizable = false;
		return caps;
	}
	BufferSize BufferMemread_::size() const
	{
		return mBufferSize;
	}

	void BufferMemread_::flush( compat::span<compat::byte const> const& )
	{}
	void BufferMemread_::resize( BufferSize, bool )
	{
		FLUX_THROW( io::error::BufferOpNotSupported )
			<< einfo::BufferMode( EBufferMode::read )
			<< einfo::BufferDesc( description() )
			<< flux::einfo::ErrorMeta( "Buffer::resize()" )
		;
	}

	std::string BufferMemread_::description() const
	{
		return format( "memread@{:p}+{:d}", mBuffer, mBufferSize );
	}
	ftl::Any BufferMemread_::native_handle() const
	{
		return ftl::Any( mBuffer );
	}
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
