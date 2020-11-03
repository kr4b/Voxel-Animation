/*-******************************************************* -- HEADER -{{{1- */
/*- Win32 api subset
 *
 * This may seem superfluous, since the Win32 API will always be available on
 * machines that support it (so static linking doesn't give us any problems).
 * However, this enables the use of the Win32 API when running from Cygwin
 * where <windows.h> isn't always available.
 *
 * Meh.
 *
 * Besides, we can use the Win32 API for some things *WITHOUT* touching
 * <windows.h> (*), which is ... liberating. (Mainly for the global namespace.)
 *
 * (*) Other than once for LoadLibrary() et al.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_OS_WIN32_HPP_438A117E_6F28_40F0_B461_CB654CF55070
#define FLUXDLAPI_OS_WIN32_HPP_438A117E_6F28_40F0_B461_CB654CF55070

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/arch.hpp>

#include <flux/std/identity.hpp>
#include <flux/std/inttypes.hpp>


#include <flux/sys/dylib.hpp>

#include <cstddef>

//--    >>> namespace = dlapi::os >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    macros                          ///{{{2///////////////////////////////

#if !defined(FLUXWIN32_API)
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32 || FLUX_PLATCAP_CYGWIN
#		define FLUXWIN32_API __stdcall
#	else
#		define FLUXWIN32_API
#	endif
#endif // ~ defined(FLUXWIN32_API)

//--    constants                       ///{{{2///////////////////////////////

/** \brief List of default Win32 API DLLs
 *
 * Colon-delimited list of DLLs in which Win32 API functions are searched for
 * and loaded from. This list is used by default, i.e., when no argument is
 * passed to e.g. `acquire_win32()`.
 *
 * \note DLLs are inspected in the order they are listed. Frequently sourced
 * DLLs should be listed first, as this will aid search performance. (However,
 * it is possible inject custom DLLs first if certain functions are to be
 * redirected.)
 */
extern char const* const kDefaultWin32DLLs;

//--    enums                           ///{{{2///////////////////////////////
namespace Win32
{
	// FILE_INFO_BY_HANDLE_CLASS (partial)
	enum FILE_INFO_BY_HANDLE_CLASS
	{
		file_basic_info,
		file_standard_info,
		file_name_info,
		file_rename_info,
		//...
		maximum_file_info_by_handle_class = 21
	};
}

//--    types                           ///{{{2///////////////////////////////
namespace Win32
{
	using HANDLE = void*;
	using LPHANDLE = HANDLE*;

	typedef struct HDCOpaque_* HDC;
	typedef struct HINSTANCEOpaque_* HINSTANCE;

	using HLOCAL = HANDLE;
	using HMODULE = HINSTANCE;

	using INT = int;
	using BOOL = int;
	using CHAR = char;
	using LONG = long;
	using UINT = unsigned int;
	using SHORT = short;
	using WCHAR = wchar_t;
	using LONGLONG = int64_t;
	using ULONGLONG = uint64_t;

	using BYTE = unsigned char;
	using WORD = unsigned short;
	using DWORD = unsigned long;
	using DWORDLONG = ULONGLONG;

	using PSTR = CHAR*;
	using PCSTR = CHAR const*;
	using PWSTR = WCHAR*;
	using PCWSTR = WCHAR const*;
	using PDWORD = DWORD*;
	using PVOID = void*;

	using LPSTR = CHAR*;
	using LPCSTR = CHAR const*;
	using LPCCH = CHAR const*;
	using LPWSTR = WCHAR*;
	using LPDWORD = DWORD*;
	using LPVOID = void*;
	using LPCVOID = void const*;

#	if FLUX_ARCH == FLUX_ARCH_X86_32	
	using INT_PTR = int;
	using UINT_PTR = unsigned int;
	using LONG_PTR = long;
	using ULONG_PTR = unsigned long;
#	else
	using INT_PTR = long long;
	using UINT_PTR = unsigned long long;
	using LONG_PTR = long long;
	using ULONG_PTR = unsigned long long;
#	endif

