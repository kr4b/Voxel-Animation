/*-******************************************************* -- HEADER -{{{1- */
/*-	System Information
 *
 * TODO - WIP
 *
 * TODO: remove "refresh" parameters from various functions, and instead
 *     get a function that invalidates the currently chached values.
 *     (Invalidate if some functions detect inconsistecies, e.g., set affinity
 *     fails when it shouldn't?)
 *
 * TODO: CPU flags strings?
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_INFO_HPP_02422695_08E1_4CB3_A8A6_74A993416F45
#define FLUX_SYS_INFO_HPP_02422695_08E1_4CB3_A8A6_74A993416F45

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/span.hpp>
#include <flux/compat/noexcept.hpp>
#include <flux/compat/shared_mutex.hpp>

#include <flux/util/attrib.hpp>
#include <flux/sys/execctrl.hpp> // LogicalCoreId, PhysicalCoreId

#include <string>
#include <cstddef>

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    types                       ///{{{1///////////////////////////////////

/* TODO: note should not be held for a long time => has a mutex.
 */
class CpuTopology
{
	using UnspecifiedBoolType_ = bool (CpuTopology::*)() const FLUX_TYPEDEF_NOEXCEPT;

	public:
		~CpuTopology();

		CpuTopology( CpuTopology const& ) = delete;
		CpuTopology& operator= (CpuTopology const&) = delete;

		CpuTopology( CpuTopology&& ) noexcept;
		CpuTopology& operator= (CpuTopology&&) noexcept;

	public:
		operator UnspecifiedBoolType_() const noexcept { 
			return valid() ? &CpuTopology::valid : nullptr;
		}

	public:
		/** Is this CPU topology valid?
		 *
		 * A CPU topology is valid if the following could be determined
		 *   - the number of logical CPUs
		 *   - the number of enabled/accessible CPUs, and the corresponding
		 *     logical CPU IDs
		 *   - the number of physical CPUS, their IDs, and the mapping between
		 *     logical CPU IDs and physical core indices
		 */
		bool valid() const noexcept;
	
		/** Return number of logical CPUs
		 *
		 * Returns the number of logical CPUs in the system. This is equal to
		 * the number returned by `get_logical_cpu_count()`. If the number
		 * cannot be determined, `logical_count()` returns zero.
		 * 
		 * \see get_logical_cpu_count()
		 */
		std::size_t logical_count() const noexcept;
		/** Return number of enabled CPUs
		 *
		 * Returns the number of CPUs that are enabled and usable by the
		 * calling process. Under normal conditions, this is equal to the
		 * number of logical CPUs (see `enabled()` for notes). If it's not
		 * possible to determine this number, `enabled_count()` returns zero.
		 *
		 * \see enabled()
		 */
		std::size_t enabled_count() const noexcept;
		/** Return number of physical CPU cores
		 *
		 * Returns the number of physical CPU cores. The number of physical CPU
		 * cores is equal to or less than the number of logical CPUs, depending
		 * on whether or not the CPU supports hyperthreading (or equivalent
		 * technologies). If it's not possible to determine this number,
		 * `enabled_count()` returns zero.
		 *
		 * \note The physical core count may be inaccurate if not all logical
		 * CPUs are accessible to the current process. In particular, the
		 * physical core count is equal to the number of unique physical core
		 * IDs seen by the accessible CPUs.
		 *
		 * \warning Physical core IDs are not assigned consecutively, i.e.,
		 * there can be physical core IDs that are larger than the number
		 * returned by `physical_count()`.
		 *
		 * \see logical_to_physical(), physical_to_logical()
		 */
		std::size_t physical_count() const noexcept;

		/** Return logical core IDs of enabled CPUs
		 *
		 * Returns a list of logical core IDs of currently enabled CPUs. The
		 * returned list is under normal conditions equal to \f$\left[0 ...
		 * \mbox{logical_count}\right)\f$. If it is not possible to determine
		 * the enabled CPUs, an empty list is returned.
		 *
		 * \note The number of enabled CPUs may diverge from the number of
		 * logical CPUs/cores in some cases. For example, using Linux cgroups,
		 * processes can be restricted to specific CPUs. In this case, the
		 * enabled CPU count reflects this. Flux-sys uses `SetThreadAffinity()`
		 * under Windows, i.e., it does not modifiy the process affinity mask.
		 * Therefore, CPUs may be "disabled" by setting the process affinity
		 * (e.g. `start /affinity <hexmask> ...`) to a subset of the system's
		 * CPUs.
		 */
		compat::span<LogicalCoreId const> enabled() const noexcept;

