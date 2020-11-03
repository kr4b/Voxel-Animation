/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	GLFW api loader implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux_dlapi/os/glfw.hpp>

#include <flux/sys/dylib.hpp>

#include <cassert>

#include "load_util.hxx"
#include "load_extended.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = dlapi::os >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
	detail::ApiRecord<GLFWapi> gGlfwApi_;

	GLFWapi* load_api_( void* (*)(char const*,void*), void* );
}

//--    glfw()                          ///{{{2///////////////////////////////
GLFWapi const* glfw( bool aAutoload ) noexcept
{
	if( auto ptr = gGlfwApi_.api() )
		return ptr;

	return aAutoload ? gGlfwApi_.try_autoload() : nullptr;
}

//--    acquire_glfw()                  ///{{{2///////////////////////////////
bool acquire_glfw( char const* aDlibName )
{
	return gGlfwApi_.acquire( aDlibName );
}
bool acquire_glfw( void* (*aGetFn)(char const*,void*), void* aUser )
{
	return gGlfwApi_.acquire( aGetFn, aUser );
}

//--    release_glfw()                  ///{{{2///////////////////////////////
void release_glfw()
{
	return gGlfwApi_.release();
}

//--    ex::unload()                    ///{{{2///////////////////////////////
namespace ex
{
	void unload( GLFWapi const* aGLFW )
	{
		GLFWapi* mptr = const_cast<GLFWapi*>(aGLFW);
		assert( mptr );

		mptr->valid = false;

		release( aGLFW );
	}
}

//--    ex::acquire()                   ///{{{2///////////////////////////////
namespace ex
{
	void acquire( GLFWapi const* aGLFW )
	{
		assert( aGLFW );
		++const_cast<GLFWapi*>(aGLFW)->referenceCount_;
	}
}

//--    ex::release()                   ///{{{2///////////////////////////////
namespace ex
{
	void release( GLFWapi const* aGLFW )
	{
		assert( aGLFW );
		if( 0 == --const_cast<GLFWapi*>(aGLFW)->referenceCount_ )
		{
			if( aGLFW->libHandle_ )
				sys::dylib_close( aGLFW->libHandle_ );

			delete aGLFW;
		}
	}
}

//--    ex::load()                      ///{{{2///////////////////////////////
namespace ex
{
	GLFWapi const* load( Identity<GLFWapi>, void* (*aGetFn)(char const*,void*), void* aUser )
	{
		return load_api_( aGetFn, aUser );
	}

	GLFWapi const* load( Identity<GLFWapi>, char const* aDlibName )
	{
		sys::DylibHandle handle = nullptr;
		if( aDlibName )
		{
			handle = sys::dylib_open( aDlibName );
		}
		else
		{
			using detail::EExtendedOptions;
			auto const opts = 
				  EExtendedOptions::unspecified
				//| EExtendedOptions::vfsSystem | EExtendedOptions::vfsSystem //TODO: need to construct name
				| EExtendedOptions::cached
				| EExtendedOptions::bindist
				| EExtendedOptions::autofetch
			;

			static constexpr char const* paths[] = {
				"glfw3.3.dll",
				"libglfw.so.3.3",
				nullptr
			};

			handle = detail::extended_search(
				opts,
				paths,
				nullptr /*"testso.so.0.0"*/,
				"glfw", "3.3"
			);
		}

		if( handle )
		{
			using FnType_ = void* (*)(char const*, void*);
			FnType_ const get = [] (char const* aName, void* aHandle) {
				auto const handle = static_cast<sys::DylibHandle>(aHandle);
				return (void*)sys::dylib_get_proc( handle, aName );
			};

			if( auto const ret = load_api_( get, handle ) )
			{
				ret->libHandle_ = handle;
				return ret;
			}

			sys::dylib_close( handle );
		}

		return nullptr;
	}
}

