/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	New logging implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/log2.hpp>
#include <flux/std/log2_ctrl.hpp> //XXX
#include <flux/std/clock.hpp>

#include "logger.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::log >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(log)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	constexpr std::size_t kMaxScopeDepth = 8;
	constexpr char const* kDefaultMessage = "Couldn't format message";
	
	struct ThreadLogData_
	{
		std::size_t logScopeDepth = 0;

		Clock::time_point scopeBegin[kMaxScopeDepth];
		fmt::memory_buffer scopeNames[kMaxScopeDepth];
	};
	
	thread_local ThreadLogData_ tlLogData;
}

//--    d: log()                            ///{{{2///////////////////////////
namespace detail
{
	void log( LogMeta const& aMeta, flux_ext::fmt::memory_buffer aBuf )
	{
		detail::logger().log(
			aMeta,
			std::move(aBuf),
			Clock::now(),
			tlLogData.logScopeDepth
		);
	}

	void panic( LogMeta const& aMeta, char const* aMessage, char const* aMessageMeta, char const* aName, char const* aWhat ) noexcept
	{
		if( !aMessage )
			aMessage = kDefaultMessage;
		
		detail::logger().panic( 
			aMeta, 
			aMessage, aMessageMeta,
			aName, aWhat,
			Clock::now(),
			tlLogData.logScopeDepth
		);
	}
	void panic( LogMeta const& aMeta, char const* aMessage, compat::string_view const& aMessageMeta, char const* aName, char const* aWhat ) noexcept
	{
		if( !aMessage )
			aMessage = kDefaultMessage;
		
		detail::logger().panic( 
			aMeta, 
			aMessage, aMessageMeta,
			aName, aWhat,
			Clock::now(),
			tlLogData.logScopeDepth
		);
	}
}

//--    <<< ~ flux::log namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(log)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
