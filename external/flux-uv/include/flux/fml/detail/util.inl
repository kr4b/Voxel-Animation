/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Utilities implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/compat/voidt.hpp>
#include <flux/compat/constexpr.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::fml >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    apply_quat_rot()                    ///{{{2///////////////////////////
template< class Q, class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_quaternion<Q>::value && is_small_vector<V,3>::value && space_compatible<quat_src_space_t<Q>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,quat_dst_space_t<Q>>) apply_quat_rot( Q const& aQ, V const& aV ) noexcept(V::staticSize)
{
	// See http://gamedev.stackexchange.com/a/50545
	using T_ = svec_type_t<V>;
	using V3_ = SmallVector<ftl::StaticSize<3>,T_>;
	auto const u = make_vector<V3_>( aQ[0], aQ[1], aQ[2] );
	auto const v = make_vector<V3_>( aV[0], aV[1], aV[2] );
	auto const s = aQ[3];

	auto const res =
		  T_(2) * dot(u,v) * u 
		+ (s*s - length_squared(u)) * v
		+ T_(2) * s * cross(u, v)
	;

	return make_vector<V>( res[0], res[1], res[2] );
}
template< class Q, class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_quaternion<Q>::value && is_small_vector<V,4>::value && space_compatible<quat_src_space_t<Q>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,quat_dst_space_t<Q>>) apply_quat_rot( Q const& aQ, V const& aV ) noexcept(V::staticSize)
{
	using V3_ = SmallVector<ftl::StaticSize<3>,svec_type_t<V>,svec_spec_t<V>>;
	using Ret_ = svec_rebind_space_t<V,quat_dst_space_t<Q>>;
	return expand<V>( apply_quat_rot( aQ, shrink<V3_>(aV) ), vector_cast<Ret_>(aV) );
}

//--    apply_dquat()                       ///{{{2///////////////////////////
template< class Q, class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_dual_quaternion<Q>::value && is_small_vector<V,3>::value && space_compatible<dquat_src_space_t<Q>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,dquat_dst_space_t<Q>>) apply_dquat( Q const& aQ, V const& aV ) noexcept(V::staticSize)
{
	auto const rpr = apply_quat_rot( aQ.real(), aV );
	auto const rd = aQ.real() * conj(aQ.dual());

	using V3_ = decltype(rpr);
	using T = typename V3_::value_type;
	return V3_( rpr[0] - T(2)*rd[0], rpr[1] - T(2)*rd[1], rpr[2] - T(2)*rd[2] );
}
template< class Q, class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_dual_quaternion<Q>::value && is_small_vector<V,4>::value && space_compatible<dquat_src_space_t<Q>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,dquat_dst_space_t<Q>>) apply_dquat( Q const& aQ, V const& aV ) noexcept(V::staticSize)
{
	using V3_ = SmallVector<ftl::StaticSize<3>,svec_type_t<V>,svec_spec_t<V>>;
	using Ret_ = svec_rebind_space_t<V,dquat_dst_space_t<Q>>;
	return expand<V>( apply_dquat( aQ, shrink<V3_>(aV) ), vector_cast<Ret_>(aV) );
}

//--    apply()                             ///{{{2///////////////////////////
namespace detail
{
	template< class T, class V, class = compat::void_t<> > class TformApply_;

	//TODO
	//TODO    VectorAspect vs PositionAspect
	//TODO
	//TODO  How to deal with 3x4 matrix + vector3? return vector4?
	//TODO
	//TODO
}

/*
template< class T, class V > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_transform_3d<T>::value, is_small_vector<V,3>::value && space_compatible<tform_src_space_t<T>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,tform_dst_space_t<T>>) apply( T const& aTfm, V const& aVec ) noexcept(V::staticSize)
{
}
template< class T, class V > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_transform_3d<T>::value, is_small_vector<V,4>::value && space_compatible<tform_src_space_t<T>,svec_space_t<V>>::value )
(svec_rebind_space_t<V,tform_dst_space_t<T>>) apply( T const& aTfm, V const& aVec ) noexcept(V::staticSize)
{
}
*/

//--    <<< ~ flux::fml namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
