/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	System root entry implementation
 *
 * TODO: List/enumerate via
 *    - FindFirstVolumeW() + FindNextVolumeW() + FindVolumeClose()
 *    - GetVolumePathNamesForVolumeNameW()
 *
 *  -> See https://docs.microsoft.com/en-us/windows/desktop/fileio/displaying-volume-paths
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfsi/sysroot.hpp>

#include <flux/io/vpath.hpp>
#include <flux/io/errors.hpp>
#include <flux/io/vfsi/sysdir.hpp>
#include <flux/io/vfsi/sysfile.hpp>
#include <flux/io/buffer/sysfile.hpp>
#include <flux/io/stream/sysfile.hpp>

#include <flux/setup/platform.hpp>

#include <flux/sys/errors.hpp>
#include <flux/sys/syspath.hpp>
#include <flux/sys/fsquery.hpp>
#include <flux/sys/fsmanip.hpp>
#include <flux/sys/pathutil.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/format.hpp>
#include <flux/std/except/einfo.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io::vfsi >>>          ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	class SysRootWin32_ final : public VINode
	{
		public:
			SysRootWin32_();

		public: /* virtuals: generic */
			std::string desc() const override;

			//void update( bool ) override;
			//void remove( bool ) override;

		public: /* virtuals: directory access */
			optional<VfsStat> dir_stat( compat::string_view const&, compat::error_code& ) noexcept override;
			shared_ptr<VINode> dir_open( compat::string_view const&, compat::error_code& ) noexcept override;

			unique_ptr<Buffer> dir_open_buffer( 
				compat::string_view const&,
				EBufferMode,
				bool,
				BufferSize,
				compat::error_code&
			) noexcept override;
			unique_ptr<Stream> dir_open_stream( 
				compat::string_view const&,
				EStreamMode,
				compat::error_code&
			) noexcept override;

			void dir_create( compat::string_view const&, bool ) override;
			bool dir_remove( compat::string_view const&, bool ) override;

		private:
			sys::SysPathStr make_path_( compat::string_view const& );
	};
	class NetRootWin32_ final : public VINode
	{
		public:
			NetRootWin32_( compat::string_view const&, weak_ptr<VINode> );

		public: /* virtuals: generic */
			std::string desc() const override;

			//void update( bool ) override;
			//void remove( bool ) override;

		public: /* virtuals: directory access */
			optional<VfsStat> dir_stat( compat::string_view const&, compat::error_code& ) noexcept override;
			shared_ptr<VINode> dir_open( compat::string_view const&, compat::error_code& ) noexcept override;

			unique_ptr<Buffer> dir_open_buffer( 
				compat::string_view const&,
				EBufferMode,
				bool,
				BufferSize,
				compat::error_code&
			) noexcept override;
			unique_ptr<Stream> dir_open_stream( 
				compat::string_view const&,
				EStreamMode,
				compat::error_code&
			) noexcept override;

			void dir_create( compat::string_view const&, bool ) override;
			bool dir_remove( compat::string_view const&, bool ) override;

		private:
			sys::SysPathStr make_path_( compat::string_view const& );
	};

	/* The following are workarounds for VisualStudio 2017.
	 *
	 * VS produces a number of nonsensical errors on the final 
	 *   einfo::ErrorMeta( FLUX_FMT_STRING(...), ... )
	 * in this file. I'm honestly not sure at this point as to what's going on,
	 * as the errors also randomly mutate when the arguments after the format
	 * string are varied. My final WAG is that we're hitting the limits of what
	 * MSVC is prepared to give us in terms of constexpr evaluation. This ...
	 * doesn't trigger the problems.
	 */
	flux::einfo::ErrorMeta make_error_meta_to_sidestep_vs_problems_( compat::string_view const& );
#	endif // ~ WIN32
}

//--    inode_create_sysroot()              ///{{{2///////////////////////////
shared_ptr<VINode> inode_create_sysroot()
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	return make_shared<SysDir>(
		sys::path_from_generic( "/" ),
		ESysDirConstraints::contain,
		"/",
		weak_ptr<VINode>()
	);
#	else
	return make_shared<SysRootWin32_>();
#	endif
}

