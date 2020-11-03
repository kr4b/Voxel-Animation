/*-******************************************************* -- HEADER -{{{1- */
/*- Quaternion
 *
 * TODO: to/from SmallVector<>s ??+
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FML_QUATERNION_HPP_93EDCBB3_6DCC_4006_896E_CFD973ABAC58
#define FLUX_FML_QUATERNION_HPP_93EDCBB3_6DCC_4006_896E_CFD973ABAC58

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/identity.hpp>
#include <flux/std/enable_if.hpp>

#include <flux/compat/voidt.hpp>
#include <flux/compat/constexpr.hpp>

#include <flux/ftl/explicit_init_list.hpp>

#include "common.hpp"

#include <type_traits>

//--    >>> namespace = flux::fml >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    aspect                          ///{{{2///////////////////////////////
namespace aspect
{
	template< class tDstSpace = space::AnySpace, class tSrcSpace = space::AnySpace >
	struct QuatSpec
	{
		using SrcSpace = tSrcSpace;
		using DstSpace = tDstSpace;
	};
}

//--    detail                          ///{{{2///////////////////////////////
namespace detail
{
	template< class, class, class = compat::void_t<> > struct QuatSpecCompat;
}

//--    Quaternion                      ///{{{2///////////////////////////////

/** Quaternion
 *
 * A quaternion. The four elements of the quaternion are stored in a linear
 * array of type \a T in the order \f$\left[i, j, k, r\right]\f$.
 * 
 * A `SmallVector` can be rotated by a `Quaternion` using either the
 * `apply_quat_rot()` function or the generic `apply()`. A `Quaternion` can be
 * converted to a `SmallMatrix` (and vice-versa) using the `make_rotation()`
 * methods:
   \code
   using quatf = Quaternion<float>;
   using mat33f = SmallMatrix<ftl::StaticSize<3>,ftl::StaticSize<3>,float>;
  
   quatf q = make_quaternion<quatf>( ... );
   mat3f m = make_rotation<mat33f>( q );  // (1)
   quatf r = make_rotation<quatf>( m );  // (2)
   \endcode
 * (1) creates a rotation matrix from the quaternion \a q; any matrix type that
 * supports rotations may be used here. (2) converts a matrix to a quaterion.
 * Any matrix type that supports rotations can be used here; but for non-ortho-
 * normal matrices the results will be undefined.
 *
 * `Quaternion`, like `SmallMatrix` and other transforms, can be used to
 * describe transforms between spaces. Each `Quaternion<>` type stores a source
 * and destination space, and describe a transformation between those spaces.
 *
 * \see space_compatible, space::AnySpace
 */
template< typename T = float, class tSA = aspect::QuatSpec<> >
struct Quaternion
{
	using value_type = T;

	union
	{
		struct { T i, j, k, r; };
		T elem[4];
	};

	Quaternion() = default;
	Quaternion( Quaternion const& ) = default;
	Quaternion& operator=( Quaternion const& ) = default;

	FLUX_CONSTEXPR_EX explicit Quaternion( Init::Zero ) noexcept;
	FLUX_CONSTEXPR_EX explicit Quaternion( Init::Identity ) noexcept;
	FLUX_CONSTEXPR_EX Quaternion( T, T, T, T ) noexcept;

	Quaternion( ftl::ExplicitInitList<T> ) noexcept;

	template< typename Y, class tSB, 
		class = FLUX_ENABLE_IF(detail::QuatSpecCompat<tSA,tSB>)(int) 
	> FLUX_CONSTEXPR_EX
	Quaternion( Quaternion<Y,tSB> const& ) noexcept;
	template< typename Y, class tSB, 
		class = FLUX_ENABLE_IF(detail::QuatSpecCompat<tSA,tSB>)(int) 
	> FLUX_CONSTEXPR_EX
	Quaternion& operator=( Quaternion<Y,tSB> const& ) noexcept;

