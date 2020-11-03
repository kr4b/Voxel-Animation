/*-******************************************************* -- HEADER -{{{1- */
/*-	system_error & co
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_SYSTEM_ERROR_HPP_346F37E8_599D_4413_8C35_70FF77A3A6A5
#define FLUX_COMPAT_SYSTEM_ERROR_HPP_346F37E8_599D_4413_8C35_70FF77A3A6A5

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/stdfeat.hpp>

//--    <system_error>              ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX11_SYSERROR
#	include <system_error>

FLUX_NAMESPACE_ENTER1(compat)
	using std::errc;

	using std::error_code;
	using std::error_condition;
	using std::error_category;

	using std::system_category;
	using std::generic_category;

	using std::make_error_code;
	using std::make_error_condition;

	using std::system_error;

	using std::is_error_code_enum;
	using std::is_error_condition_enum;
FLUX_NAMESPACE_LEAVE1(compat)

/** Helper: mark enum \a e as an error code
 *
 * Mark enum \a as an error code by definiting the specialization
 * `::std::is_error_code_enum<e>` to be `std::true_type`.
 *
 * \warning This macro must be placed in the global namespace!
 */
#	define FLUX_GLOBAL_MAKE_ERROR_CODE_ENUM(e)                        \
		namespace std { template<> struct is_error_code_enum<e>       \
			:  std::true_type {};                                     \
		} /*ENDM*/
/** Helper: mark enum \a e as an error condition
 *
 * Mark enum \a as an error condition by definiting the specialization
 * `::std::is_error_condition_enum<e>` to be `std::true_type`.
 *
 * \warning This macro must be placed in the global namespace!
 */
#	define FLUX_GLOBAL_MAKE_ERROR_CONDITION_ENUM(e)                    \
		namespace std { template<> struct is_error_condition_enum<e>  \
			:  std::true_type {};                                     \
		} /*ENDM*/

#else // nih_
#	include <flux/nih_/system_error.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::errc;

	using nih_::error_code;
	using nih_::error_condition;
	using nih_::error_category;

	using nih_::system_category;
	using nih_::generic_category;

	using nih_::make_error_code;
	using nih_::make_error_condition;

	using nih_::system_error;

	using nih_::is_error_code_enum;
	using nih_::is_error_condition_enum;
FLUX_NAMESPACE_LEAVE1(compat)

#	define FLUX_GLOBAL_MAKE_ERROR_CODE_ENUM(e)                        \
		FLUX_GLOBAL_NIH_MAKE_ERROR_CODE_ENUM(e)                       \
		/*ENDM*/
#	define FLUX_GLOBAL_MAKE_ERROR_CONDITION_ENUM(e)                   \
		FLUX_GLOBAL_NIH_MAKE_ERROR_CONDITION_ENUM(e)                  \
		/*ENDM*/

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_SYSTEM_ERROR_HPP_346F37E8_599D_4413_8C35_70FF77A3A6A5
