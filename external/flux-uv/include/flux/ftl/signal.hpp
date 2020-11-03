/*-******************************************************* -- HEADER -{{{1- */
/*-	Signal system, take 2
 *
 * Signal system, without dependencies, but with
 *   - signal priorities
 *   - return value reduction
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_SIGNAL_HPP_83231F87_3254_4468_AF37_2E56B681009A
#define FLUX_FTL_SIGNAL_HPP_83231F87_3254_4468_AF37_2E56B681009A

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/function.hpp>
#include <flux/std/identity.hpp>
#include <flux/std/inttypes.hpp>
#include <flux/std/enable_if.hpp>
#include <flux/std/shared_ptr.hpp>

#include <flux/ftl/vector.hpp>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    types                               ///{{{2///////////////////////////

using SignalPriority = std::int16_t;

class SignalLink;
class AutoSignalLink;

//--    aspects                             ///{{{2///////////////////////////
namespace aspect
{
	struct SignalReduceDiscard
	{
		using ResultType = void;
		
		template< typename tType >
		bool operator()( tType&& );

		ResultType finalize();
	};
};

//--    detail                              ///{{{2///////////////////////////
namespace detail
{
	using SignalId = unsigned;
	constexpr SignalId kInvalidSignalId = ~0u;
	
	struct SignalAnchor
	{
		void* signal;
		void (*disconnect)( void*, SignalId );
	};

	struct SignalLinkProxy
	{
		SignalId id;
		SignalAnchor anchor;

		std::shared_ptr<SignalAnchor>& anchorPtr;

		SignalLinkProxy( SignalLinkProxy const& ) = delete;
		SignalLinkProxy& operator= (SignalLinkProxy const&) = delete;

		SignalLinkProxy( SignalLinkProxy&& ) = default;

#		if FLUX_COMPILER == FLUX_COMPILER_MSVC && _MSC_VER <= 1800
		SignalLinkProxy( SignalId aId, SignalAnchor aAnchor, std::shared_ptr<SignalAnchor>& aPtr )
			: id(aId)
			, anchor(aAnchor)
			, anchorPtr(aPtr)
		{}
#		endif // ~ VisualStudio 2013

		operator SignalLink() const;
	};

	template< class tSig, class tFun >
	struct SignalIsNoexcept;

	template< class tFun >
	struct SignalRemoveNoexcept
	{
		using type = tFun;
	};
	template< typename tRet, typename... tArgs >
	struct SignalRemoveNoexcept< tRet(tArgs...) noexcept >
	{
		using type = tRet(tArgs...);
	};
}

//--    Signal                              ///{{{2///////////////////////////
template< class tFun, class tReduce = aspect::SignalReduceDiscard >
class Signal final
{
	public:
		using Link = SignalLink;
		using Signature = tFun;
		using ResultType = typename tReduce::ResultType;

	private:
		using Except_ = typename detail::SignalRemoveNoexcept<Signature>::type;

	public:
		static constexpr bool isNoexcept = !std::is_same<Except_,Signature>::value;

	public:
		Signal() = default;

		Signal( Signal&& ) noexcept;
		Signal& operator= (Signal&&) noexcept;

		Signal( Signal const& ) = delete;
		Signal& operator= (Signal const&) = delete;
	
	public:
		void clear();
		bool empty() const noexcept;

		template< typename... tActualArgs >
		ResultType emit( tActualArgs&&... ) const noexcept(isNoexcept);


		template< class tCallable >
		detail::SignalLinkProxy connect( tCallable&& );
		template< class tCallable >
		detail::SignalLinkProxy connect( tCallable&&, SignalPriority );

		void disconnect( Link const& );

	private:
		struct Item_
		{
			unique_function<Except_> f;
			//unique_function< tRet(tArgs...) > f;
			detail::SignalId id;
			SignalPriority prio;

			Item_() = default;
			
			template< class tCallable >
			Item_( tCallable&& aCall, detail::SignalId aId, SignalPriority aPrio )
				: f( std::forward<tCallable>(aCall) )
				, id( aId )
				, prio( aPrio )
			{}
		};

	private:
		void disconnect_( detail::SignalId );
		void cleanup_deleted_() const;

		template< typename... tActualArgs >
		void emit_( Identity<void>, tActualArgs&&... ) const noexcept(isNoexcept);
		template< typename tType, typename... tActualArgs >
		ResultType emit_( Identity<tType>, tActualArgs&&... ) const noexcept(isNoexcept);

		static void disconnect_fn_( void*, detail::SignalId );

	private:
		mutable ftl::Vector<
			Item_,
			sizeof(Item_)
		> mItems;
	
		shared_ptr<detail::SignalAnchor> mAnchor;

		detail::SignalId mNextId = 0;
		mutable bool mInFlight = false;
};

//--    SignalLink                          ///{{{2///////////////////////////
class SignalLink final
{
	public:
		SignalLink() = default;

	private:
		friend struct detail::SignalLinkProxy;
		SignalLink( shared_ptr<detail::SignalAnchor>, detail::SignalId );

		template< class, class > friend class Signal;

	public:
		void clear() noexcept;
		void disconnect();

	private:
		detail::SignalId mId;
		weak_ptr<detail::SignalAnchor> mAnchor;
};

//--    AutoSignalLink                      ///{{{2///////////////////////////
class AutoSignalLink final
{
	public:
		AutoSignalLink() = default;
		~AutoSignalLink();

		AutoSignalLink( AutoSignalLink&& ) = default;
		AutoSignalLink& operator= (AutoSignalLink&&) = default;

		AutoSignalLink( AutoSignalLink const& ) = delete;
		AutoSignalLink& operator= (AutoSignalLink const&) = delete;

		AutoSignalLink( SignalLink&& ) noexcept;
		AutoSignalLink& operator= (SignalLink&&) noexcept;

		AutoSignalLink( SignalLink const& ) noexcept;
		AutoSignalLink& operator= (SignalLink const&) noexcept;

		AutoSignalLink( detail::SignalLinkProxy const& ) noexcept;
		AutoSignalLink& operator= (detail::SignalLinkProxy const&) noexcept;

	public:
		void clear() noexcept;
		void disconnect();

		SignalLink reset( SignalLink = SignalLink() ) noexcept;

	private:
		SignalLink mLink;
};

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/signal.inl"
#endif // FLUX_FTL_SIGNAL_HPP_83231F87_3254_4468_AF37_2E56B681009A
