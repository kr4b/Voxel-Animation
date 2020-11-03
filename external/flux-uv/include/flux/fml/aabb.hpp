/*-******************************************************* -- HEADER -{{{1- */
/*- AABB
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FML_AABB_HPP_F91D6D9D_7BDB_4CA1_B078_88FED4300E0B
#define FLUX_FML_AABB_HPP_F91D6D9D_7BDB_4CA1_B078_88FED4300E0B

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/identity.hpp>
#include <flux/std/enable_if.hpp>

#include <flux/compat/constexpr.hpp>

#include <flux/ftl/dsvalue.hpp>
#include <flux/ftl/not_null.hpp>

#include "small_vector.hpp"

//--    >>> namespace = flux::fml >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    aspect                          ///{{{2///////////////////////////////
namespace aspect
{
	template< 
		class tDim, 
		typename tType, 
		class = space::AnySpace 
	>
	struct AabbExtrema;

	template< 
		class tDim, 
		typename tType, 
		typename tDelta = tType, 
		class = space::AnySpace
	>
	struct AabbDelta;

	template< 
		class tDim, 
		typename tType,
		typename tExtent = tType,
		class = space::AnySpace 
	>
	struct AabbExtents;
};

//--    AABB                            ///{{{2///////////////////////////////
template< class tStorage = aspect::AabbExtrema<ftl::StaticSize<3>,float> >
struct Aabb
{
	using value_type = typename tStorage::value_type;
	using Storage = tStorage;
	using Extent = typename tStorage::Extent;
	using Position = typename tStorage::Position;

	static constexpr bool staticSize = tStorage::staticSize;

	FLUX_CONSTEXPR_EX Position min() const noexcept(staticSize);
	FLUX_CONSTEXPR_EX Position max() const noexcept(staticSize);
	FLUX_CONSTEXPR_EX Extent diagonal() const noexcept(staticSize);

	FLUX_CONSTEXPR_EX Position center() const noexcept(staticSize);
	FLUX_CONSTEXPR_EX Extent extent() const noexcept(staticSize);

	FLUX_CONSTEXPR_EX bool valid() const noexcept;

	/*private:*/ typename tStorage::Data aabbData;
};

//--    standard types                  ///{{{2///////////////////////////////

using aabb2f = Aabb< aspect::AabbExtrema<ftl::StaticSize<2>,float> >;
using aabb3f = Aabb< aspect::AabbExtrema<ftl::StaticSize<3>,float> >;

//--    meta functions                  ///{{{2///////////////////////////////

template< class > struct is_aabb : std::false_type {};
template< class tStorage > 
struct is_aabb< Aabb<tStorage> > 
	: std::true_type
{};

namespace detail
{
	template< class > struct AabbSpace;
}

template< class tAabb >
using aabb_space_t = typename detail::AabbSpace<tAabb>::type;

//TODO: aabb_rebind_space_t

//--    functions                       ///{{{2///////////////////////////////
/** Create AABB from minimum and maximum points
 *
 * Return an AABB that spans from \a aMin to \a aMax. 
 *
 * \warning All components of \a aMin must be strictly less or equal than those
 * of \a aMax.
 */
template< class tAabb > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_aabb<tAabb> )
(tAabb) make_aabb_extrema( 
	typename tAabb::Position const& aMin,
	typename tAabb::Position const& aMax
) noexcept(tAabb::staticSize);
/** Create AABB from a minimum point and a diagonal
 *
 * Return an AABB that spans from \f$\mbox{aMin}\f$ to 
 * \f$\mbox{aMin}+\mbox{aDiag}\f$.
 */
template< class tAabb > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_aabb<tAabb> )
(tAabb) make_aabb_min_delta(
	typename tAabb::Position const& aMin,
	typename tAabb::Extent const& aDiag
) noexcept(tAabb::staticSize);
/** Create AABB from a center point and its extent
 *
 * Return an AABB that spans from \f$\mbox{aCenter}-\mbox{aExtent}\f$ to 
 * \f$\mbox{aCenter}+\mbox{aExtnet}\f$.
 */
template< class tAabb > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_aabb<tAabb> )
(tAabb) make_aabb_center_extent(
	typename tAabb::Position const& aCenter,
	typename tAabb::Extent const& aExtent
) noexcept(tAabb::staticSize);

template< class tAabb >
FLUX_ENABLE_IF( is_aabb<tAabb> )
(tAabb) make_aabb( 
	ftl::not_null<typename tAabb::Position const*>, 
	std::size_t aCount 
) noexcept(tAabb::staticSize);

/** Return invalid AABB
 *
 * Returns an invalid/inverted AABB, i.e., an AABB whose minimum point is set
 * to a value larger than its maximum point. Such an AABB would have a negative
 * diagonal / extent.
 *
 * \warning Some AABB configurations can not represent invalid AABBs. In this
 * case, a zero-volume AABB is returned instead. This occurs, for example, for
 * AABBs with min-diagonal or center-extent storage, if the diagnonal or extent
 * has an unsigned type.
 *
 * \see make_inverted_aabb()
 */
template< class tAabb > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_aabb<tAabb> )
(tAabb) make_invalid( Identity<tAabb> = {} ) noexcept(tAabb::staticSize);
/** Return inverted AABB
 *
 * Returns an inverted AABB, where the minimum point is placed at the most 
 * positive possible position, and the maximum point is placed at the most
 * negative one.
 *
 * \warning This function only works for AABBss with extrema storage.
 *
 * \see make_invalid()
 */
template< class tDim, typename tType, class tSpace > FLUX_CONSTEXPR_EX
Aabb<aspect::AabbExtrema<tDim,tType,tSpace>> make_inverted_aabb() noexcept(ftl::is_static_value<tDim>::value);

#if 0
//TODO
template< class tA, class tB > inline
FLUX_ENABLE_IF_C( is_aabb<tA>::value && is_aabb<tB>::value )
(tA) aabb_cast( tB const& ); //TODO



template< class tAabb > inline
FLUX_ENABLE_IF( is_aabb<tAabb> )
(tAabb) combine( tAabb const&, tAabb const& ); //TODO

template< class tAabb > inline
FLUX_ENABLE_IF( is_aabb<tAabb> )
(tAabb) extend( tAabb const&, typename tAabb::Position const& ); //TODO


template< class tAabb > inline
FLUX_ENABLE_IF( is_aabb<tAabb> )
(typename tAabb::value_type) volume( tAabb const& ); //TODO
template< class tAabb > inline
FLUX_ENABLE_IF( is_aabb<tAabb> )
(typename tAabb::value_type) surface_area( tAabb const& ); //TODO

// TODO: tests: overlaps(), inside()

// TODO: transform by matrix.
#endif



//TODO: is_bitwise_* tags??

//--    <<< ~ flux::fml namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/aabb.inl"
#endif // FLUX_FML_AABB_HPP_F91D6D9D_7BDB_4CA1_B078_88FED4300E0B
