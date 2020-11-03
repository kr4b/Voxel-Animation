/*-******************************************************* -- HEADER -{{{1- */
/*- WGL-WGL api subset
 *
 * See notes in wgl.hpp
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_OS_WGL_HPP_99705F90_CEDD_4ADF_B060_A9D131D110A9
#define FLUXDLAPI_OS_WGL_HPP_99705F90_CEDD_4ADF_B060_A9D131D110A9

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/sys/dylib.hpp>

#include <flux_dlapi/os/win32.hpp>

#include <cstddef>

//--    >>> namespace = dlapi::os >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    types                           ///{{{2///////////////////////////////
namespace WGL
{
	typedef struct HGLRCOpaque_* HGLRC;
}

//--    constants                       ///{{{2///////////////////////////////
namespace WGL
{
	constexpr int CONTEXT_MAJOR_VERSION_ARB = 0x2091;
	constexpr int CONTEXT_MINOR_VERSION_ARB = 0x2092;
	constexpr int CONTEXT_LAYER_PLANE_ARB = 0x2093;
	constexpr int CONTEXT_FLAGS_ARB = 0x2094;
	constexpr int CONTEXT_PROFILE_MASK_ARB = 0x0126;
	
	constexpr int CONTEXT_DEBUG_BIT_ARB = 1;
	constexpr int CONTEXT_FORWARD_COMPATIBLE_BIT_ARB = 2;

	constexpr int CONTEXT_CORE_PROFILE_BIT_ARB = 1;
	constexpr int CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB = 2;
}

//--    prot                            ///{{{2///////////////////////////////
namespace WGLproto
{
	// Generic
	using GetProcAddressFn = Win32::PROC (FLUXWIN32_API*)( Win32::LPCSTR );
	
	// Context management
	using GetCurrentDCFn = Win32::HDC (FLUXWIN32_API*)();
	using GetCurrentContextFn = WGL::HGLRC (FLUXWIN32_API*)();
	using CreateContextFn = WGL::HGLRC (FLUXWIN32_API*)( Win32::HDC );
	using DeleteContextFn = Win32::BOOL (FLUXWIN32_API*)( WGL::HGLRC );
	using MakeCurrentFn = Win32::BOOL (FLUXWIN32_API*)( Win32::HDC, WGL::HGLRC );

	using ShareListsFn = Win32::BOOL (FLUXWIN32_API*)( WGL::HGLRC, WGL::HGLRC );

	// Framebuffer / rendering
	//TODO: SwapBuffers


	// Extensions
	using CreateContextAttribsARBFn = WGL::HGLRC (FLUXWIN32_API*)( Win32::HDC, WGL::HGLRC, int const* );
}

//--    WGLapi                        ///{{{2///////////////////////////////
struct WGLapi
{
	bool valid;

	WGLproto::GetProcAddressFn getProcAddress;

	WGLproto::GetCurrentDCFn getCurrentDC;
	WGLproto::GetCurrentContextFn getCurrentContext;
	WGLproto::CreateContextFn createContext;
	WGLproto::DeleteContextFn deleteContext;
	WGLproto::MakeCurrentFn makeCurrent;

	WGLproto::ShareListsFn shareLists;

	// Note: the following are *NOT* loaded by default
	bool ARB_create_context;
	WGLproto::CreateContextAttribsARBFn createContextAttribsARB;


	std::size_t referenceCount_;
	sys::DylibHandle libHandle_; //TODO: Hide?
};


//--    functions                       ///{{{2///////////////////////////////

/* Note: this doesn't have the "new" API where there is a global API instance.
 * We potentially have to deal with multiple WGLapi instances due to the
 * extensions that require a GL context to exist.
 */

WGLapi const* load_wgl( char const* = nullptr );
WGLapi const* load_wgl(
	void* (*aGetFn)( char const* aFnName, void* aUser ),
	void* aUser
);

/** Load WGL extensions
 * 
 * Loads WGL extension pointers. This requires a WGL context to be current. 
 * Returns true if it succeeded in loading *any* extension, and false if none
 * could be loaded.
 * 
 * \warning The `WGLapi` structure must be one returned by `load_wgl()`. The
 * contents of the structure are updated in-place, despite being passed in by
 * a `const` handle.
 */
bool load_wgl_extensions( WGLapi const* );

void unload( WGLapi const* );


void acquire( WGLapi const* );
void release( WGLapi const* );

//--    <<< ~ dlapi::os namespace <<<          ///{{{1////////////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_OS_WGL_HPP_99705F90_CEDD_4ADF_B060_A9D131D110A9
