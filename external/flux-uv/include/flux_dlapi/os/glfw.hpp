/*-******************************************************* -- HEADER -{{{1- */
/*- GLFW api subset
 *
 * GLFW:
 * --> 
 * GLFW is an Open Source, multi-platform library for OpenGL, OpenGL ES and
 * Vulkan development on the desktop. It provides a simple API for creating
 * windows, contexts and surfaces, receiving input and events.
 *
 * GLFW is written in C and supports Windows, macOS, the X Window System and
 * the Wayland protocol.
 *
 * GLFW is licensed under the zlib/libpng license.
 * <--
 *
 * See https://www.glfw.org/  and  https://github.com/glfw/glfw 
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_OS_GLFW_HPP_E6ED30A2_D185_4616_86A9_F191A21C375F
#define FLUXDLAPI_OS_GLFW_HPP_E6ED30A2_D185_4616_86A9_F191A21C375F

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>


#include <flux/sys/dylib.hpp>
#include <flux/std/identity.hpp>

#if FLUX_WITH_FLUXVK
#	include <flux_dlapi/VK/vkapi.hpp>
#endif // ~ WITH_FLUXVK

#include <cstddef>
#include <cstdint>

//--    >>> namespace = dlapi::os >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    types                           ///{{{2///////////////////////////////
namespace GLFW
{
	typedef struct WindowOpaque_ Window;
	typedef struct MonitorOpaque_ Monitor;

	struct Vidmode
	{
		int width;
		int height;
		int redBits;
		int greenBits;
		int blueBits;
		int refreshRate;
	};
	struct Gammaramp
	{
		unsigned short* red;
		unsigned short* green;
		unsigned short* blue;
		unsigned int size;
	};
	struct Gamepadstate
	{
		unsigned char buttons[15];
		float axes[6];
	};
}

//--    constants                       ///{{{2///////////////////////////////
namespace GLFW
{
	constexpr int GLFW_FALSE = 0; // too high of a chance of collisions with just FALSE
	constexpr int GLFW_TRUE = 1; // ditto.

	// error codes -{{{3-
	//TODO TODO TODO
	// -}}}3-
	// TODO: what else

	// Note: see section at the bottom for additional definitions, including
	// window hints, key codes and the like.
}

//--    prot                            ///{{{2///////////////////////////////
namespace GLFWproto
{
	// Generic
	using InitFn = int (*)(void);

	using ErrorCb = void (*)(int, char const*);
	using SetErrorCallbackFn = ErrorCb (*)( ErrorCb );

	using GetVersionFn = void (*)(int*, int*, int*);
	using GetVersionStringFn = char const* (*)(void);
	using TerminateFn = void (*)(void);

	// Monitors
	using GetMonitorsFn = GLFW::Monitor** (*)( int* );
	using GetPrimaryMonitorFn = GLFW::Monitor* (*)( void );
	using GetMonitorPosFn = void (*) (GLFW::Monitor*, int*, int* );
	using GetMonitorPhysicalSizeFn = void (*) (GLFW::Monitor*, int*, int*);
	using GetMonitorNameFn = char const* (*) (GLFW::Monitor*);

	using MonitorCb = void (*) (GLFW::Monitor*, int);
	using SetMonitorCallbackFn = MonitorCb (*) (MonitorCb);

	using GetVideoModesFn = GLFW::Vidmode const* (*)(GLFW::Monitor*, int*);
	using GetVideoModeFn = GLFW::Vidmode const* (*)(GLFW::Monitor*);
	using SetGammaFn = void (*) (GLFW::Monitor*, float);
	using GetGammaRampFn = GLFW::Gammaramp const* (*)(GLFW::Monitor*);
	using SetGammaRampFn = void (*)(GLFW::Monitor*, GLFW::Gammaramp const*);

	// Window
	using DefaultWindowHintsFn = void (*)(void);
	using WindowHintFn = void (*)( int, int );
	using CreateWindowFn = GLFW::Window* (*)( int, int, char const*, GLFW::Monitor*, GLFW::Window* );

	using GetWindowMonitorFn = GLFW::Monitor* (*)(GLFW::Window*);
	using SetWindowMonitorFn = void (*)(GLFW::Window*, GLFW::Monitor*, int, int, int, int, int);

	using SetWindowUserPointerFn = void (*)( GLFW::Window*, void* );
	using GetWindowUserPointerFn = void* (*)( GLFW::Window* );

	using WindowShouldCloseFn = int (*) (GLFW::Window*);
	using SetWindowShouldCloseFn = void (*) (GLFW::Window*, int);

	using SetWindowTitleFn = void (*)(GLFW::Window*, char const*);
	
	using DestroyWindowFn = void (*)( GLFW::Window* );

	// GL Context and framebuffer
	using MakeContextCurrentFn = void (*)( GLFW::Window* );
	using GetCurrentContextFn = GLFW::Window* (*)(void);

	using SwapIntervalFn = void (*)( int );
	using SwapBuffersFn = void (*)( GLFW::Window* );

	using GetFramebufferSizeFn = void (*)( GLFW::Window*, int*, int* );

	// VK Context
#	if FLUX_WITH_FLUXVK
	using GetRequiredInstanceExtensionsFn = char const** (*)( std::uint32_t* );
	using GetPhysicalDevicePresentationSupportFn = int (*)( VkInstance, VkPhysicalDevice, std::uint32_t );
	using CreateWindowSurfaceFn = VkResult (*)( VkInstance, GLFW::Window*, VkAllocationCallbacks const*, VkSurfaceKHR* );
#	endif // ~ WITH_FLUXVK

	// Events and Input
	using PollEventsFn = void (*)(void);
	using WaitEventsFn = void (*)(void);
	using WaitEventsTimeoutFn = void (*)( double );

	using PostEmptyEventFn = void (*)(void);

	using WindowPosCb = void (*)(GLFW::Window*, int, int);
	using WindowSizeCb = void (*)(GLFW::Window*, int, int);
	using WindowCloseCb = void (*)(GLFW::Window*);
	using WindowRefreshCb = void (*)(GLFW::Window*);
	using WindowFocusCb = void (*)(GLFW::Window*, int);
	using WindowIconifyCb = void (*)(GLFW::Window*, int);
	using FramebufferResizeCb = void (*)(GLFW::Window*, int, int);

	using SetWindowPosCallbackFn = WindowPosCb (*)(GLFW::Window*, WindowPosCb);
	using SetWindowSizeCallbackFn = WindowSizeCb (*)(GLFW::Window*, WindowSizeCb);
	using SetWindowCloseCallbackFn = WindowCloseCb (*)(GLFW::Window*, WindowCloseCb);
	using SetWindowRefreshCallbackFn = WindowRefreshCb (*)(GLFW::Window*, WindowRefreshCb);
	using SetWindowFocusCallbackFn = WindowFocusCb (*)(GLFW::Window*, WindowFocusCb);
	using SetWindowIconifyCallbackFn = WindowIconifyCb (*)(GLFW::Window*, WindowIconifyCb);
	using SetFramebufferSizeCallbackFn = FramebufferResizeCb (*)(GLFW::Window*, FramebufferResizeCb);

	using SetInputModeFn = void (*)( GLFW::Window*, int, int );

	using KeyCb = void (*) (GLFW::Window*, int, int, int, int);
	using MouseBtnCb = void (*) (GLFW::Window*, int, int, int);
	using CursorPosCb = void (*) (GLFW::Window*, double, double);
	using CursorEnterCb = void (*) (GLFW::Window*, int);
	using ScrollCb = void (*) (GLFW::Window*, double, double);
	using CharCb = void (*) (GLFW::Window*, unsigned int);
	using DropCb = void (*) (GLFW::Window*, int, char const**);

	using SetKeyCallbackFn = KeyCb (*)( GLFW::Window*, KeyCb );
	using SetCursorPosCallbackFn = CursorPosCb (*)( GLFW::Window*, CursorPosCb );
	using SetMouseButtonCallbackFn = MouseBtnCb (*)( GLFW::Window*, MouseBtnCb );
	using SetCursorEnterCallbackFn = CursorEnterCb (*)( GLFW::Window*, CursorEnterCb );
	using SetScrollCallbackFn = ScrollCb (*)( GLFW::Window*, ScrollCb );
	using SetCharCallbackFn = CharCb (*)( GLFW::Window*, CharCb );
	using SetDropCallbackFn = DropCb (*)( GLFW::Window*, DropCb );

	using GetKeyNameFn = char const* (*)( int, int );
	using GetKeyScancodeFn = int (*)( int );

	using RawMouseMotionSupportedFn = int (*)();

	// Joystick stuff
	using JoystickPresentFn = int (*)( int );
	using GetJoystickNameFn  = char const* (*)( int );
	using GetJoystickAxesFn = float const* (*) ( int, int* );
	using GetJoystickButtonsFn = unsigned char const* (*) ( int, int* );

	using JoystickCb = void (*) (int, int);
	using SetJoystickCallbackFn =  JoystickCb (*) ( JoystickCb );

	//TODO: other event callbacks..
	
	using JoystickIsGamepadFn = int (*) ( int );
	using UpdateGamepadMappingsFn = int (*) ( char const* );
	using GetGamepadNameFn = char const* (*) ( int );
	using GetGamepadStateFn = int (*) ( int, GLFW::Gamepadstate* );

	// Clipboard manipulation
	using SetClipboardStringFn = void (*) (GLFW::Window*, char const*);
	using GetClipboardStringFn = char const* (*) (GLFW::Window*);

	//TODO: other stuff
}

//--    GLFWapi                          ///{{{2///////////////////////////////
struct GLFWapi
{
	bool valid;

	GLFWproto::InitFn init;
	GLFWproto::SetErrorCallbackFn setErrorCallback;
	GLFWproto::GetVersionFn getVersion;
	GLFWproto::GetVersionStringFn getVersionString;
	GLFWproto::TerminateFn terminate;

	GLFWproto::GetMonitorsFn getMonitors;
	GLFWproto::GetPrimaryMonitorFn getPrimaryMonitor;
	GLFWproto::GetMonitorPosFn getMonitorPos;
	GLFWproto::GetMonitorPhysicalSizeFn getMonitorPhysicalSize;
	GLFWproto::GetMonitorNameFn getMonitorName;
	GLFWproto::SetMonitorCallbackFn setMonitorCallback;

	GLFWproto::GetVideoModesFn getVideoModes;
	GLFWproto::GetVideoModeFn getVideoMode;
	GLFWproto::SetGammaFn setGamma;
	GLFWproto::GetGammaRampFn getGammaRamp;
	GLFWproto::SetGammaRampFn setGammaRamp;

	GLFWproto::DefaultWindowHintsFn defaultWindowHints;
	GLFWproto::WindowHintFn windowHint;
	GLFWproto::CreateWindowFn createWindow;
	GLFWproto::GetWindowMonitorFn getWindowMonitor;
	GLFWproto::SetWindowMonitorFn setWindowMonitor;
	GLFWproto::SetWindowUserPointerFn setWindowUserPointer;
	GLFWproto::GetWindowUserPointerFn getWindowUserPointer;
	GLFWproto::WindowShouldCloseFn windowShouldClose;
	GLFWproto::SetWindowShouldCloseFn setWindowShouldClose;
	GLFWproto::SetWindowTitleFn setWindowTitle;
	GLFWproto::DestroyWindowFn destroyWindow;

	GLFWproto::MakeContextCurrentFn makeContextCurrent;
	GLFWproto::GetCurrentContextFn getCurrentContext;
	GLFWproto::SwapIntervalFn swapInterval;
	GLFWproto::SwapBuffersFn swapBuffers;
	GLFWproto::GetFramebufferSizeFn getFramebufferSize;

#	if FLUX_WITH_FLUXVK
	GLFWproto::GetRequiredInstanceExtensionsFn getRequiredInstanceExtensions;
	GLFWproto::GetPhysicalDevicePresentationSupportFn getPhysicalDevicePresentationSupport;
	GLFWproto::CreateWindowSurfaceFn createWindowSurface;
#	endif // ~ WITH_FLUXVK

	GLFWproto::PollEventsFn pollEvents;
	GLFWproto::WaitEventsFn waitEvents;
	GLFWproto::WaitEventsTimeoutFn waitEventsTimeout;
	GLFWproto::PostEmptyEventFn postEmptyEvent;
	GLFWproto::SetWindowPosCallbackFn setWindowPosCallback;
	GLFWproto::SetWindowSizeCallbackFn setWindowSizeCallback;
	GLFWproto::SetWindowCloseCallbackFn setWindowCloseCallback;
	GLFWproto::SetWindowRefreshCallbackFn setWindowRefreshCallback;
	GLFWproto::SetWindowFocusCallbackFn setWindowFocusCallback;
	GLFWproto::SetWindowIconifyCallbackFn setWindowIconifyCallback;
	GLFWproto::SetFramebufferSizeCallbackFn setFramebufferSizeCallback;
	GLFWproto::SetInputModeFn setInputMode;
	GLFWproto::SetKeyCallbackFn setKeyCallback;
	GLFWproto::SetCursorPosCallbackFn setCursorPosCallback;
	GLFWproto::SetMouseButtonCallbackFn setMouseButtonCallback;
	GLFWproto::SetCursorEnterCallbackFn setCursorEnterCallback;
	GLFWproto::SetScrollCallbackFn setScrollCallback;
	GLFWproto::SetCharCallbackFn setCharCallback;
	GLFWproto::SetDropCallbackFn setDropCallback;

	GLFWproto::GetKeyNameFn getKeyName;
	GLFWproto::GetKeyScancodeFn getKeyScancode;

	GLFWproto::RawMouseMotionSupportedFn rawMouseMotionSupported;

	GLFWproto::JoystickPresentFn joystickPresent;
	GLFWproto::GetJoystickNameFn getJoystickName;
	GLFWproto::GetJoystickAxesFn getJoystickAxes;
	GLFWproto::GetJoystickButtonsFn getJoystickButtons;
	GLFWproto::SetJoystickCallbackFn setJoystickCallback;

	GLFWproto::JoystickIsGamepadFn joystickIsGamepad;
	GLFWproto::UpdateGamepadMappingsFn updateGamepadMappings;
	GLFWproto::GetGamepadNameFn getGamepadName;
	GLFWproto::GetGamepadStateFn getGamepadState;

	GLFWproto::SetClipboardStringFn setClipboardString;
	GLFWproto::GetClipboardStringFn getClipboardString;


	std::size_t referenceCount_;
	sys::DylibHandle libHandle_; //TODO: Hide?
};

//--    functions                       ///{{{2///////////////////////////////

GLFWapi const* glfw( bool aAutoload = true ) noexcept;

bool acquire_glfw( char const* = nullptr );
bool acquire_glfw(
	void* (*aGetFn)( char const* aFnName, void* aUser ),
	void* aUser
);

void release_glfw();


namespace ex
{
	/* "Extended" API that enables loading of multiple copies of the GLFW API.
	 * This is an older revision of the API exposed here. Since the new API
	 * builds on it, I am leaving this in (hidden in a custom namespace) for
	 * the moment.
	 */
	GLFWapi const* load( Identity<GLFWapi>, char const* = nullptr );
	GLFWapi const* load(
		 Identity<GLFWapi>,
		void* (*aGetFn)( char const* aFnName, void* aUser ),
		void* aUser
	);

	void unload( GLFWapi const* );

	void acquire( GLFWapi const* );
	void release( GLFWapi const* );
}

