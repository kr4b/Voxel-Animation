/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Create default entry-point VfsRecord
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfs/default_entry_record.hpp>

#include <flux/std/log.hpp>
#include <flux/std/throw.hpp>
#include <flux/std/appinfo.hpp>

#include <flux/sys/syspath.hpp>
#include <flux/sys/defpath.hpp>
#include <flux/sys/pathutil.hpp>

#include <flux/io/errors.hpp>
#include <flux/io/vfsi/pdir.hpp>
#include <flux/io/vfsi/vdir.hpp>
#include <flux/io/vfsi/union.hpp>
#include <flux/io/vfsi/sysdir.hpp>
#include <flux/io/vfsi/zipdir.hpp>
#include <flux/io/vfsi/sysfile.hpp>
#include <flux/io/vfsi/sysroot.hpp>
#include <flux/io/vfsi/overlay.hpp>
#include <flux/io/vfsi/userdir.hpp>
#include <flux/io/vfsi/lazydir.hpp>
#include <flux/io/vfsi/masquerade.hpp>

#include <memory>
#include <sstream>
#include <cstdlib>

#include "vfs_record_factory.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io::vfs >>>           ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,vfs)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	std::string sys_to_vfs_( sys::SysPathView const& aView )
	{
		auto gen = sys::path_to_generic( aView );
		char const* beg = gen.data();
		char const* end = gen.data()+aView.size();

		while( end != beg && '/' == *beg )
			++beg;

		return std::string(beg,end);
	}

	void try_add_vdir_(
		shared_ptr<vfsi::UnionDir> const&,
		VfsRecord const&, 
		compat::string_view,
		char const*,
		char const* 
	);

	unsigned find_parent_( VfsRecord const&, compat::string_view, VfsRecord& );
}

