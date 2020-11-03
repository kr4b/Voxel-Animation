/*-******************************************************* -- HEADER -{{{1- */
/*-	Dynamic/Static Value Helpers
 *
 * Dynamic/Static value templates. These simpify the use of either a static or
 * a dynamic value in templates. Consider:
 *
 *   template< class tValue > auto square( tValue aValue )
 *   {
 *       return aValue * aValue;
 *   }
 *
 *   auto sv = square( StaticValue<unsigned, 5>{} );
 *   auto dv = square( DynamicValue<unsigned>(5) );
 *
 * `sv` will be of type `StaticValue<unsigned,25>` and `dv` will be of type
 * `DynamicValue<unsigned>` storing a runtime-value of 25. Both will decay to
 * their underlying integer type (`unsigned` in this case).
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_DSVALUE_HPP_40847A9D_230E_4E35_AE34_A8E96B5B38FB
#define FLUX_FTL_DSVALUE_HPP_40847A9D_230E_4E35_AE34_A8E96B5B38FB

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/assert.hpp>
#include <flux/compat/constexpr.hpp>

#include <cstddef>
#include <type_traits>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    fwd                                 ///{{{2///////////////////////////

/* This is a slightly lame mitigation to a Visual Studio 2015 limitation on the
 * symbol length. The Static/Dynamic values are used somewhat extensively in
 * template meta-programming and contribute to lengthy symbol names. This
 * reduces the problem slightly (albeit the architecture-astronautic flux-meta
 * is still problematic).
 *
 * Aliases with the original names are included below.
 */
template< typename > class DV;
template< typename tType, tType > class SV;

template< typename tType >
using DynamicValue = DV<tType>;
template< typename tType, tType tValue >
using StaticValue = SV<tType,tValue>;

//--    Static Value                        ///{{{2///////////////////////////
template< typename tType, tType tValue >
class SV
{
	public:
		SV() = default;
		SV( SV const& ) = default;
		SV& operator= (SV const&) = default;

		FLUX_CONSTEXPR_EX SV( tType aValue ) noexcept {
			/* MSVC 1913 (seen on 191326128) complains about the assert below
			 * not being constexpr (or possibly about it triggering). Neither
			 * really makes sense, though. Later MSVCs don't suffer from this.
			 *
			 * TODO: check with other VS2017 releases. The condition below
			 * could end up being e.g.,_MSC_VER > 1913
			 */
#			if FLUX_COMPILER != FLUX_COMPILER_MSVC || _MSC_FULL_VER > 191326128
			FLUX_ASSERT( tValue == aValue ), (void)aValue;
#			endif
		}

	public:
		using type = tType;

		static constexpr bool isStatic = true;
		static constexpr type staticValue = tValue;

	public:
		inline constexpr operator tType() const noexcept {
			return staticValue;
		}
		inline constexpr tType value() const noexcept {
			return staticValue;
		}
};

namespace detail
{
	/* This is a workaround for VisualStudio 2015. VS2015 will complain:
	 *   "error C2675: unary '+': 'tType' does not define this operator or a
	 *   conversion to a type acceptable to the predefined operator"
	 * if "+tValue" is used directly instead of the detour below in the
	 * unary operator definitions. This is probably a compiler bug?
	 */
	template< typename tType, tType tValue >
	struct SValUPlus
	{
		static constexpr decltype(+tValue) value = +tValue;
	};
	template< typename tType, tType tValue >
	struct SValUMinus
	{
		static constexpr decltype(-tValue) value = -tValue;
	};
	template< typename tType, tType tValue >
	struct SValUNot
	{
		static constexpr decltype(!tValue) value = !tValue;
	};
}

template< typename tType, tType tValue > inline constexpr
auto operator+ (SV<tType,tValue> const&) noexcept
	-> SV<decltype(+tValue),detail::SValUPlus<tType,tValue>::value> { return {}; }
template< typename tType, tType tValue > inline constexpr
auto operator- (SV<tType,tValue> const&) noexcept
	-> SV<decltype(-tValue),detail::SValUMinus<tType,tValue>::value> { return {}; }
template< typename tType, tType tValue > inline constexpr
auto operator! (SV<tType,tValue> const&) noexcept
	-> SV<decltype(!tValue),detail::SValUNot<tType,tValue>::value> { return {}; }

