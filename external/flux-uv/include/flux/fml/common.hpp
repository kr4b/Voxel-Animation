/*-******************************************************* -- HEADER -{{{1- */
/*- Common definitions
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FML_COMMON_HPP_E389F592_D53E_4A6D_8E96_44953CFD78A7
#define FLUX_FML_COMMON_HPP_E389F592_D53E_4A6D_8E96_44953CFD78A7

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <type_traits>

//--    macros              ///{{{1///////////////////////////////////////////

/* This is a workaround for Visual Studio 2017, where the compiler ICEs when
 * certain (inline) methods are declared and defined separately. The workaround
 * eliminites the declaration via a #ifdef, and instead relies on only the
 * definition in the .inl file. There, the definition is augmented with default
 * arguments.
 * 
 * The behaviour has been observed in (_MSC_FULL_VER):
 *   - 191426433
 *   - 191526730 // Doesn't ICE, but errors on non-constexpr (*)
 *   - 191627025 // Errors on non-constexpr, and possibly also ICEs. Good times.
 *
 * (*) Which is BS, since they are constexpr just fine with the WO applied.
 *
 * It hasn't been observed in:
 *	 - 191225830
 *   - 191326128
 *
 * I'm not a 100% sure what is triggering the ICE. Currently, the problem is
 * affecting mainly the `make_foo<Thing>()` methods. It seems to be related to
 * the default arguments (in particular, all methods have in common the
 * foo_size_t<X> = foo_size_t<X>{} construct) combined with forward
 * declarations.
 */
#if FLUX_COMPILER == FLUX_COMPILER_MSVC && (_MSC_VER >= 1914)// && _MSC_VER <= 1916)
#	define FLUX_FML_MSVC_ICE_WORKAROUND 1
#else
#	define FLUX_FML_MSVC_ICE_WORKAROUND 0
#endif // ~ Workaround: MSVC

//--    >>> namespace = flux::fml >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    operations                      ///{{{2///////////////////////////////
namespace op
{
	struct ClassUnaryPM {}; // unary +,-

	struct ClassAdd {}; // binary  +,-
	struct ClassMul {}; // binary  *,/
	struct ClassCmp {}; // binary  ==, !=

	struct ClassInc {}; // +=, -=
	struct ClassScale {}; // *=, /=

	struct ClassDot {}; // dot() product
	struct ClassNorm {}; // length(), length_squared(), norm_*())
	struct ClassProj {}; // project(), reflect()
	struct ClassLerp {}; // lerp(), mix()
	struct ClassBound {}; // min(), max(), abs(), sign()
}

//--    spaces                          ///{{{2///////////////////////////////
/** \brief Well-known and special spaces
 *
 * Vectors/Positions can be bound to specific spaces, and transforms (matrices,
 * quaternions, and similar) can be associated with a source and destination
 * space. Transforming a vector/position requires the source space of the
 * transform to match with the vector's/position's space. The resulting vector
 * will reside in the destination space of the transform.
 *
 * The special `space::AnySpace` is compatible with all other spaces.
 *
 * \see space::AnySpace
 */
namespace space
{
	/** \brief `AnySpace`: space compatible with any space
	 *
	 * The `AnySpace` is a special meta-space that is compatible with all other
	 * spaces. For example, a vector in `AnySpace` can be added to a vector in
	 * a specific space. The result will be a vector in the more specific space.
	 * 
	 * `AnySpace` is the default space. The standard types (`vec3f`, `mat33f`, 
	 * `quatf` etc.) are all in `AnySpace`.
	 *
	 * Using only `AnySpace` is essentially equivalent to ignoring the spaces
	 * entirely.
	 */
	struct AnySpace {};

	struct ModelSpace {};
	struct WorldSpace {};
	struct CameraSpace {};
	struct ClipSpace {};
	struct ScreenSpace {};
}

template< class, class > struct space_compatible;
template< class, class > struct space_specialized;

template< class tUSpace, class tVSpace >
using space_specialized_t = typename space_specialized<tUSpace,tVSpace>::type;

//TODO space_compatible_v if VARIABLE_TEMPLATES and INLINE_VARIABLES

//--    constants                       ///{{{2///////////////////////////////
struct Init
{
	struct Zero {};
	struct Fill {};
	struct Identity {};

	static constexpr Zero zero{};
	static constexpr Fill fill{};
	static constexpr Identity identity{};
};

//--    <<< ~ flux::fml namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/common.inl"
#endif // FLUX_FML_COMMON_HPP_E389F592_D53E_4A6D_8E96_44953CFD78A7
