/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	`errno` error category
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/errno_category.hpp>

#include <flux/util/diagnostics.hpp>

#include <string>
#include <type_traits>

#include <errno.h>
#include <string.h>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    $ local                             ///{{{2///////////////////////////
#if FLUX_STDLIB == FLUX_STDLIB_STDCPP || FLUX_STDLIB == FLUX_STDLIB_LIBCPP
/* Copy-pasta from flux-nih_/src/exception.cpp ; essentially, one version of
 * the strerror_fixup_() methods will be called depending on the strerror_r()
 * version that we get (XSI or GNU), and the other will be unused. That's OK,
 * don't warn about it.
 */
FLUX_DIAGNOSTIC_STATE_SET(IGNORED,UNUSED_FUNCTION)
#endif // ~ STDLIB

namespace
{
	struct ErrnoCategory_ : compat::error_category
	{
		char const* name() const noexcept override;
		std::string message( int ) const override;
		compat::error_condition default_error_condition( int ) const noexcept override;
	};

	std::string errno_message_( int );
	bool errno_to_errc_( int, compat::errc& );

#	if FLUX_STDLIB == FLUX_STDLIB_STDCPP || FLUX_STDLIB == FLUX_STDLIB_LIBCPP
	struct LocalType_ {};
	namespace x
	{
		LocalType_ strerror_r( int, ... );
	}

	using x::strerror_r;
	constexpr bool kHasStrerrorR_ = !std::is_same<
		decltype(strerror_r( 0, std::declval<char*>(), std::size_t(0) )), LocalType_
	>::value;

	char const* strerror_fixup_( int, char const* );
	char const* strerror_fixup_( char const*, char const* );
	char const* strerror_fixup_( LocalType_, char const* );
#	endif // ~ STDLIB
}

//--    errno_category()                    ///{{{2///////////////////////////
compat::error_category const& errno_category() noexcept
{
	static ErrnoCategory_ cat;
	return cat;
}

//--    make_errno_error_code()             ///{{{2///////////////////////////
compat::error_code make_errno_error_code() noexcept
{
	return make_errno_error_code( errno );
}
compat::error_code make_errno_error_code( int aErrno ) noexcept
{
	return compat::error_code( aErrno, errno_category() );
}

//--    l: ErrnoCategory_                   ///{{{2///////////////////////////
namespace
{
	char const* ErrnoCategory_::name() const noexcept
	{
		return "errno";
	}

	std::string ErrnoCategory_::message( int aValue ) const
	{
		return errno_message_( aValue );
	};

	compat::error_condition ErrnoCategory_::default_error_condition( int aValue ) const noexcept
	{
		if( 0 == aValue ) 
			return compat::error_condition( 0, compat::generic_category() );

		compat::errc ec;
		if( errno_to_errc_( aValue, ec ) )
			return compat::error_condition( ec );

		return compat::error_condition( aValue, *this );
	}
}


//--    l: errno_message_()                 ///{{{2///////////////////////////
namespace 
{
#	if FLUX_STDLIB == FLUX_STDLIB_STDCPP || FLUX_STDLIB == FLUX_STDLIB_LIBCPP
	template< bool >
	struct ErrnoMsg_
	{
		static std::string msg( int aErrno )
		{
			char b[128];
			char const* msg = strerror_fixup_(strerror_r( aErrno, b, sizeof(b) ), b);
			return std::string(msg);
		}
	};
	template<>
	struct ErrnoMsg_<false>
	{
		static std::string msg( int aErrno )
		{
			return std::string(strerror(aErrno) );
		}
	};
#	endif // ~ FLUX_STDLIB
	
	std::string errno_message_( int aErrno )
	{
#		if FLUX_STDLIB == FLUX_STDLIB_STDCPP || FLUX_STDLIB == FLUX_STDLIB_LIBCPP
		return ErrnoMsg_< kHasStrerrorR_ >::msg( aErrno );

#		elif FLUX_STDLIB == FLUX_STDLIB_VSCPP
		char buff[128];
		if( 0 != strerror_s( buff, sizeof(buff), aErrno ) )
			return std::string( "(strerror_s() returned additional error)" );

		return std::string(buff);

#		else
		return std::string(strerror(aErrno));
#		endif // ~ FLUX_STDLIB
	}
}

