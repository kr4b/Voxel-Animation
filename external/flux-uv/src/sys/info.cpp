/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	System Info
 *
 * TODO: Win32 vs Cygwin? Linux vs Cygwin?
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/info.hpp>

#include <flux/setup/arch.hpp>
#include <flux/setup/platform.hpp>
#include <flux/setup/stdfeat.hpp>

#include <flux/std/log.hpp>
#include <flux/std/assert.hpp>

#include <flux/compat/constexpr.hpp>
#include <flux/compat/shared_mutex.hpp>

#include <flux/util/narrow.hpp>
#include <flux/util/scope_exit.hpp>
#include <flux/util/bit_twiddle.hpp>

#include <flux/ftl/vector.hpp>
#include <flux/sys/execctrl.hpp>

#include <flux_ext/phmap.hpp>

#include "x86_cpuid.hxx"

#include <mutex>
#include <atomic>
#include <thread>
#include <sstream>
#include <algorithm>

#include <cstdio>
#include <cstring>
#include <cstdlib>

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#	include <unistd.h>
#else // PLATFORM_WIN32
#	include <flux_dlapi/os/win32.hpp>
#endif // ~ PLATFORM

#if FLUX_PLATFORM == FLUX_PLATFORM_LINUX || FLUX_PLATFORM == FLUX_PLATFORM_ANDROID
#	include <sys/sysinfo.h>
#endif // ~ PLATFORM

#if FLUX_PLATFORM == FLUX_PLATFORM_LINUX || FLUX_PLATFORM == FLUX_PLATFORM_ANDROID || FLUX_PLATFORM == FLUX_PLATFORM_MACOS
#	define FLUXCFG_WITH_UNAME_ 1
#	include <sys/utsname.h>
#endif // ~ PLATFORM

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)

//--    $ local                             ///{{{2///////////////////////////
namespace
{
	struct TopoInfo_
	{
		std::size_t logicalCoreCount = 0, physicalCoreCount = 0;
		ftl::Vector<PhysicalCoreId,64*sizeof(PhysicalCoreId)> logicalToPhysicalId;
		ftl::Vector<LogicalCoreId,64*sizeof(LogicalCoreId)> physicalToLogicalId;

		ftl::Vector<LogicalCoreId,64*sizeof(LogicalCoreId)> enabledLogicalCores;
	} gTopo_;

	compat::shared_mutex gTopoMut_;
	std::atomic<bool> gTopoValid_{0};

	void query_enabled_cores_() noexcept;

#	if FLUX_SYS_WITH_X86CPUID
	bool x86_query_cputopo_() noexcept;
#	endif // ~ WITH_X86CPUID
}

//--    CpuTopology                         ///{{{2///////////////////////////
CpuTopology::CpuTopology( SharedLock_ aLock ) noexcept
	: mLock( std::move(aLock) )
{}

CpuTopology::~CpuTopology() = default;

CpuTopology::CpuTopology( CpuTopology&& aTopo ) noexcept
	: mLock( std::move(aTopo.mLock) )
{}
CpuTopology& CpuTopology::operator= (CpuTopology&& aTopo) noexcept
{
	std::swap( mLock, aTopo.mLock );
	return *this;
}

bool CpuTopology::valid() const noexcept
{
	return 0 != gTopo_.logicalCoreCount
		&& !gTopo_.enabledLogicalCores.empty()
		&& !gTopo_.physicalToLogicalId.empty()
	;
}

std::size_t CpuTopology::logical_count() const noexcept
{
	return gTopo_.logicalCoreCount;
}
std::size_t CpuTopology::enabled_count() const noexcept
{
	return gTopo_.enabledLogicalCores.size();
}
std::size_t CpuTopology::physical_count() const noexcept
{
	return gTopo_.physicalCoreCount;
}

compat::span<LogicalCoreId const> CpuTopology::enabled() const noexcept
{
	return compat::span<LogicalCoreId const>(
		gTopo_.enabledLogicalCores.data(),
		gTopo_.enabledLogicalCores.size()
	);
}

PhysicalCoreId CpuTopology::logical_to_physical( LogicalCoreId aCpu ) const noexcept
{
	if( aCpu >= gTopo_.logicalToPhysicalId.size() )
		return ~PhysicalCoreId(0);

	return gTopo_.logicalToPhysicalId[aCpu];
}
LogicalCoreId CpuTopology::physical_to_logical( PhysicalCoreId aPhy ) const noexcept
{
	if( aPhy >= gTopo_.physicalToLogicalId.size() )
		return ~LogicalCoreId(0);

	return gTopo_.physicalToLogicalId[aPhy];
}

compat::span<LogicalCoreId const> CpuTopology::physical_to_logicals( PhysicalCoreId ) const noexcept
{
	//TODO
	//TODO
	//TODO
	//TODO
	//TODO
	return compat::span<LogicalCoreId const>{};
}