//--    default_entry_record()              ///{{{2///////////////////////////
VfsRecord default_entry_record( EDefaultEntryOptions aOptions )
{
	//TODO: this needs quite a bit of fixing up eventually
	//
	//   - CWD should be a magic directory that floats with the current CWD
	//      - But doesn't rely on the relative "." path that we use for now.
	//   

	// TODO:
	//   
	//	- Add two customization points into /@flux
	//     - /@flux/opt/
	//     - /@flux/internal/
	//     And add API to create new entries inside of the customization points.
	//     The intention is *not* for them to be fully free-form, but more like
	//			
	//			- vfs_default_entry_opt_datadir_default()
	//			- vfs_default_entry_opt_datadir_set( VfsRecord )
	//       This would e.g., create the /@flux/opt/data directory. The default one
	//       performs the search, setting manually sets it to the specified dir.
	//
	//    ISSUE: /@flux/opt/data vs /@flux/opt/rodata? Maybe /@flux/opt/rwdata?
	//       Should these stack/shadow each other?
	//      RESOLVED: /@flux/opt/assets.
	//
	//       Other possible points to add:
	//				/@flux/opt/output
	//
	//
	//   - /@flux/internal: vfs_default_entry_internal_add( VfsRecord, string_view name )? + masquerade
	//       Allow flux-internal modules to add new entries. For example: the profiler
	//       could add /@flux/internal/profiler_output as the profiler's output
	//       directory.
	//
	//   - TODO: maybe a third point for user/app stuff? Or should we just make it 
	//       possible to add items to the base overlay. Probably that.
	//
	//       -- TODO: go over the below. Not sure if useful defaults are possible.
	//
	//  OLD notes:
	//   - /@flux/rodata   try to locate a usable datadir; FLUXENV_IO_RODATADIR
	//        - Check self-path (native)
	//           - if self-path ends on ${somepath}/bin/${name}.exe
	//               - *nix* try ${somepath}/share/${PROCESSEDNAME}/
	//               - *nix* try ${somepath}/lib/${PROCESSEDNAME}/
	//               - *win32* try ${somepath}/data
	//
	//   - /@flux/rwdata   try to locate a usable datadir; FLUXENV_IO_RWDATADIR
	//        - *nix* try CWD/data
	//        - *nix* try /var/lib/${PROCESSEDNAME}
	//
	//   - /@flux/data == {/@flux/rwdata over /@flux/rodata}
	//   - /@flux/output
	//
	// TODO: maybe provide a bitmask with selection on what paths should be included...
	//		E.G. disable the <portal> stuff, disable @flux completely ...
	// 

	// Create the root node
	// The root node is a SysRoot with a OverlayDir
	auto const sysroot = vfsi::inode_create_sysroot();
	auto root = make_shared<vfsi::OverlayDir>( sysroot );

	// Create the default "/@flux" node
	// By default, the root-OverlayDir has a @flux entry, where there are some
	// shortcuts for convenience:
	//   - /@flux/self.exe     SysFile for the current binary
	//   - /@flux/self/        Directory where the current binary resides
	//   - /@flux/temp/        System temporary directory
	//   - /@flux/user/        Several per-user directories; see below.
	//   - /@flux/<portal>/    Special <portal> directories; see below
	auto flux = make_shared<vfsi::VDir>( "@flux", root );
	root->add( flux );

	{
		//TODO: change path_get_*dir() paths to \\?\ style.
		auto const selfExe = make_shared<vfsi::SysFile>(
			sys::path_get_self(),
			"self.exe",
			flux
		);
		flux->add( selfExe );

		auto const self = make_shared<vfsi::SysDir>(
			sys::path_get_selfdir(),
			vfsi::ESysDirConstraints::none,
			"self",
			flux
		);
		flux->add( self );


		auto const temp = make_shared<vfsi::SysDir>(
			sys::path_get_tempdir(),
			vfsi::ESysDirConstraints::none,
			"temp",
			flux
		);
		flux->add( temp );

		auto const iwd = make_shared<vfsi::SysDir>(
			sys::path_get_current(),
			vfsi::ESysDirConstraints::none,
			"initial_working_dir",
			flux
		);
		flux->add( iwd );
	}

	// Per-user directories and per-app directories
	//
	// User directories:
	//   - /@flux/user/home     The user's home directory
	//   - /@flux/user/appdata  Per-user application data
	//   ..
	//
	// App directories:
	//   - /@flux/app/appdata   Per-application data
	//   ... 
	auto user = make_shared<vfsi::VDir>( "user", flux );
	auto app = make_shared<vfsi::VDir>( "app", flux );
	{
		// Create the home directory as an ordinary SysDir
		auto const home = make_shared<vfsi::SysDir>( 
			sys::path_get_userdir( sys::EUserPath::home ),
			vfsi::ESysDirConstraints::none,
			"home",
			user
		);
		user->add( home );

		// Others are special directories. UserDir nodes will create the
		// underlying directory on demand, possibly via OS facilities. 
		// LazyDir will create the underlying directory on demand via 
		// standard means.
		auto const appident = appinfo_ident();

#		define FLUXTMP_UADIR_(x,d) do {                                   \
			auto udir = make_shared<vfsi::UserDir>( x, d, user );         \
			user->add( udir );                                            \
			app->add( make_shared<vfsi::LazyDir>( appident, udir ), d );  \
		} while(0) /*ENDM*/

		FLUXTMP_UADIR_( sys::EUserPath::data, "appdata" );
		FLUXTMP_UADIR_( sys::EUserPath::cache, "cache" );
		FLUXTMP_UADIR_( sys::EUserPath::config, "config" );
		FLUXTMP_UADIR_( sys::EUserPath::saves, "saves" );
		FLUXTMP_UADIR_( sys::EUserPath::screenshots, "screenshots" );
#		undef FLUXTMP_UADIR_

#		define FLUXTMP_UDIR_(x,d) user->add( make_shared<vfsi::UserDir>( x, d, user ) )
		FLUXTMP_UDIR_( sys::EUserPath::desktop, "desktop" );
		FLUXTMP_UDIR_( sys::EUserPath::downloads, "downloads" );
#		undef FLUXTMP_UDIR_
	}
	flux->add( user );
	flux->add( app );

	// Customization points
	//   - /@flux/opt/        Customization point for optional "default" directories
	//   - /@flux/internal/   Customization point for flux-internal directories
	// TODO
	flux->add( make_shared<vfsi::VDir>( "opt", flux ) );
	flux->add( make_shared<vfsi::VDir>( "internal", flux ) );

	// Special portal directories
	// These links to some of the same directories as above, however with the
	// difference that the parent paths go to the system-parent of the node 
	// and not the VFS parent.
	auto portal = make_shared<vfsi::PDir>( "<portal>", flux );
	{
		compat::error_code ecs;
		if( auto const self = sysroot->dir_open( sys_to_vfs_(sys::path_get_selfdir()), ecs ) )
		{
			portal->add( "self", self );
		}
		else
		{
			FLUX_LOG( WARNING, "VFS: /@flux/<portal>/self: dir_open() on '{}': {}", sys_to_vfs_(sys::path_get_selfdir()), ecs.message() );
		}

		compat::error_code ect;
		if( auto const temp = sysroot->dir_open( sys_to_vfs_(sys::path_get_tempdir()), ect ) )
		{
			portal->add( "temp", temp );
		}
		else
		{
			FLUX_LOG( WARNING, "VFS: /@flux/<portal>/temp: dir_open() on '{}': {}", sys_to_vfs_(sys::path_get_tempdir()), ect.message() );
		}

		compat::error_code eci;
		if( auto const iwd = sysroot->dir_open( sys_to_vfs_(sys::path_get_current()), eci ) )
		{
			portal->add( "initial_working_dir", iwd );
		}
		else
		{
			FLUX_LOG( WARNING, "VFS: /@flux/<portal>/temp: dir_open() on '{}': {}", sys_to_vfs_(sys::path_get_current()), eci.message() );
		}
	}
	flux->add( portal );
	
	// CWD
	auto const cwdPath = sys_to_vfs_(sys::path_get_current());

	compat::error_code ec;
	auto cwd = root->dir_open( cwdPath, ec );

	if( !cwd )
	{
		FLUX_LOG( INFO, "VFS: couldn't get CWD through root ({}). Using floating CWD.", ec.message() );
		cwd = make_shared<vfsi::SysDir>( sys::path_get_current(), vfsi::ESysDirConstraints::none );
	}

	// finalize
	auto ret = io::detail::VfsRecordFactory::make( cwd, root );

	if( !!(EDefaultEntryOptions::mountAssetsDir & aOptions) )
		default_entry_add_asset_dir( ret );
	if( !!(EDefaultEntryOptions::mountSystemDir & aOptions) )
		default_entry_add_system_dir( ret );

	return ret;
}


