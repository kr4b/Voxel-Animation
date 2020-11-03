/*-******************************************************* -- HEADER -{{{1- */
/*- Dual Quaternion
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FML_DUAL_QUATERNION_HPP_AD57781B_5C9A_4B6F_B0F7_850469FF60B5
#define FLUX_FML_DUAL_QUATERNION_HPP_AD57781B_5C9A_4B6F_B0F7_850469FF60B5

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/identity.hpp>
#include <flux/std/enable_if.hpp>

#include <flux/compat/voidt.hpp>
#include <flux/compat/constexpr.hpp>

#include <flux/ftl/explicit_init_list.hpp>

#include "common.hpp"
#include "quaternion.hpp"

//--    >>> namespace = flux::fml >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    aspect                          ///{{{2///////////////////////////////
namespace aspect
{
	template< class tDstSpace = space::AnySpace, class tSrcSpace = space::AnySpace >
	struct DQuatSpec
	{
		using SrcSpace = tSrcSpace;
		using DstSpace = tDstSpace;
	};
}

//--    detail                          ///{{{2///////////////////////////////
namespace detail
{
	template< class, class, class = compat::void_t<> > struct DQuatSpecCompat;
}

//--    DualQuaternion                  ///{{{2///////////////////////////////

/** \brief Dual Quaternion
 *
 * Dual quaternion \f$\mbox{dq} = q + \epsilon\,p\f$, where the dual element
 * \f$\epsilon\f$ has the property \f$\epsilon^2 = 0\f$. Dual quaternions are
 * stored as a linear array of eight values of type \a T in the order
 * \f$\left[q_i, q_j, q_k, q_r, p_i, p_j, p_k, p_r\right]\f$, i.e., as as the
 * quaternion \f$q\f$ followed by the quaternion \f$p\f$.
 *
 * Dual quaternions can represent both rotations and translations in 3D space.
 * The rotation is encoded in the real part \f$q\f$ as an ordinary quaternion.
 * The translation is stored in the dual part \f$p\f$ where \f$\left(t_x,t_y,
 * t_z\right) = 2\,p\,q^*\f$.
 *
 * Dual quaternion rotations and translations can be constructed by using the
 * `make_rotation_3d()` and `make_translation_3d()` functions:
   \code
   using DQuat = DualQuaternion<>;
   DQuat rot = make_rotation_3d<DQuat>( 90.0_degf, 0.f, 0.f, 1.f );
   DQuat trn = make_translation_3d<DQuat>( 11.f, -13.f, 17.f );
   auto const rt = rot * trn;
   \endcode
 * Here, `rt` is a dual quaternion that represents first a translation and
 * then a rotation, i.e., the order of operations is identical to standard
 * matrices.
 *
 * The translation and rotation can be extracted using the same functions:
   \code
   DQuat rt = ...; // as above
   vec3f vt = make_translation_3d<vec3f>( rt );
   mat33f mr = make_rotation_3d<mat33f>( rt );
   \endcode
 *
 * Similar to other classes representing transformations, dual quaternions
 * store a source and destination space.
 * 
 * \see make_translation_3d, make_rotation_3d, space::AnySpace
 */
template< typename T = float, class tSA = aspect::DQuatSpec<> >
struct DualQuaternion
{
	using value_type = T;
	using Real = Quaternion<T,aspect::QuatSpec<typename tSA::DstSpace,typename tSA::SrcSpace>>;
	using Dual = Quaternion<T,aspect::QuatSpec<typename tSA::DstSpace,typename tSA::SrcSpace>>;

	union
	{
		struct { Real q; Dual eq; };
		struct { value_type i, j, k, r, ei, ej, ek, er; }; 
		value_type elems[8];
	};

	DualQuaternion() = default;
	DualQuaternion( DualQuaternion const& ) = default;
	DualQuaternion& operator=( DualQuaternion const& ) = default;

	FLUX_CONSTEXPR_EX explicit DualQuaternion( Init::Zero ) noexcept;
	FLUX_CONSTEXPR_EX explicit DualQuaternion( Init::Identity ) noexcept;

	FLUX_CONSTEXPR_EX DualQuaternion( Real const&, Dual const& ) noexcept;
	FLUX_CONSTEXPR_EX DualQuaternion( T, T, T, T, T, T, T, T ) noexcept;

	DualQuaternion( ftl::ExplicitInitList<T> ) noexcept;

