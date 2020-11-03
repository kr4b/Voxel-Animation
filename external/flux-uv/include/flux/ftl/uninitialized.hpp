/*-******************************************************* -- HEADER -{{{1- */
/*-	Helpers: uninitialized_* and uninitialized_*_n; destroy/destroy_n
 * 
 * This header defines several methods relating to uninitialized memory. 
 *
 * TODO: expose IsNothrowUnReloc<> et al as public API.
 * TODO-maybe: uninitialized_move_if_noexcept() could be NOEXCEPT if either 
 *             move is nothrow or copy is nothrow. (It's unlikely that copy
 *             is nothrow if move isn't, so this doesn't matter *that* much.)
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_UNINITIALIZED_HPP_94651553_C3D7_4CE6_9766_97CEC8D284B4
#define FLUX_FTL_UNINITIALIZED_HPP_94651553_C3D7_4CE6_9766_97CEC8D284B4

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/restrict.hpp>

#include <cstddef>
#include <utility>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    noexcept helpers                    ///{{{2///////////////////////////
namespace detail
{
	template< typename, typename > struct IsNothrowUnCtor;

	template< typename, typename > struct IsNothrowUnCopy;
	template< typename, typename > struct IsNothrowUnMove;
	template< typename, typename > struct IsNothrowUnReloc;

	template< typename > struct IsNothrowDtor;
	template< typename > struct IsNothrowValueCtor;
	template< typename > struct IsNothrowDefaultCtor;
}

//--    uninitialized_copy*()               ///{{{2///////////////////////////

/** Copy to uninitialized destination
 *
 * See <a href="http://en.cppreference.com/w/cpp/memory/uninitialized_copy">
 * `std::uninitialized_copy`</a>.
 *
 * `ftl::uninitialized_copy` optimizes the case where
 *   - both \a tInputIt and \a tForwardIt are contiguous iterators
 *   - refer to the same type, `ValueType`
 *   - and `ValueType` is bitwise duplicable.
 * In this case, `uninitialized_copy` will simply `std::memcpy()` elements from
 * \a tInputIt to \a tForwardIt.
 *
 * \note `ftl::uninitialized_copy_n()` returns a
 * `std::pair<tInputIt,tForwardIt>` rather than just `tForwardIt` like the
 * underlying `std::uninitialized_copy_n()`. The adjusted behaviour is more in
 * line with `std::uninitialized_move_n()`.
 *
 * Overloads taking an allocator \a tAlloc will copy-construct elements using
 * the allocator's `construct()` method when appropriate. (This ensures that
 * child objects will use the allocator as well.)
 */
