/*-******************************************************* -- HEADER -{{{1- */
/*-	Helpers: initialized_*, initialized_*_n, initialized_*_p..
 *
 * Functions similar to `uninitialzied_*()` except that the destination already
 * contains valid objects (to be overwritten or otherwise manhandled).
 *
 *  TODO: relocate_rev_*() ???? 
 *      - would require that we can relocate overlapping ranges, which currently
 *        isn't the case.
 *
 *  TODO: stricter noexcept for move_if_nothrow (if copy is noexcept)
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_INITIALIZED_HPP_83AA0028_174E_42D5_AA0F_7472C4E0A363
#define FLUX_FTL_INITIALIZED_HPP_83AA0028_174E_42D5_AA0F_7472C4E0A363

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/ftl/dsvalue.hpp>

#include <cstddef>
#include <utility>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    config                              ///{{{2///////////////////////////
namespace aspect
{
	//TODO: base this on C++17 hardware_constructive_interference_size
	constexpr std::size_t kInitializedSwapBufferBytes = 128;
}

//--    noexcept helpers                    ///{{{2///////////////////////////
namespace detail
{
	template< typename, typename > struct IsNothrowInCtor;
	template< typename, typename > struct IsNothrowInCopy;
	template< typename, typename > struct IsNothrowInMove;
	template< typename, typename > struct IsNothrowInReloc;
	template< typename, typename > struct IsNothrowInSwap;

	template< typename, typename > struct InitSwapElements;
}

//--    initialized_copy*()                 ///{{{2///////////////////////////
/** Copy elements
 *
 * Copy elements from the range \f$\left[\mbox{aBeg}, \mbox{aEnd}\right)\f$
 * to \a aOut, similar to `std::copy`. The output iterator \a tForwardIt must
 * be outside of the range \f$\left[\mbox{aBeg}, \mbox{aEnd}\right)\f$ (but
 * the ranges may overlap otherwise).
 *
 * `initialized_copy()` optimizes the case where the both source and 
 * destination refer to the same type and
 *   - are in a continuous buffer
 *   - are `is_bitwise_duplicable` and `is_bitwise_dismissible`.
 * In this case, `initialized_copy()` will perform a single `memmove()` to
 * copy the elements from the source to the destination.
 */
template< typename tInputIt, typename tForwardIt >
tForwardIt initialized_copy( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut );
/** \copydoc initialized_copy() */
template< typename tInputIt, typename tForwardIt >
std::pair<tInputIt,tForwardIt> initialized_copy_n( tInputIt, std::size_t, tForwardIt );

/** Copy elements
 *
 * Copy elements from the range \f$\left[\mbox{aBeg}, \mbox{aEnd}\right)\f$
 * to \a aOut, while updating the passed-in iterators as the function
 * progresses:
   \code
   input a = ..., b = ...;
   output c = ...;
  
   auto at = a;
   auto ct = c;
   initialized_copy_p( at, b, ct );
   \endcode
 * If no exception occurs, then `at == b` and `ct` equals the iterator that
 * would have been returned by `initialized_copy()`. If an exception occurs,
 * then the range \f$\left[\mbox{c},\mbox{ct}\right)\f$ contains elements
 * that were successfully copied from the range \f$\left[\mbox{a}, \mbox{at}
 * \right)\f$.
 *
 * `initialized_copy()` optimizes the case where the both source and 
 * destination refer to the same type and
 *   - are in a continuous buffer
 *   - are `is_bitwise_duplicable` and `is_bitwise_dismissible`.
 * In this case, `initialized_copy()` will perform a single `memmove()` to
 * copy the elements from the source to the destination.
 */
template< typename tInputIt, typename tForwardIt >
void initialized_copy_p( tInputIt&, tInputIt, tForwardIt& ) noexcept((detail::IsNothrowInCopy<tInputIt,tForwardIt>::value));
/** \copydoc initialized_copy_p() */
template< typename tInputIt, typename tForwardIt >
void initialized_copy_pn( tInputIt&, std::size_t, tForwardIt& ) noexcept((detail::IsNothrowInCopy<tInputIt,tForwardIt>::value));
/** \copydoc initialized_copy_p() */
template< typename tInputIt, typename tForwardIt >
void initialized_copy_p( tInputIt&, tInputIt, tForwardIt&, tForwardIt ) noexcept((detail::IsNothrowInCopy<tInputIt,tForwardIt>::value));