//--    get_logical_cpu_count()             ///{{{2///////////////////////////
std::size_t get_logical_cpu_count() noexcept
{
	// See: http://stackoverflow.com/a/150971

	// Can return 0 if unable to determine. In that case, fall-through to 
	// platform-dependent detection (if any).
	if( auto const count = std::thread::hardware_concurrency() )
		return std::size_t(count);

#	if FLUX_PLATFORM == FLUX_PLATFORM_LINUX || FLUX_PLATFORM == FLUX_PLATFORM_ANDROID
	return std::size_t(::sysconf( _SC_NPROCESSORS_ONLN ));
#	elif FLUX_PLATFORM == FLUX_PLATFORM_MACOS
	int count = 0;
	std::size_t len = sizeof(count);

	int mib[4];
	std::memset( mib, 0, sizeof(mib) );
	mib[0] = CTL_HW;
	mib[1] HW_AVAILCPU;

	if( 0 != ::sysctl( mib, 2, &count, &len, 0, 0 ) )
	{
		if( count < 1 )
		{
			mib[1] = HW_NCPU;
			if( 0 != ::sysctl( mib, 2, &count, &len, 0, 0 ) )
			{
				if( count < 1 )
					count = 1;
			}
		}
	}

	return std::size_t(count);
#	elif FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	namespace Win32_ = dlapi::os::Win32;
	auto const* w32 = dlapi::os::win32();
	FLUX_ASSERT( w32 );

	Win32_::SYSTEM_INFO si;
	w32->getSystemInfo( &si );
	return std::size_t(si.dwNumberOfProcessors);
#	endif // ~ PLATFORM

	return 0;
}

//--    get_cpu_topology()                  ///{{{2///////////////////////////
CpuTopology get_cpu_topology() noexcept
{
	if( !gTopoValid_.load() )
		cpu_topology_refresh();

	return CpuTopology( compat::shared_lock<compat::shared_mutex>(gTopoMut_) );
}

//--    cpu_topology_refresh()              ///{{{2///////////////////////////
void cpu_topology_refresh() noexcept
{
	query_enabled_cores_();
	
#	if FLUX_SYS_WITH_X86CPUID
	if( x86_query_cputopo_() )
		return;
#	endif // ~ WITH_X86CPUID

#	if FLUX_PLATFORM == FLUX_PLATFORM_LINUX
		
	//TODO
	//TODO   Parse /proc/cpuinfo
	//TODO

#	elif FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	
	//TODO
	//TODO   Use Magic Win32 API: GetLogicalProcessorInformationEx()
	//TODO

#	endif // ~ PLATFORM

	{
		std::unique_lock<compat::shared_mutex> lock(gTopoMut_);
		gTopo_.logicalCoreCount = 0;
		gTopo_.logicalToPhysicalId.clear();
		gTopo_.physicalToLogicalId.clear();

		// query_enabled_cores_() set/cleared gTopo.enabledLogicalCores.

		gTopoValid_.store( true );
	}
}

//--    cpu_topology_invalidate()           ///{{{2///////////////////////////
void cpu_topology_invalidate() noexcept
{
	gTopoValid_.store( false );
}

//--    get_hw_concurrency()                ///{{{2///////////////////////////
std::size_t get_hw_concurrency() noexcept
{
	return get_logical_cpu_count();
}

//--    get_page_size()                     ///{{{2///////////////////////////
std::size_t get_page_size() noexcept
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_LINUX || FLUX_PLATFORM == FLUX_PLATFORM_ANDROID
	return ::sysconf( _SC_PAGESIZE );
#	elif FLUX_PLATFORM == FLUX_PLATFORM_MACOS
	return ::getpagesize();
#	elif FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	namespace Win32_ = dlapi::os::Win32;
	auto const* w32 = dlapi::os::win32();
	FLUX_ASSERT( w32 );

	Win32_::SYSTEM_INFO si;
	w32->getSystemInfo( &si );
	return std::size_t(si.dwPageSize);
#	endif // ~ PLATFORM

	return 0;
}

//--    get_physical_mem()                  ///{{{2///////////////////////////
std::size_t get_physical_mem() noexcept
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_LINUX || FLUX_PLATFORM == FLUX_PLATFORM_ANDROID
	struct sysinfo si;

	if( 0 == ::sysinfo( &si ) )
		return std::size_t(si.totalram) * si.mem_unit;
#	elif FLUX_PLATFORM == FLUX_PLATFORM_MACOS
	// XXX- UNTESTED
	// XXX- UNTESTED

	int mib[2];
	mib[0] = CTL_HW;
	mib[1] = HW_MEMSIZE;

	uint64_t size = 0;
	std::size_t len = sizeof(size);

	if( 0 == ::sysctl( mib, 2, &size, &len, 0, 0 ) )
		return std::size_t(size);
#	elif FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	namespace Win32_ = dlapi::os::Win32;
	auto const* w32 = dlapi::os::win32();
	FLUX_ASSERT( w32 );

	Win32_::MEMORYSTATUSEX ms;
	std::memset( &ms, 0, sizeof(ms) );
	ms.dwLength = sizeof(ms);

	if( w32->globalMemoryStatusEx( &ms ) )
		return std::size_t(ms.ullTotalPhys);
#	endif // ~ PLATFORM

	return 0;
}

