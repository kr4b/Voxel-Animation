/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Transforms stuff
 */
/*-***************************************************************** -}}}1- */

#include <flux/compat/voidt.hpp>
#include <flux/compat/constexpr.hpp>
#include <flux/compat/type_traits.hpp>

#include <flux/ftl/dsvalue.hpp>
#include <flux/ftl/meta_bool.hpp>

#include "../common.hpp"
#include "../small_vector.hpp"

#include <cmath>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::fml >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
}

//--    meta functions                      ///{{{2///////////////////////////
namespace detail
{
	template< class, class = compat::void_t<> >
	struct SupportsScale3d
		: std::false_type
	{};
	template< class, class = compat::void_t<> >
	struct SupportsRot3d
		: std::false_type
	{};
	template< class, class = compat::void_t<> >
	struct SupportsTrn3d
		: std::false_type
	{};
	template< class, class = compat::void_t<> >
	struct SupportsProj3d
		: std::false_type
	{};

	
	// Register matrices
	template< typename T, class tSpec >
	struct SupportsScale3d< SmallMatrix<ftl::StaticSize<3>,ftl::StaticSize<3>,T,tSpec> >
		: std::true_type
	{};
	template< typename T, class tSpec >
	struct SupportsScale3d< SmallMatrix<ftl::StaticSize<3>,ftl::StaticSize<4>,T,tSpec> >
		: std::true_type
	{};
	template< typename T, class tSpec >
	struct SupportsScale3d< SmallMatrix<ftl::StaticSize<4>,ftl::StaticSize<4>,T,tSpec> >
		: std::true_type
	{};

	template< typename T, class tSpec >
	struct SupportsRot3d< SmallMatrix<ftl::StaticSize<3>,ftl::StaticSize<3>,T,tSpec> >
		: std::true_type
	{};
	template< typename T, class tSpec >
	struct SupportsRot3d< SmallMatrix<ftl::StaticSize<3>,ftl::StaticSize<4>,T,tSpec> >
		: std::true_type
	{};
	template< typename T, class tSpec >
	struct SupportsRot3d< SmallMatrix<ftl::StaticSize<4>,ftl::StaticSize<4>,T,tSpec> >
		: std::true_type
	{};

	template< typename T, class tSpec >
	struct SupportsTrn3d< SmallMatrix<ftl::StaticSize<3>,ftl::StaticSize<4>,T,tSpec> >
		: std::true_type
	{};
	template< typename T, class tSpec >
	struct SupportsTrn3d< SmallMatrix<ftl::StaticSize<4>,ftl::StaticSize<4>,T,tSpec> >
		: std::true_type
	{};

	template< typename T, class tSpec >
	struct SupportsProj3d< SmallMatrix<ftl::StaticSize<4>,ftl::StaticSize<4>,T,tSpec> >
		: std::true_type
	{};


	// Register vectors
	template< typename T, class tSpec >
	struct SupportsTrn3d< SmallVector<ftl::StaticSize<3>,T,tSpec> >
		: std::is_same< typename tSpec::Aspect, aspect::VectorAspect >
	{};
	template< typename T, class tSpec >
	struct SupportsTrn3d< SmallVector<ftl::StaticSize<4>,T,tSpec> >
		: std::is_same< typename tSpec::Aspect, aspect::VectorAspect >
	{};


	// Register quaterion
	template< typename T, class tSpec >
	struct SupportsRot3d< Quaternion<T,tSpec> >
		: std::true_type
	{};

	// Register dual quaterion
	template< typename T, class tSpec >
	struct SupportsRot3d< DualQuaternion<T,tSpec> >
		: std::true_type
	{};

	template< typename T, class tSpec >
	struct SupportsTrn3d< DualQuaternion<T,tSpec> >
		: std::true_type
	{};
}

template< class tTForm >
struct supports_scaling_3d
	: detail::SupportsScale3d<compat::decay_t<tTForm>>
{};
template< class tTForm >
struct supports_rotation_3d
	: detail::SupportsRot3d<compat::decay_t<tTForm>>
{};
template< class tTForm >
struct supports_translation_3d
	: detail::SupportsTrn3d<compat::decay_t<tTForm>>
{};
template< class tTForm >
struct supports_projection_3d
	: detail::SupportsProj3d<compat::decay_t<tTForm>>
{};