//--    more constants                  ///{{{2///////////////////////////////
namespace GLFW
{
	// The following is (obivously) largely based on the glfw.h header.

	// Window hints -{{{3-
	constexpr int FOCUSED = 0x20001;
	constexpr int ICONIFIED = 0x20002;
	constexpr int RESIZABLE = 0x20003;
	constexpr int VISIBLE = 0x20004;
	constexpr int DECORATED = 0x20005;
	constexpr int AUTO_ICONIFY = 0x20006;
	constexpr int FLOATING = 0x20007;
	constexpr int MAXIMIZED = 0x20008;

	constexpr int RED_BITS = 0x21001;
	constexpr int GREEN_BITS = 0x21002;
	constexpr int BLUE_BITS = 0x21003;
	constexpr int ALPHA_BITS = 0x21004;
	constexpr int DEPTH_BITS = 0x21005;
	constexpr int STENCIL_BITS = 0x21006;
	constexpr int ACCUM_RED_BITS = 0x21007;
	constexpr int ACCUM_GREEN_BITS = 0x21008;
	constexpr int ACCUM_BLUE_BITS = 0x21009;
	constexpr int ACCUM_ALPHA_BITS = 0x2100A;
	constexpr int AUX_BUFFERS = 0x2100B;
	constexpr int STEREO = 0x2100C;
	constexpr int SAMPLES = 0x2100D;
	constexpr int SRGB_CAPABLE = 0x2100E;
	constexpr int REFRESH_RATE = 0x2100F;
	constexpr int DOUBLEBUFFER = 0x21010;

