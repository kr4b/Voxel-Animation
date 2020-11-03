/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	`errno` error category
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/win32_category.hpp>

#include <flux/std/format.hpp>
#include <flux/setup/platform.hpp>

#include <flux/util/switch.hpp>
#include <flux/util/scope_exit.hpp>

#include <flux_dlapi/os/win32.hpp>

//#if FLUX_PLATCAP_WIN32LIB
//#	include <flux/sys/win32/windows.h>
//#endif // ~ WIN32LIB

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	struct Win32Category_ final : compat::error_category
	{
		char const* name() const noexcept override;
		std::string message( int ) const override;
		compat::error_condition default_error_condition( int ) const noexcept override;
	};

	bool win32_to_errc_( int, compat::errc& );

	dlapi::os::Win32api const* win32_() noexcept;
}

//--    win32_category()                    ///{{{2///////////////////////////
compat::error_category const& win32_category() noexcept
{
	static Win32Category_ cat;
	return cat;
}

//--    make_win32_error_code()             ///{{{2///////////////////////////
compat::error_code make_win32_error_code() noexcept
{
	auto const* w32 = win32_();
	return make_win32_error_code( w32 ? w32->getLastError() : 0 );
#if 0
#	if FLUX_PLATCAP_WIN32LIB
	return make_win32_error_code( ::GetLastError() );
#	else
	return make_win32_error_code( 0 );
#	endif // ~ WIN32LIB
#endif
}

compat::error_code make_win32_error_code( unsigned long aCode ) noexcept
{
	return compat::error_code( int(aCode), win32_category() );
}

//--    l: Win32Category_                   ///{{{2///////////////////////////
namespace
{
	char const* Win32Category_::name() const noexcept
	{
		return "Win32Error";
	}

	std::string Win32Category_::message( int aValue ) const
	{
		namespace W32 = dlapi::os::Win32;
		if( auto const* w32 = win32_() )
		{
			W32::LPSTR message = nullptr;
			W32::DWORD ret = w32->formatMessageA(
				W32::format_message_from_system | W32::format_message_allocate_buffer | W32::format_message_ignore_inserts,
				0,
				W32::DWORD(aValue),
				W32::makelangid( W32::lang_neutral, W32::sublang_default ),
				(W32::LPSTR)&message,
				0,
				0
			);

			if( 0 == ret || !message )
			{
				return format( "<FormatMessageA() returned additional error {:#x} when attempting to resolve message for original error {:#x}", w32->getLastError(), aValue );
			}

			FLUX_UTIL_ON_SCOPE_EXIT
			{
				w32->localFree( message );
			};

			if( message[ret-1] == '\n' )
				--ret;

			return std::string( message, message+ret );
		}

		if( 0 == aValue )
			return "(success)";

		return format( "Win32-error {:#x}", aValue );

#	if 0
#		if FLUX_PLATCAP_WIN32LIB
		LPSTR message = nullptr;
		DWORD ret = ::FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
			0,
			DWORD(aValue),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&message,
			0,
			0
		);

		if( 0 == ret || !message )
		{
			return format( "<FormatMessageA() returned additional error {:#x} when attempting to resolve message for original erro {:#x}", ::GetLastError(), aValue );
		}

		FLUX_UTIL_ON_SCOPE_EXIT
		{
			::LocalFree( message );
		};

		if( message[ret-1] == '\n' )
			--ret;

		return std::string( message, message+ret );
#		else
		if( 0 == aValue )
			return "(success)";

		return format( "Win32-error {:#x}", aValue );
#		endif // ~ WIN32LIB
#	endif
	};

	compat::error_condition Win32Category_::default_error_condition( int aValue ) const noexcept
	{
		if( 0 == aValue ) 
			return compat::error_condition( 0, compat::generic_category() );

		compat::errc ec;
		if( win32_to_errc_( aValue, ec ) )
			return compat::error_condition( ec );

		return compat::error_condition( aValue, *this );
	}
}


