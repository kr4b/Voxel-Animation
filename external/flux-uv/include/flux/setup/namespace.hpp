/*-******************************************************* -- HEADER -{{{1- */
/*-	Namespace macros
 *
 * Macro-ify namespace management, for aesthetic reasons.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SETUP_NAMESPACE_HPP_1319D1CC_4BD7_46B5_8C5A_497F4F22F8CC
#define FLUX_SETUP_NAMESPACE_HPP_1319D1CC_4BD7_46B5_8C5A_497F4F22F8CC

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include "defaults.hpp"
#include "version.hpp"

//--    namespace names             ///{{{1///////////////////////////////////


/** Main namespace name.
 *
 * \warning Redefining `FLUX_NAMESPACE` to something other than `flux` is 
 * likely going to end in tears.
 */
#define FLUX_NAMESPACE flux

/** Current version's sub-namespace
 */
#if defined(FLUXCFG_NAMESPACE_CURRENT)
#	define FLUX_NAMESPACE_CURRENT FLUXCFG_NAMESPACE_CURRENT
#else // !defined(CFG_NAMESPACE_CURRENT)
#	define FLUX_NAMESPACE_CURRENT FLUX_VERSION_NAME
#endif // ~ CFG_NAMESPACE_CURRENT


/** Helper: anonymous namespaces
 *
 * Example: 
 *
   \code
   FLUX_NAMESPACE_ENTER1( FLUX_ANONYMOUS_NAMESPACE )
   ...
   FLUX_NAMESPACE_LEAVE1( FLUX_ANONYMOUS_NAMESPACE )
   \endcode
 */
#define FLUX_ANONYMOUS_NAMESPACE /*intentionally empty*/


//--    helpers                     ///{{{1///////////////////////////////////

#	define FLUX_NAMESPACE_LEAVE() } }
#	define FLUX_NAMESPACE_ENTER() namespace FLUX_NAMESPACE { inline namespace FLUX_NAMESPACE_CURRENT {

/* For now, avoid pulling in "langfeat.hpp" here.
#if FLUX_LANGFEAT_NESTED_NAMESPACES
#	define FLUX_NAMESPACE_LEAVE1(a) } FLUX_NAMESPACE_LEAVE()
#	define FLUX_NAMESPACE_ENTER1(a) FLUX_NAMESPACE_ENTER() namespace a {

#	define FLUX_NAMESPACE_LEAVE2(a,b) } FLUX_NAMESPACE_LEAVE()
#	define FLUX_NAMESPACE_ENTER2(a,b) FLUX_NAMESPACE_ENTER() namespace a::b {

#	define FLUX_NAMESPACE_LEAVE3(a,b,c) } FLUX_NAMESPACE_LEAVE()
#	define FLUX_NAMESPACE_ENTER3(a,b,c) FLUX_NAMESPACE_ENTER() namespace a::b::c {

#else // !LANGFEAT_NESTED_NAMESPACES
*/
#	define FLUX_NAMESPACE_LEAVE1(a) } FLUX_NAMESPACE_LEAVE()
#	define FLUX_NAMESPACE_ENTER1(a) FLUX_NAMESPACE_ENTER() namespace a {

#	define FLUX_NAMESPACE_LEAVE2(a,b) } } FLUX_NAMESPACE_LEAVE()
#	define FLUX_NAMESPACE_ENTER2(a,b) FLUX_NAMESPACE_ENTER() namespace a { namespace b {

#	define FLUX_NAMESPACE_LEAVE3(a,b,c) } } } FLUX_NAMESPACE_LEAVE()
#	define FLUX_NAMESPACE_ENTER3(a,b,c) FLUX_NAMESPACE_ENTER() namespace a { namespace b { namespace c {
/*
#endif // ~ LANGFEAT_NESTED_NAMESPACES
*/

//--    namespaces                  ///{{{1///////////////////////////////////

/** \namespace flux
 * \brief Main namespace
 *
 * Root namespace of flux.
 */
namespace FLUX_NAMESPACE
{
	inline namespace FLUX_NAMESPACE_CURRENT
	{}
};

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SETUP_NAMESPACE_HPP_1319D1CC_4BD7_46B5_8C5A_497F4F22F8CC
