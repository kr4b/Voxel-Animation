/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Protected<> implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>

#include <flux/compat/constexpr.hpp>

#include <utility>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    Locked<>                            ///{{{2///////////////////////////
template< typename tType > inline
Locked<tType>::Locked( tType& aRef, std::unique_lock<std::mutex> aLock )
	: mPtr(&aRef)
	, mLock( std::move(aLock) )
{}

#if 1
template< typename tType > inline FLUX_CONSTEXPR_EX
tType& Locked<tType>::operator*() const
{
	return FLUX_ASSERT(mPtr), *mPtr;
}
template< typename tType > inline FLUX_CONSTEXPR_EX
tType* Locked<tType>::operator->() const
{
	return FLUX_ASSERT(mPtr), mPtr;
}
#endif

/*template< typename tType > inline FLUX_CONSTEXPR_EX
Locked<tType>::operator tType&() const
{
	return FLUX_ASSERT(mPtr), *mPtr;
}*/

//--    Protected<>                         ///{{{2///////////////////////////
template< typename tType > template< typename... tArgs > inline
Protected<tType>::Protected( tArgs&&... aArgs )
	: mValue( std::forward<tArgs>(aArgs)... )
{}

#if 0
template< typename tType > inline
Locked<tType> Protected<tType>::operator*()
{
	return Locked<tType>( mValue, Lock_(mMutex) );
}
template< typename tType > inline
Locked<tType const> Protected<tType>::operator*() const
{
	return Locked<tType const>( mValue, Lock_(mMutex) );
}

template< typename tType > inline
Locked<tType> Protected<tType>::operator->()
{
	return Locked<tType>( mValue, Lock_(mMutex) );
}
template< typename tType > inline
Locked<tType const> Protected<tType>::operator->() const
{
	return Locked<tType const>( mValue, Lock_(mMutex) );
}
#endif

template< typename tType > inline
Locked<tType> Protected<tType>::acquire()
{
	return Locked<tType>( mValue, Lock_(mMutex) );
}
template< typename tType > inline
Locked<tType const> Protected<tType>::acquire() const
{
	return Locked<tType const>( mValue, Lock_(mMutex) );
}

template< typename tType > inline
compat::optional<Locked<tType>> Protected<tType>::try_acquire() 
{
	if( auto lock = Lock_( mMutex, std::try_to_lock ) )
		return Locked<tType>( mValue, std::move(lock) );

	return compat::nullopt;
}
template< typename tType > inline
compat::optional<Locked<tType const>> Protected<tType>::try_acquire() const
{
	if( auto lock = Lock_( mMutex, std::try_to_lock ) )
		return Locked<tType>( mValue, std::move(lock) );

	return compat::nullopt;
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
