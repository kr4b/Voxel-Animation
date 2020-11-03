/*-******************************************************* -- HEADER -{{{1- */
/*- Processing state
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_STATE_HPP_05D6764C_830E_47EC_9BEC_82D30D9BC619
#define FLUX_REPORT_STATE_HPP_05D6764C_830E_47EC_9BEC_82D30D9BC619

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

//#include <flux/std/function.hpp>


//--    >>> namespace = flux::report >>>        ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(report)
//--    constants                       ///{{{2///////////////////////////////

enum class EState : int
{
	inherit = 0,
	ignored = 0, //TODO: used by BADNESS() to always register badness
	
	cold,
	hot,
	critical,

	loading = -1,
	offline = -2
};

//--    functions                       ///{{{2///////////////////////////////

EState thread_state() noexcept;

EState set_thread_state( EState ) noexcept;
EState set_process_state( EState ) noexcept;

//void on_state_change( unique_function<void(EState,EState) noexcept> );

//--    classes                         ///{{{2///////////////////////////////

class ScopedThreadState final
{
	public:
		explicit ScopedThreadState( EState ) noexcept;
		~ScopedThreadState() noexcept;

	private:
		EState mOldState;
};
class ScopedProcessState final
{
	public:
		explicit ScopedProcessState( EState ) noexcept;
		~ScopedProcessState() noexcept;

	private:
		EState mOldState;
};

//--    <<< ~ flux::report namespace <<<        ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(report)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_STATE_HPP_05D6764C_830E_47EC_9BEC_82D30D9BC619
