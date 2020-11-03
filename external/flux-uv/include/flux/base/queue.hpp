/*-******************************************************* -- HEADER -{{{1- */
/*-	Queue
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_BASE_QUEUE_HPP_2A5D1214_BD08_49B2_9D73_EA521078A7AF
#define FLUX_BASE_QUEUE_HPP_2A5D1214_BD08_49B2_9D73_EA521078A7AF

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/identity.hpp>
#include <flux/std/unique_ptr.hpp>

#include <flux/ftl/signal.hpp>
#include <flux/ftl/catalog_key.hpp>

#include <typeindex>

#include "action.hpp"
#include "message.hpp"

//--    >>> namespace = flux::base >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(base)
//--    types                               ///{{{2///////////////////////////

struct QueuePump
{
	enum Blocking { blocking = 0 };
	enum Nonblocking { nonblocking = 1 };
};

struct QueueSource;

//--    Queue                               ///{{{2///////////////////////////
/** Message queue
 *
 * A message queue, intended to be used for routing messages from the OS (see 
 * e.g. `flux::os::Context`) and potentially between threads.
 *
 * `Queue` receives and routes messages from two sources:
 *   - messages manually entered into the queue using the `queue()` method
 *   - messages received via a primary source (`QueueSource`)
 * Each `Queue` has at most a single primary source. The intented use of the
 * primary source is to provide a mechanism to receive from a OS specific
 * message source (e.g.  `GetMessage` / `PeekMessage` (Win32), `XNextEvent` /
 * `XPeekEvent` (X11), `xcb_wait_for_event()` / `xcb_poll_for_event()` etc.).
 *
 * `Message`s are processed by calling `pump()`. `pump()` will route messages
 * from the primary source and manually queued messages to specified `Action`s.
 * `Action`s may be registered for 
 *   - a specific origin and specific message type
 *   - a specific origin
 *   - a specific destination and specific message type
 *   _ a specific destination
 *   - a specific message type only(*)
 *   - any message(*)
 * Messages are routed to the above in the order listed. At each level, an
 * `Action` can stop further routing of the message by returning
 * `EOutcome::discard`. Returning `EOutcome::ignored` causes routing to
 * continue to other matching `Action`s (the same as if no matching `Action`s
 * were found at an earlier level). `EOutcome::proceed` causes routing to
 * continue until (and including) specific destinations, but to the last two
 * levels (marked with (*)).
 *
 * Binding `Action`s to an origin (and optionally message type) may be used to
 * translate messages. In this case, the `Action` can `queue()` new messages
 * and return `EOutcome::discard` to stop processing of the current message.
 *
 * The Queue is <b>not</b> thread safe, except for the
 *  - `queue()`
 *  - `interrupt()`
 * methods. These methods may be called from a different thread without extra
 * synchronization. (Care should be taken when sharing a `Queue` between 
 * different threads, even with synchronization, as the primary source may be
 * thread-specific, and actions may not expect being called from a different
 * thread than the one they were originally created in.)
 *
 * \note `Queue` should not be used for high-volume (inter-thread)
 * communication. In that case, using `flux_ext::ConcurrentQueue` or
 * `flux_ext::BlockingConcurrentQueue` directly probably provides a
 * better-performing alternative. `Queue` is implemented on top of these data
 * structures.
 */
class Queue final
{
	public:
		Queue(), ~Queue();

		Queue( Queue const& ) = delete;
		Queue& operator= (Queue const&) = delete;

	public:
		using Action = base::Action<Queue>;

	public:
		
		/** Pump messages
		 *
		 * Route `queue()`d messages, and messages from the primary source.
		 * The non-blocking (`QueuePump::nonblocking`) version will return
		 * immediately if no messages are available for processing. The
		 * blocking (`QueuePump.:blocking`) version will wait for messages
		 * <b>on the primary source</b> to become available, or until woken by
		 * either calling `interrupt()` or `queue()`ing a message with the \a
		 * aInterrupt parameter set to true.
		 *
		 * `pump()` returns true unless it has encountered a `msg::EQuit`
		 * message, in which case it will return false.
		 */
		bool pump( QueuePump::Blocking = QueuePump::blocking /*TODO timeout*/ );
		/** \copydoc pump */
		bool pump( QueuePump::Nonblocking );
		
		//TODO-maybe: pump() with lambda that receives all messages
		// first.


