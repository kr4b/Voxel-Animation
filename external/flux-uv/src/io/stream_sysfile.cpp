/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	System native Stream implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/stream/sysfile.hpp>
#include <flux/io/errors.hpp>

#include <flux/std/log.hpp>
#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>

#include <flux/sys/defpath.hpp>
#include <flux/sys/pathutil.hpp>
#include <flux/sys/errno_category.hpp>

#include <flux/util/intent.hpp>
#include <flux/util/narrow.hpp>

#include <flux/compat/byte.hpp>

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#	include <fcntl.h>
#	include <errno.h>
#	include <unistd.h>

#	include <sys/stat.h>
#	include <sys/types.h>
#else // PLATFORM_WIN32
#	include <flux/sys/win32/windows.h>
#	include <flux/sys/win32_category.hpp>
#endif // ~ PLATFORM

#include "pathbuf.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	using SysHandle_ = int;
	mode_t getumask_() noexcept;
#	else // PLATFORM_WIN32
	using SysHandle_ = HANDLE;
#	endif // ~ PLATFORM

	class StreamSysfile_ final : public Stream
	{
		public:	
			StreamSysfile_( SysHandle_, EStreamMode, std::string );
			~StreamSysfile_();

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
			bool valid_() const;
			void check_mode_( EStreamMode ) const;

		private:
			SysHandle_ mHandle;

			EStreamMode mMode;
			std::string mDesc;

			bool mEof;
	};
}

//--    stream_create_sysfile()             ///{{{2///////////////////////////
unique_ptr<Stream> stream_create_sysfile( sys::SysPathView const& aPath, EStreamMode aMode, std::string aDesc )
{
	if( aDesc.empty() )
		aDesc = format( "sysfile:{}", sys::path_to_generic(aPath) );

	SysHandle_ handle = [&] {
		auto const rd = (EStreamMode::read == (EStreamMode::read & aMode));
		auto const wr = (EStreamMode::write == (EStreamMode::write & aMode));
		
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		int flags = O_LARGEFILE;

		if( rd && wr )
			flags |= O_RDWR;
		else if( rd )
			flags |= O_RDONLY;
		else if( wr )
			flags |= O_WRONLY;
		
		if( EStreamMode::append == (EStreamMode::append & aMode) )
			flags |= O_APPEND;
		else if( wr )
			flags |= O_TRUNC;

		if( wr )
			flags |= O_CREAT;

		detail::SysPathBuf buf;
		detail::make_zpath( buf, aPath );
		int fd = ::open( buf.data(), flags, DEFFILEMODE & ~getumask_() );

		int errno_ = errno;
		FLUX_THROW_IF( -1 == fd, error::StreamNotAvailable )
			<< einfo::StreamMode( aMode )
			<< flux::einfo::ApiFunction( "::open" )
			<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), aPath )
		;

		return fd;
#		else // PLATFORM_WIN32
		DWORD mode = 0;
		if( rd )
			mode |= GENERIC_READ;
		if( wr )
			mode |= GENERIC_WRITE;

		DWORD share = FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE;
		
		DWORD disp = OPEN_EXISTING;
		if( EStreamMode::append == (EStreamMode::append & aMode) )
			disp = OPEN_ALWAYS;
		else if( wr )
			disp = CREATE_ALWAYS;

		detail::SysPathBuf buf;
		detail::make_zpath( buf, aPath );
		auto hh = ::CreateFileW( buf.data(), mode, share, nullptr, disp, FILE_ATTRIBUTE_NORMAL, nullptr );

		auto err = ::GetLastError();
		FLUX_THROW_IF( INVALID_HANDLE_VALUE == hh, error::StreamNotAvailable )
			<< einfo::StreamMode( aMode )
			<< flux::einfo::ApiFunction( "::CreateFileW" )
			<< flux::einfo::ApiError( sys::make_win32_error_code(err) )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), sys::path_to_utf8(aPath) )
		;

		if( EStreamMode::append == (EStreamMode::append & aMode) )
		{
			LARGE_INTEGER li = {0};
			if( !::SetFilePointerEx( hh, li, nullptr, FILE_END ) )
			{
				auto err = ::GetLastError();
				FLUX_THROW( error::StreamNotAvailable )
					<< einfo::StreamMode( aMode )
					<< flux::einfo::ApiFunction( "::SetFilePointer" )
					<< flux::einfo::ApiError( sys::make_win32_error_code(err) )
					<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), sys::path_to_utf8(aPath) )
				;
			}
		}

		return hh;