template< class tForm >
struct is_transform_3d
	: ftl::meta::Any<
		supports_scaling_3d<tForm>,
		supports_rotation_3d<tForm>,
		supports_translation_3d<tForm>,
		supports_projection_3d<tForm>
	>
{};

namespace detail
{
	// Spaces for matrices
	template< typename T, class tSpec >
	struct TformSrcSpace< SmallMatrix<ftl::StaticSize<3>,ftl::StaticSize<3>,T,tSpec> > {
		using type = typename tSpec::RowSpace;
	};
	template< typename T, class tSpec >
	struct TformSrcSpace< SmallMatrix<ftl::StaticSize<3>,ftl::StaticSize<4>,T,tSpec> > {
		using type = typename tSpec::RowSpace;
	};
	template< typename T, class tSpec >
	struct TformSrcSpace< SmallMatrix<ftl::StaticSize<4>,ftl::StaticSize<4>,T,tSpec> > {
		using type = typename tSpec::RowSpace;
	};

	template< typename T, class tSpec >
	struct TformDstSpace< SmallMatrix<ftl::StaticSize<3>,ftl::StaticSize<3>,T,tSpec> > {
		using type = typename tSpec::ColSpace;
	};
	template< typename T, class tSpec >
	struct TformDstSpace< SmallMatrix<ftl::StaticSize<3>,ftl::StaticSize<4>,T,tSpec> > {
		using type = typename tSpec::ColSpace;
	};
	template< typename T, class tSpec >
	struct TformDstSpace< SmallMatrix<ftl::StaticSize<4>,ftl::StaticSize<4>,T,tSpec> > {
		using type = typename tSpec::ColSpace;
	};


	// Spaces for vectors
	template< typename T, class tSpec >
	struct TformSrcSpace< SmallVector<ftl::StaticSize<3>,T,tSpec> > {
		using type = typename tSpec::Space;
	};
	template< typename T, class tSpec >
	struct TformDstSpace< SmallVector<ftl::StaticSize<3>,T,tSpec> > {
		using type = typename tSpec::Space;
	};
	
	// Register quaterion
	template< typename T, class tSpec >
	struct TformSrcSpace< Quaternion<T,tSpec> > {
		using type = typename tSpec::SrcSpace;
	};
	template< typename T, class tSpec >
	struct TformDstSpace< Quaternion<T,tSpec> > {
		using type = typename tSpec::DstSpace;
	};
}

//--    affine transforms                   ///{{{2///////////////////////////

	// scalings -{{{3-
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_scaling_3d<M>::value )
(M) make_scaling_3d( typename M::value_type aX, Identity<M> ) noexcept(M::staticSize)
{
	return make_scaling_3d( aX, aX, aX, Identity<M>{} );
}
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_scaling_3d<M>::value )
(M) make_scaling_3d( typename M::value_type aX, typename M::value_type aY, typename M::value_type aZ, Identity<M> ) noexcept(M::staticSize)
{
	M ret( Init::identity );
	ret(0,0) = aX;
	ret(1,1) = aY;
	ret(2,2) = aZ;
	return ret;
}
template< class M, class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && is_small_vector<V,3>::value && supports_scaling_3d<M>::value )
(M) make_scaling_3d( V const& aV, Identity<M> ) noexcept(M::staticSize)
{
	return make_scaling_3d( aV[0], aV[1], aV[2], Identity<M>{} );
}

	// rotations -{{{3-
