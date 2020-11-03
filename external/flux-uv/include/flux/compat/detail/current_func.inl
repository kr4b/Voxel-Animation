/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Implementation: FLUX_COMPAT_FUNCTION_*
 */
/*-***************************************************************** -}}}1- */

#include <flux/setup/compiler.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    FUNCTION_*                  ///{{{1///////////////////////////////////

#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#	undef FLUX_COMPAT_FUNCTION_NAME
#	define FLUX_COMPAT_FUNCTION_NAME __func__

#	undef FLUX_COMPAT_FUNCTION_PROTO
#	define FLUX_COMPAT_FUNCTION_PROTO __PRETTY_FUNCTION__

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#	undef FLUX_COMPAT_FUNCTION_NAME
#	define FLUX_COMPAT_FUNCTION_NAME __func__

#	undef FLUX_COMPAT_FUNCTION_PROTO
#	define FLUX_COMPAT_FUNCTION_PROTO __PRETTY_FUNCTION__

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#	undef FLUX_COMPAT_FUNCTION_NAME
#	define FLUX_COMPAT_FUNCTION_NAME __FUNCTION__

#	undef FLUX_COMPAT_FUNCTION_PROTO
#	define FLUX_COMPAT_FUNCTION_PROTO __FUNCSIG__

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_INTEL
#	undef FLUX_COMPAT_FUNCTION_NAME
#	define FLUX_COMPAT_FUNCTION_NAME __func__

#	undef FLUX_COMPAT_FUNCTION_PROTO
#	define FLUX_COMPAT_FUNCTION_PROTO __PRETTY_FUNCTION__

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_PGI
#	undef FLUX_COMPAT_FUNCTION_NAME
#	define FLUX_COMPAT_FUNCTION_NAME __func__

#	undef FLUX_COMPAT_FUNCTION_PROTO
#	define FLUX_COMPAT_FUNCTION_PROTO __PRETTY_FUNCTION__

#else // unknown compiler

/* Other compilers: no implementation */

#endif // ~ compiler

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
