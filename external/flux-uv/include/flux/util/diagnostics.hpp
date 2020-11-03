/*-******************************************************* -- HEADER -{{{1- */
/*-	Control diagnostics.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_UTIL_DIAGNOSTICS_HPP_32C31821_1592_44EA_8A8E_E24CEB8BEBD0
#define FLUX_UTIL_DIAGNOSTICS_HPP_32C31821_1592_44EA_8A8E_E24CEB8BEBD0

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/compiler.hpp>
#include <flux/setup/normalize.hpp>

#include <flux/pp/join.hpp>

//--    macros                      ///{{{1///////////////////////////////////

	// Diagnostic Modes -{{{2-
/** Diagnostic mode: ignore / suppress warnings
 *
 * Suppress diagnostics (i.e., do not emit warning or similar).
 */
#define FLUX_DIAGNOSTIC_MODE_IGNORED           0
/** Diagnostic mode: emit warning once
 *
 * Emit warning once. Not supported by all compilers (e.g., GCC). On 
 * unsupported compilers, `FLUX_DIAGNOSTIC_MODE_ONCE` is equivalent to
 * `FLUX_DIAGNOSTIC_MODE_WARNING`.
 */
#define FLUX_DIAGNOSTIC_MODE_ONCE              1
/** Diagnostic mode: emit warnings
 *
 * Always emit warnings.
 */
#define FLUX_DIAGNOSTIC_MODE_WARNING           2
/** Diagnostic mode: emit error
 *
 * Emit errors instead of warnings. Typically this will prevent compilation
 * from continuing.
 */
#define FLUX_DIAGNOSTIC_MODE_ERROR             3

	// Diagnostic IDs -{{{2-
/** Diagnostic: deprecation
 *
 * Control diagnostics related to deprecation (see `FLUX_ATTR_DEPRECATED`).
 * Suppressing deprecation warnings can be useful for implementations of 
 * deprecated classes, which can result in many repeated warnings that are
 * less interesting.
 *
 * \see FLUX_ATTR_DEPRECATED
 */
#define FLUX_DIAGNOSTIC_ID_DEPRECATION         FLUX_DIAGNOSTIC_UNSUPPORTED

/** Diagnostic: unused function arguments
 */
#define FLUX_DIAGNOSTIC_ID_UNUSED_ARGUMENT     FLUX_DIAGNOSTIC_UNSUPPORTED
/** Diagnostic: unused variables
 */
#define FLUX_DIAGNOSTIC_ID_UNUSED_VARIABLE     FLUX_DIAGNOSTIC_UNSUPPORTED
/** Diagnostic: unused functions
 */
#define FLUX_DIAGNOSTIC_ID_UNUSED_FUNCTION     FLUX_DIAGNOSTIC_UNSUPPORTED

/** Diagnostic: unreachable code
 */
#define FLUX_DIAGNOSTIC_ID_UNREACHABLE_CODE    FLUX_DIAGNOSTIC_UNSUPPORTED

/** Diagnostic: ambiguous 'else'
 */
#define FLUX_DIAGNOSTIC_ID_AMBIGUOUS_ELSE      FLUX_DIAGNOSTIC_UNSUPPORTED

/** Diagnostic: unneeded internal declaration
 *
 * Clang: "warning: function XYZ is not needed and will not be emitted".
 * Generated for a lot of the helper methods in flux::mirror::
 */
#define FLUX_DIAGNOSTIC_ID_UNNEEDED_INTERNAL   FLUX_DIAGNOSTIC_UNSUPPORTED

/** Diagnostic: undefined #%define
 *
 * \warning All current versions of GCC happily ignore this; see
 *   https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53431
 * tl;dr: the preprocessor doesn't know about the GCC pragmas, so they cannot
 * affect the preprocessor. Enabling the preprocessor to parse pragmas is
 * a PITA so nobody wants to fix that.
 */
#define FLUX_DIAGNOSTIC_ID_UNDEFINED_MACRO     FLUX_DIAGNOSTIC_UNSUPPORTED

/** Diagnostic: `sizeof()` on an expression
 *
 * MSVC may complain about C6334, when applying `sizeof()` to an expression,
 * since, if you're not aware of the rules, it may not do what you want it
 * to (but, alas, welcome to C++). 
 *
 * Unfortunately, applying `sizeof()` to an expression is a common technique
 * used when e.g., detecting presence of member variables or functions.
 */
#define FLUX_DIAGNOSTIC_ID_SIZEOF_EXPR         FLUX_DIAGNOSTIC_UNSUPPORTED
/** Diagnostic: comma-operator in tested expression ignores left side
 *
 * MSVC may complain about C6319 when doing something like `if( a(), b )`, 
 * which, of course, ignores whatever `a()` might or might not return, in 
 * favour of `b`. 
 *
 * A useful warning, except when this is exactly what we're trying to do.
 */