template< class M > inline
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_rotation_3d<M>::value )
(M) make_rotation_3d( Angle< typename M::value_type, aspect::Radians > aRadians, typename M::value_type aX, typename M::value_type aY, typename M::value_type aZ, Identity<M> ) noexcept(M::staticSize)
{
	using Ty3_ = SmallVector<ftl::StaticSize<3>, typename M::value_type>;
	return make_rotation_3d( aRadians, make_vector<Ty3_>(aX,aY,aZ), Identity<M>{} );
}
template< class M, class V > inline
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_rotation_3d<M>::value && is_small_vector<V,3>::value )
(M) make_rotation_3d( Angle< typename M::value_type, aspect::Radians > aRadians, V const& aAxis, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;

	auto const v = normalize( aAxis );
	Ty_ sinA = std::sin(aRadians.value());
	Ty_ cosA = std::cos(aRadians.value());
	Ty_ cosI = Ty_(1) - cosA;

	M res( Init::identity );
	res(0,0) = (cosI * v.x * v.x) + (cosA);
	res(0,1) = (cosI * v.x * v.y) - (sinA * v.z );
	res(0,2) = (cosI * v.x * v.z) + (sinA * v.y );
			
	res(1,0) = (cosI * v.y * v.x) + (sinA * v.z);
	res(1,1) = (cosI * v.y * v.y) + (cosA);
	res(1,2) = (cosI * v.y * v.z) - (sinA * v.x);
			
	res(2,0) = (cosI * v.z * v.x) - (sinA * v.y);
	res(2,1) = (cosI * v.z * v.y) + (sinA * v.x);
	res(2,2) = (cosI * v.z * v.z) + (cosA);

	return res;
}

template< class M, class Q > inline
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_rotation_3d<M>::value && is_quaternion<Q>::value )
(M) make_rotation_3d( Q const& aQ, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;

	Ty_ const s = Ty_(2) / Ty_(norm_2(aQ));

	M res( Init::identity );
	res(0,0) = Ty_(1) - s*(aQ[1]*aQ[1] + aQ[2]*aQ[2]);
	res(1,0) = s*(aQ[0]*aQ[1] + aQ[3]*aQ[2]);
	res(2,0) = s*(aQ[0]*aQ[2] - aQ[3]*aQ[1]);

	res(0,1) = s*(aQ[0]*aQ[1] - aQ[3]*aQ[2]);
	res(1,1) = Ty_(1) - s*(aQ[0]*aQ[0] + aQ[2]*aQ[2]);
	res(2,1) = s*(aQ[1]*aQ[2] + aQ[3]*aQ[0]);

	res(0,2) = s*(aQ[0]*aQ[2] + aQ[3]*aQ[1]);
	res(1,2) = s*(aQ[1]*aQ[2] - aQ[3]*aQ[0]); 
	res(2,2) = Ty_(1) - s*(aQ[0]*aQ[0] + aQ[1]*aQ[1]);
	return res;
}

template< class M, class DQ > inline
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_rotation_3d<M>::value && is_dual_quaternion<DQ>::value )
(M) make_rotation_3d( DQ const& aQ, Identity<M> ) noexcept(M::staticSize)
{
	return make_rotation_3d( aQ.real(), Identity<M>{} );
}


	// translations -{{{3-
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_translation_3d<M>::value )
(M) make_translation_3d( typename M::value_type aX, typename M::value_type aY, typename M::value_type aZ, Identity<M>) noexcept(M::staticSize)
{
	M res( Init::identity );
	res(0,3) = aX;
	res(1,3) = aY;
	res(2,3) = aZ;
	return res;
}
template< class M, class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_translation_3d<M>::value && is_small_vector<V,3>::value )
(M) make_translation_3d( V const& aV, Identity<M> ) noexcept(M::staticSize)
{
	return make_translation_3d( aV[0], aV[1], aV[2], Identity<M>{} );
}

template< class M, class DQ > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_translation_3d<M>::value && is_dual_quaternion<DQ>::value )
(M) make_translation_3d( DQ const& aQ, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename DQ::value_type;
	auto const res = Ty_(2) * aQ.dual() * conj(aQ.real());
	return make_translation_3d( res.i, res.j, res.k, Identity<M>{} );
}

	// transforms -{{{3-
//TODO

