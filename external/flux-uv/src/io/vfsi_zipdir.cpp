/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	ZIP archive VINode
 *
 * Note: this assumes that it's safe to call mz_zip_reader_extract_iter_read()
 * from multiple threads concurrently under the assumption that the reader
 * function is thread safe (we're making sure of that).
 *
 * This is not 100% the case, as the code tries to set the error state in the
 * zip archive (which is shared across threads). Everything else seems to be
 * fine. 
 *
 * This means that we can open multiple streams from a single zip archive. 
 * Although all of these will share the same IO stream (so, no concurrent
 * reads), they may be able use different cores for decompression. 
 *
 * In a quick test, reading a file (uncompressed = 250MB) gave almost perfect
 * scaling with up to four cores. In this test, the ZIP-archive was most likely
 * cached by the OS. Cold loading might be different, but for development
 * (where the program is often rerun many times), this is a nice gain. In this
 * test, neither valgrind nor helgrind report any errors (although, the code
 * likely never sets the per-ZIP archive error code during this test).
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfsi/zipdir.hpp>

#include <flux/io/vpath.hpp>
#include <flux/io/buffer.hpp>
#include <flux/io/stream.hpp>
#include <flux/io/errors.hpp>
#include <flux/io/buffer/stream.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/format.hpp>

#include <flux/ftl/vector.hpp>
#include <flux/compat/byte.hpp>
#include <flux/util/narrow.hpp>

#include <flux_ext/miniz.hpp>

#include <mutex>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io::vfsi >>>          ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	using PathBuf_ = ftl::Vector<char, 128>;

	class ZipDirBase_ : public VINode
	{
		public:
			ZipDirBase_(
				unique_ptr<Stream>,
				std::string,
				weak_ptr<VINode>
			);

			~ZipDirBase_();

		public: /* virtuals: generic */
			//std::string desc() const override;
			//bool update( bool, compat::error_code& ) noexcept override;

		public: /* virtuals: file access */
			VfsSize file_size( compat::error_code& ) noexcept override;

			unique_ptr<Stream> file_as_stream(
				EStreamMode,
				compat::error_code&
			) noexcept override;

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

		protected:
			void init_();
			int lookup_( compat::string_view const&, bool* = nullptr ) const noexcept;

			friend class ZipProxy_;

		protected:
			mutable mz_zip_archive mArch;
			mutable std::recursive_mutex mMutex;
			unique_ptr<Stream> mStream;
	};
	
	class ZipDirStream_ final : public ZipDirBase_
	{
		public:
			ZipDirStream_(
				unique_ptr<Stream>,
				std::string,
				weak_ptr<VINode>
			);

		public: /* virtuals: generic */
			std::string desc() const override;
			bool update( bool, compat::error_code& ) noexcept override;
	};
	class ZipDirRecord_ final : public ZipDirBase_
	{
		public:
			ZipDirRecord_(
				shared_ptr<VINode>,
				unique_ptr<Stream>,
				std::string,
				weak_ptr<VINode>
			);

		public: /* virtuals: generic */
			std::string desc() const override;

			bool update( bool, compat::error_code& ) noexcept override;
			bool remove( bool, compat::error_code& ) noexcept override;
			
			optional<sys::SysPathStr> native_path( compat::error_code& ) const override;

		public: /* virtuals: file access */
			VfsSize file_size( compat::error_code& ) noexcept override;

			unique_ptr<Buffer> file_as_buffer( 
				EBufferMode,
				bool aMustExist,
				BufferSize aResizeTo,
				compat::error_code&
			) noexcept override;
			unique_ptr<Stream> file_as_stream(
				EStreamMode,
				compat::error_code&
			) noexcept override;

		private:
			shared_ptr<VINode> mFileNode;
	};

	class ZipProxy_ final : public VINode
	{
		public:
			ZipProxy_(
				shared_ptr<ZipDirBase_>,
				std::string aPathFromAncestor,
				VfsStat,
				VfsSize
			);

		public: /* virtuals: generic */
			std::string desc() const override;

			bool update( bool, compat::error_code& ) noexcept override;
			bool remove( bool, compat::error_code& ) noexcept override;

		public: /* virtuals: file access */
			VfsSize file_size( compat::error_code& ) noexcept override;

			unique_ptr<Buffer> file_as_buffer( 
				EBufferMode,
				bool aMustExist,
				BufferSize aResizeTo,
				compat::error_code&
			) noexcept override;
			unique_ptr<Stream> file_as_stream(
				EStreamMode,
				compat::error_code&
			) noexcept override;

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
			shared_ptr<ZipDirBase_> mParent;
			VfsSize mFileSize;
	};

	class ZipStream_ final : public Stream
	{
		public:
			ZipStream_( mz_zip_reader_extract_iter_state*, shared_ptr<ZipDirBase_> aParent );
			~ZipStream_();

		public:
			StreamSize read( void*, StreamSize, EIoRequest ) override;

			EStreamStatus status() const noexcept override;

			StreamCaps caps() const noexcept override;
			StreamSize size() const override;
			//StreamSize tell() const override;
			//StreamSize seek( StreamOffset, ESeekWhence ) override;
			StreamSize remaining() const override;

			std::string description() const override;
			ftl::Any native_handle() const override;

		private:
			mz_zip_reader_extract_iter_state* mState;
			shared_ptr<ZipDirBase_> mParent;
			StreamSize mRemaining;
			bool mReadable;
	};
}

