/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	call_once<> implementation
 */
/*-***************************************************************** -}}}1- */

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    once_flag                           ///{{{2///////////////////////////
inline once_flag::once_flag() noexcept
	: mFlag(false)
{}

//--    call_once                           ///{{{2///////////////////////////
template< class tCallable, typename... tArgs > inline
void call_once( once_flag& aOnce, tCallable&& aCall, tArgs&&... aArgs )
{
	if( !aOnce.mFlag.load() )
	{
		std::unique_lock<std::mutex> lock( aOnce.mMutex );
		if( !aOnce.mFlag.load() )
		{
			aCall( std::forward<tArgs>(aArgs)... );
			aOnce.mFlag.store( true );
		}
	}
}

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