template< typename tLTy, tLTy tLVal, typename tRTy, tRTy tRVal > inline constexpr
auto operator+ (SV<tLTy,tLVal> const&, SV<tRTy,tRVal> const&) noexcept
	-> SV<decltype(tLVal+tRVal),tLVal+tRVal> { return {}; }
template< typename tLTy, tLTy tLVal, typename tRTy, tRTy tRVal > inline constexpr
auto operator- (SV<tLTy,tLVal> const&, SV<tRTy,tRVal> const&) noexcept
	-> SV<decltype(tLVal-tRVal),tLVal-tRVal> { return {}; }
template< typename tLTy, tLTy tLVal, typename tRTy, tRTy tRVal > inline constexpr
auto operator* (SV<tLTy,tLVal> const&, SV<tRTy,tRVal> const&) noexcept
	-> SV<decltype(tLVal*tRVal),tLVal*tRVal> { return {}; }
template< typename tLTy, tLTy tLVal, typename tRTy, tRTy tRVal > inline constexpr
auto operator/ (SV<tLTy,tLVal> const&, SV<tRTy,tRVal> const&) noexcept
	-> SV<decltype(tLVal/tRVal),tLVal/tRVal> { return {}; }

template< typename tLTy, tLTy tLVal, typename tRTy, tRTy tRVal > inline constexpr
auto operator& (SV<tLTy,tLVal> const&, SV<tRTy,tRVal> const&) noexcept
	-> SV<decltype(tLVal&tRVal),tLVal&tRVal> { return {}; }
template< typename tLTy, tLTy tLVal, typename tRTy, tRTy tRVal > inline constexpr
auto operator| (SV<tLTy,tLVal> const&, SV<tRTy,tRVal> const&) noexcept
	-> SV<decltype(tLVal|tRVal),tLVal|tRVal> { return {}; }
template< typename tLTy, tLTy tLVal, typename tRTy, tRTy tRVal > inline constexpr
auto operator^ (SV<tLTy,tLVal> const&, SV<tRTy,tRVal> const&) noexcept
	-> SV<decltype(tLVal^tRVal),tLVal^tRVal> { return {}; }
template< typename tLTy, tLTy tLVal, typename tRTy, tRTy tRVal > inline constexpr
auto operator<< (SV<tLTy,tLVal> const&, SV<tRTy,tRVal> const&) noexcept
	-> SV<decltype(tLVal<<tRVal),(tLVal<<tRVal)> { return {}; }
template< typename tLTy, tLTy tLVal, typename tRTy, tRTy tRVal > inline constexpr
auto operator>> (SV<tLTy,tLVal> const&, SV<tRTy,tRVal> const&) noexcept
	-> SV<decltype(tLVal>>tRVal),(tLVal>>tRVal)> { return {}; }

template< typename tLTy, tLTy tLVal, typename tRTy, tRTy tRVal > inline constexpr
auto operator== (SV<tLTy,tLVal> const&, SV<tRTy,tRVal> const&) noexcept
	-> SV<decltype(tLVal==tRVal),(tLVal==tRVal)> { return {}; }
template< typename tLTy, tLTy tLVal, typename tRTy, tRTy tRVal > inline constexpr
auto operator!= (SV<tLTy,tLVal> const&, SV<tRTy,tRVal> const&) noexcept
	-> SV<decltype(tLVal!=tRVal),(tLVal!=tRVal)> { return {}; }
template< typename tLTy, tLTy tLVal, typename tRTy, tRTy tRVal > inline constexpr
auto operator&& (SV<tLTy,tLVal> const&, SV<tRTy,tRVal> const&) noexcept
	-> SV<decltype(tLVal&&tRVal),(tLVal&&tRVal)> { return {}; }
template< typename tLTy, tLTy tLVal, typename tRTy, tRTy tRVal > inline constexpr
auto operator|| (SV<tLTy,tLVal> const&, SV<tRTy,tRVal> const&) noexcept
	-> SV<decltype(tLVal||tRVal),(tLVal||tRVal)> { return {}; }

template< typename tTy, tTy tLVal, tTy tRVal > inline constexpr
auto min( SV<tTy,tLVal> const&, SV<tTy,tRVal> const& ) noexcept
	-> SV<tTy, (tLVal < tRVal ? tLVal : tRVal)> { return {}; }
template< typename tTy, tTy tLVal, tTy tRVal > inline constexpr
auto max( SV<tTy,tLVal> const&, SV<tTy,tRVal> const& ) noexcept
	-> SV<tTy, (tLVal < tRVal ? tRVal : tLVal)> { return {}; }

