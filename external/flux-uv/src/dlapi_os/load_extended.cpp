/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Extended loader
 *
 * Performs an extended search, and, optionally, goes beyond any reasonable
 * expectations, and downloads a pre-built binary from the interwebs.
 */
/*-***************************************************************** -}}}1- */

#include "load_extended.hxx"

#include <flux/std/log.hpp>
#include <flux/std/byte.hpp>
#include <flux/std/throw.hpp>
#include <flux/std/format.hpp>

#include <flux/compat/system_error.hpp>

#if FLUX_WITH_FLUXIO
#	include <flux/base/def/vfs.hpp>
#	include <flux/base/default_catalogs.hpp>

#	include <flux/io/vfs.hpp>
#	include <flux/io/vfs/native_path.hpp>
#	include <flux/io/stream/zstd.hpp>
#	include <flux/io/stream/special.hpp>
#	include <flux/io/stream/sysfile.hpp>
#	include <flux/io/errors.hpp>
#endif // ~ FLUX_WITH_FLUXIO

#if FLUXCFG_DLAPI_LOAD_AUTOFETCH
#	include <flux/io/stream/http.hpp>
#	endif // ~ LOAD_AUTOFETCH

#include <flux/sys/info.hpp>
#include <flux/sys/syspath.hpp>

#include <flux/ftl/vector.hpp>
#include <flux/util/narrow.hpp>
#include <flux/setup/platform.hpp>

#include <flux_ext/monocypher.hpp>
#include <flux_dlapi/os/defaults.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::dlapi::os::detail >>> ///{{{1///////////////////
FLUX_NAMESPACE_ENTER3(dlapi,os,detail)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
#	if FLUX_WITH_FLUXIO
	constexpr char const* kDmodCacheDir_ = "/@flux/app/appdata";
	constexpr char const* kDmodBindistDir_ = "/@flux/opt/system/bindist";

	constexpr uint8_t kDmodPubkeys_[][32] = {
		{ 0xe8, 0x3e, 0xec, 0x62, 0xca, 0x58, 0xd3, 0xd4, 0x6c, 0xbb, 0xd0,
		  0xed, 0x00, 0xd8, 0x6d, 0x07, 0x47, 0xbb, 0x97, 0x08, 0x47, 0xeb,
		  0x8d, 0xd7, 0x30, 0xdf, 0x77, 0x16, 0xed, 0xac, 0x09, 0xb1 },
	};
#	endif // ~ WITH_FLUXIO

#	if FLUXCFG_DLAPI_LOAD_AUTOFETCH
	constexpr unsigned kAutofetchTimeout_ = FLUXCFG_DLAPI_AUTOFETCH_TIMEOUT;
	constexpr char const* kAutofetchSrc_ = "http://stas.is/flux/uv201806";
#	endif // ~ LOAD_AUTOFETCH

#	if FLUX_WITH_FLUXIO
	optional<sys::DylibHandle> try_load_( io::VfsRecord const& ); 

	optional<sys::DylibHandle> dmod_process_(
		unique_ptr<io::Stream>,
		char const* aDmodBaseName,
		char const* aDmodVersionStr,
		char const* aSourcePath,
		char const* aMethodDesc
	);

	unique_ptr<io::Stream> dmod_v201806_process_(
		unique_ptr<io::Stream>,
		char const* aPlatformName,
		char const* aDmodName
	);
	void dmod_v201806_extract_(
		io::VfsRecord const&,
		unique_ptr<io::Stream>,
		char const* aPlatformDir,
		char const* aPlatformName,
		char const* aDmodName
	);

	std::string platform_name_(
		char const* aDmodBaseName,
		char const* aDmodVersionStr
	);
#	endif // ~ WITH_FLUXIO

#	if FLUX_WITH_FLUXIO
	namespace error_
	{
		struct DmodError_
			: flux::error::RuntimeError
		{};

		struct DmodUnknownPubkey
			: DmodError_
		{};
		struct DmodSignatureMismatch_
			: DmodError_
		{};
		struct DmodNotFound_
			: DmodError_
		{};
	}
#	endif // ~ WITH_FLUXIO
}

