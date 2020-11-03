/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Minimal system_error fallback implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/nih_/system_error.hpp>

#include <flux/std/assert.hpp>

#include <sstream>
#include <ostream>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    $ local                             ///{{{2///////////////////////////
namespace 
{
	struct SystemCategoryImpl_ : error_category
	{
		char const* name() const noexcept override;
		std::string message( int ) const override;

		//TODO: windows error codes? Use flux-native/somethingsomething.
		//TODO: equivalent() etc
	};
	struct GenericCategoryImpl_ : error_category
	{
		char const* name() const noexcept override;
		std::string message( int ) const override;
	};

	std::string errc_message_( errc );
}

//--    error_code                          ///{{{2///////////////////////////
error_code::error_code() noexcept
	: mValue(0)
	, mCategory( &system_category() )
{}

error_code::error_code( int aValue, error_category const& aCategory ) noexcept
	: mValue(aValue)
	, mCategory( &aCategory )
{}

error_code::error_code( error_code const& aOther ) noexcept
	: mValue(aOther.mValue)
	, mCategory(aOther.mCategory)
{}


error_code& error_code::operator= (error_code const& aOther) noexcept
{
	mValue = aOther.mValue;
	mCategory = aOther.mCategory;
	return *this;
}

bool error_code::operator== (error_code const& aOther) const noexcept
{
	return mCategory == aOther.mCategory && mValue == aOther.mValue;
}
bool error_code::operator!= (error_code const& aOther) const noexcept
{
	return mCategory != aOther.mCategory || mValue != aOther.mValue;
}
bool error_code::operator< (error_code const& aOther) const noexcept
{
	if( mCategory != aOther.mCategory )
		return category() < aOther.category();

	return mValue < aOther.mValue;
}

error_code::operator unspecified_boolean_type() const noexcept
{
	return mValue != 0 ? &error_code::clear : nullptr;
}


void error_code::assign( int aValue, error_category const& aCategory ) noexcept
{
	mValue = aValue;
	mCategory = &aCategory;
}
void error_code::clear() noexcept
{
	mValue = 0;
	mCategory = &system_category();
}

int error_code::value() const noexcept
{
	return mValue;
}
error_category const& error_code::category() const noexcept
{
	FLUX_ASSERT( mCategory );
	return *mCategory;
}

error_condition error_code::default_error_condition() const noexcept
{
	FLUX_ASSERT( mCategory );
	return mCategory->default_error_condition( mValue );
}
std::string error_code::message() const
{
	FLUX_ASSERT( mCategory );
	return mCategory->message( mValue );
}


std::ostream& operator<< (std::ostream& aOs, error_code const& aEC)
{
	aOs << aEC.category().name() << "." << aEC.value() << ": " << aEC.message();
	return aOs;
}

//--    error_condition                     ///{{{2///////////////////////////
error_condition::error_condition() noexcept
	: mValue(0)
	, mCategory( &generic_category() )
{}

error_condition::error_condition( int aValue, error_category const& aCat ) noexcept
	: mValue(aValue)
	, mCategory( &aCat )
{}

error_condition::error_condition( error_condition const& aOther ) noexcept
	: mValue(aOther.mValue)
	, mCategory(aOther.mCategory)
{}


error_condition& error_condition::operator= (error_condition const& aOther) noexcept
{
	mValue = aOther.mValue;
	mCategory = aOther.mCategory;
	return *this;
}


bool error_condition::operator== (error_condition const& aOther) const noexcept
{
	return mCategory == aOther.mCategory && mValue == aOther.mValue;
}
bool error_condition::operator!= (error_condition const& aOther) const noexcept
{
	return mCategory != aOther.mCategory || mValue != aOther.mValue;
}

bool error_condition::operator< (error_condition const& aOther) const noexcept
{
	return category() < aOther.category() && mValue < aOther.mValue;
}

error_condition::operator unspecified_boolean_type() const noexcept
{
	return mValue != 0 ? &error_condition::clear : nullptr;
}


void error_condition::assign( int aValue, error_category const& aCategory ) noexcept
{
	mValue = aValue;
	mCategory = &aCategory;
}
void error_condition::clear() noexcept
{
	mValue = 0;
	mCategory = &generic_category();
}

int error_condition::value() const noexcept
{
	return mValue;
}
error_category const& error_condition::category() const noexcept
{
	FLUX_ASSERT( mCategory );
	return *mCategory;
}

std::string error_condition::message() const
{
	FLUX_ASSERT( mCategory );
	return mCategory->message( mValue );
}


