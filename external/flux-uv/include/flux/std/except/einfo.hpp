/*-******************************************************* -- HEADER -{{{1- */
/*-	Standard exception info
 *
 * Somewhat inspired by the list in N3758.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_EXCEPT_EINFO_HPP_0FAFCD0B_013E_4D0F_B500_7E9A0215D6BA
#define FLUX_STD_EXCEPT_EINFO_HPP_0FAFCD0B_013E_4D0F_B500_7E9A0215D6BA

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/format.hpp>
#include <flux/std/enable_if.hpp>

#include <flux/compat/type_traits.hpp>
#include <flux/compat/system_error.hpp>

#include <flux/util/attrib.hpp>

#include <string>
#include <cstdint>

#include "../exception.hpp"

//--    >>> namespace = flux::einfo >>>             ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(einfo)
//--    ErrorInfo                           ///{{{2///////////////////////////
namespace detail
{
	struct ErrorInfoBase {};
}

/** Attachable error information
 *
 * `ErrorInfo` is a type that can be used to create additional information that
 * is attachable to flux exceptions (`flux::error::Exception`). Several 
 * standardized `ErrorInfo` types are defined already:
 *   - `SourceFile`, `SourceLine`, `SourceFunc`
 *   - `ApiError`, `ApiFunction`, `ApiResource`
 *   - `ErrorDesc`, `ErrorExpr`, `ErrorMeta`
 *
 * Error info can be attached to a flux exception using `operator<<`:
 * \code
 * try
 * {
 * 	FLUX_THROW( error::RuntimeError )
 * 		<< einfo::ApiFunction( "some_api_function" )
 * 	;
 * }
 * catch( error::Exception const& eErr )
 * {
 * 	eErr << einfo::ErrorDesc( "While doing something" );
 * 	throw;
 * }
 * \endcode
 *
 * Attached information can be queried using `einfo::query_error_info()` and is
 * included in the diagnostic information from `einfo::query_diagnostic_info()`.
 *
 * New types can be defined freely using one of the two following options:
 *   - `typedef` of `ErrorInfo` with a custom tag and an data type
 *   - Inheriting from a `ErrorInfo` with a custom tag and data type.
 * Examples:
   \code
   // Simple ErrorInfo:
   using SimpleInfo = einfo::ErrorInfo< struct SimpleInfo_, float >;
  
   // Complex ErrorInfo:
   struct ComplexInfo : einfo::ErrorInfo< struct ComplexInfo_, DataType >
   {
     ... 
   };
   \endcode
 *
 * `ComplexInfo` may "override" standard behaviour by providing custom 
 * constructors, and by providing custom implementations of the `name()` and
 * `to_string()` static methods.
 */
template< class tTag, typename tType >
class ErrorInfo : public detail::ErrorInfoBase
{
	public:
		template< typename... tArgs >
		explicit ErrorInfo( tArgs&&... ) noexcept(std::is_nothrow_constructible<tType, tArgs...>::value);
	
	public:
		using type = tType;

	public:
		tType&& get() && noexcept;
		tType const& get() const& noexcept;
	
	public:
		static char const* name() noexcept;
		static std::string to_string( tType const& );
		
	private:
		tType mValue;
};

/* Specialization for std::string. Allows input as for flux::format. */
template< class tTag >
class ErrorInfo< tTag, std::string > : public detail::ErrorInfoBase
{
	public:
		explicit ErrorInfo( char const* );
		explicit ErrorInfo( std::string&& ) noexcept;
		explicit ErrorInfo( std::string const& );
		
		template< typename tS, typename... tArgs >
		explicit ErrorInfo( tS const&, tArgs&&... );

		template< typename... tArgs > 
		FLUX_DEPRECATED_EX( "Use FLUX_FMT_STRING()" )
		explicit ErrorInfo( char const*, tArgs&&... );

		ErrorInfo( ErrorInfo&& ) = default;
		ErrorInfo& operator=(ErrorInfo&&) = default;

	public:
		using tag = tTag;
		using type = std::string;
	
	public:
		std::string&& get() && noexcept;
		std::string const& get() const& noexcept;

	public:
		static char const* name() noexcept;
		static std::string const& to_string( std::string const& );

	private:
		std::string mString;
};

//--    default einfo                       ///{{{2///////////////////////////

/** \name Error info: source code location
 * \brief Error info: source code location
 *
 * Error info encapsulating the source code location, i.e., source line, 
 * source file and source function. 
 *
 * \note The `FLUX_THROW*()` macros automatically append this information to
 * exceptions based on the values of `__LINE__`, `__FILE__` and using
 * `FLUX_COMPAT_FUNCTION_PROTO`. If needed, they can neverhtless be overriden
 * later.
 *
 * \warning The source file name and the source function name are stored as
 * C string pointers, meaning that they only accept literal strings (as 
 * provided by the aforementioned macros).
 */
//@{
using SourceLine = ErrorInfo< struct SourceLine_, int >;
using SourceFile = ErrorInfo< struct SourceFile_, char const* >;
using SourceFunc = ErrorInfo< struct SourceFunc_, char const* >;
//@}

