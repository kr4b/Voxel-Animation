/*-******************************************************* -- HEADER -{{{1- */
/*- Generate common transforms
 *
 * TODO: make_translation_3d( DualQuat ) -> VecN that supports translation
 * TODO: make_translation_3d( Mat44 ) -> VecN (for symmetry above)
 * TODO: make_transform_3d( DualQuat ) -> Mat44
 * TODO: make_transform_3d( Mat44 ) -> DualQuat
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FML_TRANSFORM_HPP_BF2D4325_51BC_4CDF_AFC9_6379FEF3BDC4
#define FLUX_FML_TRANSFORM_HPP_BF2D4325_51BC_4CDF_AFC9_6379FEF3BDC4

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/identity.hpp>
#include <flux/std/enable_if.hpp>

#include <flux/compat/constexpr.hpp>

#include <flux/util/attrib.hpp>

#include "forward.hpp"
#include "angle.hpp"

#include <type_traits>

//--    >>> namespace = flux::fml >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    meta functions                  ///{{{2///////////////////////////////

/** \name Meta: transformation traits
 *
 * Traits to identify what kind of transformations various types can support,
 * and optionally between what spaces a transformation takes place.
 */
/** @{ */

template< class > struct supports_scaling_3d;
template< class > struct supports_rotation_3d;
template< class > struct supports_translation_3d;
template< class > struct supports_projection_3d;

//TODO: class of transforms that are rotation+translation only
//TODO: add/update classes: rigid_body_transform_2d, linear_transform_3d, affine_transform_3d
//TODO: add/update classes: 2d versions? + is_transform_2d below.
template< class > struct supports_linear_transform_3d; //TODO
template< class > struct supports_affine_transform_3d; //TODO


/** Meta: can \a class represent any 3D transformations?
 *
 * Meta-function that returns true if \a class can represent 3D transformations
 * such as scaling, rotation, translation or projection.
 */
template< class > struct is_transform_3d;

namespace detail
{
	template< class > struct TformSrcSpace;
	template< class > struct TformDstSpace;
}

template< class T > using tform_src_space_t = typename detail::TformSrcSpace<T>::type;
template< class T > using tform_dst_space_t = typename detail::TformDstSpace<T>::type;

/** @} */

//--    functions - matrices            ///{{{2///////////////////////////////

/** \name  Construct 3D affine transformation matrices
 *  \brief Construct matrices representing affine transformations in 3D space
 *
 * Construct matrices that represent scaling, rotation, translation or
 * combinations thereof in 3D space.
 */
/** @{ */

/** Uniform scaling
 *
 * Return a matrix representing an uniform scaling in 3D:
   \code
   auto S = make_scaling_3d<mat33f>(s);
   \endcode
 * returns \f$S = \begin{bmatrix} s & 0 & 0 \\ 0 & s & 0 \\ 0 & 0 & s
 * \end{bmatrix}\f$. Any matrix that `supports_scaling_3d` may be used in lieu
 * of `mat33f`. Parts outside of the top-left \f$3\times 3\f$ region will be
 * set to correspond to the contents of an equally-sized identity matrix.
 */
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_scaling_3d<M>::value )
(M) make_scaling_3d(
	typename M::value_type,
	Identity<M> = {}
) noexcept(M::staticSize);
/** Non-uniform scaling
 *
 * Return a matrix representing a non-uniform scaling in 3D:
   \code
   auto S = make_scaling_3d<mat33f>(sx,sy,sz); // Or vec3f(sx,sy,sz)
   \endcode
 * returns \f$S = \begin{bmatrix} s_X & 0 & 0 \\ 0 & s_y & 0 \\ 0 & 0 & s_z
 * \end{bmatrix}\f$. Any matrix that `supports_scaling_3d` may be used in lieu
 * of `mat33f`. Parts outside of the top-left \f$3\times 3\f$ region will be
 * set to correspond to the contents of an equally-sized identity matrix.
 */
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_scaling_3d<M>::value )
(M) make_scaling_3d( 
	typename M::value_type, 
	typename M::value_type, 
	typename M::value_type,
	Identity<M> = {}
) noexcept(M::staticSize);
/** \copydoc make_scaling_3d */
template< class M, class V > FLUX_CONSTEXPR_EX //TODO: require same types? same spaces?
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_scaling_3d<M>::value && is_small_vector<V,3>::value )
(M) make_scaling_3d( 
	V const&,
	Identity<M> = {}
) noexcept(M::staticSize);