//--    l: win32_to_errc_()                 ///{{{2///////////////////////////
namespace
{
	bool win32_to_errc_( int aErrno, compat::errc& aOut )
	{
		using compat::errc;

		switch( aErrno )
		{
			// See https://msdn.microsoft.com/en-us/library/cc231199.aspx
			case 0x0001 /*ERROR_INVALID_FUNCTION*/:
				aOut = errc::function_not_supported;
				break;
			case 0x0002 /*ERROR_FILE_NOT_FOUND*/: FLUX_SWITCH_FALLTHROUGH();
			case 0x0003 /*ERROR_PATH_NOT_FOUND*/:
				aOut = errc::no_such_file_or_directory;
				break;
			case 0x0004 /*ERROR_TOO_MANY_OPEN_FILES*/:
				aOut = errc::too_many_files_open;
				break;
			case 0x0005 /*ERROR_ACCESS_DENIED*/:
				aOut = errc::permission_denied;
				break;
			case 0x0006 /*ERROR_INVALID_HANDLE*/:
				aOut = errc::bad_file_descriptor;
				break;
			// 0x0007 ERROR_ARENA_TRASHED not translated
			case 0x0008 /*ERROR_NOT_ENOUGH_MEMORY*/:
				aOut = errc::not_enough_memory;
				break;
			// 0x0009 ERROR_INVALID_BLOCK not translated
			// 0x000A ERROR_BAD_ENVIRONMENT not translated
			case 0x000B /*ERROR_BAD_FORMAT*/:
				aOut = errc::executable_format_error;
				break;
			// 0x000C ERROR_INVALID_ACCESS not translated 
			// 0x000D ERROR_INVALID_DATA not translated
			case 0x000E /*ERROR_OUTOFMEMORY*/:
				aOut = errc::not_enough_memory;
				break;
			case 0x000F /*ERROR_INVALID_DRIVE*/:
				aOut = errc::no_such_device;
				break;
			case 0x0010 /*ERROR_CURRENT_DIRECTORY*/:
				aOut = errc::device_or_resource_busy;
				break;
			// 0x0011 ERROR_NOT_SAME_DEVICE not translated
			// 0x0012 ERROR_NO_MORE_FILES not translated
			case 0x0013 /*ERROR_WRITE_PROTECT*/:
				aOut = errc::read_only_file_system;
				break;
			case 0x0014 /*ERROR_BAD_UNIT*/: // "The system cannot find the device ..."
				aOut = errc::no_such_device; 
				break;
			case 0x0015 /*ERROR_NOT_READY*/:
				aOut = errc::device_or_resource_busy;
				break;
			case 0x0016 /*ERROR_BAD_COMMAND*/:
				aOut = errc::not_supported;
				break;
			// 0x0017 ERROR_CRC not translated
			case 0x0018 /*ERROR_BAD_LENGTH*/:
				aOut = errc::invalid_argument;
				break;
			case 0x0019 /*ERROR_SEEK*/:
				aOut = errc::invalid_seek;
				break;
			// 0x001A ERROR_NOT_DOS_DISK not translated
			// 0x001B ERROR_SECTOR_NOT_FOUND not translated
			// 0x001C ERROR_OUT_OF_PAPER not translated
			case 0x001D /*ERROR_WRITE_FAULT*/: FLUX_SWITCH_FALLTHROUGH();
			case 0x001E /*ERROR_READ_FAULT*/:
				aOut = errc::io_error;
				break;
			// 0x001F ERROR_GEN_FAILURE not translated
			case 0x0020 /*ERROR_SHARING_VIOLATION*/: FLUX_SWITCH_FALLTHROUGH();
			case 0x0021 /*ERROR_LOCK_VIOLATION*/:
				aOut = errc::device_or_resource_busy;
				break;
			// 0x0022 ERROR_WRONG_DISK not translated
			// 0x0023 does not exist
			case 0x0024 /*ERROR_SHARING_BUFFER_EXCEEDED*/:
				aOut = errc::too_many_files_open;
				break;
			// 0x0025 does not exist
			// 0x0026 ERROR_HANDLE_EOF not translated
			case 0x0027 /*ERROR_HANDLE_DISK_FULL*/:
				aOut = errc::no_space_on_device;
				break;

			// ... several codes unused/undocumented ...

			case 0x0032 /*ERROR_NOT_SUPPORTED*/:
				aOut = errc::not_supported;
				break;
			// 0x0033 ERROR_REM_NOT_LIST not translated
			// 0x0034 ERROR_DUP_NAME not translated
			// 0x0035 ERROR_BAD_NETPATH not translated
			// 0x0036 ERROR_NETWORK_BUSY not translated
			case 0x0037 /*ERROR_DEV_NOT_EXIST*/:
				aOut = errc::no_such_device_or_address;
				break;
			// 0x0038 ERROR_TOO_MANY_CMDS not translated

			//TODO-maybe: several omitted; a few selected ones are listed
			// from here on.

			case 0x006D /*ERROR_BROKEN_PIPE*/:
				aOut = errc::broken_pipe;
				break;

			case 0x0070 /*ERROR_DISK_FULL*/:
				aOut = errc::no_space_on_device;
				break;

			case 0x0091 /*ERROR_DIR_NOT_EMPTY*/:
				aOut = errc::directory_not_empty;
				break;

			case 0x00AA /*ERROR_BUSY*/:
				aOut = errc::device_or_resource_busy;
				break;

			case 0x00C1 /*ERROR_BAD_EXE_FORMAT*/:
				aOut = errc::executable_format_error;
				break;

			default: return false;
		}

		return true;
	}
}

//--    l: win32_()                         ///{{{2///////////////////////////
namespace
{
	struct AutoWin32Api_
	{
		AutoWin32Api_() { flux::dlapi::os::acquire_win32(); }
		~AutoWin32Api_() { flux::dlapi::os::release_win32(); }
	};

	dlapi::os::Win32api const* win32_() noexcept
	{
		static AutoWin32Api_ autoApi;
		return flux::dlapi::os::win32();
	}
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
