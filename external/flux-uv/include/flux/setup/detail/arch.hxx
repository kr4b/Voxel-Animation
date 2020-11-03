/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Detect and setup arch
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_ARCH_HXX_883E6A64_70CB_410D_8F4B_CEDC9C0ECD66
#define DETAIL_ARCH_HXX_883E6A64_70CB_410D_8F4B_CEDC9C0ECD66

//--//////////////////////////////////////////////////////////////////////////
//--    Detect ISA                  ///{{{1///////////////////////////////////
#if defined(FLUXCFG_ARCH)
#	undef FLUX_ARCH
#	define FLUX_ARCH FLUXCFG_ARCH

#elif FLUX_COMPILER_ACTUAL == FLUX_COMPILER_NVCC && FLUX_NVCC_ACTIVE_PASS == FLUX_NVCC_PASS_DEVICE

	/* NVCC Device Pass. Target architecture is FLUX_ARCH_PTX. Find the
	 * value for FLUX_ARCH_PTX_VERSION (=compute level)
	 */

#	undef FLUX_ARCH
#	define FLUX_ARCH FLUX_ARCH_PTX

#	undef FLUX_ARCH_PTX_VERSION
#	define FLUX_ARCH_PTX_VERSION __CUDA_ARCH__

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC || FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#	if defined(__amd64__) || defined(__x86_64__)
#		undef FLUX_ARCH
#		define FLUX_ARCH FLUX_ARCH_X86_64

#	elif defined(__i386__)
#		undef FLUX_ARCH
#		define FLUX_ARCH FLUX_ARCH_X86_32

#	elif defined(__thumb__)
#		undef FLUX_ARCH
#		define FLUX_ARCH FLUX_ARCH_THUMB

#	elif defined(__arm__)
#		undef FLUX_ARCH
#		define FLUX_ARCH FLUX_ARCH_ARM

		/* ARM Version */
#		undef FLUX_ARCH_ARM_VERSION

#		if defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__)
#			define FLUX_ARCH_ARM_VERSION 7
#		elif defined(__ARM_ARCH_6__)
#			define FLUX_ARCH_ARM_VERSION 6
#		elif defined(__ARM_ARCH_5__)
#			define FLUX_ARCH_ARM_VERSION 5
#		elif defined(__ARM_ARCH_4__)
#			define FLUX_ARCH_ARM_VERSION 4
#		elif defined(__ARM_ARCH_3__)
#			define FLUX_ARCH_ARM_VERSION 3
#		elif defined(__ARM_ARCH_2__)
#			define FLUX_ARCH_ARM_VERSION 2
#		else
#			define FLUX_ARCH_ARM_VERSION 1
#		endif // ARM Version

		/* ARM Features */
#		if defined(__ARM_PCS_VFP) && __ARM_PCS_VFP
#			undef FLUX_ARCH_ARM_HARDFP_ABI
#			define FLUX_ARCH_ARM_HARDFP_ABI 1
#		endif // ~ __ARM_PCS_VFP

#		if defined(__ARM_FP) && 0x4 == (__ARM_FP & 0x4)
#			undef FLUX_ARCH_ARM_HARDFP
#			define FLUX_ARCH_ARM_HARDFP 1
#		endif // ~ __ARM_FP
#		if defined(__ARM_FP) && 0x8 == (__ARM_FP & 0x8)
#			undef FLUX_ARCH_ARM_HARDFP_DBL
#			define FLUX_ARCH_ARM_HARDFP_DBL 1
#		endif // ~ __ARM_FP
#		if defined(__ARM_FP) && 0x2 == (__ARM_FP & 0x2)
#			undef FLUX_ARCH_ARM_HARDFP_HALF
#			define FLUX_ARCH_ARM_HARDFP_HALF 1
#		endif // ~ __ARM_FP

#	elif defined(__aarch64__)
#		undef FLUX_ARCH
#		define FLUX_ARCH FLUX_ARCH_ARM_64

#	endif // GCC Arch Detection

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#	if defined(_M_X64) || defined(_M_AMD64)
#		undef FLUX_ARCH
#		define FLUX_ARCH FLUX_ARCH_X86_64

#	elif defined(_M_IX86)
#		undef FLUX_ARCH
#		define FLUX_ARCH FLUX_ARCH_X86_32

#	elif defined(_M_ARMT)
#		undef FLUX_ARCH
#		define FLUX_ARCH FLUX_ARCH_THUMB

#	elif defined(_M_ARM)
#		undef FLUX_ARCH
#		define FLUX_ARCH FLUX_ARCH_ARM

		/* ARM Version */
#		undef FLUX_ARCH_ARM_VERSION
#		define FLUX_ARCH_ARM_VERSION _M_ARM

#	endif // MSVC Arch Detection

#endif // ~ FLUX_COMPILER_FAMILY

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_ARCH_HXX_883E6A64_70CB_410D_8F4B_CEDC9C0ECD66