/** Rotation around axis
 *
 * Return a matrix representing a rotation of \a Angle around an axis in 3D:
   \code
   auto R = make_rotation_3d<mat33f>( 90.0_degf, ax, ay, az ); // OR: vec3f(ax,ay,az)
   \endcode
 * Any matrix that `supports_rotation_3d` may be used in lieu of `mat33f`.
 * Parts outside of the top-left \f$3\times 3\f$ region will be set to
 * correspond to the contents of an equally-sized identity matrix.
 */
template< class M >
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_rotation_3d<M>::value )
(M) make_rotation_3d( 
	Angle< typename M::value_type, aspect::Radians >,
	typename M::value_type,
	typename M::value_type,
	typename M::value_type,
	Identity<M> = {}
) noexcept(M::staticSize);
/** \copydoc make_rotation_3d */
template< class M, class V > //TODO: require same types? same spaces?
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_rotation_3d<M>::value && is_small_vector<V,3>::value )
(M) make_rotation_3d( 
	Angle< typename M::value_type, aspect::Radians >,
	V const&,
	Identity<M> = {}
) noexcept(M::staticSize);
/** Rotation matrix from quaternion
 *
 * Return a matrix representing the same rotation as the quaternion \a Q:
   \code
   quatf Q = ...;
   auto R = make_rotation_3d<mat33f>( Q );
   \endcode
 * Any matrix that `supports_rotation_3d` may be used in lieu of `mat33f`.
 * Parts outside of the top-left \f$3\times 3\f$ region will be set to
 * correspond to the contents of an equally-sized identity matrix.
 */
template< class M, class Q > //TODO: require same types? same spaces?
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_rotation_3d<M>::value && is_quaternion<Q>::value )
(M) make_rotation_3d( 
	Q const&,
	Identity<M> = {}
) noexcept(M::staticSize);
/** Rotation matrix from dual quaternion
 *
 * Return a matrix representing the same rotation as the dual quaternion \a Q:
   \code
   dquatf Q = ...;
   auto R = make_rotation_3d<mat33f>( Q );
   \endcode
 * Any matrix that `supports_rotation_3d` may be used in lieu of `mat33f`.
 * Parts outside of the top-left \f$3\times 3\f$ region will be set to
 * correspond to the contents of an equally-sized identity matrix.
 *
 * The translation part (if any) of the dual quaternion is ignored.
 */
template< class M, class DQ > //TODO: require same types? same spaces?
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_rotation_3d<M>::value && is_dual_quaternion<DQ>::value )
(M) make_rotation_3d( 
	DQ const&,
	Identity<M> = {}
) noexcept(M::staticSize);

/** Translation
 *
 * Return a matrix representing a translation:
   \code
   auto T = make_translation_3d<mat44f>( tx, ty, tz ); // OR: vec3f(tx,ty,tz)
   \endcode
 * Any matrix that `supports_translation_3d` may be used in lieu of `mat44f`.
 */
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_translation_3d<M>::value )
(M) make_translation_3d(
	typename M::value_type,
	typename M::value_type,
	typename M::value_type,
	Identity<M> = {}
) noexcept(M::staticSize);
/** \copydoc make_translation_3d */
template< class M, class V > FLUX_CONSTEXPR_EX //TODO: require same types? same space?
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_translation_3d<M>::value && is_small_vector<V,3>::value )
(M) make_translation_3d(
	V const&,
	Identity<M> = {}
) noexcept(M::staticSize);
/** Translation matrix from dual quaternion
 *
 * Return a matrix representing the same translation as the dual quaternion \a DQ:
   \code
   dquatf Q = ...;
   auto R = make_translation_3d<mat44f>( Q );
   \endcode
 * Any matrix that `supports_translation_3d` may be used in lieu of `mat44f`.
 *
 * The rotation part (if any) of the dual quaternion is ignored.
 */
