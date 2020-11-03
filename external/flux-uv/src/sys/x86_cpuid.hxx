#ifndef DETAIL_X86_CPUID_HXX_B3209434_6FBD_4EED_81C2_D28B31D515FE
#define DETAIL_X86_CPUID_HXX_B3209434_6FBD_4EED_81C2_D28B31D515FE

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/arch.hpp>

#include <flux/std/inttypes.hpp>

//--    >>> namespace = flux::sys::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(sys,detail)
//--    macros                              ///{{{2///////////////////////////

#if FLUX_ARCH == FLUX_ARCH_X86_32 || FLUX_ARCH == FLUX_ARCH_X86_64
#	define FLUX_SYS_WITH_X86CPUID 1
#else // !x86
#	define FLUX_SYS_WITH_X86CPUID 0
#endif // ~ ARCH

//--    X86CpuId                            ///{{{2///////////////////////////

struct X86CpuId
{
	uint32_t eax, ebx, ecx, edx;
	X86CpuId( uint32_t aEax, uint32_t aEcx = 0 ) noexcept;
};

//--    <<< ~ flux::sys::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(sys,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_X86_CPUID_HXX_B3209434_6FBD_4EED_81C2_D28B31D515FE
