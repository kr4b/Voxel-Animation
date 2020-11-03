/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Miscellaneous Intrinsics, implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/setup/arch.hpp>
#include <flux/setup/compiler.hpp>

#if FLUX_ARCH == FLUX_ARCH_X86_32 || FLUX_ARCH == FLUX_ARCH_X86_64
#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#		include <intrin.h>
#	else // !MSVC
#		include <x86intrin.h>
#	endif // ~ COMPILER
#endif // ~ ARCH

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    hwclock_now()                       ///{{{2///////////////////////////
#if FLUX_ARCH == FLUX_ARCH_X86_32 || FLUX_ARCH == FLUX_ARCH_X86_64
FLUX_ATTR_ALWAYS_INLINE
void hwpause() noexcept
{
	_mm_pause();
}

#else // Unknown architecture
FLUX_ATTR_ALWAYS_INLINE
void hwpause() noexcept
{
}
#endif // ~ ARCH

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