	FLUX_CONSTEXPR_EX T& operator[] (std::size_t) noexcept;
	constexpr T const& operator[] (std::size_t) const noexcept;

	FLUX_CONSTEXPR_EX T* data() noexcept;
	constexpr T const* data() const noexcept;
};

//--    meta functions                  ///{{{2///////////////////////////////

/** \anchor QuatTraits \name Meta: Quaternion traits
 * \brief Quaternion traits and meta-functions
 *
 * Meta functions to identify and manipulate the quaternion type.
 */
//@{

template< typename > struct is_quaternion : std::false_type {};

template< typename T, class tSA > 
struct is_quaternion< Quaternion<T,tSA> > 
	: std::true_type
{};

namespace detail
{
	template< typename > struct QuatSrcSpace;
	template< typename > struct QuatDstSpace;

	template< typename, class > struct QuatRebindSrc;
	template< typename, class > struct QuatRebindDst;
	template< typename, class, class > struct QuatRebind;

	template< class > struct QuatInvSpec;
	template< class, class > struct QuatSpec;
	template< class, class > struct QuatMulSpec;

	template< class tSA > 
	using quat_inv_spec_t = typename QuatInvSpec<tSA>::type;
	template< class tSA, class tSB > 
	using quat_mul_spec_t = typename QuatMulSpec<tSA,tSB>::type;
}

template< typename Q > using quat_type_t = typename Q::value_type;
template< typename Q > using quat_src_space_t = typename detail::QuatSrcSpace<Q>::type;
template< typename Q > using quat_dst_space_t = typename detail::QuatDstSpace<Q>::type;

template< typename Q, class tSpace > 
using quat_rebind_src_space_t = typename detail::QuatRebindSrc<Q,tSpace>::type;
template< typename Q, class tSpace > 
using quat_rebind_dst_space_t = typename detail::QuatRebindDst<Q,tSpace>::type;
template< typename Q, class tDstSpace, class tSrcSpace >
using quat_rebind_spaces_t = typename detail::QuatRebind<Q,tDstSpace,tSrcSpace>::type;
//@} 

//--    operators                       ///{{{2///////////////////////////////
/** \name Quaternion operations
 * \brief Quaternion operations
 *
 * Quaternion operations in operator form. With the exception of the
 * quaternion-by-quaternion multiplications, these operations are element-wise.
 *
 * For element-wise operations, the spaces of the quaternions must match. For
 * the multiplications, the spaces are propagated. Consider the quaternions
   \code
   using MWQuat = Quaternion< ..., aspect::QuatSpec<WorldSpace,ModelSpace> >;
   using WCQuat = Quaternion< ..., aspect::QuatSpec<CameraSpace,WorldSpace> >;
  
   MWQuat mw = ...;
   WCQuat wc = ...;
  
   auto const mc = wc * mw;
   \endcode
 * The quaternion `mc` will have be `aspect::QuatSpec<CameraSpace,ModelSpace>`,
 * i.e. a quaternion that transforms from `ModelSpace` to `CameraSpace`. 
 */
//@{
template< typename T, class tSA > constexpr
Quaternion<T,tSA> operator- (Quaternion<T,tSA> const&) noexcept;
template< typename T, class tSA > constexpr
Quaternion<T,tSA> operator+ (Quaternion<T,tSA> const&) noexcept;

template< typename T, class tSA, class tSB, class = typename detail::QuatSpec<tSA,tSB>::type > constexpr
bool operator== (Quaternion<T,tSA> const&, Quaternion<T,tSB> const& ) noexcept;
template< typename T, class tSA, class tSB, class = typename detail::QuatSpec<tSA,tSB>::type > constexpr
bool operator!= (Quaternion<T,tSA> const&, Quaternion<T,tSB> const& ) noexcept;


