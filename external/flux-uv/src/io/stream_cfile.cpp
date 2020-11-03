/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	C FILE* Stream implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/stream/cfile.hpp>
#include <flux/io/errors.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>

#include <flux/compat/byte.hpp>
#include <flux/compat/system_error.hpp>

#include <flux/util/narrow.hpp>
#include <flux/pp/stringize.hpp>

#include <flux/sys/syspath.hpp>
#include <flux/sys/pathutil.hpp>
#include <flux/sys/errno_category.hpp>

#include <algorithm>
#include <errno.h>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	class StreamCFILE_ final : public Stream
	{
		public:	
			StreamCFILE_( FILE*, EStreamMode, std::string );
			~StreamCFILE_();

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

		private:
			void check_mode_( EStreamMode ) const;

		private:
			FILE* mFile;

			EStreamMode mMode;
			std::string mDesc;
	};

#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
#		define FLUXTMP_SYS_OFFT_ __int64
#		define FLUXTMP_SYS_FTELLO_ ::_ftelli64
#		define FLUXTMP_SYS_FSEEKO_ ::_fseeki64

#	elif FLUX_PLATCAP_CYGWIN
#		define FLUXTMP_SYS_OFFT_ int
#		define FLUXTMP_SYS_FTELLO_ ::ftell
#		define FLUXTMP_SYS_FSEEKO_ ::fseek

#	else // others/defaults
#		define FLUXTMP_SYS_OFFT_ off_t
#		define FLUXTMP_SYS_FTELLO_ ::ftello
#		define FLUXTMP_SYS_FSEEKO_ ::fseeko

#	endif // ~ PLATFORM
}

//--    stream_create_cfile()               ///{{{2///////////////////////////

unique_ptr<Stream> stream_create_cfile( ftl::not_null<FILE*> aFile, EStreamMode aMode, std::string aDesc )
{
	if( aDesc.empty() )
		aDesc = format( "cfile@{:p}", static_cast<void*>(aFile) );

	return make_unique<StreamCFILE_>(
		static_cast<FILE*>(aFile),
		aMode,
		std::move(aDesc)
	);
}
unique_ptr<Stream> stream_create_cfile( char const* aPath, EStreamMode aMode )
{
	char mode[8];
	{
		char* modePtr = mode;

		if( !!(EStreamMode::write & aMode) )
		{
			if( EStreamMode::append == (EStreamMode::append & aMode) )
				*modePtr++ = 'a';
			else
				*modePtr++ = 'w';

			if( !!(EStreamMode::read & aMode) )
				*modePtr++ = '+';
		}
		else if( !!(EStreamMode::read & aMode) )
			*modePtr++ = 'r';

		if( !(EStreamMode::text & aMode) )
			*modePtr++ = 'b';

		*modePtr = '\0';

		FLUX_ASSERT( modePtr <= mode+sizeof(mode)/sizeof(mode[0]) );
	}

	FILE* cfile = std::fopen( aPath, mode );

	auto const errno_ = errno;
	FLUX_THROW_IF( !cfile, error::StreamNotAvailable )
		<< einfo::StreamMode( aMode )
		<< flux::einfo::ApiFunction( "std::fopen" )
		<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
		<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), aPath )
	;

	return make_unique<StreamCFILE_>(
		cfile,
		aMode,
		format( "cfile:{}", aPath )
	);
}

#if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
unique_ptr<Stream> stream_create_cfile( wchar_t const* aPath, EStreamMode aMode )
{
	wchar_t mode[8];
	{
		wchar_t* modePtr = mode;

		if( !!(EStreamMode::write & aMode) )
		{
			if( EStreamMode::append == (EStreamMode::append & aMode) )
				*modePtr++ = L'a';
			else
				*modePtr++ = L'w';

			if( !!(EStreamMode::read & aMode) )
				*modePtr++ = L'+';
		}
		else if( !!(EStreamMode::read & aMode) )
			*modePtr++ = L'r';

		if( !(EStreamMode::text & aMode) )
			*modePtr++ = L'b';

		*modePtr = L'\0';

		FLUX_ASSERT( modePtr < mode+sizeof(mode)/sizeof(mode[0])+1 );
	}

	FILE* cfile = _wfopen( aPath, mode );

	auto const errno_ = errno;
	FLUX_THROW_IF( !cfile, error::StreamNotAvailable )
		<< einfo::StreamMode( aMode )
		<< flux::einfo::ApiFunction( "_wfopen" )
		<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
		<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), sys::path_to_utf8(sys::path_from_wchar(aPath)) )
	;

	return make_unique<StreamCFILE_>(
		cfile,
		aMode,
		format( "cfile:{}", sys::path_to_utf8(sys::path_from_wchar(aPath)).c_str() )
	);
}
#endif // ~ PLATFORM_WIN32


