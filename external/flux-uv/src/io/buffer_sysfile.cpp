/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Sysfile Buffer implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/buffer/sysfile.hpp>
#include <flux/io/errors.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/stdexcept.hpp>

#include <flux/sys/errors.hpp>
#include <flux/sys/filemap.hpp>
#include <flux/sys/pathutil.hpp>

#include <flux/util/intent.hpp>
#include <flux/util/narrow.hpp>
#include <flux/util/scope_exit.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	class BufferSysFile_ final : public Buffer
	{
		public:	
			BufferSysFile_( sys::FileMapInfo const&, EBufferMode, std::string ) noexcept;
			~BufferSysFile_();

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
			void check_mode_( EBufferMap ) const;

		private:
			sys::FileMapInfo mMapInfo;
			EBufferMode mMapMode;
			std::string mDesc;
	};

#	if FLUX_COMPILER == FLUX_COMPILER_GCC && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ <= 9))
#		define FLUXTMP_SUPERFLUOUS_MOVE_(x) std::move(x)
#	else
#		define FLUXTMP_SUPERFLUOUS_MOVE_(x) x
#	endif 
}

//--    buffer_create_sysfile()             ///{{{2///////////////////////////
unique_ptr<Buffer> buffer_create_sysfile( sys::SysPathView const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo ) try
{
	sys::EFileMapMode mmode;
	if( EBufferMode::read == aMode) mmode = sys::EFileMapMode::ro;
	else if( EBufferMode::write == aMode ) mmode = sys::EFileMapMode::wo;
	else if( (EBufferMode::read|EBufferMode::write) == aMode )
		mmode = sys::EFileMapMode::rw;
	else FLUX_ASSERT( false );
	
	// Sanity check..
	if( ~BufferSize(0) != aResizeTo )
	{
		FLUX_THROW_IF( EBufferMode::read == aMode, io::error::BufferOpNotSupported )
			<< einfo::BufferMode( aMode )
			<< einfo::BufferDesc( FLUX_FMT_STRING("sysfile:{}"), sys::path_to_utf8(aPath) )
			<< flux::einfo::ErrorDesc( "Cannot resize read-only buffer" )
		;
	}
	
	auto fmi = sys::file_map_prepare( aPath, mmode, aMustExist );
	FLUX_UTIL_ON_SCOPE_EXIT{ sys::file_map_destroy( fmi ); };

	if( ~BufferSize(0) != aResizeTo )
		sys::file_map_resize( fmi, util::narrow<std::uintmax_t>(aResizeTo) );

	unique_ptr<Buffer> ret = make_unique<BufferSysFile_>( 
		fmi,
		aMode, 
		format( "sysfile:{}", sys::path_to_utf8(aPath).c_str() )
	);

	fmi = sys::FileMapInfo();
	return FLUXTMP_SUPERFLUOUS_MOVE_(ret);
}
catch( flux::error::RuntimeError const& )
{
	FLUX_THROW_WITH_NESTED( io::error::BufferNotAvailable )
		<< einfo::BufferMode( aMode )
		<< einfo::BufferDesc( FLUX_FMT_STRING("sysfile:{}"), sys::path_to_utf8(aPath) )
	;
}