bool operator== (error_code const& aCode, error_condition const& aCond) noexcept
{
	return aCode.category().equivalent(aCode.value(), aCond)
	    || aCond.category().equivalent(aCode, aCond.value())
	;
}
bool operator!= (error_code const& aCode, error_condition const& aCond) noexcept
{
	return !aCode.category().equivalent(aCode.value(), aCond)
	    && !aCond.category().equivalent(aCode, aCond.value())
	;
}

bool operator== (error_condition const& aCond, error_code const& aCode) noexcept
{
	return aCode.category().equivalent(aCode.value(), aCond)
	    || aCond.category().equivalent(aCode, aCond.value())
	;
}
bool operator!= (error_condition const& aCond, error_code const& aCode) noexcept
{
	return !aCode.category().equivalent(aCode.value(), aCond)
	    && !aCond.category().equivalent(aCode, aCond.value())
	;
}

std::ostream& operator<< (std::ostream& aOs, error_condition const& aEC)
{
	aOs << aEC.category().name() << "." << aEC.value() << ": " << aEC.message();
	return aOs;
}



//--    error_category                      ///{{{2///////////////////////////
error_category::~error_category()
{}

bool error_category::operator== (error_category const& aOther) const noexcept
{
	return this == &aOther;
}
bool error_category::operator!= (error_category const& aOther) const noexcept
{
	return this != &aOther;
}
bool error_category::operator< (error_category const& aOther) const noexcept
{
	return this < &aOther;
}

error_condition error_category::default_error_condition( int aValue ) const noexcept
{
	return error_condition( aValue, *this );
}

bool error_category::equivalent( int aValue, error_condition const& aCond ) const noexcept
{
	return default_error_condition(aValue) == aCond;
}
bool error_category::equivalent( error_code const& aCode, int aValue ) const noexcept
{
	return *this == aCode.category() && aCode.value() == aValue;
}


//--    system_error                        ///{{{2///////////////////////////
system_error::system_error( error_code aCode )
	: mCode( aCode )
{
	format_( nullptr );
}
system_error::system_error( error_code aCode, std::string const& aMsg )
	: mCode( aCode )
{
	format_( aMsg.c_str() );
}
system_error::system_error( error_code aCode, char const* aMsg )
	: mCode( aCode )
{
	format_( aMsg );
}

system_error::system_error( int aCode, error_category const& aCat )
	: mCode( aCode, aCat )
{
	format_( nullptr );
}
system_error::system_error( int aCode, error_category const& aCat, std::string const& aMsg )
	: mCode( aCode, aCat )
{
	format_( aMsg.c_str() );
}
system_error::system_error( int aCode, error_category const& aCat, char const* aMsg )
	: mCode( aCode, aCat )
{
	format_( aMsg );
}

system_error::~system_error() noexcept
{}

error_code const& system_error::code() const noexcept
{
	return mCode;
}
char const* system_error::what() const noexcept
{
	return mWhat.c_str();
}

void system_error::format_( char const* aMsg )
{
	std::ostringstream oss;

	if( aMsg )
	{
		oss << aMsg << ": ";
	}

	oss << mCode.message() << "(" << mCode.category().name() << "." << mCode.value() << ")" ;

	mWhat = oss.str();
}

//--    make_error_*()                      ///{{{2///////////////////////////
error_code make_error_code( errc aEnum ) noexcept
{
	return error_code( static_cast<int>(aEnum), generic_category() );
}

error_condition make_error_condition( errc aEnum ) noexcept
{
	return error_condition( static_cast<int>(aEnum), generic_category() );
}

//--    *_category(  )                      ///{{{2///////////////////////////
error_category const& system_category() noexcept
{
	static SystemCategoryImpl_ cat;
	return cat;
}
error_category const& generic_category() noexcept
{
	static GenericCategoryImpl_ cat;
	return cat;
}

//--    l: SystemCategoryImpl_              ///{{{2///////////////////////////
namespace 
{
	//TODO use from flux-native
	char const* SystemCategoryImpl_::name() const noexcept
	{
		return "system";
	}
	std::string SystemCategoryImpl_::message( int aValue ) const
	{
		std::ostringstream oss;
		oss << "FIXME-system-error-" << aValue;
		return oss.str();
	}
}

//--    l: GenericCategoryImpl_             ///{{{2///////////////////////////
namespace 
{
	char const* GenericCategoryImpl_::name() const noexcept
	{
		return "generic";
	}
	std::string GenericCategoryImpl_::message( int aValue ) const
	{
		return errc_message_( static_cast<errc>(aValue) );
	}
}

