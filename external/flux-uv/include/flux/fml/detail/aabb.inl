/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- AABB
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>

#include <flux/compat/constexpr.hpp>

#include <limits>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::fml >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	template< typename tVector > inline FLUX_CONSTEXPR_EX
	bool all_greater_zero_( tVector const& aVec ) noexcept
	{
		using Value_ = typename tVector::value_type;
		for( std::size_t i = 0; i < aVec.size(); ++i )
		{
			if( aVec[i] < Value_(0) )
				return false;
		}

		return true;
	}
	template< typename tVector > inline FLUX_CONSTEXPR_EX
	bool all_less_equal_( tVector const& aX, tVector const& aY ) noexcept
	{
		FLUX_ASSERT( aX.size() == aY.size() );
		for( std::size_t i = 0; i < aX.size(); ++i )
		{
			if( aX[i] > aY[i] )
				return false;
		}

		return true;
	}
}

//--    a: AabbExtrema                      ///{{{2///////////////////////////
namespace aspect
{
	template< class tDim, typename tType, class tSpace >
	struct AabbExtrema
	{
		// types
		using value_type = tType;
		using Extent = svec_rebind_space_t< SmallVector<tDim,tType>, tSpace >;
		using Position = svec_rebind_space_t< SmallPosition<tDim,tType>, tSpace >;

		struct Data
		{
			Position aabbMin, aabbMax;
		};

		static constexpr bool staticSize = ftl::is_static_value<tDim>::value;

		// helpers
		static FLUX_CONSTEXPR_EX 
		Position min( Data const& aData ) noexcept(staticSize) {
			return aData.aabbMin;
		}
		static FLUX_CONSTEXPR_EX 
		Position max( Data const& aData ) noexcept(staticSize) {
			return aData.aabbMax;
		}
		static FLUX_CONSTEXPR_EX 
		Extent diagonal( Data const& aData ) noexcept(staticSize) {
			return aData.aabbMax - aData.aabbMin;
		}
		static FLUX_CONSTEXPR_EX 
		Position center( Data const& aData ) noexcept(staticSize) {
			return aData.aabbMin + (aData.aabbMax - aData.aabbMin) / tType(2);
		}
		static FLUX_CONSTEXPR_EX 
		Extent extent( Data const& aData ) noexcept(staticSize) {
			return (aData.aabbMax - aData.aabbMin) / tType(2);
		}

		static FLUX_CONSTEXPR_EX 
		void init_invalid( Data& aData ) noexcept(staticSize)
		{
			aData.aabbMin = make_splat<Position>( std::numeric_limits<tType>::max() );
			aData.aabbMax = make_splat<Position>( std::numeric_limits<tType>::min() );
		}
		static FLUX_CONSTEXPR_EX 
		void init_extrema( Data& aData, Position const& aMin, Position const& aMax ) noexcept(staticSize)
		{
			FLUX_ASSERT( detail::all_less_equal_( aMin, aMax ) );
			aData.aabbMin = aMin;
			aData.aabbMax = aMax;
		}
		static FLUX_CONSTEXPR_EX 
		void init_delta( Data& aData, Position const& aMin, Extent const& aDiag ) noexcept(staticSize)
		{
			FLUX_ASSERT( detail::all_greater_zero_( aDiag ) );
			aData.aabbMin = aMin;
			aData.aabbMax = aMin + aDiag;
		}
		static FLUX_CONSTEXPR_EX
		void init_extent( Data& aData, Position const& aCenter, Extent const& aExtent ) noexcept(staticSize)
		{
			FLUX_ASSERT( detail::all_greater_zero_( aExtent ) );
			aData.aabbMin = aCenter - aExtent;
			aData.aabbMax = aCenter + aExtent;
		}
	};
}

//--    a: AabbDelta                        ///{{{2///////////////////////////
namespace aspect
{	
	template< class tDim, typename tType, typename tDelta, class tSpace >
	struct AabbDelta
	{
		// types
		using value_type = tType;
		using Extent = svec_rebind_space_t< SmallVector<tDim,tDelta>, tSpace >;
		using Position = svec_rebind_space_t< SmallPosition<tDim,tType>, tSpace >;

		struct Data
		{
			Position aabbMin;
			Extent aabbDiag;
		};

		static constexpr bool staticSize = ftl::is_static_value<tDim>::value;

