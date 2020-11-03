/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Intent implementations
 */
/*-***************************************************************** -}}}1- */

#include <flux/setup.pkg>
#include <flux/setup/compiler.hpp>

#if FLUX_DEBUG
#	include <flux/std/assert.hpp>
#endif

//--//////////////////////////////////////////////////////////////////////////
//--    INTENT_UNREACHABLE()        ///{{{1///////////////////////////////////

#if FLUX_DEBUG
#	undef FLUX_INTENT_UNREACHABLE
#	define FLUX_INTENT_UNREACHABLE()   FLUX_ASSERT_TRIGGER("Unreachable code")
 
#else // !FLUX_DEBUG
#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#		undef FLUX_INTENT_UNREACHABLE
#		define FLUX_INTENT_UNREACHABLE() __builtin_unreachable()

#	elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#		undef FLUX_INTENT_UNREACHABLE
#		define FLUX_INTENT_UNREACHABLE() __builtin_unreachable()

#	elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#		undef FLUX_INTENT_UNREACHABLE
#		define FLUX_INTENT_UNREACHABLE() __assume(0); do {} while(1)

/* See http://msdn.microsoft.com/en-us/library/1b3fsfxw(v=vs.100).aspx for
 * this use of __assume(0).
 */

#	elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_INTEL
#		undef FLUX_INTENT_UNREACHABLE
#		define FLUX_INTENT_UNREACHABLE() __builtin_unreachable()

#	elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_PGI
/* Recognizes but ignores __builtin_unreachable(). This may cause additional
 * warnings if the compiler correctly deduces that said code is unreachable.
 *
 * => Leave empty
 */
#		undef FLUX_INTENT_UNREACHABLE
#		define FLUX_INTENT_UNREACHABLE()

#	else // unknown compiler
#		undef FLUX_INTENT_UNREACHABLE
#		define FLUX_INTENT_UNREACHABLE()

#	endif // ~ FLUX_COMPILER_FAMILY
#endif // ~ FLUX_DEBUG

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