//--    initialized_copy_rev*()             ///{{{2///////////////////////////
/** Copy elements backwards
 *
 * TODO
 */
template< typename tBidiItA, typename tBidiItB >
tBidiItB initialized_copy_rev( tBidiItA aBeg, tBidiItA aEnd, tBidiItB aEndOut );
/** \copydoc initialized_copy_rev() */
template< typename tBidiItA, typename tBidiItB >
std::pair<tBidiItA,tBidiItB> initialized_copy_rev_n( tBidiItA aEnd, std::size_t aCount, tBidiItB aEndOut );

/** Copy elements backwards
 *
 * TODO
 */
template< typename tBidiItA, typename tBidiItB >
void initialized_copy_rev_p( tBidiItA aBeg, tBidiItA& aEnd, tBidiItB& aEndOut ) noexcept((detail::IsNothrowInCopy<tBidiItA,tBidiItB>::value));
/** \copydoc initialized_copy_rev_p() */
template< typename tBidiItA, typename tBidiItB >
void initialized_copy_rev_pn( tBidiItA& aEnd, std::size_t aCount, tBidiItB& aEndOut ) noexcept((detail::IsNothrowInCopy<tBidiItA,tBidiItB>::value));

//--    initialized_move*()                 ///{{{2///////////////////////////
/** Move elements
 *
 * Move elements from the range \f$\left[\mbox{aBeg}, \mbox{aEnd}\right)\f$
 * to \a aOut.
 *
 * `initialized_move()` optimizes the case where the both source and 
 * destination refer to the same type and
 *   - are in a continuous buffer
 *   - are `is_bitwise_duplicable` and `is_bitwise_dismissible`.
 * In this case, `initialized_move()` will perform a single `memmove()` to
 * copy the elements from the source to the destination.
 */
template< typename tInputIt, typename tForwardIt >
tForwardIt initialized_move( tInputIt, tInputIt, tForwardIt ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value));
/** \copydoc initialized_move() */
template< typename tInputIt, typename tForwardIt >
std::pair<tInputIt,tForwardIt> initialized_move_n( tInputIt, std::size_t, tForwardIt );

/** Move elements
 *
 * Move elements from the range \f$\left[\mbox{aBeg}, \mbox{aEnd}\right)\f$
 * to \a aOut, while updating the passed-in iterators as the function
 * progresses:
   \code
   input a = ..., b = ...;
   output c = ...;
  
   auto at = a;
   auto ct = c;
   initialized_move_p( at, b, ct );
   \endcode
 * If no exception occurs, then `at == b` and `ct` equals the iterator that
 * would have been returned by `initialized_move()`. If an exception occurs,
 * then the range \f$\left[\mbox{c},\mbox{ct}\right)\f$ contains elements
 * that were successfully copied from the range \f$\left[\mbox{a}, \mbox{at}
 * \right)\f$.
 *
 * `initialized_move_p()` optimizes the case where the both source and 
 * destination refer to the same type and
 *   - are in a continuous buffer
 *   - are `is_bitwise_duplicable` and `is_bitwise_dismissible`.
 * In this case, `initialized_move_p()` will perform a single `memmove()` to
 * copy the elements from the source to the destination.
 */
template< typename tInputIt, typename tForwardIt >
void initialized_move_p( tInputIt&, tInputIt, tForwardIt& ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value));
/** \copydoc initialized_move_p() */
template< typename tInputIt, typename tForwardIt >
void initialized_move_pn( tInputIt&, std::size_t, tForwardIt& ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value));
/** \copydoc initialized_move_p() */
template< typename tInputIt, typename tForwardIt >
void initialized_move_p( tInputIt&, tInputIt, tForwardIt&, tForwardIt ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value));

//--    initialized_move_rev*()             ///{{{2///////////////////////////
/** Move elements backwards
 *
 * TODO
 */
