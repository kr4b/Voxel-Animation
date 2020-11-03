/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- logger2.hxx implementation
 */
/*-***************************************************************** -}}}1- */

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::log::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(log,detail)
//--    Event                               ///{{{2///////////////////////////
inline
Event::Event( EvMessage* aMessage ) noexcept
	: type( EEventType::message )
{
	message = aMessage;
}

inline
Event::Event( EvTerminate ) noexcept
	: type( EEventType::terminate )
{}

//--    <<< ~ flux::log::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(log,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