//--    default_entry_add_asset_dir()       ///{{{2///////////////////////////
namespace
{
	template< typename tType >
	char const* tyname_( tType const* aPtr )
	{
		/* This avoids a warning regarding operator* being evaluated despite
		 * being used in a typeid(). Which is OK in this case.
		 */
		return typeid(*aPtr).name();
	}
}

void default_entry_add_asset_dir( VfsRecord aDefaultEntry, char const* aAppName, EAssetDirSearch aSearch, ftl::Vector<std::string> aRequired, ftl::Vector<std::string> aRequested )
{
	// Get the /@flux/opt "mount point"
	auto root = aDefaultEntry.root();

	compat::error_code ec;
	auto opt = root->dir_open( "/@flux/opt", ec );
	FLUX_THROW_IF( !opt, error::VfsInvalidRecord )
		<< einfo::VfsRecordStat( root->stat() )
		<< einfo::VfsDesc( root->desc() )
		<< flux::einfo::ErrorDesc( "Couldn't find mount point for assets dir" )
		<< flux::einfo::ErrorMeta( "trying at '/@flux/opt" )
		<< flux::einfo::ApiError( ec )
	;

	auto vdir = std::dynamic_pointer_cast<vfsi::VDir>(opt);
	FLUX_THROW_IF( !opt, error::VfsInvalidRecord )
		<< einfo::VfsRecordStat( opt->stat() )
		<< einfo::VfsDesc( opt->desc() )
		<< flux::einfo::ErrorDesc( "/@flux/opt: Invalid directory type" )
		<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("expected 'vfsi::VDir', got {}"), tyname_(opt.get()) )
	;

	// Get the assets dir
	bool alreadyMounted = true;
	auto assets = [&] {
		compat::error_code ec;
		if( auto adir = vdir->dir_open( "assets", ec ) )
			return std::dynamic_pointer_cast<vfsi::UnionDir>(adir);

		alreadyMounted = false;
		return make_shared<vfsi::UnionDir>( "assets", vdir );
	}();

	FLUX_THROW_IF( !assets, error::VfsInvalidRecord )
		<< einfo::VfsRecordStat( opt->stat() )
		<< einfo::VfsDesc( opt->desc() )
		<< flux::einfo::ErrorDesc( FLUX_FMT_STRING("/@flux/opt/assets: {}"), alreadyMounted ? "wrong directory type":"unable to create" )
		<< flux::einfo::ErrorMeta( "expected 'vfsi::UnionDir'" )
	;

	// Try the required locations
	for( auto const& dir : aRequired )
	{
		auto rec = vfs_open( aDefaultEntry, dir );
		assets->add_back( rec.self() );
	}

	// Try the requested locations
	for( auto const& dir : aRequested )
	{
		try_add_vdir_( assets, aDefaultEntry, dir, "<user requested dir>", "assets" );
	}

	// Try to add the various default locatiosn
	if( !!(EAssetDirSearch::env & aSearch) )
	{
		if( char const* ad = std::getenv( "FLUXENV_ASSET_DIR" ) )
			try_add_vdir_( assets, aDefaultEntry, ad, "$FLUXENV_ASSET_DIR", "assets" );
	}

	if( !!(EAssetDirSearch::cwd & aSearch) )
		try_add_vdir_( assets, aDefaultEntry, "./assets", "cwd", "assets" );

	if( !!(EAssetDirSearch::selfdir & aSearch) )
		try_add_vdir_( assets, aDefaultEntry, "/@flux/self/assets", "selfdir", "assets" );

	if( !!(EAssetDirSearch::selfdirParent & aSearch) )
	{
		try
		{
			auto ps = vfs_open( aDefaultEntry, "/@flux/<portal>/self" );
			try_add_vdir_( assets, ps, "../assets", "/@flux/<portal>/self", "assets" );
		}
		catch( error::VfsError const& eError )
		{
			FLUX_LOG( 1, "VFS: assets dir: /@flux/<portal>/self: {}", eError.what() );
		}
	}

	if( !!(EAssetDirSearch::selfdirBinParent & aSearch) )
	{
		try
		{
			auto ps = vfs_open( aDefaultEntry, "/@flux/<portal>/self" );

			VfsRecord rec;
			unsigned lvl = find_parent_( ps, "bin", rec );
			if( ~unsigned(0) != lvl )
			{
				if( 0 != lvl || !(EAssetDirSearch::selfdirParent & aSearch) )
				{
					try_add_vdir_( assets, rec, "../assets", rec.self()->desc().c_str(), "assets" );
				}
			}
		}
		catch( error::VfsError const& eError )
		{
			FLUX_LOG( 1, "VFS: assets dir: /@flux/<portal>/self: {}", eError.what() );
		}
	}


	bool const inParent = [&] {
		auto sdp = sys::path_to_generic(sys::path_get_selfdir());
		auto last = sdp.find_last_of( '/' );
		if( decltype(sdp)::npos == last )
			last = 0;

		return compat::string_view("bin") == compat::string_view(sdp.data()+last, sdp.size()-last);
	}();

	if( !!(EAssetDirSearch::appLocalLib & aSearch) && aAppName )
	{
		auto path = format( "{}lib/{}", inParent ? "../":"", aAppName );

		try
		{
			auto ps = vfs_open( aDefaultEntry, "/@flux/<portal>/self" );
			try_add_vdir_( assets, ps, path, "/@flux/<portal>/self", "assets" );
		}
		catch( error::VfsError const& eError )
		{
			FLUX_LOG( 1, "VFS: assets dir: '{}': {}", path, eError.what() );
		}
	}
	if( !!(EAssetDirSearch::appLocalShare & aSearch) && aAppName )
	{
		auto path = format( "{}share/{}", inParent ? "../":"", aAppName );
		
		try
		{
			auto ps = vfs_open( aDefaultEntry, "/@flux/<portal>/self" );
			try_add_vdir_( assets, ps, path, "/@flux/<portal>/self", "assets" );
		}
		catch( error::VfsError const& eError )
		{
			FLUX_LOG( 1, "VFS: assets dir: '{}': {}", path, eError.what() );
		}
	}


	if( !!(EAssetDirSearch::appUserData & aSearch) && aAppName )
	{
		try_add_vdir_( assets, aDefaultEntry, "/@flux/app/appdata", "/@flux/app/appdata", "assets" );
		//auto path = format( "/@flux/user/appdata/{}", aAppName );
		//try_add_vdir_( assets, aDefaultEntry, path, "EUserPath::appdata", "assets" );
	}
	if( !!(EAssetDirSearch::appUserData & aSearch) && aAppName )
	{
		try_add_vdir_( assets, aDefaultEntry, "/@flux/app/cache", "/@flux/app/cache", "assets" );
		//auto path = format( "/@flux/user/cache/{}", aAppName );
		//try_add_vdir_( assets, aDefaultEntry, path, "EUserPath::cache", "assets" );
	}

	// Zip files
	if( !!(EAssetDirSearch::baseAssetsZip & aSearch) )
	{
		compat::error_code ec0;
		if( auto fnode = assets->dir_open( "base-assets.zip", ec0 ) )
		{
			try
			{
				assets->add_back( vfsi::inode_zip_archive( std::move(fnode), assets->path( vfsi::kAncestorNode ), assets->ancestor() ) );
			}
			catch( error::VfsError const& eErr )
			{
				FLUX_LOG( 1, "VFS: assets dir: 'base-assets.zip' not usable: {}", eErr.what() );
			}
		}
		else FLUX_LOG( 1, "VFS: assets dir: 'base-assets.zip' not found in assets directories: {}", ec0.message() );
	}
	if( !!(EAssetDirSearch::fluxDefaultZip & aSearch) )
	{
		compat::error_code ec0;
		if( auto fnode = assets->dir_open( "flux-default-assets.zip", ec0 ) )
		{
			try
			{
				assets->add_back( vfsi::inode_zip_archive( std::move(fnode), assets->path( vfsi::kAncestorNode ), assets->ancestor() ) );
			}
			catch( error::VfsError const& eErr )
			{
				FLUX_LOG( 1, "VFS: assets dir: 'flux-default-assets.zip' not usable: {}", eErr.what() );
			}
		}
		else FLUX_LOG( 1, "VFS: assets dir: 'flux-default-assets.zip' not found in assets directories: {}", ec0.message() );
	}

	// "mount" the assets directory into /@flux/opt
	if( !alreadyMounted )
		vdir->add( assets );

	// summarize
	std::stringstream ss;
	ss << format( "/@flux/opt/assets: {:d} entries:\n", assets->count() );
	for( std::size_t i = 0, j = assets->count(); i < j; ++i )
	{
		auto item = assets->get(i);
		ss << format( " - '{}'\n", item->desc() );
	}

	FLUX_LOG( INFO, "{}", ss.str() );
}