template< typename tBidiItA, typename tBidiItB >
tBidiItB initialized_move_rev( tBidiItA aBeg, tBidiItA aEnd, tBidiItB aEndOut )  noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value));
/** \copydoc initialized_move_rev() */
template< typename tBidiItA, typename tBidiItB >
std::pair<tBidiItA,tBidiItB> initialized_move_rev_n( tBidiItA aEnd, std::size_t aCount, tBidiItB aEndOut );

/** Move elements backwards
 *
 * TODO
 */
template< typename tBidiItA, typename tBidiItB >
void initialized_move_rev_p( tBidiItA aBeg, tBidiItA& aEnd, tBidiItB& aEndOut ) noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value));
/** \copydoc initialized_move_rev_p() */
template< typename tBidiItA, typename tBidiItB >
void initialized_move_rev_pn( tBidiItA& aEnd, std::size_t aCount, tBidiItB& aEndOut ) noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value));

//--    initialized_move_if_noexcept*()     ///{{{2///////////////////////////
/** Move/Copy elements
 *
 * Move/Copy elements from the range \f$\left[\mbox{aBeg}, 
 * \mbox{aEnd}\right)\f$ to \a aOut.
 *
 * `initialized_move_if_noexcept()` optimizes the case where the both source
 * and destination refer to the same type and
 *   - are in a continuous buffer
 *   - are `is_bitwise_duplicable` and `is_bitwise_dismissible`.
 * In this case, `initialized_move_if_noexcept()` will perform a single 
 * `memmove()` to copy the elements from the source to the destination.
 */
template< typename tInputIt, typename tForwardIt >
tForwardIt initialized_move_if_noexcept( tInputIt, tInputIt, tForwardIt ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value));
/** \copydoc initialized_move_if_noexcept() */
template< typename tInputIt, typename tForwardIt >
std::pair<tInputIt,tForwardIt> initialized_move_if_noexcept_n( tInputIt, std::size_t, tForwardIt );

/** Move/Copy elements
 *
 * Move/Copy elements from the range \f$\left[\mbox{aBeg}, 
 * \mbox{aEnd}\right)\f$ to \a aOut, while updating the passed-in iterators as
 * the function progresses:
   \code
   input a = ..., b = ...;
   output c = ...;
  
   auto at = a;
   auto ct = c;
   initialized_move_if_noexcept_p( at, b, ct );
   \endcode
 * If no exception occurs, then `at == b` and `ct` equals the iterator that
 * would have been returned by `initialized_move_if_noexcept()`. If an 
 * exception occurs, then the range \f$\left[\mbox{c},\mbox{ct}\right)\f$ 
 * contains elements that were successfully copied from the range 
 * \f$\left[\mbox{a}, \mbox{at}\right)\f$.
 *
 * `initialized_move_if_noexcept_p()` optimizes the case where the both source
 *  and destination refer to the same type and
 *   - are in a continuous buffer
 *   - are `is_bitwise_duplicable` and `is_bitwise_dismissible`.
 * In this case, `initialized_move_if_noexcept_p()` will perform a single 
 * `memmove()` to copy the elements from the source to the destination.
 */
template< typename tInputIt, typename tForwardIt >
void initialized_move_if_noexcept_p( tInputIt&, tInputIt, tForwardIt& ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value));
/** \copydoc initialized_move_if_noexcept_p() */
template< typename tInputIt, typename tForwardIt >
void initialized_move_if_noexcept_pn( tInputIt&, std::size_t, tForwardIt& ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value));
/** \copydoc initialized_move_if_noexcept_p() */
template< typename tInputIt, typename tForwardIt >
void initialized_move_if_noexcept_p( tInputIt&, tInputIt, tForwardIt&, tForwardIt ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value));

//--    initialized_move_if_noexcept_rev*() ///{{{2///////////////////////////
/** Move/copy elements backwards
 *
 * TODO
 */
template< typename tBidiItA, typename tBidiItB >
tBidiItB initialized_move_if_noexcept_rev( tBidiItA aBeg, tBidiItA aEnd, tBidiItB aEndOut ) noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value));
/** \copydoc initialized_move_if_noexcept_rev() */
template< typename tBidiItA, typename tBidiItB >
std::pair<tBidiItA,tBidiItB> initialized_move_if_noexcept_n( tBidiItA aEnd, std::size_t aCount, tBidiItB aEndOut );

/** Move/copy elements backwards
 *
 * TODO
 */