template< class M, class DQ > FLUX_CONSTEXPR_EX //TODO: require same types? same spaces?
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_translation_3d<M>::value && is_dual_quaternion<DQ>::value )
(M) make_translation_3d(
	DQ const&,
	Identity<M> = {}
) noexcept(M::staticSize);

/** @} */

//TODO: DualQuaternion to matrix with rotation and translation

/** \name Construct 3D projection matrices
 *  \brief Construct matrices representing 3D projections
 *
 * Construct \f$4\times 4\f$ matrices representing 3D projections. 
 */
/** @{ */

template< class M > FLUX_DEPRECATED_EX( "Use make_projection_ortho_{gl,vk}_3d" ) FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_ortho_3d(
	typename M::value_type aLeft, typename M::value_type aRight,
	typename M::value_type aBottom, typename M::value_type aTop,
	Identity<M> = {}
) noexcept(M::staticSize);
template< class M > FLUX_DEPRECATED_EX( "Use make_projection_ortho_{gl,vk}_3d" ) FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_ortho_3d(
	typename M::value_type aLeft, typename M::value_type aRight,
	typename M::value_type aBottom, typename M::value_type aTop,
	typename M::value_type aNear, typename M::value_type aFar,
	Identity<M> = {}
) noexcept(M::staticSize);

template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_ortho_gl_3d(
	typename M::value_type aLeft, typename M::value_type aRight,
	typename M::value_type aBottom, typename M::value_type aTop,
	Identity<M> = {}
) noexcept(M::staticSize);
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_ortho_gl_3d(
	typename M::value_type aLeft, typename M::value_type aRight,
	typename M::value_type aBottom, typename M::value_type aTop,
	typename M::value_type aNear, typename M::value_type aFar,
	Identity<M> = {}
) noexcept(M::staticSize);

template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_ortho_vk_3d(
	typename M::value_type aLeft, typename M::value_type aRight,
	typename M::value_type aBottom, typename M::value_type aTop,
	Identity<M> = {}
) noexcept(M::staticSize);
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_ortho_vk_3d(
	typename M::value_type aLeft, typename M::value_type aRight,
	typename M::value_type aBottom, typename M::value_type aTop,
	typename M::value_type aNear, typename M::value_type aFar,
	Identity<M> = {}
) noexcept(M::staticSize);


template< class M > FLUX_DEPRECATED_EX( "Use make_projection_perspective_{gl,vk}_3d" )
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_3d(
	Angle< typename M::value_type, aspect::Radians >,
	typename M::value_type aAspect,
	typename M::value_type aNear, typename M::value_type aFar,
	Identity<M> = {}
) noexcept(M::staticSize);
template< class M > FLUX_DEPRECATED_EX( "Use make_projection_perspective_{gl,vk}_3d" ) FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_3d(
	typename M::value_type aLeft, typename M::value_type aRight,
	typename M::value_type aBottom, typename M::value_type aTop,
	typename M::value_type aNear, typename M::value_type aFar,
	Identity<M> = {}
) noexcept(M::staticSize);