//--    get_avaialable_mem()                ///{{{2///////////////////////////
std::size_t get_available_mem( bool aIncludeCaches ) noexcept
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_LINUX || FLUX_PLATFORM == FLUX_PLATFORM_ANDROID
	struct sysinfo si;

	if( 0 == ::sysinfo( &si ) )
	{
		std::size_t cached = 0;
		if( aIncludeCaches )
		{
			/* Unfortunately, sysinfo() doesn't include the cached memory;
			 * apparently the most straight-forward way is to parse 
			 * /proc/meminfo :-(
		     */

			if( auto* f = std::fopen( "/proc/meminfo", "r" ) )
			{
				FLUX_UTIL_ON_SCOPE_EXIT
				{
					std::fclose( f );
				};

				char name[128], value[128];
				while( 2 == std::fscanf( f, "%127[^:]%*[:]%*[ \t]%127[^\n\r]%*[\n\r]", name, value ) )
				{
					name[127] = '\0';
					value[127] = '\0';

					static char const cachedLine[] = "Cached";
					std::size_t const bytes = std::min( sizeof(name), sizeof(cachedLine)-1 );
					if( 0 == std::memcmp( cachedLine, name, bytes ) )
					{
						/* atoll() is sometimes missing from namespace std; yet
						 * most stdlibs have it somewhere.
						 */
						using namespace std;
						cached = atoll(value) * 1024; //XXX-is this always kB?
						break;
					}
				}
			}
		}

		return std::size_t(si.freeram) * si.mem_unit + cached;
	}
#	elif FLUX_PLATFORM == FLUX_PLATFORM_MACOS
#		error "TODO"
#	elif FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	namespace Win32_ = dlapi::os::Win32;
	auto const* w32 = dlapi::os::win32();
	FLUX_ASSERT( w32 );

	Win32_::MEMORYSTATUSEX ms{};
	ms.dwLength = sizeof(ms);

	if( w32->globalMemoryStatusEx( &ms ) )
		return std::size_t(ms.ullAvailPhys); // TODO- what about caches?
#	endif // ~ PLATFORM

	return 0;
}

//--    get_uptime()                        ///{{{2///////////////////////////
std::size_t get_uptime() noexcept
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_LINUX || FLUX_PLATFORM == FLUX_PLATFORM_ANDROID
	struct sysinfo si;

	if( 0 == ::sysinfo( &si ) )
		return std::size_t(si.uptime);
#	elif FLUX_PLATFORM == FLUX_PLATFORM_MACOS
	// XXX- UNTESTED
	// XXX- UNTESTED

	int mib[2];
	mib[0] = CTL_KERN;
	mib[1] = KERN_BOOTTIME;

	timeval ts;
	std::size_t len = sizeof(ts);

	if( 0 == ::sysctl( mib, 2, &ts, &len, 0, 0 ) )
		return std::size_t(ts.tv_sec);
#	elif FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	auto const* w32 = dlapi::os::win32();
	FLUX_ASSERT( w32 );

	return std::size_t( w32->getTickCount64() / 1000 );
#	endif // ~ PLATFORM

	return 0;
}

//--    get_sys_name()                      ///{{{2///////////////////////////
std::string get_sys_name()
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	namespace Win32_ = dlapi::os::Win32;
	auto const* w32 = dlapi::os::win32();
	FLUX_ASSERT( w32 );

	std::ostringstream oss;
	/* Quick intro: there are two versions. The actual Windows version, and the
	 * version that the OS will pretend to be (based on the manifest).
	 *
	 * This attempt to figure out both, the first via the driver/kernel
	 * RtlGetVersion() (=actual version), and the second using the official
	 * GetVersionEx() (=pretended version). Note that GetVersionEx() has been
	 * deprecated since.
	 */
	auto decodeOsVersion = [&w32,&oss] (Win32_::OSVERSIONINFOEXA& aOsv) {
		bool const isWorkstation = (aOsv.wProductType == Win32_::ver_nt_workstation);
		switch( aOsv.dwMajorVersion )
		{
			case 10:
			{
				if( isWorkstation )
					oss << "10";
				else
					oss << "Server 10";
				break;
			}
			case 6: 
			{
				if( isWorkstation )
				{
					switch( aOsv.dwMinorVersion )
					{
						case 3: oss << "8.1"; break;
						case 2: oss << "8"; break;
						case 1: oss << "7"; break;
						case 0: oss << "Vista"; break;
						default: oss << "version 6." << aOsv.dwMinorVersion; break;
					}
				}
				else
				{
					switch( aOsv.dwMinorVersion )
					{
						case 3: oss << "Server 2012 R2"; break;
						case 2: oss << "Server 2012"; break;
						case 1: oss << "Server 2008 R2"; break;
						case 0: oss << "Server 2008"; break;
						default: oss << "version 6." << aOsv.dwMinorVersion << " server"; break;
					}
				}
				break;
			}
			case 5: 
			{
				switch( aOsv.dwMinorVersion )
				{
					case 2: 
					{
						if( isWorkstation /*&& si.wProcessorArchitecture == Win32_::processor_architecture_amd64*/ )
						{
							oss << "XP Professional x64";
						}
						else if( aOsv.wSuiteMask & Win32_::ver_suite_wh_server )
						{
							oss << "Home Server";
						}
						else
						{
							if( w32->getSystemMetrics( Win32_::sm_serverr2 ) != 0 )
								oss << "Server 2003 R2";
							else
								oss << "Server 2003";
						}

						break;
					}
					case 1: oss << "XP"; break;
					case 0: oss << "2000"; break;
					default: oss << "version 5." << aOsv.dwMinorVersion << ""; break;
				}
				break;
			}
		
			default:
				oss << "version " << aOsv.dwMajorVersion << "." << aOsv.dwMinorVersion;
				break;
		}

		if( 0 != aOsv.wServicePackMajor )
		{
			oss << " SP" << aOsv.wServicePackMajor << "." << aOsv.wServicePackMinor;
		}
	};

	using RtlGetVersionFn_ = /*NTSTATUS*/Win32_::LONG (FLUXWIN32_API*)(void*);
	if( RtlGetVersionFn_ rtlGetVersion = (RtlGetVersionFn_)w32->getProcAddress(w32->getModuleHandleA("ntdll"), "RtlGetVersion") )
	{
		Win32_::OSVERSIONINFOEXA actual;
		memset( &actual, 0, sizeof(Win32_::OSVERSIONINFOEXA) );
		actual.dwOSVersionInfoSize = sizeof(actual);

		if( rtlGetVersion( &actual ) >= 0 )
		{
			oss << "Windows ";
			decodeOsVersion( actual );
			oss << " (" << actual.dwBuildNumber << ") ";
		}
	}
	else
	{
		oss << "Unknown Windows ";
	}
	
	Win32_::OSVERSIONINFOEXA pretend;
	memset( &pretend, 0, sizeof(Win32_::OSVERSIONINFOEXA) );
	pretend.dwOSVersionInfoSize = sizeof(pretend);

	if( w32->getVersionExA( &pretend ) )
	{
		oss << "as Windows ";
		decodeOsVersion( pretend );
	}
	else
	{
		oss << "as unknown Windows";
	}

	Win32_::SYSTEM_INFO si;
	w32->getSystemInfo( &si );

	switch( si.wProcessorArchitecture )
	{
		case Win32_::processor_architecture_amd64: oss << " (x86_64)"; break;
		case Win32_::processor_architecture_arm: oss << " (arm)"; break;
		case Win32_::processor_architecture_ia64: oss << " (ia64)"; break;
		case Win32_::processor_architecture_intel: oss << " (x86)"; break;
	}

	return oss.str();
