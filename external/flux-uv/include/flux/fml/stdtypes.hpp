/*-******************************************************* -- HEADER -{{{1- */
/*- Standard types
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FML_STDTYPES_HPP_B876BBDC_5034_4D45_9B97_762F90AAD9A8
#define FLUX_FML_STDTYPES_HPP_B876BBDC_5034_4D45_9B97_762F90AAD9A8

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/ftl/dsvalue.hpp>

#include "quaternion.hpp"
#include "small_vector.hpp"
#include "small_matrix.hpp"
#include "dual_quaternion.hpp"

//--    >>> namespace = flux::fml >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    types                           ///{{{2///////////////////////////////

inline namespace stdtypes
{
	/** \anchor FMLStdTypes \name Standard FML types
	 *  \brief Standard FML types
	 *
	 * Standard vector/position, matrix and quaternion types. The standard types
	 * are all in the special `space::AnySpace` pseudo-space. This makes the 
	 * standard types compatible with all other spaces, i.e., the standard types
	 * can be used in expressions containing elements from more specific spaces.
	 *
	 * Several predefined extended types (see \ref FMLExtTypes) define aliases to
	 * the standard types, but bound to specific spaces.
	 */
	//@{
	/** \brief Standard FML type
	 * See \ref FMLStdTypes
	 */
	using vec2f = SmallVector<ftl::StaticSize<2>,float>;
	using vec3f = SmallVector<ftl::StaticSize<3>,float>;
	using vec4f = SmallVector<ftl::StaticSize<4>,float>;

	using pos2f = SmallPosition<ftl::StaticSize<2>,float>;
	using pos3f = SmallPosition<ftl::StaticSize<3>,float>;
	using pos4f = SmallPosition<ftl::StaticSize<4>,float>;

	using mat33f = SmallMatrix<ftl::StaticSize<3>,ftl::StaticSize<3>,float>;
	using mat44f = SmallMatrix<ftl::StaticSize<4>,ftl::StaticSize<4>,float>;

	using quatf = Quaternion<float>;
	using dquatf = DualQuaternion<float>;

	using vec2i = SmallVector<ftl::StaticSize<2>,int>;
	using vec3i = SmallVector<ftl::StaticSize<3>,int>;

	using pos2i = SmallPosition<ftl::StaticSize<2>,int>;
	using pos3i = SmallPosition<ftl::StaticSize<3>,int>;
	//@}
}

//--    <<< ~ flux::fml namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_FML_STDTYPES_HPP_B876BBDC_5034_4D45_9B97_762F90AAD9A8