/** Standard OpenGL projection matrix
 *
 * Returns the standard OpenGL projection matrix defined from a field of view
 * \f$\theta\f$, aspect ratio \f$\mathcal{A}\f$, the near plane distance
 * \f$n\f$ and the far plane distance \f$f\f$:
 *
 * \f$P_{\mbox{gl}} = \begin{bmatrix} s_x & 0 & 0 & 0 \\ 0 & s_y & 0 & 0 \\ 0 & 0 & -\frac{f+n}{f-n} & -2\,\frac{f\,n}{f-n} \\ 0 & 0 & -1 & 0 \end{bmatrix},\f$
 *
 * with \f$s_x =
 * \frac{1}{\mathcal{A}\,\mbox{tan}\left(\frac{\theta}{2}\right)}\f$ and \f$s_y
 * = \frac{1}{\mbox{tan}\left(\frac{\theta}{2}\right)}\f$.
 *
 * See article [OpenGL Projection Matrix](http://www.songho.ca/opengl/gl_projectionmatrix.html) for a derivation.
 */
template< class M >
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_gl_3d(
	Angle< typename M::value_type, aspect::Radians >,
	typename M::value_type aAspect,
	typename M::value_type aNear, typename M::value_type aFar,
	Identity<M> = {}
) noexcept(M::staticSize);
/** Standard OpenGL projection matrix
 *
 * Returns the standard OpenGL projection matrix defined from left \f$l\f$,
 * right \f$r\f$, top \f$t\f$, bottom \f$b\f$, near \f$n\f$ and far \f$f\f$
 * distances:
 *
 * \f$P_{\mbox{gl}} = \begin{bmatrix} 2\,\frac{n}{r-l} & 0 & \frac{r+l}{r-l} & 0 \\ 0 & 2\,\frac{n}{t-b} & \frac{t+b}{t-b} & 0 \\ 0 & 0 & -\frac{f+n}{f-n} & -2\,\frac{f\,n}{f-n} \\ 0 & 0 & -1 & 0 \end{bmatrix},\f$
 *
 * See article [OpenGL Projection Matrix](http://www.songho.ca/opengl/gl_projectionmatrix.html) for a derivation.
 */
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_gl_3d( /*XXX-untested*/
	typename M::value_type aLeft, typename M::value_type aRight,
	typename M::value_type aBottom, typename M::value_type aTop,
	typename M::value_type aNear, typename M::value_type aFar,
	Identity<M> = {}
) noexcept(M::staticSize);

/** Standard Vulkan projection matrix
 *
 * Returns the standard Vulkan projection matrix defined from a field of view
 * \f$\theta\f$, aspect ratio \f$\mathcal{A}\f$, the near plane distance
 * \f$n\f$ and the far plane distance \f$f\f$:
 *
 * \f$P_{\mbox{gl}} = \begin{bmatrix} s_x & 0 & 0 & 0 \\ 0 & -s_y & 0 & 0 \\ 0 & 0 & -\frac{f}{f-n} & -\frac{f\,n}{f-n} \\ 0 & 0 & -1 & 0 \end{bmatrix},\f$
 *
 * with \f$s_x =
 * \frac{1}{\mathcal{A}\,\mbox{tan}\left(\frac{\theta}{2}\right)}\f$ and \f$s_y
 * = \frac{1}{\mbox{tan}\left(\frac{\theta}{2}\right)}\f$.
 *
 * \warning This matrix flips the Y axis, i.e., transforms from a left-handed
 * system to a right-handed one!
 *
 * See [The new Vulkan Coordinate System](https://matthewwellings.com/blog/the-new-vulkan-coordinate-system/)
 * and [code from the Vulkan Cookbook](https://github.com/PacktPublishing/Vulkan-Cookbook/blob/master/Library/Source%20Files/10%20Helper%20Recipes/04%20Preparing%20a%20perspective%20projection%20matrix.cpp)
 * for additional discussion.
 */