#		endif // ~ PLATFORM
	}();

	return make_unique<StreamSysfile_>(
		handle,
		aMode,
		std::move(aDesc)
	);
}

//--    stream_create_sysfile()             ///{{{2///////////////////////////
unique_ptr<Stream> stream_create_systemp()
{
	SysHandle_ handle = [&] {
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		static constexpr sys::SysPathChar const kTempl_[] = "/fluxtmp-XXXXXX";
		auto const tempdir = sys::path_get_tempdir();

		detail::SysPathBuf buf( tempdir.size() + sizeof(kTempl_) );
		std::memcpy( buf.data(), tempdir.data(), tempdir.size() );
		std::memcpy( buf.data()+tempdir.size(), kTempl_, sizeof(kTempl_) );

		/* mkstemp() mentions that the application should ensure that "its file
		 * node creation mask  [...] is set appropriately before callling
		 * mkstemp()". Not setting the umask further seems to trigger an
		 * abort() on older glibc versions. (On the other hand, setting and
		 * resetting the umask is probably a race-condition within the
		 * process.)
		 */
		auto old = ::umask( ~0600 );
		int fd = ::mkstemp( buf.data() );
		::umask( old ); 

		int errno_ = errno;
		FLUX_THROW_IF( -1 == fd, error::StreamNotAvailable )
			<< einfo::StreamMode( EStreamMode::read|EStreamMode::write )
			<< flux::einfo::ApiFunction( "::mkstemp" )
			<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("tempfile in '{}'"), tempdir )
		;

		if( -1 == ::unlink( buf.data() ) )
		{
			int errno_ = errno;
			FLUX_LOG( ERROR, "Can't remove temporary file '{}'; errno = {:d} ({})", buf.data(), errno_, sys::make_errno_error_code(errno_).message() );
		}

		return fd;
#		else // PLATFORM_WIN32
		/* Based on
		 * https://docs.microsoft.com/en-us/windows/win32/fileio/creating-and-using-a-temporary-file
		 */

		DWORD mode = GENERIC_READ|GENERIC_WRITE;;
		DWORD share = FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE;
		DWORD disp = CREATE_ALWAYS;
		DWORD flags = FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE;

		static constexpr std::size_t kMaxPath_ = 260;
		auto const tempdir = sys::path_get_tempdir();

		sys::SysPathChar buf[kMaxPath_];
		auto rr = ::GetTempFileNameW( 
			tempdir.data(),
			L"fluxtmp-",
			0,
			buf
		);
		
		auto hh = ::CreateFileW( buf, mode, share, nullptr, disp, flags, nullptr );

		auto err = ::GetLastError();
		FLUX_THROW_IF( INVALID_HANDLE_VALUE == hh, error::StreamNotAvailable )
			<< einfo::StreamMode( EStreamMode::read|EStreamMode::write )
			<< flux::einfo::ApiFunction( "::CreateFileW" )
			<< flux::einfo::ApiError( sys::make_win32_error_code(err) )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), sys::path_to_utf8(buf) )
		;

		return hh;
#		endif // ~ PLATFORM
	}();

	return make_unique<StreamSysfile_>(
		handle,
		 EStreamMode::read|EStreamMode::write,
		 "systempfile"
	);
}

//--    $ StreamSysfile_                    ///{{{2///////////////////////////
namespace
{
	StreamSysfile_::StreamSysfile_( SysHandle_ aHandle, EStreamMode aMode, std::string aDesc )
		: mHandle( aHandle )
		, mMode( aMode )
		, mDesc( aDesc )
		, mEof( false )
	{}

