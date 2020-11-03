/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Signal implementation, take 2
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>
#include <flux/std/static_assert.hpp>
#include <flux/util/scope_exit.hpp>

#include <functional>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	template< class tNoeSig, class tFun >
	struct SignalIsNoexcept_;
	template< typename tRet, typename... tArgs, class tFun >
	struct SignalIsNoexcept_< tRet(tArgs...), tFun >
	{
		static constexpr bool value = noexcept( std::declval<tFun>()(std::declval<tArgs>()...) );
	};
	template< typename tRet, typename... tArgs, class tFun >
	struct SignalIsNoexcept_< tRet(tArgs...), std::reference_wrapper<tFun> >
	{
		static constexpr bool value = noexcept( std::declval<tFun>()(std::declval<tArgs>()...) );
	};

	template< class tFun >
	struct SignalRetType_;
	template< typename tRet, typename... tArgs >
	struct SignalRetType_< tRet(tArgs...) >
	{
		using type = tRet;
	};

	template< class tFun, class tBase >
	struct SignalChangeType_
	{
		using type = tBase;
	};

	template< class tFun, class tBase > struct SignalChangeType_<tFun&,tBase> {
		using type = tBase&;
	};
	template< class tFun, class tBase > struct SignalChangeType_<tFun&&,tBase> {
		using type = tBase&&;
	};
	template< class tFun, class tBase > struct SignalChangeType_<tFun const&,tBase> {
		using type = tBase const&;
	};
	template< class tFun, class tBase > struct SignalChangeType_<tFun const&&,tBase> {
		using type = tBase const&&;
	};
}

//--    d: SignalLinkProxy                  ///{{{2///////////////////////////
namespace detail
{
	inline
	SignalLinkProxy::operator SignalLink() const
	{
		if( !anchorPtr )
			anchorPtr = make_shared<SignalAnchor>( anchor );

		return SignalLink( anchorPtr, id );
	}
}

//--    Signal                              ///{{{2///////////////////////////

#define FLUXTMP_SIGNAL_CLASS_ Signal<tFun,tReduce>
#define FLUXTMP_SIGNAL_TEMPL_ template< class tFun, class tReduce >

	// ctors -{{{3-
FLUXTMP_SIGNAL_TEMPL_ inline
FLUXTMP_SIGNAL_CLASS_::Signal( Signal&& aOther ) noexcept
	: mItems( std::move(aOther.mItems) )
	, mAnchor( std::move(aOther.mAnchor) )
{
	if( mAnchor )
		mAnchor->signal = this;

	std::swap( mInFlight, aOther.mInFlight );
	std::swap( mNextId, aOther.mNextId );
}
FLUXTMP_SIGNAL_TEMPL_ inline
auto FLUXTMP_SIGNAL_CLASS_::operator= (Signal&& aOther) noexcept -> Signal&
{
	mItems = std::move(aOther.mItems);
	mAnchor = std::move(aOther.mAnchor);

	if( mAnchor )
		mAnchor->signal = this;

	std::swap( mInFlight, aOther.mInFlight );
	std::swap( mNextId, aOther.mNextId );

	return *this;
}

	// public -{{{3-
FLUXTMP_SIGNAL_TEMPL_ inline
void FLUXTMP_SIGNAL_CLASS_::clear() 
{
	FLUX_ASSERT( !mInFlight );

	mItems.clear();

	if( mAnchor )
	{
		mAnchor->signal = nullptr;
		mAnchor.reset();
	}

	mNextId = 0;
}
FLUXTMP_SIGNAL_TEMPL_ inline
bool FLUXTMP_SIGNAL_CLASS_::empty() const noexcept
{
	return mItems.empty();
}

FLUXTMP_SIGNAL_TEMPL_ template< typename... tActualArgs > inline
auto FLUXTMP_SIGNAL_CLASS_::emit( tActualArgs&&... aArgs ) const noexcept(isNoexcept) -> ResultType
{
	using Ret_ = typename detail::SignalRetType_<Except_>::type;
	return emit_( Identity<Ret_>{}, std::forward<tActualArgs>(aArgs)... );
}