template< class M >
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_vk_3d(
	Angle< typename M::value_type, aspect::Radians >,
	typename M::value_type aAspect,
	typename M::value_type aNear, typename M::value_type aFar,
	Identity<M> = {}
) noexcept(M::staticSize);
/** Standard Vulkan projection matrix
 *
 * Returns the standard Vulkan projection matrix defined from from left \f$l\f$,
 * right \f$r\f$, top \f$t\f$, bottom \f$b\f$, near \f$n\f$ and far \f$f\f$
 * distances:
 *
 * \f$P_{\mbox{gl}} = \begin{bmatrix} 2\,\frac{n}{r-l} & 0 & \frac{r+l}{r-l} & 0 \\ 0 & -2\,\frac{n}{t-b} & -\frac{t+b}{t-b} & 0 \\ 0 & 0 & -\frac{f}{f-n} & -\frac{f\,n}{f-n} \\ 0 & 0 & -1 & 0 \end{bmatrix},\f$
 *
 * \warning This matrix flips the Y axis, i.e., transforms from a left-handed
 * system to a right-handed one!
 *
 * See [The new Vulkan Coordinate System](https://matthewwellings.com/blog/the-new-vulkan-coordinate-system/)
 * and [code from the Vulkan Cookbook](https://github.com/PacktPublishing/Vulkan-Cookbook/blob/master/Library/Source%20Files/10%20Helper%20Recipes/04%20Preparing%20a%20perspective%20projection%20matrix.cpp)
 * for additional discussion.
 */
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_vk_3d(
	typename M::value_type aLeft, typename M::value_type aRight,
	typename M::value_type aBottom, typename M::value_type aTop,
	typename M::value_type aNear, typename M::value_type aFar,
	Identity<M> = {}
) noexcept(M::staticSize);

template< class M > 
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_reversez_gl_3d( // XXX-untested, not very useful?
	Angle< typename M::value_type, aspect::Radians >,
	typename M::value_type aAspect,
	typename M::value_type aNear, typename M::value_type aFar,
	Identity<M> = {}
) noexcept(M::staticSize);
#if 0
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_reversez_gl_3d( /*XXX-untested*/
	typename M::value_type aLeft, typename M::value_type aRight,
	typename M::value_type aBottom, typename M::value_type aTop,
	typename M::value_type aNear, typename M::value_type aFar,
	Identity<M> = {}
) noexcept(M::staticSize);
#endif

/** Reverse-Z Vulkan projection matrix
 *
 * Returns a Vulkan projection matrix with reversed \f$Z\f$ defined from a field of view
 * \f$\theta\f$, aspect ratio \f$\mathcal{A}\f$, the near plane distance
 * \f$n\f$ and the far plane distance \f$f\f$:
 *
 * \f$P_{\mbox{vk}} = \begin{bmatrix} s_x & 0 & 0 & 0 \\ 0 & -s_y & 0 & 0 \\ 0 & 0 & \frac{f}{f-n}-1 & \frac{f\,n}{f-n} \\ 0 & 0 & -1 & 0 \end{bmatrix},\f$
 *
 * with \f$s_x =
 * \frac{1}{\mathcal{A}\,\mbox{tan}\left(\frac{\theta}{2}\right)}\f$ and \f$s_y
 * = \frac{1}{\mbox{tan}\left(\frac{\theta}{2}\right)}\f$.
 *
 * \warning This matrix flips the Y axis, i.e., transforms from a left-handed
 * system to a right-handed one!
 *
 * Reverse-Z projection matrices promise a better distribution of precision 
 * across the view frustum. They require that the depth-test be reversed
 * (i.e., `VK_COMPARE_OP_GREATER` instead of `VK_COMPARE_OP_LESS`) and that the
 * depth buffer is cleared to zero instead of one. Reverse-Z projection 
 * matrices work best with a floating point depth buffer.
 *
 * See [Depth Precision](http://dev.theomader.com/depth-precision/) and [Depth Precision Visualized](https://developer.nvidia.com/content/depth-precision-visualized)
 * for additional discussion and an analysis of the precision.
 */