//--    projections                         ///{{{2///////////////////////////
	
	// ortho -{{{3-
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_ortho_3d( typename M::value_type aLeft, typename M::value_type aRight,typename M::value_type aBottom, typename M::value_type aTop, Identity<M> ) noexcept(M::staticSize)
{
	return make_projection_ortho_gl_3d<M>( aLeft, aRight, aBottom, aTop );
}
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_ortho_3d( typename M::value_type aLeft, typename M::value_type aRight, typename M::value_type aBottom, typename M::value_type aTop, typename M::value_type aNear, typename M::value_type aFar, Identity<M> ) noexcept(M::staticSize)
{
	return make_projection_ortho_gl_3d<M>( aLeft, aRight, aBottom, aTop, aNear, aFar );
}

template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_ortho_gl_3d( typename M::value_type aLeft, typename M::value_type aRight,typename M::value_type aBottom, typename M::value_type aTop, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;
	return make_projection_ortho_gl_3d( aLeft, aRight, aBottom, aTop, Ty_(-1), Ty_(1), Identity<M>{} );
}
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_ortho_gl_3d( typename M::value_type aLeft, typename M::value_type aRight, typename M::value_type aBottom, typename M::value_type aTop, typename M::value_type aNear, typename M::value_type aFar, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;

	M res( Init::identity );
	res(0,0) = Ty_(2/(Ty_(aRight)-aLeft));
	res(1,1) = Ty_(2/(Ty_(aTop)-aBottom));
	res(2,2) = Ty_(2/(Ty_(aFar)-aNear));

	res(0,3) = -Ty_((aRight+aLeft)/(Ty_(aRight)-aLeft));
	res(1,3) = -Ty_((aTop+aBottom)/(Ty_(aTop)-aBottom));
	res(2,3) = -Ty_((aFar+aNear)/(Ty_(aFar)-aNear));
	return res;
}

template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_ortho_vk_3d( typename M::value_type aLeft, typename M::value_type aRight,typename M::value_type aBottom, typename M::value_type aTop, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;
	return make_projection_ortho_vk_3d( aLeft, aRight, aBottom, aTop, Ty_(-1), Ty_(1), Identity<M>{} );
}
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_ortho_vk_3d( typename M::value_type aLeft, typename M::value_type aRight, typename M::value_type aBottom, typename M::value_type aTop, typename M::value_type aNear, typename M::value_type aFar, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;

	Ty_ const w = aRight-aLeft;
	Ty_ const h = aTop-aBottom;
	Ty_ const d = aFar-aNear;

	M res( Init::identity );
	res(0,0) = 2/w;
	res(1,1) = -2/h;
	res(2,2) = 1/d;

	res(0,3) = -(aRight+aLeft)/w;
	res(1,3) = (aTop+aBottom)/h;
	res(2,3) = -aNear/d;
	return res;
}

	// perspective -{{{3-
template< class M > inline
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_3d( Angle< typename M::value_type, aspect::Radians > aRadians, typename M::value_type aAspect, typename M::value_type aNear, typename M::value_type aFar, Identity<M> ) noexcept(M::staticSize)
{
	return make_projection_perspective_gl_3d<M>( aRadians, aAspect, aNear, aFar );
}
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_3d( typename M::value_type aLeft, typename M::value_type aRight, typename M::value_type aBottom, typename M::value_type aTop, typename M::value_type aNear, typename M::value_type aFar, Identity<M> ) noexcept(M::staticSize)
{
	return make_projection_perspective_gl_3d<M>( aLeft, aRight, aBottom, aTop, aNear, aFar );
}