template< typename T, class tSA, class tSB, class tNA = typename detail::QuatSpec<tSA,tSB>::type > constexpr
Quaternion<T,tNA> operator+ (Quaternion<T,tSA> const&, Quaternion<T,tSB> const& ) noexcept;
template< typename T, class tSA , class tSB, class tNA = typename detail::QuatSpec<tSA,tSB>::type > constexpr
Quaternion<T,tNA> operator- (Quaternion<T,tSA> const&, Quaternion<T,tSB> const& ) noexcept;
template< typename T, class tSA, class tSB, class tNS = detail::quat_mul_spec_t<tSA,tSB> > constexpr
Quaternion<T,tNS> operator* (Quaternion<T,tSA> const&, Quaternion<T,tSB> const& ) noexcept;

template< typename T, class tSA > constexpr
Quaternion<T,tSA> operator* (Quaternion<T,tSA> const&, T ) noexcept;
template< typename T, class tSA > constexpr
Quaternion<T,tSA> operator/ (Quaternion<T,tSA> const&, T ) noexcept;
template< typename T, class tSA > constexpr
Quaternion<T,tSA> operator* (T, Quaternion<T,tSA> const& ) noexcept;


template< typename T, class tSA, class tSB, class = typename detail::QuatSpec<tSA,tSB>::type > FLUX_CONSTEXPR_EX
Quaternion<T,tSA>& operator+= (Quaternion<T,tSA>&, Quaternion<T,tSB> const&) noexcept;
template< typename T, class tSA, class tSB, class = typename detail::QuatSpec<tSA,tSB>::type > FLUX_CONSTEXPR_EX
Quaternion<T,tSA>& operator-= (Quaternion<T,tSA>&, Quaternion<T,tSB> const&) noexcept;
template< typename T, class tSA, class tSB, class = typename detail::QuatSpec<tSA,tSB>::type  > FLUX_CONSTEXPR_EX
Quaternion<T,tSA>& operator*= (Quaternion<T,tSA>&, Quaternion<T,tSB> const&) noexcept;

template< typename T, class tSA > FLUX_CONSTEXPR_EX
Quaternion<T,tSA>& operator*= (Quaternion<T,tSA>&, T) noexcept;
template< typename T, class tSA > FLUX_CONSTEXPR_EX
Quaternion<T,tSA>& operator/= (Quaternion<T,tSA>&, T) noexcept;
//@}

//--    functions                       ///{{{2///////////////////////////////

	// pseudo constructors
/** \name Quaternion pseudo-constructors
 * \brief Quaternion pseudo-constructors
 *
 * Create `Quaternion` of type \a Q as follows:
 *
 *  - `make_quat`: from the provided four values
 *  - `make_zero`: `Quaternion` with all elements equal to zero
 *  - `make_one`: `Quaternion` with all elements equal to one
 *  - `make_identity`: Identity `Quaternion` \f$=\left(0,0,0,1\right)\f$
 *
 * Examples:
   \code
   auto const v = make_quat<quatf>( 1.f, 2.f, 3.f, 4.f );
   auto const z = make_zero<quatf>(); // alt: make_zero( flux::Identity<quatf>{} );
   auto const I = make_identity<quat>(); // idenentity quaternion
   \endcode
 *
 * To construct quternions representing a rotation, see `make_rotation_3d`.
 *
 * \see make_rotation_3d
 */
//@{
template< typename Q > constexpr
FLUX_ENABLE_IF( is_quaternion<Q> )
(Q) make_quat( quat_type_t<Q>, quat_type_t<Q>, quat_type_t<Q>, quat_type_t<Q> ) noexcept;

template< typename Q >
FLUX_ENABLE_IF( is_quaternion<Q> )
(Q) make_quat( quat_type_t<Q> const* ) noexcept;

template< typename Q > constexpr
FLUX_ENABLE_IF( is_quaternion<Q> )
(Q) make_zero( Identity<Q> = {} ) noexcept;
template< typename Q > constexpr
FLUX_ENABLE_IF( is_quaternion<Q> )
(Q) make_identity( Identity<Q> = {} ) noexcept;
//@}

	// conversion