/** Error info: API function
 *
 * Error info identifying the failed API function. A typical use would be
 * as follows:
   \code
   FILE* f = fopen( "blam", "rb" );
   FLUX_THROW_IF( !f, error::RuntimeError )
   	<< einfo::ApiFunction( "fopen" )
   ;
   \endcode
 */
using ApiFunction = ErrorInfo< struct ApiFunction_, char const* >;
/** Error info: API resources
 *
 * Error info indentifying the API resource involved in a failed API call.
 * The underlying type is `std::uintptr_t`, but it expected to hold pointer
 * and other integer-like values:
   \code
   FILE* f = ...;
  
   auto ret = fread( ..., f );
   FLUX_THROW_IF( ret != expected, error::RuntimeError )
   	<< einfo::ApiResource( f )
   ;
   \endcode
 * 
 * \note Like all error info objects, `ApiResource` is intended to be purely
 * informational, so at no point should the original handle be recovered from
 * an `ApiResource` einfo object. In fact, the handle is not required to be
 * valid (it may have been closed and freed).
 */
using ApiResource = ErrorInfo< struct ApiResource_, std::uintptr_t >;

/** Error info: API Error
 *
 * Error info representing an API error using the C++11 error code framework
 * (i.e., using `compat::error_code`/`std::error_code`).
 *
 * Example:
   \code
   int errno_ = errno;
   FLUX_THROW( error::RuntimeError )
   	<< einfo::ApiError( errno_, sys::errno_category() )
   ;
   \endcode
 *
 * \see compat::error_code
 */
struct ApiError : ErrorInfo< struct ApiError_, compat::error_code >
{
	/** \name ApiError constructors
	 * \brief `ApiError` constructors
	 *
	 * `ApiError` may be constructed the same way an `compat::error_code`
	 * (i.e. `std::error_code` where available) is constructed, i.e. using
	 * the following arguments:
	 *   - a `compat::error_code` (i.e., `std::error_code` where available)
	 *   - an integer error code value and an `compat::error_category`
	 *   - any type for which `is_error_code_enum` evalutes to true, and for
	 *     which a corresponding `make_error_code()` exists.
	 *
	 * In addition, `ApiError()` accepts arguments of type `compat::errc`
	 * (i.e., `std::errc`).
	 */
	//@{
	explicit ApiError( compat::error_code const& ) noexcept;
	explicit ApiError( int, compat::error_category const& ) noexcept;

	template<
		typename tErrorEnum,
		typename = FLUX_ENABLE_IF( compat::is_error_code_enum<tErrorEnum> )(int)
	> explicit
	ApiError( tErrorEnum ) noexcept;

	explicit ApiError( compat::errc ) noexcept;
	//@}

	/** Generic name
	 *
	 * Returns the string `ApiError`. (Note: this does not show up in the
	 * diagnostic output, as `ApiError` is handled specially in the output.)
	 */
	static char const* name() noexcept;

	private: using Parent_ = ErrorInfo< ApiError_, compat::error_code >;
};

/** Error info: generic error description
 *
 * Free-form generic error description
   \code
   FLUX_THROW( error::LogicError )
   	<< einfo::ErrorDesc( "Argument is outside of the expected domain" )
	<< einfo::ErrorMeta( FLUX_FMT_STRING("argument = {:d}"), argument )
   ;
   \endcode
 *
 * \see ErrorMeta
 */
using ErrorDesc = ErrorInfo< struct ErrorDesc_, std::string >;
/** Error info: error expression
 *
 * `FLUX_THROW_IF()` and related macros attach an `ErrorExpr` einfo that
 * contains the expression that caused the exception to be thrown in the
 * first place. This is a stringized version of the first argument to the
 * macro.
 */
using ErrorExpr = ErrorInfo< struct ErrorExpr_, std::string >;
/** Error info: error meta
 *
 * Free-form error meta information. A suggested use of `ErrorMeta` is to 
 * capture additional values, i.e., for instance the value of an argument that
 * was found to be invalid, etc:
   \code
   FLUX_THROW( error::LogicError )
   	<< einfo::ErrorDesc( "Argument is outside of the expected domain" )
	<< einfo::ErrorMeta( FLUX_FMT_STRING("argument = {:d}"), argument )
   ;
   \endcode
 *
 * \see ErrorDesc
 */
using ErrorMeta = ErrorInfo< struct ErrorMeta_, std::string >;

//--    operators                           ///{{{2///////////////////////////

template< class tExcept, class tInfo >
FLUX_ENABLE_IF_C( std::is_base_of<error::Exception, tExcept>::value && std::is_base_of<einfo::detail::ErrorInfoBase,compat::remove_reference_t<tInfo>>::value )
(tExcept const&) operator<< (tExcept const&, tInfo&& );

//--    <<< ~ flux::einfo namespace <<<             ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(einfo)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "../detail/except-einfo.inl"
#endif // FLUX_STD_EXCEPT_EINFO_HPP_0FAFCD0B_013E_4D0F_B500_7E9A0215D6BA
