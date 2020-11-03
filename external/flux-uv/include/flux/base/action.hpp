/*-******************************************************* -- HEADER -{{{1- */
/*-	Action
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_BASE_ACTION_HPP_76166C54_172C_4C4A_891B_E27C80D86AAE
#define FLUX_BASE_ACTION_HPP_76166C54_172C_4C4A_891B_E27C80D86AAE

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/ftl/signal.hpp>

#include <flux/compat/constexpr.hpp>

#include "message.hpp"

//--    >>> namespace = flux::base >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(base)
//--    types                               ///{{{2///////////////////////////

enum class EOutcome
{
	proceed = 0,
	discard,

	ignored
};

//--    Action                              ///{{{2///////////////////////////
namespace detail
{
	struct ActionReduce
	{
		using ResultType = EOutcome;
		
		FLUX_CONSTEXPR_EX
		bool operator() (EOutcome aOutcome) noexcept
		{
			if( EOutcome::ignored == aOutcome ) return true;

			mOutcome = aOutcome;
			
			if( EOutcome::discard == aOutcome ) return false;

			return true;
		}
		FLUX_CONSTEXPR_EX
		EOutcome finalize() const noexcept
		{
			return mOutcome;
		}

		private:
			EOutcome mOutcome = EOutcome::ignored;
	};
}

template< class tRelay >
using Action = ftl::Signal< 
	EOutcome (Message const&, tRelay&), 
	detail::ActionReduce
>;

//--    <<< ~ flux::base namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(base)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_BASE_ACTION_HPP_76166C54_172C_4C4A_891B_E27C80D86AAE