//--    Dynamic Value                       ///{{{2///////////////////////////
template< typename tType >
class DV
{
	public:
		DV() = default; /*XXX-good or bad idea??*/

		inline constexpr DV( tType aValue /*= tType()*/ ) noexcept
			: mValue(aValue)
		{}

		template< tType tValue > inline
		constexpr DV( SV<tType,tValue> const& ) noexcept
			: mValue( tValue )
		{}
		template< tType tValue > inline
		FLUX_CONSTEXPR_EX DV& operator= (SV<tType,tValue> const&) noexcept
		{
			mValue = tValue;
			return *this;
		}

	public:
		using type = tType;

		static constexpr bool isStatic = false;

	public:
		inline constexpr operator tType() const noexcept {
			return mValue;
		}
		inline constexpr tType value() const noexcept {
			return mValue;
		}

	private:
		tType mValue;
};

template< typename tType > inline constexpr
auto operator+ (DV<tType> const& aValue) noexcept
	-> DV<decltype(+std::declval<tType>())> {
	return {+aValue.value()};
}
template< typename tType > inline constexpr
auto operator- (DV<tType> const& aValue) noexcept
	-> DV<decltype(-std::declval<tType>())> {
	return {-aValue.value()};
}
template< typename tType > inline constexpr
auto operator! (DV<tType> const& aValue) noexcept
	-> DV<decltype(!std::declval<tType>())> {
	return {!aValue.value()};
}

template< typename tLTy, typename tRTy > inline constexpr
auto operator+ (DV<tLTy> const& aLHS, DV<tRTy> const& aRHS) noexcept
	-> DV<decltype(std::declval<tLTy>()+std::declval<tRTy>())> {
	return {aLHS.value() + aRHS.value()};
}
template< typename tLTy, typename tRTy > inline constexpr
auto operator- (DV<tLTy> const& aLHS, DV<tRTy> const& aRHS) noexcept
	-> DV<decltype(std::declval<tLTy>()-std::declval<tRTy>())> {
	return {aLHS.value() - aRHS.value()};
}
template< typename tLTy, typename tRTy > inline constexpr
auto operator* (DV<tLTy> const& aLHS, DV<tRTy> const& aRHS) noexcept
	-> DV<decltype(std::declval<tLTy>()*std::declval<tRTy>())> {
	return {aLHS.value() * aRHS.value()};
}
template< typename tLTy, typename tRTy > inline constexpr
auto operator/ (DV<tLTy> const& aLHS, DV<tRTy> const& aRHS) noexcept
	-> DV<decltype(std::declval<tLTy>()/std::declval<tRTy>())> {
	return {aLHS.value() / aRHS.value()};
}

template< typename tLTy, typename tRTy > inline constexpr
auto operator& (DV<tLTy> const& aLHS, DV<tRTy> const& aRHS) noexcept
	-> DV<decltype(std::declval<tLTy>()&std::declval<tRTy>())> {
	return {aLHS.value() & aRHS.value()};
}
template< typename tLTy, typename tRTy > inline constexpr
auto operator| (DV<tLTy> const& aLHS, DV<tRTy> const& aRHS) noexcept
	-> DV<decltype(std::declval<tLTy>()|std::declval<tRTy>())> {
	return {aLHS.value() | aRHS.value()};
}
template< typename tLTy, typename tRTy > inline constexpr
auto operator^ (DV<tLTy> const& aLHS, DV<tRTy> const& aRHS) noexcept
	-> DV<decltype(std::declval<tLTy>()^std::declval<tRTy>())> {
	return {aLHS.value() ^ aRHS.value()};
}
template< typename tLTy, typename tRTy > inline constexpr
auto operator<< (DV<tLTy> const& aLHS, DV<tRTy> const& aRHS) noexcept
	-> DV<decltype(std::declval<tLTy>()<<std::declval<tRTy>())> {
	return {aLHS.value() << aRHS.value()};
}
template< typename tLTy, typename tRTy > inline constexpr
auto operator>> (DV<tLTy> const& aLHS, DV<tRTy> const& aRHS) noexcept
	-> DV<decltype(std::declval<tLTy>()>>std::declval<tRTy>())> {
	return {aLHS.value() >> aRHS.value()};
}