	template< typename Y, class tSB, 
		class = FLUX_ENABLE_IF(detail::DQuatSpecCompat<tSA,tSB>)(int) 
	> FLUX_CONSTEXPR_EX
	DualQuaternion( DualQuaternion<Y,tSB> const& ) noexcept;
	template< typename Y, class tSB, 
		class = FLUX_ENABLE_IF(detail::DQuatSpecCompat<tSA,tSB>)(int) 
	> FLUX_CONSTEXPR_EX
	DualQuaternion& operator=( DualQuaternion<Y,tSB> const& ) noexcept;

	FLUX_CONSTEXPR_EX T& operator[] (std::size_t) noexcept;
	constexpr T const& operator[] (std::size_t) const noexcept;

	FLUX_CONSTEXPR_EX Real& real() noexcept;
	FLUX_CONSTEXPR_EX Real const& real() const noexcept;
	FLUX_CONSTEXPR_EX Dual& dual() noexcept;
	FLUX_CONSTEXPR_EX Dual const& dual() const noexcept;

	FLUX_CONSTEXPR_EX T* data() noexcept;
	constexpr T const* data() const noexcept;
};

//--    meta functions                  ///{{{2///////////////////////////////

/** \anchor DQuatTraits \name Meta: Dual quaternion traits
 * \brief Dual quaternion traits and meta-functions
 *
 * Meta functions to identify and manipulate the dual quaternion type.
 */
//@{
/** \brief Meta: dual quaternion trait
 * See \ref DQuatTraits
 */
template< typename > struct is_dual_quaternion : std::false_type {};

template< typename T, class tSA > 
struct is_dual_quaternion< DualQuaternion<T,tSA> > 
	: std::true_type
{};

namespace detail
{
	template< typename > struct DQuatSrcSpace;
	template< typename > struct DQuatDstSpace;

	template< typename, class > struct DQuatRebindSrc;
	template< typename, class > struct DQuatRebindDst;
	template< typename, class, class > struct DQuatRebind;

	template< class > struct DQuatInvSpec;
	template< class, class > struct DQuatSpec;
	template< class, class > struct DQuatMulSpec;

	template< class tSA > 
	using dquat_inv_spec_t = typename DQuatInvSpec<tSA>::type;
	template< class tSA, class tSB > 
	using dquat_mul_spec_t = typename DQuatMulSpec<tSA,tSB>::type;
}

template< typename Q > using dquat_type_t = typename Q::value_type;
template< typename Q > using dquat_src_space_t = typename detail::DQuatSrcSpace<Q>::type;
template< typename Q > using dquat_dst_space_t = typename detail::DQuatDstSpace<Q>::type;

template< typename Q, class tSpace > 
using dquat_rebind_src_space_t = typename detail::DQuatRebindSrc<Q,tSpace>::type;
template< typename Q, class tSpace > 
using dquat_rebind_dst_space_t = typename detail::DQuatRebindDst<Q,tSpace>::type;
template< typename Q, class tDstSpace, class tSrcSpace >
using dquat_rebind_spaces_t = typename detail::DQuatRebind<Q,tDstSpace,tSrcSpace>::type;
//@} 

//--    operators                       ///{{{2///////////////////////////////

/** \anchor DQuatOperators \name Dual quaternion operators
 * \brief Dual quaternion operators
 *
 * Dual quaternion operations in perator form. With the exception of the
 * dual-quaternion by dual-quaternion multiplicatio, these operations are
 * element-wise.
 *
 * For element-wise operations the spaces of the dual quaternions must match
 * (or be `space::AnySpace`). For multiplications, the spaces are propagated:
 * \code
   using MWDQuat = DualQuaternion< ..., aspect::DQuatSpec<WorldSpace,ModelSpace> >;
   using WCDQuat = DualQuaternion< ..., aspect::DQuatSpec<CameraSpace,WorldSpace> >;
  
   MWDQuat mw = ...;
   WCDQuat wc = ...;
  
   auto const mc = wc * mw;
   \endcode
 * The dual quaternion `mc` will have be
 * `aspect::DQuatSpec<CameraSpace,ModelSpace>`, i.e. a dual quaternion that
 * transforms from `ModelSpace` to `CameraSpace`.
 */
//@{
/** \brief Dual quaternion operator
 * See \ref DQuatOperators
 */
template< typename T, class tSA > constexpr
DualQuaternion<T,tSA> operator- (DualQuaternion<T,tSA> const&) noexcept;
template< typename T, class tSA > constexpr
DualQuaternion<T,tSA> operator+ (DualQuaternion<T,tSA> const&) noexcept;

template< typename T, class tSA, class tSB, class = typename detail::DQuatSpec<tSA,tSB>::type > constexpr
bool operator== (DualQuaternion<T,tSA> const&, DualQuaternion<T,tSB> const& ) noexcept;
template< typename T, class tSA, class tSB, class = typename detail::DQuatSpec<tSA,tSB>::type > constexpr
bool operator!= (DualQuaternion<T,tSA> const&, DualQuaternion<T,tSB> const& ) noexcept;