//--    l: errc_message_()                  ///{{{2///////////////////////////
namespace 
{
	std::string errc_message_( errc aValue )
	{
		if( static_cast<errc>(0) == aValue )
			return "Success";

		switch( aValue )
		{
			case errc::address_family_not_supported: return "Address family not supported by protocol";
			case errc::address_in_use: return "Address already in use";
			case errc::address_not_available: return "Cannot assign requested address";
			case errc::already_connected: return "Transport endpoint is already connected";
			case errc::argument_list_too_long: return "Argument list too long";
			case errc::argument_out_of_domain: return "Numerical argument out of domain";
			case errc::bad_address: return "Bad address";
			case errc::bad_file_descriptor: return "Bad file descriptor";
			case errc::bad_message: return "Bad message";
			case errc::broken_pipe: return "Broken pipe";
			case errc::connection_aborted: return "Software caused connection abort";
			case errc::connection_already_in_progress: return "Operation already in progress";
			case errc::connection_refused: return "Connection refused";
			case errc::connection_reset: return "Connection reset by peer";
			case errc::cross_device_link: return "Invalid cross-device link";
			case errc::destination_address_required: return "Destination address required";
			case errc::device_or_resource_busy: return "Device or resource busy";
			case errc::directory_not_empty: return "Directory not empty";
			case errc::executable_format_error: return "Exec format error";
			case errc::file_exists: return "File exists";
			case errc::file_too_large: return "File too large";
			case errc::filename_too_long: return "File name too long";
			case errc::function_not_supported: return "Function not implemented";
			case errc::host_unreachable: return "No route to host";
			case errc::identifier_removed: return "Identifier removed";
			case errc::illegal_byte_sequence: return "Invalid or incomplete multibyte or wide character";
			case errc::inappropriate_io_control_operation: return "Inappropriate ioctl for device";
			case errc::interrupted: return "Interrupted system call";
			case errc::invalid_argument: return "Invalid argument";
			case errc::invalid_seek: return "Illegal seek";
			case errc::io_error: return "Input/output error";
			case errc::is_a_directory: return "Is a directory";
			case errc::message_size: return "Message too long";
			case errc::network_down: return "Network is down";
			case errc::network_reset: return "Network dropped connection on reset";
			case errc::network_unreachable: return "Network is unreachable";
			case errc::no_buffer_space: return "No buffer space available";
			case errc::no_child_process: return "No child processes";
			case errc::no_link: return "Link has been severed";
			case errc::no_lock_available: return "No locks available";
			case errc::no_message_available: return "No data available";
			case errc::no_message: return "No message of desired type";
			case errc::no_protocol_option: return "Protocol not available";
			case errc::no_space_on_device: return "No space left on device";
			case errc::no_stream_resources: return "Out of streams resources";
			case errc::no_such_device_or_address: return "No such device or address";
			case errc::no_such_device: return "No such device";
			case errc::no_such_file_or_directory: return "No such file or directory";
			case errc::no_such_process: return "No such process";
			case errc::not_a_directory: return "Not a directory";
			case errc::not_a_socket: return "Socket operation on non-socket";
			case errc::not_a_stream: return "Device not a stream";
			case errc::not_connected: return "Transport endpoint is not connected";
			case errc::not_enough_memory: return "Cannot allocate memory";
			case errc::not_supported: return "Operation not supported";
			case errc::operation_canceled: return "Operation canceled";
			case errc::operation_in_progress: return "Operation now in progress";
			case errc::operation_not_permitted: return "Operation not permitted";
			case errc::operation_not_supported: return "Operation not supported";
			case errc::operation_would_block: return "Resource temporarily unavailable";
			case errc::owner_dead: return "Owner died";
			case errc::permission_denied: return "Permission denied";
			case errc::protocol_error: return "Protocol error";
			case errc::protocol_not_supported: return "Protocol not supported";
			case errc::read_only_file_system: return "Read-only file system";
			case errc::resource_deadlock_would_occur: return "Resource deadlock avoided";
			case errc::resource_unavailable_try_again: return "Resource temporarily unavailable";
			case errc::result_out_of_range: return "Numerical result out of range";
			case errc::state_not_recoverable: return "State not recoverable";
			case errc::stream_timeout: return "Timer expired";
			case errc::text_file_busy: return "Text file busy";
			case errc::timed_out: return "Connection timed out";
			case errc::too_many_files_open_in_system: return "Too many open files in system";
			case errc::too_many_files_open: return "Too many open files";
			case errc::too_many_links: return "Too many links";
			case errc::too_many_symbolic_link_levels: return "Too many levels of symbolic links";
			case errc::value_too_large: return "Value too large for defined data type";
			case errc::wrong_protocol_type: return "Protocol wrong type for socket";
		}

		std::ostringstream oss;
		oss << "Unknown errc value " << static_cast<int>(aValue);
		return oss.str();
	}
}

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