	constexpr int CLIENT_API = 0x22001;
	constexpr int CONTEXT_VERSION_MAJOR = 0x22002;
	constexpr int CONTEXT_VERSION_MINOR = 0x22003;
	constexpr int CONTEXT_REVISION = 0x22004;
	constexpr int CONTEXT_ROBUSTNESS = 0x22005;
	constexpr int OPENGL_FORWARD_COMPAT = 0x22006;
	constexpr int OPENGL_DEBUG_CONTEXT = 0x22007;
	constexpr int OPENGL_PROFILE = 0x22008;
	constexpr int CONTEXT_RELEASE_BEHAVIOR = 0x22009;
	constexpr int CONTEXT_NO_ERROR = 0x2200A;
	constexpr int CONTEXT_CREATION_API = 0x2200B;

	constexpr int NO_API = 0;
	constexpr int OPENGL_API = 0x30001;
	constexpr int OPENGL_ES_API = 0x30002;

	constexpr int NO_ROBUSTNESS = 0;
	constexpr int NO_RESET_NOTIFICATION = 0x31001;
	constexpr int LOSE_CONTEXT_ON_RESET = 0x31002;

	constexpr int OPENGL_ANY_PROFILE = 0;
	constexpr int OPENGL_CORE_PROFILE = 0x32001;
	constexpr int OPENGL_COMPAT_PROFILE = 0x32002;
	// -}}}3-
	