template< class M > inline
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_gl_3d( Angle< typename M::value_type, aspect::Radians > aRadians, typename M::value_type aAspect, typename M::value_type aNear, typename M::value_type aFar, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;

	Ty_ const f = Ty_(1)/std::tan(aRadians.value()/Ty_(2));
	Ty_ const d = aFar-aNear;

	M res( Init::zero );
	res(0,0) = f/aAspect;
	res(1,1) = f;
	res(2,2) = -(aFar+aNear) / d;
	res(3,2) = -Ty_(1);
	res(2,3) = -Ty_(2)*aFar*aNear / d;
	return res;
}
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_gl_3d( typename M::value_type aLeft, typename M::value_type aRight, typename M::value_type aBottom, typename M::value_type aTop, typename M::value_type aNear, typename M::value_type aFar, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;

	M res( Init::zero );
	res(0,0) = Ty_(2*aNear/(Ty_(aRight)-aLeft));
	res(1,1) = Ty_(2*aNear/(Ty_(aTop)-aBottom));
	
	res(0,2) = Ty_((aRight+aLeft)/(Ty_(aRight)-aLeft));
	res(1,2) = Ty_((aTop+aBottom)/(Ty_(aTop)-aBottom));
	res(2,2) = -Ty_((aFar+aNear)/(Ty_(aFar)-aNear));
	res(3,2) = Ty_(-1);
	
	res(2,3) = -Ty_(2*aFar*aNear/(Ty_(aFar)-aNear));
	return res;
}

template< class M > inline
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_vk_3d( Angle< typename M::value_type, aspect::Radians > aRadians, typename M::value_type aAspect, typename M::value_type aNear, typename M::value_type aFar, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;

	Ty_ const f = Ty_(1)/std::tan(aRadians.value()/Ty_(2));
	Ty_ const d = aFar-aNear;

	M res( Init::zero );
	res(0,0) = f/aAspect;
	res(1,1) = -f;
	res(2,2) = -aFar/d;
	res(3,2) = -Ty_(1);
	res(2,3) = -aFar*aNear / d;
	return res;
}
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_vk_3d( typename M::value_type aLeft, typename M::value_type aRight, typename M::value_type aBottom, typename M::value_type aTop, typename M::value_type aNear, typename M::value_type aFar, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;

	Ty_ const nn = 2*aNear;
	Ty_ const w = aRight-aLeft;
	Ty_ const h = aTop-aBottom;
	Ty_ const d = aFar-aNear;

	M res( Init::zero );
	res(0,0) = nn/w;
	res(1,1) = -nn/h;
	
	res(0,2) = (aRight+aLeft)/w;
	res(1,2) = -(aTop+aBottom)/h;
	res(2,2) = -aFar/d;
	res(3,2) = -Ty_(1);
	
	res(2,3) = -aFar*aNear / d;
	return res;
}

template< class M > inline
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_reversez_gl_3d( Angle< typename M::value_type, aspect::Radians > aRadians, typename M::value_type aAspect, typename M::value_type aNear, typename M::value_type aFar, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;

	Ty_ const f = Ty_(1)/std::tan(aRadians.value()/Ty_(2));
	Ty_ const d = aFar-aNear;

	M res( Init::zero );
	res(0,0) = f/aAspect;
	res(1,1) = f;
	res(2,2) = (aFar+aNear) / d - Ty_(1);
	res(3,2) = -Ty_(1);
	res(2,3) = Ty_(2)*aFar*aNear / d;
	return res;
}

template< class M > inline
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_reversez_vk_3d( Angle< typename M::value_type, aspect::Radians > aRadians, typename M::value_type aAspect, typename M::value_type aNear, typename M::value_type aFar, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;

	Ty_ const f = Ty_(1)/std::tan(aRadians.value()/Ty_(2));
	Ty_ const d = aFar-aNear;

	M res( Init::zero );
	res(0,0) = f/aAspect;
	res(1,1) = -f;
	res(2,2) = aFar/d - Ty_(1);
	res(3,2) = -Ty_(1);
	res(2,3) = aFar*aNear / d;
	return res;
}
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_reversez_vk_3d( typename M::value_type aLeft, typename M::value_type aRight, typename M::value_type aBottom, typename M::value_type aTop, typename M::value_type aNear, typename M::value_type aFar, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;

	Ty_ const nn = 2*aNear;
	Ty_ const w = aRight-aLeft;
	Ty_ const h = aTop-aBottom;
	Ty_ const d = aFar-aNear;

	M res( Init::zero );
	res(0,0) = nn/w;
	res(1,1) = -nn/h;
	res(0,2) = (aRight+aLeft)/w;
	res(1,2) = -(aTop+aBottom)/h;

	res(2,2) = aFar/d - Ty_(1);
	res(3,2) = -Ty_(1);
	res(2,3) = aFar*aNear / d;
	return res;
}