template< typename tBidiItA, typename tBidiItB >
void initialized_move_if_noexcept_rev_p( tBidiItA aBeg, tBidiItA& aEnd, tBidiItB& aEndOut ) noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value));
/** \copydoc initialized_move_if_noexcept_rev_p() */
template< typename tBidiItA, typename tBidiItB >
void initialized_move_if_noexcept_rev_pn( tBidiItA& aEnd, std::size_t aCount, tBidiItB& aEndOut ) noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value));

//--    initialized_relocate*()             ///{{{2///////////////////////////
/** Relocate elements
 *
 * TODO/WARNING: cannot call on overlapping ranges currently!
 * 
 * Relocate elements from the range \f$\left[\mbox{aBeg}, \mbox{aEnd}\right)\f$
 * to \a aOut. `initialized_relocate()` leaves the input range uninitialized,
 * as if by the following:
   \code
   void initialized_relocate( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut )
   {
   		initialized_move( aBeg, aEnd, aOut );
   		destroy( aBeg, aEnd )
   }
   \endcode
 *
 * `initialized_relocate()` optimizes the case where the both source and 
 * destination refer to the same type and
 *   - are in a continuous buffer
 *   - are `is_bitwise_relocatable` and `is_bitwise_dismissible`.
 * In this case, `initialized_relocate()` will perform a single `memmove()` to
 * copy the elements from the source to the destination.
 */
template< typename tInputIt, typename tForwardIt >
tForwardIt initialized_relocate( tInputIt, tInputIt, tForwardIt ) noexcept((detail::IsNothrowInReloc<tInputIt,tForwardIt>::value));
/** \copydoc initialized_relocate() */
template< typename tInputIt, typename tForwardIt >
std::pair<tInputIt,tForwardIt> initialized_relocate_n( tInputIt, std::size_t, tForwardIt );

/** Relocate elements
 *
 * TODO/WARNING: cannot call on overlapping ranges currently!
 *
 * Relocate elements from the range \f$\left[\mbox{aBeg}, \mbox{aEnd}\right)\f$
 * to \a aOut, while updating the passed-in iterators as the function
 * progresses:
   \code
   input a = ..., b = ...;
   output c = ...;
  
   auto at = a;
   auto ct = c;
   initialized_relocate_p( at, b, ct );
   \endcode
 * If no exception occurs, then `at == b` and `ct` equals the iterator that
 * would have been returned by `initialized_relocate()`. If an exception occurs,
 * then the range \f$\left[\mbox{c},\mbox{ct}\right)\f$ contains elements
 * that were successfully moved from the range \f$\left[\mbox{a}, \mbox{at}
 * \right)\f$. The range \f$\left[\mbox{a}, \mbox{at}\right)\f$ <b>always</b>
 * denotes uninitialized storage, regardless of whether the function returns
 * or throws.
 *
 * `initialized_relocate_p()` optimizes the case where the both source and 
 * destination refer to the same type and
 *   - are in a continuous buffer
 *   - are `is_bitwise_relocatable` and `is_bitwise_dismissible`.
 * In this case, `initialized_relocate_p()` will perform a single `memmove()` to
 * copy the elements from the source to the destination.
 */
template< typename tInputIt, typename tForwardIt >
void initialized_relocate_p( tInputIt&, tInputIt, tForwardIt& ) noexcept((detail::IsNothrowInReloc<tInputIt,tForwardIt>::value));
/** \copydoc initialized_relocate_p() */
template< typename tInputIt, typename tForwardIt >
void initialized_relocate_pn( tInputIt&, std::size_t, tForwardIt& ) noexcept((detail::IsNothrowInReloc<tInputIt,tForwardIt>::value));
/** \copydoc initialized_relocate_p() */
template< typename tInputIt, typename tForwardIt >
void initialized_relocate_p( tInputIt&, tInputIt, tForwardIt&, tForwardIt ) noexcept((detail::IsNothrowInReloc<tInputIt,tForwardIt>::value));


//NOTE: there are initialized_relocate_rev(); these do not make sense with the
//      separate destroy() step.

