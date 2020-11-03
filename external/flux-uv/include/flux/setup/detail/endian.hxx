/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Detect endian
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_ENDIAN_HXX_D7C09797_B886_4A13_90C3_08500D299A29
#define DETAIL_ENDIAN_HXX_D7C09797_B886_4A13_90C3_08500D299A29

//--//////////////////////////////////////////////////////////////////////////
//--    Detect endian               ///{{{1///////////////////////////////////

/* Ugh! There's no really good way of knowning this at compile time. We can
 * check a few different sources, though.
 */
#if defined(FLUXCFG_ENDIAN)
#	undef FLUX_ENDIAN
#	define FLUX_ENDIAN FLUXCFG_ENDIAN

#elif defined(__LITTLE_ENDIAN__) // Defined on MacOS
#	undef FLUX_ENDIAN
#	define FLUX_ENDIAN FLUX_ENDIAN_LITTLE

#elif defined(__BIG_ENDIAN__) // Defined on MacOS
#	undef FLUX_ENDIAN
#	define FLUX_ENDIAN FLUX_ENDIAN_BIG

#elif FLUX_ARCH == FLUX_ARCH_X86_32 || FLUX_ARCH == FLUX_ARCH_X86_64
#	undef FLUX_ENDIAN
#	define FLUX_ENDIAN FLUX_ENDIAN_LITTLE

#endif // endian detection, part I

#if !defined(FLUXCFG_ENDIAN) && FLUX_ENDIAN == FLUX_ENDIAN_UNKNOWN
#	if defined(__has_include) && __has_include(<endian.h>)
#		include <endian.h>

#		if __BYTE_ORDER == __LITTLE_ENDIAN
#			undef FLUX_ENDIAN
#			define FLUX_ENDIAN FLUX_ENDIAN_LITTLE

#		elif __BYTE_ORDER == __BIG_ENDIAN
#			undef FLUX_ENDIAN
#			define FLUX_ENDIAN FLUX_ENDIAN_BIG

#		endif // ~ __BYTE_ORDER
#endif // ~ __has_include(<endian.h>)

#endif // ~ endian detection, part II

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_ENDIAN_HXX_D7C09797_B886_4A13_90C3_08500D299A29