//--    dlapi_category()                    ///{{{2///////////////////////////
sys::DylibHandle extended_search( EExtendedOptions aOpts, char const* const* aSystemNames, char const* aExtendedName, char const* aDmodBaseName, char const* aDmodVersionStr )
{
	// First, try the specified system names.
	if( !!(EExtendedOptions::unspecified & aOpts) )
	{
		FLUX_ASSERT( aSystemNames );
		for( char const* const* item = aSystemNames; *item; ++item )
		{
			if( auto const handle = sys::dylib_open( *item ) )
			{
				FLUX_LOG( INFO, "dlapi: extended search: using '{}' (system default)", *item );
				return handle;
			}
		}
	}

	// Next, try the VFS paths
#	if FLUXCFG_DLAPI_LOAD_VFSSYSTEM
	if( !!(EExtendedOptions::vfsSystem & aOpts) )
	{
		FLUX_ASSERT( aExtendedName );
		auto const& root = base::process_catalog().acquire<base::def::ProcessVfs>();

		//XXX-FIXME-multiple or platform dependent names?
		//XXX-FIXME-multiple or platform dependent names?
		//XXX-FIXME-multiple or platform dependent names?
		auto const path = format( "/@flux/opt/system/{}/{}/{}", sys::get_os_ident(), sys::get_arch_ident(), aExtendedName );

		compat::error_code ec;
		if( auto node = io::vfs_open( root, path, ec ) )
		{
			if( auto handle = try_load_( node.value() ) )
			{
				FLUX_LOG( INFO, "dlapi: extended search: using '{}'", path );
				return handle.value();
			}
		}
	}

	if( !!(EExtendedOptions::vfsSystemPlain & aOpts) )
	{
		FLUX_ASSERT( aExtendedName );
		auto const& root = base::process_catalog().acquire<base::def::ProcessVfs>();
		auto const path = format( "/@flux/opt/system/{}", aExtendedName );

		compat::error_code ec;
		if( auto node = io::vfs_open( root, path, ec ) )
		{
			if( auto handle = try_load_( node.value() ) )
			{
				FLUX_LOG( INFO, "dlapi: extended search: using '{}'", path );
				return handle.value();
			}
		}
	}
#	else // !LOAD_VFSSYSTEM
	(void)aExtendedName;
#	endif // ~ LOAD_VFSSYSTEM

#	if FLUX_WITH_FLUXIO
	if( !!(EExtendedOptions::cached & aOpts) )
	{
		auto const& root = base::process_catalog().acquire<base::def::ProcessVfs>();

		auto const os = sys::get_os_ident();
		auto const arch = sys::get_arch_ident();
		auto const name = platform_name_( aDmodBaseName, aDmodVersionStr );
		auto const path = format( "{}/{}/{}/{}", kDmodCacheDir_, os, arch, name );

		compat::error_code ec;
		if( auto node = io::vfs_open( root, path, ec ) )
		{
			if( auto handle = try_load_( node.value() ) )
			{
				FLUX_LOG( INFO, "dlapi: extended search: using '{}' (cached)", path );
				return handle.value();
			}
		}
	}

	if( !!(EExtendedOptions::bindist & aOpts) )
	{
		FLUX_ASSERT( aDmodBaseName && aDmodVersionStr );

		if( !(EExtendedOptions::cached & aOpts) )
			FLUX_LOG( WARNING, "dlapi: bindist: {}-{}: 'cached' search option is not set, but should be.", aDmodBaseName, aDmodVersionStr );
		
		auto const os = sys::get_os_ident();
		auto const arch = sys::get_arch_ident();
		auto const dmod = flux::format( "{}/{}-{}.dmod_{}-{}", kDmodBindistDir_, aDmodBaseName, aDmodVersionStr, os, arch );

		try
		{
			auto const& root = base::process_catalog().acquire<base::def::ProcessVfs>();
			auto stream = vfs_open_stream( root, dmod );

			if( stream )
			{
				auto handle = dmod_process_( 
					std::move(stream), 
					aDmodBaseName, 
					aDmodVersionStr,
					dmod.c_str(),
					"bindist"
				);

				if( handle )
					return handle.value();
			}
		}
		catch( io::error::VfsNotFound const& )
		{
			FLUX_LOG( INFO, "dlapi: bindist: {} not found", dmod );
		}
		catch( io::error::VfsError const& eError )
		{
			FLUX_LOG( ERROR, "dlapi: bindist: {}: error\n{}", dmod, eError.what() );
		}
		catch( io::error::StreamError const& eErr )
		{
			FLUX_LOG( ERROR, "dlapi: bindist: {}: stream:\n{}", dmod, eErr.what() );
		}
		catch( flux::error::RuntimeError const& eErr )
		{
			FLUX_LOG( ERROR, "dlapi: bindist: {}: error:\n{}", dmod, eErr.what() );
		}
		catch( std::exception const& eErr )
		{
			FLUX_LOG( ERROR, "dlapi: bindist: {}: exception:\n{}", dmod, eErr.what() );
		}
		catch( ... )
		{
			FLUX_LOG( ERROR, "dlapi: bindist: {}: unknown exception", dmod );
		}
	}
#	endif // ~ WITH_FLUXIO

	// Finally, try to autofetch the requested object.
#	if FLUXCFG_DLAPI_LOAD_AUTOFETCH
	if( !!(EExtendedOptions::autofetch & aOpts) )
	{
		FLUX_ASSERT( aDmodBaseName && aDmodVersionStr );

		if( !(EExtendedOptions::cached & aOpts) )
			FLUX_LOG( WARNING, "dlapi: autofetch: {}-{}: 'cached' search option is not set, but should be.", aDmodBaseName, aDmodVersionStr );

		return [&] () -> sys::DylibHandle {
			auto const os = sys::get_os_ident();
			auto const arch = sys::get_arch_ident();
			auto const url = format( "{}/{}/{}/{}-{}.dmod", kAutofetchSrc_, os, arch, aDmodBaseName, aDmodVersionStr );

			try
			{
				// Download and verify+store
				auto get = io::stream_create_httpget( url, kAutofetchTimeout_ );
				FLUX_ASSERT( get );
				FLUX_LOG( INFO, "dlapi: fetching \"{}\" ({})", url, get->caps().size ? format( "{:d}kB", get->size()/1024 ) : "unknown size" );

				auto handle = dmod_process_( 
					std::move(get), 
					aDmodBaseName, 
					aDmodVersionStr,
					url.c_str(),
					"autofetch"
				);

				if( handle )
					return handle.value();
			}
			catch( io::error::StreamExpired const& )
			{
				FLUX_LOG( INFO, "dlapi: autofetch: {} timed out", url );
			}
			catch( io::error::StreamNotAvailable const& )
			{
				FLUX_LOG( INFO, "dlapi: autofetch: {} returned 404", url );
			}
			catch( io::error::StreamError const& eErr )
			{
				FLUX_LOG( ERROR, "dlapi: autofetch: {}: stream:\n{}", url, eErr.what() );
			}
			catch( flux::error::RuntimeError const& eErr )
			{
				FLUX_LOG( ERROR, "dlapi: autofetch: {}: error:\n{}", url, eErr.what() );
			}
			catch( std::exception const& eErr )
			{
				FLUX_LOG( ERROR, "dlapi: autofetch: {}: exception:\n{}", url, eErr.what() );
			}
			catch( ... )
			{
				FLUX_LOG( ERROR, "dlapi: autofetch: {}: unknown exception", url );
			}

			return nullptr;
		}();
	}
#	endif // ~ LOAD_AUTOFETCH

#	if !FLUX_WITH_FLUXIO
	(void)aDmodBaseName;
	(void)aDmodVersionStr;
#	endif // ~ WITH_FLUXIO
	
	return nullptr;
}