//--    default_entry_add_system_dir()       ///{{{2///////////////////////////
void default_entry_add_system_dir( VfsRecord aDefaultEntry, char const* aAppName, ESystemDirSearch aSearch, ftl::Vector<std::string> aRequired, ftl::Vector<std::string> aRequested )
{
	// Get the /@flux/opt "mount point"
	auto root = aDefaultEntry.root();

	compat::error_code ec;
	auto opt = root->dir_open( "/@flux/opt", ec );
	FLUX_THROW_IF( !opt, error::VfsInvalidRecord )
		<< einfo::VfsRecordStat( root->stat() )
		<< einfo::VfsDesc( root->desc() )
		<< flux::einfo::ErrorDesc( "Couldn't find mount point for system dir" )
		<< flux::einfo::ErrorMeta( "trying at '/@flux/opt" )
		<< flux::einfo::ApiError( ec )
	;

	auto vdir = std::dynamic_pointer_cast<vfsi::VDir>(opt);
	FLUX_THROW_IF( !opt, error::VfsInvalidRecord )
		<< einfo::VfsRecordStat( opt->stat() )
		<< einfo::VfsDesc( opt->desc() )
		<< flux::einfo::ErrorDesc( "/@flux/opt: Invalid directory type" )
		<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("expected 'vfsi::VDir', got {}"), tyname_(opt.get()) )
	;

	// Get the assets dir
	bool alreadyMounted = true;
	auto system = [&] {
		compat::error_code ec;
		if( auto adir = vdir->dir_open( "system", ec ) )
			return std::dynamic_pointer_cast<vfsi::UnionDir>(adir);

		alreadyMounted = false;
		return make_shared<vfsi::UnionDir>( "system", vdir );
	}();

	FLUX_THROW_IF( !system, error::VfsInvalidRecord )
		<< einfo::VfsRecordStat( opt->stat() )
		<< einfo::VfsDesc( opt->desc() )
		<< flux::einfo::ErrorDesc( FLUX_FMT_STRING("/@flux/opt/system: {}"), alreadyMounted ? "wrong directory type":"unable to create" )
		<< flux::einfo::ErrorMeta( "Expected 'vfsi::UnionDir'" )
	;

	// Try the required locations
	for( auto const& dir : aRequired )
	{
		auto rec = vfs_open( aDefaultEntry, dir );
		system->add_back( rec.self() );
	}

	// Try the requested locations
	for( auto const& dir : aRequested )
	{
		try_add_vdir_( system, aDefaultEntry, dir, "<user requested dir>", "system" );
	}

	// Try to add the various default locatiosn
	if( !!(ESystemDirSearch::env & aSearch) )
	{
		if( char const* ad = std::getenv( "FLUXENV_SYSTEM_DIR" ) )
			try_add_vdir_( system, aDefaultEntry, ad, "$FLUXENV_SYSTEM_DIR", "system" );
	}

	if( !!(ESystemDirSearch::selfdir & aSearch) )
		try_add_vdir_( system, aDefaultEntry, "/@flux/self/system", "selfdir", "system" );

	if( !!(ESystemDirSearch::selfdirParent & aSearch) )
	{
		try
		{
			auto ps = vfs_open( aDefaultEntry, "/@flux/<portal>/self" );
			try_add_vdir_( system, ps, "../system", "/@flux/<portal>/self", "assets" );
		}
		catch( error::VfsError const& eError )
		{
			FLUX_LOG( 1, "VFS: system dir: /@flux/<portal>/self: {}", eError.what() );
		}
	}
	if( !!(ESystemDirSearch::selfdirBinParent & aSearch) )
	{
		try
		{
			auto ps = vfs_open( aDefaultEntry, "/@flux/<portal>/self" );

			VfsRecord rec;
			unsigned lvl = find_parent_( ps, "bin", rec );
			if( ~unsigned(0) != lvl )
			{
				if( 0 != lvl || !(ESystemDirSearch::selfdirParent & aSearch) )
				{
					try_add_vdir_( system, rec, "../system", rec.self()->desc().c_str(), "system" );
				}
			}
		}
		catch( error::VfsError const& eError )
		{
			FLUX_LOG( 1, "VFS: system dir: /@flux/<portal>/self: {}", eError.what() );
		}
	}


	bool const inParent = [&] {
		auto sdp = sys::path_to_generic(sys::path_get_selfdir());
		auto last = sdp.find_last_of( '/' );
		if( decltype(sdp)::npos == last )
			last = 0;

		return compat::string_view("bin") == compat::string_view(sdp.data()+last, sdp.size()-last);
	}();

	if( !!(ESystemDirSearch::appLocalLib & aSearch) && aAppName )
	{
		auto path = format( "{}lib/{}", inParent ? "../":"", aAppName );
		
		try
		{
			auto ps = vfs_open( aDefaultEntry, "/@flux/<portal>/self" );
			try_add_vdir_( system, ps, path, "/@flux/<portal>/self", "system" );
		}
		catch( error::VfsError const& eError )
		{
			FLUX_LOG( 1, "VFS: system dir: '{}': {}", path, eError.what() );
		}
	}

	if( !!(ESystemDirSearch::appUserData & aSearch) && aAppName )
	{
		try_add_vdir_( system, aDefaultEntry, "/@flux/app/appdata", "/@flux/app/appdata", "system" );
	}

	// "mount" the system directory into /@flux/opt
	if( !alreadyMounted )
		vdir->add( system );
	
	// summarize
	std::stringstream ss;
	ss << format( "/@flux/opt/system: {:d} entries:\n", system->count() );
	for( std::size_t i = 0, j = system->count(); i < j; ++i )
	{
		auto item = system->get(i);
		ss << format( " - '{}'\n", item->desc() );
	}

	FLUX_LOG( INFO, "{}", ss.str() );
}