//--    $ load_api_()                   ///{{{2///////////////////////////////
namespace
{
	GLFWapi* load_api_( void* (*aGetFn)(char const*,void*), void* aUser )
	{
		assert( aGetFn );
		auto load = [&] (char const* aName) {
			return detail::FnPtrProxy(aGetFn( aName, aUser ));
		};

		GLFWapi* glfw = new GLFWapi();
		glfw->libHandle_ = nullptr;

		bool ok = true;

#		define LOADFN_( fld, name ) ok = !!(glfw->fld = load( name )) && ok
		LOADFN_( init, "glfwInit" );
		LOADFN_( setErrorCallback, "glfwSetErrorCallback" );
		LOADFN_( getVersion, "glfwGetVersion" );
		LOADFN_( getVersionString, "glfwGetVersionString" );
		LOADFN_( terminate, "glfwTerminate" );

		LOADFN_( getMonitors, "glfwGetMonitors" );
		LOADFN_( getPrimaryMonitor, "glfwGetPrimaryMonitor" );
		LOADFN_( getMonitorPos, "glfwGetMonitorPos" );
		LOADFN_( getMonitorPhysicalSize, "glfwGetMonitorPhysicalSize" );
		LOADFN_( getMonitorName, "glfwGetMonitorName" );
		LOADFN_( setMonitorCallback, "glfwSetMonitorCallback" );

		LOADFN_( getVideoModes, "glfwGetVideoModes" );
		LOADFN_( getVideoMode, "glfwGetVideoMode" );
		LOADFN_( setGamma, "glfwSetGamma" );
		LOADFN_( getGammaRamp, "glfwGetGammaRamp" );
		LOADFN_( setGammaRamp, "glfwSetGammaRamp" );

		LOADFN_( defaultWindowHints, "glfwDefaultWindowHints" );
		LOADFN_( windowHint, "glfwWindowHint" );
		LOADFN_( createWindow, "glfwCreateWindow" );
		LOADFN_( getWindowMonitor, "glfwGetWindowMonitor" );
		LOADFN_( setWindowMonitor, "glfwSetWindowMonitor" );
		LOADFN_( setWindowUserPointer, "glfwSetWindowUserPointer" );
		LOADFN_( getWindowUserPointer, "glfwGetWindowUserPointer" );
		LOADFN_( windowShouldClose, "glfwWindowShouldClose" );
		LOADFN_( setWindowShouldClose, "glfwSetWindowShouldClose" );
		LOADFN_( setWindowTitle, "glfwSetWindowTitle" );
		LOADFN_( destroyWindow, "glfwDestroyWindow" );

		LOADFN_( makeContextCurrent, "glfwMakeContextCurrent" );
		LOADFN_( getCurrentContext, "glfwGetCurrentContext" );
		LOADFN_( swapInterval, "glfwSwapInterval" );
		LOADFN_( swapBuffers, "glfwSwapBuffers" );
		LOADFN_( getFramebufferSize, "glfwGetFramebufferSize" );

#		if FLUX_WITH_FLUXVK
		LOADFN_( getRequiredInstanceExtensions, "glfwGetRequiredInstanceExtensions" );
		LOADFN_( getPhysicalDevicePresentationSupport, "glfwGetPhysicalDevicePresentationSupport" );
		LOADFN_( createWindowSurface, "glfwCreateWindowSurface" );
#		endif // ~ WITH_FLUXVK

		LOADFN_( pollEvents, "glfwPollEvents" );
		LOADFN_( waitEvents, "glfwWaitEvents" );
		LOADFN_( waitEventsTimeout, "glfwWaitEventsTimeout" );
		LOADFN_( postEmptyEvent, "glfwPostEmptyEvent" );
		LOADFN_( setWindowPosCallback, "glfwSetWindowPosCallback" );
		LOADFN_( setWindowSizeCallback, "glfwSetWindowSizeCallback" );
		LOADFN_( setWindowCloseCallback, "glfwSetWindowCloseCallback" );
		LOADFN_( setWindowRefreshCallback, "glfwSetWindowRefreshCallback" );
		LOADFN_( setWindowFocusCallback, "glfwSetWindowFocusCallback" );
		LOADFN_( setWindowIconifyCallback, "glfwSetWindowIconifyCallback" );
		LOADFN_( setFramebufferSizeCallback, "glfwSetFramebufferSizeCallback" );
		LOADFN_( setInputMode, "glfwSetInputMode" );
		LOADFN_( setKeyCallback, "glfwSetKeyCallback" );
		LOADFN_( setCursorPosCallback, "glfwSetCursorPosCallback" );
		LOADFN_( setMouseButtonCallback, "glfwSetMouseButtonCallback" );
		LOADFN_( setCursorEnterCallback, "glfwSetCursorEnterCallback" );
		LOADFN_( setScrollCallback, "glfwSetScrollCallback" );
		LOADFN_( setCharCallback, "glfwSetCharCallback" );
		LOADFN_( setDropCallback, "glfwSetDropCallback" );

		LOADFN_( getKeyName, "glfwGetKeyName" );
		LOADFN_( getKeyScancode, "glfwGetKeyScancode" );

		LOADFN_( rawMouseMotionSupported, "glfwRawMouseMotionSupported" );

		LOADFN_( joystickPresent, "glfwJoystickPresent" );
		LOADFN_( getJoystickName, "glfwGetJoystickName" );
		LOADFN_( getJoystickAxes, "glfwGetJoystickAxes" );
		LOADFN_( getJoystickButtons, "glfwGetJoystickButtons" );
		LOADFN_( setJoystickCallback, "glfwSetJoystickCallback" );

		LOADFN_( joystickIsGamepad, "glfwJoystickIsGamepad" );
		LOADFN_( updateGamepadMappings, "glfwUpdateGamepadMappings" );
		LOADFN_( getGamepadName, "glfwGetGamepadName" );
		LOADFN_( getGamepadState, "glfwGetGamepadState" );

		LOADFN_( setClipboardString, "glfwSetClipboardString" );
		LOADFN_( getClipboardString, "glfwGetClipboardString" );
#		undef LOADFN_

		// done.
		if( ok )
		{
			glfw->valid = true;
			glfw->referenceCount_ = 1;
			return glfw;
		}

		delete glfw;
		return nullptr;
	}
}

//--    <<< ~ dlapi::os namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