template< class M > 
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_reversez_vk_3d(
	Angle< typename M::value_type, aspect::Radians >,
	typename M::value_type aAspect,
	typename M::value_type aNear, typename M::value_type aFar,
	Identity<M> = {}
) noexcept(M::staticSize);
/** Reverse-Z Vulkan projection matrix
 *
 * Returns a Vulkan projection matrix with reversed \f$Z\f$ defined from left
 * \f$l\f$, right \f$r\f$, top \f$t\f$, bottom \f$b\f$, near \f$n\f$, and from
 * far \f$f\f$.
 *
 * \f$P_{\mbox{vk}} = \begin{bmatrix} 2\,\frac{n}{r-l} & 0 & \frac{r+l}{r-l} & 0 \\ 0 & -2\,\frac{n}{t-b} & -\frac{t+b}{t-b} & 0 \\ 0 & 0 & \frac{f}{f-n}-1 & \frac{f\,n}{f-n} \\ 0 & 0 & -1 & 0 \end{bmatrix},\f$
 *
 * \warning This matrix flips the Y axis, i.e., transforms from a left-handed
 * system to a right-handed one!
 *
 * Reverse-Z projection matrices promise a better distribution of precision 
 * across the view frustum. They require that the depth-test be reversed
 * (i.e., `VK_COMPARE_OP_GREATER` instead of `VK_COMPARE_OP_LESS`) and that the
 * depth buffer is cleared to zero instead of one. Reverse-Z projection 
 * matrices work best with a floating point depth buffer.
 *
 * See [Depth Precision](http://dev.theomader.com/depth-precision/) and [Depth Precision Visualized](https://developer.nvidia.com/content/depth-precision-visualized)
 * for additional discussion and an analysis of the precision.
 */
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_reversez_vk_3d(
	typename M::value_type aLeft, typename M::value_type aRight,
	typename M::value_type aBottom, typename M::value_type aTop,
	typename M::value_type aNear, typename M::value_type aFar,
	Identity<M> = {}
) noexcept(M::staticSize);

/** Infinite Reverse-Z Vulkan projection matrix
 *
 * Returns a Vulkan projection matrix with reversed \f$Z\f$ defined from a field of view
 * \f$\theta\f$, aspect ratio \f$\mathcal{A}\f$ and the near plane distance
 * \f$n\f$. The far plane is at infinity.
 *
 * \f$P_{\mbox{vk}} = \begin{bmatrix} s_x & 0 & 0 & 0 \\ 0 & -s_y & 0 & 0 \\ 0 & 0 & 0 & n \\ 0 & 0 & -1 & 0 \end{bmatrix},\f$
 *
 * with \f$s_x =
 * \frac{1}{\mathcal{A}\,\mbox{tan}\left(\frac{\theta}{2}\right)}\f$ and \f$s_y
 * = \frac{1}{\mbox{tan}\left(\frac{\theta}{2}\right)}\f$.
 *
 * \warning This matrix flips the Y axis, i.e., transforms from a left-handed
 * system to a right-handed one!
 *
 * Reverse-Z projection matrices promise a better distribution of precision 
 * across the view frustum. They require that the depth-test be reversed
 * (i.e., `VK_COMPARE_OP_GREATER` instead of `VK_COMPARE_OP_LESS`) and that the
 * depth buffer is cleared to zero instead of one. Reverse-Z projection 
 * matrices work best with a floating point depth buffer.
 *
 * See [Depth Precision](http://dev.theomader.com/depth-precision/) and [Depth Precision Visualized](https://developer.nvidia.com/content/depth-precision-visualized)
 * for additional discussion and an analysis of the precision.
 */
