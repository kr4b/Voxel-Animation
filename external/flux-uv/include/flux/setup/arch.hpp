/*-******************************************************* -- HEADER -{{{1- */
/*-	Target architecture
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SETUP_ARCH_HPP_BCF0EA28_07DA_40CA_90F6_EE820042002F
#define FLUX_SETUP_ARCH_HPP_BCF0EA28_07DA_40CA_90F6_EE820042002F

//--//////////////////////////////////////////////////////////////////////////
//--    Include                     ///{{{1///////////////////////////////////

#include "defaults.hpp"
#include "compiler.hpp"

//--    Target ISA                  ///{{{1///////////////////////////////////

#define FLUX_ARCH_UNKNOWN             0
#define FLUX_ARCH_X86_32              1
#define FLUX_ARCH_X86_64              2
#define FLUX_ARCH_THUMB               10
// XXX-Thumb2?
#define FLUX_ARCH_ARM                 20
#define FLUX_ARCH_ARM64               21
#define FLUX_ARCH_PTX                 30 /* CUDA PTX; requires FLUX_NVCC_PASS_DEVICE */


/** Identifies target ISA
 *
 * `FLUX_ARCH` identifies the target architecture.
 *
 * The value of `FLUX_ARCH` is determined from
 *   - `FLUXCFG_ARCH` (if defined)
 *   - default pre-defined macros (otherwise)
 *
 * See [Pre-defined Macros](http://sourceforge.net/p/predef/wiki/Architectures/)
 * for pre-defined macros used to determine `FLUX_ARCH`.
 *
 * \see FLUXCFG_ARCH
 */
#define FLUX_ARCH              FLUX_ARCH_UNKNOWN /*OVERRIDDEN_ELSEWHERE*/

//--    ISA Versions                ///{{{1///////////////////////////////////

/** ARM Version
 *
 * Only defined if `FLUX_ARCH == FLUX_ARCH_ARM`. 
 *
 *
 * \todo What about `FLUX_ARCH_THUMB`?
 */
#define FLUX_ARCH_ARM_VERSION         0 /*OVERRIDDEN_ELSEWHERE*/
/** PTX Version
 *
 * Only defined if `FLUX_ARCH == FLUX_ARCH_PTX`. In that case it's equivalent
 * to `__CUDA_ARCH__`. 
 *
 * \note `FLUX_ARCH == FLUX_ARCH_PTX` only when `FLUX_NVCC_ACTIVE_PASS` equals
 * `FLUX_NVCC_PASS_DEVICE`, i.e., only when processing device code.
 *
 * \see FLUX_NVCC_ACTIVE_PASS
 */
#define FLUX_ARCH_PTX_VERSION         0 /*OVERRIDDEN_ELSEWHERE*/

//--    ISA Features                ///{{{1///////////////////////////////////

/** ARM: ABI with hardware floating point
 */
#define FLUX_ARCH_ARM_HARDFP_ABI   0 /*OVERRIDDEN_ELSEWHERE*/

/** ARM: Hardware single-precision floating point 
 */
#define FLUX_ARCH_ARM_HARDFP       0 /*OVERRIDDEN_ELSEWHERE*/
/** ARM: Hardware double-precision floating point 
 */
#define FLUX_ARCH_ARM_HARDFP_DBL   0 /*OVERRIDDEN_ELSEWHERE*/
/** ARM: Hardware half-precision floating point 
 */
#define FLUX_ARCH_ARM_HARDFP_HALF  0 /*OVERRIDDEN_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/arch.hxx"
#endif // FLUX_SETUP_ARCH_HPP_BCF0EA28_07DA_40CA_90F6_EE820042002F
