/*-******************************************************* -- HEADER -{{{1- */
/*-	flux.ftl configuration
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_DEFAULTS_HPP_86B69953_2B60_449D_B593_6BC9836BF510
#define FLUX_FTL_DEFAULTS_HPP_86B69953_2B60_449D_B593_6BC9836BF510

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

//--    defaults                    ///{{{1///////////////////////////////////

/** \name Settings for FLUXCFG_FTL_BITWISE
 * \brief Permissible values for `FLUXCFG_FTL_BITWISE`.
 */
//@{
#define FLUX_FTL_BITWISE_USER 0
#define FLUX_FTL_BITWISE_TRIVIAL 1
#define FLUX_FTL_BITWISE_NEVER 2
//@}

/** Config: control `is_bitwise_*`
 *
 * Controls the `is_bitwise_*<>` traits. There are three possible settings:
 *  - `FLUX_FTL_BITWISE_USER`: default behaviour. The `is_bitwise_*<>` traits
 *    derive their default values from `is_trivially_<>` as described in their
 *    documentation. Additionally, types may be tagged via `tag_is_bitwise_*()`
 *    functions that will force `is_bitwise_*<>` to be true.
 *  - `FLUX_FTL_BITWISE_TRIVIAL`: The traits derive their values from the
 *    `is_trivially_*<>` traits as for `FLUX_FTL_BITWISE_USER`, but manual
 *    tagging is ignored.
 *  - `FLUX_FTL_BITWISE_NEVER`: Force `is_bitwise_*<>` to always evaluate to
 *    false.
 *
 * Note that the default setting `FLUX_FTL_BITWISE_USER` causes code with
 * implementation defined (or undefined) behaviour to be emitted. Manually
 * tagged types thus require exhaustive testing. This can be avoided by
 * choosing `FLUX_FTL_BITWISE_TRIVIAL`.
 */
#if !defined(FLUXCFG_FTL_BITWISE)
#	define FLUXCFG_FTL_BITWISE FLUX_FTL_BITWISE_USER
#endif


/** Config: use custom `realloc` allocator?
 *
 * Control whether or not the custom `realloc()`-enabled allocator should be
 * used in `#flux::ftl::Vector` and `#flux::ftl::VectorBase`. By default, the
 * realloc-enabled allocator is used for types that are
 * `is_bitwise_relocatable<>`. It can be disabled by defining
 * `FLUXCFG_FTL_REALLOC_ALLOC` to zero.
 *
 * \see Vector, VectorBase, FLUXCFG_FTL_BITWISE, is_bitwise_relocatable
 */
#if !defined(FLUXCFG_FTL_REALLOC_ALLOC)
#	define FLUXCFG_FTL_REALLOC_ALLOC 1
#endif

/** Config: attempt to detect contiguous iterators?
 *
 * Controls whether or not the `#flux::ftl::is_contiguous_iterator<>` meta
 * function should attempt to detect contiguous iterators via fragile
 * platform-specific means.
 *
 * Enabling this allows `#flux::ftl::is_contiguous_iterator<>` to return true
 * for iterators from standard containers like `std::vector` where no real
 * support for such detection is provided. Disabling it avoids fragile platform
 * specfic code.
 */
#if !defined(FLUXCFG_FTL_FRAGILE_CONTIGUOUS_DETECTION)
#	define FLUXCFG_FTL_FRAGILE_CONTIGUOUS_DETECTION 1
#endif

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_FTL_DEFAULTS_HPP_86B69953_2B60_449D_B593_6BC9836BF510