template< typename tLTy, typename tRTy > inline constexpr
auto operator== (DV<tLTy> const& aLHS, DV<tRTy> const& aRHS) noexcept
	-> DV<decltype(std::declval<tLTy>()==std::declval<tRTy>())> {
	return {aLHS.value() == aRHS.value()};
}
template< typename tLTy, typename tRTy > inline constexpr
auto operator!= (DV<tLTy> const& aLHS, DV<tRTy> const& aRHS) noexcept
	-> DV<decltype(std::declval<tLTy>()!=std::declval<tRTy>())> {
	return {aLHS.value() != aRHS.value()};
}
template< typename tLTy, typename tRTy > inline constexpr
auto operator&& (DV<tLTy> const& aLHS, DV<tRTy> const& aRHS) noexcept
	-> DV<decltype(std::declval<tLTy>()&&std::declval<tRTy>())> {
	return {aLHS.value() && aRHS.value()};
}
template< typename tLTy, typename tRTy > inline constexpr
auto operator|| (DV<tLTy> const& aLHS, DV<tRTy> const& aRHS) noexcept
	-> DV<decltype(std::declval<tLTy>()||std::declval<tRTy>())> {
	return {aLHS.value() || aRHS.value()};
}

template< typename tTy > inline constexpr
auto min( DV<tTy> const& aX, DV<tTy> const& aY ) noexcept
	-> DV<tTy> {
	return {aX.value() < aY.value() ? aX.value() : aY.value()};
}
template< typename tTy > inline constexpr
auto max( DV<tTy> const& aX, DV<tTy> const& aY ) noexcept
	-> DV<tTy> {
	return {aX.value() < aY.value() ? aX.value() : aY.value()};
}

//--    Default Types                       ///{{{2///////////////////////////

template< std::size_t tValue >
using StaticSize = SV< std::size_t, tValue >;
using DynamicSize = DV< std::size_t >;

//--    Literals                            ///{{{2///////////////////////////
namespace literals
{
	namespace detail
	{
		template< std::size_t, char... > struct StaticSizeImpl;
		template< std::size_t tVal, char tHead, char... tTail >
		struct StaticSizeImpl< tVal, tHead, tTail... > {
			static constexpr std::size_t value = StaticSizeImpl<
				tVal * std::size_t(10) + std::size_t(tHead - '0'),
				tTail...
			>::value;
		};
		template< std::size_t tVal >
		struct StaticSizeImpl< tVal > {
			static constexpr std::size_t value = tVal;
		};
	}

	template< char... tChar > inline constexpr
	auto operator ""_ssize() -> StaticSize<
		detail::StaticSizeImpl< 0, tChar... >::value
	> { return {}; }
}

//--    meta functions                      ///{{{2///////////////////////////

namespace detail
{
	template< class > struct IsDSValueImpl
		: std::false_type
	{};
	template< typename tType, tType tValue >
	struct IsDSValueImpl< SV<tType,tValue> >
		: std::true_type
	{};
	template< typename tType >
	struct IsDSValueImpl< DV<tType> >
		: std::true_type
	{};

	template< class > struct IsSVImpl
		: std::false_type
	{};
	template< typename tType, tType tValue >
	struct IsSVImpl< SV<tType,tValue> >
		: std::true_type
	{};

	template< class > struct IsDVImpl
		: std::false_type
	{};
	template< typename tType >
	struct IsDVImpl< DV<tType> >
		: std::true_type
	{};
}

template< class tValue > struct is_dsvalue
	: detail::IsDSValueImpl< typename std::decay<tValue>::type >
{};

template< class tValue > struct is_static_value
	: detail::IsSVImpl< typename std::decay<tValue>::type >
{};
template< class tValue > struct is_dynamic_value
	: detail::IsDVImpl< typename std::decay<tValue>::type >
{};

#if FLUX_LANGFEAT_VARIABLE_TEMPLATE
template< class tValue > constexpr
bool is_dsvalue_v = is_dsvalue<tValue>::value;
template< class tValue > constexpr
bool is_static_value_v = is_static_value<tValue>::value;
template< class tValue > constexpr
bool is_dynamic_value_v = is_dynamic_value<tValue>::value;
#endif // ~ LANGFEAT_VARIABLE_TEMPLATE

//--    Static/Dynamic Mixed Operators      ///{{{2///////////////////////////
//TODO

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab:
//#include "detail/dsvalue.inl"
#endif // FLUX_FTL_DSVALUE_HPP_40847A9D_230E_4E35_AE34_A8E96B5B38FB
