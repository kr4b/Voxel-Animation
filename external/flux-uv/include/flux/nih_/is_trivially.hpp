/*-******************************************************* -- HEADER -{{{1- */
/*-	std::is_trivially_* fallbacks
 *
 * Note: that we rely on is_trivial<> begin available. This seems to be the
 * case for implementations that lack the other is_trivially_* traits.
 *
 * These is a best-effort implementation, and minor departures from the
 * "correct" results should be expected. (Even the standard is unsure about
 * this, for example, there are LWG issues relating to is_nothrow_constructible
 * and similar that are unresolved). In particular __has_trivial_copy() seems
 * to differ with GCC and Clang w.r.t. deleted copy assigment operators (and
 * there too is an open LWG issue).
 *
 * TODO: currently falls back on is_pod<>; is_trivial<> should maybe be 
 * sufficient?
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_IS_TRIVIALLY_HPP_B627E2CC_C29A_4562_B615_96FEAFE1E20A
#define FLUX_NIH_IS_TRIVIALLY_HPP_B627E2CC_C29A_4562_B615_96FEAFE1E20A

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <type_traits>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    is_trivially_*()            ///{{{1///////////////////////////////////
/* is_trivial<> requires 
 *   - is_trivially_copyable<> 
 *   - is_trivally_default_constructible<>
 * which means that is_trivial<> implies both of these.
 *
 * Note: technically GCC and Clang have __has_trivial_constructor
 */
template< typename tType > 
struct is_trivially_copyable
	: std::is_trivial<tType>
{};
template< typename tType >
struct is_trivially_default_constructible
	: std::is_trivial<tType>
{};

/* GCC and Clang have some builtins for these. POD types should always be
 * eligible.
 */
#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC || FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
template< typename tType >
struct is_trivially_copy_assignable
	: std::integral_constant< bool, __has_trivial_assign(tType) >
{};
template< typename tType >
struct is_trivially_copy_constructible
	: std::integral_constant< bool, __has_trivial_copy(tType) >
{};

template< typename tType >
struct is_trivially_destructible
	: std::integral_constant< bool, __has_trivial_destructor(tType) >
{};

#else // others
template< typename tType >
struct is_trivially_copy_assignable
	: std::is_pod<tType>
{};
template< typename tType >
struct is_trivially_copy_constructible
	: std::is_pod<tType>
{};

template< typename tType >
struct is_trivially_destructible
	: std::is_pod<tType>
{};

#endif // ~ COMPILER_FAMILY

/* POD types are trivially movable. (Not a particularly interesting case,
 * but whatever.
 */
template< typename tType >
struct is_trivially_move_assignable
	: std::is_pod<tType>
{};
template< typename tType >
struct is_trivially_move_constructible
	: std::is_pod<tType>
{};

/* In the general case, we don't know the following, albeit may be able to
 * derive results for some special cases from more specific traits.
 */
template< typename, typename... >
struct is_trivially_constructible
	: std::false_type
{};
template< typename tType >
struct is_trivially_constructible<tType>
	: is_trivially_default_constructible<tType>
{};
template< typename tType >
struct is_trivially_constructible<tType,tType>
	: is_trivially_copy_constructible<tType>
{};
template< typename tType >
struct is_trivially_constructible<tType,tType&&>
	: is_trivially_move_constructible<tType>
{};
template< typename tType >
struct is_trivially_constructible<tType,tType const&>
	: is_trivially_copy_constructible<tType>
{};

template< typename, typename >
struct is_trivially_assignable
	: std::false_type
{};
template< typename tType >
struct is_trivially_assignable<tType&,tType>
	: is_trivially_copy_assignable<tType>
{};
template< typename tType >
struct is_trivially_assignable<tType&,tType&&>
	: is_trivially_move_assignable<tType>
{};
template< typename tType >
struct is_trivially_assignable<tType&,tType const&>
	: is_trivially_copy_assignable<tType>
{};

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_NIH_IS_TRIVIALLY_HPP_B627E2CC_C29A_4562_B615_96FEAFE1E20A