#	elif defined(FLUXCFG_WITH_UNAME_)
	utsname un;
	if( 0 == uname( &un ) )
	{
		std::ostringstream oss;
		oss << un.sysname << " " << un.release << " (" << un.machine << ")";
		return oss.str();
	}
#	endif // ~ system detection mechanisms

	return "unknown";
}

//--    get_cpu_name()                      ///{{{2///////////////////////////
std::string get_cpu_name()
{
#	if FLUX_SYS_WITH_X86CPUID
	auto const highest = detail::X86CpuId( 0x80000000u );
	if( (highest.eax & 0x80000000u) && highest.eax >= 0x80000004u )
	{
		char brand[12*sizeof(uint32_t)+1];
		std::memset( brand, 0, sizeof(brand) );

		for( uint32_t i = 0x80000002u; i <= 0x80000004u; ++i )
		{
			auto const data = detail::X86CpuId( i );
			
			auto const j = i - 0x80000002u;
			std::memcpy( &brand[j*16+ 0], &data.eax, sizeof(uint32_t) );
			std::memcpy( &brand[j*16+ 4], &data.ebx, sizeof(uint32_t) );
			std::memcpy( &brand[j*16+ 8], &data.ecx, sizeof(uint32_t) );
			std::memcpy( &brand[j*16+12], &data.edx, sizeof(uint32_t) );
		}

		return std::string(brand);
	}
#	endif // ~ WITH_X86CPUID
	
#	if FLUX_PLATFORM == FLUX_PLATFORM_LINUX || FLUX_PLATFORM == FLUX_PLATFORM_ANDROID
	if( auto* f = std::fopen( "/proc/cpuinfo", "r" ) )
	{
		FLUX_UTIL_ON_SCOPE_EXIT
		{
			std::fclose( f );
		};

		char name[128], value[128];
		while( 2 == std::fscanf( f, "%127[^:]%*[:]%*[ \t]%127[^\n\r]%*[\n\r]", name, value ) )
		{
			name[127] = '\0';
			value[127] = '\0';

			static char const modelName[] = "model name";
			std::size_t const bytes = std::min( sizeof(name), sizeof(modelName)-1 );
			if( 0 == std::memcmp( modelName, name, bytes ) )
				return std::string(value);
		}
	}
#	elif FLUX_PLATFORM == FLUX_PLATFORM_MACOS
	
	/* Look at sysctlbyname() ? */

#	elif FLUX_PLATFORM == FLUX_PLATFORM_WIN32

	/* Relies on CPUID */

#	endif // ~ PLATFORM

	return "unknown";
}

//--    get_host_name()                     ///{{{2///////////////////////////
std::string get_host_name()
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	char buffer[256];

	if( 0 == gethostname( buffer, 255 ) )
	{
		buffer[255] = '\0';
		return std::string(buffer);
	}
#	else // WIN32
	namespace Win32_ = dlapi::os::Win32;
	auto const w32 = dlapi::os::win32();
	FLUX_ASSERT( w32 );

	char buffer[256];
	Win32_::DWORD size = sizeof(buffer);
	if( w32->getComputerNameA( buffer, &size ) )
		return std::string(buffer);