template< class M > inline
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_reversez_vk_3d( Angle< typename M::value_type, aspect::Radians > aRadians, typename M::value_type aAspect, typename M::value_type aNear, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;

	Ty_ const f = Ty_(1)/std::tan(aRadians.value()/Ty_(2));

	M res( Init::zero );
	res(0,0) = f/aAspect;
	res(1,1) = -f;
	res(3,2) = -Ty_(1);
	res(2,3) = aNear;
	return res;
}
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_reversez_vk_3d( typename M::value_type aLeft, typename M::value_type aRight, typename M::value_type aBottom, typename M::value_type aTop, typename M::value_type aNear, Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;

	Ty_ const nn = 2*aNear;
	Ty_ const w = aRight-aLeft;
	Ty_ const h = aTop-aBottom;

	M res( Init::zero );
	res(0,0) = nn/w;
	res(1,1) = -nn/h;
	res(0,2) = (aRight+aLeft)/w;
	res(1,2) = -(aTop+aBottom)/h;

	res(3,2) = -Ty_(1);
	res(2,3) = aNear;
	return res;
}

	// helpers
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_vulkan_projection_correction_3d( Identity<M> ) noexcept(M::staticSize)
{
	using Ty_ = typename M::value_type;

	M res( Init::zero );
	res(0,0) = Ty_(1);
	res(1,1) = -Ty_(1);
	res(2,2) = Ty_(1)/Ty_(2);
	res(2,3) = Ty_(1)/Ty_(2);
	res(3,3) = Ty_(1);
	return res;
}

//--    with quaternions                    ///{{{2///////////////////////////

template< class Q > inline
FLUX_ENABLE_IF_C( is_quaternion<Q>::value )
(Q) make_rotation_3d( Angle< typename Q::value_type, aspect::Radians > aRadians, typename Q::value_type aX, typename Q::value_type aY, typename Q::value_type aZ, Identity<Q> ) noexcept
{
	using Ty3_ = SmallVector<ftl::StaticSize<3>, typename Q::value_type>;
	return make_rotation_3d( aRadians, Ty3_( aX, aY, aZ ), Identity<Q>{} );
}
template< class Q, class V > inline
FLUX_ENABLE_IF_C( is_quaternion<Q>::value && is_small_vector<V>::value )
(Q) make_rotation_3d( Angle< typename Q::value_type, aspect::Radians > aRadians, V const& aAxis, Identity<Q> ) noexcept
{
	typedef typename Q::value_type Ty_;

	auto const halfAngle = aRadians.value() / Ty_(2);
	auto const nn = std::sin(halfAngle) * normalize(aAxis);
	return Q( nn[0], nn[1], nn[2], std::cos(halfAngle) );
}

template< class Q, class M > inline
FLUX_ENABLE_IF_C( is_quaternion<Q>::value && is_small_matrix<M>::value && supports_rotation_3d<M>::value )
(Q) make_rotation_3d( M const& aM, Identity<Q> ) noexcept
{
	/* adopted from the old <linmath> lib from Chalmers; there, the method
	 * is attributed to Ken Shoemake and his SIGGRAPH'87 course notes on 
	 * "Quaternion Calculus and Fast Animation".
	 */
	using Ty_ = typename Q::value_type;

	// Don't use trace(); matrix might not be 3x3:
	Ty_ const tr = aM(0,0)+aM(1,1)+aM(2,2); 
	
	if( tr > Ty_(0) )
	{
		Ty_ const root = std::sqrt( tr+Ty_(1) );
		Ty_ const rroot = Ty_(0.5) / root;

		return Q(
			rroot*(aM(2,1)-aM(1,2)),
			rroot*(aM(0,2)-aM(2,0)),
			rroot*(aM(1,0)-aM(0,1)),
			root / Ty_(2)
		);
	}

	int i = 0;
	if( aM(1,1) > aM(0,0) ) i = 1;
	if( aM(2,2) > aM(i,i) ) i = 2;

	static constexpr int next[] = { 1, 2, 0 };
	int const j = next[i];
	int const k = next[j];

	Ty_ const root = std::sqrt(aM(i,i)-aM(j,j)-aM(k,k)+Ty_(1));
	Ty_ const rroot = Ty_(0.5) / root;
	
	Q res;
	res[i] = root / Ty_(2);
	res[j] = rroot * (aM(j,i)+aM(i,j));
	res[k] = rroot * (aM(k,i)+aM(i,k));
	res[3] = rroot * (aM(k,j)-aM(j,k));
	return res;
}