unique_ptr<Buffer> buffer_create_sysfile( sys::SysPathStr const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	
	sys::EFileMapMode mmode;
	if( EBufferMode::read == aMode) mmode = sys::EFileMapMode::ro;
	else if( EBufferMode::write == aMode ) mmode = sys::EFileMapMode::wo;
	else if( (EBufferMode::read|EBufferMode::write) == aMode )
		mmode = sys::EFileMapMode::rw;
	else FLUX_ASSERT( false );
	
	// Sanity check..
	if( ~BufferSize(0) != aResizeTo )
	{
		aErrorOut = compat::make_error_code( compat::errc::argument_out_of_domain );
		return nullptr;
	}
	
	auto fmi = sys::file_map_prepare( aPath, mmode, aMustExist, aErrorOut );
	if( !fmi )
		return nullptr;
	
	FLUX_UTIL_ON_SCOPE_EXIT{ if(fmi) sys::file_map_destroy( fmi.value() ); };

	if( ~BufferSize(0) != aResizeTo )
	{
		auto x = util::narrow<std::uintmax_t>(aResizeTo, aErrorOut);
		if( !x )
			return nullptr;
		
		sys::file_map_resize( fmi.value(), x.value() );
	}

	try
	{
		unique_ptr<Buffer> ret = make_unique<BufferSysFile_>(
			fmi.value(),
			aMode,
			format( "sysfile:{}", sys::path_to_utf8(aPath).c_str() )
		);
		
		/* Note: format() potentially throws a TinyformatError, which is a
		 * LogicError.  This really shouldn't happen, so std::terminate()ing on
		 * those is fine.
		 */

		fmi.reset();
		return FLUXTMP_SUPERFLUOUS_MOVE_(ret);
	}
	catch( std::bad_alloc const& )
	{
		aErrorOut = compat::make_error_code( compat::errc::not_enough_memory );
		return nullptr;
	}
}

//--    $ BufferSysFile_                    ///{{{2///////////////////////////
namespace
{
	BufferSysFile_::BufferSysFile_( sys::FileMapInfo const& aMapInfo, EBufferMode aMode, std::string aDesc ) noexcept
		: mMapInfo( aMapInfo )
		, mMapMode( aMode )
		, mDesc( std::move(aDesc) )
	{}

	BufferSysFile_::~BufferSysFile_()
	{
		sys::file_map_destroy( mMapInfo );
	}

	compat::span<compat::byte> BufferSysFile_::map( EBufferMapRW aRW, BufferSize aBegin, BufferSize aLength ) try
	{
		check_mode_( aRW );

		sys::EFileMapMode mmode;
		switch( aRW )
		{
			case EBufferMapRW::wo: mmode = sys::EFileMapMode::wo; break;
			case EBufferMapRW::rw: mmode = sys::EFileMapMode::rw; break;
			default: FLUX_ASSERT( false );
		}

		auto const beg = util::narrow<std::uintmax_t>(aBegin);
		auto const len = (~BufferSize(0) != aLength)
			? util::narrow<std::uintmax_t>(aLength)
			: sys::file_map_view_max_size( mMapInfo, beg )
		;
		void* res = sys::file_map_view_create( mMapInfo, mmode, beg, len );
		FLUX_ASSERT( res );

		auto* ptr = static_cast<compat::byte*>(res);
		return compat::make_span( ptr, ptr + len );
	}
	catch( error::BufferError const& )
	{
		throw;
	}
	catch( sys::error::MapOutOfRange const& )
	{
		FLUX_THROW_WITH_NESTED( io::error::BufferOutOfRange )
			<< einfo::BufferMode( mMapMode )
			<< einfo::BufferDesc( mDesc )
		;
	}
	catch( flux::error::RuntimeError const& )
	{
		FLUX_THROW_WITH_NESTED( io::error::BufferError )
			<< einfo::BufferMode( mMapMode )
			<< einfo::BufferDesc( mDesc )
		;
	}
	compat::span<compat::byte const> BufferSysFile_::map( EBufferMapRO aRO, BufferSize aBegin, BufferSize aLength ) try
	{
		check_mode_( aRO );

		auto const beg = util::narrow<std::uintmax_t>(aBegin);
		auto const len = (~BufferSize(0) != aLength)
			? util::narrow<std::uintmax_t>(aLength)
			: sys::file_map_view_max_size( mMapInfo, beg )
		;
		void* res = sys::file_map_view_create( mMapInfo, sys::EFileMapMode::ro, beg, len );
		FLUX_ASSERT( res );

		auto* ptr = static_cast<compat::byte const*>(res);
		return compat::make_span( ptr, ptr + len );
	}
	catch( error::BufferError const& )
	{
		throw;
	}
	catch( sys::error::MapOutOfRange const& )
	{
		FLUX_THROW_WITH_NESTED( io::error::BufferOutOfRange )
			<< einfo::BufferMode( mMapMode )
			<< einfo::BufferDesc( mDesc )
		;
	}
	catch( flux::error::RuntimeError const& )
	{
		FLUX_THROW_WITH_NESTED( io::error::BufferError )
			<< einfo::BufferMode( mMapMode )
			<< einfo::BufferDesc( mDesc )
		;
	}