FLUXTMP_SIGNAL_TEMPL_ template< class tCallable > inline
auto FLUXTMP_SIGNAL_CLASS_::connect( tCallable&& aCallable ) -> detail::SignalLinkProxy
{
	FLUX_STATIC_ASSERTM( !isNoexcept || (detail::SignalIsNoexcept_<Except_,tCallable>::value), "Signal<>::connect(): Signal signature is noexcept: must only connect() noexcept functions" );

	using CallNoe_ = typename detail::SignalChangeType_< 
		tCallable, 
		typename detail::SignalRemoveNoexcept<
			typename std::remove_reference<tCallable>::type
		>::type
	>::type;
	
	auto id = mNextId++;
	mItems.emplace_back( std::forward<CallNoe_>(aCallable), id, std::numeric_limits<SignalPriority>::max() );

	auto adata = detail::SignalAnchor{ this, &disconnect_fn_ };
	return detail::SignalLinkProxy{
		id,
		adata,
		mAnchor
	};
}
FLUXTMP_SIGNAL_TEMPL_ template< class tCallable > inline
auto FLUXTMP_SIGNAL_CLASS_::connect( tCallable&& aCallable, SignalPriority aPrio ) -> detail::SignalLinkProxy
{
	auto pos = std::lower_bound( mItems.begin(), mItems.end(), aPrio, 
		[] (Item_ const& aItem, SignalPriority aPrio) {
			return aItem.prio < aPrio;
		}
	);

	using CallNoe_ = typename detail::SignalChangeType_< 
		tCallable, 
		typename detail::SignalRemoveNoexcept<
			typename std::remove_reference<tCallable>::type
		>::type
	>::type;

	auto id = mNextId++;
	mItems.emplace( pos, std::forward<CallNoe_>(aCallable), id, aPrio );

	auto adata = detail::SignalAnchor{ this, &disconnect_fn_ };
	return detail::SignalLinkProxy{
		id,
		adata,
		mAnchor
	};
}

FLUXTMP_SIGNAL_TEMPL_ inline
void FLUXTMP_SIGNAL_CLASS_::disconnect( Link const& aLink )
{
	if( auto sp = aLink.mAnchor.lock() )
	{
		FLUX_ASSERT( sp->signal == this );
		disconnect_( aLink.mId );
	}
}

	// private -{{{3-
FLUXTMP_SIGNAL_TEMPL_ inline
void FLUXTMP_SIGNAL_CLASS_::disconnect_( detail::SignalId aId )
{
	auto it = std::find_if( mItems.begin(), mItems.end(), 
		[&aId] (Item_ const& aItem) { return aId == aItem.id; }
	);

	if( it != mItems.end() )
	{
		if( mInFlight )
			it->id = detail::kInvalidSignalId;
		else
			mItems.erase( it );
	}
}

FLUXTMP_SIGNAL_TEMPL_ inline
void FLUXTMP_SIGNAL_CLASS_::cleanup_deleted_() const
{
	FLUX_ASSERT( !mInFlight );

	mItems.erase( 
		std::remove_if( mItems.begin(), mItems.end(),
			[] (Item_ const& aItem ) { return detail::kInvalidSignalId == aItem.id; }
		),
		mItems.end()
	);
}

FLUXTMP_SIGNAL_TEMPL_ template< typename... tActualArgs > inline
void FLUXTMP_SIGNAL_CLASS_::emit_( Identity<void>, tActualArgs&&... aArgs ) const noexcept(isNoexcept)
{
	mInFlight = true;
	FLUX_UTIL_ON_SCOPE_EXIT{ mInFlight = false; cleanup_deleted_(); };

	for( auto& fn : mItems )
	{
		if( fn.id != detail::kInvalidSignalId )
			fn.f( aArgs... );
	}
}
FLUXTMP_SIGNAL_TEMPL_ template< typename tType, typename... tActualArgs > inline
auto FLUXTMP_SIGNAL_CLASS_::emit_( Identity<tType>, tActualArgs&&... aArgs ) const noexcept(isNoexcept) -> ResultType
{
	mInFlight = true;
	FLUX_UTIL_ON_SCOPE_EXIT{ mInFlight = false; cleanup_deleted_(); };

	tReduce red;
	for( auto& fn : mItems )
	{
		if( fn.id != detail::kInvalidSignalId )
		{
			if( !red( fn.f( aArgs... ) ) )
				break;
		}
	}

	return red.finalize();
}

FLUXTMP_SIGNAL_TEMPL_ inline
void FLUXTMP_SIGNAL_CLASS_::disconnect_fn_( void* aSelf, detail::SignalId aId )
{
	FLUX_ASSERT( aSelf );
	static_cast<Signal*>(aSelf)->disconnect_( aId );
}

#undef FLUXTMP_SIGNAL_TEMPL_
#undef FLUXTMP_SIGNAL_CLASS_

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