template< class M > 
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_reversez_vk_3d(
	Angle< typename M::value_type, aspect::Radians >,
	typename M::value_type aAspect,
	typename M::value_type aNear,
	Identity<M> = {}
) noexcept(M::staticSize);
/** Infinite Reverse-Z Vulkan projection matrix
 *
 * Returns a Vulkan projection matrix with reversed \f$Z\f$ defined from left
 * \f$l\f$, right \f$r\f$, top \f$t\f$, bottom \f$b\f$ and near \f$n\f$. The
 * far plane is at infinity.
 *
 * \f$P_{\mbox{vk}} = \begin{bmatrix} 2\,\frac{n}{r-l} & 0 & \frac{r+l}{r-l} & 0 \\ 0 & -2\,\frac{n}{t-b} & -\frac{t+b}{t-b} & 0 \\ 0 & 0 & 0 & n \\ 0 & 0 & -1 & 0 \end{bmatrix},\f$

 * \warning This matrix flips the Y axis, i.e., transforms from a left-handed
 * system to a right-handed one!
 *
 * Reverse-Z projection matrices promise a better distribution of precision 
 * across the view frustum. They require that the depth-test be reversed
 * (i.e., `VK_COMPARE_OP_GREATER` instead of `VK_COMPARE_OP_LESS`) and that the
 * depth buffer is cleared to zero instead of one. Reverse-Z projection 
 * matrices work best with a floating point depth buffer.
 *
 * See [Depth Precision](http://dev.theomader.com/depth-precision/) and [Depth Precision Visualized](https://developer.nvidia.com/content/depth-precision-visualized)
 * for additional discussion and an analysis of the precision.
 */
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_projection_perspective_reversez_vk_3d(
	typename M::value_type aLeft, typename M::value_type aRight,
	typename M::value_type aBottom, typename M::value_type aTop,
	typename M::value_type aNear,
	Identity<M> = {}
) noexcept(M::staticSize);

	// helpers
//TODO: make_look_at()

/** Vulkan "correction" matrix 
 *
 * The Vulkan NDC coordinates differ from the OpenGL ones in that the y-axis is
 * flipped and (shader output) expects the z-coordinate to be in the range
 * \f$\left[0,1\right]\f$. 
 *
 * The correction matrix \f$\begin{bmatrix} 1 & 0 & 0 & 0 \\ 0 & -1 & 0 & 0 
 * \\ 0 & 0 & .5 & .5 \\ 0 & 0 & 0 & 1 \end{bmatrix}\f$ returned by this 
 * function may be <b>pre-multiplied</b> with an OpenGL projection matrix (such
 * as those returned by `make_projection_perspective_3d`) to account for these
 * differences.
 *
 * See https://matthewwellings.com/blog/the-new-vulkan-coordinate-system/
 */
template< class M > FLUX_DEPRECATED_EX( "Use make_projection_..._vk_3d() directly" ) FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && supports_projection_3d<M>::value )
(M) make_vulkan_projection_correction_3d(
	Identity<M> = {}
) noexcept(M::staticSize);

/** @} */

//--    functions - quaternions         ///{{{2///////////////////////////////

/** \name Construct rotation quaternions
 *  \brief Construct quaternions representing 3D rotations
 */
/** @{ */

/** Rotation around axis
 *
 * Return a quaternion representing a rotation of \a Angle around an axis in 3D:
   \code
   auto Q = make_rotation_3d<quatf>( 90.0_degf, ax, ay, az ); // OR: vec3f(ax,ay,az)
   \endcode
 */
template< class Q > 
FLUX_ENABLE_IF_C( is_quaternion<Q>::value )
(Q) make_rotation_3d( 
	Angle< typename Q::value_type, aspect::Radians >,
	typename Q::value_type,
	typename Q::value_type,
	typename Q::value_type,
	Identity<Q> = {}
) noexcept;
/** \copydoc make_rotation_3d */
template< class Q, class V > //TODO: require same types? same spaces?
FLUX_ENABLE_IF_C( is_quaternion<Q>::value && is_small_vector<V>::value )
(Q) make_rotation_3d( 
	Angle< typename Q::value_type, aspect::Radians >,
	V const&,
	Identity<Q> = {}
) noexcept;