/* \name Quaternion conversion and accessors
 * \brief Quaternion conversion and accessors
 *
 * Functions to convert between different quaternions.
 *
 * `quaternion_cast<>` can be used to convert between `Quaternion`s with
 * different element types and with different spaces.
 *
 * `data_ptr` retrieves a pointer to the `Quaternion`'s data. It is equivalent
 * to calling the `Quaternion`'s `.data()` method.
 */
//@{
template< class Q, class R > constexpr
FLUX_ENABLE_IF_C( is_quaternion<Q>::value && is_quaternion<R>::value )
(Q) quaternion_cast( R const& ) noexcept;

template< typename T, class tSA > FLUX_CONSTEXPR_EX
T* data_ptr( Quaternion<T,tSA>& ) noexcept;
template< typename T, class tSA > constexpr
T const* data_ptr( Quaternion<T,tSA> const& ) noexcept;
//@}

	// math. functions
/** \name Quaternion functions
 * \brief Quaternion functions
 *
 * General mathematical quaternion functions, including conjugate, inverse,
 * norms/normalization and interpolation.
 */
//@{
template< typename T, class tSA, class tNA = detail::quat_inv_spec_t<tSA> > constexpr
Quaternion<T,tNA> conj( Quaternion<T,tSA> const& ) noexcept;
template< typename T, class tSA, class tNA = detail::quat_inv_spec_t<tSA> >
Quaternion<T,tNA> invert( Quaternion<T,tSA> const& ) noexcept;

template< typename T, class tSA >
T length( Quaternion<T,tSA> const& ) noexcept;
template< typename T, class tSA > constexpr
T length_squared( Quaternion<T,tSA> const& ) noexcept;
template< typename T, class tSA >
T norm_2( Quaternion<T,tSA> const& ) noexcept;

template< typename T, class tSA >
Quaternion<T,tSA> normalize( Quaternion<T,tSA> const& ) noexcept;
template< typename T, class tSA > constexpr
Quaternion<T,tSA> normalize_eps( Quaternion<T,tSA> const& ) noexcept;

template< typename T, class tSA, class tSB, class tNA = typename detail::QuatSpec<tSA,tSB>::type > constexpr
Quaternion<T,tNA> mix( T, Quaternion<T,tSA> const&, Quaternion<T,tSB> const& ) noexcept;
template< typename T, class tSA, class tSB, class tNA = typename detail::QuatSpec<tSA,tSB>::type  > constexpr
Quaternion<T,tNA> lerp( T, Quaternion<T,tSA> const&, Quaternion<T,tSB> const& ) noexcept;
template< typename T, class tSA, class tSB, class tNA = typename detail::QuatSpec<tSA,tSB>::type  > 
Quaternion<T,tNA> slerp( T, Quaternion<T,tSA> const&, Quaternion<T,tSB> const& ) noexcept;
//@}

//--    meta: bitwise traits            ///{{{2///////////////////////////////

/** \addtogroup FTLBitwiseTags
 * @{
 */
template< typename tType, class tSpec >
auto tag_is_bitwise_conjurable( Quaternion<tType,tSpec> const& )
	-> std::true_type { return {}; }
template< class tSize, typename tType, class tSpec >
auto tag_is_bitwise_materializable( Quaternion<tType,tSpec> const& )
	-> std::true_type { return {}; }
template< class tSize, typename tType, class tSpec >
auto tag_is_bitwise_duplicable( Quaternion<tType,tSpec> const& )
	-> std::true_type { return {}; }
template< class tSize, typename tType, class tSpec >
auto tag_is_bitwise_relocatable( Quaternion<tType,tSpec> const& )
	-> std::true_type { return {}; }
template< class tSize, typename tType, class tSpec >
auto tag_is_bitwise_dismissible( Quaternion<tType,tSpec> const& )
	-> std::true_type { return {}; }
/** @} */

//--    <<< ~ flux::fml namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/quaternion.inl"
#endif // FLUX_FML_QUATERNION_HPP_93EDCBB3_6DCC_4006_896E_CFD973ABAC58