template< typename T, class tSA, class tSB, class tNA = typename detail::DQuatSpec<tSA,tSB>::type > constexpr
DualQuaternion<T,tNA> operator+ (DualQuaternion<T,tSA> const&, DualQuaternion<T,tSB> const& ) noexcept;
template< typename T, class tSA , class tSB, class tNA = typename detail::DQuatSpec<tSA,tSB>::type > constexpr
DualQuaternion<T,tNA> operator- (DualQuaternion<T,tSA> const&, DualQuaternion<T,tSB> const& ) noexcept;
template< typename T, class tSA, class tSB, class tNS = detail::dquat_mul_spec_t<tSA,tSB> > constexpr
DualQuaternion<T,tNS> operator* (DualQuaternion<T,tSA> const&, DualQuaternion<T,tSB> const& ) noexcept;

template< typename T, class tSA > constexpr
DualQuaternion<T,tSA> operator* (DualQuaternion<T,tSA> const&, T ) noexcept;
template< typename T, class tSA > constexpr
DualQuaternion<T,tSA> operator/ (DualQuaternion<T,tSA> const&, T ) noexcept;
template< typename T, class tSA > constexpr
DualQuaternion<T,tSA> operator* (T, DualQuaternion<T,tSA> const& ) noexcept;


template< typename T, class tSA, class tSB, class = typename detail::DQuatSpec<tSA,tSB>::type > FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>& operator+= (DualQuaternion<T,tSA>&, DualQuaternion<T,tSB> const&) noexcept;
template< typename T, class tSA, class tSB, class = typename detail::DQuatSpec<tSA,tSB>::type > FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>& operator-= (DualQuaternion<T,tSA>&, DualQuaternion<T,tSB> const&) noexcept;
template< typename T, class tSA, class tSB, class = detail::dquat_mul_spec_t<tSA,tSB> /* TODO: is_same<> etc */ > FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>& operator*= (DualQuaternion<T,tSA>&, DualQuaternion<T,tSB> const&) noexcept;

template< typename T, class tSA > FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>& operator*= (DualQuaternion<T,tSA>&, T) noexcept;
template< typename T, class tSA > FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>& operator/= (DualQuaternion<T,tSA>&, T) noexcept;
//@}

//--    functions                       ///{{{2///////////////////////////////

	// pseudo constructors
/** \anchor DQuatPseudoConstructors \name Dual quaternion pseudo-constructors
 * \brief Dual quaternion pseudo-constructors
 *
 * Create `DualQuaternion` of type \a Q as follows:
 *
 *  - `make_dquat`: from the provided eight values
 *  - `make_zero`: `DualQuaternion` with all elements equal to zero
 *  - `make_one`: `QDualuaternion` with all elements equal to one
 *  - `make_identity`: Identity `DualQuaternion` \f$=\left(0,0,0,1\right) +
 *     \epsilon\,\left(0,0,0,0\right)\f$.
 *
 * Examples:
   \code
   auto const v = make_quat<dquatf>( 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f );
   auto const z = make_zero<dquatf>(); // alt: make_zero( flux::Identity<dquatf>{} );
   auto const I = make_identity<dquat>(); // idenentity quaternion
   \endcode
 *
 * Dual quaternionss representing rotations or translations can be created
 * using the `#make_rotation_3d` and `#make_translation_3d` methods.
 *
 * \see make_rotation_3d, make_translation_3d
 */
//@{
/** \brief Dual quaternion pseudo-constructor
 * See \ref DQuatPseudoConstructors
 */
template< typename DQ > constexpr
FLUX_ENABLE_IF( is_dual_quaternion<DQ> )
(DQ) make_dquat( 
	dquat_type_t<DQ>, dquat_type_t<DQ>, dquat_type_t<DQ>, dquat_type_t<DQ>, 
	dquat_type_t<DQ>, dquat_type_t<DQ>, dquat_type_t<DQ>, dquat_type_t<DQ> 
) noexcept;


template< typename DQ >
FLUX_ENABLE_IF( is_dual_quaternion<DQ> )
(DQ) make_dquat( dquat_type_t<DQ> const* ) noexcept;

template< typename DQ > constexpr
FLUX_ENABLE_IF( is_dual_quaternion<DQ> )
(DQ) make_zero( Identity<DQ> = {} ) noexcept;
template< typename DQ > constexpr
FLUX_ENABLE_IF( is_dual_quaternion<DQ> )
(DQ) make_identity( Identity<DQ> = {} ) noexcept;

	// conversion
