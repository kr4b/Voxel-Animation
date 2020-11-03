/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Queue implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/base/queue.hpp>
#include <flux/base/message.hpp>
#include <flux/base/msg/stdmsg.hpp>
#include <flux/base/default_catalogs.hpp>


#include <flux_ext/phmap.hpp>
#include <flux_ext/concurrentqueue.hpp>

#include <mutex>
#include <condition_variable>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::base >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(base)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	constexpr std::size_t kDefaultDequeue_ = 8;

	// Workaround for some versions of VisualStudio 2017, where there is a
	// weird implementation of hash_value() in stdext for std::type_index
	// that is not actually compatible with type_index.
	struct HashTypeIndex_
	{
		//constexpr // hash_code() isn't actually constexpr.
		std::size_t operator() (std::type_index const& aTi) const noexcept
		{
			// Only noexcept since C++14.
			return aTi.hash_code();
		}
	};

	struct EndpointData_
	{
		Action<Queue> fallback;

		//flux_ext::node_hash_map<std::type_index, Action<Queue>> typed;
		flux_ext::node_hash_map<
			std::type_index, 
			Action<Queue>,
			HashTypeIndex_
		> typed;
	};

	using QueuePtr_ = Queue*;
}

//--    d: gDefaultQueueAutoconf            ///{{{2///////////////////////////
namespace detail
{
	using FPtr_ = void (*)(void*, ftl::Catalog&);

	ftl::CatalogKeyAutoconf const gProcessMainQueueAutoconf = {
		base::kProcessCatalog,
		sizeof(Queue*),
		static_cast<FPtr_>([] (void* p, ftl::Catalog&) { new (p) QueuePtr_(&thread_catalog().retain<ThreadDefaultQueue>()); }),
		static_cast<FPtr_>([] (void*, ftl::Catalog&) { thread_catalog().dismiss<ThreadDefaultQueue>(); })
	};

	ftl::CatalogKeyAutoconf const gDefaultThreadQueueAutoconf = {
		base::kThreadCatalog,
		sizeof(Queue),
		static_cast<FPtr_>([] (void* p, ftl::Catalog&) { new (p) Queue(); }),
		static_cast<FPtr_>([] (void* p, ftl::Catalog&) { reinterpret_cast<Queue*>(p)->~Queue(); })
	};
}

//--    Queue::Self_                        ///{{{2///////////////////////////
struct Queue::Self_
{
	unique_ptr<QueueSource> primary;
	flux_ext::ConcurrentQueue<Message> queue;
	//flux_ext::BlockingConcurrentQueue<Message> queue;

	flux_ext::node_hash_map<Endpoint,EndpointData_> origins;
	flux_ext::node_hash_map<Endpoint,EndpointData_> destinations;

	//flux_ext::node_hash_map<std::type_index, Action> typed;
	flux_ext::node_hash_map<
		std::type_index, 
		Action,
		HashTypeIndex_
	> typed;

	Action fallback;
};

//--    Queue                               ///{{{2///////////////////////////

	// [cd]tor -{{{3-
Queue::Queue()
	: self( make_unique<Self_>() )
{
	self->primary = default_primary_source();
}

Queue::~Queue() = default;


	// public -{{{3-
bool Queue::pump( QueuePump::Blocking )
{
	bool exit = false;
	bool msg = pump_queued_( exit );
	
	bool pending = false;
	if( !msg ) pending = self->primary->wait();
	if( pending || msg ) pump_source_( exit );
	
	pump_queued_( exit );
	return !exit;
}
bool Queue::pump( QueuePump::Nonblocking )
{
	bool exit = false;
	pump_queued_( exit );
	pump_source_( exit );
	pump_queued_( exit );
	return !exit;
}


void Queue::queue( Message&& aMessage, bool aWake )
{
	self->queue.enqueue( std::move(aMessage) );
	if( aWake ) self->primary->interrupt();
}
void Queue::queue( Message const& aMessage, bool aWake )
{
	self->queue.enqueue( aMessage );
	if( aWake ) self->primary->interrupt();
}


auto Queue::action( std::type_index aTy ) -> Action& 
{
	return self->typed[aTy];
}

auto Queue::action_for_origin( Endpoint aEp ) -> Action&
{
	return self->origins[aEp].fallback;
}
auto Queue::action_for_origin( Endpoint aEp, std::type_index aTy ) -> Action&
{
	return self->origins[aEp].typed[aTy];
}

auto Queue::action_for_dest( Endpoint aEp ) -> Action&
{
	return self->destinations[aEp].fallback;
}
auto Queue::action_for_dest( Endpoint aEp, std::type_index aTy ) -> Action&
{
	return self->destinations[aEp].typed[aTy];
}

auto Queue::action_fallback() -> Action&
{
	return self->fallback;
}

QueueSource* Queue::primary_source() const noexcept
{
	return self->primary.get();
}
unique_ptr<QueueSource> Queue::replace_primary_source( unique_ptr<QueueSource> aNew ) noexcept
{
	std::swap( self->primary, aNew );
	return aNew;
}

void Queue::interrupt() const
{
	self->primary->interrupt();
}

void Queue::forget_origin( Endpoint aEp )
{
	self->origins.erase( aEp );
}
void Queue::forget_destination( Endpoint aEp )
{
	self->destinations.erase( aEp );
}
void Queue::forget_type( std::type_index aTy )
{
	for( auto it = self->origins.begin(); it != self->origins.end(); ++it )
		it->second.typed.erase( aTy );

	for( auto it = self->destinations.begin(); it != self->destinations.end(); ++it )
		it->second.typed.erase( aTy );

	self->typed.erase( aTy );
}