	using SIZE_T = ULONG_PTR;
	using SSIZE_T = LONG_PTR;

	using KAFFINITY = ULONG_PTR;
	using DWORD_PTR = ULONG_PTR; // ¯\_(ツ)_/¯
	using PDWORD_PTR = DWORD_PTR*;

	using PROC = INT_PTR (FLUXWIN32_API*)();
	using FARPROC = INT_PTR (FLUXWIN32_API*)();

	typedef union LARGE_INTEGER_
	{
		struct { DWORD LowPart; LONG HighPart; };
		struct { DWORD LowPart; LONG HighPart; } u;
		LONGLONG QuadPart;
	} LARGE_INTEGER;

	struct GUID
	{
		unsigned long Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char Data4[8];
	};

	using LPGUID = GUID*;
	using LPCGUID = GUID const*;

	struct OVERLAPPED
	{
		ULONG_PTR internalLo;
		ULONG_PTR internalHi;
		union
		{
			struct 
			{
				DWORD offsetLo;
				DWORD offsetHi;
			} dummy;
			PVOID pointer;
		} dummy;

		HANDLE hEvent;
	};

	using LPOVERLAPPED = OVERLAPPED*;

	struct SECURITY_ATTRIBUTES
	{
		DWORD nLength;
		LPVOID lpSecurityDescriptor;
		BOOL bInheritHandle;
	};

	using LPSECURITY_ATTRIBUTES = SECURITY_ATTRIBUTES*;

	struct GROUP_AFFINITY
	{
		KAFFINITY Mask;
		WORD Group;
		WORD Reserved[3];
	};

	using PGROUP_AFFINITY = GROUP_AFFINITY*;

	struct PROCESSOR_RELATIONSHIP
	{
		BYTE Flags;
		BYTE EfficiencyClass;
		BYTE Reserved[20];
		WORD GroupCount;
		GROUP_AFFINITY GroupMask[1]; // ANYSIZE_ARRAY = 1
	};

	using PPROCESSOR_RELATIONSHIP = PROCESSOR_RELATIONSHIP*;

	struct NUMA_NODE_RELATIONSHIP
	{
		DWORD NodeNumber;
		BYTE Reserverd[20];
		GROUP_AFFINITY GroupMask;
	};

	using PNUMA_NODE_RELATIONSHIP = NUMA_NODE_RELATIONSHIP*;

	struct CACHE_RELATIONSHIP
	{
		BYTE Level;
		BYTE Associativity;
		WORD LineSize;
		DWORD CacheSize;
		uint8_t /* technically a random enum */ Type;
		BYTE Reserved[20];
		GROUP_AFFINITY GroupMask;
	};

	using PCACHE_RELATIONSHIP = CACHE_RELATIONSHIP*;

	struct PROCESSOR_GROUP_INFO
	{
		BYTE MaximumProcessorCount;
		BYTE ActiveProcessorCount;
		BYTE Reserved[38];
		KAFFINITY ActiveProcessorMask;
	};

	using PPROCESSOR_GROUP_INFO = PROCESSOR_GROUP_INFO*;

	struct GROUP_RELATIONSHIP
	{
		WORD MaximumGroupCount;
		WORD ActiveGroupCount;
		BYTE Reserved[20];
		PROCESSOR_GROUP_INFO GroupInfo[1]; // ANYSIZE_ARRAY = 1
	};

	using PGROUP_RELATIONSHIP = GROUP_RELATIONSHIP*;

	struct SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX
	{
		uint16_t /* technically a 16-bit enum */ Relationship;
		DWORD Size;
		union
		{
			PROCESSOR_RELATIONSHIP Processor;
			NUMA_NODE_RELATIONSHIP NumaNode;
			CACHE_RELATIONSHIP Cache;
			GROUP_RELATIONSHIP Group;
		};
	};