#define FLUX_DIAGNOSTIC_ID_COMMA_IN_IF         FLUX_DIAGNOSTIC_UNSUPPORTED
/** Diagnostic: noreturn function does return
 *
 * Particularly in combination with NVCC, the compilers occasionally miss
 * that `[[noreturn]]` methods actually never will return, and complain
 * (mistakenly) that they may return.
 * 
 * Unfortuantely, `FLUX_ASSERT()` intrduces such a case, which results in
 * large amounts of spam.
 */
#define FLUX_DIAGNOSTIC_ID_NORETURN_RETURNS   FLUX_DIAGNOSTIC_UNSUPPORTED
/** Diagnostic: zero as null pointer constant
 *
 * Suppress warnings enabled by `-Wzero-as-null-pointer-constant`.
 */
#define FLUX_DIAGNOSTIC_ID_ZERO_AS_NULLPTR    FLUX_DIAGNOSTIC_UNSUPPORTED

/** Diagnostic: copying an object of non-trivial type 
 *
 * Suppress warnings enabled by `-Wclass-memaccess`. This is especially
 * prevalent in some versions of the `vulkan.hpp` header, and our stuff in
 * flux-ftl.
 */
#define FLUX_DIAGNOSTIC_ID_CLASS_MEMACCESS    FLUX_DIAGNOSTIC_UNSUPPORTED

/** Diagnostic: regarding sign+promotion
 *
 * Suppress warnings enabled by `-Wsign-promo`. This seems to occur to some
 * degree in the fmtlib header.
 */
#define FLUX_DIAGNOSTIC_ID_SIGN_PROMO         FLUX_DIAGNOSTIC_UNSUPPORTED

/** Diagnostic: strict aliasing violations
 *
 * Disable warnings regarding strict aliasing violations. This occurs in the
 * miniz sources.
 *
 * \warning While none of these should be used in the flux proper, this really
 * really really shouldn't be used. Respect strict aliasing!
 */
#define FLUX_DIAGNOSTIC_ID_STRICT_ALIASING    FLUX_DIAGNOSTIC_UNSUPPORTED

/** Diagnostic: restrict violated
 *
 * Recent GCCs warn if we pass a pointer both via a restrict and via a non
 * restrict argument. The warning is valid, however in one of the test cases
 * this is used to catch some extra zealous compilers.
 */
#define FLUX_DIAGNOSTIC_ID_RESTRICT           FLUX_DIAGNOSTIC_UNSUPPORTED

/** Diagnostic: type limits
 *
 * "-Wtype-limits" on GCC; this results in e.g., "warning: comparison of
 * unsigned expression < 0 is always false". This occurs in the Vulkan
 * Memory Allocator when `VMA_DEBUG_MARGIN` is set to its default value of
 * zero.
 */
#define FLUX_DIAGNOSTIC_ID_TYPE_LIMITS        FLUX_DIAGNOSTIC_UNSUPPORTED

/** Diagnostic: missing field initializers
 *
 * "-Wmissing-field-initializers" on Clang. The syntax used in the Vulkan
 * Memory Allocator results in a number of these.
 */
#define FLUX_DIAGNOSTIC_ID_MISSING_FIELD_INIT FLUX_DIAGNOSTIC_UNSUPPORTED

/** Miscellaneous extra diagnostics
 *
 * "-Wextra" on GCC. `miniz.c` generates a few warnings from -Wextra that
 * do not seem to be covered otherwise.
 */
#define FLUX_DIAGNOSTIC_ID_EXTRA              FLUX_DIAGNOSTIC_UNSUPPORTED

/** Diagnostic: implicit fall-through in switch
 *
 * "-Wimplicit-fallthrough` generates a bunch of warnings in external
 * libraries.
 */
#define FLUX_DIAGNOSTIC_ID_IMPLICIT_FALLTHROUGH FLUX_DIAGNOSTIC_UNSUPPORTED

/** Conversion from type1 to type2, possible loss of data
 *
 * Warning C4242 in Visual Studio: implicit conversion from type1 to type2.
 *
 * \note Apparently doesn't include conversions from std::size_t to other 
 * types in Visual Studio, as that is a separate warning (C4267, or
 * `FLUX_DIAGNOSTIC_ID_CONVERSION_SIZET`).
 */
#define FLUX_DIAGNOSTIC_ID_CONVERSION           FLUX_DIAGNOSTIC_UNSUPPORTED
/** Conversion from size_t to type2, possible loss of data
 *
 * Warning C4242 in Visual Studio: implicit conversion from size_t to type2.
 *
 * \note For general types, see `FLUX_DIAGNOSTIC_ID_CONVERSION`. Visual 
 * Studio treats size_t as something special.
 */
#define FLUX_DIAGNOSTIC_ID_CONVERSION_SIZET     FLUX_DIAGNOSTIC_UNSUPPORTED

/** Unnecessary parentheses in declaration
 *
 * Unecessary parentheses in a declaration; e.g. `for( size_t (i) = ...`).
 * Monocypher does this in several macros.
 */
#define FLUX_DIAGNOSTIC_ID_DECLARATION_PARENS   FLUX_DIAGNOSTIC_UNSUPPORTED