//--    $ try_load_()                       ///{{{2///////////////////////////
namespace
{
#	if FLUX_WITH_FLUXIO
	optional<sys::DylibHandle> try_load_( io::VfsRecord const& aRecord )
	{
		if( io::vfs_stat( aRecord ).hasNativePath )
		{
			compat::error_code ec;
			auto np = io::vfs::native_path( aRecord, ec );
			if( !np )
			{
				FLUX_LOG( WARNING, "VINode '{}' advertises native path, but doesn't return one.", io::vfs_description(aRecord) );
			}
			else
			{
				if( auto const handle = sys::dylib_open( np.value() ) )
					return handle;
			}
		}

		return {};
	}
#	endif // ~ WITH_FLUXIO
}

//--    $ dmod_process_()                   ///{{{2///////////////////////////
namespace
{
#	if FLUX_WITH_FLUXIO
	optional<sys::DylibHandle> dmod_process_( unique_ptr<io::Stream> aStream, char const* aDmodBaseName, char const* aDmodVersionStr, char const* aSourcePath, char const* aMethodDesc )
	{
		char filesig[8];
		aStream->read( filesig, 8, io::EIoRequest::mustComplete );

		if( 0 != std::memcmp( filesig, "fluxDMOD", 8 ) )
		{
			FLUX_LOG( ERROR, "dlapi: {}: invalid file signature", aSourcePath );
			return {};
		}

		uint32_t version;
		aStream->read( &version, sizeof(uint32_t), io::EIoRequest::mustComplete );

		switch( version )
		{
			case 201806: {
				auto const name = platform_name_( aDmodBaseName, aDmodVersionStr );

				auto local = dmod_v201806_process_( 
					std::move(aStream), 
					name.c_str(),
					aSourcePath
				);

				auto const& root = base::process_catalog().acquire<base::def::ProcessVfs>();

				auto const os = sys::get_os_ident();
				auto const arch = sys::get_arch_ident();
				auto const base = format( "{}/{}/{}", kDmodCacheDir_, os, arch );
				auto const path = format( "{}/{}", base, name );

				dmod_v201806_extract_( 
					root, 
					std::move(local), 
					base.c_str(), 
					name.c_str(),
					aSourcePath
				);
				
				auto node = io::vfs_open( root, path );
				if( auto handle = try_load_( node ) )
				{
					FLUX_LOG( INFO, "dlapi: extended search: using '{}' ({})", path, aMethodDesc );
					return handle;
				}
			} break;

			default: {
				FLUX_LOG( ERROR, "dlapi: {}: unknown version {:d}", aSourcePath, version );
				return {};
			};
		}

		return {};
	}
#	endif // ~ WITH_FLUXIO
}