//--    inode_zip_archive()                 ///{{{2///////////////////////////
shared_ptr<VINode> inode_zip_archive( shared_ptr<VINode> const& aNode, std::string aPathFromAncestor, weak_ptr<VINode> aAncestor )
{
	FLUX_ASSERT( aNode );

	auto const nstat = aNode->stat();
	FLUX_THROW_IF( !nstat.file, error::VfsInvalidRecord )
		<< einfo::VfsRecordStat( nstat )
		<< einfo::VfsDesc( aNode->desc() )
		<< flux::einfo::ErrorDesc( "inode_zip_archive(): requires file node" )
	;

	compat::error_code ec;
	if( auto stream = aNode->file_as_stream( EStreamMode::read, ec ) )
	{
		auto const scaps = stream->caps();
		FLUX_THROW_IF( !scaps.seekFwd || !scaps.seekRev, error::VfsError )
			<< einfo::StreamDesc( stream->description() )
			<< flux::einfo::ErrorDesc( "inode_zip_archive(): requires fully seekable stream" )
		;
		FLUX_THROW_IF( !scaps.size, error::VfsError )
			<< einfo::StreamDesc( stream->description() )
			<< flux::einfo::ErrorDesc( "inode_zip_archive(): requires stream with known size" )
		;

		if( aPathFromAncestor.empty() )
		{
			aPathFromAncestor = aNode->path( kAncestorNode );
			aAncestor = aNode->ancestor();
		}

		return make_shared<ZipDirRecord_>( aNode, std::move(stream), std::move(aPathFromAncestor), std::move(aAncestor) );
	}

	FLUX_THROW( error::VfsError )
		<< einfo::VfsRecordStat( nstat )
		<< einfo::VfsDesc( aNode->desc() )
		<< flux::einfo::ErrorDesc( "inode_zip_archive(): could not open stream for reading" )
		<< flux::einfo::ApiError( ec )
	;
}

shared_ptr<VINode> inode_zip_archive( unique_ptr<Stream> aStream, std::string aPathFromAncestor, weak_ptr<VINode> aAncestor )
{
	FLUX_ASSERT( aStream );

	auto const scaps = aStream->caps();
	FLUX_THROW_IF( !scaps.readable, error::VfsError )
		<< einfo::StreamDesc( aStream->description() )
		<< flux::einfo::ErrorDesc( "inode_zip_archive(): requires readable stream" )
	;
	FLUX_THROW_IF( !scaps.seekFwd || !scaps.seekRev, error::VfsError )
		<< einfo::StreamDesc( aStream->description() )
		<< flux::einfo::ErrorDesc( "inode_zip_archive(): requires fully seekable stream" )
	;
	FLUX_THROW_IF( !scaps.size, error::VfsError )
		<< einfo::StreamDesc( aStream->description() )
		<< flux::einfo::ErrorDesc( "inode_zip_archive(): requires stream with known size" )
	;

	return make_shared<ZipDirStream_>( std::move(aStream), std::move(aPathFromAncestor), std::move(aAncestor) );
}