	using PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX = SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX*;

	struct SYSTEM_INFO
	{
		union
		{
			DWORD dwOemId;
			struct
			{
				WORD wProcessorArchitecture;
				WORD wReserved;
			} /* dummystructname */;
		} /* dummyunionname */;

		DWORD dwPageSize;
		LPVOID lpMinimumApplicationAddress;
		LPVOID lpMaximumApplicationAddress;
		DWORD_PTR dwActiveProcessorMask;
		DWORD dwNumberOfProcessors;
		DWORD dwProcessorType;
		DWORD dwAllocationGranularity;
		WORD wProcessorLevel;
		WORD wProcessorRevision;
	};

	using LPSYSTEM_INFO = SYSTEM_INFO*;

	struct OSVERSIONINFOEXA
	{
		DWORD dwOSVersionInfoSize;
		DWORD dwMajorVersion;
		DWORD dwMinorVersion;
		DWORD dwBuildNumber;
		DWORD dwPlatformId;
		CHAR szCSDVersion[128];
		WORD wServicePackMajor;
		WORD wServicePackMinor;
		WORD wSuiteMask;
		BYTE wProductType;
		BYTE wReserved;
	};

	using POSVERSIONINFOEXA = OSVERSIONINFOEXA*;
	using LPOSVERSIONINFOEXA = OSVERSIONINFOEXA*;

	struct MEMORYSTATUSEX
	{
		DWORD dwLength;
		DWORD dwMemoryLoad;
		DWORDLONG ullTotalPhys;
		DWORDLONG ullAvailPhys;
		DWORDLONG ullTotalPageFile;
		DWORDLONG ullAvailPageFile;
		DWORDLONG ullTotalVirtual;
		DWORDLONG ullAvailVirtual;
		DWORDLONG ullAvailExtendedVirtual;
	};

	using LPMEMORYSTATUSEX = MEMORYSTATUSEX*;

	struct COORD 
	{
		SHORT X;
		SHORT Y;
	};

	using PCOORD = COORD*;

	struct SMALL_RECT
	{
		SHORT Left;
		SHORT Top;
		SHORT Right;
		SHORT Bottom;
	};

	struct CONSOLE_SCREEN_BUFFER_INFO
	{
		COORD dwSize;
		COORD dwCursorPosition;
		WORD wAttributes;
		SMALL_RECT srWindow;
		COORD dwMaximumWindowSize;
	};

	using PCONSOLE_SCREEN_BUFFER_INFO = CONSOLE_SCREEN_BUFFER_INFO*;

	using va_list = char*;
}

//--    constants                       ///{{{2///////////////////////////////
namespace Win32
{
	/* Note: All of the Windows constants are a bunch of #defines, so we really
	 * can't use the same identifiers here (as those would collide with an
	 * accidentally included <windows.h> in hilarious ways). To avoid that the
	 * names are lowercased here.
	 */

	/*constexpr*/ const HANDLE invalid_handle_value = ((HANDLE)(LONG_PTR)-1);

	// mb <-> wchar
	constexpr UINT cp_acp         = 0;
	constexpr UINT cp_thread_acp  = 3;
	constexpr UINT cp_utf8        = 65001;

	constexpr DWORD mb_precomposed        = 0x1;
	constexpr DWORD mb_composite          = 0x2;
	constexpr DWORD mb_useglyphchars      = 0x4;
	constexpr DWORD mb_err_invalid_chars  = 0x8;

	// format message
	constexpr DWORD format_message_from_system      = 0x1000;
	constexpr DWORD format_message_allocate_buffer  =  0x100;
	constexpr DWORD format_message_ignore_inserts   =  0x200;
	constexpr DWORD format_message_max_width_mask   =   0xff;

	constexpr WORD lang_neutral     = 0x00;
	constexpr WORD sublang_default  = 0x01;

	constexpr
	DWORD makelangid( WORD p, WORD s) { return (s << 10) | p; }