#	endif // ~ WIN32

	return "unknown";
}

//--    get_arch_ident()                    ///{{{2///////////////////////////
std::string get_arch_ident()
{
	std::ostringstream oss;
#	if FLUX_ARCH == FLUX_ARCH_ARM
	oss << "arm" << FLUX_ARCH_ARM_VERSION << 
		(FLUX_ARCH_ARM_HARDFP_ABI ? "hfp":"")
	;
#	elif FLUX_ARCH == FLUX_ARCH_ARM64
	oss << "arm64";
#	elif FLUX_ARCH == FLUX_ARCH_X86_32
	oss << "x86";
#	elif FLUX_ARCH == FLUX_ARCH_X86_64
	oss << "x64";
#	else
	oss << "unknown";
#	endif // ~ arch

	return oss.str();
}

//--    get_os_ident()                      ///{{{2///////////////////////////
std::string get_os_ident()
{
	std::ostringstream oss;
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	oss << "windows"; /* TODO version? */
#	elif FLUX_PLATFORM == FLUX_PLATFORM_ANDROID
	oss << "android"; /* TODO version? */
#	elif FLUX_PLATFORM == FLUX_PLATFORM_LINUX
	utsname un;
	if( 0 == uname( &un ) )
	{
		for( char const* sn = un.sysname; *sn; ++sn )
			oss << char(std::tolower(*sn));
	}
#	else
	oss << "unknown";
#	endif // ~ platform

	return oss.str();
}

//--    $ query_enabled_cores_()            ///{{{2///////////////////////////
namespace
{
	void query_enabled_cores_() noexcept try
	{
		ftl::Vector<LogicalCoreId,64*sizeof(LogicalCoreId)> enabled;
		/*std::size_t ret =*/ exec_for_each_enabled_cpu( 
			[&enabled] (LogicalCoreId aEnabled) {
				enabled.emplace_back( aEnabled );
			}
		);

		std::unique_lock<compat::shared_mutex> lock(gTopoMut_);
		gTopo_.enabledLogicalCores = std::move(enabled);
	}
	catch( ... )
	{
		std::unique_lock<compat::shared_mutex> lock(gTopoMut_);
		gTopo_.enabledLogicalCores.clear();
	}
}

//--    $ x86_query_cputopo_()              ///{{{2///////////////////////////
namespace
{
#	if FLUX_SYS_WITH_X86CPUID
	FLUX_CONSTEXPR_EX
	uint32_t next_pot_( uint32_t aX ) noexcept
	{
		aX--;
		aX |= aX >>  1u;
		aX |= aX >>  2u;
		aX |= aX >>  4u;
		aX |= aX >>  8u;
		aX |= aX >> 16u;
		aX++;
		return aX;
	}
	uint32_t ilog2_( uint32_t aX ) noexcept
	{
		return 31 - util::bit_count_leading_zeros( aX );
	}

	template< class tList >
	void warn_inaccessible_( tList const& aList ) noexcept
	{
		std::ostringstream oss;
		for( std::size_t i = 0; i < aList.size(); ++i )
		{
			if( ~uint32_t(0) == aList[i] )
				oss << i << " ";
		}

		FLUX_LOG( WARNING, "Logical CPUs {}are inaccessible; physical core detection may be inaccurate", oss.str() );
	}

	template< class tList >
	bool enum_core_apic_( tList& aList ) noexcept
	{
		bool inaccessible = false;
		exec_for_each_logical_cpu(
			[&aList] (LogicalCoreId aCoreId) {
				FLUX_ASSERT( aCoreId < aList.size() );
				aList[aCoreId] = (detail::X86CpuId(1).ebx >> 24) & 0xff;
			},
			[&aList, &inaccessible] (LogicalCoreId aCoreId) {
				FLUX_ASSERT( aCoreId < aList.size() );
				aList[aCoreId] = ~uint32_t(0);
				inaccessible = true;
			}
		);
		return inaccessible;
	}
	template< uint32_t tLeaf, class tList >
	bool enum_core_x2apic_( tList& aList ) noexcept
	{
		bool inaccessible = false;
		exec_for_each_logical_cpu(
			[&aList] (LogicalCoreId aCoreId) {
				FLUX_ASSERT( aCoreId < aList.size() );
				aList[aCoreId] = detail::X86CpuId(tLeaf).edx;
			},
			[&aList,&inaccessible] (LogicalCoreId aCoreId) {
				FLUX_ASSERT( aCoreId < aList.size() );
				aList[aCoreId] = ~uint32_t(0);
				inaccessible = true;
			}
		);
		return inaccessible;
	}

