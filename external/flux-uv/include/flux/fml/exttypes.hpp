/*-******************************************************* -- HEADER -{{{1- */
/*- Extended standard types
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FML_EXTTYPES_HPP_A2A6FC59_CD3E_491C_92B3_70E1EF59080B
#define FLUX_FML_EXTTYPES_HPP_A2A6FC59_CD3E_491C_92B3_70E1EF59080B

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include "stdtypes.hpp"
#include "quaternion.hpp"
#include "small_vector.hpp"
#include "small_matrix.hpp"
#include "dual_quaternion.hpp"

//--    >>> namespace = flux::fml >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    types                           ///{{{2///////////////////////////////

inline namespace exttypes
{
	/** \anchor FMLExtTypes \name Extended FML types
	 *  \brief Extended FML types
	 *
	 * Extended vector/position, matrix and quaternion types. The extended types
	 * reside in specific spaces (i.e., `space::ModelSpace`, `space::WorldSpace`,
	 * `space::CameraSpace`, `space::ClipSpace`, and `space::ScreenSpace`).
	 *
	 * Vectors fully reside in a single space. Transforms (such as matrices, 
	 * (dual) quaternions and similar) typically map from a source space to a 
	 * destination space. For example, transforming a `ModelVec4f` with a 
	 * model-to-world matrix `M2WMat44f` will result in a `WorldVec4f`:
	   \code
	   ModelVec4f mv = ...;
	   M2WMat44f model2world = ...;
	  
	   WorldVec4f wv = model2world * mv;
	   \endcode
	 *
	 * Operations other than multiplication typically require that the operands
	 * reside in the same space(s). 
	 *
	 * The special `space::AnySpace` is an exception, and is compatible with all
	 * other space. The standard types (\ref FMLStdTypes) reside in
	 * `space::AnySpace` making, for example, the following legal:
	   \code
	   ModelVec3f mv = ...;
	   mv += make_zero<vec3f>();
	   \endcode
	 */
	//@{
	/** \brief Extended FML type
	 * See \ref FMLExtTypes
	 */
	using ModelVec3f = svec_rebind_space_t<vec3f, space::ModelSpace>;
	using ModelVec4f = svec_rebind_space_t<vec4f, space::ModelSpace>;
	using ModelPos3f = svec_rebind_space_t<pos3f, space::ModelSpace>;
	using ModelPos4f = svec_rebind_space_t<pos4f, space::ModelSpace>;

	using ModelVec3i = svec_rebind_space_t<vec3i, space::ModelSpace>;
	using ModelPos3i = svec_rebind_space_t<pos3i, space::ModelSpace>;

	using WorldVec3f = svec_rebind_space_t<vec3f, space::WorldSpace>;
	using WorldVec4f = svec_rebind_space_t<vec4f, space::WorldSpace>;
	using WorldPos3f = svec_rebind_space_t<pos3f, space::WorldSpace>;
	using WorldPos4f = svec_rebind_space_t<pos4f, space::WorldSpace>;

	using WorldVec3i = svec_rebind_space_t<vec3i, space::WorldSpace>;
	using WorldPos3i = svec_rebind_space_t<pos3i, space::WorldSpace>;

	using CameraVec3f = svec_rebind_space_t<vec3f, space::CameraSpace>;
	using CameraVec4f = svec_rebind_space_t<vec4f, space::CameraSpace>;
	using CameraPos3f = svec_rebind_space_t<pos3f, space::CameraSpace>;
	using CameraPos4f = svec_rebind_space_t<pos4f, space::CameraSpace>;

	using CameraVec3i = svec_rebind_space_t<vec3i, space::CameraSpace>;
	using CameraPos3i = svec_rebind_space_t<pos3i, space::CameraSpace>;

	using ClipVec3f = svec_rebind_space_t<vec3f, space::ClipSpace>;
	using ClipVec4f = svec_rebind_space_t<vec4f, space::ClipSpace>;
	using ClipPos3f = svec_rebind_space_t<pos3f, space::ClipSpace>;
	using ClipPos4f = svec_rebind_space_t<pos4f, space::ClipSpace>;

	using ClipVec3i = svec_rebind_space_t<vec3i, space::ClipSpace>;
	using ClipPos3i = svec_rebind_space_t<pos3i, space::ClipSpace>;

	using ScreenVec2f = svec_rebind_space_t<vec2f, space::ScreenSpace>;
	using ScreenVec3f = svec_rebind_space_t<vec3f, space::ScreenSpace>;
	using ScreenVec4f = svec_rebind_space_t<vec4f, space::ScreenSpace>;
	using ScreenPos2f = svec_rebind_space_t<pos2f, space::ScreenSpace>;
	using ScreenPos3f = svec_rebind_space_t<pos3f, space::ScreenSpace>;
	using ScreenPos4f = svec_rebind_space_t<pos4f, space::ScreenSpace>;

	using ScreenVec2i = svec_rebind_space_t<vec2i, space::ScreenSpace>;
	using ScreenVec3i = svec_rebind_space_t<vec3i, space::ScreenSpace>;
	using ScreenPos2i = svec_rebind_space_t<pos2i, space::ScreenSpace>;
	using ScreenPos3i = svec_rebind_space_t<pos3i, space::ScreenSpace>;


	using M2WMat33f = smat_rebind_spaces_t<mat33f, space::WorldSpace, space::ModelSpace>;
	using M2WMat44f = smat_rebind_spaces_t<mat44f, space::WorldSpace, space::ModelSpace>;

	using WorldMat33f = smat_rebind_spaces_t<mat33f, space::WorldSpace, space::WorldSpace>;
	using WorldMat44f = smat_rebind_spaces_t<mat44f, space::WorldSpace, space::WorldSpace>;

	using W2CMat33f = smat_rebind_spaces_t<mat33f, space::CameraSpace, space::WorldSpace>;
	using W2CMat44f = smat_rebind_spaces_t<mat44f, space::CameraSpace, space::WorldSpace>;

	using CamMat33f = smat_rebind_spaces_t<mat33f, space::CameraSpace, space::CameraSpace>;
	using CamMat44f = smat_rebind_spaces_t<mat44f, space::CameraSpace, space::CameraSpace>;

	using ProjMat44f = smat_rebind_spaces_t<mat44f, space::ClipSpace, space::CameraSpace>;


	using WorldQuatf = quat_rebind_spaces_t<quatf, space::WorldSpace, space::WorldSpace>;
	using CamQuatf = quat_rebind_spaces_t<quatf, space::CameraSpace, space::CameraSpace>;

	using M2WQuatf = quat_rebind_spaces_t<quatf, space::WorldSpace, space::ModelSpace>;
	using W2CQuatf = quat_rebind_spaces_t<quatf, space::CameraSpace, space::WorldSpace>;


	using WorldDQuatf = dquat_rebind_spaces_t<dquatf, space::WorldSpace, space::WorldSpace>;
	using CamDQuatf = dquat_rebind_spaces_t<dquatf, space::CameraSpace, space::CameraSpace>;

	using M2WDQuatf = dquat_rebind_spaces_t<dquatf, space::WorldSpace, space::ModelSpace>;
	using W2CDQuatf = dquat_rebind_spaces_t<dquatf, space::CameraSpace, space::WorldSpace>;
	//@}
}

//--    <<< ~ flux::fml namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_FML_EXTTYPES_HPP_A2A6FC59_CD3E_491C_92B3_70E1EF59080B