//--    $ StreamCFILE_                      ///{{{2///////////////////////////
namespace
{
	StreamCFILE_::StreamCFILE_( FILE* aFile, EStreamMode aMode, std::string aDesc )
		: mFile( aFile )
		, mMode( aMode )
		, mDesc( aDesc )
	{}

	StreamCFILE_::~StreamCFILE_()
	{
		std::fclose( mFile );
	}


	StreamSize StreamCFILE_::read( void* aBuffer, StreamSize aSize, EIoRequest aReq )
	{
		FLUX_ASSERT( mFile && aBuffer );
		check_mode_( EStreamMode::read );

		auto req = util::narrow<std::size_t>(aSize);
		auto res = std::size_t(0);
		auto buf = static_cast<compat::byte*>(aBuffer);


		do
		{
			auto const rr = std::fread( buf+res, 1, req-res, mFile );
			if( 0 == rr )
				break;

			res += rr;
		} while( EIoRequest::mustComplete == aReq && res != req );

		if( res != req )
		{
			auto const errno_ = errno;
			FLUX_THROW_IF( std::ferror( mFile ), io::error::StreamSysError )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ApiFunction( "fread" )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			;
			
			FLUX_THROW_IF( EIoRequest::mustComplete == aReq, io::error::StreamIoIncomplete )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("read(mustComplete): requested {:d} bytes, got {:d}"), req, res )
			;
		}