//--    $ *_dmod_v201806_()                 ///{{{2///////////////////////////
namespace
{
#	if FLUX_WITH_FLUXIO
	template< typename tType > inline
	uint8_t const* u8ptr_( tType const* aPtr )
	{
		return reinterpret_cast<uint8_t const*>(aPtr);
	}

	unique_ptr<io::Stream> dmod_v201806_process_( unique_ptr<io::Stream> aStream, char const* aPlatformName, char const* aDmodName )
	{
		FLUX_ASSERT( aStream && aDmodName );
		
		// Read public key version
		uint32_t keyID = 0;
		aStream->read( &keyID, sizeof(uint32_t), io::EIoRequest::mustComplete );

		if( keyID >= sizeof(kDmodPubkeys_)/sizeof(kDmodPubkeys_[0]) )
		{
			FLUX_THROW( error_::DmodUnknownPubkey )
				<< einfo::ErrorDesc( FLUX_FMT_STRING("Unknown public key ID {}"), keyID )
				<< einfo::ErrorMeta( FLUX_FMT_STRING("dmod = {}"), aDmodName )
			;
		}

		// Read signature and set up verification
		uint8_t sig[64];
		aStream->read( sig, 64, io::EIoRequest::mustComplete );

		crypto_check_ctx cctx;
		auto actx = reinterpret_cast<crypto_check_ctx_abstract*>(&cctx);

		crypto_check_init(
			actx,
			sig,
			kDmodPubkeys_[keyID]
		);

		// Tempfile for uncompressed (but packed) data
		auto temp = io::stream_create_systemp();
		FLUX_ASSERT( temp );

		// Read and verify payload
		auto cis = io::stream_create_zstd_read( aStream.get() );
		[&cis,actx,&temp,aPlatformName,aDmodName] {
			uint16_t count;
			cis->read( &count, sizeof(uint16_t), io::EIoRequest::mustComplete );
			temp->write( &count, sizeof(uint16_t), io::EIoRequest::mustComplete );
			crypto_check_update( actx, u8ptr_(&count), sizeof(uint16_t) );

			bool found = false;
			auto const modNameLength = std::strlen( aPlatformName );
			ftl::VectorBase<flux::byte> iobuf( 128*1024 );
			for( std::size_t i = 0; i < std::size_t(count); ++i )
			{
				uint16_t nlen;
				cis->read( &nlen, sizeof(uint16_t), io::EIoRequest::mustComplete );
				temp->write( &nlen, sizeof(uint16_t), io::EIoRequest::mustComplete );
				crypto_check_update( actx, u8ptr_(&nlen), sizeof(uint16_t) );

				ftl::Vector<char> name(nlen+1, 0);
				cis->read( name.data(), nlen, io::EIoRequest::mustComplete );
				temp->write( name.data(), nlen, io::EIoRequest::mustComplete );
				crypto_check_update( actx, u8ptr_(name.data()), nlen );

				if( modNameLength == std::size_t(nlen) && 0 == std::memcmp( aPlatformName, name.data(), modNameLength ) )
					found = true;

				uint64_t clen;
				cis->read( &clen, sizeof(uint64_t), io::EIoRequest::mustComplete );
				temp->write( &clen, sizeof(uint64_t), io::EIoRequest::mustComplete );
				crypto_check_update( actx, u8ptr_(&clen), sizeof(uint64_t) );

				while( clen )
				{
					auto req = std::min( iobuf.size(), util::narrow<std::size_t>(clen) );
					cis->read( iobuf.data(), req, io::EIoRequest::mustComplete );
					temp->write( iobuf.data(), req, io::EIoRequest::mustComplete );

					crypto_check_update( actx, u8ptr_(iobuf.data()), req );
					clen -= req;
				}
			}

			FLUX_THROW_IF( 0 != crypto_check_final( actx ), error_::DmodSignatureMismatch_ )
				<< einfo::ErrorDesc( ".dmod failed signature check" )
				<< einfo::ErrorMeta( FLUX_FMT_STRING("dmod = {}"), aDmodName )
			;
			FLUX_THROW_IF( !found, error_::DmodNotFound_ )
				<< einfo::ErrorDesc( FLUX_FMT_STRING("no component '{}'"), aPlatformName )
				<< einfo::ErrorMeta( FLUX_FMT_STRING("dmod = {}"), aDmodName )
			;
		}();

		cis.reset();

		// rewind temporary storage and return
		temp->seek( 0 );
		return temp;
	}