//--    $ ZipDirBase_                           ///{{{2///////////////////////////
namespace
{
	// [cd]tor -{{{3-
	ZipDirBase_::ZipDirBase_( unique_ptr<Stream> aStream, std::string aPathFromAncestor, weak_ptr<VINode> aAncestor )
		: VINode( std::move(aPathFromAncestor), std::move(aAncestor) )
		, mStream( std::move(aStream) )
	{
		FLUX_ASSERT( mStream->caps().readable );
		FLUX_ASSERT( mStream->caps().seekFwd && mStream->caps().seekRev );
		FLUX_ASSERT( mStream->caps().size );

		init_();
	}

	ZipDirBase_::~ZipDirBase_()
	{
		mz_zip_reader_end( &mArch );
	}

	// virtuals: generic -{{{3-
#	if 0
	std::string ZipDirBase_::desc() const
	{
		return format( "zipdir:{}", mStream->description() );
	}

	bool ZipDirBase_::update( bool, compat::error_code& ) noexcept
	{
		auto const scaps = mStream->caps();
		mStat.removable = false;
		mStat.file = scaps.clonable;
		mStat.readable = scaps.clonable && scaps.readable;
		mStat.writable = false;//scaps.writable; //NOTE-file_as_stream() with write should truncate this file. Can't do with a Stream.
		mStat.mappable = false;
		mStat.sized = scaps.clonable && scaps.size;

		mStat.directory = true;
		mStat.accessible = true;
		mStat.listable = true;
		mStat.changeable = false;
		mStat.flat = false;

		mStat.special = false;
		mStat.hasNativeMap = false;
		mStat.hasNativePath = false;
		
		return true;
	}
#	endif

	// virtuals: file access -{{{3-
	VfsSize ZipDirBase_::file_size( compat::error_code& aErrorOut ) noexcept
	{
		FLUX_ASSERT( !aErrorOut );
		return mStream->size();
	}

	unique_ptr<Stream> ZipDirBase_::file_as_stream( EStreamMode aMode, compat::error_code& aErrorOut ) noexcept
	{
		if( EStreamMode::read == aMode && mStat.file )
			return mStream->clone();

		aErrorOut = compat::make_error_code( compat::errc::not_supported );
		return nullptr;
	}

