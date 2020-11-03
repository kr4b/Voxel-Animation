/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Hardware clock implementations
 */
/*-***************************************************************** -}}}1- */

#include <flux/setup/arch.hpp>
#include <flux/setup/compiler.hpp>

#if FLUX_ARCH == FLUX_ARCH_X86_32 || FLUX_ARCH == FLUX_ARCH_X86_64
#	undef FLUX_SYS_HWCLOCK_SUPPORTED
#	define FLUX_SYS_HWCLOCK_SUPPORTED 1

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
HwClockTicks hwclock_now() noexcept
{
	uint32_t aux;
	auto const ret = __rdtscp( &aux );
	_mm_lfence();
	return ret;
}

#else // Unknown architecture
FLUX_ATTR_ALWAYS_INLINE
HwClockTicks hwclock_now() noexcept
{
	return HwClockTicks{};
}
#endif // ~ ARCH

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