	void BufferSysFile_::unmap( compat::span<compat::byte const> const& aView ) try
	{
		auto ptr = const_cast<compat::byte*>(aView.data());
		sys::file_map_view_delete( mMapInfo, ptr, aView.size() );
	}
	catch( flux::error::RuntimeError const& )
	{
		FLUX_THROW_WITH_NESTED( io::error::BufferError )
			<< einfo::BufferMode( mMapMode )
			<< einfo::BufferDesc( mDesc )
		;
	}

	BufferCaps BufferSysFile_::caps() const noexcept
	{
		BufferCaps caps;
		caps.minOffsetAlignment = util::narrow<BufferSize>(sys::file_map_offset_minimum_alignment());
		
		caps.readable = !!(EBufferMode::read & mMapMode);
		caps.writable = !!(EBufferMode::write & mMapMode);

		caps.partialMap = true;
		caps.partialUnmap = sys::kFileMapSupportPartialUnmap;
		caps.multiMap = true; // XXX-XXX-XXX test

		caps.resizable = !!(EBufferMode::write & mMapMode);
		return caps;
	}
	BufferSize BufferSysFile_::size() const
	{
		// Hmm.. file_map_view_max_size() isn't a very good name for this.
		return util::narrow<BufferSize>(sys::file_map_view_max_size( mMapInfo ));
	}

	void BufferSysFile_::flush( compat::span<compat::byte const> const& aRange ) try
	{
		auto ptr = const_cast<compat::byte*>(aRange.data());
		sys::file_map_view_flush( mMapInfo, ptr, aRange.size() );
	}
	catch( flux::error::RuntimeError const& )
	{
		FLUX_THROW_WITH_NESTED( io::error::BufferError )
			<< einfo::BufferMode( mMapMode )
			<< einfo::BufferDesc( mDesc )
		;
	}

	void BufferSysFile_::resize( BufferSize aNewSize, bool ) try
	{
		check_mode_( EBufferMap::wo );
		sys::file_map_resize( mMapInfo, util::narrow<std::uintmax_t>(aNewSize) );
	}
	catch( error::BufferError const& )
	{
		throw;
	}
	catch( flux::error::RuntimeError const& )
	{
		FLUX_THROW_WITH_NESTED( io::error::BufferError )
			<< einfo::BufferMode( mMapMode )
			<< einfo::BufferDesc( mDesc )
		;
	}

	std::string BufferSysFile_::description() const
	{
		return mDesc;
	}
	ftl::Any BufferSysFile_::native_handle() const
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		return ftl::Any( mMapInfo.fd );
#		else // PLATFORM_WIN32
		return ftl::Any( mMapInfo.file );
#		endif // ~ PLATFORM
	}


	void BufferSysFile_::check_mode_( EBufferMap aMap ) const
	{
		bool const rreq = (EBufferMap::ro == aMap || EBufferMap::rw == aMap);
		FLUX_THROW_IF( rreq && !(EBufferMode::read & mMapMode), io::error::BufferOpNotSupported )
			<< einfo::BufferMode( mMapMode )
			<< einfo::BufferDesc( mDesc )
			<< flux::einfo::ErrorDesc( "Buffer does not support read access" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("desired access {}"), to_string(aMap) );
		;

		bool const wreq = (EBufferMap::wo == aMap || EBufferMap::rw == aMap);
		FLUX_THROW_IF( wreq && !(EBufferMode::write & mMapMode), io::error::BufferOpNotSupported )
			<< einfo::BufferMode( mMapMode )
			<< einfo::BufferDesc( mDesc )
			<< flux::einfo::ErrorDesc( "Buffer does not support write access" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("desired access {}"), to_string(aMap) );
		;
	}
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