	StreamSysfile_::~StreamSysfile_()
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		::close( mHandle );
#		else // PLATFORM_WIN32
		::CloseHandle( mHandle );
#		endif // ~ PLATFORM
	}


	StreamSize StreamSysfile_::read( void* aBuffer, StreamSize aSize, EIoRequest aReq )
	{
		FLUX_ASSERT( valid_() && aBuffer );
		check_mode_( EStreamMode::read );

#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		auto req = util::narrow<std::size_t>(aSize);
		auto res = std::size_t(0);
		auto buf = static_cast<compat::byte*>(aBuffer);

		do
		{
			auto const rr = ::read( mHandle, buf+res, req-res );
			if( -1 == rr )
			{
				auto const errno_ = errno;
				FLUX_THROW( error::StreamSysError )
					<< einfo::StreamDesc( mDesc )
					<< einfo::StreamMode( mMode )
					<< flux::einfo::ApiFunction( "::read" )
					<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
				;
			}

			if( 0 == rr )
			{
				mEof = true;
				break;
			}

			res += rr;
		} while( EIoRequest::mustComplete == aReq && res != req );

		if( res != req )
		{
			FLUX_THROW_IF( EIoRequest::mustComplete == aReq, io::error::StreamIoIncomplete )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("read(mustComplete): requested {:d} bytes, got {:d}"), req, res )
			;
		}
#		else // PLATFORM_WIN32
		auto req = util::narrow<DWORD>(aSize);
		auto res = DWORD(0);
		auto buf = static_cast<compat::byte*>(aBuffer);

		do
		{
			DWORD rr = 0;
			if( !::ReadFile( mHandle, buf+res, req-res, &rr, nullptr ) )
			{
				auto err = ::GetLastError();
				FLUX_THROW( error::StreamSysError )
					<< einfo::StreamDesc( mDesc )
					<< einfo::StreamMode( mMode )
					<< flux::einfo::ApiFunction( "::ReadFile" )
					<< flux::einfo::ApiError( sys::make_win32_error_code(err) )
				;
			}
			
			if( 0 == rr )
			{
				mEof = true;
				break;
			}

			res += rr;
		} while( EIoRequest::mustComplete == aReq && res != req );

		if( res != req )
		{
			FLUX_THROW_IF( EIoRequest::mustComplete == aReq, io::error::StreamIoIncomplete )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("read(mustComplete): requested {:d} bytes, got {:d}"), req, res )
			;
		}
#		endif // ~ PLATFORM

		return util::narrow<StreamSize>(res);
	}
	StreamSize StreamSysfile_::write( void const* aBuffer, StreamSize aSize, EIoRequest aReq )
	{
		FLUX_ASSERT( valid_() && aBuffer );
		check_mode_( EStreamMode::write );

#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		auto req = util::narrow<std::size_t>(aSize);
		auto res = std::size_t(0);
		auto buf = static_cast<compat::byte const*>(aBuffer);

		do
		{
			auto const rr = ::write( mHandle, buf+res, req-res );
			if( -1 == rr )
			{
				auto const errno_ = errno;
				FLUX_THROW( error::StreamSysError )
					<< einfo::StreamDesc( mDesc )
					<< einfo::StreamMode( mMode )
					<< flux::einfo::ApiFunction( "::write" )
					<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
				;
			}

			if( 0 == rr )
				break;

			res += rr;
		} while( EIoRequest::mustComplete == aReq && res != req );

		if( res != req )
		{
			FLUX_THROW_IF( EIoRequest::mustComplete == aReq, io::error::StreamIoIncomplete )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("write(mustComplete): specified {:d} bytes, wrote {:d}"), req, res )
			;
		}

#		else // PLATFORM_WIN32
		auto req = util::narrow<DWORD>(aSize);
		auto res = DWORD(0);
		auto buf = static_cast<compat::byte const*>(aBuffer);

		do
		{
			DWORD rr;
			if( !::WriteFile( mHandle, buf+res, req-res, &rr, nullptr ) )
			{
				auto const err = ::GetLastError();
				FLUX_THROW( error::StreamSysError )
					<< einfo::StreamDesc( mDesc )
					<< einfo::StreamMode( mMode )
					<< flux::einfo::ApiFunction( "::WriteFile" )
					<< flux::einfo::ApiError( sys::make_win32_error_code(err) )
				;
			}

			if( 0 == rr )
				break;

			res += rr;
		} while( EIoRequest::mustComplete == aReq && res != req );

		if( res != req )
		{
			FLUX_THROW_IF( EIoRequest::mustComplete == aReq, io::error::StreamIoIncomplete )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("write(mustComplete): specified {:d} bytes, wrote {:d}"), req, res )
			;
		}
