/*-******************************************************* -- HEADER -{{{1- */
/*-	Miscellaneous Intrinsics
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_INTRIN_HPP_316EA291_C9B7_40E1_9EAF_C6907191BACE
#define FLUX_SYS_INTRIN_HPP_316EA291_C9B7_40E1_9EAF_C6907191BACE

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/util/attrib.hpp>

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    functions                   ///{{{1///////////////////////////////////

/** \brief Portable `_mm_pause()`
 *
 * Portable version of the x86 `_mm_pause()`. Does nothing on platforms that
 * do not expose/support the functionality.
 */
FLUX_ATTR_ALWAYS_INLINE
void hwpause() noexcept;

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/intrin.inl"
#endif // FLUX_SYS_INTRIN_HPP_316EA291_C9B7_40E1_9EAF_C6907191BACE
