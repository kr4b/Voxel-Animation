/*-******************************************************* -- HEADER -{{{1- */
/*-	std::to_address() fallback
 *
 * Not entierly compilant to the C++2a version. The C++2a version doesn't form
 * a reference to the pointee, but we can't avoid that without compiler magic.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_TO_ADDRESS_HPP_D05EBCB0_B910_4085_B85B_7347800F1E7B
#define FLUX_NIH_TO_ADDRESS_HPP_D05EBCB0_B910_4085_B85B_7347800F1E7B

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>


#include <memory>
#include <utility>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    to_address()                ///{{{1///////////////////////////////////
namespace detail
{
	template< unsigned tRank > 
	struct ToAddrRank_
		: ToAddrRank_<tRank-1> 
	{};

	template<> 
	struct ToAddrRank_<0> 
	{};
	
	template< class tPtr > inline
	auto to_addr_( tPtr const& aPtr, ToAddrRank_<1> ) noexcept
		-> decltype(aPtr.operator->())
	{
		return aPtr.operator->();
	}
	template< class tPtr > inline
	auto to_addr_( tPtr const& aPtr, ToAddrRank_<0> ) noexcept
		-> decltype(std::addressof(*aPtr))
	{
		return std::addressof(*aPtr);
	}
}

template< class tPtr > inline
auto to_address( tPtr const& aPtr ) noexcept
	-> decltype(detail::to_addr_(aPtr, detail::ToAddrRank_<1>{}))
{
	return detail::to_addr_(aPtr, detail::ToAddrRank_<1>{});
}

template< typename tType > inline constexpr
tType* to_address( tType* aRawPtr ) noexcept 
{
	return aRawPtr;
}

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_NIH_TO_ADDRESS_HPP_D05EBCB0_B910_4085_B85B_7347800F1E7B