template< class DQ, class R > constexpr
FLUX_ENABLE_IF_C( is_dual_quaternion<DQ>::value && is_dual_quaternion<R>::value )
(DQ) dual_quaternion_cast( R const& ) noexcept;

template< typename T, class tSA > FLUX_CONSTEXPR_EX
T* data_ptr( DualQuaternion<T,tSA>& ) noexcept;
template< typename T, class tSA > constexpr
T const* data_ptr( DualQuaternion<T,tSA> const& ) noexcept;
//@}

	// math. functions
/** \anchor DQuatMathFun \name Dual quaternion math functions
 * \brief Dual quaternion mathematical functions
 *
 * Common mathematical functions, such as conjuage, inverse and norms for
 * dual quaternions.
 */
//@{
/** \brief Dual quaternion math functions
 * See \ref DQuatMathFun
 */
template< typename T, class tSA, class tNA = detail::dquat_inv_spec_t<tSA> > constexpr
DualQuaternion<T,tNA> conj( DualQuaternion<T,tSA> const& ) noexcept;
/** Invert dual quaternion
 *
 * \see invert_normalized
 */
template< typename T, class tSA, class tNA = detail::dquat_inv_spec_t<tSA> >
DualQuaternion<T,tNA> invert( DualQuaternion<T,tSA> const& ) noexcept;
/** Invert normalized dual quaternion
 *
 * Invert a dual quaternion under the assumption that it is already normalized,
 * i.e., that \f$\left\lVertQ\right\rVert = \left(1,0\right)\f$. In this case,
 * the inverse is just the conjugate.
 */
template< typename T, class tSA, class tNA = detail::dquat_inv_spec_t<tSA> > constexpr
DualQuaternion<T,tNA> invert_normalized( DualQuaternion<T,tSA> const& ) noexcept;

template< typename T, class tSA >
T norm_2_real( DualQuaternion<T,tSA> const& ) noexcept;
template< typename T, class tSA >
T norm_2_dual( DualQuaternion<T,tSA> const& ) noexcept;
template< typename T, class tSA >
T norm_2_real_squared( DualQuaternion<T,tSA> const& ) noexcept;
template< typename T, class tSA >
T norm_2_dual_squared( DualQuaternion<T,tSA> const& ) noexcept;

template< typename T, class tSA >
DualQuaternion<T,tSA> normalize( DualQuaternion<T,tSA> const& ) noexcept;
template< typename T, class tSA > FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA> normalize_eps( DualQuaternion<T,tSA> const& ) noexcept;

template< typename T, class tSA, class tSB, class tNA = typename detail::DQuatSpec<tSA,tSB>::type > constexpr
DualQuaternion<T,tNA> mix( T, DualQuaternion<T,tSA> const&, DualQuaternion<T,tSB> const& ) noexcept;
template< typename T, class tSA, class tSB, class tNA = typename detail::DQuatSpec<tSA,tSB>::type  > constexpr
DualQuaternion<T,tNA> lerp( T, DualQuaternion<T,tSA> const&, DualQuaternion<T,tSB> const& ) noexcept;
#if 0
template< typename T, class tSA, class tSB, class tNA = typename detail::QuatSpec<tSA,tSB>::type  > 
Quaternion<T,tNA> slerp( T, Quaternion<T,tSA> const&, Quaternion<T,tSB> const& ) noexcept;
#endif
//@}

//--    meta: bitwise traits            ///{{{2///////////////////////////////

/** \addtogroup FTLBitwiseTags
 * @{
 */
template< typename tType, class tSpec >
auto tag_is_bitwise_conjurable( DualQuaternion<tType,tSpec> const& )
	-> std::true_type { return {}; }
template< class tSize, typename tType, class tSpec >
auto tag_is_bitwise_materializable( DualQuaternion<tType,tSpec> const& )
	-> std::true_type { return {}; }
template< class tSize, typename tType, class tSpec >
auto tag_is_bitwise_duplicable( DualQuaternion<tType,tSpec> const& )
	-> std::true_type { return {}; }
template< class tSize, typename tType, class tSpec >
auto tag_is_bitwise_relocatable( DualQuaternion<tType,tSpec> const& )
	-> std::true_type { return {}; }
template< class tSize, typename tType, class tSpec >
auto tag_is_bitwise_dismissible( DualQuaternion<tType,tSpec> const& )
	-> std::true_type { return {}; }
/** @} */

//--    <<< ~ flux::fml namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/dual_quaternion.inl"
#endif // FLUX_FML_DUAL_QUATERNION_HPP_AD57781B_5C9A_4B6F_B0F7_850469FF60B5