//--    l: errno_to_errc_()                 ///{{{2///////////////////////////
namespace
{
	bool errno_to_errc_( int aErrno, compat::errc& aOut )
	{
		using compat::errc;

		switch( aErrno )
		{
			case EAFNOSUPPORT: aOut = errc::address_family_not_supported; break;
			case EADDRINUSE: aOut = errc::address_in_use; break;
			case EADDRNOTAVAIL: aOut = errc::address_not_available; break;
			case EISCONN: aOut = errc::already_connected; break;
			case E2BIG: aOut = errc::argument_list_too_long; break;
			case EDOM: aOut = errc::argument_out_of_domain; break;
			case EFAULT: aOut = errc::bad_address; break;
			case EBADF: aOut = errc::bad_file_descriptor; break;
			case EBADMSG: aOut = errc::bad_message; break;
			case EPIPE: aOut = errc::broken_pipe; break;
			case ECONNABORTED: aOut = errc::connection_aborted; break;
			case EALREADY: aOut = errc::connection_already_in_progress; break;
			case ECONNREFUSED: aOut = errc::connection_refused; break;
			case ECONNRESET: aOut = errc::connection_reset; break;
			case EXDEV: aOut = errc::cross_device_link; break;
			case EDESTADDRREQ: aOut = errc::destination_address_required; break;
			case EBUSY: aOut = errc::device_or_resource_busy; break;
			case ENOTEMPTY: aOut = errc::directory_not_empty; break;
			case ENOEXEC: aOut = errc::executable_format_error; break;
			case EEXIST: aOut = errc::file_exists; break;
			case EFBIG: aOut = errc::file_too_large; break;
			case ENAMETOOLONG: aOut = errc::filename_too_long; break;
			case ENOSYS: aOut = errc::function_not_supported; break;
			case EHOSTUNREACH: aOut = errc::host_unreachable; break;
			case EIDRM: aOut = errc::identifier_removed; break;
			case EILSEQ: aOut = errc::illegal_byte_sequence; break;
			case ENOTTY: aOut = errc::inappropriate_io_control_operation; break;
			case EINTR: aOut = errc::interrupted; break;
			case EINVAL: aOut = errc::invalid_argument; break;
			case ESPIPE: aOut = errc::invalid_seek; break;
			case EIO: aOut = errc::io_error; break;
			case EISDIR: aOut = errc::is_a_directory; break;
			case EMSGSIZE: aOut = errc::message_size; break;
			case ENETDOWN: aOut = errc::network_down; break;
			case ENETRESET: aOut = errc::network_reset; break;
			case ENETUNREACH: aOut = errc::network_unreachable; break;
			case ENOBUFS: aOut = errc::no_buffer_space; break;
			case ECHILD: aOut = errc::no_child_process; break;
			case ENOLINK: aOut = errc::no_link; break;
			case ENOLCK: aOut = errc::no_lock_available; break;
			case ENODATA: aOut = errc::no_message_available; break;
			case ENOMSG: aOut = errc::no_message; break;
			case ENOPROTOOPT: aOut = errc::no_protocol_option; break;
			case ENOSPC: aOut = errc::no_space_on_device; break;
			case ENOSR: aOut = errc::no_stream_resources; break;
			case ENXIO: aOut = errc::no_such_device_or_address; break;
			case ENODEV: aOut = errc::no_such_device; break;
			case ENOENT: aOut = errc::no_such_file_or_directory; break;
			case ESRCH: aOut = errc::no_such_process; break;
			case ENOTDIR: aOut = errc::not_a_directory; break;
			case ENOTSOCK: aOut = errc::not_a_socket; break;
			case ENOSTR: aOut = errc::not_a_stream; break;
			case ENOTCONN: aOut = errc::not_connected; break;
			case ENOMEM: aOut = errc::not_enough_memory; break;
			case ENOTSUP: aOut = errc::not_supported; break;
			case ECANCELED: aOut = errc::operation_canceled; break;
			case EINPROGRESS: aOut = errc::operation_in_progress; break;
			case EPERM: aOut = errc::operation_not_permitted; break;
#			if ENOTSUP != EOPNOTSUPP
			case EOPNOTSUPP: aOut = errc::operation_not_supported; break;
#			endif
			case EWOULDBLOCK: aOut = errc::operation_would_block; break;
			case EOWNERDEAD: aOut = errc::owner_dead; break;
			case EACCES: aOut = errc::permission_denied; break;
			case EPROTO: aOut = errc::protocol_error; break;
			case EPROTONOSUPPORT: aOut = errc::protocol_not_supported; break;
			case EROFS: aOut = errc::read_only_file_system; break;
			case EDEADLK: aOut = errc::resource_deadlock_would_occur; break;
#			if EAGAIN != EWOULDBLOCK
			case EAGAIN: aOut = errc::resource_unavailable_try_again; break;
#			endif
			case ERANGE: aOut = errc::result_out_of_range; break;
			case ENOTRECOVERABLE: aOut = errc::state_not_recoverable; break;
			case ETIME: aOut = errc::stream_timeout; break;
			case ETXTBSY: aOut = errc::text_file_busy; break;
			case ETIMEDOUT: aOut = errc::timed_out; break;
			case ENFILE: aOut = errc::too_many_files_open_in_system; break;
			case EMFILE: aOut = errc::too_many_files_open; break;
			case EMLINK: aOut = errc::too_many_links; break;
			case ELOOP: aOut = errc::too_many_symbolic_link_levels; break;
			case EOVERFLOW: aOut = errc::value_too_large; break;
			case EPROTOTYPE: aOut = errc::wrong_protocol_type; break;

#			if FLUX_STDLIB == FLUX_STDLIB_VSCPP
			// MSVC specific: no corresponding errc value!
			case STRUNCATE: return false;
#			endif // ~ FLUX_STDLIB

			default: return false;
		}

		return true;
	}
}

//--    l: strerror_fixup_()                ///{{{2///////////////////////////
namespace
{
#	if FLUX_STDLIB == FLUX_STDLIB_STDCPP || FLUX_STDLIB == FLUX_STDLIB_LIBCPP
	char const* strerror_fixup_( int aRet, char const* aBuff )
	{
		// XSI version, returns an int, which is non-zero on failure.
		if( aRet ) return "(strerror_r() returned additional error)";
		// ... or otherwise places the result in the provided buffer
		return aBuff;
	}
	char const* strerror_fixup_( char const* aRet, char const* )
	{
		// GNU version, returns a char const*, which is NULL on failure
		if( !aRet ) return "(strerror_r() returned additional error)";
		// ... or otherwise a pointer that holds the string
		return aRet;
	}
#	endif // ~ STDLIB
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