		// helpers
		static FLUX_CONSTEXPR_EX
		Position min( Data const& aData ) noexcept(staticSize) {
			return aData.aabbMin;
		}
		static FLUX_CONSTEXPR_EX
		Position max( Data const& aData ) noexcept(staticSize) {
			return aData.aabbMin + aData.aabbDiag;
		}
		static FLUX_CONSTEXPR_EX
		Extent diagonal( Data const& aData ) noexcept(staticSize) {
			return aData.aabbDiag;
		}
		static FLUX_CONSTEXPR_EX
		Position center( Data const& aData ) noexcept(staticSize) {
			return aData.aabbMin + aData.aabbDiag / tDelta(2);
		}
		static FLUX_CONSTEXPR_EX
		Extent extent( Data const& aData ) noexcept(staticSize) {
			return aData.aabbDiag / tDelta(2);
		}

		static FLUX_CONSTEXPR_EX
		void init_invalid( Data& aData ) noexcept(staticSize)
		{
			tDelta ext = tDelta(std::numeric_limits<tDelta>::is_signed ? -1 : 0);
			aData.aabbDiag = make_splat<Position>( ext );
			aData.aabbMin = make_zero<Position>();
		}
		static inline FLUX_CONSTEXPR_EX
		void init_extrema( Data& aData, Position const& aMin, Position const& aMax ) noexcept(staticSize)
		{
			FLUX_ASSERT( detail::all_less_equal_( aMin, aMax ) );
			aData.aabbDiag = aMax - aMin;
			aData.aabbMin = aMin;
		}
		static inline FLUX_CONSTEXPR_EX
		void init_delta( Data& aData, Position const& aMin, Extent const& aDiag ) noexcept(staticSize)
		{
			FLUX_ASSERT( detail::all_greater_zero_( aDiag ) );
			aData.aabbDiag = aDiag;
			aData.aabbMin = aMin;
		}
		static inline FLUX_CONSTEXPR_EX
		void init_extent( Data& aData, Position const& aCenter, Extent const& aExtent ) noexcept(staticSize)
		{
			FLUX_ASSERT( detail::all_greater_zero_( aExtent ) );
			aData.aabbDiag = aExtent * tDelta(2);
			aData.aabbMin = aCenter - aExtent;
		}
	};
}

//--    a: AabbExtents                      ///{{{2///////////////////////////
namespace aspect
{
	template< class tDim, typename tType, typename tExtent, class tSpace >
	struct AabbExtents
	{
		// types
		using value_type = tType;
		using Extent = svec_rebind_space_t< SmallVector<tDim,tExtent>, tSpace >;
		using Position = svec_rebind_space_t< SmallPosition<tDim,tType>, tSpace >;

		struct Data
		{
			Position aabbCenter;
			Extent aabbExtent;
		};

		static constexpr bool staticSize = ftl::is_static_value<tDim>::value;

		// helpers
		static FLUX_CONSTEXPR_EX
		Position min( Data const& aData ) noexcept(staticSize) {
			return aData.aabbCenter - aData.aabbExtent;
		}
		static FLUX_CONSTEXPR_EX
		Position max( Data const& aData ) noexcept(staticSize) {
			return aData.aabbCenter + aData.aabbExtent;
		}
		static FLUX_CONSTEXPR_EX
		Extent diagonal( Data const& aData ) noexcept(staticSize) {
			return aData.aabbExtent * tExtent(2);
		}
		static FLUX_CONSTEXPR_EX
		Position center( Data const& aData ) noexcept(staticSize) {
			return aData.aabbCenter;
		}
		static FLUX_CONSTEXPR_EX
		Extent extent( Data const& aData ) noexcept(staticSize) {
			return aData.aabbExtent;
		}

		static FLUX_CONSTEXPR_EX
		void init_invalid( Data& aData ) noexcept(staticSize)
		{
			tExtent ext = tExtent(std::numeric_limits<tExtent>::is_signed ? -1 : 0);
			aData.aabbExtent = make_splat<Position>( ext );
			aData.aabbCenter = make_zero<Position>();
		}
		static FLUX_CONSTEXPR_EX
		void init_extrema( Data& aData, Position const& aMin, Position const& aMax ) noexcept(staticSize)
		{
			FLUX_ASSERT( detail::all_less_equal_( aMin, aMax ) );
			aData.aabbExtent = (aMax - aMin) / tExtent(2);
			aData.aabbCenter = aMin + aData.aabbExtent;
		}
		static FLUX_CONSTEXPR_EX
		void init_delta( Data& aData, Position const& aMin, Extent const& aDiag ) noexcept(staticSize)
		{
			FLUX_ASSERT( detail::all_greater_zero_( aDiag ) );
			aData.aabbExtent = aDiag / tExtent(2);
			aData.aabbCenter = aMin + aData.aabbExtent;
		}
		static FLUX_CONSTEXPR_EX
		void init_extent( Data& aData, Position const& aCenter, Extent const& aExtent ) noexcept(staticSize)
		{
			FLUX_ASSERT( detail::all_greater_zero_( aExtent ) );
			aData.aabbExtent = aExtent;
			aData.aabbCenter = aCenter;
		}
	};
}