		//TODO: is_enabled()? To check if a logical core is enabled.

		/** Get physical core index of a logical CPU
		 * 
		 * Return the physical core index to which the \a LogicalCoreId logical
		 * CPU belongs. Returns `~PhysicalCoreId(0)` if the corresponding
		 * physical core index cannot be determined.
		 *
		 * \note Establishing the mapping between logical and physical CPUs may
		 * require the corresponding logical cores to be enabled and usable.
		 * Therefore `logical_to_physical()` may return `~PhysicalCoreId(0)`
		 * for inaccessible logical CPUs.
		 *
		 * \warning Physical core IDs are not assigned consecutively and are
		 * determined by the system.
		 */
		PhysicalCoreId logical_to_physical( LogicalCoreId ) const noexcept;
		/** Get a logical CPU ID from physical core index
		 * 
		 * Return `LogicalCoreId` for a logical CPU that belongs to the
		 * physical core specified by \a PhysicalCoreId. Returns
		 * `~LogicalCoreId(0)` if no corresponding logical CPU has been
		 * determined (e.g., no logical CPU of the physical core is
		 * accessible).
		 *
		 * \warning Physical core IDs are not assigned consecutively and are
		 * determined by the system.
		 */
		LogicalCoreId physical_to_logical( PhysicalCoreId ) const noexcept;
		
		//TODO: naming
		compat::span<LogicalCoreId const> physical_to_logicals( PhysicalCoreId ) const noexcept;

	private:
		using SharedLock_ = compat::shared_lock<compat::shared_mutex>;

		explicit CpuTopology( SharedLock_ ) noexcept;
		friend CpuTopology get_cpu_topology() noexcept;

	private:
		compat::shared_lock<compat::shared_mutex> mLock;
};

//--    constants                   ///{{{1///////////////////////////////////

//constexpr std::size_t kUnknownValue = ~std::size_t(0);

//--    functions                   ///{{{1///////////////////////////////////

	// System CPUs
std::size_t get_logical_cpu_count() noexcept;

CpuTopology get_cpu_topology() noexcept;

//TODO-docs refresh mustn't be called by a thread currently holding a 
//   CpuTopology object. ==> deadlock.
void cpu_topology_refresh() noexcept;
void cpu_togology_invalidate() noexcept;

FLUX_DEPRECATED_EX( "Use get_{enabled,logical,physical}_core_count()" )
std::size_t get_hw_concurrency() noexcept;

	// System memory
std::size_t get_page_size() noexcept;
std::size_t get_physical_mem() noexcept;
std::size_t get_available_mem( bool aIncludeCaches = true ) noexcept;
//std::size_t get_virtual_mem() noexcept;

	// System state
std::size_t get_uptime() noexcept;

	// System strings
std::string get_sys_name();
std::string get_cpu_name();
std::string get_host_name();

/** Identifier for architecture
 *
 * Returns identifier for the current target architecture, based on the value
 * of `FLUX_ARCH` and related values. Values include:
 *  - `arm<N>`: ARM version N (e.g., `arm7`)
 *  - `arm<N>hfp`: ARM version N, with hardfloat ABI (e.g., `arm7hfp`)
 *  - `arm64`: ARM64
 *  - `x86`: x86, 32-bit
 *  - `x64`: x86, 64-bit
 *  - `unknwon` : unknown value of `FLUX_ARCH`
 */
std::string get_arch_ident();
/** Identifier for OS
 *
 * Returns identifier for operating system, based on the value of
 * `FLUX_PLATFORM` and possible runtime-reported values. Values include:
 *  - `windows`: Windows (`FLUX_PLATFORM_WIN32`)
 *  - `android`: Android (`FLUX_PLATFORM_ANDROID`)
 *  - `linux`: Linux (`FLUX_PLATFORM_LINUX`, based on `::uname()`)
 *  - `cygwin`: Cygwin (`FLUX_PLATFORM_LINUX`, based on `::uname()`)
 *  - `unknown`: Unknown value of `FLUX_PLATFORM`
 *
 * TODO: cygwin? should we return cygwin? or just windows?
 */
std::string get_os_ident();
// TODO get_os_version();

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_INFO_HPP_02422695_08E1_4CB3_A8A6_74A993416F45
