/*-******************************************************* -- HEADER -{{{1- */
/*-	Include PEGTL with appropriate settings
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_MISC_PEGTL_HPP_352D298F_D1D5_47ED_8242_FB3B62A24DD3
#define FLUX_EXT_MISC_PEGTL_HPP_352D298F_D1D5_47ED_8242_FB3B62A24DD3

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 
#include <flux/pp/join.hpp>

#include "defaults.hpp"

//--    <tao/pegtl.hpp>             ///{{{1///////////////////////////////////

#if FLUXCFG_EXT_WITH_PEGTL
	/* Name of the flux-specific PEGTL namespace. See information regarding 
	 * embedding PEGTL in other libraries in the PEGTL 2.x documentation in the
	 * PEGTL Github (https://github.com/taocpp/PEGTL).
	 */
#	define FLUX_EXT_PEGTL_NAMESPACE FLUX_PP_JOIN(flux_pegtl_,FLUX_VERSION)

	/* Include PEGTL with the custom namespace name.
	 */
#	define TAO_PEGTL_NAMESPACE FLUX_EXT_PEGTL_NAMESPACE
#	include <tao/pegtl.hpp>

	/* Also place PEGTL in the namespace `flux_ext::pegtl`.
	 */
	namespace flux_ext
	{
		namespace pegtl = tao::FLUX_EXT_PEGTL_NAMESPACE;
	}
#endif // ~ EXT_WITH_PEGTL

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_MISC_PEGTL_HPP_352D298F_D1D5_47ED_8242_FB3B62A24DD3