#		endif // ~ PLATFORM

		return util::narrow<StreamSize>(res);
	}

	EStreamStatus StreamSysfile_::status() const noexcept
	{
		EStreamStatus status = EStreamStatus(0);
		
		if( !!(EStreamMode::write & mMode) )
			status |= EStreamStatus::writable;

		if( !!(EStreamMode::read & mMode) && !mEof )
			status |= EStreamStatus::readable;
	
		return status;
	}

	StreamCaps StreamSysfile_::caps() const noexcept
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
		caps.clonable = true;
		return caps;
	}

	StreamSize StreamSysfile_::size() const
	{
		FLUX_ASSERT( valid_() );

#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		struct stat64 sb;
		if( -1 == ::fstat64( mHandle, &sb ) )
		{
			int errno_ = errno;
			FLUX_THROW( error::StreamSysError )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ApiFunction( "::fstat64" )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			;
		}

		return util::narrow<StreamSize>(sb.st_size);
#		else // PLATFORM_WIN32
		LARGE_INTEGER li;
		if( !::GetFileSizeEx( mHandle, &li ) )
		{
			auto err = ::GetLastError();
			FLUX_THROW( error::StreamSysError )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ApiFunction( "::GetFileSizeEx" )
				<< flux::einfo::ApiError( sys::make_win32_error_code(err) )
			;
		}

		return util::narrow<StreamSize>(li.QuadPart);
#		endif // ~ PLATFORM
	}

	StreamSize StreamSysfile_::tell() const
	{
		FLUX_ASSERT( valid_() );

#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		auto ret = ::lseek64( mHandle, 0, SEEK_CUR );
		if( -1 == ret )
		{
			int errno_ = errno;
			FLUX_THROW( error::StreamSysError )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ApiFunction( "::lseek64" )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			;
		}

		return util::narrow<StreamSize>(ret);
#		else // PLATFORM_WIN32
		LARGE_INTEGER li, zero = {0};
		if( !::SetFilePointerEx( mHandle, zero, &li, FILE_CURRENT ) )
		{
			auto err = ::GetLastError();
			FLUX_THROW( error::StreamSysError )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ApiFunction( "::SetFilePointerEx" )
				<< flux::einfo::ApiError( sys::make_win32_error_code(err) )
			;
		}

		return util::narrow<StreamSize>(li.QuadPart);
#		endif // ~ PLATFORM
	}
	StreamSize StreamSysfile_::seek( StreamOffset aOffs, ESeekWhence aWhence )
	{
		FLUX_ASSERT( valid_() );

		mEof = false;

#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		auto off = util::narrow<off64_t>(aOffs);
		auto whence = [&] {
			switch(aWhence) 
			{
				case ESeekWhence::set: return SEEK_SET;
				case ESeekWhence::cur: return SEEK_CUR;
				case ESeekWhence::end: return SEEK_END;
			}

			FLUX_INTENT_UNREACHABLE();
		}();
		
		auto ret = ::lseek64( mHandle, off, whence );
		if( -1 == ret )
		{
			int errno_ = errno;
			FLUX_THROW( error::StreamSysError )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ApiFunction( "::lseek64" )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			;
		}

		return util::narrow<StreamSize>(ret);
#		else // PLATFORM_WIN32
		LARGE_INTEGER li, ret;
		li.QuadPart = util::narrow<decltype(li.QuadPart)>(aOffs);
		auto whence = [&] {
			switch(aWhence) 
			{
				case ESeekWhence::set: return FILE_BEGIN;
				case ESeekWhence::cur: return FILE_CURRENT;
				case ESeekWhence::end: return FILE_END;
			}

			FLUX_INTENT_UNREACHABLE();
		}();
		
		if( !::SetFilePointerEx( mHandle, li, &ret, whence ) )
		{
			auto err = ::GetLastError();
			FLUX_THROW( error::StreamSysError )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ApiFunction( "::SetFilePointerEx" )
				<< flux::einfo::ApiError( sys::make_win32_error_code(err) )
			;
		}

		return util::narrow<StreamSize>(ret.QuadPart);
#		endif // ~ PLATFORM
	}
	StreamSize StreamSysfile_::remaining() const
	{
		return size()-tell();
	}

	void StreamSysfile_::flush()
	{
		FLUX_ASSERT( valid_() );
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		if( -1 == ::fdatasync( mHandle ) )
		{
			int errno_ = errno;
			FLUX_THROW( error::StreamSysError )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ApiFunction( "::fdatasync" )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			;
		}
#		else // PLATFORM_WIN32
		if( !::FlushFileBuffers( mHandle ) )
		{
			auto err = ::GetLastError();
			FLUX_THROW( error::StreamSysError )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ApiFunction( "::FlushFileBuffers" )
				<< flux::einfo::ApiError( sys::make_win32_error_code(err) )
			;
		}
#		endif // ~ PLATFORM
	}

	std::string StreamSysfile_::description() const
	{
		return mDesc;
	}
	ftl::Any StreamSysfile_::native_handle() const
	{
		return ftl::Any( mHandle );
	}

	unique_ptr<Stream> StreamSysfile_::clone() const
	{
		FLUX_ASSERT( valid_() );

		auto const rd = (EStreamMode::read == (EStreamMode::read & mMode));
		auto const wr = (EStreamMode::write == (EStreamMode::write & mMode));

#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		int flags = O_LARGEFILE;

		if( rd && wr )
			flags |= O_RDWR;
		else if( rd )
			flags |= O_RDONLY;
		else if( wr )
			flags |= O_WRONLY;
		
		if( EStreamMode::append == (EStreamMode::append & mMode) )
			flags |= O_APPEND;
		
		int fd = ::open( format("/proc/self/fd/{:d}", mHandle).c_str(), flags );

		int errno_ = errno;
		FLUX_THROW_IF( -1 == fd, error::StreamNotAvailable )
			<< einfo::StreamMode( mMode )
			<< flux::einfo::ApiFunction( "::open" )
			<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '/proc/self/fd/{:d}'"), mHandle )
		;

		return make_unique<StreamSysfile_>( fd, mMode, description() );
#		else // PLATFORM_WIN32
		DWORD mode = 0;
		if( rd )
			mode |= GENERIC_READ;
		if( wr )
			mode |= GENERIC_WRITE;

		DWORD share = FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE;

		auto hh = ::ReOpenFile( mHandle, mode, share, 0 );

		auto err = ::GetLastError();
		FLUX_THROW_IF( INVALID_HANDLE_VALUE == hh, error::StreamNotAvailable )
			<< einfo::StreamMode( mMode )
			<< flux::einfo::ApiFunction( "::ReOpenFile" )
			<< flux::einfo::ApiError( sys::make_win32_error_code(err) )
		;

		if( EStreamMode::append == (EStreamMode::append & mMode) )
		{
			LARGE_INTEGER li = {0};
			if( !::SetFilePointerEx( hh, li, nullptr, FILE_END ) )
			{
				auto err = ::GetLastError();
				FLUX_THROW( error::StreamNotAvailable )
					<< einfo::StreamMode( mMode )
					<< flux::einfo::ApiFunction( "::SetFilePointerEx" )
					<< flux::einfo::ApiError( sys::make_win32_error_code(err) )
				;
			}
		}

		return make_unique<StreamSysfile_>( hh, mMode, description() );
#		endif // ~ PLATFORM
	}

	bool StreamSysfile_::valid_() const
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		return -1 != mHandle;
#		else // PLATFORM_WIN32
		return INVALID_HANDLE_VALUE != mHandle;
#		endif // ~ PLATFORM
	}
	void StreamSysfile_::check_mode_( EStreamMode aRequired ) const
	{
		FLUX_THROW_IF( aRequired != (aRequired & mMode), error::StreamOpNotSupported )
			<< einfo::StreamMode( mMode )
			<< einfo::StreamDesc( mDesc )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("required mode: {}"), to_string(aRequired) )
		;
	}
}

//--    $ getumask_()                       ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	mode_t getumask_() noexcept
	{
		/* getumask(): "This function is documented but not implemented yet in
		 * glibc, as at version 2.9". It's equivalent to the implementation
		 * given here, minus the race condition if somebody else is modifying
		 * the umask of the process concurrently.
		 *
		 * Oh, well.
		 */
		mode_t ret = umask(0);
		umask(ret);
		return ret;
	}
#	endif // ~ !PLATFORM_WIN32
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
