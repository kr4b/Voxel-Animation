/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- narrow*() implementations
 */
/*-***************************************************************** -}}}1- */

#include <limits>
#include <utility>

#include <flux/util/hint.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::util >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(util)
//--    narrow_cast()                       ///{{{2///////////////////////////
template< typename tTarget, typename tSource > inline
tTarget narrow_cast( tSource&& aSource ) noexcept
{
	return static_cast<tTarget>(std::forward<tSource>(aSource));
}

//--    narrow()                            ///{{{2///////////////////////////
namespace detail
{
	template< bool tSameSign, typename tTarget, typename tSource >
	struct NarrowImpl0_
	{
		static tTarget enact( tSource aValue )
		{
			tTarget x = narrow_cast<tTarget>(aValue);
			if( FLUX_HINT_UNLIKELY(static_cast<tSource>(x) != aValue) )
				throw error::NarrowError{};
			return x;
		}
		static optional<tTarget> enact( tSource aValue, compat::error_code& aErrorOut ) noexcept
		{
			tTarget x = narrow_cast<tTarget>(aValue);
			if( FLUX_HINT_UNLIKELY(static_cast<tSource>(x) != aValue) )
			{
				aErrorOut = compat::make_error_code( compat::errc::result_out_of_range );
				return {};
			}
			
			return x;
		}

	};
	template< typename tTarget, typename tSource >
	struct NarrowImpl0_<false,tTarget,tSource>
	{
		static tTarget enact( tSource aValue )
		{
			tTarget x = narrow_cast<tTarget>(aValue);
			if( FLUX_HINT_UNLIKELY(static_cast<tSource>(x) != aValue) )
				throw error::NarrowError{};
			if( FLUX_HINT_UNLIKELY((x < tTarget{}) != (aValue < tSource{})) )
				throw error::NarrowError{};
			return x;
		}
		static optional<tTarget> enact( tSource aValue, compat::error_code& aErrorOut ) noexcept
		{
			tTarget x = narrow_cast<tTarget>(aValue);
			if( FLUX_HINT_UNLIKELY(static_cast<tSource>(x) != aValue) ||  FLUX_HINT_UNLIKELY((x < tTarget{}) != (aValue < tSource{})) )
			{
				aErrorOut = compat::make_error_code( compat::errc::result_out_of_range );
				return {};
			}

			return x;
		}

	};

	template< typename tTarget, typename tSource >
	struct NarrowImpl_
		: NarrowImpl0_< 
			std::is_signed<tTarget>::value == std::is_signed<tSource>::value,
			tTarget,
			tSource
		> 
	{};
	template< typename tType >
	struct NarrowImpl_<tType,tType> {
		static tType enact( tType aValue ) 
		{
			return aValue;
		}
		static optional<tType> enact( tType aValue, compat::error_code& ) noexcept
		{
			return aValue;
		}
	};
}

template< typename tTarget, typename tSource > inline
tTarget narrow( tSource aValue )
{
	return detail::NarrowImpl_<tTarget,tSource>::enact( aValue );
}

template< typename tTarget, typename tSource > inline
optional<tTarget> narrow( tSource aValue, compat::error_code& aErrorOut ) noexcept
{
	return detail::NarrowImpl_<tTarget,tSource>::enact( aValue, aErrorOut );
}


//--    <<< ~ flux::util namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(util)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
