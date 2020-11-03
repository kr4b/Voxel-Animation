/*-******************************************************* -- HEADER -{{{1- */
/*-	Standard messages
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_BASE_MSG_STDMSG_HPP_EE13B712_E460_49B7_8022_F0C0371937B3
#define FLUX_BASE_MSG_STDMSG_HPP_EE13B712_E460_49B7_8022_F0C0371937B3

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

//--    >>> namespace = flux::base >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(base)
//--    messages                            ///{{{2///////////////////////////
namespace msg
{
	enum class EQuit
	{
		quit
	};
}

//--    <<< ~ flux::base namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(base)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_BASE_MSG_STDMSG_HPP_EE13B712_E460_49B7_8022_F0C0371937B3
