/*-******************************************************* -- HEADER -{{{1- */
/*- GLX api subset
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_OS_GLX_HPP_DA9DC5B2_FBC2_4BE0_A5E6_C04603CFF48E
#define FLUXDLAPI_OS_GLX_HPP_DA9DC5B2_FBC2_4BE0_A5E6_C04603CFF48E

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/sys/dylib.hpp>

#include <flux_dlapi/os/x11.hpp>

#include <cstddef>

//--    >>> namespace = dlapi::os >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    types                           ///{{{2///////////////////////////////
namespace GLX
{
	typedef struct ContextOpaque_* Context;
	typedef struct FBConfigOpaque_* FBConfig;

	using Drawable = X11::XID;

	/* Redefine GLubyte here, since this avoids a somewhat annoying dependency
	 * on all the OpenGL definitions. The only function that uses it currently
	 * is `GetProcAddressFn`/`glxGetProcAddress`.
	 */
	using GLubyte = unsigned char;
}

//--    constants                       ///{{{2///////////////////////////////
namespace GLX
{
	// attributes for ChooseFBConfig et al. -{{{3-
	constexpr int SCREEN = 0x800c;
	constexpr int RENDER_TYPE = 0x8011;
	constexpr int FBCONFIG_ID = 0x8013;
	constexpr int RGBA_TYPE = 0x8014;
	constexpr int COLOR_INDEX_TYPE = 0x8015;
	// -}}}3-
	
	// misc -{{{3-
	constexpr int RGBA_BIT = 1;
	constexpr int COLOR_INDEX_BIT = 2; // meh.
	// -}}}3-
	
	// ARB_create_context -{{{3-
	constexpr int CONTEXT_MAJOR_VERSION_ARB = 0x2091;
	constexpr int CONTEXT_MINOR_VERSION_ARB = 0x2092;
	constexpr int CONTEXT_FLAGS_ARB = 0x2094;
	constexpr int CONTEXT_PROFILE_MASK_ARB = 0x9126;

	constexpr int CONTEXT_DEBUG_BIT_ARB = 1;
	constexpr int CONTEXT_FORWARD_COMPATIBLE_BIT_ARB = 2;

	constexpr int CONTEXT_CORE_PROFILE_BIT_ARB = 1;
	constexpr int CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB = 2;
	// -}}}3-
}

//--    prot                            ///{{{2///////////////////////////////
namespace GLXproto
{
	// Generic
	using QueryVersionFn = X11::Bool (*)( X11::Display*, int*, int* );
	using QueryExtensionsStringFn = char const* (*)( X11::Display*, int );
	using IsDirectFn = X11::Bool (*)( X11::Display*, GLX::Context );

	using VoidFn = void (*)();
	using GetProcAddressFn = VoidFn (*)( GLX::GLubyte const* );

	// Context management
	using GetCurrentContextFn = GLX::Context (*)(void);
	using GetCurrentDisplayFn = X11::Display* (*)(void);
	using GetCurrentDrawableFn = GLX::Drawable (*)(void);
	using CreateNewContextFn = GLX::Context (*)( X11::Display*, GLX::FBConfig, int, GLX::Context, X11::Bool );
	using DestroyContextFn = void (*)( X11::Display*, GLX::Context );

	using MakeCurrentFn = X11::Bool (*)( X11::Display*, GLX::Drawable, GLX::Context );


	// Misc
	using QueryContextFn = int (*)( X11::Display*, GLX::Context, int, int* );
	using QueryDrawableFn = int (*)( X11::Display*, GLX::Drawable, int, unsigned int* );
	
	using ChooseFBConfigFn = GLX::FBConfig* (*)( X11::Display*, int, int const*, int* );
	using GetFBConfigAttribFn = int (*)( X11::Display*, GLX::FBConfig, int, int* );

	// Framebuffer / rendering
	using SwapBuffersFn = void (*)( X11::Display*, GLX::Drawable );


	// Extensions
	using CreateContextAttribsARBFn = GLX::Context (*)( X11::Display*, GLX::FBConfig, GLX::Context, X11::Bool, int const* );
}

//--    GLXapi                          ///{{{2///////////////////////////////
struct GLXapi
{
	bool valid;

	GLXproto::QueryVersionFn queryVersion;
	GLXproto::QueryExtensionsStringFn queryExtensionsString;
	GLXproto::IsDirectFn isDirect;
	GLXproto::GetProcAddressFn getProcAddress;

	GLXproto::GetCurrentContextFn getCurrentContext;
	GLXproto::GetCurrentDisplayFn getCurrentDisplay;
	GLXproto::GetCurrentDrawableFn getCurrentDrawable;
	GLXproto::CreateNewContextFn createNewContext;
	GLXproto::DestroyContextFn destroyContext;
	GLXproto::MakeCurrentFn makeCurrent;

	GLXproto::QueryContextFn queryContext;
	GLXproto::QueryDrawableFn queryDrawable;
	GLXproto::ChooseFBConfigFn chooseFBConfig;
	GLXproto::GetFBConfigAttribFn getFBConfigAttrib;

	GLXproto::SwapBuffersFn swapBuffers;

	// Extensions
	bool ARB_create_context;
	GLXproto::CreateContextAttribsARBFn createContextAttribsARB;

	std::size_t referenceCount_;
	sys::DylibHandle libHandle_; //TODO: Hide?
};


//--    functions                       ///{{{2///////////////////////////////

/* Note: this doesn't have the "new" API where there is a global API instance.
 * We potentially have to deal with multiple GLXapi instances due to the
 * extensions that require a GL context to exist.
 */

GLXapi const* load_glx( char const* = nullptr );
GLXapi const* load_glx(
	void* (*aGetFn)( char const* aFnName, void* aUser ),
	void* aUser
);

/** Load GLX extensions
 * 
 * LoadsGLX extension pointers. This requires a GLX context to be current. 
 * Returns true if it succeeded in loading *any* extension, and false if none
 * could be loaded.
 * 
 * \warning The `GLXapi` structure must be one returned by `load_glx()`. The
 * contents of the structure are updated in-place, despite being passed in by
 * a `const` handle.
 */
bool load_glx_extensions( GLXapi const* );

void unload( GLXapi const* );


void acquire( GLXapi const* );
void release( GLXapi const* );

//--    <<< ~ dlapi::os namespace <<<          ///{{{1////////////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_OS_GLX_HPP_DA9DC5B2_FBC2_4BE0_A5E6_C04603CFF48E
