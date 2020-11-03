/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Implementation: FLUX_RESTRICT
 */
/*-***************************************************************** -}}}1- */

#include <flux/setup/compiler.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    RESTRICT                    ///{{{1///////////////////////////////////

#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#	undef FLUX_RESTRICT
#	define FLUX_RESTRICT __restrict__

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#	undef FLUX_RESTRICT
#	define FLUX_RESTRICT __restrict__

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#	undef FLUX_RESTRICT
#	define FLUX_RESTRICT __restrict

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_INTEL
/* Intel seems to accept either */
#	undef FLUX_RESTRICT
#	define FLUX_RESTRICT __restrict

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_PGI
/* PGI seems to accept either */
#	undef FLUX_RESTRICT
#	define FLUX_RESTRICT __restrict

#else // unknown compiler

/* Other compilers: no implementation */

#endif // ~ compiler

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
