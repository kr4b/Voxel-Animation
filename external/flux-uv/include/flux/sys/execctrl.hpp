/*-******************************************************* -- HEADER -{{{1- */
/*-	(Thread) execution control
 *
 * Thread affinity, priority, and information of CPU topology (currently
 * physical cores vs logical, not sockets and other super fancy stuff).
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_EXECCTRL_HPP_98C58182_716E_4165_805D_D240174F266B
#define FLUX_SYS_EXECCTRL_HPP_98C58182_716E_4165_805D_D240174F266B

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/platform.hpp>

#include <flux/std/function.hpp>
#include <flux/std/inttypes.hpp>

#include <flux/compat/system_error.hpp>

#include <flux/util/attrib.hpp>

#include <flux_ext/phmap_fwd_decl.hpp> //XXX

#include <cstddef>

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)

//--    types                       ///{{{1///////////////////////////////////

using LogicalCoreId = uint32_t;
using PhysicalCoreId = uint32_t;

class FLUX_ATTR_TRIVIAL_ABI ThreadHandle final
{
	public:
		ThreadHandle(), ~ThreadHandle();

		ThreadHandle( ThreadHandle const& ) = delete;
		ThreadHandle& operator= (ThreadHandle const&) = delete;

		ThreadHandle( ThreadHandle&& ) noexcept;
		ThreadHandle& operator= (ThreadHandle&&) noexcept;

		ThreadHandle( std::nullptr_t ) noexcept;
		ThreadHandle& operator= (std::nullptr_t) noexcept;

		ThreadHandle( void* ) noexcept;
		ThreadHandle& operator= (void*) noexcept;

	public:
		void* opaque() const;

	private:
		void* mOpaqueHandle;
};

//XXX
using LogicalCoreSet = flux_ext::flat_hash_set<LogicalCoreId>;

//--    constants                   ///{{{1///////////////////////////////////

//TODO
//constexpr LogicalCoreId kInvalidLogicalCore = ~LogicalCoreId(0);
//constexpr PhysicalCoreId kInvalidPhysicalCore = ~PhysicalCoreId(0);

/** Thread priorities
 * 
 * The thread priorities roughly correspond to the Win32 options:
 *  - `normal` == `THREAD_PRIORITY_NORMAL`
 *  - `low` == `THREAD_PRIORITY_BELOW_NORMAL`
 *  - `lower` == `THREAD_PRIORITY_LOWEST`
 *  - `high` == `THREAD_PRIORITY_ABOVE_NORMAL`
 *  - `higher` == `TRHEAD_PRIORITY_HIGHEST`
 *  - `idle` == `THREAD_PRIORITY_IDLE`
 *  - `critical` == `THREAD_PRIORITY_TIME_CRITICAL`
 *
 * Under pthreads/Linux these are interpreted as follows:
 *  - `normal`, `low`, `lower`, `high`, `higher` use the `SCHED_OTHER` policy.
 *    Under Linux, settings different from `normal` additionally adjust the
 *    per-thread nice value. Under non-Linux pthreads, the listed settings are
 *    equivalent. 
 *  - `idle` uses the `SCHED_IDLE` policy.
 *  - `critical` uses the `SCHED_FIFO` with a priority of 1. This may fail if
 *    the calling process has insufficient priviledges.
 *
 * \note Linux-specific per-thread nice values are as follows:
 *  - `lower`: uses an arbitrarily chosen nice value of 12
 *  - `low`: uses an arbitrarily chosen nice value of 6
 *  - `normal`: nice = 0
 *  - `higher`: uses a nice value corresponding to lowest permitted value, as
 *    determined by RLIMIT_NICE (but at most zero).
 *  - `high`: uses a nice value equal to half of that of higher
 *
 * The `unidentified` value is returned by `thread_get_priority()` if the
 * values reported by the OS cannot be translated into one of the listed
 * priorities.
 *
 * \warning `critical` priority should be used sparingly and carefully. The
 * documentation for Windows mentions that a real-time thread preempts even OS
 * threads, which can prevent disk caches to not flush or mouse to be
 * unresponsive. Under Linux, a `SCHED_FIFO` can hog CPU resources indefinitely
 * and prevent any other threads from running (although there is some options
 * mitigate that via administrative configuration).
 *
 * TODO-maybe: deadline priority. Seems fun. Windows doesn't have it.
 */
