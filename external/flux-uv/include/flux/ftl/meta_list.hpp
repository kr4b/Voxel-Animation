/*-******************************************************* -- HEADER -{{{1- */
/*-	Compile-time list and meta-functions to manipulate such
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_META_LIST_HPP_BFB726A1_9244_4953_9CCF_BAB687AF305C
#define FLUX_FTL_META_LIST_HPP_BFB726A1_9244_4953_9CCF_BAB687AF305C

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/langfeat.hpp>

#include <tuple>
#include <cstddef>

//--    >>> namespace = flux::ftl::meta >>>         ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,meta)
//--    List                                ///{{{2///////////////////////////

/** Compile-time list of types
 *
 * Compile-time list of the types `tElems`.
 */
template< typename...  tElems >
struct List
{};

//--    meta functions                      ///{{{2///////////////////////////

/** Utility: create `List<>` of types
 *
 * Create a compile-time `List<>` of the types `tElems`.
 */
template< typename... tElems >
using make_list_t = List<tElems...>;


/** Size of compile-time `List<>`
 *
 * Compute size of compile-time `List<>` \a class. The constexpr `std::size_t`
 * `::%value` member will be set to the number of elements in \a tList.
 */
template< class tList > struct list_size;

#if FLUX_LANGFEAT_VARIABLE_TEMPLATE && FLUX_LANGFEAT_INLINE_VARIABLE
/** \copydoc list_size */
template< class tList > inline constexpr
std::size_t list_size_v = list_size<tList>::value;
#endif // ~ VARIABLE_TEMPLATE && INLINE_VARIABLE

/** Compare two `List<>`s for equality
 *
 * Compare \a tListA and \a tListB for equality. The `List<>`s are considered
 * equal if they contain the same elements in the same order.
 */
template< class tListA, class tListB > struct list_equal;
#if FLUX_LANGFEAT_VARIABLE_TEMPLATE && FLUX_LANGFEAT_INLINE_VARIABLE
/** \copydoc list_equal */
template< class tListA, class tListB > inline constexpr
bool list_equal_v = list_equal<tListA,tListB>::value;
#endif // ~ VARIABLE_TEMPLATE && INLINE_VARIABLE

/** Retrieve element at given index
 *
 * Retrives the element at the given in dex in the compile-time `List<>` \a
 * tList. The element is exposed via the `::%type` member.
 */
template< class, std::size_t > struct list_at;
/** \copydoc list_at */
template< class tList, std::size_t tIndex >
using list_at_t = typename list_at<tList,tIndex>::type;

/** Retrieve first element of compile-time `List<>`
 *
 * Retrieve the first element of the compile-time `List<>` \a tList. The
 * element is exposed via the `::%type` member.
 */
template< class > struct list_head;
/** \copydoc list_head */
template< class tList >
using list_head_t = typename list_head<tList>::type;
/** Construct `List<>` containing all but the first element
 *
 * Retrieve all but the first element of the compile-time `List<>` \a tList as
 * a new `List<>` in the `::%type` member.
 */
template< class > struct list_tail;
/** \copydoc list_tail */
template< class tList >
using list_tail_t = typename list_tail<tList>::type;

/** Take N elements from the front of a `List<>`
 *
 * Take the first \a tCount elements from the `List<>` \a tList. Return the
 * resulting `List<>` in the `::%type` member.
 */
template< class, std::size_t > struct list_take;
/** \copydoc list_take */
template< class tList, std::size_t tCount >
using list_take_t = typename list_take<tList,tCount>::type;
/** Drop N elements from the front of a `List<>`
 *
 * Drop the first \a tCount elements from the `List<>` \a tList. Return the
 * resulting `List<>` in the `::%type` member.
 */
template< class, std::size_t > struct list_drop;
/** \copydoc list_drop */
template< class tList, std::size_t tCount >
using list_drop_t = typename list_drop<tList,tCount>::type;

/** Does compile-time `List<>` contain specified element?
 *
 * Check if the compile-time `List<>` \a tList contains the element \a
 * tElement. If this is the case, the constexpr boolean `::%value` will be set
 * to true. Otherwise it will be false.
 *
 * Optionally, the `tBegin` parameter may be used to skip a specific number of
 * elements:
   \code
   constexpr bool a = list_contains< List<A,B,A>, A >::value;
   constexpr bool b = list_contains< List<A,B,A,1>, A >::value;
   constexpr bool c = list_contains< List<A,B,A,3>, A >::value;
   \endcode
 * produces `a = true`, `b = true`, and `c = false`.
 *
 * \see `list_index_of<>`
 */
template< class, typename, std::size_t = 0 > struct list_contains;
#if FLUX_LANGFEAT_VARIABLE_TEMPLATE && FLUX_LANGFEAT_INLINE_VARIABLE
/** \copydoc list_contains */
template< class tList, typename tTarget, std::size_t tBegin = 0 > inline constexpr
bool list_contains_v = list_contains<tList,tTarget,tBegin>::value;
#endif // ~ VARIABLE_TEMPLATE && INLINE_VARIABLE

/** Compute index of element in compile-time `List<>`
 *
 * Find the index of the element \a tTarget in the compile-time `List<>` \a
 * tList. If the element is found, the nested constexpr `std::size_t`
 * `::%value` is set to that element's linear index. If the element is not
 * present, no nested `::%value` is defined (usually leading to a compile-time
 * error).
 *
 * Optionally, \a tBegin may be used to skip over a number of elements:
   \code
   constexpr std::size_t x = list_index_of< List<A,B,A>, A >::value;
   constexpr std::size_t y = list_index_of< List<A,B,A>, A, 1 >::value;
   \code
 * produces `x = 0` and `y = 2`.
 */
