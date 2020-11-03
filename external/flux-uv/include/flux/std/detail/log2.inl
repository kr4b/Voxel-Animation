/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	log2.hpp implementation
 */
/*-***************************************************************** -}}}1- */

#include <tuple>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::log >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(log)
//--    log()                               ///{{{2///////////////////////////
template< typename... tArgs > inline
void log( char const* aFmt, tArgs&&... aArgs ) noexcept
{
	log( 
		LogMeta{
			uint16_t(ELogSeverity::info),
			uint16_t(ELogFacility::unspecified),
			0,
			{}
		}, 
		aFmt, 
		std::forward<tArgs>(aArgs)... 
	);
}
template< typename tFmt, typename... tArgs > inline
void log( ELogSeverity aSev, tFmt const& aFmt, tArgs&&... aArgs ) noexcept
{
	log( 
		LogMeta{
			uint16_t(aSev), 
			uint16_t(ELogFacility::unspecified),
			0,
			{}
		}, 
		aFmt, 
		std::forward<tArgs>(aArgs)...
	);
}

template< typename tFmt, typename... tArgs > inline
void log( LogMeta const& aMeta, tFmt const& aFmt, tArgs&&... aArgs ) noexcept
{
	try
	{
		flux_ext::fmt::memory_buffer buf;
		flux_ext::fmt::format_to( buf, aFmt, std::forward<tArgs>(aArgs)... );

		detail::log( aMeta, std::move(buf) );
	}
	catch( std::exception const& eErr )
	{
		detail::panic( 
			aMeta, 
			nullptr,
			detail::convert_fmtstr(aFmt), 
			typeid(eErr).name(),
			eErr.what() 
		);
	}
}

//--    d: LogSpec_                         ///{{{2///////////////////////////
namespace detail
{
	template< bool tFac > constexpr
	LogSpec_<true,tFac> operator| (LogSpec_<false,tFac> aSpec, ELogSeverity aSev) noexcept {
		return LogSpec_<true,tFac>{ uint16_t(aSev), aSpec.facility, aSpec.flags, aSpec.aux };
	}
	template< bool tSev > constexpr
	LogSpec_<tSev,true> operator| (LogSpec_<tSev,false> aSpec, ELogFacility aFac) noexcept {
		return LogSpec_<tSev,true>{ aSpec.severity, uint16_t(aFac), aSpec.flags, aSpec.aux };
	}
	template< bool tSev, bool tFac > constexpr
	LogSpec_<tSev,tFac> operator| (LogSpec_<tSev,tFac> aSpec, ELogFlag aFlag) noexcept {
		return LogSpec_<tSev,tFac>{ aSpec.severity, aSpec.facility, aSpec.flags|(1u << uint32_t(aFlag)), aSpec.aux };
	}

	template< bool tSev, bool tFac > constexpr
	LogMeta operator+ (LogSpec_<tSev,tFac> aSpec) noexcept {
		static_assert( tSev, "Log severity not specified" );
		return LogMeta{ aSpec.severity, aSpec.facility, aSpec.flags, aSpec.aux };
	}
}

//--    d: convert_fmtstr()                 ///{{{2///////////////////////////
namespace detail
{
	constexpr 
	char const* convert_fmtstr( char const* aStr ) noexcept
	{
		return aStr;
	}

	inline
	compat::string_view convert_fmtstr( fmt::basic_string_view<char> const& aFmt ) noexcept
	{
		return compat::string_view( aFmt.data(), aFmt.size() );
	}
}

//--    d: LogFwd                           ///{{{2///////////////////////////
namespace detail
{
	template< typename... tArgs > inline
	LogFwd::LogFwd( LogMeta const& aMeta, tArgs&&... aArgs ) noexcept
		: LogFwd(
			Key_<0>{}, 
			aMeta, 
			compat::make_index_sequence< sizeof...(tArgs)-1 >{},
			std::forward_as_tuple( std::forward<tArgs>(aArgs)... )
		)
	{}

	template< std::size_t... tIndices, typename tTuple > inline
	LogFwd::LogFwd( Key_<0>, LogMeta const& aMeta, compat::index_sequence<tIndices...>, tTuple&& aTuple ) noexcept
		: LogFwd(
			Key_<1>{},
			aMeta,
			std::get<tIndices>(std::forward<tTuple>(aTuple))...
		)
	{}

	template< typename... tArgs > inline
	LogFwd::LogFwd( Key_<1>, LogMeta const& aMeta, tArgs&&... aArgs ) noexcept
	{
		log( aMeta, std::forward<tArgs>(aArgs)... );
	}
}

//--    <<< ~ flux::log namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(log)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
