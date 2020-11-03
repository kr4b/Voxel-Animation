/*-******************************************************* -- HEADER -{{{1- */
/*-	Short type traits (C++14 / C++17)
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_TYPE_TRAITS_HPP_EB36C8E6_AF44_4399_8050_6334BA718AB9
#define FLUX_COMPAT_TYPE_TRAITS_HPP_EB36C8E6_AF44_4399_8050_6334BA718AB9

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>

#include <type_traits>

//--    type traits                 ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX14_TYPE_TRAITS

FLUX_NAMESPACE_ENTER1(compat)
	using std::remove_cv_t;
	using std::remove_const_t;
	using std::remove_volatile_t;

	using std::add_cv_t;
	using std::add_const_t;
	using std::add_volatile_t;

	using std::remove_reference_t;
	using std::add_lvalue_reference_t;
	using std::add_rvalue_reference_t;
	
	using std::add_pointer_t;
	using std::remove_pointer_t;

	using std::decay_t;
	using std::common_type_t;
	using std::underlying_type_t;
	using std::result_of_t;
FLUX_NAMESPACE_LEAVE1(compat)

#else // fallback

FLUX_NAMESPACE_ENTER1(compat)
	
	template< typename tType >
	using remove_cv_t = typename std::remove_cv<tType>::type;
	template< typename tType >
	using remove_const_t = typename std::remove_const<tType>::type;
	template< typename tType >
	using remove_volatile_t = typename std::remove_volatile<tType>::type;

	template< typename tType >
	using add_cv_t = typename std::add_cv<tType>::type;
	template< typename tType >
	using add_const_t = typename std::add_const<tType>::type;
	template< typename tType > 
	using add_volatile_t = typename std::add_volatile<tType>::type;

	template< typename tType >
	using remove_reference_t = typename std::remove_reference<tType>::type;
	template< typename tType >
	using add_lvalue_reference_t = typename std::add_lvalue_reference<tType>::type;
	template< typename tType >
	using add_rvalue_reference_t = typename std::add_rvalue_reference<tType>::type;

	template< typename tType >
	using add_pointer_t = typename std::add_pointer<tType>::type;
	template< typename tType >
	using remove_pointer_t = typename std::remove_pointer<tType>::type;

	template< typename tType >
	using decay_t = typename std::decay<tType>::type;
	template< typename... tTypes >
	using common_type_t = typename std::common_type<tTypes...>::type;
	template< typename tType >
	using underlying_type_t = typename std::underlying_type<tType>::type;
	template< typename tType >
	using result_of_t = typename std::result_of<tType>::value;

FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_TYPE_TRAITS_HPP_EB36C8E6_AF44_4399_8050_6334BA718AB9
