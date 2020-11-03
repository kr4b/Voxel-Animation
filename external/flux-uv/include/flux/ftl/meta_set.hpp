/*-******************************************************* -- HEADER -{{{1- */
/*-	Compile-time set and meta-functions to manipulate such
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_META_SET_HPP_D7FDDA76_3B6F_427B_80D2_138178C83878
#define FLUX_FTL_META_SET_HPP_D7FDDA76_3B6F_427B_80D2_138178C83878

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

//--    >>> namespace = flux::ftl::meta >>>         ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,meta)
//--    Set<>                               ///{{{2///////////////////////////

/** Compile-time set of types
 *
 * Compile time set of the types \a tItems.
 *
 * \note While not currently enforced, `Set<>` should be a proper set, and as
 * such it shouldn't contain duplicate entries.
 */
template< typename... tItems >
struct Set
{};

//--    meta functions                      ///{{{2///////////////////////////

/** Utility: create compile-time `Set<>` of types
 *
 * Create a `Set<>` containing the types `tItems`.
 */
template< typename... tItems >
using make_set = Set<tItems...>;


/** Check if `Set<>` contains a single element
 *
 * Check if the `Set<>` \a class contains the element (type) \a typename. If
 * this is the case, the nested boolean `::%value` will be set to true.
 * Otherwise, it will be false.
 */
template< class, typename > struct set_contains_element;

/** Check if two sets are equal
 *
 * Checks if the `Set<>`s \a class and \a class contain the same elements. If
 * this is the case, the nested boolean `::%value` will be set to true.
 * Otherwise, it will be false.
 */
template< class, class > struct set_equal;
/** Checks for subsets
 *
 * Checks if the left-hand `Set<>` \a class contains all elements of the
 * right-hand `Set<> \a class, that is, if the right-hand `Set<>` is a subset
 * of the left-hand one.
 */
template< class, class > struct set_contains_set;

//TODO
//template< class tX, class tY >
//struct set_union;
//template< class tX, class tY >
//struct set_difference;

//--    <<< ~ flux::ftl::meta namespace <<<         ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,meta)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/meta_set.inl"
#endif // FLUX_FTL_META_SET_HPP_D7FDDA76_3B6F_427B_80D2_138178C83878