	template< uint32_t tLeaf >
	bool x86_intel_query_extended_()
	{
		// tLeaf can be either 0xb or 0x1f
		auto const support = detail::X86CpuId( tLeaf );
		if( 0 != support.ebx )
		{
			// Figure out shifts
			uint32_t shiftSmt = 0, shiftCore = 0, numCores = 0;
			for( uint32_t i = 0, ebx = support.ebx; !!(ebx & 0xffff); ++i )
			{
				auto const data = detail::X86CpuId( tLeaf, i );
				auto const type = (data.ecx >> 8) & 0xff;
				switch( type )
				{
					case 0: /* invalid */ break;
					case 1: {
						/* Level Type = "SMT" */
						shiftSmt = data.eax & 0xf;
					} break;
					case 2: {
						/* Level Type = "Core" */
						shiftCore = data.eax & 0xf;
						numCores = data.ebx & 0xffff;
					} break;
					case 3: /* 0x1f only! */ {
						/* Level Type = "Module" */
					} break;
					case 4: /* 0x1f only! */ {
						/* Level Type = "Tile" */
					} break;
					case 5: /* 0x1f only! */ {
						/* Level Type = "Die" */
					} break;

					/* 6-255: Reserved. (3-255 reserved if 0xb) */
				}

				ebx = data.ebx;
			}

			auto const logicalCount = [&] {
				auto const count = get_logical_cpu_count();
				return ~std::size_t(0) == count
					? std::size_t(numCores)
					: count
				;
			}();

			ftl::Vector<uint32_t,64*sizeof(uint32_t)> coreApic( logicalCount, ~uint32_t(0) );
			if( enum_core_x2apic_<tLeaf>( coreApic ) )
				warn_inaccessible_( coreApic );

			auto const smtMask = ~((~0u)<<shiftSmt);
			auto const coreMask = smtMask ^ ~((~0u)<<shiftCore);

			// smtId = apic & smtMask;
			// coreId = (apic & coreMask) >> shiftSmt

			PhysicalCoreId maxPhysical = 0;
			flux_ext::flat_hash_set<PhysicalCoreId> physicals;
			for( auto const core : coreApic )
			{
				if( ~uint32_t(0) == core ) continue;
				auto const phy = (core & coreMask) >> shiftSmt;
				if( phy > maxPhysical )
					maxPhysical = phy;

				physicals.emplace( phy );
			}

			{
				std::unique_lock<compat::shared_mutex> lock(gTopoMut_);
				gTopo_.logicalToPhysicalId.resize( coreApic.size() );
				gTopo_.physicalToLogicalId.resize( maxPhysical+1, ~LogicalCoreId(0) );

				for( std::size_t i = 0; i < coreApic.size(); ++i )
				{
					auto const phy = (coreApic[i] != ~uint32_t(0))
						? (coreApic[i] & coreMask) >> shiftSmt
						: ~uint32_t(0)
					;
					gTopo_.logicalToPhysicalId[i] = phy;
					if( ~uint32_t(0) != phy && ~LogicalCoreId(0) == gTopo_.physicalToLogicalId[phy] )
						gTopo_.physicalToLogicalId[phy] = LogicalCoreId(i); //XXX-fixme int_cast<>
				}

				gTopo_.logicalCoreCount = logicalCount;
				gTopo_.physicalCoreCount = physicals.size();
				gTopoValid_.store(true);
			}

			return true;
		}

		return false;
	}
	bool x86_intel_query_legacy_()
	{
		auto const core = detail::X86CpuId( 1 );
		auto const maxNumCores = (core.ebx >> 16) & 0xff; //WARNING: this is **not** the number of cores for e.g., i7-8700K with 12 cores. "maxNumCores" here is 16. Note that i7-8700 supports CPUID.0x1b.

		auto const topo = detail::X86CpuId( 0x4 );
		auto const thing = 1 + ((topo.eax >> 26) & 0x3f); // "maximum number of addressable IDs for processor cores in the physical package"

		auto const shiftSmt = ilog2_( next_pot_(maxNumCores) / thing );
		auto const shiftCore = shiftSmt + ilog2_( thing );

		auto const logicalCount = [&] {
			auto const count = get_logical_cpu_count();
			return ~std::size_t(0) == count
				? std::size_t(maxNumCores)
				: count
			;
		}();

		ftl::Vector<uint32_t,64*sizeof(uint32_t)> coreApic( logicalCount, ~uint32_t(0) );
		bool const inaccessible = enum_core_apic_( coreApic );
		if( inaccessible ) warn_inaccessible_( coreApic );

		auto const smtMask = ~((~0u)<<shiftSmt);
		auto const coreMask = smtMask ^ ~((~0u)<<shiftCore);

		// smtId = apic & smtMask;
		// coreId = (apic & coreMask) >> shiftSmt

		PhysicalCoreId maxPhysical = 0;
		flux_ext::flat_hash_set<PhysicalCoreId> physicals;
		for( auto const core : coreApic )
		{
			if( ~uint32_t(0) == core ) continue;
			auto const phy = (core & coreMask) >> shiftSmt;
			if( phy > maxPhysical )
				maxPhysical = phy;

			physicals.emplace( phy );
		}

		{
			std::unique_lock<compat::shared_mutex> lock(gTopoMut_);
			gTopo_.logicalToPhysicalId.resize( coreApic.size() );
			gTopo_.physicalToLogicalId.resize( maxPhysical+1, ~LogicalCoreId(0) );

			for( std::size_t i = 0; i < coreApic.size(); ++i )
			{
				auto const phy = (coreApic[i] != ~uint32_t(0))
					? (coreApic[i] & coreMask) >> shiftSmt
					: ~uint32_t(0)
				;
				gTopo_.logicalToPhysicalId[i] = phy;
				if( ~uint32_t(0) != phy && ~LogicalCoreId(0) == gTopo_.physicalToLogicalId[phy] )
					gTopo_.physicalToLogicalId[phy] = LogicalCoreId(i); // XXX-fixme int_cast<>
			}

			gTopo_.logicalCoreCount = logicalCount;
			gTopo_.physicalCoreCount = physicals.size();
			gTopoValid_.store(true);
		}

		return true;
	}