//--    $ SysRootWin32_                     ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	// [cd]tor -{{{3-
	SysRootWin32_::SysRootWin32_()
		: VINode()
	{
		mStat.directory = 1;
		mStat.accessible = 1;
		mStat.listable = 0; //XXX
		mStat.flat = 0;
		mStat.hasNativePath = 0;
	}

	// virtuals: generic -{{{3-
	std::string SysRootWin32_::desc() const
	{
		return "magic-win32-sysroot";
	}

	// virtuals: directory access -{{{3-
	optional<VfsStat> SysRootWin32_::dir_stat( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
	{
		std::string simple;
		if( auto def = dir_stat_defaults( aPath, true, &simple, aErrorOut ) )
			return def.value();

		auto const path = make_path_(simple); //XXX allocates
		if( auto sb = sys::stat_path(path, aErrorOut) )
		{
			VfsStat ret;
			switch( sys::stat_get_type(sb) )
			{
				case sys::EFileKind::regular:
				{
					compat::error_code ec0, ec1;
					auto rr = sys::check_access( path, sys::EPathAccess::read, ec0 );
					auto rw = sys::check_access( path, sys::EPathAccess::write, ec1 );
				
					ret.file = 1;
					ret.readable = rr.value_or( false );
					ret.writable = rr.value_or( false );
					ret.mappable = ret.readable || ret.writable;
					ret.sized = 1;
					ret.hasNativeMap = 1;
					break;
				}

				case sys::EFileKind::directory:
				{
					compat::error_code ec0, ec1, ec2;
					auto dr = sys::check_access( path, sys::EPathAccess::read, ec0 );
					auto dw = sys::check_access( path, sys::EPathAccess::write, ec1 );
					auto dx = sys::check_access( path, sys::EPathAccess::execute, ec2 );

					ret.directory = 1;
					ret.accessible = dx.value_or( false );
					ret.listable = dx.value_or( false );
					ret.changeable = dx.value_or( false );
					ret.flat = 0;
					break;
				}

				default:
				{
					compat::error_code ec0, ec1;
					auto sr = sys::check_access( path, sys::EPathAccess::read, ec0 );
					auto sw = sys::check_access( path, sys::EPathAccess::write, ec1 );

					ret.special = 1;
					ret.readable = sr.value_or( false );
					ret.writable = sw.value_or( false );
					break;
				}
			}
			
			ret.hasNativePath = 1;
			return ret;
		}

		aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
		return {};
	}
	shared_ptr<VINode> SysRootWin32_::dir_open( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
	{
		std::string simple;
		if( auto def = dir_open_defaults( aPath, true, &simple, aErrorOut ) )
			return def.value();

		/* If we're asking for the host part of a network share (i.e., when the
		 * vfs path contains a single element), then return the NetRootWin32_
		 * proxy -- Win32 doesn't consider just e.g. \\?\UNC\Host to be a valid
		 * directory.
		 */
		auto split = vpath_split_first( simple );
		if( std::get<1>(split).empty() )
			return make_shared<NetRootWin32_>( std::get<0>(split), weak_ptr<VINode>(shared_from_this()) );

		auto const path = make_path_(simple); //XXX allocates:
		if( auto sb = sys::stat_path(path, aErrorOut ) )
		{
			if( sys::EFileKind::directory == sys::stat_get_type(sb) )
				return make_shared<SysDir>( path, ESysDirConstraints::none, std::string(aPath), shared_from_this() ); //XXX allocates
			else
				return make_shared<SysFile>( path, std::string(aPath), shared_from_this() ); //XXX allocates
		}

		aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
		return nullptr;
	}


	unique_ptr<Buffer> SysRootWin32_::dir_open_buffer( compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept try
	{
		std::string simple;
		if( auto def = dir_open_buffer_defaults( aPath, aMode, aMustExist, aResizeTo, true, &simple, aErrorOut ) )
			return std::move(def.value());

		return buffer_create_sysfile( make_path_(simple), aMode, aMustExist, aResizeTo );
	}
	catch( error::BufferNotAvailable const& )
	{
		aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
		return nullptr;
	}
	unique_ptr<Stream> SysRootWin32_::dir_open_stream( compat::string_view const& aPath, EStreamMode aMode, compat::error_code& aErrorOut ) noexcept try
	{
		std::string simple;
		if( auto def = dir_open_stream_defaults( aPath, aMode, true, &simple, aErrorOut ) )
			return std::move(def.value());

		return stream_create_sysfile( make_path_(simple), aMode );
	}
	catch( error::StreamNotAvailable const& )
	{
		aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
		return nullptr;
	}


	void SysRootWin32_::dir_create( compat::string_view const& aPath, bool aRecursive ) try
	{
		std::string simple;
		if( dir_create_defaults( aPath, aRecursive, true, &simple ) )
			return;

		FLUX_THROW_IF( vpath_escapes(aPath, &simple), error::VfsDenied )
			<< einfo::VfsDesc( desc() )
			<< einfo::VfsRecordStat( stat() )
			<< flux::einfo::ErrorDesc( "Path escapes Win32-Sys-Root" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = {}"), simple )
		;

		sys::dir_create( make_path_(simple), aRecursive );

		compat::error_code ec;
		SysRootWin32_::update( false, ec );
		(void)ec;
	}
	catch( sys::error::FsError const& )
	{
		FLUX_THROW_WITH_NESTED( error::VfsNotFound ) 
			<< einfo::VfsDesc( desc() )
			<< flux::einfo::ErrorDesc( "Requested path does not exist" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), aPath )
		;
	}

	bool SysRootWin32_::dir_remove( compat::string_view const& aPath, bool aRecursive )
	{
		std::string simple;
		if( auto def = dir_remove_defaults( aPath, aRecursive, true, &simple ) )
			return def.value();

		auto const path = make_path_(simple);
		if( auto sb = sys::stat_path(path) )
		{
			if( sys::EFileKind::directory == sys::stat_get_type(sb) )
				sys::dir_remove( path, aRecursive );
			else
				sys::file_remove( path );

			compat::error_code ec;
			SysRootWin32_::update( false, ec );
			return true;
		}

		return false;
	}

	// private -{{{3-
	sys::SysPathStr SysRootWin32_::make_path_( compat::string_view const& aVfsPath )
	{
		//TODO: maybe check that aVfsPath doesn't start with a '/' ... register badness?

		// Handle drive letters
		//TODO: verify that the first part is a letter?
		if( aVfsPath.size() >= 2 && aVfsPath[1] == ':' )
			return L"\\\\?\\" + sys::path_from_generic(aVfsPath);

		// Assume this is a fancy //foo/bar thing
		//TODO: //foo shares?
		return L"\\\\?\\UNC\\" + sys::path_from_generic(aVfsPath);
	}
#	endif // ~ WIN32
}

//--    $ NetRootWin32_                     ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	// [cd]tor -{{{3-
	NetRootWin32_::NetRootWin32_( compat::string_view const& aNetRoot, weak_ptr<VINode> aSysRoot )
		: VINode( std::string(aNetRoot), std::move(aSysRoot) )
	{
		mStat.directory = 1;
		mStat.accessible = 1;
		mStat.listable = 0; //XXX
		mStat.flat = 0;
		mStat.hasNativePath = 0;
	}

	// virtuals: generic -{{{3-
	std::string NetRootWin32_::desc() const
	{
		return format( "magic-win32-netroot:{}", name() );
	}

	// virtuals: directory access -{{{3-
	optional<VfsStat> NetRootWin32_::dir_stat( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
	{
		std::string simple;
		if( auto def = dir_stat_defaults( aPath, true, &simple, aErrorOut ) )
			return def.value();

		auto const path = make_path_(simple); //XXX allocates
		if( auto sb = sys::stat_path(path, aErrorOut) )
		{
			VfsStat ret;
			switch( sys::stat_get_type(sb) )
			{
				case sys::EFileKind::regular:
				{
					compat::error_code ec0, ec1;
					auto rr = sys::check_access( path, sys::EPathAccess::read, ec0 );
					auto rw = sys::check_access( path, sys::EPathAccess::write, ec1 );
				
					ret.file = 1;
					ret.readable = rr.value_or( false );
					ret.writable = rr.value_or( false );
					ret.mappable = ret.readable || ret.writable;
					ret.sized = 1;
					ret.hasNativeMap = 1;
					break;
				}

				case sys::EFileKind::directory:
				{
					compat::error_code ec0, ec1, ec2;
					auto dr = sys::check_access( path, sys::EPathAccess::read, ec0 );
					auto dw = sys::check_access( path, sys::EPathAccess::write, ec1 );
					auto dx = sys::check_access( path, sys::EPathAccess::execute, ec2 );

					ret.directory = 1;
					ret.accessible = dx.value_or( false );
					ret.listable = dx.value_or( false );
					ret.changeable = dx.value_or( false );
					ret.flat = 0;
					break;
				}

				default:
				{
					compat::error_code ec0, ec1;
					auto sr = sys::check_access( path, sys::EPathAccess::read, ec0 );
					auto sw = sys::check_access( path, sys::EPathAccess::write, ec1 );

					ret.special = 1;
					ret.readable = sr.value_or( false );
					ret.writable = sw.value_or( false );
					break;
				}
			}
			
			ret.hasNativePath = 1;
			return ret;
		}

		aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
		return {};
	}
	shared_ptr<VINode> NetRootWin32_::dir_open( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
	{
		std::string simple;
		if( auto def = dir_open_defaults( aPath, true, &simple, aErrorOut ) )
			return def.value();

		auto const path = make_path_(simple); //XXX allocates:
		if( auto sb = sys::stat_path(path, aErrorOut ) )
		{
			if( sys::EFileKind::directory == sys::stat_get_type(sb) )
				return make_shared<SysDir>( path, ESysDirConstraints::none, std::string(aPath), shared_from_this() ); //XXX allocates
			else
				return make_shared<SysFile>( path, std::string(aPath), shared_from_this() ); //XXX allocates
		}

		aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
		return nullptr;
	}


	unique_ptr<Buffer> NetRootWin32_::dir_open_buffer( compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept try
	{
		std::string simple;
		if( auto def = dir_open_buffer_defaults( aPath, aMode, aMustExist, aResizeTo, true, &simple, aErrorOut ) )
			return std::move(def.value());

		return buffer_create_sysfile( make_path_(simple), aMode, aMustExist, aResizeTo );
	}
	catch( error::BufferNotAvailable const& )
	{
		aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
		return nullptr;
	}
	unique_ptr<Stream> NetRootWin32_::dir_open_stream( compat::string_view const& aPath, EStreamMode aMode, compat::error_code& aErrorOut ) noexcept try
	{
		std::string simple;
		if( auto def = dir_open_stream_defaults( aPath, aMode, true, &simple, aErrorOut ) )
			return std::move(def.value());

		return stream_create_sysfile( make_path_(simple), aMode );
	}
	catch( error::StreamNotAvailable const& )
	{
		aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
		return nullptr;
	}


	void NetRootWin32_::dir_create( compat::string_view const& aPath, bool aRecursive ) try
	{
		std::string simple;
		if( dir_create_defaults( aPath, aRecursive, true, &simple ) )
			return;

		FLUX_THROW_IF( vpath_escapes(aPath, &simple), error::VfsDenied )
			<< einfo::VfsDesc( desc() )
			<< einfo::VfsRecordStat( stat() )
			<< flux::einfo::ErrorDesc( "Path escapes Win32-Net-Root" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), simple )
		;

		sys::dir_create( make_path_(simple), aRecursive );

		compat::error_code ec;
		NetRootWin32_::update( false, ec );
		(void)ec;
	}
	catch( sys::error::FsError const& )
	{
		FLUX_THROW_WITH_NESTED( error::VfsNotFound ) 
			<< einfo::VfsDesc( desc() )
			<< flux::einfo::ErrorDesc( "Requested path does not exist" )
			//<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), aPath ) // THIS BREAKS VISUAL STUDIO
			<< make_error_meta_to_sidestep_vs_problems_( aPath )
		;
	}

	bool NetRootWin32_::dir_remove( compat::string_view const& aPath, bool aRecursive )
	{
		std::string simple;
		if( auto def = dir_remove_defaults( aPath, aRecursive, true, &simple ) )
			return def.value();

		auto const path = make_path_(simple);
		if( auto sb = sys::stat_path(path) )
		{
			if( sys::EFileKind::directory == sys::stat_get_type(sb) )
				sys::dir_remove( path, aRecursive );
			else
				sys::file_remove( path );

			compat::error_code ec;
			NetRootWin32_::update( false, ec );
			return true;
		}

		return false;
	}

	// private -{{{3-
	sys::SysPathStr NetRootWin32_::make_path_( compat::string_view const& aVfsPath )
	{
		//TODO: maybe check that aVfsPath doesn't start with a '/' ... register badness?
		return L"\\\\?\\UNC\\" + sys::path_lexical_join( 
			sys::path_from_generic(mPathFromAncestor),
			sys::path_from_generic(aVfsPath)
		);
	}
#	endif // ~ WIN32
}

//--    $ detail                            ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	flux::einfo::ErrorMeta make_error_meta_to_sidestep_vs_problems_( compat::string_view const& aPath )
	{
		return flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), aPath );
	}
#	endif // ~ WIN32
}

//--    <<< ~ flux::io::vfsi namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