	// events
	constexpr DWORD infinite = 0xffffffff;

	constexpr DWORD wait_abandoned  =       0x80;
	constexpr DWORD wait_object_0   =          0;
	constexpr DWORD wait_timeout    =      0x102;
	constexpr DWORD wait_failed     = 0xffffffff;

	// file types
	constexpr DWORD file_type_char                = 0x02;
	constexpr DWORD file_type_disk                = 0x01;
	constexpr DWORD file_type_pipe                = 0x03;
	constexpr DWORD file_type_remote              = 0x8000;
	constexpr DWORD file_type_unknown             = 0;

	// errors (select)
	constexpr DWORD error_invalid_parameter       =   87;
	constexpr DWORD error_insufficient_buffer     =  122;
	constexpr DWORD error_io_pending              =  997;
	constexpr DWORD error_invalid_flags           = 1004;
	constexpr DWORD error_no_unicode_translation  = 1113;

	// LOGICAL_PROCESSOR_RELATIONSHIP
	constexpr uint16_t relation_processor_core     = 0;
	constexpr uint16_t relation_numa_node          = 1;
	constexpr uint16_t relation_cache              = 2;
	constexpr uint16_t relation_processor_package  = 3;
	constexpr uint16_t relation_group              = 4;
	constexpr uint16_t relation_all                = 0xffff;

	// PROCESSOR_CACHE_TYPE
	constexpr uint8_t cache_unified      = 0;
	constexpr uint8_t cache_instruction  = 1;
	constexpr uint8_t cache_data         = 2;
	constexpr uint8_t cache_trace        = 3;

	// Thread/Process priority
	constexpr int thread_priority_above_normal   =   1;
	constexpr int thread_priority_below_normal   =  -1;
	constexpr int thread_priority_highest        =   2;
	constexpr int thread_priority_idle           = -15;
	constexpr int thread_priority_lowest         =  -2;
	constexpr int thread_priority_normal         =   0;
	constexpr int thread_priority_time_critical  =  15;
	constexpr int thread_priority_error_return   = 0x7fffffff;

	constexpr DWORD above_normal_priority_class  = 0x8000;
	constexpr DWORD below_normal_priority_class  = 0x4000;
	constexpr DWORD high_priority_class          = 0x80;
	constexpr DWORD idle_priority_class          = 0x40;
	constexpr DWORD normal_priority_class        = 0x20;
	constexpr DWORD realtime_priority_class      = 0x100;

	// Processor architecture and type
	constexpr WORD processor_architecture_amd64    =  9;
	constexpr WORD processor_architecture_arm      =  5;
	constexpr WORD processor_architecture_arm64    = 12;
	constexpr WORD processor_architecture_ia64     =  6;
	constexpr WORD processor_architecture_intel    =  0;
	constexpr WORD processor_architecture_unknown  = 0xffff;

	constexpr DWORD processor_intel_386      =  386;
	constexpr DWORD processor_intel_486      =  486;
	constexpr DWORD processor_intel_pentium  =  586;
	constexpr DWORD processor_intel_ia64     = 2200;
	constexpr DWORD processor_amd_x8664      = 8664;

	constexpr WORD ver_nt_workstation        = 1;
	constexpr WORD ver_nt_domain_controller  = 2;
	constexpr WORD ver_nt_server             = 3;

	constexpr WORD ver_suite_wh_server = 0x8000;

	// System metrics (select)
	constexpr int sm_tablet_pc    = 86;
	constexpr int sm_mediacenter  = 87;
	constexpr int sm_starter      = 88;
	constexpr int sm_serverr2     = 89;

	// Console stuff
	constexpr DWORD std_input_handle   = DWORD(-10);
	constexpr DWORD std_output_handle  = DWORD(-11);
	constexpr DWORD std_error_handle   = DWORD(-12);

