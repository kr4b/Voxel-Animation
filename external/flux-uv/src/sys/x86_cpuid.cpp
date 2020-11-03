/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	X86 CPUID helpers
 */
/*-***************************************************************** -}}}1- */

#include "x86_cpuid.hxx"

#include <flux/setup/compiler.hpp>

#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#	include <intrin.h>
#endif // ~ COMPILER_MSVC

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(sys,detail)
//--    X86CpuId                            ///{{{2///////////////////////////
X86CpuId::X86CpuId( uint32_t aEax, uint32_t aEcx ) noexcept
	: eax(0), ebx(0), ecx(0), edx(0)
#if !FLUX_SYS_WITH_X86CPUID
{}
#else // WITH_X86CPUID
{
#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
	int res[4];
	__cpuidex( res, aEax, aEcx );

	eax = res[0];
	ebx = res[1];
	ecx = res[2];
	edx = res[3];
#	else // !COMPILER_MSVC
	__asm__(
		"cpuid"
		: "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
		: "a"(aEax), "c"(aEcx)
	);
#	endif // ~ COMPILER
}
#endif // ~ WITH_X86CPUID

//--    <<< ~ flux::sys::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(sys,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
