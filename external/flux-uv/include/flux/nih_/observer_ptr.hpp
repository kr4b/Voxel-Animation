/*-******************************************************* -- HEADER -{{{1- */
/*-	observer_ptr fallback
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_OBSERVER_PTR_HPP_05959AC7_8D58_4D63_92A3_D8E1B241C576
#define FLUX_NIH_OBSERVER_PTR_HPP_05959AC7_8D58_4D63_92A3_D8E1B241C576

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/constexpr.hpp>

#include <functional>
#include <type_traits>

#include <cstddef>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    observer_ptr                ///{{{1///////////////////////////////////
template< class tW >
class observer_ptr
{
	public:
		using element_type = tW;

		using Ref_ = typename std::add_lvalue_reference<element_type>::type;
	
	public:
		constexpr observer_ptr() noexcept;
		constexpr observer_ptr( std::nullptr_t ) noexcept;

		FLUX_CONSTEXPR_EX explicit observer_ptr( element_type* ) noexcept;

		template< class tOther > FLUX_CONSTEXPR_EX
		observer_ptr( observer_ptr<tOther> ) noexcept;

	public:
		constexpr explicit operator bool() const noexcept;
		constexpr explicit operator element_type*() const noexcept;

		constexpr Ref_ operator*() const;
		constexpr element_type* operator->() const noexcept;

	public:
		FLUX_CONSTEXPR_EX element_type* release() noexcept;
		FLUX_CONSTEXPR_EX void reset( element_type* = nullptr ) noexcept;
		FLUX_CONSTEXPR_EX void swap( observer_ptr& ) noexcept;

		constexpr element_type* get() const noexcept;

	private:
		element_type* mPtr;
};

//--    functions                   ///{{{1///////////////////////////////////

template< class tW >
observer_ptr<tW> make_observer( tW* ) noexcept;

template< class tW >
void swap( observer_ptr<tW>&, observer_ptr<tW>& ) noexcept;

//--    operators                   ///{{{1///////////////////////////////////

template< class tX, class tY >
bool operator== (observer_ptr<tX> const&, observer_ptr<tY> const&) noexcept;
template< class tX, class tY >
bool operator!= (observer_ptr<tX> const&, observer_ptr<tY> const&) noexcept;

template< class tX >
bool operator== (observer_ptr<tX> const&, std::nullptr_t) noexcept;
template< class tX >
bool operator== (std::nullptr_t, observer_ptr<tX> const&) noexcept;
template< class tX >
bool operator!= (observer_ptr<tX> const&, std::nullptr_t) noexcept;
template< class tX >
bool operator!= (std::nullptr_t, observer_ptr<tX> const&) noexcept;

template< class tX, class tY >
bool operator< (observer_ptr<tX> const&, observer_ptr<tY> const&) noexcept;
template< class tX, class tY >
bool operator> (observer_ptr<tX> const&, observer_ptr<tY> const&) noexcept;
template< class tX, class tY >
bool operator<= (observer_ptr<tX> const&, observer_ptr<tY> const&) noexcept;
template< class tX, class tY >
bool operator>= (observer_ptr<tX> const&, observer_ptr<tY> const&) noexcept;

//TODO: hash specialization

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/observer_ptr.inl"
#endif // FLUX_NIH_OBSERVER_PTR_HPP_05959AC7_8D58_4D63_92A3_D8E1B241C576