template< typename tInputIt, typename tForwardIt >
tForwardIt uninitialized_copy( tInputIt, tInputIt, tForwardIt );
/** \copydoc uninitialized_copy() */
template< typename tInputIt, typename tForwardIt >
std::pair<tInputIt,tForwardIt> uninitialized_copy_n( tInputIt, std::size_t, tForwardIt );
/** \copydoc uninitialized_copy() */
template< class tAlloc, typename tInputIt, typename tForwardIt >
tForwardIt uninitialized_copy( tAlloc& FLUX_RESTRICT, tInputIt, tInputIt, tForwardIt ) noexcept((detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_copy() */
template< class tAlloc, typename tInputIt, typename tForwardIt >
std::pair<tInputIt,tForwardIt> uninitialized_copy_n( tAlloc& FLUX_RESTRICT, tInputIt, std::size_t, tForwardIt );


/** Copy to uninitialized destination
 *
 * Copy to uninitialized destination buffer, similar to `uninitialized_copy()`,
 * except that the passed-in iterators are updated as the function progresses.
 *
 * If an exception occurs, \a tForwardIt& will point to the first element that
 * was not successfully initialized:
   \code
   input a = ..., b = ...;
   output c = ...;
  
   input at = a;
   output ct = c;
   uninitialized_copy_p( at, b, ct );
   \endcode
 * If no exception occurs, `at == b` and `ct` equals the iterator that would
 * have been returned by `uninitialized_copy()`. If an exception occurs, the
 * range \f$\left[\mbox{c} ... \mbox{ct}\right)\f$ contains elements that were
 * successfully initialized from \f$\left[\mbox{a} ... \mbox{at}\right)\f$.
 * (`uninitialized_copy()` would automatically destroy the elements in the
 * range \f$\left[\mbox{c} ... \mbox{ct}\right)\f$ as part of the cleanup;
 * here, this is left to caller).
 *
 * Overloads taking an allocator \a tAlloc will copy-construct elements using
 * the allocator's `construct()` method when appropriate. (This ensures that
 * child objects will use the allocator as well.)
 */
template< typename tInputIt, typename tForwardIt >
void uninitialized_copy_p( tInputIt& FLUX_RESTRICT, tInputIt, tForwardIt& FLUX_RESTRICT ) 
	noexcept((detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_copy_p() */
template< typename tInputIt, typename tForwardIt >
void uninitialized_copy_pn( tInputIt& FLUX_RESTRICT, std::size_t, tForwardIt& FLUX_RESTRICT )
	noexcept((detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_copy_p() */
template< class tAlloc, typename tInputIt, typename tForwardIt >
void uninitialized_copy_p( tAlloc& FLUX_RESTRICT, tInputIt& FLUX_RESTRICT, tInputIt, tForwardIt& FLUX_RESTRICT ) 
	noexcept((detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_copy_p() */
template< class tAlloc, typename tInputIt, typename tForwardIt >
void uninitialized_copy_pn( tAlloc& FLUX_RESTRICT, tInputIt& FLUX_RESTRICT, std::size_t, tForwardIt& FLUX_RESTRICT )
	noexcept((detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value));

//--    uninitialized_move*()               ///{{{2///////////////////////////
/** Move to uninitialized destination
 *
 * See <a href="http://en.cppreference.com/w/cpp/memory/uninitialized_move">
 * `std::uninitialized_move`</a>.
 *
 * `ftl::uninitialized_move` optimizes the case where
 *   - both \a tInputIt and \a tForwardIt are contiguous iterators
 *   - refer to the same type, `ValueType`
 *   - and `ValueType` is bitwise relocatable and dismissible.
 * In this case, `uninitialized_move` will simply `std::memcpy()` elements from
 * \a tInputIt to \a tForwardIt.
 *
 * \note The additional requirement of being `bitwise_dismissible` stems from
 * the fact that `uninitialized_move()` leaves the input in a moved-from state,
 * meaning that it will be destroyed properly later. See
 * `uninitialized_relocate()` for a function without this requirement, which
 * also leaves the input in an uninitialized state.
 *
 * Overloads taking an allocator \a tAlloc will move-construct elements using
 * the allocator's `construct()` method with a rvalue reference to the source
 * when appropriate. 
 */
template< typename tInputIt, typename tForwardIt >
tForwardIt uninitialized_move( tInputIt, tInputIt, tForwardIt );
/** \copydoc uninitialized_move() */
template< typename tInputIt, typename tForwardIt >
std::pair<tInputIt,tForwardIt> uninitialized_move_n( tInputIt, std::size_t, tForwardIt );
/** \copydoc uninitialized_move() */
template< class tAlloc, typename tInputIt, typename tForwardIt >
tForwardIt uninitialized_move( tAlloc& FLUX_RESTRICT, tInputIt, tInputIt, tForwardIt ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_move() */
template< class tAlloc, typename tInputIt, typename tForwardIt >
std::pair<tInputIt,tForwardIt> uninitialized_move_n( tAlloc& FLUX_RESTRICT, tInputIt, std::size_t, tForwardIt );

/** Move to uninitialized destination
 *
 * Move to uninitialized destination buffer, similar to `uninitialized_move()`,
 * except that the passed-in iterators are updated as the function progresses.
 *
 * If an exception occurs, \a tForwardIt& will point to the first element that
 * was not successfully initialized:
   \code
   input a = ..., b = ...;
   output c = ...;
  
   input at = a;
   output ct = c;
   uninitialized_move_p( at, b, ct );
   \endcode
 * If no exception occurs, `at == b` and `ct` equals the iterator that would
 * have been returned by `uninitialized_move()`. If an exception occurs, the
 * range \f$\left[\mbox{c} ... \mbox{ct}\right)\f$ contains elements that were
 * successfully initialized from \f$\left[\mbox{a} ... \mbox{at}\right)\f$.
 * (`uninitialized_move()` would automatically destroy the elements in the
 * range \f$\left[\mbox{c} ... \mbox{ct}\right)\f$ as part of the cleanup;
 * here, this is left to caller). The input elements in the range \f$\left[
 * \mbox{at} ... \mbox{b}\right)\f$ have not been moved from (albeit care 
 * should potentially be taken with the element at `at`). 
 *
 * Overloads taking an allocator \a tAlloc will move-construct elements using
 * the allocator's `construct()` method with a rvalue reference to the source
 * when appropriate. 
 */
template< typename tInputIt, typename tForwardIt >
void uninitialized_move_p( tInputIt& FLUX_RESTRICT, tInputIt, tForwardIt& FLUX_RESTRICT ) 
	noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_move_p() */
template< typename tInputIt, typename tForwardIt >
void uninitialized_move_pn( tInputIt& FLUX_RESTRICT, std::size_t, tForwardIt& FLUX_RESTRICT )
	noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_move_p() */
template< class tAlloc, typename tInputIt, typename tForwardIt >
void uninitialized_move_p( tAlloc& FLUX_RESTRICT, tInputIt& FLUX_RESTRICT, tInputIt, tForwardIt& FLUX_RESTRICT ) 
	noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_move_p() */
template< class tAlloc, typename tInputIt, typename tForwardIt >
void uninitialized_move_pn( tAlloc& FLUX_RESTRICT, tInputIt& FLUX_RESTRICT, std::size_t, tForwardIt& FLUX_RESTRICT )
	noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value));

//--    uninitialized_move_if_noexcept()    ///{{{2///////////////////////////
/** Move/copy to uninitialized memory
 *
 * Either move or copy to uninitialized memory, depending on whether moving
 * can throw. This is useful when providing certain exception guarantees,
 * see <a href="http://en.cppreference.com/w/cpp/utility/move_if_noexcept">
 * `std::move_if_noexcept`</a>.
 *
 * `uninitialized_move_if_noexcept_p()` will call either of 
 *   - `ftl::uninitialized_copy_p()` 
 *   - `ftl::uninitialized_move_p()`
 * depending on whether the target type is noexcept move-constructible from
 * the source type.
 */
template< typename tInputIt, typename tForwardIt >
tForwardIt uninitialized_move_if_noexcept( tInputIt, tInputIt, tForwardIt ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_move_if_noexcept() */
template< typename tInputIt, typename tForwardIt >
std::pair<tInputIt,tForwardIt> uninitialized_move_if_noexcept_n( tInputIt, std::size_t, tForwardIt );
/** \copydoc uninitialized_move_if_noexcept() */
template< class tAlloc, typename tInputIt, typename tForwardIt >
tForwardIt uninitialized_move_if_noexcept( tAlloc& FLUX_RESTRICT, tInputIt, tInputIt, tForwardIt ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_move_if_noexcept() */
template< class tAlloc, typename tInputIt, typename tForwardIt >
std::pair<tInputIt,tForwardIt> uninitialized_move_if_noexcept_n( tAlloc& FLUX_RESTRICT, tInputIt, std::size_t, tForwardIt );

/** Move/copy to uninitialized memory
 *
 * Either move or copy to uninitialized memory while updating passed-in 
 * iterators as the function progresses. Moves/copies, depending on whether 
 * moving can throw. This is useful when providing exception guarantees,
 * see <a href="http://en.cppreference.com/w/cpp/utility/move_if_noexcept">
 * `std::move_if_noexcept`</a>.
 *
 * `uninitialized_move_if_noexcept()` will call either of 
 *   - `ftl::uninitialized_copy()` 
 *   - `ftl::uninitialized_move()`
 * depending on whether the target type is noexcept move-constructible from
 * the source type.
 *
 * If an exception occurs, \a tForwardIt& will point to the first element that
 * was not successfully initialized:
   \code
   input a = ..., b = ...;
   output c = ...;
  
   input at = a;
   output ct = c;
   uninitialized_move_if_noexcept_p( at, b, ct );
   \endcode
 * If no exception occurs, `at == b` and `ct` equals the iterator that would
 * have been returned by `uninitialized_move_if_noexcept()`. If an exception
 * occurs, the range \f$\left[\mbox{c} ... \mbox{ct}\right)\f$ contains
 * elements that were successfully initialized from \f$\left[\mbox{a} ...
 * \mbox{at}\right)\f$. (`uninitialized_move_if_noexcept()` would automatically
 * destroy the elements in the range \f$\left[\mbox{c} ... \mbox{ct}\right)\f$ 
 * as part of the cleanup; here, this is left to caller). Since the method will
 * make copies if exceptions are possible, the input range is left as-is in
 * the event of an exception. 
 *
 * Overloads taking an allocator \a tAlloc will move/copy-construct elements 
 * using the allocator's `construct()` method. 
 */
template< typename tInputIt, typename tForwardIt >
void uninitialized_move_if_noexcept_p( tInputIt& FLUX_RESTRICT, tInputIt, tForwardIt& FLUX_RESTRICT )  noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_move_if_noexcept_p() */
template< typename tInputIt, typename tForwardIt >
void uninitialized_move_if_noexcept_pn( tInputIt& FLUX_RESTRICT, std::size_t, tForwardIt& FLUX_RESTRICT ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_move_if_noexcept_p() */
template< class tAlloc, typename tInputIt, typename tForwardIt >
void uninitialized_move_if_noexcept_p( tAlloc& FLUX_RESTRICT, tInputIt& FLUX_RESTRICT, tInputIt, tForwardIt& FLUX_RESTRICT )  noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_move_if_noexcept_p() */
template< class tAlloc, typename tInputIt, typename tForwardIt >
void uninitialized_move_if_noexcept_pn( tAlloc& FLUX_RESTRICT, tInputIt& FLUX_RESTRICT, std::size_t, tForwardIt& FLUX_RESTRICT ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value));

//--    uninitialized_relocate()            ///{{{2///////////////////////////

/** Relocate to uninitialized destination
 *
 * Similar to `uninitialized_move()`, `uninitialized_relocate()` moves objects
 * from an input buffer to an uninitialized destination. In contrast to the
 * other method, when `uninitialized_relocate()` returns, the input is in an
 * uninitialized state, as if
   \code
   void uninitialized_relocate( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut )
   {
   		uninitialized_move( aBeg, aEnd, aOut );
   		destroy( aBeg, aEnd )
   }
   \endcode
 *
 * The function behaves like the above w.r.t. exceptions as well, i.e., should
 * an exception be thrown during the `uninitialized_move()` some objects in the
 * range \f$\left[\mbox{aBeg} ... \mbox{aEnd}\right)\f$ are left in a valid but
 * unspecified state. The output is returned to an uninitialized state.
 *
 * `ftl::uninitialized_relocate()` optimizes the case where
 *   - both \a tFwdIn and \a tFwdOut are contiguous iterators
 *   - refer to the same type, `ValueType`
 *   - and `ValueType` is bitwise relocatable
 * In this case, `uninitialized_relocate()` will simply `std::memcpy()`
 * elements from \a tFwdIn to \a tFwdOut.
 *
 * \note Unlike `uninitialized_move()` et al., `uninitialized_relocate()`
 * requires both the input and the output iterators to be forward iterators.
 *
 * Additional overloads accept an allocator as their first argument. These will
 * call the `construct` method of the allocator with a rvalue reference to the
 * source object, when applicable.
 */
template< typename tFwdIn, typename tFwdOut >
tFwdOut uninitialized_relocate( tFwdIn, tFwdIn, tFwdOut ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value));
/** \copydoc uninitialized_relocate() */
template< typename tFwdIn, typename tFwdOut >
std::pair<tFwdIn,tFwdOut> uninitialized_relocate_n( tFwdIn, std::size_t, tFwdOut );
/** \copydoc uninitialized_relocate() */
template< class tAlloc, typename tFwdIn, typename tFwdOut >
tFwdOut uninitialized_relocate( tAlloc& FLUX_RESTRICT, tFwdIn, tFwdIn, tFwdOut ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value));
/** \copydoc uninitialized_relocate() */
template< class tAlloc, typename tFwdIn, typename tFwdOut >
std::pair<tFwdIn,tFwdOut> uninitialized_relocate_n( tAlloc& FLUX_RESTRICT, tFwdIn, std::size_t, tFwdOut );

/** Relocate to uninitialized destination
 *
 * Relocate to uninitialized destination buffer, while passed-in iterators are
 * updated as the function progresses.
 *
 * If an exception occurs, \a tForwardIt& will point to the first element that
 * was not successfully initialized:
   \code
   input a = ..., b = ...;
   output c = ...;
  
   input at = a;
   output ct = c;
   uninitialized_relocate_p( at, b, ct );
   \endcode
 * If no exception occurs, `at == b` and `ct` equals the iterator that would
 * have been returned by `uninitialized_relocate()`. If an exception occurs, the
 * range \f$\left[\mbox{c} ... \mbox{ct}\right)\f$ contains elements that were
 * successfully initialized from \f$\left[\mbox{a} ... \mbox{at}\right)\f$. 
 *
 * <b>Important:</b> the range \f$\left[\mbox{a} ... \mbox{at}\right)\f$ always
 * <b>contains uninitialized memory after the function call</b>, regardless of
 * whether the function throws or not. Likewise, the range \f$\left[\mbox{c} ...
 * \mbox{ct}\right)\f$ always contains initialized objects.
 *
 * Additional overloads accept an allocator as their first argument. These will
 * call the `construct` method of the allocator with a rvalue reference to the
 * source object, when applicable.
 */
template< typename tInputIt, typename tForwardIt >
void uninitialized_relocate_p( tInputIt& FLUX_RESTRICT, tInputIt, tForwardIt& FLUX_RESTRICT ) 
	noexcept((detail::IsNothrowUnReloc<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_relocate_p() */
template< typename tInputIt, typename tForwardIt >
void uninitialized_relocate_pn( tInputIt& FLUX_RESTRICT, std::size_t, tForwardIt& FLUX_RESTRICT )
	noexcept((detail::IsNothrowUnReloc<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_relocate_p() */
template< class tAlloc, typename tInputIt, typename tForwardIt >
void uninitialized_relocate_p( tAlloc& FLUX_RESTRICT, tInputIt& FLUX_RESTRICT, tInputIt, tForwardIt& FLUX_RESTRICT ) 
	noexcept((detail::IsNothrowUnReloc<tInputIt,tForwardIt>::value));
/** \copydoc uninitialized_relocate_p() */
template< class tAlloc, typename tInputIt, typename tForwardIt >
void uninitialized_relocate_pn( tAlloc& FLUX_RESTRICT, tInputIt& FLUX_RESTRICT, std::size_t, tForwardIt& FLUX_RESTRICT )
	noexcept((detail::IsNothrowUnReloc<tInputIt,tForwardIt>::value));

//--    uninitialized_relocate_if_noexcept()    ///{{{2///////////////////////
/** Relocate to uninitialized memory
 *
 * Similar to `uninitialized_relocate()`, this function relocates objects from
 * a source to uninitialized memory. `uninitialized_relocate_if_noexcept()`
 * does so with `uninitialized_move_if_noexcept()`, i.e, with move operations
 * if they are `noexcept`, and otherwise by copying. The source objects will be
 * destroyed if the moving phase is successful. If an exception occurs, the
 * source objects will be left inact. `uninitialized_relocate_if_noexcept()`
 * thusly works as if:
   \code
   void uninitialized_relocate( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut )
   {
   		uninitialized_move_if_noexcept( aBeg, aEnd, aOut );
   		destroy( aBeg, aEnd )
   }
   \endcode
 * 
 * The method specially optimizes the same cases as `uninitialized_relocate()`.
 * 
 * \note Like the `uninitialized_relocate()` group of methods,
 * `uninitialized_relocate_if_noexcept()` requires both the input and the
 * output iterators to be forward iterators.
 */
template< typename tFwdIn, typename tFwdOut >
tFwdOut uninitialized_relocate_if_noexcept( tFwdIn, tFwdIn, tFwdOut ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value));
/** \copydoc uninitialized_relocate_if_noexcept() */
template< typename tFwdIn, typename tFwdOut >
std::pair<tFwdIn,tFwdOut> uninitialized_relocate_if_noexcept_n( tFwdIn, std::size_t, tFwdOut );
/** \copydoc uninitialized_relocate_if_noexcept() */
template< class tAlloc, typename tFwdIn, typename tFwdOut >
tFwdOut uninitialized_relocate_if_noexcept( tAlloc& FLUX_RESTRICT, tFwdIn, tFwdIn, tFwdOut ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value));
/** \copydoc uninitialized_relocate_if_noexcept() */
template< class tAlloc, typename tFwdIn, typename tFwdOut >
std::pair<tFwdIn,tFwdOut> uninitialized_relocate_if_noexcept_n( tAlloc& FLUX_RESTRICT, tFwdIn, std::size_t, tFwdOut );

/** Relocate to uninitialized memory
 *
 * Relocate to uninitialized destination buffer, while passed-in iterators are
 * updated as the function progresses.
 *
 * If an exception occurs, \a tFwdOut& will point to the first element that was
 * not successfully initialized:
   \code
   input a = ..., b = ...;
   output c = ...;
  
   input at = a;
   output ct = c;
   uninitialized_relocate_if_noexcept_p( at, b, ct );
   \endcode
 * If no exception occurs, `at == b` and `ct` equals the iterator that would
 * have been returned by `uninitialized_relocate_if_noexcept()`. If an 
 * exception occurs, the range \f$\left[\mbox{c} ... \mbox{ct}\right)\f$ 
 * contains elements that were successfully initialized; the input range is
 * left intact, however (and `at` is left at its original value).
 *
 * <b>Important:</b> the range \f$\left[\mbox{a} ... \mbox{at}\right)\f$ always
 * <b>contains uninitialized memory after the function call</b>, regardless of
 * whether the function throws or not. Likewise, the range \f$\left[\mbox{c} ...
 * \mbox{ct}\right)\f$ always contains initialized objects.
 *
 * <i>Note:</i> if move-construction is `noexcept`,  `tFwdIn` can be an 
 * `InputIterator` rather than a `ForwardIterator` as is required otherwise.
 *
 * Additional overloads accept an allocator as their first argument. These will
 * call the `construct` method of the allocator with a rvalue reference to the
 * source object, when applicable.
 */
template< typename tFwdIn, typename tFwdOut >
void uninitialized_relocate_if_noexcept_p( tFwdIn& FLUX_RESTRICT, tFwdIn, tFwdOut& FLUX_RESTRICT )  noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value));
/** \copydoc uninitialized_relocate_if_noexcept_p() */
template< typename tFwdIn, typename tFwdOut >
void uninitialized_relocate_if_noexcept_pn( tFwdIn& FLUX_RESTRICT, std::size_t, tFwdOut& FLUX_RESTRICT ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value));
/** \copydoc uninitialized_relocate_if_noexcept_p() */
template< class tAlloc, typename tFwdIn, typename tFwdOut >
void uninitialized_relocate_if_noexcept_p( tAlloc& FLUX_RESTRICT, tFwdIn& FLUX_RESTRICT, tFwdIn, tFwdOut& FLUX_RESTRICT ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value));
/** \copydoc uninitialized_relocate_if_noexcept_p() */
template< class tAlloc, typename tFwdIn, typename tFwdOut >
void uninitialized_relocate_if_noexcept_pn( tAlloc& FLUX_RESTRICT, tFwdIn& FLUX_RESTRICT, std::size_t, tFwdOut& FLUX_RESTRICT ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value));

//--    uninitialized_value_construct()     ///{{{2///////////////////////////

/** Value construct objects in uninitialized memory
 *
 * See <a href="http://en.cppreference.com/w/cpp/memory/uninitialized_value_construct">`std::uninitialized_value_construct`</a>.
 *
 * `ftl::uninitialized_value_construct()` optimizes the case where
 *   - \a tForward is a contiguous iterator
 *   - `ValueType` is bitwise conjurable
 * In this case, `uninitialized_value_construct()` uses `std::memset()` to
 * initialize the memory to zero.
 */
template< typename tForward >
void uninitialized_value_construct( tForward, tForward );
/** \copydoc uninitialized_value_construct() */
template< typename tForward >
tForward uninitialized_value_construct_n( tForward, std::size_t );
/** \copydoc uninitialized_value_construct() */
template< class tAlloc, typename tForward >
void uninitialized_value_construct( tAlloc& FLUX_RESTRICT, tForward, tForward ) noexcept(detail::IsNothrowValueCtor<tForward>::value);
/** \copydoc uninitialized_value_construct() */
template< class tAlloc, typename tForward >
tForward uninitialized_value_construct_n( tAlloc& FLUX_RESTRICT, tForward, std::size_t ) noexcept(detail::IsNothrowValueCtor<tForward>::value);

/** Value construct objects in uninitialized memory
 *
 * Value construct objects in uninitialized memory, while passed-in iterators
 * are updated as the function progresses.
 *
 * If an exception occurs, \a tForwardIt& will point to the first element that
 * was not successfully initialized:
   \code
   ptr a = ...;
   ptr b = a + ...;

   ptr at = a;
   uninitialized_value_construct_p( at, b );
   \endcode
 * If no exception occurs, `at == b`. If an exception occurs, the range
 * \f$\left[\mbox{a} ... \mbox{at}\right)\f$ contains elements that were
 * successfully value constructed. 
 *
 * `ftl::uninitialized_value_construct_p()` optimizes the case where
 *   - \a tForward is a contiguous iterator
 *   - `ValueType` is bitwise conjurable
 * In this case, `uninitialized_value_construct()` uses `std::memset()` to
 * initialize the memory to zero.
 */
template< typename tForward >
void uninitialized_value_construct_p( tForward& FLUX_RESTRICT, tForward ) noexcept((detail::IsNothrowValueCtor<tForward>::value));
/** \copydoc uninitialized_value_construct_p() */
template< typename tForward >
void uninitialized_value_construct_pn( tForward& FLUX_RESTRICT, std::size_t ) noexcept((detail::IsNothrowValueCtor<tForward>::value));
/** \copydoc uninitialized_value_construct_p() */
template< class tAlloc, typename tForward >
void uninitialized_value_construct_p( tAlloc& FLUX_RESTRICT, tForward& FLUX_RESTRICT, tForward ) noexcept((detail::IsNothrowValueCtor<tForward>::value));
/** \copydoc uninitialized_value_construct_pn() */
template< class tAlloc, typename tForward >
void uninitialized_value_construct_pn( tAlloc& FLUX_RESTRICT, tForward& FLUX_RESTRICT, std::size_t ) noexcept(detail::IsNothrowValueCtor<tForward>::value);

//--    uninitialized_default_construct()   ///{{{2///////////////////////////

/** Default construct objects in uninitialized memory
 *
 * See <a href="http://en.cppreference.com/w/cpp/memory/uninitialized_default_construct">`std::uninitialized_default_construct`</a>.
 *
 * `ftl::uninitialized_default_construct` optimizes the case where the
 * underlying type `is_bitwise_awakeable<>`. In such a case,
 * `uninitialized_default_construct()` does nothing (other than computing the
 * returned iterator, where necessary).
 */
template< typename tForward >
void uninitialized_default_construct( tForward, tForward ) noexcept(detail::IsNothrowDefaultCtor<tForward>::value);
/** \copydoc uninitialized_default_construct() */
template< typename tForward >
tForward uninitialized_default_construct_n( tForward, std::size_t ) noexcept(detail::IsNothrowDefaultCtor<tForward>::value);
/** \copydoc uninitialized_default_construct() */
template< class tAlloc, typename tForward >
void uninitialized_default_construct( tAlloc& FLUX_RESTRICT, tForward, tForward ) noexcept(detail::IsNothrowDefaultCtor<tForward>::value);
/** \copydoc uninitialized_default_construct() */
template< class tAlloc, typename tForward >
tForward uninitialized_default_construct_n( tAlloc& FLUX_RESTRICT, tForward, std::size_t ) noexcept(detail::IsNothrowDefaultCtor<tForward>::value);

/** Default construct objects in uninitialized memory
 *
 * Default construct objects in uninitialized memory, while passed-in iterators
 * are updated as the function progresses.
 *
 * If an exception occurs, \a tForwardIt& will point to the first element that
 * was not successfully initialized:
   \code
   ptr a = ...;
   ptr b = a + ...;

   ptr at = a;
   uninitialized_default_construct_p( at, b );
   \endcode
 * If no exception occurs, `at == b`. If an exception occurs, the range
 * \f$\left[\mbox{a} ... \mbox{at}\right)\f$ contains elements that were
 * successfully value constructed. 
 *
 * `ftl::uninitialized_default_construct_p()` optimizes the case where the
 * underlying type `is_bitwise_awakeable<>`. In this case 
 * `uninitialized_default_construct_p()` does nothing other than update the
 * passed-in iterators to their expected output values.
 */
template< typename tForward >
void uninitialized_default_construct_p( tForward& FLUX_RESTRICT, tForward ) noexcept((detail::IsNothrowDefaultCtor<tForward>::value));
/** \copydoc uninitialized_default_construct_p() */
template< typename tForward >
void uninitialized_default_construct_pn( tForward& FLUX_RESTRICT, std::size_t ) noexcept((detail::IsNothrowDefaultCtor<tForward>::value));
/** \copydoc uninitialized_default_construct_p() */
template< class tAlloc, typename tForward >
void uninitialized_default_construct_p( tAlloc& FLUX_RESTRICT, tForward& FLUX_RESTRICT, tForward ) noexcept((detail::IsNothrowDefaultCtor<tForward>::value));
/** \copydoc uninitialized_default_construct_pn() */
template< class tAlloc, typename tForward >
void uninitialized_default_construct_pn( tAlloc& FLUX_RESTRICT, tForward& FLUX_RESTRICT, std::size_t ) noexcept(detail::IsNothrowDefaultCtor<tForward>::value);

//--    uninitialized_fill()                ///{{{2///////////////////////////

/** Construct objects based on template in uninitialized memory
 *
 * See <a href="http://en.cppreference.com/w/cpp/memory/uninitialized_value_construct">`std::uninitialized_fill`</a>.
 *
 * `ftl::uninitialized_fill()` doesn't provide any special optimizations over 
 * the`std::` versions; however, additional overloads optionally taking an
 * allocator as their first argument exist.
 */
template< typename tForward, typename tType >
void uninitialized_fill( tForward, tForward, tType const& FLUX_RESTRICT );
/** \copydoc uninitialized_fill() */
template< typename tForward, typename tType >
tForward uninitialized_fill_n( tForward, std::size_t, tType const& FLUX_RESTRICT );
/** \copydoc uninitialized_fill() */
template< class tAlloc, typename tForward, typename tType >
void uninitialized_fill( tAlloc& FLUX_RESTRICT, tForward, tForward, tType const& FLUX_RESTRICT ) noexcept((detail::IsNothrowUnCtor<tForward,tType>::value));
/** \copydoc uninitialized_fill() */
template< class tAlloc, typename tForward, typename tType >
tForward uninitialized_fill_n( tAlloc& FLUX_RESTRICT, tForward, std::size_t, tType const& FLUX_RESTRICT ) noexcept((detail::IsNothrowUnCtor<tForward,tType>::value));

/** Construct objects based on template in uninitialized memory
 *
 * Construct objects based on template in uninitialized memory, while passed-in 
 * iterators are updated as the function progresses.
 *
 * If an exception occurs, \a tForwardIt& will point to the first element that
 * was not successfully initialized:
   \code
   ptr a = ...;
   ptr b = a + ...;

   ptr at = a;
   uninitialized_fill_p( at, b, Template{} );
   \endcode
 * If no exception occurs, `at == b`. If an exception occurs, the range
 * \f$\left[\mbox{a} ... \mbox{at}\right)\f$ contains elements that were
 * successfully value constructed. 
 */
template< typename tForward, typename tType >
void uninitialized_fill_p( tForward& FLUX_RESTRICT, tForward, tType const& FLUX_RESTRICT ) noexcept((detail::IsNothrowUnCtor<tForward,tType>::value));
/** \copydoc uninitialized_fill_p() */
template< typename tForward, typename tType >
void uninitialized_fill_pn( tForward& FLUX_RESTRICT, std::size_t, tType const& FLUX_RESTRICT ) noexcept((detail::IsNothrowUnCtor<tForward,tType>::value));
/** \copydoc uninitialized_fill_p() */
template< class tAlloc, typename tForward, typename tType >
void uninitialized_fill_p( tAlloc&, tForward& FLUX_RESTRICT, tForward, tType const& FLUX_RESTRICT ) noexcept((detail::IsNothrowUnCtor<tForward,tType>::value));
/** \copydoc uninitialized_fill_p() */
template< class tAlloc, typename tForward, typename tType >
void uninitialized_fill_pn( tAlloc&, tForward& FLUX_RESTRICT, std::size_t, tType const& FLUX_RESTRICT ) noexcept((detail::IsNothrowUnCtor<tForward,tType>::value));

//--    destroy()                           ///{{{2///////////////////////////

/** Destroy element(s)
 *
 * Destroys specified element or range of elements, similar to <a
 * href="http:/en.cppreference.com/w/cpp/memory/destroy"> `std::destroy`</a> et
 * al..
 *
 * `ftl::destroy()` et al. optimize the case where the underlying type is
 * bitwise dismissible. In such a case, the `destroy()` methods do nothing
 * (other than potentially compute the returned iterator's value).
 *
 * Additional overloads that take an allocator as their first argument are
 * also provided.
 */
template< typename tForward >
void destroy( tForward, tForward ) noexcept(detail::IsNothrowDtor<tForward>::value);
/** \copydoc destroy() */
template< typename tForward >
tForward destroy_n( tForward, std::size_t ) noexcept(detail::IsNothrowDtor<tForward>::value);
/** \copydoc destroy() */
template< typename tType >
void destroy_at( tType* ) noexcept(std::is_nothrow_destructible<tType>::value);

/** \copydoc destroy() */
template< class tAlloc, typename tForward >
void destroy( tAlloc& FLUX_RESTRICT aAlloc, tForward, tForward ) noexcept(detail::IsNothrowDtor<tForward>::value);
/** \copydoc destroy() */
template< class tAlloc, typename tForward >
tForward destroy_n( tAlloc& FLUX_RESTRICT aAlloc, tForward, std::size_t ) noexcept(detail::IsNothrowDtor<tForward>::value);
/** \copydoc destroy() */
template< class tAlloc, typename tType >
void destroy_at( tAlloc& FLUX_RESTRICT aAlloc, tType* ) noexcept(std::is_nothrow_destructible<tType>::value);

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/uninitialized.inl"
#endif // FLUX_FTL_UNINITIALIZED_HPP_94651553_C3D7_4CE6_9766_97CEC8D284B4