//--    $ try_add_vdir_()                   ///{{{2///////////////////////////
namespace
{
	void try_add_vdir_( shared_ptr<vfsi::UnionDir> const& aUD, VfsRecord const& aDE, compat::string_view aVPath, char const* aName, char const* aIdent )
	{
		FLUX_ASSERT( aUD );

		try
		{
			auto rec = vfs_open( aDE, aVPath );
			FLUX_ASSERT( rec.self() && rec.self()->stat().directory );
			aUD->add_back( rec.self() );
		}
		catch( error::VfsNotFound const& )
		{
			FLUX_LOG( 1, "VFS: {} dir: in {} ('{}') not found. Omitting.", aIdent, aName, aVPath );
		}
		catch( error::VfsDenied const& )
		{
			FLUX_LOG( 1, "VFS: {} dir: in {} ('{}') not accessible. Omitting.", aIdent, aName, aVPath );
		}
		catch( error::VfsError const& eError )
		{
			FLUX_LOG( 1, "VFS: {} dir: in {} ('{}'): {}", aIdent, aName, aVPath, eError.what() );
		}
	}
}

//--    $ find_parent_()                    ///{{{2///////////////////////////
namespace
{
	unsigned find_parent_( VfsRecord const& aStart, compat::string_view aParentName, VfsRecord& aOutput )
	{
		auto record = make_optional( aStart );
		unsigned level = 0;

		while( record )
		{
			auto const& self = record.value().self();
			if( self->stat().directory && self->name() == aParentName )
			{
				aOutput = std::move(record).value();
				return level;
			}

			compat::error_code ec;
			record = vfs_open( record.value(), "..", ec );
			++level;
		}

		return ~unsigned(0);
	}
}

//--    <<< ~ flux::io::vfs namespace <<<           ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfs)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
