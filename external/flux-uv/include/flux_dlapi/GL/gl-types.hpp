/*-******************************************************* -- HEADER -{{{1- */
/*- flux-dlapi_gl : default GL type name alises
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_GL_GL_TYPES_HPP_10772F00_86ED_4357_AC0A_B9EB3E50C38C
#define FLUXDLAPI_GL_GL_TYPES_HPP_10772F00_86ED_4357_AC0A_B9EB3E50C38C

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include "types.hpp"

//--//////////////////////////////////////////////////////////////////////////
//--    compat                          ///{{{2///////////////////////////////

#define FLUXTMP_GLTYPES_ALREADY_DEFINED_ 0

#if defined(__glew_h__) || defined(__GLEW_H__)
#	undef FLUXTMP_GLTYPES_ALREADY_DEFINED_
#	define FLUXTMP_GLTYPES_ALREADY_DEFINED_ 1
#endif
#if defined(__gl_h_) || defined(__GL_H__)
#	undef FLUXTMP_GLTYPES_ALREADY_DEFINED_
#	define FLUXTMP_GLTYPES_ALREADY_DEFINED_ 1
#endif
#if defined(__glext_h_) || defined(__GLEXT_H_)
#	undef FLUXTMP_GLTYPES_ALREADY_DEFINED_
#	define FLUXTMP_GLTYPES_ALREADY_DEFINED_ 1
#endif
#if defined(__gltypes_h_)
#	undef FLUXTMP_GLTYPES_ALREADY_DEFINED_
#	define FLUXTMP_GLTYPES_ALREADY_DEFINED_ 1
#endif
#if defined(__gl_ATI_h_)
#	undef FLUXTMP_GLTYPES_ALREADY_DEFINED_
#	define FLUXTMP_GLTYPES_ALREADY_DEFINED_ 1
#endif

//--//////////////////////////////////////////////////////////////////////////
//--    types                           ///{{{2///////////////////////////////

#if !FLUXTMP_GLTYPES_ALREADY_DEFINED_
using GLenum = flux::dlapi::gl::GL::Enum;
using GLboolean = flux::dlapi::gl::GL::Boolean;
using GLbitfield = flux::dlapi::gl::GL::Bitfield;
using GLvoid = flux::dlapi::gl::GL::Void;
using GLbyte = flux::dlapi::gl::GL::Byte;
using GLshort = flux::dlapi::gl::GL::Short;
using GLint = flux::dlapi::gl::GL::Int;
using GLubyte = flux::dlapi::gl::GL::UByte;
using GLushort = flux::dlapi::gl::GL::UShort;
using GLuint = flux::dlapi::gl::GL::UInt;
using GLsizei = flux::dlapi::gl::GL::Sizei;
using GLfloat = flux::dlapi::gl::GL::Float;
using GLclampf = flux::dlapi::gl::GL::Clampf;
using GLdouble = flux::dlapi::gl::GL::Double;
using GLclampd = flux::dlapi::gl::GL::Clampd;
using GLintptr = flux::dlapi::gl::GL::Intptr;
using GLsizeiptr = flux::dlapi::gl::GL::Sizeiptr;
using GLchar = flux::dlapi::gl::GL::Char;
using GLhalf = flux::dlapi::gl::GL::Half;
using GLint64 = flux::dlapi::gl::GL::Int64;
using GLuint64 = flux::dlapi::gl::GL::UInt64;
using GLsync = flux::dlapi::gl::GL::Sync;
using GLcharARB = flux::dlapi::gl::GL::CharARB;
using GLhandleARB = flux::dlapi::gl::GL::HandleARB;
using GLhalfARB = flux::dlapi::gl::GL::HalfARB;
using GLfixed = flux::dlapi::gl::GL::Fixed;
using GLintptrARB = flux::dlapi::gl::GL::IntptrARB;
using GLsizeiptrARB = flux::dlapi::gl::GL::SizeiptrARB;
using GLint64EXT = flux::dlapi::gl::GL::Int64EXT;
using GLuint64EXT = flux::dlapi::gl::GL::UInt64EXT;
using GLhalfNV = flux::dlapi::gl::GL::HalfNV;
using GLvdpauSurfaceNV = flux::dlapi::gl::GL::VdpauSurfaceNV;

using GLDEBUGPROC = flux::dlapi::gl::GL::DEBUGPROC;
using GLDEBUGPROCARB = flux::dlapi::gl::GL::DEBUGPROCARB;
using GLDEBUGPROCAMD = flux::dlapi::gl::GL::DEBUGPROCAMD;
#endif // ~ FLUXTMP_GLTYPES_ALREADY_DEFINED_

//--//////////////////////////////////////////////////////////////////////////
//--    cleanup                         ///{{{2///////////////////////////////

#undef FLUXTMP_GLTYPES_ALREADY_DEFINED_

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_GL_GL_TYPES_HPP_10772F00_86ED_4357_AC0A_B9EB3E50C38C