	bool x86_amd_query_legacy_()
	{
		/* There is some information on how to do this. First, there's the
		 * OSDev wiki [1], which seems to be out of date (and the described
		 * method to derive the logical_CPU_bits doesn't work in my tests). The
		 * descriptions there are clearly not the best either, and there are no
		 * refencences.
		 *
		 * AMD provides the "CPUID specification" [2], but that is dated 2010,
		 * and it's unclear if it contains the correct information for newer
		 * CPUs. I was unable to find a newer version on AMD's page. CPUID is
		 * also documented in their "OSRR for AMD Family 17h processors"
		 * document [3], that is at least 2018. There is however no newer
		 * version (this being written on a Family 23h processor).  Finally,
		 * there is the 3000 page document [4] that does include some
		 * information around page 1700.
		 *
		 * Note: technically my test processor claims support for CPUID leaf
		 * 0x10 and 0x80000020. The former should include 0xb (where Intel
		 * keeps this information), but the registers there are marked as
		 * reserved.
		 *
		 * [1] https://wiki.osdev.org/Detecting_CPU_Topology_(80x86)#AMD:_CPUID_eax.3D0x80000008
		 * [2] https://www.amd.com/system/files/TechDocs/25481.pdf
		 * [3] https://developer.amd.com/wp-content/resources/56255_3_03.PDF
		 * [4] https://www.amd.com/system/files/TechDocs/40332.pdf
		 */

		auto const core = detail::X86CpuId( 1 );
		auto const maxNumCores = (core.ebx >> 16) & 0xff;
		auto const hasHTT = (core.edx & (1u<<28));
		
		//auto const info = detail::X86CpuId( 0x80000001u );
		//bool const legacy = !!(info.ecx & (1u<<1)); //XXX-off by one?
		//auto const logicalProcessorCount = (info.ebx >> 16) & 0xff; // **ONLY** valid if hasHTT is true

		auto const counts = detail::X86CpuId( 0x80000008u );
		//auto const numPhysical = 1 + (counts.ecx & 0xff);
		auto const apicCoreIdSize = (counts.ecx >> 12) & 0xf;

		// NOTE: if apicCoreIdSize is zero, the "legacy" method should be used
		// whatever that means.
		// Updated: From
		// https://wiki.osdev.org/Detecting_CPU_Topology_(80x86)#AMD:_CPUID_eax.3D0x80000008
		// > "First check ECX bits 12 to 15, if it is not zero, then it
		// > contains "core_bits". Otherwise, use ECX bits 0 to 7 to determine
		// > the number of cores, round it up to the next power of 2 and use it
		// > to determine "core_bits". 

		auto const logicalCount = [&] {
			auto const count = get_logical_cpu_count();
			return ~std::size_t(0) == count
				? std::size_t(maxNumCores)
				: count
			;
		}();

		ftl::Vector<uint32_t,64*sizeof(uint32_t)> coreApic( logicalCount, ~uint32_t(0) );
		bool const inaccessible = enum_core_apic_( coreApic );
		if( inaccessible ) warn_inaccessible_( coreApic );

		auto const coreMask = (1u<<apicCoreIdSize)-1;
		//auto const coreShift = next_pot_( maxNumCores >> apicCoreIdSize ); // Suggested by OSDev, but doens't work.
		auto const coreShift = hasHTT ? 1 : 0; // Based on 2.1.8.2.1.3 in https://developer.amd.com/wp-content/resources/56255_3_03.PDF, with a bit of artistic license.

		PhysicalCoreId maxPhysical = 0;
		flux_ext::flat_hash_set<PhysicalCoreId> physicals;
		for( auto const core : coreApic )
		{
			if( ~uint32_t(0) == core ) continue;
			auto const phy = (core >> coreShift) & coreMask;
			if( phy > maxPhysical )
				maxPhysical = phy;

			physicals.emplace( phy );
		}

		{
			std::unique_lock<compat::shared_mutex> lock(gTopoMut_);
			gTopo_.logicalToPhysicalId.resize( coreApic.size() );
			gTopo_.physicalToLogicalId.resize( maxPhysical+1, ~LogicalCoreId(0) );

			for( std::size_t i = 0; i < coreApic.size(); ++i )
			{
				auto const phy = (coreApic[i] != ~uint32_t(0))
					?  (coreApic[i] >> coreShift) & coreMask
					: ~uint32_t(0)
				;
				gTopo_.logicalToPhysicalId[i] = phy;

				if( ~uint32_t(0) != phy && ~LogicalCoreId(0) == gTopo_.physicalToLogicalId[phy] )
					gTopo_.physicalToLogicalId[phy] = LogicalCoreId(i); //XXX-fixme int_cast<>
			}

			gTopo_.logicalCoreCount = logicalCount;
			gTopo_.physicalCoreCount = physicals.size();
			gTopoValid_.store(true);
		}

		return true;
	}
	bool x86_amd_query_extended_()
	{
		FLUX_ASSERT( detail::X86CpuId( 0x80000000u ).eax >= 0x8000001eu );

		auto const core = detail::X86CpuId( 1 );
		auto const maxNumCores = (core.ebx >> 16) & 0xff;

		auto const logicalCount = [&] {
			auto const count = get_logical_cpu_count();
			return ~std::size_t(0) == count
				? std::size_t(maxNumCores)
				: count
			;
		}();

		bool inaccessible = false;
		ftl::Vector<uint32_t,64*sizeof(uint32_t)> phy( logicalCount, ~uint32_t(0) );

		exec_for_each_logical_cpu(
			[&phy] (LogicalCoreId aLogical) {
				FLUX_ASSERT( aLogical < phy.size() );
				auto const ext = detail::X86CpuId( 0x8000001eu );
				//auto const threadsPerCore = 1 + ((ext.ebx >> 8) & 0xff);
				auto const coreId = (ext.ebx & 0xff);

				phy[aLogical] = coreId;
			},
			[&phy, &inaccessible] (LogicalCoreId aLogical) {
				FLUX_ASSERT( aLogical < phy.size() );
				phy[aLogical] = ~uint32_t(0);
				inaccessible = true;
			}
		);

		if( inaccessible ) warn_inaccessible_( phy );

		uint32_t maxPhysical = 0;
		flux_ext::flat_hash_set<PhysicalCoreId> physicals;

		for( auto const id : phy )
		{
			maxPhysical = std::max( maxPhysical, id );
			physicals.emplace ( id );
		}

		{
			std::unique_lock<compat::shared_mutex> lock(gTopoMut_);
			gTopo_.logicalToPhysicalId.resize( phy.size() );
			gTopo_.physicalToLogicalId.resize( maxPhysical+1, ~LogicalCoreId(0) );

			for( std::size_t i = 0; i < phy.size(); ++i )
			{
				gTopo_.logicalToPhysicalId[i] = phy[i];
				gTopo_.physicalToLogicalId[phy[i]] = util::narrow<uint32_t>(i);
			}

			gTopo_.logicalCoreCount = logicalCount;
			gTopo_.physicalCoreCount = physicals.size();
			gTopoValid_.store(true);
		}

		return true;
	}

