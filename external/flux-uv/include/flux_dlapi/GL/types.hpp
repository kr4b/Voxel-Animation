/*-******************************************************* -- HEADER -{{{1- */
/*- flux-dlapi_gl GL types
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_GL_TYPES_HPP_E335EFF3_C206_4F90_8482_FE0A87C75B43
#define FLUXDLAPI_GL_TYPES_HPP_E335EFF3_C206_4F90_8482_FE0A87C75B43

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include "forward.hpp"

#include <cstddef>
#include <cstdint>

//--    >>> namespace = dlapi::gl >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(dlapi,gl)
//--    FLUXGL_APIENTRY                 ///{{{2///////////////////////////////

#ifndef FLUX_APIENTRY
	// Argh! Avoid <windows.h> and its plenthora of useless definitions.
#	if defined(__MINGW32__)
#		define FLUXGL_APIENTRY __stdcall
#	elif (defined(_MSC_VER) && _MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
#		define FLUXGL_APIENTRY __stdcall
#	else
#		define FLUXGL_APIENTRY
#	endif
/*	#if defined(__MINGW32__)
		#ifndef WIN32_LEAN_AND_MEAN
			#define WIN32_LEAN_AND_MEAN 1
		#endif
		#ifndef NOMINMAX
			#define NOMINMAX
		#endif
		#include <windows.h>
	#elif (defined(_MSC_VER) && _MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
		#ifndef WIN32_LEAN_AND_MEAN
			#define WIN32_LEAN_AND_MEAN 1
		#endif
		#ifndef NOMINMAX
			#define NOMINMAX
		#endif
		#include <windows.h>
	#else
		#define APIENTRY
	#endif */
#endif /*APIENTRY*/

#ifndef FLUXGL_API
	#if defined(_WIN32)
		#define FLUXGL_API FLUXGL_APIENTRY
	#else
		#define FLUXGL_API
	#endif
#endif /*CODEGEN_FUNCPTR*/

//--    types                           ///{{{2///////////////////////////////
namespace GL
{
	using Enum = unsigned int;
	using Boolean = unsigned char;
	using Bitfield = unsigned int;
	using Void = void;
	using Byte = signed char;
	using Short = short;
	using Int = int;
	using UByte = unsigned char;
	using UShort = unsigned short;
	using UInt = unsigned int;
	using Sizei = int;
	using Float = float;
	using Clampf = float;
	using Double = double;
	using Clampd = double;

	// GL_1_5
	using Intptr = std::ptrdiff_t;
	using Sizeiptr = std::ptrdiff_t;

	// GL_2_0
	using Char = char;

	// GL_3_0
	using Half = unsigned short;

	// GL_3_5
	using Int64 = std::int64_t;
	using UInt64 = std::uint64_t;

	struct Sync_;
	using Sync = Sync_*;

	// ARB_shader_objects
	using CharARB = char;
#	ifdef __APPLE__
	using HandleARB = void*;
#	else
	using HandleARB = unsigned int;
#	endif

	// ARB_half_float_pixel
	using HalfARB = unsigned short;

	// OES_fixed_point
	using Fixed = int;

	// ARB_vertex_buffer_object
	using IntptrARB = std::ptrdiff_t;
	using SizeiptrARB = std::ptrdiff_t;
	
	// AMD_gpu_shader_int64
	using Int64EXT = std::int64_t;

	// ARB_bindless_texture
	using UInt64EXT = std::uint64_t;

	// NV_half_float
	using HalfNV = unsigned short;

	// NV_vdpau_interop
	using VdpauSurfaceNV = Intptr;

	// Misc:
	//struct _cl_context;
	//struct _cl_event;
	
	// Callbacks
	typedef void (FLUXGL_APIENTRY *DEBUGPROC)(Enum source,Enum type,UInt id,Enum severity,Sizei length,const char *message,const void *userParam);
	typedef void (FLUXGL_APIENTRY *DEBUGPROCARB)(Enum source,Enum type,UInt id,Enum severity,Sizei length,const char *message,const void *userParam);
	typedef void (FLUXGL_APIENTRY *DEBUGPROCAMD)(UInt id,Enum category,Enum severity,Sizei length,const char *message,void *userParam);
}

//--    <<< ~ dlapi::gl namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_GL_TYPES_HPP_E335EFF3_C206_4F90_8482_FE0A87C75B43