	// Mouse buttons -{{{3-
	constexpr int MOUSE_BUTTON_1 = 0;
	constexpr int MOUSE_BUTTON_2 = 1;
	constexpr int MOUSE_BUTTON_3 = 2;
	constexpr int MOUSE_BUTTON_4 = 3;
	constexpr int MOUSE_BUTTON_5 = 4;
	constexpr int MOUSE_BUTTON_6 = 5;
	constexpr int MOUSE_BUTTON_7 = 6;
	constexpr int MOUSE_BUTTON_8 = 7;

	constexpr int MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1;
	constexpr int MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2;
	constexpr int MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3;
	// -}}}3-

	// Keys -{{{3-
	constexpr int KEY_UNKNOWN = -1;
	constexpr int KEY_SPACE = 32;
	constexpr int KEY_APOSTROPHE = 39;
	constexpr int KEY_COMMA = 44;
	constexpr int KEY_MINUS = 45;
	constexpr int KEY_PERIOD = 46;
	constexpr int KEY_SLASH = 47;
	constexpr int KEY_0 = 48;
	constexpr int KEY_1 = 49;
	constexpr int KEY_2 = 50;
	constexpr int KEY_3 = 51;
	constexpr int KEY_4 = 52;
	constexpr int KEY_5 = 53;
	constexpr int KEY_6 = 54;
	constexpr int KEY_7 = 55;
	constexpr int KEY_8 = 56;
	constexpr int KEY_9 = 57;
	constexpr int KEY_SEMICOLON = 59;
	constexpr int KEY_EQUAL = 61;
	constexpr int KEY_A = 65;
	constexpr int KEY_B = 66;
	constexpr int KEY_C = 67;
	constexpr int KEY_D = 68;
	constexpr int KEY_E = 69;
	constexpr int KEY_F = 70;
	constexpr int KEY_G = 71;
	constexpr int KEY_H = 72;
	constexpr int KEY_I = 73;
	constexpr int KEY_J = 74;
	constexpr int KEY_K = 75;
	constexpr int KEY_L = 76;
	constexpr int KEY_M = 77;
	constexpr int KEY_N = 78;
	constexpr int KEY_O = 79;
	constexpr int KEY_P = 80;
	constexpr int KEY_Q = 81;
	constexpr int KEY_R = 82;
	constexpr int KEY_S = 83;
	constexpr int KEY_T = 84;
	constexpr int KEY_U = 85;
	constexpr int KEY_V = 86;
	constexpr int KEY_W = 87;
	constexpr int KEY_X = 88;
	constexpr int KEY_Y = 89;
	constexpr int KEY_Z = 90;
	constexpr int KEY_LEFT_BRACKET = 91;
	constexpr int KEY_BACKSLASH = 92;
	constexpr int KEY_RIGHT_BRACKET = 93;
	constexpr int KEY_GRAVE_ACCENT = 96;
	constexpr int KEY_WORLD_1 = 161;
	constexpr int KEY_WORLD_2 = 162;
	constexpr int KEY_ESCAPE = 256;
	constexpr int KEY_ENTER = 257;
	constexpr int KEY_TAB = 258;
	constexpr int KEY_BACKSPACE = 259;
	constexpr int KEY_INSERT = 260;
	constexpr int KEY_DELETE = 261;
	constexpr int KEY_RIGHT = 262;
	constexpr int KEY_LEFT = 263;
	constexpr int KEY_DOWN = 264;
	constexpr int KEY_UP = 265;
	constexpr int KEY_PAGE_UP = 266;
	constexpr int KEY_PAGE_DOWN = 267;
	constexpr int KEY_HOME = 268;
	constexpr int KEY_END = 269;
	constexpr int KEY_CAPS_LOCK = 280;
	constexpr int KEY_SCROLL_LOCK = 281;
	constexpr int KEY_NUM_LOCK = 282;
	constexpr int KEY_PRINT_SCREEN = 283;
	constexpr int KEY_PAUSE = 284;
	constexpr int KEY_F1 = 290;
	constexpr int KEY_F2 = 291;
	constexpr int KEY_F3 = 292;
	constexpr int KEY_F4 = 293;
	constexpr int KEY_F5 = 294;
	constexpr int KEY_F6 = 295;
	constexpr int KEY_F7 = 296;
	constexpr int KEY_F8 = 297;
	constexpr int KEY_F9 = 298;
	constexpr int KEY_F10 = 299;
	constexpr int KEY_F11 = 300;
	constexpr int KEY_F12 = 301;
	constexpr int KEY_F13 = 302;
	constexpr int KEY_F14 = 303;
	constexpr int KEY_F15 = 304;
	constexpr int KEY_F16 = 305;
	constexpr int KEY_F17 = 306;
	constexpr int KEY_F18 = 307;
	constexpr int KEY_F19 = 308;
	constexpr int KEY_F20 = 309;
	constexpr int KEY_F21 = 310;
	constexpr int KEY_F22 = 311;
	constexpr int KEY_F23 = 312;
	constexpr int KEY_F24 = 313;
	constexpr int KEY_F25 = 314;
	constexpr int KEY_KP_0 = 320;
	constexpr int KEY_KP_1 = 321;
	constexpr int KEY_KP_2 = 322;
	constexpr int KEY_KP_3 = 323;
	constexpr int KEY_KP_4 = 324;
	constexpr int KEY_KP_5 = 325;
	constexpr int KEY_KP_6 = 326;
	constexpr int KEY_KP_7 = 327;
	constexpr int KEY_KP_8 = 328;
	constexpr int KEY_KP_9 = 329;
	constexpr int KEY_KP_DECIMAL = 330;
	constexpr int KEY_KP_DIVIDE = 331;
	constexpr int KEY_KP_MULTIPLY = 332;
	constexpr int KEY_KP_SUBTRACT = 333;
	constexpr int KEY_KP_ADD = 334;
	constexpr int KEY_KP_ENTER = 335;
	constexpr int KEY_KP_EQUAL = 336;
	constexpr int KEY_LEFT_SHIFT = 340;
	constexpr int KEY_LEFT_CONTROL = 341;
	constexpr int KEY_LEFT_ALT = 342;
	constexpr int KEY_LEFT_SUPER = 343;
	constexpr int KEY_RIGHT_SHIFT = 344;
	constexpr int KEY_RIGHT_CONTROL = 345;
	constexpr int KEY_RIGHT_ALT = 346;
	constexpr int KEY_RIGHT_SUPER = 347;
	constexpr int KEY_MENU = 348;
	// -}}}3-