		/** Queue message
		 *
		 * Queue message, optionally `interrupt()`ing a blocking operation if 
		 * \a aInterrupt is set to true.
		 *
		 * `queue()` is thread safe, and may be called from a different thread
		 * without synchronization.
		 *
		 * \see interrupt
		 */
		void queue( Message&&, bool aInterrupt = false );
		/** \copydoc queue */
		void queue( Message const&, bool aInterrupt = false );
		// queue() with bulk argument...?
		
		Action& action( std::type_index ); // XXX-noexcept?

		Action& action_for_origin( Endpoint );
		Action& action_for_origin( Endpoint, std::type_index );

		Action& action_for_dest( Endpoint );
		Action& action_for_dest( Endpoint, std::type_index );

		Action& action_fallback();


		template< typename tType >
		Action& action( Identity<tType> );
		template< typename tType >
		Action& action_for_origin( Endpoint, Identity<tType> );
		template< typename tType >
		Action& action_for_dest( Endpoint, Identity<tType> );


		QueueSource* primary_source() const noexcept;
		unique_ptr<QueueSource> replace_primary_source( unique_ptr<QueueSource> ) noexcept;

		//TODO: add secondary sources
		// Won't wait() on those, but still dequeue() in the right phase

		/** Interrupt blocking wait
		 *
		 * Interrupt a blocking call to `pump()`. It is safe to call
		 * `interrupt()` from a different thread.
		 */
		void interrupt() const;

		/** Forget all actions bound to a certain origin
		 *
		 * Forget all actions that are bound to the origin \a Endpoint. This is
		 * useful when a certain origin ceases to produce messages.
		 */
		void forget_origin( Endpoint );
		/** Forget all actions bound to a certain destination
		 *
		 * Forget all actions that are bound to the origin \a Endpoint. This is
		 * useful when a certain origin ceases to produce messages.
		 */
		void forget_destination( Endpoint );
		/** Forget all actions bound to a certain message type
		 *
		 * Forget all actions that are bound to the specified message type.
		 * This is useful when a certain type of message is no longer expected
		 * to occur.
		 */
		void forget_type( std::type_index );
		
		/** \copydoc forget_type */
		template< typename tType >
		void forget_type( Identity<tType> );

		/** Maintenance: free unused memory
		 *
		 * Free up unused resources used for internal book-keeping.
		 */
		void compact();

	private:
		bool pump_queued_( bool& );
		bool pump_source_( bool& );

		bool route_( Message const& );
		
	private:
		struct Self_;
		unique_ptr<Self_> const self;
};

//--    QueueSource                  ///{{{2///////////////////////////
struct QueueSource
{
	virtual ~QueueSource() = 0;

	virtual bool wait() = 0; //TODO: with timeout
	virtual void interrupt() = 0;

	virtual std::size_t dequeue( Message*, std::size_t ) = 0;

	virtual ftl::Any native_source_handle();
};

//--    DefaultQueue                        ///{{{2///////////////////////////

namespace detail
{
	extern ftl::CatalogKeyAutoconf const gProcessMainQueueAutoconf;
	extern ftl::CatalogKeyAutoconf const gDefaultThreadQueueAutoconf;
}

/** Key: Default thread queue
 *
 * Key identifying the default thread queue. The `Queue` instance may be 
 * retrieved from the `thread_catalog()`:
   \code
   Queue& q = thread_catalog().acquire<ThreadDefaultQueue>();
   ...
   \endcode
 */
using ThreadDefaultQueue = ftl::CatalogKey<
	Queue,
	Queue&,
	struct DefaultQueueKeyTag_,
	ftl::ECatalogKeyKind::value,
	detail::gDefaultThreadQueueAutoconf
>;

/** Kay: Process' main queue
 *
 * Key identifying the process' main queue. 
 *
 * By default, the `ProcessMainQueue` will be set to the `ThreadDefaultQueue`
 * of the thread first acquiring the `ProcessMainQueue`. It is therefore
 * suggested to call e.g.
   \code
   process_catalog().acquire<ProcessMainQueue>()
   \endcode
 * in the main event handling thread at startup. The `ProcessMainQueue` can be
 * reconfigured (`ftl::Catalog::configure`) to an arbitrary `Queue*`.
 */
using ProcessMainQueue = ftl::CatalogKey<
	Queue*,
	Queue&,
	struct ProcessMainQueueKeyTag_,
	ftl::ECatalogKeyKind::value,
	detail::gProcessMainQueueAutoconf
>;

//--    functions                           ///{{{2///////////////////////////

unique_ptr<QueueSource> default_primary_source();

//--    <<< ~ flux::base namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(base)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/queue.inl"
#endif // FLUX_BASE_QUEUE_HPP_2A5D1214_BD08_49B2_9D73_EA521078A7AF