//--    initialized_relocate_if_noexcept*()     ///{{{2///////////////////////
/** Relocate elements
 *
 * Relocate elements from the range \f$\left[\mbox{aBeg}, \mbox{aEnd}\right)\f$
 * to \a aOut. `initialized_relocate_if_noexcept()` leaves the input range 
 * uninitialized, as if by the following:
   \code
   void initialized_relocate_if_noexcept( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut )
   {
   		initialized_move_if_noexcept( aBeg, aEnd, aOut );
   		destroy( aBeg, aEnd )
   }
   \endcode
 *
 * `initialized_relocate_if_noexcept()` optimizes the case where the both
 * source and destination refer to the same type and
 *   - are in a continuous buffer
 *   - are `is_bitwise_relocatable` and `is_bitwise_dismissible`.
 * In this case, `initialized_relocate_if_noexcept()` will perform a single 
 * `memmove()` to copy the elements from the source to the destination.
 */
template< typename tInputIt, typename tForwardIt >
tForwardIt initialized_relocate_if_noexcept( tInputIt, tInputIt, tForwardIt ) noexcept((detail::IsNothrowInReloc<tInputIt,tForwardIt>::value));
/** \copydoc initialized_relocate_if_noexcept() */
template< typename tInputIt, typename tForwardIt >
std::pair<tInputIt,tForwardIt> initialized_relocate_if_noexcept_n( tInputIt, std::size_t, tForwardIt );

/** Relocate elements
 *
 * Relocate elements from the range \f$\left[\mbox{aBeg}, \mbox{aEnd}\right)\f$
 * to \a aOut, while updating the passed-in iterators as the function
 * progresses:
   \code
   input a = ..., b = ...;
   output c = ...;
  
   auto at = a;
   auto ct = c;
   initialized_relocate_if_noexcept_p( at, b, ct );
   \endcode
 * If no exception occurs, then `at == b` and `ct` equals the iterator that
 * would have been returned by `initialized_relocate_if_noexcept()`. If an
 * exception occurs, then the range \f$\left[\mbox{c},\mbox{ct}\right)\f$ 
 * contains elements that were successfully copied from the range 
 * \f$\left[\mbox{a}, \mbox{at}\right)\f$. The input range is, however, left
 * intact (and `at` is left at its original value).
 *
 * \note The range \f$\left[\mbox{a}, \mbox{at}\right)\f$ may always be assumed
 * to contain uninitialized memory. It is either an empty range or the complete 
 * range.
 *
 * `initialized_relocate_if_noexcept_p()` optimizes the case where the both
 * source and destination refer to the same type and
 *   - are in a continuous buffer
 *   - are `is_bitwise_relocatable` and `is_bitwise_dismissible`.
 * In this case, `initialized_relocate_if_noexcept_p()` will perform a 
 * single `memmove()` to copy the elements from the source to the destination.
 */
template< typename tInputIt, typename tForwardIt >
void initialized_relocate_if_noexcept_p( tInputIt&, tInputIt, tForwardIt& ) noexcept((detail::IsNothrowInReloc<tInputIt,tForwardIt>::value));
/** \copydoc initialized_relocate_if_noexcept_p() */
template< typename tInputIt, typename tForwardIt >
void initialized_relocate_if_noexcept_pn( tInputIt&, std::size_t, tForwardIt& ) noexcept((detail::IsNothrowInReloc<tInputIt,tForwardIt>::value));
/** \copydoc initialized_relocate_if_noexcept_p() */
template< typename tInputIt, typename tForwardIt >
void initialized_relocate_if_noexcept_p( tInputIt&, tInputIt, tForwardIt&, tForwardIt ) noexcept((detail::IsNothrowInReloc<tInputIt,tForwardIt>::value));

//--    initialized_swap*()                 ///{{{2///////////////////////////
/** Swap elements
 *
 * Swap elements from the range \f$\left[\mbox{aBeg}, \mbox{aEnd}\right)\f$
 * with \a aOut.
 *
 * `initialized_swap()` optimizes the case where the both source and 
 * destination refer to the same type and
 *   - are in a continuous buffer
 *   - are `is_bitwise_relocatable`
 * In this case, `initialized_swap()` will perform a number of `memmove()`s.
 */
