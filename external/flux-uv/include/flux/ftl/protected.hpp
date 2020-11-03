/*-******************************************************* -- HEADER -{{{1- */
/*-	Protected<>: protect access to shared objects
 *
 * Protected<> is used to mandate synchronization when accessing shared objects
 * (such as globals). 
 *
 * TODO-WIP-TODO-
 * TODO: is this really a sensible interface? do we need it anywhere?
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_PROTECTED_HPP_C8C03EAF_8AAC_4DD5_AE50_89F2D130B630
#define FLUX_FTL_PROTECTED_HPP_C8C03EAF_8AAC_4DD5_AE50_89F2D130B630

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/optional.hpp>
#include <flux/compat/constexpr.hpp>

#include <mutex>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    Locked<>                            ///{{{2///////////////////////////
template< typename tType >
class Locked
{
	public:
		Locked( tType&, std::unique_lock<std::mutex> );

		Locked( Locked const& ) = delete;
		Locked& operator= (Locked const&) = delete;

		Locked( Locked&& ) = default;
		Locked& operator= (Locked&&) = default;

	public:
		FLUX_CONSTEXPR_EX tType& operator*() const;
		FLUX_CONSTEXPR_EX tType* operator->() const;
		
		//FLUX_CONSTEXPR_EX operator tType&() const;

	private:
		tType* mPtr;
		std::unique_lock<std::mutex> mLock;
};


//--    Protected<>                         ///{{{2///////////////////////////
template< typename tType >
class Protected
{
	public:
		template< typename... tArgs >
		Protected( tArgs&&... );

		Protected( Protected const& ) = delete;
		Protected& operator= (Protected const&) = delete;

	/*public:
		Locked<tType> operator*();
		Locked<tType const> operator*() const;

		Locked<tType> operator->();
		Locked<tType const> operator->() const;*/

	public:
		Locked<tType> acquire();	
		Locked<tType const> acquire() const;

		compat::optional<Locked<tType>> try_acquire();
		compat::optional<Locked<tType const>> try_acquire() const;

	private:
		using Lock_ = std::unique_lock<std::mutex>;

	private:
		tType mValue;
		mutable std::mutex mMutex;
};

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/protected.inl"
#endif // FLUX_FTL_PROTECTED_HPP_C8C03EAF_8AAC_4DD5_AE50_89F2D130B630
