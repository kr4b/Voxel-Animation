/*-******************************************************* -- HEADER -{{{1- */
/*-	flux.ext_misc configuration
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXEXT_MISC_DEFAULTS_HPP_D62B5E5F_30F5_4290_94BB_DDFDC8ED2317
#define FLUXEXT_MISC_DEFAULTS_HPP_D62B5E5F_30F5_4290_94BB_DDFDC8ED2317

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

//--    defaults                    ///{{{1///////////////////////////////////

/** Config: Enable the use of "vk_mem_alloc" library
 *
 * "vk_mem_alloc" is the Vulkan memory allocator library by AMD.
 *
 * Use of vk_mem_alloc is enabled if Vulkan suppport is enabled. 
 */
#if !defined(FLUXCFG_EXT_WITH_VKMEMALLOC)
#	define FLUXCFG_EXT_WITH_VKMEMALLOC FLUX_WITH_FLUXVK
#endif

/** Config: Disable use of bundled "vk_mem_alloc" implementation.
 *
 * If enabled, the bundled Vulkan memory allocator implementation will not be
 * built, and an external implementation must be linked in by the end-user.
 * This facilitates use of flux with other projects that already include the
 * Vulkan memory allocator.  (`FLUXCFG_EXT_VKMEMALLOC_HEADER` may additional be
 * used to include a different `vk_mem_alloc.h` header, to avoid version
 * mismatches.)
 */
#if !defined(FLUXCFG_EXT_VKMEMALLOC_NO_IMPLEMENTATION)
#	define FLUXCFG_EXT_VKMEMALLOC_NO_IMPLEMENTATION 0
#endif


/** Config: Enable the use of "miniz" library
 *
 * "miniz" is a library for compressing/decompressing zlib-like compression.
 * It additionally supports reading (and writing) .zip archives, and provides
 * facilities for compressing PNG files.
 *
 * Use of miniz is enabled by default.
 */
#if !defined(FLUXCFG_EXT_WITH_MINIZ)
#	define FLUXCFG_EXT_WITH_MINIZ 1
#endif

/** Config: Disable use of bundled "miniz" implementation.
 *
 * If enabled, the bundled miniz implementation will not be built, and an
 * external miniz implementation must be linked in by the end-user. This
 * facilitates use of flux with other projects that already include miniz.
 * (`FLUXCFG_EXT_MINIZ_HEADER` may additional be used to include a different
 * `miniz.h` header, to avoid version mismatches.)
 */
#if !defined(FLUXCFG_EXT_MINIZ_NO_IMPLEMENTATION)
#	define FLUXCFG_EXT_MINIZ_NO_IMPLEMENTATION 0
#endif


/** Config: Enable the use of zstd compression
 *
 * ZStd is a recent compression method that often outperforms e.g., zlib-like
 * methods. See https://github.com/facebook/zstd for additional information.
 *
 * Use of zstd is enabled by default.
 */
#if !defined(FLUXCFG_EXT_WITH_ZSTD)
#	define FLUXCFG_EXT_WITH_ZSTD 1
#endif

/** Config: Disable use of bundled "zstd" implementation.
 *
 * If enabled, the bundled zstd implementation will not be built, and an
 * external zstd implementation must be linked in by the end-user. This
 * facilitates use of flux with other projects that already include zstd.
 * (`FLUXCFG_EXT_ZSTD_HEADER` may additional be used to include a different
 * `zstd.h` header, to avoid version mismatches.)
 */
#if !defined(FLUXCFG_EXT_ZSTD_NO_IMPLEMENTATION)
#	define FLUXCFG_EXT_ZSTD_NO_IMPLEMENTATION 0
#endif

/** Config: Enable the use of "monocypher" library
 *
 * "monocypher" provides several cryptographic primitives. While less well
 * analyzed and tested than e.g., (Tweet)NaCl, it comes with documentation that
 * doesn't suck.
 *
 * Use of monocypher is enabled by default.
 */
#if !defined(FLUXCFG_EXT_WITH_MONOCYPHER)
#	define FLUXCFG_EXT_WITH_MONOCYPHER 1
#endif

/** Config: Disable use of bundled "monocypher" implementation.
 *
 * If enabled, the bundled monocypher implementation will not be built, and an
 * external monocypher implementation must be linked in by the end-user. This
 * facilitates use of flux with other projects that already include monocypher.
 * (`FLUXCFG_EXT_MONOCYPHER_HEADER` may additional be used to include a
 * different `monocypher.h` header, to avoid version mismatches.)
 */
#if !defined(FLUXCFG_EXT_MONOCYPHER_NO_IMPLEMENTATION)
#	define FLUXCFG_EXT_MONOCYPHER_NO_IMPLEMENTATION 0
#endif


/** Config: Enable the use of the "pegtl" library
 *
 * PEGTL (Parsing Expression Grammar Template Library) makes parsing text more
 * humane in C++.
 *
 * Use of PEGTL is enabled by default.
 */
#if !defined(FLUXCFG_EXT_WITH_PEGTL)
#	define FLUXCFG_EXT_WITH_PEGTL 1
#endif

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXEXT_MISC_DEFAULTS_HPP_D62B5E5F_30F5_4290_94BB_DDFDC8ED2317
