/*-******************************************************* -- HEADER -{{{1- */
/*- system_error minimal fallback
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_SYSTEM_ERROR_HPP_63EB1CE4_A693_4CBD_B70F_4DE2069C6486
#define FLUX_NIH_SYSTEM_ERROR_HPP_63EB1CE4_A693_4CBD_B70F_4DE2069C6486

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/enable_if.hpp>
#include <flux/compat/constexpr.hpp>

#include <iosfwd>
#include <string>
#include <exception>
#include <type_traits>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    macros                              ///{{{2///////////////////////////

/** Helper: mark enum \a e as an error code
 *
 * Mark enum \a e as an error code by definiting the specialization
 * `::flux::nih_::is_error_code_enum<e>` to be `std::true_type`.
 *
 * \warning This macro must be invoked from the global namespace!
 */
#define FLUX_GLOBAL_NIH_MAKE_ERROR_CODE_ENUM(e) FLUX_DEFINED_ELSEWHERE

/** Helper: mark enum \a e as an error condition
 *
 * Mark enum \a e as an error condition by definiting the specialization
 * `::flux::nih_::is_error_condition_enum<e>` to be `std::true_type`.
 *
 * \warning This macro must be invoked from the global namespace!
 */
#define FLUX_GLOBAL_NIH_MAKE_ERROR_CONDITION_ENUM(e) FLUX_DEFINED_ELSEWHERE

//--    forward                             ///{{{2///////////////////////////

class system_error;

class error_code;
class error_category;
class error_condition;

//--    meta functions                      ///{{{2///////////////////////////

template< typename tErrorEnum >
struct is_error_code_enum : std::false_type {};
template< typename tErrorEnum >
struct is_error_condition_enum : std::false_type {};

//--    constants                           ///{{{2///////////////////////////
enum class errc
{
	/* Constants from
	 *   http://en.cppreference.com/w/cpp/error/errc
	 *
	 * 0 => no error, so don't use that here.
	 */
	address_family_not_supported = 1,
	address_in_use,
	address_not_available,
	already_connected,
	argument_list_too_long,
	argument_out_of_domain,
	bad_address,
	bad_file_descriptor,
	bad_message,
	broken_pipe,
	connection_aborted,
	connection_already_in_progress,
	connection_refused,
	connection_reset,
	cross_device_link,
	destination_address_required,
	device_or_resource_busy,
	directory_not_empty,
	executable_format_error,
	file_exists,
	file_too_large,
	filename_too_long,
	function_not_supported,
	host_unreachable,
	identifier_removed,
	illegal_byte_sequence,
	inappropriate_io_control_operation,
	interrupted,
	invalid_argument,
	invalid_seek,
	io_error,
	is_a_directory,
	message_size,
	network_down,
	network_reset,
	network_unreachable,
	no_buffer_space,
	no_child_process,
	no_link,
	no_lock_available,
	no_message_available,
	no_message,
	no_protocol_option,
	no_space_on_device,
	no_stream_resources,
	no_such_device_or_address,
	no_such_device,
	no_such_file_or_directory,
	no_such_process,
	not_a_directory,
	not_a_socket,
	not_a_stream,
	not_connected,
	not_enough_memory,
	not_supported,
	operation_canceled,
	operation_in_progress,
	operation_not_permitted,
	operation_not_supported,
	operation_would_block,
	owner_dead,
	permission_denied,
	protocol_error,
	protocol_not_supported,
	read_only_file_system,
	resource_deadlock_would_occur,
	resource_unavailable_try_again,
	result_out_of_range,
	state_not_recoverable,
	stream_timeout,
	text_file_busy,
	timed_out,
	too_many_files_open_in_system,
	too_many_files_open,
	too_many_links,
	too_many_symbolic_link_levels,
	value_too_large,
	wrong_protocol_type
};

//--    error_code                          ///{{{2///////////////////////////
class error_code
{
	public:
		error_code() noexcept;
		error_code( int, error_category const& ) noexcept;

		error_code( error_code const& ) noexcept;

		template< typename tErrorEnum > inline
		error_code(
			tErrorEnum, 
			FLUX_ENABLE_IF(is_error_code_enum<tErrorEnum>)(void*) = nullptr
		) noexcept;

	public:
		error_code& operator= (error_code const&) noexcept;