	constexpr DWORD enable_echo_input                   = 0x004;
	constexpr DWORD enable_insert_mode                  = 0x020;
	constexpr DWORD enable_line_input                   = 0x002;
	constexpr DWORD enable_mouse_input                  = 0x010;
	constexpr DWORD enable_processed_input              = 0x001;
	constexpr DWORD enable_quick_edit_mode              = 0x040;
	constexpr DWORD enable_window_input                 = 0x008;
	constexpr DWORD enable_virtual_terminal_input       = 0x200;
	constexpr DWORD enable_extended_flags               = 0x080;

	constexpr DWORD enable_processed_output             = 0x01;
	constexpr DWORD enable_wrap_at_eol_output           = 0x02;
	constexpr DWORD enable_virtual_terminal_processing  = 0x04;
	constexpr DWORD disable_newline_auto_return         = 0x08;
	constexpr DWORD enable_lvb_grid_worldwide           = 0x10;
}

//--    prot                            ///{{{2///////////////////////////////
namespace Win32proto
{
	//-/
	//-/ Windows 2000 / Server 2000
	// From kernel32.dll:
	using CloseHandleFn = Win32::BOOL (FLUXWIN32_API*)( Win32::HANDLE );

	using GetSystemInfoFn = void (FLUXWIN32_API*)( Win32::LPSYSTEM_INFO );
	using GetVersionExA = Win32::BOOL (FLUXWIN32_API*)( Win32::LPOSVERSIONINFOEXA );
	using GetComputerNameAFn = Win32::BOOL (FLUXWIN32_API*)( Win32::LPSTR, Win32::LPDWORD );

	using QueryPerformanceCounterFn = Win32::BOOL (FLUXWIN32_API*)( Win32::LARGE_INTEGER* );
	using QueryPerformanceFrequencyFn = Win32::BOOL (FLUXWIN32_API*)( Win32::LARGE_INTEGER* );

	using MultiByteToWideCharFn = int (FLUXWIN32_API*)(
		Win32::UINT,
		Win32::DWORD,
		Win32::LPCCH,
		int,
		Win32::LPWSTR,
		int
	);

	using GetStdHandleFn = Win32::HANDLE (FLUXWIN32_API*)( Win32::DWORD );
	using GetConsoleModeFn = Win32::BOOL (FLUXWIN32_API*)( Win32::HANDLE, Win32::LPDWORD );
	using SetConsoleModeFn  = Win32::BOOL (FLUXWIN32_API*)( Win32::HANDLE, Win32::DWORD );
	using GetConsoleCPFn = Win32::UINT (FLUXWIN32_API*)();
	using GetConsoleOutputCPFn = Win32::UINT (FLUXWIN32_API*)();
	using SetConsoleCPFn = Win32::BOOL (FLUXWIN32_API*)( Win32::UINT );
	using SetConsoleOutputCPFn = Win32::BOOL (FLUXWIN32_API*)( Win32::UINT );
	using GetConsoleScreenBufferInfoFn = Win32::BOOL (FLUXWIN32_API*)(
		Win32::HANDLE,
		Win32::PCONSOLE_SCREEN_BUFFER_INFO
	);

	// From user32.dll:
	using GetSystemMetricsFn = int (FLUXWIN32_API*)( int );

	//-/
	//-/ Windows XP / Server 2003
	// From kernel32.dll:
	using GetLastErrorFn = Win32::DWORD (FLUXWIN32_API*)();

	using GetProcAddressFn = Win32::FARPROC (FLUXWIN32_API*)( Win32::HMODULE, Win32::LPCSTR );
	using GetModuleHandleAFn = Win32::HMODULE (FLUXWIN32_API*)( Win32::LPCSTR );
	using GetCurrentProcessFn = Win32::HANDLE (FLUXWIN32_API*)();

	using LocalFreeFn = Win32::HLOCAL (FLUXWIN32_API*)( Win32::HLOCAL );