//--    with dual quaternions               ///{{{2///////////////////////////
template< class DQ > inline
FLUX_ENABLE_IF_C( is_dual_quaternion<DQ>::value )
(DQ) make_rotation_3d( Angle< typename DQ::value_type, aspect::Radians > aRadians, typename DQ::value_type aX, typename DQ::value_type aY, typename DQ::value_type aZ, Identity<DQ> ) noexcept
{
	using Dual_ = typename DQ::Dual;
	return DQ( 
		make_rotation_3d<typename DQ::Real>( aRadians, aX, aY, aZ ),
		Dual_( Init::zero )
	);
}
template< class DQ, class V > inline
FLUX_ENABLE_IF_C( is_dual_quaternion<DQ>::value && is_small_vector<V>::value )
(DQ) make_rotation_3d( Angle< typename DQ::value_type, aspect::Radians > aRadians, V const& aAxis, Identity<DQ> ) noexcept
{
	using Dual_ = typename DQ::Dual;
	return DQ( 
		make_rotation_3d<typename DQ::Real>( aRadians, aAxis ),
		Dual_( Init::zero )
	);
}

template< class DQ, class M > inline
FLUX_ENABLE_IF_C( is_dual_quaternion<DQ>::value && is_small_matrix<M>::value && supports_rotation_3d<M>::value )
(DQ) make_rotation_3d( M const& aM, Identity<DQ> ) noexcept
{
	using Dual_ = typename DQ::Dual;
	return DQ( 
		make_rotation_3d<typename DQ::Real>( aM ),
		Dual_( Init::zero )
	);
}

template< class DQ > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_dual_quaternion<DQ>::value )
(DQ) make_translation_3d( typename DQ::value_type aX, typename DQ::value_type aY, typename DQ::value_type aZ, Identity<DQ>) noexcept
{
	using Real_ = typename DQ::Real;
	using Dual_ = typename DQ::Dual;
	using Ty_ = typename Dual_::value_type;
	return DQ( 
		Real_( Init::identity ),
		Dual_( .5f * aX, .5f * aY, .5f * aZ, Ty_(0) )
	);
}
template< class DQ, class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_dual_quaternion<DQ>::value && is_small_vector<V,3>::value )
(DQ) make_translation_3d( V const& aV, Identity<DQ> ) noexcept
{
	using Real_ = typename DQ::Real;
	using Dual_ = typename DQ::Dual;
	using Ty_ = typename Dual_::value_type;
	return DQ( 
		Real_( Init::identity ),
		Dual_( .5f * aV[0], .5f * aV[1], .5f * aV[2], Ty_(0) )
	);
}

//--    with vectors                        ///{{{2///////////////////////////

template< class V, class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_vector<V>::value && supports_translation_3d<V>::value && is_small_matrix<M>::value && supports_translation_3d<M>::value )
(V) make_translation_3d( M const& aM, Identity<V> ) noexcept(V::staticSize)
{
	return make_vector<V>( aM(0,3), aM(1,3), aM(2,3) );
}
template< class V, class Q > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_vector<V>::value && supports_translation_3d<V>::value && is_dual_quaternion<Q>::value )
(V) make_translation_3d( Q const& aQ, Identity<V> ) noexcept(V::staticSize)
{
	using Ty_ = typename Q::value_type;
	auto const res = Ty_(2) * aQ.dual() * conj(aQ.real());
	return make_vector<V>( res.i, res.j, res.k );
}

//--    <<< ~ flux::fml namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