/** Type qualifiers ignored on **cast** result type
 *
 * GCC warns (rightly) on casts like `(int const)someVar`, where the type
 * qualifier doesn't affect the result. This occurs frequently in ZSTD.
 *
 * \note This is different from `(int const&)someVar`.
 */
#define FLUX_DIAGNOSTIC_ID_CAST_TYPE_QUALIFIERS FLUX_DIAGNOSTIC_UNSUPPORTED

/** Explicitly assigning value to itself
 *
 * Clang warning for self-assignment with overloaded assignment operator.
 * This occurs in some tests.
 */
#define FLUX_DIAGNOSTIC_ID_SELF_ASSIGN          FLUX_DIAGNOSTIC_UNSUPPORTED

	// Manipulate diagnostic state -{{{2-
/** Set diagnostic state
 *
 * Change diagnostic messages for \a id to \a mode. Note that this macro takes
 * short names for both \a mode and \a id. Example:
   \code
   FLUX_DIAGNOSTIC_STATE_SET(IGNORED,UNREACHABLE_CODE)
   \endcode
 * The above example causes warnings about unreachable code to be suppressed.
 *
 * The \a mode argument should be one of
 *   - `IGNORED` (expanded to `FLUX_DIAGNOSTIC_MODE_IGNORED`)
 *   - `ONCE` (expanded to `FLUX_DIAGNOSTIC_MODE_ONCE`)
 *   - `WARNING` (expanded to `FLUX_DIAGNOSTIC_MODE_WARNING`)
 *   - `ERROR` (expanded to `FLUX_DIAGNOSTIC_MODE_ERROR`)
 *
 * The \a id argument should be the part following `FLUX_DIAGNOSTIC_ID_` 
 * matching one of the predefined `FLUX_DIAGNOSTIC_ID_*` constants.
 *
 * \note It is suggested that `FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET()` and
 * `FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT()` are used whenever possible.
 *
 * \see FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET
 */
#define FLUX_DIAGNOSTIC_STATE_SET(mode,id)                         \
	FLUX_DIAGNOSTIC_STATE_SET_RAW(                                 \
		FLUX_PP_JOIN(FLUX_DIAGNOSTIC_MODE_,mode),                  \
		FLUX_PP_JOIN(FLUX_DIAGNOSTIC_ID_,id)                       \
	) /*ENDM*/

/** Store current settings for diagnostics
 *
 * Diagnostics settings are later restored with  `FLUX_DIAGNOSTIC_STATE_POP()`.
 *
 * \note Not all compilers support this (e.g., NVCC). Therefore it is suggested
 * that (whenever possible), pairs of `FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET()`
 * and `FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT()` be used instead of the plain
 * `FLUX_DIAGNOSTIC_STATE_PUSH()` and `FLUX_DIAGNOSTIC_STATE_POP()`.
 *
 * \see FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET FLUX_DIAGNOSTIC_STATE_POP
 */
#define FLUX_DIAGNOSTIC_STATE_PUSH()                /*IMPLEMENTED_ELSEWHERE*/
/** Restore diagnostics settings
 *
 * Diagnostics settings must have been previously stored with
 * `FLUX_DIAGNOSTIC_STATE_PUSH()`.
 */
#define FLUX_DIAGNOSTIC_STATE_POP()                 /*IMPLEMENTED_ELSEWHERE*/

/** Store current diagnostics settings and change diagnostic \a id to \a mode
 *
 * Diagnostic settings can later be restored with 
 * `FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT()`. 
 */
#define FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET(mode,id)                \
	FLUX_DIAGNOSTIC_STATE_PUSH()                                   \
	FLUX_DIAGNOSTIC_STATE_SET(mode,id)                             \
	/*ENDM*/
/** Restore current diagnostics settings or reset diagnostic \a id to default
 *
 * Diagnostics settings must have been previously stored with
 * `FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET()`.
 *
 * \note Not all compilers support storing settings (e.g., NVCC). In this case,
 * `FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT()` restores the diagnostic \a id to
 * its default behaviour.
 */
#define FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT(id)                   \
	FLUX_DIAGNOSTIC_STATE_POP()                                    \
	/*ENDM*/

/** Set diagnostic state
 *
 * See `FLUX_DIAGNOSTIC_STATE_SET`. Unlike `FLUX_DIAGNOSTIC_STATE_SET`, 
 * `FLUX_DIAGNOSTIC_STATE_SET_RAW` doesn't concatenate its arguments with
 * the `FLUX_DIAGNOSTIC_MODE_` and `FLUX_DIAGNOSTIC_ID_` prefixes, 
 * respectively.
 *
 * \see FLUX_DIAGNOSTIC_STATE_SET
 */
#define FLUX_DIAGNOSTIC_STATE_SET_RAW(mode,id)      /*IMPLEMENTED_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/diagnostics.inl"
#endif // FLUX_UTIL_DIAGNOSTICS_HPP_32C31821_1592_44EA_8A8E_E24CEB8BEBD0
