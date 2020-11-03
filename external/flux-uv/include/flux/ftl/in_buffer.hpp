/*-******************************************************* -- HEADER -{{{1- */
/*-	Helper: check if pointer points to element in buffer
 * 
 * Uses `std::less<>`, since `operator<` et al. are technically undefined if 
 * pointers originate from different allocations. See
 *
 *   https://groups.google.com/forum/#!topic/comp.std.c++/YElyNHBQ-9Y
 *
 * `std::less<>` yields a total order even if `operator<` doesn't, as per
 *
 *   http://en.cppreference.com/w/cpp/utility/functional/less
 *
 * (Why? RTFC++ Standard. On "normal" machines (e.g. x86_64), this produces
 * the same code as `operator<`.)
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_IN_BUFFER_HPP_82CD54BF_67B4_490F_84CF_0557EE943692
#define FLUX_FTL_IN_BUFFER_HPP_82CD54BF_67B4_490F_84CF_0557EE943692

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/assert.hpp>

#include <functional>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    in_buffer()                         ///{{{2///////////////////////////

/** Check if pointer is inside a different pointer range
 *
 * Check if \a aPtr is inside of the range \f$\left[\mbox{aBufBeg},
 * \mbox{aBufEnd}\right)\f$. Uses `std::less` et al., since these provide a 
 * total order for pointers even if distinct objects are involved.
 *
 * See [std::less](http://en.cppreference.com/w/cpp/utility/functional/less).
 */
template< typename tType > inline constexpr
bool in_buffer( tType const* aBufBeg, tType const* aBufEnd, tType const* aPtr ) noexcept
{
	using Less_ = std::less<tType const*>;
	return FLUX_ASSERT( std::less_equal<tType const*>{}( aBufBeg, aBufEnd ) ),
		Less_{}( aPtr, aBufEnd ) && !Less_{}( aPtr, aBufBeg )
	;
}

/** Check if pointer range is inside a different pointer range
 *
 * Check if the range \f$\left[\mbox{aRngBeg}, \mbox{aRngEnd}\right)\f$ is
 * inside of the range \f$\left[\mbox{aBufBeg}, \mbox{aBufEnd}\right)\f$. Uses
 * `std::less` et al., since these provide a total order for pointers even if
 * distinct objects are involved.
 *
 * \note The empty range (\a aRngBeg == \a aRngEnd) is always considered to be 
 * inside of any other range, including any other empty range.
 *
 * See [std::less](http://en.cppreference.com/w/cpp/utility/functional/less).
 */
template< typename tType > inline constexpr
bool in_buffer( tType const* aBufBeg, tType const* aBufEnd, tType const* aRngBeg, tType const* aRngEnd ) noexcept
{
	using Less_ = std::less<tType const*>;
	using LEq_ = std::less_equal<tType const*>;
	return FLUX_ASSERT( LEq_{}( aBufBeg, aBufEnd ) && LEq_{}( aRngBeg, aRngEnd ) ), 
		aRngBeg == aRngEnd ||
		(LEq_{}( aRngEnd, aBufEnd ) && !Less_{}( aRngBeg, aBufBeg ))
	;
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_FTL_IN_BUFFER_HPP_82CD54BF_67B4_490F_84CF_0557EE943692
