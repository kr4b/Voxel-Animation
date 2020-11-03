/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	ZipWriter ArchiveWriter implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/util/archive.hpp>
#include <flux/io/buffer.hpp>
#include <flux/io/errors.hpp>
#include <flux/io/stream.hpp>
#include <flux/io/buffer/stream.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>

#include <flux/report/badness.hpp>
#include <flux/util/scope_exit.hpp>
#include <flux/compat/string_view.hpp>

#include <flux_ext/miniz.hpp>
#include <flux_ext/support/miniz_category.hpp>

#include <unordered_set>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io::util >>>          ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,util)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	class ZipWriter_ final : public ArchiveWriter
	{
		public:
			ZipWriter_( unique_ptr<Stream> aStream, int aDefaultLevel );
			~ZipWriter_( );

		public: /* virtuals from ArchiveWriter */
			void add_buffer( char const*, ftl::not_null<Buffer*>, EArchiveEntryFlags, BufferSize, BufferSize ) override;
			void add_stream( char const*, ftl::not_null<Stream*>, EArchiveEntryFlags, StreamSize ) override;
			void add_memory( char const*, void const*, std::size_t, EArchiveEntryFlags ) override;

		private:
			void add_directory_( compat::string_view const& );

		private:
			unique_ptr<Stream> mStream;
			mz_zip_archive mArchive;

			std::unordered_set<std::string> mKnownDirs;

			int mDefaultLevel;
	};
}

//--    archive_writer_create_zip()         ///{{{2///////////////////////////
unique_ptr<ArchiveWriter> archive_writer_create_zip( VfsRecord const& aRecord, char const* aName, int aDefaultLevel )
{
	FLUX_REPORT_UNIQUE_BADNESS_EX( offline, 5.f, "ArchiveWriter", "archive_writer_create_zip() is intended only for offline processing" );
	
	auto stream = vfs_open_stream( aRecord, aName, EStreamMode::write );
	return make_unique<ZipWriter_>( std::move(stream), aDefaultLevel );
}

//--    ArchiveWriter                       ///{{{2///////////////////////////

ArchiveWriter::~ArchiveWriter() = default;

//--    $ ZipWriter_                        ///{{{2///////////////////////////
namespace
{
	// [cd]tor -{{{3-
	ZipWriter_::ZipWriter_( unique_ptr<Stream> aStream, int aDefaultLevel )
		: mStream( std::move(aStream) )
		, mDefaultLevel( aDefaultLevel )
	{
		FLUX_ASSERT( mStream );
		FLUX_ASSERT( mStream->caps().writable );

		std::memset( &mArchive, 0, sizeof(mz_zip_archive) );
		mArchive.m_pIO_opaque = this;
		mArchive.m_pWrite = static_cast<mz_file_write_func>([] (void* aOpaque, mz_uint64 /*aOffs*/, void const* aData, size_t aCount) -> size_t {
			auto* self = static_cast<ZipWriter_*>(aOpaque);
			FLUX_ASSERT( self );
			
			if( aCount == 0 )
			{
				self->mStream->flush();
				return 0;
			}

			// aOffs will always be the previous aOffs + aCount. We can thus
			// ignore it.
			return self->mStream->write( aData, aCount, EIoRequest::mustComplete );
		});

		if( !mz_zip_writer_init( &mArchive, 0 ) )
		{
			using flux_ext::support::make_miniz_error_code;
			FLUX_THROW( error::IoError )
				<< flux::einfo::ErrorDesc( "ZipWriter_: failed to initialize zip archive" )
				<< flux::einfo::ApiFunction( "mz_zip_writer_init" )
				<< flux::einfo::ApiError( make_miniz_error_code(mArchive.m_last_error) )
			;
		}
	}

	ZipWriter_::~ZipWriter_()
	{
		if( !mz_zip_writer_finalize_archive( &mArchive ) )
		{
			//TODO: oops?
		}

		if( !mz_zip_writer_end( &mArchive ) )
		{
			//TODO: more oops?
		}
	}
	