		template< typename tErrorEnum > inline
		FLUX_ENABLE_IF( is_error_code_enum<tErrorEnum> )
		(error_code&) operator= (tErrorEnum) noexcept;

		bool operator== (error_code const&) const noexcept;
		bool operator!= (error_code const&) const noexcept;
		bool operator< (error_code const&) const noexcept;

		//TODO operator<< with std::ostream
		
		typedef void (error_code::*unspecified_boolean_type)();
		operator unspecified_boolean_type() const noexcept;

	public:
		void assign( int, error_category const& ) noexcept;
		void clear() noexcept;

		int value() const noexcept;
		error_category const& category() const noexcept;

		error_condition default_error_condition() const noexcept;
		std::string message() const;

	private:
		int mValue;
		error_category const* mCategory;
};

// Note: should actually be a template to std::basic_ostream<...>
std::ostream& operator<< (std::ostream&, error_code const&);

//--    error_condition                     ///{{{2///////////////////////////
class error_condition
{
	public:
		error_condition() noexcept;
		error_condition( int, error_category const& ) noexcept;

		error_condition( error_condition const& ) noexcept;

		template< typename tErrorEnum > inline
		error_condition( 
			tErrorEnum, 
			FLUX_ENABLE_IF(is_error_condition_enum<tErrorEnum>)(void*) = nullptr
		) noexcept;

	public:
		error_condition& operator= (error_condition const&) noexcept;

		template< typename tErrorEnum > inline
		FLUX_ENABLE_IF(is_error_condition_enum<tErrorEnum>)
		(error_condition&) operator= (tErrorEnum) noexcept;

		bool operator== (error_condition const&) const noexcept;
		bool operator!= (error_condition const&) const noexcept;

		bool operator< (error_condition const&) const noexcept;

		//TODO: operator<< with std::ostream
		
		typedef void (error_condition::*unspecified_boolean_type)();
		operator unspecified_boolean_type() const noexcept;

	public:
		void assign( int, error_category const& ) noexcept;
		void clear() noexcept;

		int value() const noexcept;
		error_category const& category() const noexcept;

		std::string message() const;

	private:
		int mValue;
		error_category const* mCategory;

};

bool operator== (error_code const&, error_condition const&) noexcept;
bool operator!= (error_code const&, error_condition const&) noexcept;

bool operator== (error_condition const&, error_code const&) noexcept;
bool operator!= (error_condition const&, error_code const&) noexcept;


// Note: should actually be a template to std::basic_ostream<...>
std::ostream& operator<< (std::ostream&, error_code const&);

//--    error_category                      ///{{{2///////////////////////////
class error_category
{
	public:
		FLUX_CONSTEXPR_EX error_category() {}
		virtual ~error_category() = 0;

		error_category( error_category const& ) = delete;
		error_category& operator= (error_category const&) = delete;

	public:
		bool operator== (error_category const&) const noexcept;
		bool operator!= (error_category const&) const noexcept;

		bool operator< (error_category const&) const noexcept;

	public:
		virtual char const* name() const noexcept = 0;

		virtual error_condition default_error_condition( int ) const noexcept;
		virtual bool equivalent( int, error_condition const& ) const noexcept;
		virtual bool equivalent( error_code const&, int ) const noexcept;

		virtual std::string message( int ) const = 0;
};

//--    system_error                        ///{{{2///////////////////////////
class system_error : public std::exception
{
	public:
		system_error( error_code );
		system_error( error_code, std::string const& );
		system_error( error_code, char const* );

		system_error( int, error_category const& );
		system_error( int, error_category const&, std::string const& );
		system_error( int, error_category const&, char const* );

		virtual ~system_error() noexcept;

	public:
		error_code const& code() const noexcept;

	public: // virtuals from std::exception
		char const* what() const noexcept override;

	private:
		void format_( char const* );

	private:
		error_code mCode;
		std::string mWhat;
};

//--    functions                           ///{{{2///////////////////////////

error_category const& system_category() noexcept;
error_category const& generic_category() noexcept;

error_code make_error_code( errc ) noexcept;
error_condition make_error_condition( errc ) noexcept;

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/system_error.inl"
#endif // FLUX_NIH_SYSTEM_ERROR_HPP_63EB1CE4_A693_4CBD_B70F_4DE2069C6486