template< class tList, typename tTarget, std::size_t tBegin = 0 > struct list_index_of;
#if FLUX_LANGFEAT_VARIABLE_TEMPLATE && FLUX_LANGFEAT_INLINE_VARIABLE
/** \copydoc list_index_of */
template< class tList, typename tTarget, std::size_t tBegin = 0 > inline constexpr
std::size_t list_index_of_v = list_index_of<tList,tTarget,tBegin>::value;
#endif // ~ VARIABLE_TEMPLATE && INLINE_VARIABLE

/** Flatten multiple `List<>`s
 *
 * Flatten the `List<>`s \a tLists into a single `List<>` by concatenating
 * these in the order specified:
   \code
   using ty = list_flatten_t< List<A,B>, List<C>, List<>, List<D,E> >;
   \endcode
 * thus produces `ty = List<A,B,C,D,E>`.
 */
template< class... > struct list_flatten;
/** \copydoc list_flatten */
template< class... tLists > 
using list_flatten_t = typename list_flatten<tLists...>::type;

/** Filter elements of compile-time `List<>`
 *
 * Filter the compile-time `List<>` \a tList by applying the `::%Eval` member
 * template of \a tFunct to each element. The `tFunct::template Eval` member
 * template must be instantiable as follows
   \code
   constexpr bool keep = tFunc::template Eval<ELEMENT,tOptional...>::value;
   \endcode
 * where `ELEMENT` is one of the list elements. If the above evaluates to true
 * (i.e., `keep` would end up true), the element is kept. It is discarded
 * otherwise.
 */
template< class, class, typename... > struct list_filter;
/** \copydoc list_filter */
template< class tList, class tFunct, typename... tOptional >
using list_filter_t = typename list_filter<tList,tFunct,tOptional...>::type;

/** Filter elements of compile-time `List<>`
 *
 * Filter the compile-time `List<>` \a tList by applying the wrapped
 * meta-function \a class to each element. The \a tEval is a template being
 * instantiable as follows:
   \code
   constexpr bool keep = tEval<ELEMENT,tOptional...>::value;
   \endcode
 * where `ELEMENT` is one of the list elements. If the above evaluates to true,
 * the element is kept. It is discarded otherwise.
 */
template< class, template<typename...> class, typename... > struct list_filter0;
/** \copydoc list_filter0 */
template< class tList, template<typename...> class tEval, typename... tOptional >
using list_filter0_t = typename list_filter0<tList,tEval,tOptional...>::type;

/** Transform elements of compile-time `List<>`
 *
 * Transform elements of compile-time `List<>` \a tList by evaluating the
 * `::Eval` member-template of \a tFunct on each element of the input list. The
 * code `using ty = list_transform_t< tList, tFunct, Thing >` behaves
 * conceptually as
   \code
   using ty = List<
   	tFunct::template Eval< element 0 of tList, Thing >::type,
   	tFunct::template Eval< element 1 of tList, Thing >::type,
   	...
   	tFunct::template Eval< element N of tList, Thing >::type
   >;
   \endcode
 * (the \a tOptional arguments are simply forwarded to `Eval`).
 *
 * \see list_transform0, list_transform0_t
 */
template< class, class, typename... > struct list_transform;
/** \copydoc list_transform */
template< class tList, class tFunct, typename... tOptional > 
using list_transform_t = typename list_transform<tList,tFunct,tOptional...>::type;

/** Transform elements of compile-time `List<>`
 *
 * Transform elements of compile-time `List<>` \a tList by instantiating \a
 * tEval on each element of the input list. The code `using ty =
 * list_transform0_t< tList, tEval, Thing >` behaves conceptually as
   \code
   using ty = List<
   	tEval< element 0 of tList, Thing >::type,
   	tEval< element 1 of tList, Thing >::type,
   	...
   	tEval< element N of tList, Thing >::type
   >;
   \endcode
 * (the \a tOptional arguments are simply forwarded to `tEval` after each 
 * element from the list).
 *
 * \see list_transform, list_transform_t
 */
template< class, template<typename...> class, typename... > struct list_transform0;
/** \copydoc list_transform0 */
template< class tList, template<typename...> class tEval, typename... tOptional > 
using list_transform0_t = typename list_transform0<tList,tEval,tOptional...>::type;


/** Call run-time function for each element of compile-time `List<>`
 *
 * Call a run-time function for each element of the compile-time `List<>` \a
 * class. The right-hand side \a class must define a `enact()` method accepting
 * a `List<tType>` as its first argument; this method is called for each
 * element in the passed-in `List<>`. Additional arguments are forwarded to the
 * `enact()` method, if such as specified.
 *
 * Example:
   \code
   struct Enactor
   {
   	template< typename tType >
   	static void enact( List<tType>, std::ostream& aOut ) {
   		aOut << typeid(tType).name() << " ";
   	}
   };
  
   ...
  
   using MyList = List< ... >;
   list_enact_for_each< MyList, Enactor >::enact( std::cout );
   \endcode
 */
template< class, class > struct list_enact_for_each; // no result



/** Convert compile-time `List<>` to a `std::tuple`
 *
 * Convert compile-time `List<>` to a `std::tuple` with the corresponding
 * types.
 */
template< class > struct list_to_tuple; // ::type = std::tuple<>
/** \copydoc list_to_tuple */
template< class tList >
using list_to_tuple_t = typename list_to_tuple<tList>::type;

//--    <<< ~ flux::ftl::meta namespace <<<         ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,meta)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/meta_list.inl"
#endif // FLUX_FTL_META_LIST_HPP_BFB726A1_9244_4953_9CCF_BAB687AF305C