//--    AABB                                ///{{{2///////////////////////////
template< class tStore > inline FLUX_CONSTEXPR_EX
auto Aabb<tStore>::min() const noexcept(staticSize) -> Position
{
	return tStore::min(aabbData);
}
template< class tStore > inline FLUX_CONSTEXPR_EX
auto Aabb<tStore>::max() const noexcept(staticSize) -> Position
{
	return tStore::max(aabbData);
}
template< class tStore > inline FLUX_CONSTEXPR_EX
auto Aabb<tStore>::diagonal() const noexcept(staticSize) -> Extent
{
	return tStore::diagonal(aabbData);
}
template< class tStore > inline FLUX_CONSTEXPR_EX
auto Aabb<tStore>::center() const noexcept(staticSize) -> Position
{
	return tStore::center(aabbData);
}
template< class tStore > inline FLUX_CONSTEXPR_EX
auto Aabb<tStore>::extent() const noexcept(staticSize) -> Extent
{
	return tStore::extent(aabbData);
}

//--    make_aabb_*()                       ///{{{2///////////////////////////
template< class tAabb > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_aabb<tAabb> )
(tAabb) make_aabb_extrema( typename tAabb::Position const& aMin, typename tAabb::Position const& aMax ) noexcept(tAabb::staticSize)
{
	tAabb ret{};
	tAabb::Storage::init_extrema( ret.aabbData, aMin, aMax );
	return ret;
}
template< class tAabb > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_aabb<tAabb> )
(tAabb) make_aabb_min_delta( typename tAabb::Position const& aMin, typename tAabb::Extent const& aDiag ) noexcept(tAabb::staticSize)
{
	tAabb ret{};
	tAabb::Storage::init_delta( ret.aabbData, aMin, aDiag );
	return ret;
}
template< class tAabb > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_aabb<tAabb> )
(tAabb) make_aabb_center_extent( typename tAabb::Position const& aCenter, typename tAabb::Extent const& aExtent ) noexcept(tAabb::staticSize)
{
	tAabb ret{};
	tAabb::Storage::init_extent( ret.aabbData, aCenter, aExtent );
	return ret;
}

template< class tAabb > inline
FLUX_ENABLE_IF( is_aabb<tAabb> )
(tAabb) make_aabb( typename tAabb::Position const* aPoints, std::size_t aCount ) noexcept(tAabb::staticSize)
{
	FLUX_ASSERT( aPoints && aCount );

	using Pos_ = typename tAabb::Position;
	using Val_ = typename Pos_::value_type;

	Pos_ pmin( Init::fill, std::numeric_limits<Val_>::max() );
	Pos_ pmax( Init::fill, std::numeric_limits<Val_>::min() );

	for( std::size_t i = 0; i < aCount; ++i )
	{
		pmin = min( pmin, aPoints[i] );
		pmax = max( pmax, aPoints[i] );
	}

	return make_aabb_extrema<tAabb>( pmin, pmax );
}

template< class tAabb > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_aabb<tAabb> )
(tAabb) make_invalid() noexcept(tAabb::staticSize)
{
	tAabb ret{};
	tAabb::Storage::init_invalid( ret.aabbData );
	return ret;
}
template< class tDim, typename tType, class tSpace > inline FLUX_CONSTEXPR_EX
Aabb<aspect::AabbExtrema<tDim,tType,tSpace>> make_inverted_aabb() noexcept(ftl::is_static_value<tDim>::value)
{
	tType const tmin = std::numeric_limits<tType>::min();
	tType const tmax = std::numeric_limits<tType>::max();
	
	using Aabb_ = Aabb< aspect::AabbExtrema<tDim,tType,tSpace> >;
	using Pos_ = typename Aabb_::Position;
		
	Aabb_ ret{};
	ret.aabbData.aabbMin = make_splat<Pos_>( tmax );
	ret.aabbData.aabbMax = make_splat<Pos_>( tmin );
	return ret;
}

//--    <<< ~ flux::fml namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