	bool x86_query_cputopo_() noexcept try
	{
		auto const basic = detail::X86CpuId( 0 );
		auto const highest = basic.eax;
		
		char mfg[3*sizeof(uint32_t)+1];
		std::memcpy( &mfg[0], &basic.ebx, sizeof(uint32_t) );
		std::memcpy( &mfg[4], &basic.edx, sizeof(uint32_t) );
		std::memcpy( &mfg[8], &basic.ecx, sizeof(uint32_t) );
		mfg[12] = '\0';

		if( 0 == std::memcmp( "GenuineIntel", mfg, 12 ) )
		{
			// See e.g., https://www.felixcloutier.com/x86/cpuid
			// The note indicates that 0x1f should be preferred over 0xb. However,
			// it seems that the two CPUID leafs return nearly identical, with the
			// difference that types 3-255 are reserved for 0xb and Module, Tile
			// and Die respectively for 0x1f. 0x4 is a legacy fallback.
			//
			// Also see
			// https://software.intel.com/en-us/articles/intel-64-architecture-processor-topology-enumeration/
			//
			// XXX-note: leaf = 0x1f currently untested, as none of my machines
			// support it.
			if( highest >= 0x1fu && x86_intel_query_extended_<0x1fu>() )
				return true;

			if( highest >= 0xbu && x86_intel_query_extended_<0xbu>() )
				return true;

			if( highest >= 0x4 && x86_intel_query_legacy_() )
				return true;
		}
		else if( 0 == std::memcmp( "AuthenticAMD", mfg, 12 ) || 0 ==  std::memcmp( "AMDisbetter!", mfg, 12 ) )
		{
			// See https://www.amd.com/system/files/TechDocs/25481.pdf and
			// additional documents referenced in x86_amd_query_legacy_().
			auto const ext = detail::X86CpuId( 0x80000000u );
			auto const ehighest = ext.eax;

			if( ehighest >= 0x8000001eu && x86_amd_query_extended_() )
				return true;
			
			/* The following uses the Intel query using function 0xb. This is 
			 * somewhat sketchy, but seems to return the correct results in
			 * the minimal tests that I've performed. (A very quick and very
			 * incomplete reading of the Linux sources seem to indicate that
			 * Linux might attempt this as well.)
			 *
			 * On my test CPU (Ryzen 3 3100), the CPUID function claims that
			 * functions up to 0x10 are available. AMD's documentation however
			 * marks the corresponding registers as reserved..
			 */
			if( highest >= 0xbu && x86_intel_query_extended_<0xbu>() )
				return true;
			
			if( ehighest >= 0x80000008u && x86_amd_query_legacy_() )
				return true;
		}

		return false;
	}
	catch( ... )
	{
		return false;
	}
#	endif // ~ WITH_X86CPUID
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