void Queue::compact()
{
	for( auto it = self->origins.begin(); it != self->origins.end(); )
	{
		auto& tys = it->second.typed;
		for( auto jt = tys.begin(); jt != tys.end(); )
		{
			if( jt->second.empty() ) jt = tys.erase( jt );
			else ++jt;
		}

		if( tys.empty() && it->second.fallback.empty() )
			it = self->origins.erase( it );
		else
			++it;
	}

	for( auto it = self->destinations.begin(); it != self->destinations.end(); )
	{
		auto& tys = it->second.typed;
		for( auto jt = tys.begin(); jt != tys.end(); )
		{
			if( jt->second.empty() ) jt = tys.erase( jt );
			else ++jt;
		}

		if( tys.empty() && it->second.fallback.empty() )
			it = self->destinations.erase( it );
		else
			++it;
	}

	{
		auto& tys = self->typed;
		for( auto jt = tys.begin(); jt != tys.end(); )
		{
			if( jt->second.empty() ) jt = tys.erase( jt );
			else ++jt;
		}
	}
}

	// private -{{{3-
bool Queue::pump_queued_( bool& aExit )
{
	Message buffer[kDefaultDequeue_];

	bool cont = true, msg = false;
	std::size_t count;
	do
	{
		count = self->queue.try_dequeue_bulk( buffer, kDefaultDequeue_ );
		for( std::size_t i = 0; i < count; ++i )
			cont = route_( buffer[i] ) && cont;

		if( count ) msg = true;
	} while( count != 0 ); // single message received ... might have queued a new one

	if( !cont )
		aExit = true;

	return msg;
}
bool Queue::pump_source_( bool& aExit )
{
	auto* src = self->primary.get();
	
	bool cont = true, msg = false;

	std::size_t count;
	Message buffer[kDefaultDequeue_];
	do
	{
		count = src->dequeue( buffer, kDefaultDequeue_ );
		msg = (count > 0) || msg;

		for( std::size_t i = 0; i < count; ++i )
			cont = route_( buffer[i] ) && cont;
	} while( kDefaultDequeue_ == count );
	
	if( !cont )
		aExit = true;
	
	return msg;
}

bool Queue::route_( Message const& aMsg )
{
	auto const tid = aMsg.msg.type_index();

	EOutcome res = EOutcome::ignored;

	// origins
	if( aMsg.org )
	{
		auto const& orgs = self->origins;
		auto const it = orgs.find( aMsg.org );
		if( it != orgs.end() )
		{
			auto const& tys = it->second.typed;
			auto const jt = tys.find( tid );

			EOutcome oc = EOutcome::ignored;
			if( jt != tys.end() )
				oc = jt->second.emit( aMsg, *this );

			if( EOutcome::discard == oc )
				return true;
				
			EOutcome pc = it->second.fallback.emit( aMsg, *this );
			if( EOutcome::discard == pc )
				return true;

			res = (EOutcome::ignored == pc) ? oc : pc;
		}
	}

	// destinations
	auto const& dests = self->destinations;
	auto const it = dests.find( aMsg.dest );
	if( it != dests.end() )
	{
		auto const& tys = it->second.typed;
		auto const jt = tys.find( tid );

		EOutcome oc = EOutcome::ignored;
		if( jt != tys.end() )
			oc = jt->second.emit( aMsg, *this );

		if( EOutcome::discard == oc )
			return true;
			
		EOutcome pc = it->second.fallback.emit( aMsg, *this );
		if( EOutcome::discard == pc )
			return true;

		auto const t = (EOutcome::ignored == pc) ? oc : pc;
		if( EOutcome::ignored != t )
			res = t;
	}

	// generic handlers
	if( EOutcome::ignored == res )
	{
		auto const& tys = self->typed;
		auto const it = tys.find( tid );

		EOutcome oc = EOutcome::ignored;
		if( it != tys.end() )
			oc = it->second.emit( aMsg, *this );

		if( EOutcome::discard == oc )
			return true;

		if( EOutcome::ignored == oc )
		{
			res = self->fallback.emit( aMsg, *this );
		}
	}

	if( EOutcome::discard == res )
		return true;

	if( aMsg.msg.is<msg::EQuit>() )
		return false;

	return true;
}

//--    QueueSource                  ///{{{2///////////////////////////
QueueSource::~QueueSource() = default;

ftl::Any QueueSource::native_source_handle()
{
	return {};
}

//--    default_primary_source()            ///{{{2///////////////////////////
namespace 
{
	class DefaultPrimary_ final : public QueueSource
	{
		public:
			bool wait() override;
			void interrupt() override;

			std::size_t dequeue( Message*, std::size_t ) override;

		private:
			std::mutex mMut;
			std::condition_variable mCond;

			unsigned mPending = 0;
	};

	bool DefaultPrimary_::wait()
	{
		std::unique_lock<std::mutex> lock(mMut);
		if( !mPending )
			mCond.wait( lock );

		--mPending;
		return false;
	}
	void DefaultPrimary_::interrupt() 
	{
		{
			std::unique_lock<std::mutex> lock(mMut);
			++mPending;
		}
		mCond.notify_one();
	}

	std::size_t DefaultPrimary_::dequeue( Message* aMsgBuffer, std::size_t )
	{
		FLUX_ASSERT( aMsgBuffer );
		(void)aMsgBuffer;
		return 0;
	}
}

unique_ptr<QueueSource> default_primary_source()
{
	return make_unique<DefaultPrimary_>();
}

//--    <<< ~ flux::base namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(base)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