		return util::narrow<StreamSize>(res);
	}
	StreamSize StreamCFILE_::write( void const* aBuffer, StreamSize aSize, EIoRequest aReq )
	{
		FLUX_ASSERT( mFile && aBuffer );
		check_mode_( EStreamMode::write );

		auto req = util::narrow<std::size_t>(aSize);
		auto res = std::size_t(0);
		auto buf = static_cast<compat::byte const*>(aBuffer);


		do
		{
			auto const rr = std::fwrite( buf+res, 1, req-res, mFile );
			if( 0 == rr )
				break;

			res += rr;
		} while( EIoRequest::mustComplete == aReq && res != req );

		if( res != req )
		{
			auto const errno_ = errno;
			FLUX_THROW_IF( std::ferror( mFile ), io::error::StreamSysError )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ApiFunction( "fwrite" )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			;
			
			FLUX_THROW_IF( EIoRequest::mustComplete == aReq, io::error::StreamIoIncomplete )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("write(mustComplete): specified {:d} bytes, wrote {:d}"), req, res )
			;
		}

		return util::narrow<StreamSize>(res);
	}

	EStreamStatus StreamCFILE_::status() const noexcept
	{
		if( std::ferror( mFile ) )
			return EStreamStatus::error;

		EStreamStatus status = EStreamStatus(0);
		
		if( !!(EStreamMode::write & mMode) )
			status |= EStreamStatus::writable;

		if( !!(EStreamMode::read & mMode) && !std::feof(mFile) )
			status |= EStreamStatus::readable;
	
		return status;
	}

	StreamCaps StreamCFILE_::caps() const noexcept
	{
		StreamCaps caps;
		caps.readable = !!(EStreamMode::read & mMode);
		caps.writable = !!(EStreamMode::write & mMode);
		caps.size = true;
		caps.tell = true;
		caps.seekFwd = true;
		caps.seekRev = true;
		caps.remaining = true;
		caps.infinite = false;
		caps.clonable = false;
		return caps;
	}

	StreamSize StreamCFILE_::size() const
	{
		FLUX_ASSERT( mFile );

		/* Warning: stupid amounts of error "handling" */

		auto const cur = FLUXTMP_SYS_FTELLO_( mFile );
		if( -1 == cur )
		{ 
			auto errno_ = errno;
			FLUX_THROW( io::error::StreamSysError )
				<< einfo::StreamMode( mMode )
				<< einfo::StreamDesc( mDesc )
				<< flux::einfo::ApiFunction( FLUX_PP_STRINGIZE(FLUXTMP_SYS_FTELLO_) )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			;
		}

		if( -1 == FLUXTMP_SYS_FSEEKO_( mFile, 0, SEEK_END ) )
		{
			auto errno_ = errno;
			FLUX_THROW( io::error::StreamSysError )
				<< einfo::StreamMode( mMode )
				<< einfo::StreamDesc( mDesc )
				<< flux::einfo::ApiFunction( FLUX_PP_STRINGIZE(FLUXTMP_SYS_FSEEKO_) )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			;
		}

		auto const res = FLUXTMP_SYS_FTELLO_( mFile );
		if( -1 == res )
		{ 
			auto errno_ = errno;
			FLUX_THROW( io::error::StreamSysError )
				<< einfo::StreamMode( mMode )
				<< einfo::StreamDesc( mDesc )
				<< flux::einfo::ApiFunction( FLUX_PP_STRINGIZE(FLUXTMP_SYS_FTELLO_) )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			;
		}

		if( -1 == FLUXTMP_SYS_FSEEKO_( mFile, cur, SEEK_SET ) )
		{
			auto errno_ = errno;
			FLUX_THROW( io::error::StreamSysError )
				<< einfo::StreamMode( mMode )
				<< einfo::StreamDesc( mDesc )
				<< flux::einfo::ApiFunction( FLUX_PP_STRINGIZE(FLUXTMP_SYS_FSEEKO_) )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			;
		}
		
		return util::narrow<StreamSize>(res);
	}

	StreamSize StreamCFILE_::tell() const
	{
		FLUX_ASSERT( mFile );
		auto const res = FLUXTMP_SYS_FTELLO_( mFile );
		if( -1 == res )
		{ 
			auto errno_ = errno;
			FLUX_THROW( io::error::StreamSysError )
				<< einfo::StreamMode( mMode )
				<< einfo::StreamDesc( mDesc )
				<< flux::einfo::ApiFunction( FLUX_PP_STRINGIZE(FLUXTMP_SYS_FTELLO_) )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			;
		}

		return util::narrow<StreamSize>(res);
	}
	StreamSize StreamCFILE_::seek( StreamOffset aOffs, ESeekWhence aWhence )
	{
		FLUX_ASSERT( mFile );

		int whence = 0;
		switch( aWhence )
		{
			case ESeekWhence::set: whence = SEEK_SET; break;
			case ESeekWhence::cur: whence = SEEK_CUR; break;
			case ESeekWhence::end: whence = SEEK_END; break;
		}

		auto const off = util::narrow<FLUXTMP_SYS_OFFT_>(aOffs);
		if( -1 == FLUXTMP_SYS_FSEEKO_( mFile, off, whence ) )
		{
			auto errno_ = errno;
			FLUX_THROW( io::error::StreamSysError )
				<< einfo::StreamMode( mMode )
				<< einfo::StreamDesc( mDesc )
				<< flux::einfo::ApiFunction( FLUX_PP_STRINGIZE(FLUXTMP_SYS_FSEEKO_) )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			;
		}
		
		return tell();
	}
	StreamSize StreamCFILE_::remaining() const
	{
		return size()-tell();
	}

	void StreamCFILE_::flush()
	{
		FLUX_ASSERT( mFile );
		std::fflush( mFile );
	}

	std::string StreamCFILE_::description() const
	{
		return mDesc;
	}
	ftl::Any StreamCFILE_::native_handle() const
	{
		return ftl::Any( mFile );
	}


	void StreamCFILE_::check_mode_( EStreamMode aRequired ) const
	{
		FLUX_THROW_IF( aRequired != (aRequired & mMode), error::StreamOpNotSupported )
			<< einfo::StreamMode( mMode )
			<< einfo::StreamDesc( mDesc )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("required mode: {}"), to_string(aRequired) )
		;
	}
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