	void dmod_v201806_extract_( io::VfsRecord const& aRoot, unique_ptr<io::Stream> aStream, char const* aPlatformDir, char const* aPlatformName, char const* aDmodName )
	{
		FLUX_ASSERT( aStream && aPlatformDir && aPlatformName && aDmodName );
		auto const modNameLength = std::strlen( aPlatformName );

		// esnure the destination directory exists
		io::vfs_mkdir( aRoot, aPlatformDir, true );

		// re-read the data and extract the components to their destination
		uint16_t count;
		aStream->read( &count, sizeof(uint16_t), io::EIoRequest::mustComplete );

		ftl::VectorBase<flux::byte> iobuf( 128*1024 );
		for( std::size_t i = 0; i < std::size_t(count); ++i )
		{
			uint16_t nlen;
			aStream->read( &nlen, sizeof(uint16_t), io::EIoRequest::mustComplete );

			ftl::Vector<char> name(nlen+1, 0);
			aStream->read( name.data(), nlen, io::EIoRequest::mustComplete );

			auto stream = [&] {
				if( modNameLength == std::size_t(nlen) && 0 == std::memcmp( aPlatformName, name.data(), modNameLength ) )
				{
					auto full = format( "{}/{}", aPlatformDir, aPlatformName );
					return io::vfs_open_stream( aRoot, full, io::EStreamMode::write );
				}
				else
				{
					if( 0 == nlen || std::memchr( name.data(), '\0', nlen ) != nullptr )
					{
						FLUX_LOG( WARNING, "dmod: {}: component {:d}: illegal name", aDmodName, i );
						return io::stream_create_null();
					}
					if( compat::string_view::npos != compat::string_view(name.data(),nlen).find_first_of( "/\\:" ) )
					{
						// Note: not a comprehensive check. For now, let's just avoid
						// subdirectories, and alternate data streams. (Other names
						// should fail from the OS.)
						FLUX_LOG( WARNING, "dmod: {}: component {:d}: illegal name ({})", aDmodName, i, name.data() );
						return io::stream_create_null();
					}

					auto full = format( "{}/{}-{}", aPlatformDir, aPlatformName, name.data() );
					return io::vfs_open_stream( aRoot, full, io::EStreamMode::write );
				}
			}();

			uint64_t clen;
			aStream->read( &clen, sizeof(uint64_t), io::EIoRequest::mustComplete );

			while( clen )
			{
				auto req = std::min( iobuf.size(), util::narrow<std::size_t>(clen) );
				aStream->read( iobuf.data(), req, io::EIoRequest::mustComplete );
				stream->write( iobuf.data(), req, io::EIoRequest::mustComplete );
				clen -= req;
			}
		}
	}
#	endif // ~ WITH_FLUXIO
}

//--    $ platform_name_()                  ///{{{2///////////////////////////
namespace
{
#	if FLUXCFG_DLAPI_LOAD_AUTOFETCH
	std::string platform_name_( char const* aDmodBaseName, char const* aDmodVersionStr )
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		return format( "{}.so.{}", aDmodBaseName, aDmodVersionStr );
#		else // PLATFORM_WIN32
		return format( "{}-{}.dll", aDmodBaseName, aDmodVersionStr );
#		endif // ~ PLATFORM
	}
#	endif // ~ LOAD_AUTOFETCH
}

//--    <<< ~ flux::dlapi::os::detail namespace <<< ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE3(dlapi,os,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