	using GetFileTypeFn = Win32::DWORD (FLUXWIN32_API*)( Win32::HANDLE );
	using GlobalMemoryStatusExFn = Win32::BOOL (FLUXWIN32_API*)( Win32::LPMEMORYSTATUSEX );

	using FormatMessageAFn = Win32::DWORD (FLUXWIN32_API*)(
		Win32::DWORD, 
		Win32::LPCVOID, 
		Win32::DWORD, 
		Win32::DWORD,
		Win32::LPSTR, 
		Win32::DWORD, 
		Win32::va_list* 
	);

	using CreateEventAFn = Win32::HANDLE (FLUXWIN32_API*)(
		Win32::LPSECURITY_ATTRIBUTES, 
		Win32::BOOL aManualReset,
		Win32::BOOL aInitialState,
		Win32::LPCSTR aName
	);
	using ResetEventFn = Win32::BOOL (FLUXWIN32_API*)( Win32::HANDLE );
	using WaitForSingleObjectFn = Win32::DWORD (FLUXWIN32_API*)( Win32::HANDLE, Win32::DWORD );

	using GetCurrentThreadFn = Win32::HANDLE (FLUXWIN32_API*)();
	using GetCurrentThreadIdFn = Win32::DWORD (FLUXWIN32_API*)(void);
	using GetPriorityClassFn = Win32::DWORD (FLUXWIN32_API*)( Win32::HANDLE );
	using SetPriorityClassFn = Win32::BOOL (FLUXWIN32_API*)( Win32::HANDLE, Win32::DWORD );
	using GetThreadPriorityFn = int (FLUXWIN32_API*)( Win32::HANDLE );
	using SetThreadPriorityFn = Win32::BOOL (FLUXWIN32_API*)( Win32::HANDLE, int );
	using GetProcessAffinityMaskFn = Win32::BOOL (FLUXWIN32_API*)( Win32::HANDLE, Win32::PDWORD_PTR, Win32::PDWORD_PTR );
	using SetThreadAffinityMaskFn = Win32::DWORD_PTR (FLUXWIN32_API*)( Win32::HANDLE, Win32::DWORD_PTR );

	//-/
	//-/ Windows Vista / Server 2008
	// From kernel32.dll:
	using GetTickCount64Fn = Win32::ULONGLONG (FLUXWIN32_API*)();
	using GetCurrentProcessorNumberFn = Win32::DWORD (FLUXWIN32_API*)();

	using GetFileInformationByHandleExFn = Win32::BOOL (FLUXWIN32_API*)(
		Win32::DWORD,
		Win32::FILE_INFO_BY_HANDLE_CLASS,
		Win32::LPVOID,
		Win32::DWORD
	);

	//-/
	//-/ Windows 7 / Server 2008 R2
	// From kernel32.dll:
	using GetLogicalProcessorInformationExFn = Win32::BOOL (FLUXWIN32_API*)(
		uint16_t /* LOGICAL_PROCESSOR_RELATIONSHIP */,
		Win32::PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX,
		Win32::PDWORD 
	);

	//-/
	//-/ Windows 8 / Server 2012
	// From api-ms-win-core-synch-l1-2-0.dll (docs claim kernel32.dll):
	using WaitOnAddressFn = Win32::BOOL (FLUXWIN32_API*)( volatile void*, Win32::PVOID, Win32::SIZE_T, Win32::DWORD );
	using WakeByAddressSingleFn = void (FLUXWIN32_API*)( Win32::PVOID );
	using WakeByAddressAllFn = void (FLUXWIN32_API*)( Win32::PVOID );
}

//--    Win32api                        ///{{{2///////////////////////////////
struct Win32api
{
	bool valid;

	bool VERSION_Win2k; // Windows 2000 / Server 2000
	Win32proto::CloseHandleFn closeHandle;

	Win32proto::GetSystemInfoFn getSystemInfo;
	Win32proto::GetVersionExA getVersionExA; // NOTE: officially deprecated
	Win32proto::GetComputerNameAFn getComputerNameA;

