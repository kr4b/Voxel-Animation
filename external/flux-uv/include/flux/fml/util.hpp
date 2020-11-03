/*-******************************************************* -- HEADER -{{{1- */
/*- Utilities
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FML_UTIL_HPP_F4876A84_8D20_494A_AADB_CE5D026813D1
#define FLUX_FML_UTIL_HPP_F4876A84_8D20_494A_AADB_CE5D026813D1

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/enable_if.hpp>

#include <flux/compat/span.hpp>
#include <flux/compat/restrict.hpp>
#include <flux/compat/constexpr.hpp>

#include "quaternion.hpp"
#include "dual_quaternion.hpp"
#include "small_vector.hpp"
#include "transform.hpp"

//--    >>> namespace = flux::fml >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    functions - quaternions         ///{{{2///////////////////////////////

template< class Q, class V > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_quaternion<Q>::value && is_small_vector<V,3>::value && space_compatible<quat_src_space_t<Q>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,quat_dst_space_t<Q>>) apply_quat_rot( Q const&, V const& ) noexcept(V::staticSize);
template< class Q, class V > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_quaternion<Q>::value && is_small_vector<V,4>::value && space_compatible<quat_src_space_t<Q>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,quat_dst_space_t<Q>>) apply_quat_rot( Q const&, V const& ) noexcept(V::staticSize);

//--    functions - dual quaternions    ///{{{2///////////////////////////////

template< class Q, class V > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_dual_quaternion<Q>::value && is_small_vector<V,3>::value && space_compatible<dquat_src_space_t<Q>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,dquat_dst_space_t<Q>>) apply_dquat( Q const&, V const& ) noexcept(V::staticSize);
template< class Q, class V > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_dual_quaternion<Q>::value && is_small_vector<V,4>::value && space_compatible<dquat_src_space_t<Q>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,dquat_dst_space_t<Q>>) apply_dquat( Q const&, V const& ) noexcept(V::staticSize);

//--    functions - apply               ///{{{2///////////////////////////////

//TODO-docs
//TODO   + positions vs vectors
//TODO   + 3x4 matrix vs vector3? return vector4? [Return Vec4 here]

/* TODO
template< class T, class V > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_transform_3d<T>::value, is_small_vector<V,3>::value && space_compatible<tform_src_space_t<T>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,tform_dst_space_t<T>>) apply( T const&, V const& ) noexcept(V::staticSize);
template< class T, class V > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_transform_3d<T>::value, is_small_vector<V,4>::value && space_compatible<tform_src_space_t<T>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,tform_dst_space_t<T>>) apply( T const&, V const& ) noexcept(V::staticSize);
*/

/* TODO REWRITE to also take output buffers.
 *
 *  + output buffers?? 
 * 
template< class T, class V > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_transform_3d<T>::value, is_small_vector<V,3>::value && space_compatible<tform_src_space_t<T>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,tform_dst_space_t<T>>) apply( T const&, ftl::not_null<V const* FLUX_RESTRICT> aVs, std::size_t ) noexcept(V::staticSize);
template< class T, class V > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_transform_3d<T>::value, is_small_vector<V,4>::value && space_compatible<tform_src_space_t<T>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,tform_dst_space_t<T>>) apply( T const&, ftl::not_null<V const* FLUX_RESTRICT> aVs, std::size_t ) noexcept(V::staticSize);

template< class T, class V > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_transform_3d<T>::value, is_small_vector<V,3>::value && space_compatible<tform_src_space_t<T>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,tform_dst_space_t<T>>) apply( T const&, compat::span<V const> ) noexcept(V::staticSize);
template< class T, class V > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_transform_3d<T>::value, is_small_vector<V,4>::value && space_compatible<tform_src_space_t<T>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,tform_dst_space_t<T>>) apply( T const&, compat::span<V const> ) noexcept(V::staticSize);
*/

//--    <<< ~ flux::fml namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/util.inl"
#endif // FLUX_FML_UTIL_HPP_F4876A84_8D20_494A_AADB_CE5D026813D1
