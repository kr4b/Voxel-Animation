/*-******************************************************* -- HEADER -{{{1- */
/*-	Include variant-lite with appropriate settings
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_VARIANT_HPP_B7ED7931_488A_45C0_842D_FF088E742600
#define FLUX_EXT_VARIANT_HPP_B7ED7931_488A_45C0_842D_FF088E742600

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 
#include <flux/setup/stdfeat.hpp> 
#include <flux/setup/langfeat.hpp> 

//--    <variant.hpp>                  ///{{{1///////////////////////////////////

// setup
#define variant_HAVE_STD_VARIANT FLUX_STDFEAT_CXX17_VARIANT
#define variant_HAVE_CONSTEXPR_14 FLUX_LANGFEAT_EXTENDED_CONSTEXPR

//TODO: fixme!
#define nonstd_lite_HAVE_IN_PLACE_TYPES 0

#include "impl/variant.hpp"

#undef variant_HAVE_STD_VARIANT
#undef variant_HAVE_CONSTEXPR_14
#undef nonstd_lite_HAVE_IN_PLACE_TYPES

/* Also place span and related stuff in the namespace `flux_ext::nonstd`.
 */
namespace flux_ext
{
	namespace nonstd = ::nonstd;
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_VARIANT_HPP_B7ED7931_488A_45C0_842D_FF088E742600