	// virtuals: directory access -{{{3-
	optional<VfsStat> ZipDirBase_::dir_stat( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
	{
		if( auto def = dir_stat_defaults( aPath, false, nullptr, aErrorOut ) )
			return def.value();

		auto idx = lookup_( aPath );
		if( -1 == idx )
		{
			aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
			return {};
		}

		mz_zip_archive_file_stat sb;
		{
			std::lock_guard<std::recursive_mutex> lock(mMutex);
			if( !mz_zip_reader_file_stat( &mArch, idx, &sb ) )
			{
				aErrorOut = compat::make_error_code( compat::errc::io_error );
				return {};
			}
		}
		
		VfsStat stat;
		stat.removable = false;
		if( sb.m_is_directory )
		{
			stat.directory = true;
			stat.accessible = true;
			stat.changeable = false;
			stat.listable = false; //XXX
		}
		else
		{
			stat.file = true;
			stat.readable = true;
			stat.writable = false;
			stat.mappable = true;
			stat.sized = true;
		}

		return stat;
	}
	shared_ptr<VINode> ZipDirBase_::dir_open( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
	{
		std::string simple;
		if( auto def = dir_open_defaults( aPath, false, &simple, aErrorOut ) )
			return def.value();

		auto idx = lookup_( aPath );
		if( -1 == idx )
		{
			aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
			return {};
		}

		mz_zip_archive_file_stat sb;
		{
			std::lock_guard<std::recursive_mutex> lock(mMutex);
			if( !mz_zip_reader_file_stat( &mArch, idx, &sb ) )
			{
				aErrorOut = compat::make_error_code( compat::errc::io_error );
				return {};
			}
		}

		VfsStat stat;
		stat.removable = false;
		if( sb.m_is_directory )
		{
			stat.directory = true;
			stat.accessible = true;
			stat.changeable = false;
			stat.listable = false; //XXX
		}
		else
		{
			stat.file = true;
			stat.readable = true;
			stat.writable = false;
			stat.mappable = true;
			stat.sized = true;
		}

		VfsSize size = util::narrow<VfsSize>( sb.m_uncomp_size );

		return make_shared<ZipProxy_>( std::dynamic_pointer_cast<ZipDirBase_>(shared_from_this()), std::move(simple), stat, size );
	}

	unique_ptr<Buffer> ZipDirBase_::dir_open_buffer( compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept
	{
		if( auto def = dir_open_buffer_defaults( aPath, aMode, aMustExist, aResizeTo, false, nullptr, aErrorOut ) )
			return std::move(def.value());

		//TODO-BADNESS-maybe: record badness for buffering the contents.
		
		if( !!(EBufferMode::write & aMode) )
		{
			aErrorOut = compat::make_error_code( compat::errc::permission_denied );
			return nullptr;
		}
		
		if( auto ss = dir_open_stream( aPath, EStreamMode::read, aErrorOut ) )
			return buffer_create_stream( std::move(ss) );

		return nullptr;
	}
	unique_ptr<Stream> ZipDirBase_::dir_open_stream( compat::string_view const& aPath, EStreamMode aMode, compat::error_code& aErrorOut ) noexcept
	{
		if( auto def = dir_open_stream_defaults( aPath, aMode, false, nullptr, aErrorOut ) )
			return std::move(def.value());

		if( !!(EStreamMode::write & aMode) )
		{
			aErrorOut = compat::make_error_code( compat::errc::permission_denied );
			return nullptr;
		}

		bool dir = false;
		auto idx = lookup_( aPath, &dir );
		if( -1 == idx )
		{
			aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
			return {};
		}
		if( dir )
		{
			aErrorOut = compat::make_error_code( compat::errc::is_a_directory );
			return {};
		}

		mz_zip_reader_extract_iter_state* ptr = nullptr;
		{
			std::lock_guard<std::recursive_mutex> lock(mMutex);
			ptr = mz_zip_reader_extract_iter_new( &mArch, idx, 0 );
		}

		if( !ptr )
		{
			aErrorOut = compat::make_error_code( compat::errc::io_error );
			return nullptr;
		}
	
		return make_unique<ZipStream_>( ptr, std::dynamic_pointer_cast<ZipDirBase_>(shared_from_this()) );
	}

	void ZipDirBase_::dir_create( compat::string_view const& aPath, bool aRecursive )
	{
		if( dir_create_defaults( aPath, aRecursive ) )
			return;

		FLUX_THROW( error::VfsOpNotSupported )
			<< einfo::VfsRecordStat( stat() )
			<< einfo::VfsDesc( desc() )
			<< flux::einfo::ErrorMeta( "dir_create" )
		;
	}
	bool ZipDirBase_::dir_remove( compat::string_view const& aPath, bool aRecursive )
	{
		if( auto def = dir_remove_defaults( aPath, aRecursive ) )
			return def.value();

		FLUX_THROW( error::VfsOpNotSupported )
			<< einfo::VfsRecordStat( stat() )
			<< einfo::VfsDesc( desc() )
			<< flux::einfo::ErrorMeta( "dir_remove" )
		;
	}

	// private -{{{3-
	void ZipDirBase_::init_()
	{
		mz_zip_zero_struct( &mArch );
		
		mArch.m_pIO_opaque = static_cast<void*>(this);
		mArch.m_pRead = static_cast<mz_file_read_func>(
			[] (void* aOpaque, mz_uint64 aOfs, void* aBuf, size_t aN) -> size_t {
				auto self = static_cast<ZipDirBase_*>(aOpaque);

				std::lock_guard<std::recursive_mutex> lock(self->mMutex);
				self->mStream->seek( util::narrow<StreamOffset>(aOfs), ESeekWhence::set );
				return util::narrow<size_t>(self->mStream->read( aBuf, aN, EIoRequest::bestEffort ));
			}
		);
		
		auto sz = util::narrow<size_t>(mStream->size());
		if( !mz_zip_reader_init( &mArch, sz, MZ_ZIP_FLAG_CASE_SENSITIVE ) )
		{
			auto err = mz_zip_get_last_error( &mArch );
			FLUX_THROW( error::VfsError )
				<< einfo::StreamDesc( mStream->description() )
				<< flux::einfo::ApiFunction( "mz_zip_reader_init" )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("mz_error = {}"), mz_zip_get_error_string(err) )
			;
		}
	}
	int ZipDirBase_::lookup_( compat::string_view const& aPath, bool* aWasDir ) const noexcept
	{
		PathBuf_ path;
		path.reserve( aPath.size()+2 );
		path.insert( path.begin(), aPath.begin(), aPath.end() );
		path.push_back( '\0' );

		std::lock_guard<std::recursive_mutex> lock(mMutex);
		auto idx = mz_zip_reader_locate_file( &mArch, path.data(), nullptr, MZ_ZIP_FLAG_CASE_SENSITIVE );
		if( -1 == idx )
		{
			// Unfortunately ... for directories, mz_zip_reader expects the
			// name to end in a slash. So retry with that.
			path.back() = '/';
			path.push_back( '\0' );

			if( -1 != (idx = mz_zip_reader_locate_file( &mArch, path.data(), nullptr, MZ_ZIP_FLAG_CASE_SENSITIVE)) )
			{
				if( aWasDir ) *aWasDir = true;
			}
		}
		
		return idx;
	}
}

//--    $ ZipDirStream_                     ///{{{2///////////////////////////
namespace
{
	// [cd]tor -{{{3-
	ZipDirStream_::ZipDirStream_( unique_ptr<Stream> aStream, std::string aPathFromAncestor, weak_ptr<VINode> aAncestor )
		: ZipDirBase_( std::move(aStream), std::move(aPathFromAncestor), std::move(aAncestor) )
	{
		compat::error_code ec;
		ZipDirStream_::update( false, ec );
	}

	// virtuals: generic -{{{3-
	std::string ZipDirStream_::desc() const
	{
		return format( "zipdir:{}", mStream->description() );
	}
	bool ZipDirStream_::update( bool, compat::error_code& ) noexcept
	{
		auto const scaps = mStream->caps();
		mStat.removable = false;
		mStat.file = scaps.clonable;
		mStat.readable = scaps.clonable && scaps.readable;
		mStat.writable = false;//scaps.writable; //NOTE-file_as_stream() with write should truncate this file. Can't do with a Stream.
		mStat.mappable = false;
		mStat.sized = scaps.clonable && scaps.size;

		mStat.directory = true;
		mStat.accessible = true;
		mStat.listable = true;
		mStat.changeable = false;
		mStat.flat = false;

		mStat.special = false;
		mStat.hasNativeMap = false;
		mStat.hasNativePath = false;
		
		return true;
	}
}

//--    $ ZipDirRecord_                     ///{{{2///////////////////////////
namespace
{
	// [cd]tor -{{{3-
	ZipDirRecord_::ZipDirRecord_( shared_ptr<VINode> aFileNode, unique_ptr<Stream> aFileStream, std::string aPathFromAncestor, weak_ptr<VINode> aAncestor )
		: ZipDirBase_( std::move(aFileStream), std::move(aPathFromAncestor), std::move(aAncestor) )
		, mFileNode( std::move(aFileNode) )
	{
		compat::error_code ec;
		ZipDirRecord_::update( false, ec );
	}

	// virtuals: generic -{{{3-
	std::string ZipDirRecord_::desc() const
	{
		return format( "zipdir:{}", mFileNode->desc() );
	}
	bool ZipDirRecord_::update( bool aRecursive, compat::error_code& aErrorOut ) noexcept
	{
		if( !mFileNode->update( aRecursive, aErrorOut ) )
			return false;

		// "inherit" file-related stat fields from the underlying file node and
		// just override the directory related ones.
		//
		// also inherit special, hasNativeMap and hasNativePath
		mStat = mFileNode->stat();

		mStat.directory = true;
		mStat.accessible = true;
		mStat.listable = true;
		mStat.changeable = false;
		mStat.flat = false;

		return true;
	}
	bool ZipDirRecord_::remove( bool aRecursive, compat::error_code& aErrorOut ) noexcept
	{
		if( mFileNode->remove( aRecursive, aErrorOut ) )
		{
			compat::error_code ec;
			update( aRecursive, ec );
			return true;
		}

		return false;
	}

	optional<sys::SysPathStr> ZipDirRecord_::native_path( compat::error_code& aErrorOut ) const
	{
		return mFileNode->native_path( aErrorOut );
	}

	// virtuals: file access -{{{3-
	VfsSize ZipDirRecord_::file_size( compat::error_code& aErrorOut ) noexcept
	{
		return mFileNode->file_size( aErrorOut );
	}

	unique_ptr<Buffer> ZipDirRecord_::file_as_buffer( EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept
	{
		return mFileNode->file_as_buffer( aMode, aMustExist, aResizeTo, aErrorOut );
	}
	unique_ptr<Stream> ZipDirRecord_::file_as_stream( EStreamMode aMode, compat::error_code& aErrorOut ) noexcept
	{
		return mFileNode->file_as_stream( aMode, aErrorOut );
	}
}

//--    $ ZipProxy_                         ///{{{2///////////////////////////
namespace
{
	// [cd]tor -{{{3-
	ZipProxy_::ZipProxy_( shared_ptr<ZipDirBase_> aParent, std::string aPathFromAncestor, VfsStat aStat, VfsSize aSize )
		: VINode( std::move(aPathFromAncestor), aParent, aStat )
		, mParent( std::move(aParent) )
		, mFileSize( aSize )
	{}

	// virtuals: generic -{{{3-
	std::string ZipProxy_::desc() const
	{
		return format( "zipproxy:[{}]/{}", mParent->desc(), mPathFromAncestor );
	}

	bool ZipProxy_::update( bool, compat::error_code& ) noexcept
	{
		return true;
	}
	bool ZipProxy_::remove( bool aRecursive, compat::error_code& aCode ) noexcept
	{
		try
		{
			return mParent->dir_remove( mPathFromAncestor, aRecursive );
		}
		catch( error::VfsDenied const& )
		{
			aCode = compat::make_error_code( compat::errc::permission_denied );
		}
		catch( error::VfsOpNotSupported const& )
		{
			aCode = compat::make_error_code( compat::errc::not_supported );
		}
		catch( error::VfsError const& )
		{
			aCode = compat::make_error_code( compat::errc::io_error );
		}

		return false;
	}

	// virtuals: file access -{{{3-
	VfsSize ZipProxy_::file_size( compat::error_code& aErrorOut ) noexcept
	{
		FLUX_ASSERT( !aErrorOut );
		return mFileSize;
	}

	unique_ptr<Buffer> ZipProxy_::file_as_buffer( EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept
	{
		return mParent->dir_open_buffer( mPathFromAncestor, aMode, aMustExist, aResizeTo, aErrorOut );
	}
	unique_ptr<Stream> ZipProxy_::file_as_stream( EStreamMode aMode, compat::error_code& aErrorOut ) noexcept
	{
		return mParent->dir_open_stream( mPathFromAncestor, aMode, aErrorOut );
	}

	// virtuals: directory access -{{{3-
	optional<VfsStat> ZipProxy_::dir_stat( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
	{
		if( auto def = dir_stat_defaults( aPath, false, nullptr, aErrorOut ) )
			return def.value();

		return mParent->dir_stat( vpath_join(mPathFromAncestor,aPath), aErrorOut );
	}
	shared_ptr<VINode> ZipProxy_::dir_open( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
	{
		if( auto def = dir_open_defaults( aPath, false, nullptr, aErrorOut ) )
			return def.value();

		return mParent->dir_open( vpath_join(mPathFromAncestor,aPath), aErrorOut );
	}

	unique_ptr<Buffer> ZipProxy_::dir_open_buffer( compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept
	{
		if( auto def = dir_open_buffer_defaults( aPath, aMode, aMustExist, aResizeTo, false, nullptr, aErrorOut ) )
			return std::move(def.value());

		return mParent->dir_open_buffer( vpath_join(mPathFromAncestor,aPath), aMode, aMustExist, aResizeTo, aErrorOut );
	}
	unique_ptr<Stream> ZipProxy_::dir_open_stream( compat::string_view const& aPath, EStreamMode aMode, compat::error_code& aErrorOut ) noexcept
	{
		if( auto def = dir_open_stream_defaults( aPath, aMode, false, nullptr, aErrorOut ) )
			return std::move(def.value());

		return mParent->dir_open_stream( vpath_join(mPathFromAncestor,aPath), aMode, aErrorOut );
	}

	void ZipProxy_::dir_create( compat::string_view const& aPath, bool aRecursive )
	{
		if( dir_create_defaults( aPath, aRecursive ) )
			return;

		return mParent->dir_create( vpath_join(mPathFromAncestor,aPath), aRecursive );
	}
	bool ZipProxy_::dir_remove( compat::string_view const& aPath, bool aRecursive )
	{
		if( auto def = dir_remove_defaults( aPath, aRecursive ) )
			return def.value();

		return mParent->dir_remove( vpath_join(mPathFromAncestor,aPath), aRecursive );
	}
}

//--    $ ZipStream_                        ///{{{2///////////////////////////
namespace
{
	// [cd]tor -{{{3-
	ZipStream_::ZipStream_( mz_zip_reader_extract_iter_state* aState, shared_ptr<ZipDirBase_> aParent )
		: mState(aState)
		, mParent(std::move(aParent))
		, mReadable( true )
	{
		FLUX_ASSERT( mState && mParent );
		mRemaining = util::narrow<StreamSize>(mState->file_stat.m_uncomp_size);
	}
	ZipStream_::~ZipStream_()
	{
		FLUX_ASSERT( mState );
		mz_zip_reader_extract_iter_free( mState );
	}

	StreamSize ZipStream_::read( void* aBuffer, StreamSize aSize, EIoRequest aMode )
	{
		auto req = util::narrow<size_t>(aSize);
		auto res = size_t(0);
		auto buf = static_cast<compat::byte*>(aBuffer);
		
		do
		{
			auto const rr = mz_zip_reader_extract_iter_read( mState, buf+res, req-res );
			if( 0 == rr )
			{
				mReadable = false;
				break;
			}

			res += rr;
		} while( EIoRequest::mustComplete == aMode && res != req );
	
		mRemaining -= res;

		if( res != req )
		{
			//NOTE: could check mState->pZip for status, but that would race if
			//we use multiple threads (that may race either way...)
			
			FLUX_THROW_IF( EIoRequest::mustComplete == aMode, io::error::StreamIoIncomplete )
				<< einfo::StreamDesc( description() )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("read(mustComplete): requested {:d} bytes, got {:d}"), req, res )
			;
		}

		return util::narrow<StreamSize>(res);
	}

	EStreamStatus ZipStream_::status() const noexcept
	{
		return mReadable ? EStreamStatus::readable : EStreamStatus(0);
	}

	StreamCaps ZipStream_::caps() const noexcept
	{
		StreamCaps caps;
		caps.readable = true;
		caps.writable = false;
		caps.size = true;
		caps.tell = false;
		caps.seekFwd = false;
		caps.seekRev = false;
		caps.remaining = true;
		caps.infinite = false;
		caps.clonable = false;
		return caps;
	}

	StreamSize ZipStream_::size() const
	{
		return util::narrow<StreamSize>(mState->file_stat.m_uncomp_size);
	}
	StreamSize ZipStream_::remaining() const
	{
		return mRemaining;
	}

	std::string ZipStream_::description() const
	{
		return format( "zipfile:[{}]/{}", mParent->desc(), mState->file_stat.m_filename );
	}
	ftl::Any ZipStream_::native_handle() const
	{
		return ftl::Any(mState);
	}
}

//--    <<< ~ flux::io::vfsi namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
