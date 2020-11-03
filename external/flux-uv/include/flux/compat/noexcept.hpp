/*-******************************************************* -- HEADER -{{{1- */
/*-	Helper for "typeful" C++17 noexcept
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_NOEXCEPT_HPP_9F05FD17_C533_4DDF_94CD_6140BA69134F
#define FLUX_COMPAT_NOEXCEPT_HPP_9F05FD17_C533_4DDF_94CD_6140BA69134F

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/langfeat.hpp>

//--    noexcept                    ///{{{1///////////////////////////////////

/** `noexcept` for typedefs.
 *
 * `FLUX_TYPEDEF_NOEXCEPT` expands to `noexcept` if the compiler supports the
 * C++17 features that integrates exception specifications into the type
 * system. If the feature is unsupported, `FLUX_TYPEDEF_NOEXCEPT` expands to
 * nothing.
 *
 * Example:
   \code
   using Pf = void (*)() FLUX_TYPEDEF_NOEXCEPT;
   typedef void (*Pg)() FLUX_TYPEDEF_NOEXCEPT;
   \endcode
 *
 * \note GCC seems to always accept `noexcept` with the `using`-form (but
 * errors on the `typedef` version), whereas MSVC errors on both if the feature
 * is unsupported.
 */
#if FLUX_LANGFEAT_TYPEDEF_NOEXCEPT
#	define FLUX_TYPEDEF_NOEXCEPT noexcept
#else // !LANGFEAT_TYPEDEF_NOEXCEPT
#	define FLUX_TYPEDEF_NOEXCEPT /*empty*/
#endif // ~ LANGFEAT_TYPEDEF_NOEXCEPT

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_NOEXCEPT_HPP_9F05FD17_C533_4DDF_94CD_6140BA69134F