	// virtuals: ArchiveWriter -{{{3-
	void ZipWriter_::add_buffer( char const* aEntryName, ftl::not_null<Buffer*> aBuffer, EArchiveEntryFlags aFlags, BufferSize aBegin, BufferSize aSize )
	{
		FLUX_ASSERT( aEntryName );
		auto map = aBuffer->map( EBufferMap::ro, aBegin, aSize );
		FLUX_UTIL_ON_SCOPE_EXIT
		{
			aBuffer->unmap( map );
		};
		
		add_memory( aEntryName, map.data(), map.size(), aFlags );
	}
	void ZipWriter_::add_stream( char const* aEntryName, ftl::not_null<Stream*> aStream, EArchiveEntryFlags aFlags, StreamSize aSize )
	{
		FLUX_ASSERT( aEntryName );
		auto buff = buffer_create_stream( aStream, aSize );
		add_buffer( aEntryName, buff.get(), aFlags, 0, buff->size() );
	}
	void ZipWriter_::add_memory( char const* aEntryName, void const* aPointer, std::size_t aBytes, EArchiveEntryFlags aFlags )
	{
		// are there any directories?
		compat::string_view name( aEntryName );
		auto dirpart = name.find_last_of( '/' );
		if( compat::string_view::npos != dirpart )
			add_directory_( name.substr( 0, dirpart ) );
		
		// add data
		int level = mDefaultLevel;
		if( !!(EArchiveEntryFlags::hintUncompressed & aFlags) ) 
			level = 0;
		else if( !!(EArchiveEntryFlags::hintCompressFast & aFlags) )
			level = MZ_BEST_SPEED;
		else if( !!(EArchiveEntryFlags::hintCompressGood & aFlags) )
			level = MZ_BEST_COMPRESSION;
		else if( !!(EArchiveEntryFlags::hintCompressBest & aFlags) )
			level = MZ_UBER_COMPRESSION;
		
		if( !mz_zip_writer_add_mem( &mArchive, aEntryName, aPointer, aBytes, level ) )
		{
			using flux_ext::support::make_miniz_error_code;
			FLUX_THROW( error::IoError )
				<< flux::einfo::ErrorDesc( "ZipWriter_: failed to add entry" )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("entry = '{}', size = {:d}"), aEntryName, aBytes )
				<< flux::einfo::ApiFunction( "mz_zip_writer_add_mem" )
				<< flux::einfo::ApiError( make_miniz_error_code(mArchive.m_last_error) )
			;
		}
	}

	// private -{{{3-
	void ZipWriter_::add_directory_( compat::string_view const& aDirName )
	{
		auto notslash = aDirName.find_last_not_of( '/' );
		if( compat::string_view::npos == notslash )
			return;
		
		auto dirname = aDirName.substr( 0, notslash+1 );
		auto dirstr = std::string(dirname);
		if( mKnownDirs.find(std::string(dirname)) != mKnownDirs.end() )
			return;
		
		auto dirpart = dirname.find_last_of( '/' );
		if( compat::string_view::npos != dirpart )
			add_directory_( dirname.substr( 0, dirpart ) );

		if( !mz_zip_writer_add_mem( &mArchive, (dirstr+"/").c_str(), nullptr, 0, MZ_DEFAULT_COMPRESSION ) )
		{
			using flux_ext::support::make_miniz_error_code;
			FLUX_THROW( error::IoError )
				<< flux::einfo::ErrorDesc( "ZipWriter_: failed to create directory entry" )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("entry = '{}/'"), dirstr )
				<< flux::einfo::ApiFunction( "mz_zip_writer_add_mem" )
				<< flux::einfo::ApiError( make_miniz_error_code(mArchive.m_last_error) )
			;
		}

		mKnownDirs.emplace( std::move(dirstr) );
	}
}

//--    <<< ~ flux::io::util namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,util)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