/** Quaternion rotation from matrix
 *
 * Return a quaternion representing the same rotation as the matrix \a M. The
 * method assumes that the matrix encodes a rotation. Any matrix that
 * `supports_rotation_3d` may be used. Values outside of the top-left
 * \f$3\times 3\f$ area are ignored.
 */
template< class Q, class M > //TODO require same types?
FLUX_ENABLE_IF_C( is_quaternion<Q>::value && is_small_matrix<M>::value && supports_rotation_3d<M>::value )
(Q) make_rotation_3d( 
	M const&,
	Identity<Q> = {}
) noexcept;

/** @} */

//--    functions - dual quaternions    ///{{{2///////////////////////////////

/** \name Construct dual quaternion transformations
 *  \brief Construct dual quaternions representing 3D rotations and translations
 */
/** @{ */

/** Rotation around axis
 *
 * Return a dual quaternion representing a rotation of \a Angle around an axis
 * in 3D:
   \code
   auto Q = make_rotation_3d<dquatf>( 90.0_degf, ax, ay, az ); // OR: vec3f(ax,ay,az)
   \endcode
 */
template< class DQ > 
FLUX_ENABLE_IF_C( is_dual_quaternion<DQ>::value )
(DQ) make_rotation_3d( 
	Angle< typename DQ::value_type, aspect::Radians >,
	typename DQ::value_type,
	typename DQ::value_type,
	typename DQ::value_type,
	Identity<DQ> = {}
) noexcept;
/** Rotation around axis
 *
 * Return a dual quaternion representing a rotation of \a Angle around an axis
 * in 3D:
   \code
   auto Q = make_rotation_3d<dquatf>( 90.0_degf, ax, ay, az ); // OR: vec3f(ax,ay,az)
   \endcode
 */
template< class DQ, class V > //TODO: require same types?
FLUX_ENABLE_IF_C( is_dual_quaternion<DQ>::value && is_small_vector<V>::value )
(DQ) make_rotation_3d( 
	Angle< typename DQ::value_type, aspect::Radians >,
	V const&,
	Identity<DQ> = {}
) noexcept;

template< class DQ, class M > //TODO require same types?
FLUX_ENABLE_IF_C( is_dual_quaternion<DQ>::value && is_small_matrix<M>::value && supports_rotation_3d<M>::value )
(DQ) make_rotation_3d( 
	M const&,
	Identity<DQ> = {}
) noexcept;

template< class DQ > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_dual_quaternion<DQ>::value )
(DQ) make_translation_3d(
	typename DQ::value_type,
	typename DQ::value_type,
	typename DQ::value_type,
	Identity<DQ> = {}
) noexcept;
template< class DQ, class V > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_dual_quaternion<DQ>::value && is_small_vector<V,3>::value )
(DQ) make_translation_3d(
	V const&,
	Identity<DQ> = {}
) noexcept;

//TODO: matrix with rotation and translation to DualQuaternion

/** @} */

//--    functions - vectors             ///{{{2///////////////////////////////

/** \name Construct translation vectors
 *  \brief Construct vectors representing 3D translations
 *
 * These are mainly provided for symmetry.
 */
/** @{ */

template< class V, class M > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_small_vector<V>::value && supports_translation_3d<V>::value && is_small_matrix<M>::value && supports_translation_3d<M>::value )
(V) make_translation_3d(
	M const&,
	Identity<V> = {}
) noexcept(V::staticSize);

template< class V, class Q > FLUX_CONSTEXPR_EX //TODO: require same types?
FLUX_ENABLE_IF_C( is_small_vector<V>::value && supports_translation_3d<V>::value && is_dual_quaternion<Q>::value )
(V) make_translation_3d(
	Q const&,
	Identity<V> = {}
) noexcept(V::staticSize);

/** @} */

//--    <<< ~ flux::fml namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/transform.inl"
#endif // FLUX_FML_TRANSFORM_HPP_BF2D4325_51BC_4CDF_AFC9_6379FEF3BDC4