	// Key modifiers -{{{3-
	// Renamed from MOD_XYZ to KEYMOD_XYZ to avoid collisions with #defines
	// in <windows.h>.
	constexpr int KEYMOD_SHIFT = 1;
	constexpr int KEYMOD_CONTROL = 2;
	constexpr int KEYMOD_ALT = 4;
	constexpr int KEYMOD_SUPER = 8;
	// -}}}3-

	// Key/button actions -{{{3-
	constexpr int RELEASE = 0;
	constexpr int PRESS = 1;
	constexpr int REPEAT = 2;
	// -}}}3-
	
	// Joysticks -{{{3-
	constexpr int JOYSTICK_1 = 0;
	constexpr int JOYSTICK_2 = 1;
	constexpr int JOYSTICK_3 = 2;
	constexpr int JOYSTICK_4 = 3;
	constexpr int JOYSTICK_5 = 4;
	constexpr int JOYSTICK_6 = 5;
	constexpr int JOYSTICK_7 = 6;
	constexpr int JOYSTICK_8 = 7;
	constexpr int JOYSTICK_9 = 8;
	constexpr int JOYSTICK_10 = 9;
	constexpr int JOYSTICK_11 = 10;
	constexpr int JOYSTICK_12 = 11;
	constexpr int JOYSTICK_13 = 12;
	constexpr int JOYSTICK_14 = 13;
	constexpr int JOYSTICK_15 = 14;
	constexpr int JOYSTICK_16 = 15;
	constexpr int JOYSTICK_LAST = JOYSTICK_16;
	// TODO: JOYSTICK_*

	// Misc. -{{{3-
	constexpr int CURSOR = 0x33001;
	constexpr int CURSOR_NORMAL = 0x34001;
	constexpr int CURSOR_HIDDEN = 0x34002;
	constexpr int CURSOR_DISABLED = 0x34003;

	constexpr int RAW_MOUSE_MOTION = 0x33005;

	constexpr int CONNECTED = 0x40001;
	constexpr int DISCONNECTED = 0x40002;
	// -}}}3-

	// error codes -{{{3-
	//TODO TODO TODO
	// -}}}3-
}


//--    <<< ~ dlapi::os namespace <<<          ///{{{1////////////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_OS_GLFW_HPP_E6ED30A2_D185_4616_86A9_F191A21C375F