	Win32proto::QueryPerformanceCounterFn queryPerformanceCounter;
	Win32proto::QueryPerformanceFrequencyFn queryPerformanceFrequency;

	Win32proto::MultiByteToWideCharFn multiByteToWideChar;

	Win32proto::GetStdHandleFn getStdHandle;
	Win32proto::GetConsoleModeFn getConsoleMode;
	Win32proto::SetConsoleModeFn setConsoleMode;
	Win32proto::GetConsoleCPFn getConsoleCP;
	Win32proto::GetConsoleOutputCPFn getConsoleOutputCP;
	Win32proto::SetConsoleCPFn setConsoleCP;
	Win32proto::SetConsoleOutputCPFn setConsoleOutputCP;
	Win32proto::GetConsoleScreenBufferInfoFn getConsoleScreenBufferInfo;

	Win32proto::GetSystemMetricsFn getSystemMetrics;

	bool VERSION_WinXP; // Windows XP / Server 2003
	Win32proto::GetLastErrorFn getLastError;

	Win32proto::GetProcAddressFn getProcAddress;
	Win32proto::GetModuleHandleAFn getModuleHandleA;
	Win32proto::GetCurrentProcessFn getCurrentProcess;

	Win32proto::LocalFreeFn localFree;

	Win32proto::GetFileTypeFn getFileType;
	Win32proto::GlobalMemoryStatusExFn globalMemoryStatusEx;

	Win32proto::FormatMessageAFn formatMessageA;

	Win32proto::CreateEventAFn createEventA;
	Win32proto::ResetEventFn resetEvent;
	Win32proto::WaitForSingleObjectFn waitForSingleObject;

	Win32proto::GetCurrentThreadFn getCurrentThread;
	Win32proto::GetCurrentThreadIdFn getCurrentThreadId;
	Win32proto::GetPriorityClassFn getPriorityClass;
	Win32proto::SetPriorityClassFn setPriorityClass;
	Win32proto::GetThreadPriorityFn getThreadPriority;
	Win32proto::SetThreadPriorityFn setThreadPriority;
	Win32proto::GetProcessAffinityMaskFn getProcessAffinityMask;
	Win32proto::SetThreadAffinityMaskFn setThreadAffinityMask;

	bool VERSION_WinVista; // Windows Vista / Server 2008
	Win32proto::GetTickCount64Fn getTickCount64; //
	Win32proto::GetCurrentProcessorNumberFn getCurrentProcessorNumber;

	Win32proto::GetFileInformationByHandleExFn getFileInformationByHandleEx;

	bool VERSION_Win7; // Windows 7 / Server 2008 R2
	Win32proto::GetLogicalProcessorInformationExFn getLogicalProcessorInformationEx;

	bool VERSION_Win8; // Windows 8 / Server 2012
	Win32proto::WaitOnAddressFn waitOnAddress;
	Win32proto::WakeByAddressSingleFn wakeByAddressSingle;
	Win32proto::WakeByAddressAllFn wakeByAddressAll;

	std::size_t referenceCount_;
	sys::DylibHandle* libs_; //TODO: Hide?
};

//--    functions                       ///{{{2///////////////////////////////

Win32api const* win32( bool aAutoload = true ) noexcept;

bool acquire_win32( char const* = nullptr );
bool acquire_win32( 	
	void* (*aGetFn)( char const* aFnName, void* aUser ),
	void* aUser
);

void release_win32();

namespace ex
{
	Win32api const* load( Identity<Win32api>, char const* = nullptr );
	Win32api const* load(
		Identity<Win32api>,
		void* (*aGetFn)( char const* aFnName, void* aUser ),
		void* aUser
	);

	void unload( Win32api const* );

	void acquire( Win32api const* );
	void release( Win32api const* );
}

//--    <<< ~ dlapi::os namespace <<<          ///{{{1////////////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_OS_WIN32_HPP_438A117E_6F28_40F0_B461_CB654CF55070