enum class EThreadPriority
{
	normal,

	low,
	lower,
	high,
	higher,

	idle,
	critical,

	unidentified
};


//--    functions                   ///{{{1///////////////////////////////////

// Threads
ThreadHandle thread_self() noexcept;
// ThreadHandle thread_from_std_thread( std::thread const& ) noexcept;

LogicalCoreSet thread_get_affinity( 
	ThreadHandle = nullptr, 
	compat::error_code* = nullptr
) noexcept;
/** Set thread affinity
 *
 * Set the affinity of \a ThreadHandle to the cores specified in \a
 * LogicalCoreSet or to the single core \a LogicalCoreId. If \a ThreadHandle is
 * set to `nullptr`, the function will affect the calling thread. If the 
 * affinity of the calling thread is changed successfully, the thread is
 * immediately migrated to one of the specified cores.
 *
 * On success, returns true. On failure, returns false and optionally sets \a
 * compat::error_code.
 */
bool thread_set_affinity( 
	LogicalCoreSet const&,
	ThreadHandle = nullptr,
	compat::error_code* = nullptr
) noexcept;
/** \copydoc thread_set_affinity */
bool thread_set_affinity( 
	LogicalCoreId,
	ThreadHandle = nullptr,
	compat::error_code* = nullptr
) noexcept;

/** Get thread priority
 *
 * Get the current priority setting of \a ThreadHandle. If \a ThreadHandle is
 * set to `nullptr`, the function will affect the calling thread. Returns a
 * value from the `EThreadPriority` enumeration.
 *
 * Since `EThreadPriority` only exposes a subset of the possible configurations
 * supported by the underlying OS, `thread_get_priority()` will return the
 * special value `EThreadPriority::unidentified` if it cannot map the internal
 * state to a know value.
 *
 * \note Under Windows, a process priority class other than the default
 * `NORMAL_PRIORITY_CLASS` will cause `thread_set_priority()` to always return
 * `EThreadPriority::unidentified`, even after setting the thread priority with
 * `thread_set_priority()` (as this does not change the process' priority
 * class). Under Linux, only a limited subset of "nice" values is handled, and
 * all others result in `unidentified`.
 */
EThreadPriority thread_get_priority( ThreadHandle = nullptr ) noexcept;

/** Set thread priority
 *
 * Set priority of \a ThreadHandle to one of the options enumerated by \a
 * EThreadPriority. If \a ThreadHandle is set to `nullptr`, the function will
 * affect the calling thread. On success, returns true; on failure, returns
 * false and optionally returns error information via \a compat::error_code.
 *
 * Increasing thread priorities may fail (and typically does under Linux,
 * unless the calling process has special permissions/resource limits applied
 * to it). Lowering thread priority is generally successful.
 *
 * \note Increasing priority may fail even if a thread's priority is first
 * successfully lowered via `thread_set_priority()` and later just increased to
 * restore the original priority.
 *
 * Calling `thread_set_priority()` with `EThreadPriority::unidentified` always
 * fails.
 *
 * \warning Under Windows, `thread_set_priority()` just sets the thread
 * priority. The meaning of this is dependent on the priority class of the
 * process (the priority class is left unchanged).
 * 
 * \see EThreadPriority
 */
bool thread_set_priority( 
	EThreadPriority, 
	ThreadHandle = nullptr, 
	compat::error_code* = nullptr
) noexcept;

/** Return logical core we're currently running on
 *
 * Returns the logical core index that the calling thread is currently running
 * on. Note that this information may be outdated by the time the call returns
 * (as the thread may have migrated).
 *
 * Returns `~LogicalCoreId(0)` on error and (optionally) sets the provided error
 * code appropriately.
 */
LogicalCoreId thread_current_logical_cpu( compat::error_code* = nullptr ) noexcept;

// Funky helpers
std::size_t exec_for_each_enabled_cpu( 
	unique_function<void(LogicalCoreId)> 
);

std::size_t exec_for_each_logical_cpu( 
	unique_function<void(LogicalCoreId)>,
	unique_function<void(LogicalCoreId)> = {}
);
std::size_t exec_for_each_physical_cpu( 
	unique_function<void(PhysicalCoreId, LogicalCoreId)>,
	unique_function<void(PhysicalCoreId)> = {}
);

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_EXECCTRL_HPP_98C58182_716E_4165_805D_D240174F266B
