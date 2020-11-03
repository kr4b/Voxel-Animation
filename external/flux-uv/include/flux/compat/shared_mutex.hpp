/*-******************************************************* -- HEADER -{{{1- */
/*-	shared_mutex & co
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_SHARED_MUTEX_HPP_2BC434B8_EE19_49E0_8C75_58AF21DF7896
#define FLUX_COMPAT_SHARED_MUTEX_HPP_2BC434B8_EE19_49E0_8C75_58AF21DF7896

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>
#include <flux/setup/stdfeat.hpp>

//--    variant<>                   ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX17_SHARED_MUTEX
#	include <shared_mutex>

FLUX_NAMESPACE_ENTER1(compat)
    using std::shared_mutex;
    using std::shared_lock;
FLUX_NAMESPACE_LEAVE1(compat)

#else // !CXX17_SHARED_MUTEX
#	include <flux_ext/shared_mutex.hpp>

FLUX_NAMESPACE_ENTER1(compat)
    using flux_ext::ting::shared_mutex;
    using flux_ext::ting::shared_lock;
FLUX_NAMESPACE_LEAVE1(compat)
#endif // ~ CXX17_SHARED_MUTEX

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_SHARED_MUTEX_HPP_2BC434B8_EE19_49E0_8C75_58AF21DF7896
