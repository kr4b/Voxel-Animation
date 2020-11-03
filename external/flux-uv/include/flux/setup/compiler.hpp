/*-******************************************************* -- HEADER -{{{1- */
/*-	Compiler identification and basic capabilities
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SETUP_COMPILER_HPP_0F319B80_006D_42AC_A14F_941559D383AB
#define FLUX_SETUP_COMPILER_HPP_0F319B80_006D_42AC_A14F_941559D383AB

//--//////////////////////////////////////////////////////////////////////////
//--    Include                     ///{{{1///////////////////////////////////

#include "defaults.hpp"

//--    Compiler                    ///{{{1///////////////////////////////////

#define FLUX_COMPILER_UNKNOWN    0
#define FLUX_COMPILER_GCC        1
#define FLUX_COMPILER_CLANG      2
#define FLUX_COMPILER_MSVC       3
#define FLUX_COMPILER_PGI        4
#define FLUX_COMPILER_INTEL      5
#define FLUX_COMPILER_NVCC       255

/** Target compiler
 *
 * Target compiler. This is synonymous to `FLUX_COMPILER_TARGET`. Typically
 * this is the only value that matters - all `FLUX_COMPILER_*` constants will
 * have the same value (such as `FLUX_COMPILER_GCC`).
 *
 * The only exception (so far) is when NVCC is involved. NVCC then serves as
 * the "front-end" compiler, which passes on parts of the code to the host's
 * default compiler (that is, GCC, Clang or MSVC).
 *
 * It's common to only invoke NVCC for CUDA code. However, this code is later
 * linked with normal C++ code. For this to succeed, compatible object code 
 * must be generated regardless of whether the source code first passed through
 * NVCC or not. Therefore Flux must use the same configuration for both code
 * paths. Flux can be told that NVCC code is included in the project by setting
 * the target compiler to NVCC. Then:
 *
 *   - `FLUX_COMPILER` = `FLUX_COMPILER_COMPAT` = `FLUX_COMPILER_NVCC`
 *   - `FLUX_COMPILER_FAMILY` = (host compiler)
 *   - `FLUX_COMPILER_ACTUAL` = (host compiler) or `FLUX_COMPILER_NVCC`
 *
 * The target compiler can be configured through `FLUXCFG_COMPILER_COMPAT`.
 *
 * \see FLUXCFG_COMPILER_COMPAT, FLUX_COMPILER_COMPAT
 */
#define FLUX_COMPILER          FLUX_COMPILER_COMPAT

/** "Top-level" compiler
 *
 * Identifies the actual "top-level" compiler. This is the compiler that is
 * invoked by the user/IDE. All code must pass through this compiler.
 *
 * \see FLUX_COMPILER
 */
#define FLUX_COMPILER_ACTUAL   FLUX_COMPILER_UNKNOWN /*OVERRIDDEN_ELSEWHERE*/
/** Target compatible compiler
 *
 * The target compiler identifies the compiler that's targeted feature-wise.
 * Flux is guaranteed(*) to be compatible with at least this compiler.
 *
 * See description of `FLUX_COMPILER` for additional information.
 *
 * The value of `FLUX_COMPILER_COMPAT` is either derived from
 *
 *   - `FLUXCFG_COMPILER_COMPAT` (if defined)
 *   - `FLUX_COMPILER_ACTUAL` (otherwise)
 *
 * \see FLUX_COMPILER, FLUXCFG_COMPILER_COMPAT
 */
#define FLUX_COMPILER_COMPAT   FLUX_COMPILER_ACTUAL /*OVERRIDDEN_ELSEWHERE*/
/** Underlying compiler family
 *
 * The underlying compiler family identifies the host compiler. Normally this
 * is the same as `FLUX_COMPILER_ACTUAL`, the exception being NVCC. When NVCC
 * is involved, `FLUX_COMPILER_FAMILY` identifies the underlying host compiler
 * (e.g., one of GCC, Clang or MSVC).
 *
 * \see FLUX_COMPILER
 */
#define FLUX_COMPILER_FAMILY   FLUX_COMPILER_ACTUAL /*OVERRIDDEN_ELSEWHERE*/

//--    Language level              ///{{{1///////////////////////////////////

#define FLUX_LANGLEVEL_CXX03    3    //!< Language level: C++03
#define FLUX_LANGLEVEL_CXX11    11   //!< Language level: C++11
#define FLUX_LANGLEVEL_CXX14    14   //!< Language level: C++14
#define FLUX_LANGLEVEL_CXX17    17   //!< Language level: C++17
#define FLUX_LANGLEVEL_CXX20    20   //!< Language level: C++20

/* TODO: C++20 isn't properly detected yet, since there is no __cplusplus
 * constant assigned at the time of writing.
 */

/** Language level
 *
 * The language level identifies what C++ standard the compiler supports (or
 * rather claims to support).
 */
#define FLUX_LANGLEVEL          FLUX_LANGLEVEL_CXX11 /*OVERRIDDEN_ELSEWHERE*/

//--    Misc. Flags                 ///{{{1///////////////////////////////////

#define FLUX_NVCC_PASS_NONE      0   //!< Not compiled via NVCC
#define FLUX_NVCC_PASS_HOST      1   //!< __host__ pre-processor pass
#define FLUX_NVCC_PASS_DEVICE    2   //!< __device__ pre-processor pass

/** Identify active NVCC pass
 *
 * NVCC performs at least two pre-processing passes. For one of the passes,
 * `__CUDA_ARCH__` is undefined the preprocessed code from that pass is 
 * compiled as host code. The remaining passes produce device code for 
 * various CUDA architectures.
 *
 * `FLUX_NVCC_PASS_DEVICE` is equal to `FLUX_NVCC_PASS_DEVICE` during the
 * device passes, and equal to `FLUX_NVCC_PASS_HOST` during the host 
 * preprocessing pass. If the code is built outside of NVCC, the value
 * of `FLUX_NVCC_ACTIVE_PASS` is set to `FLUX_NVCC_PASS_NONE`.
 *
 * \note To identify any host code (regardless of compiler), use the following
 *    \code
 *    #if FLUX_NVCC_ACTIVE_PASS != FLUX_NVCC_PASS_DEVICE
 *    // host code
 *    #endif
 *    \endcode
 */
#define FLUX_NVCC_ACTIVE_PASS    FLUX_NVCC_PASS_NONE /*OVERRIDDEN_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/compiler.hxx"
#endif // FLUX_SETUP_COMPILER_HPP_0F319B80_006D_42AC_A14F_941559D383AB
