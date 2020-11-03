/*-******************************************************* -- HEADER -{{{1- */
/*-	Map files into memory
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_FILEMAP_HPP_CC0FCD43_1095_47A3_AD92_26F2703CC5FB
#define FLUX_SYS_FILEMAP_HPP_CC0FCD43_1095_47A3_AD92_26F2703CC5FB

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/platform.hpp>

#include <flux/compat/system_error.hpp>

#include <flux/sys/syspath.hpp>
#include <flux/std/optional.hpp>

#if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
#	include <flux/sys/win32/windows.h>
#endif // ~ PLATFORM_WIN32

#include <cstddef>
#include <cstdint>

#include <string>

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    constants                   ///{{{1///////////////////////////////////

enum class EFileMapMode
{
	ro,
	wo,
	rw
};

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
constexpr bool kFileMapSupportPartialUnmap = true;
constexpr bool kFileMapSupportOnlineResize = false; //TODO
#else // PLATFORM_WIN32
constexpr bool kFileMapSupportPartialUnmap = false;
constexpr bool kFileMapSupportOnlineResize = false; //TODO
#endif // ~ PLATFORM

//TODO: OnlineResize? => file_map_view_resize() ???

//--    types                       ///{{{1///////////////////////////////////
#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
struct FileMapInfo
{
	int fd;
	std::uintmax_t size;

	FileMapInfo();
};
#else // PLATFORM_WIN32
struct FileMapInfo
{
	HANDLE file;
	HANDLE mapping;
	std::uintmax_t size;

	FileMapInfo();
};
#endif // ~ PLATFORM

//--    functions                   ///{{{1///////////////////////////////////

FileMapInfo file_map_prepare(
	SysPathView const&,
	EFileMapMode = EFileMapMode::ro,
	bool aMustExist = true
);
optional<FileMapInfo> file_map_prepare(
	SysPathView const&,
	EFileMapMode,
	bool aMustExist,
	compat::error_code&
) noexcept;

void file_map_update( FileMapInfo& );
bool file_map_update( FileMapInfo&, compat::error_code& ) noexcept;

void file_map_resize( FileMapInfo&, std::uintmax_t ); //TODO-docs: can't have active view...
bool file_map_resize( FileMapInfo&, std::uintmax_t, compat::error_code& ) noexcept; //TODO-docs: can't have active view...

void file_map_destroy( FileMapInfo& ) noexcept;

void* file_map_view_create(
	FileMapInfo const&,
	EFileMapMode = EFileMapMode::ro,
	std::uintmax_t = 0,
	std::uintmax_t = ~std::uintmax_t(0)
);
void* file_map_view_create(
	FileMapInfo const&,
	EFileMapMode,
	std::uintmax_t,
	std::uintmax_t,
	compat::error_code&
) noexcept;

void file_map_view_delete(
	FileMapInfo const&,
	void*,
	std::uintmax_t = ~std::uintmax_t(0)
);
bool file_map_view_delete(
	FileMapInfo const&,
	void*,
	std::uintmax_t,
	compat::error_code&
) noexcept;


void file_map_view_flush(
	FileMapInfo const&,
	void*,
	std::uintmax_t = ~std::uintmax_t(0),
	bool aAsync = false
);
bool file_map_view_flush(
	FileMapInfo const&,
	void*,
	std::uintmax_t,
	bool aAsync,
	compat::error_code&
) noexcept;


std::uintmax_t file_map_view_max_size( FileMapInfo const&, std::uintmax_t = 0 );
optional<std::uintmax_t> file_map_view_max_size( FileMapInfo const&, std::uintmax_t, compat::error_code& ) noexcept;

std::uintmax_t file_map_offset_minimum_alignment() noexcept;


std::string to_string( EFileMapMode );

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_FILEMAP_HPP_CC0FCD43_1095_47A3_AD92_26F2703CC5FB