template< typename tInputA, typename tInputB, std::size_t tValue = detail::InitSwapElements<tInputA,tInputB>::value >
tInputB initialized_swap( tInputA, tInputA, tInputB, StaticSize<tValue> = detail::InitSwapElements<tInputA,tInputB>::def() ) noexcept((detail::IsNothrowInSwap<tInputA,tInputB>::value));
/** \copydoc initialized_swap() */
template< typename tInputA, typename tInputB , std::size_t tValue = detail::InitSwapElements<tInputA,tInputB>::value>
std::pair<tInputA,tInputB> initialized_swap_n( tInputA, std::size_t, tInputB, StaticSize<tValue> = detail::InitSwapElements<tInputA,tInputB>::def() );

/** Swap elements
 *
 * Swap elements from the range \f$\left[\mbox{aBeg}, \mbox{aEnd}\right)\f$
 * with \a aOut, while updating the passed-in iterators as the function
 * progresses:
   \code
   input a = ..., b = ...;
   output c = ...;
  
   auto at = a;
   auto ct = c;
   initialized_swap_p( at, b, ct );
   \endcode
 * If no exception occurs, then `at == b` and `ct` equals the iterator that
 * would have been returned by `initialized_swap()`. If an exception occurs,
 * then the ranges \f$\left[\mbox{a}, \mbox{at}\right)\f$ and \f$\left[
 * \mbox{c}, \mbox{ct}\right)\f$ contain the elements which were successully
 * exchanged between the two ranges. The remaining elements are unchanged.
 *
 * `initialized_swap_p()` optimizes the case where the both source and 
 * destination refer to the same type and
 *   - are in a continuous buffer
 *   - are `is_bitwise_relocatable`
 * In this case, `initialized_swap_p()` will perform a number of `memmove()`s.
 */
template< typename tInputA, typename tInputB, std::size_t tValue = detail::InitSwapElements<tInputA,tInputB>::value >
void initialized_swap_p( tInputA&, tInputA, tInputB&, StaticSize<tValue> = detail::InitSwapElements<tInputA,tInputB>::def() ) noexcept((detail::IsNothrowInSwap<tInputA,tInputB>::value));
/** \copydoc initialized_swap_p() */
template< typename tInputA, typename tInputB, std::size_t tValue = detail::InitSwapElements<tInputA,tInputB>::value >
void initialized_swap_pn( tInputA&, std::size_t, tInputB&, StaticSize<tValue> = detail::InitSwapElements<tInputA,tInputB>::def() ) noexcept((detail::IsNothrowInSwap<tInputA,tInputB>::value));

//--    initialized_fill*()                 ///{{{2///////////////////////////
/** Fill elements
 *
 * Fill elements in the range \f$\left[\mbox{aBeg}, \mbox{aEnd}\right)\f$ with
 * \a aOut, similar to `std::fill`.
 */
template< typename tForwardIt, typename tType >
void initialized_fill( tForwardIt aBeg, tForwardIt aEnd, tType const& aVal );
/** \copydoc initialized_fill() */
template< typename tForwardIt, typename tType >
tForwardIt initialized_fill_n( tForwardIt, std::size_t, tType const& aVal );

/** Fill elements
 *
 * Fill elements in the range \f$\left[\mbox{aBeg}, \mbox{aEnd}\right)\f$ with
 * \a aOut, while updating the passed-in iterators as the function progresses:
   \code
   output a = ..., b = ...;
  
   auto at = a;
   initialized_fill_p( at, b, X{} );
   \endcode
 * If no exception occurs, then `at == b`. If an exception occurs, then the
 * range \f$\left[\mbox{a},\mbox{at}\right)\f$ contains elements that were
 * successfully initialized from the passed in value.
 */
template< typename tForwardIt, typename tType >
void initialized_fill_p( tForwardIt&, tForwardIt, tType const& ) noexcept((detail::IsNothrowInCtor<tForwardIt,tType const&>::value));
/** \copydoc initialized_fill_p() */
template< typename tForwardIt, typename tType >
void initialized_fill_pn( tForwardIt&, std::size_t, tType const& ) noexcept((detail::IsNothrowInCtor<tForwardIt,tType const&>::value));

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/initialized.inl"
#endif // FLUX_FTL_INITIALIZED_HPP_83AA0028_174E_42D5_AA0F_7472C4E0A363
