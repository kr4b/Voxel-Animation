/*-******************************************************* -- HEADER -{{{1- */
/*- std::call_once fallback
 *
 * This fallback doesn't do anything terribly intelligent; it just uses a
 * mutex + an atomic flag. On the other hand, it does handle the exception
 * case correctly...
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_CALL_ONCE_HPP_D2EE4ED6_BDE8_4F0F_9408_1556CF95EA26
#define FLUX_NIH_CALL_ONCE_HPP_D2EE4ED6_BDE8_4F0F_9408_1556CF95EA26

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <mutex>
#include <atomic>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    once_flag                           ///{{{2///////////////////////////
class once_flag
{
	public:
		once_flag() noexcept;

		once_flag( once_flag const& ) = delete;
		once_flag& operator= (once_flag const& ) = delete;

	private:
		std::mutex mMutex;
		std::atomic<bool> mFlag;

	/* Note: atomic_flag can only do test_and_set(). But we want to defer the
	 * set() part until after successfully calling the callable...
	 */

	template< class tCallable, typename... tArgs >
	friend void call_once( once_flag&, tCallable&&, tArgs&&... );
};

//--    call_once()                         ///{{{2///////////////////////////

template< class tCallable, typename... tArgs >
void call_once( once_flag&, tCallable&&, tArgs&&... );

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/call_once.inl"
#endif // FLUX_NIH_CALL_ONCE_HPP_D2EE4ED6_BDE8_4F0F_9408_1556CF95EA26
