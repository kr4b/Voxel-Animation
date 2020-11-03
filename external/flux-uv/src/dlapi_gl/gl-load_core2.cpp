/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	flux-dlapi_gl loader
 *
 * WARNING: auto-generated file! do not change!
 */
/*-***************************************************************** -}}}1- */

// generated on 2020-08-17 12:18

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux_dlapi/GL/gl.hpp>
#include <flux_dlapi/GL/load.hpp>

#include <cassert>
#include <cstring>

#include "gl_load.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = dlapi::gl::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER3(dlapi,gl,detail)
//--    load_gl_*_()                    ///{{{2///////////////////////////////
#define GETFN_(ty,name) reinterpret_cast<ty>(arg.getFn(name,arg.user))

void load_gl_core2_( GLapi& gl, LoaderArg_ const& arg )
{
	//-//-- 
	//- Core 2.0
	if( arg.major > 2 || (arg.major == 2 && arg.minor >= 0 ) )
	{
		bool ok = true;

		ok = !!(gl.attachShader = GETFN_(proto::AttachShaderFn, "glAttachShader")) && ok;
		ok = !!(gl.bindAttribLocation = GETFN_(proto::BindAttribLocationFn, "glBindAttribLocation")) && ok;
		ok = !!(gl.blendEquationSeparate = GETFN_(proto::BlendEquationSeparateFn, "glBlendEquationSeparate")) && ok;
		ok = !!(gl.compileShader = GETFN_(proto::CompileShaderFn, "glCompileShader")) && ok;
		ok = !!(gl.createProgram = GETFN_(proto::CreateProgramFn, "glCreateProgram")) && ok;
		ok = !!(gl.createShader = GETFN_(proto::CreateShaderFn, "glCreateShader")) && ok;
		ok = !!(gl.deleteProgram = GETFN_(proto::DeleteProgramFn, "glDeleteProgram")) && ok;
		ok = !!(gl.deleteShader = GETFN_(proto::DeleteShaderFn, "glDeleteShader")) && ok;
		ok = !!(gl.detachShader = GETFN_(proto::DetachShaderFn, "glDetachShader")) && ok;
		ok = !!(gl.disableVertexAttribArray = GETFN_(proto::DisableVertexAttribArrayFn, "glDisableVertexAttribArray")) && ok;
		ok = !!(gl.drawBuffers = GETFN_(proto::DrawBuffersFn, "glDrawBuffers")) && ok;
		ok = !!(gl.enableVertexAttribArray = GETFN_(proto::EnableVertexAttribArrayFn, "glEnableVertexAttribArray")) && ok;
		ok = !!(gl.getActiveAttrib = GETFN_(proto::GetActiveAttribFn, "glGetActiveAttrib")) && ok;
		ok = !!(gl.getActiveUniform = GETFN_(proto::GetActiveUniformFn, "glGetActiveUniform")) && ok;
		ok = !!(gl.getAttachedShaders = GETFN_(proto::GetAttachedShadersFn, "glGetAttachedShaders")) && ok;
		ok = !!(gl.getAttribLocation = GETFN_(proto::GetAttribLocationFn, "glGetAttribLocation")) && ok;
		ok = !!(gl.getProgramInfoLog = GETFN_(proto::GetProgramInfoLogFn, "glGetProgramInfoLog")) && ok;
		ok = !!(gl.getProgramiv = GETFN_(proto::GetProgramivFn, "glGetProgramiv")) && ok;
		ok = !!(gl.getShaderInfoLog = GETFN_(proto::GetShaderInfoLogFn, "glGetShaderInfoLog")) && ok;
		ok = !!(gl.getShaderSource = GETFN_(proto::GetShaderSourceFn, "glGetShaderSource")) && ok;
		ok = !!(gl.getShaderiv = GETFN_(proto::GetShaderivFn, "glGetShaderiv")) && ok;
		ok = !!(gl.getUniformLocation = GETFN_(proto::GetUniformLocationFn, "glGetUniformLocation")) && ok;
		ok = !!(gl.getUniformfv = GETFN_(proto::GetUniformfvFn, "glGetUniformfv")) && ok;
		ok = !!(gl.getUniformiv = GETFN_(proto::GetUniformivFn, "glGetUniformiv")) && ok;
		ok = !!(gl.getVertexAttribPointerv = GETFN_(proto::GetVertexAttribPointervFn, "glGetVertexAttribPointerv")) && ok;
		ok = !!(gl.getVertexAttribdv = GETFN_(proto::GetVertexAttribdvFn, "glGetVertexAttribdv")) && ok;
		ok = !!(gl.getVertexAttribfv = GETFN_(proto::GetVertexAttribfvFn, "glGetVertexAttribfv")) && ok;
		ok = !!(gl.getVertexAttribiv = GETFN_(proto::GetVertexAttribivFn, "glGetVertexAttribiv")) && ok;
		ok = !!(gl.isProgram = GETFN_(proto::IsProgramFn, "glIsProgram")) && ok;
		ok = !!(gl.isShader = GETFN_(proto::IsShaderFn, "glIsShader")) && ok;
		ok = !!(gl.linkProgram = GETFN_(proto::LinkProgramFn, "glLinkProgram")) && ok;
		ok = !!(gl.shaderSource = GETFN_(proto::ShaderSourceFn, "glShaderSource")) && ok;
		ok = !!(gl.stencilFuncSeparate = GETFN_(proto::StencilFuncSeparateFn, "glStencilFuncSeparate")) && ok;
		ok = !!(gl.stencilMaskSeparate = GETFN_(proto::StencilMaskSeparateFn, "glStencilMaskSeparate")) && ok;
		ok = !!(gl.stencilOpSeparate = GETFN_(proto::StencilOpSeparateFn, "glStencilOpSeparate")) && ok;
		ok = !!(gl.uniform1f = GETFN_(proto::Uniform1fFn, "glUniform1f")) && ok;
		ok = !!(gl.uniform1fv = GETFN_(proto::Uniform1fvFn, "glUniform1fv")) && ok;
		ok = !!(gl.uniform1i = GETFN_(proto::Uniform1iFn, "glUniform1i")) && ok;
		ok = !!(gl.uniform1iv = GETFN_(proto::Uniform1ivFn, "glUniform1iv")) && ok;
		ok = !!(gl.uniform2f = GETFN_(proto::Uniform2fFn, "glUniform2f")) && ok;
		ok = !!(gl.uniform2fv = GETFN_(proto::Uniform2fvFn, "glUniform2fv")) && ok;
		ok = !!(gl.uniform2i = GETFN_(proto::Uniform2iFn, "glUniform2i")) && ok;
		ok = !!(gl.uniform2iv = GETFN_(proto::Uniform2ivFn, "glUniform2iv")) && ok;
		ok = !!(gl.uniform3f = GETFN_(proto::Uniform3fFn, "glUniform3f")) && ok;
		ok = !!(gl.uniform3fv = GETFN_(proto::Uniform3fvFn, "glUniform3fv")) && ok;
		ok = !!(gl.uniform3i = GETFN_(proto::Uniform3iFn, "glUniform3i")) && ok;
		ok = !!(gl.uniform3iv = GETFN_(proto::Uniform3ivFn, "glUniform3iv")) && ok;
		ok = !!(gl.uniform4f = GETFN_(proto::Uniform4fFn, "glUniform4f")) && ok;
		ok = !!(gl.uniform4fv = GETFN_(proto::Uniform4fvFn, "glUniform4fv")) && ok;
		ok = !!(gl.uniform4i = GETFN_(proto::Uniform4iFn, "glUniform4i")) && ok;
		ok = !!(gl.uniform4iv = GETFN_(proto::Uniform4ivFn, "glUniform4iv")) && ok;
		ok = !!(gl.uniformMatrix2fv = GETFN_(proto::UniformMatrix2fvFn, "glUniformMatrix2fv")) && ok;
		ok = !!(gl.uniformMatrix3fv = GETFN_(proto::UniformMatrix3fvFn, "glUniformMatrix3fv")) && ok;
		ok = !!(gl.uniformMatrix4fv = GETFN_(proto::UniformMatrix4fvFn, "glUniformMatrix4fv")) && ok;
		ok = !!(gl.useProgram = GETFN_(proto::UseProgramFn, "glUseProgram")) && ok;
		ok = !!(gl.validateProgram = GETFN_(proto::ValidateProgramFn, "glValidateProgram")) && ok;
		ok = !!(gl.vertexAttrib1d = GETFN_(proto::VertexAttrib1dFn, "glVertexAttrib1d")) && ok;
		ok = !!(gl.vertexAttrib1dv = GETFN_(proto::VertexAttrib1dvFn, "glVertexAttrib1dv")) && ok;
		ok = !!(gl.vertexAttrib1f = GETFN_(proto::VertexAttrib1fFn, "glVertexAttrib1f")) && ok;
		ok = !!(gl.vertexAttrib1fv = GETFN_(proto::VertexAttrib1fvFn, "glVertexAttrib1fv")) && ok;
		ok = !!(gl.vertexAttrib1s = GETFN_(proto::VertexAttrib1sFn, "glVertexAttrib1s")) && ok;
		ok = !!(gl.vertexAttrib1sv = GETFN_(proto::VertexAttrib1svFn, "glVertexAttrib1sv")) && ok;
		ok = !!(gl.vertexAttrib2d = GETFN_(proto::VertexAttrib2dFn, "glVertexAttrib2d")) && ok;
		ok = !!(gl.vertexAttrib2dv = GETFN_(proto::VertexAttrib2dvFn, "glVertexAttrib2dv")) && ok;
		ok = !!(gl.vertexAttrib2f = GETFN_(proto::VertexAttrib2fFn, "glVertexAttrib2f")) && ok;
		ok = !!(gl.vertexAttrib2fv = GETFN_(proto::VertexAttrib2fvFn, "glVertexAttrib2fv")) && ok;
		ok = !!(gl.vertexAttrib2s = GETFN_(proto::VertexAttrib2sFn, "glVertexAttrib2s")) && ok;
		ok = !!(gl.vertexAttrib2sv = GETFN_(proto::VertexAttrib2svFn, "glVertexAttrib2sv")) && ok;
		ok = !!(gl.vertexAttrib3d = GETFN_(proto::VertexAttrib3dFn, "glVertexAttrib3d")) && ok;
		ok = !!(gl.vertexAttrib3dv = GETFN_(proto::VertexAttrib3dvFn, "glVertexAttrib3dv")) && ok;
		ok = !!(gl.vertexAttrib3f = GETFN_(proto::VertexAttrib3fFn, "glVertexAttrib3f")) && ok;
		ok = !!(gl.vertexAttrib3fv = GETFN_(proto::VertexAttrib3fvFn, "glVertexAttrib3fv")) && ok;
		ok = !!(gl.vertexAttrib3s = GETFN_(proto::VertexAttrib3sFn, "glVertexAttrib3s")) && ok;
		ok = !!(gl.vertexAttrib3sv = GETFN_(proto::VertexAttrib3svFn, "glVertexAttrib3sv")) && ok;
		ok = !!(gl.vertexAttrib4Nbv = GETFN_(proto::VertexAttrib4NbvFn, "glVertexAttrib4Nbv")) && ok;
		ok = !!(gl.vertexAttrib4Niv = GETFN_(proto::VertexAttrib4NivFn, "glVertexAttrib4Niv")) && ok;
		ok = !!(gl.vertexAttrib4Nsv = GETFN_(proto::VertexAttrib4NsvFn, "glVertexAttrib4Nsv")) && ok;
		ok = !!(gl.vertexAttrib4Nub = GETFN_(proto::VertexAttrib4NubFn, "glVertexAttrib4Nub")) && ok;
		ok = !!(gl.vertexAttrib4Nubv = GETFN_(proto::VertexAttrib4NubvFn, "glVertexAttrib4Nubv")) && ok;
		ok = !!(gl.vertexAttrib4Nuiv = GETFN_(proto::VertexAttrib4NuivFn, "glVertexAttrib4Nuiv")) && ok;
		ok = !!(gl.vertexAttrib4Nusv = GETFN_(proto::VertexAttrib4NusvFn, "glVertexAttrib4Nusv")) && ok;
		ok = !!(gl.vertexAttrib4bv = GETFN_(proto::VertexAttrib4bvFn, "glVertexAttrib4bv")) && ok;
		ok = !!(gl.vertexAttrib4d = GETFN_(proto::VertexAttrib4dFn, "glVertexAttrib4d")) && ok;
		ok = !!(gl.vertexAttrib4dv = GETFN_(proto::VertexAttrib4dvFn, "glVertexAttrib4dv")) && ok;
		ok = !!(gl.vertexAttrib4f = GETFN_(proto::VertexAttrib4fFn, "glVertexAttrib4f")) && ok;
		ok = !!(gl.vertexAttrib4fv = GETFN_(proto::VertexAttrib4fvFn, "glVertexAttrib4fv")) && ok;
		ok = !!(gl.vertexAttrib4iv = GETFN_(proto::VertexAttrib4ivFn, "glVertexAttrib4iv")) && ok;
		ok = !!(gl.vertexAttrib4s = GETFN_(proto::VertexAttrib4sFn, "glVertexAttrib4s")) && ok;
		ok = !!(gl.vertexAttrib4sv = GETFN_(proto::VertexAttrib4svFn, "glVertexAttrib4sv")) && ok;
		ok = !!(gl.vertexAttrib4ubv = GETFN_(proto::VertexAttrib4ubvFn, "glVertexAttrib4ubv")) && ok;
		ok = !!(gl.vertexAttrib4uiv = GETFN_(proto::VertexAttrib4uivFn, "glVertexAttrib4uiv")) && ok;
		ok = !!(gl.vertexAttrib4usv = GETFN_(proto::VertexAttrib4usvFn, "glVertexAttrib4usv")) && ok;
		ok = !!(gl.vertexAttribPointer = GETFN_(proto::VertexAttribPointerFn, "glVertexAttribPointer")) && ok;
		ok = ok && gl.VERSION_1_5;
		gl.VERSION_2_0 = ok;
		if( ok ) {
			gl.VERSION_loaded_major = 2;
			gl.VERSION_loaded_minor = 0;
		}
	}
	//- end: Core 2.0

	gl.VERSION_2_0_compatibility = gl.VERSION_1_5_compatibility;

	//-//-- 
	//- Core 2.1
	if( arg.major > 2 || (arg.major == 2 && arg.minor >= 1 ) )
	{
		bool ok = true;

		ok = !!(gl.uniformMatrix2x3fv = GETFN_(proto::UniformMatrix2x3fvFn, "glUniformMatrix2x3fv")) && ok;
		ok = !!(gl.uniformMatrix2x4fv = GETFN_(proto::UniformMatrix2x4fvFn, "glUniformMatrix2x4fv")) && ok;
		ok = !!(gl.uniformMatrix3x2fv = GETFN_(proto::UniformMatrix3x2fvFn, "glUniformMatrix3x2fv")) && ok;
		ok = !!(gl.uniformMatrix3x4fv = GETFN_(proto::UniformMatrix3x4fvFn, "glUniformMatrix3x4fv")) && ok;
		ok = !!(gl.uniformMatrix4x2fv = GETFN_(proto::UniformMatrix4x2fvFn, "glUniformMatrix4x2fv")) && ok;
		ok = !!(gl.uniformMatrix4x3fv = GETFN_(proto::UniformMatrix4x3fvFn, "glUniformMatrix4x3fv")) && ok;
		ok = ok && gl.VERSION_2_0;
		gl.VERSION_2_1 = ok;
		if( ok ) {
			gl.VERSION_loaded_major = 2;
			gl.VERSION_loaded_minor = 1;
		}
	}
	//- end: Core 2.1

	gl.VERSION_2_1_compatibility = gl.VERSION_2_0_compatibility;

}

#undef GETFN_
//--    <<< ~ dlapi::gl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE3(dlapi,gl,detail)
//--//////////////////////////////////////////////////////////////////////////
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
