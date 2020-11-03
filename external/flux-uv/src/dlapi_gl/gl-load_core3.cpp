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

void load_gl_core3_( GLapi& gl, LoaderArg_ const& arg )
{
	//-//-- 
	//- Core 3.0
	if( arg.major > 3 || (arg.major == 3 && arg.minor >= 0 ) )
	{
		bool ok = true;

		ok = !!(gl.beginConditionalRender = GETFN_(proto::BeginConditionalRenderFn, "glBeginConditionalRender")) && ok;
		ok = !!(gl.beginTransformFeedback = GETFN_(proto::BeginTransformFeedbackFn, "glBeginTransformFeedback")) && ok;
		ok = !!(gl.bindBufferBase = GETFN_(proto::BindBufferBaseFn, "glBindBufferBase")) && ok;
		ok = !!(gl.bindBufferRange = GETFN_(proto::BindBufferRangeFn, "glBindBufferRange")) && ok;
		ok = !!(gl.bindFragDataLocation = GETFN_(proto::BindFragDataLocationFn, "glBindFragDataLocation")) && ok;
		ok = !!(gl.bindFramebuffer = GETFN_(proto::BindFramebufferFn, "glBindFramebuffer")) && ok;
		ok = !!(gl.bindRenderbuffer = GETFN_(proto::BindRenderbufferFn, "glBindRenderbuffer")) && ok;
		ok = !!(gl.bindVertexArray = GETFN_(proto::BindVertexArrayFn, "glBindVertexArray")) && ok;
		ok = !!(gl.blitFramebuffer = GETFN_(proto::BlitFramebufferFn, "glBlitFramebuffer")) && ok;
		ok = !!(gl.checkFramebufferStatus = GETFN_(proto::CheckFramebufferStatusFn, "glCheckFramebufferStatus")) && ok;
		ok = !!(gl.clampColor = GETFN_(proto::ClampColorFn, "glClampColor")) && ok;
		ok = !!(gl.clearBufferfi = GETFN_(proto::ClearBufferfiFn, "glClearBufferfi")) && ok;
		ok = !!(gl.clearBufferfv = GETFN_(proto::ClearBufferfvFn, "glClearBufferfv")) && ok;
		ok = !!(gl.clearBufferiv = GETFN_(proto::ClearBufferivFn, "glClearBufferiv")) && ok;
		ok = !!(gl.clearBufferuiv = GETFN_(proto::ClearBufferuivFn, "glClearBufferuiv")) && ok;
		ok = !!(gl.colorMaski = GETFN_(proto::ColorMaskiFn, "glColorMaski")) && ok;
		ok = !!(gl.deleteFramebuffers = GETFN_(proto::DeleteFramebuffersFn, "glDeleteFramebuffers")) && ok;
		ok = !!(gl.deleteRenderbuffers = GETFN_(proto::DeleteRenderbuffersFn, "glDeleteRenderbuffers")) && ok;
		ok = !!(gl.deleteVertexArrays = GETFN_(proto::DeleteVertexArraysFn, "glDeleteVertexArrays")) && ok;
		ok = !!(gl.disablei = GETFN_(proto::DisableiFn, "glDisablei")) && ok;
		ok = !!(gl.enablei = GETFN_(proto::EnableiFn, "glEnablei")) && ok;
		ok = !!(gl.endConditionalRender = GETFN_(proto::EndConditionalRenderFn, "glEndConditionalRender")) && ok;
		ok = !!(gl.endTransformFeedback = GETFN_(proto::EndTransformFeedbackFn, "glEndTransformFeedback")) && ok;
		ok = !!(gl.flushMappedBufferRange = GETFN_(proto::FlushMappedBufferRangeFn, "glFlushMappedBufferRange")) && ok;
		ok = !!(gl.framebufferRenderbuffer = GETFN_(proto::FramebufferRenderbufferFn, "glFramebufferRenderbuffer")) && ok;
		ok = !!(gl.framebufferTexture1D = GETFN_(proto::FramebufferTexture1DFn, "glFramebufferTexture1D")) && ok;
		ok = !!(gl.framebufferTexture2D = GETFN_(proto::FramebufferTexture2DFn, "glFramebufferTexture2D")) && ok;
		ok = !!(gl.framebufferTexture3D = GETFN_(proto::FramebufferTexture3DFn, "glFramebufferTexture3D")) && ok;
		ok = !!(gl.framebufferTextureLayer = GETFN_(proto::FramebufferTextureLayerFn, "glFramebufferTextureLayer")) && ok;
		ok = !!(gl.genFramebuffers = GETFN_(proto::GenFramebuffersFn, "glGenFramebuffers")) && ok;
		ok = !!(gl.genRenderbuffers = GETFN_(proto::GenRenderbuffersFn, "glGenRenderbuffers")) && ok;
		ok = !!(gl.genVertexArrays = GETFN_(proto::GenVertexArraysFn, "glGenVertexArrays")) && ok;
		ok = !!(gl.generateMipmap = GETFN_(proto::GenerateMipmapFn, "glGenerateMipmap")) && ok;
		ok = !!(gl.getBooleani_v = GETFN_(proto::GetBooleani_vFn, "glGetBooleani_v")) && ok;
		ok = !!(gl.getFragDataLocation = GETFN_(proto::GetFragDataLocationFn, "glGetFragDataLocation")) && ok;
		ok = !!(gl.getFramebufferAttachmentParameteriv = GETFN_(proto::GetFramebufferAttachmentParameterivFn, "glGetFramebufferAttachmentParameteriv")) && ok;
		ok = !!(gl.getIntegeri_v = GETFN_(proto::GetIntegeri_vFn, "glGetIntegeri_v")) && ok;
		ok = !!(gl.getRenderbufferParameteriv = GETFN_(proto::GetRenderbufferParameterivFn, "glGetRenderbufferParameteriv")) && ok;
		ok = !!(gl.getStringi = GETFN_(proto::GetStringiFn, "glGetStringi")) && ok;
		ok = !!(gl.getTexParameterIiv = GETFN_(proto::GetTexParameterIivFn, "glGetTexParameterIiv")) && ok;
		ok = !!(gl.getTexParameterIuiv = GETFN_(proto::GetTexParameterIuivFn, "glGetTexParameterIuiv")) && ok;
		ok = !!(gl.getTransformFeedbackVarying = GETFN_(proto::GetTransformFeedbackVaryingFn, "glGetTransformFeedbackVarying")) && ok;
		ok = !!(gl.getUniformuiv = GETFN_(proto::GetUniformuivFn, "glGetUniformuiv")) && ok;
		ok = !!(gl.getVertexAttribIiv = GETFN_(proto::GetVertexAttribIivFn, "glGetVertexAttribIiv")) && ok;
		ok = !!(gl.getVertexAttribIuiv = GETFN_(proto::GetVertexAttribIuivFn, "glGetVertexAttribIuiv")) && ok;
		ok = !!(gl.isEnabledi = GETFN_(proto::IsEnablediFn, "glIsEnabledi")) && ok;
		ok = !!(gl.isFramebuffer = GETFN_(proto::IsFramebufferFn, "glIsFramebuffer")) && ok;
		ok = !!(gl.isRenderbuffer = GETFN_(proto::IsRenderbufferFn, "glIsRenderbuffer")) && ok;
		ok = !!(gl.isVertexArray = GETFN_(proto::IsVertexArrayFn, "glIsVertexArray")) && ok;
		ok = !!(gl.mapBufferRange = GETFN_(proto::MapBufferRangeFn, "glMapBufferRange")) && ok;
		ok = !!(gl.renderbufferStorage = GETFN_(proto::RenderbufferStorageFn, "glRenderbufferStorage")) && ok;
		ok = !!(gl.renderbufferStorageMultisample = GETFN_(proto::RenderbufferStorageMultisampleFn, "glRenderbufferStorageMultisample")) && ok;
		ok = !!(gl.texParameterIiv = GETFN_(proto::TexParameterIivFn, "glTexParameterIiv")) && ok;
		ok = !!(gl.texParameterIuiv = GETFN_(proto::TexParameterIuivFn, "glTexParameterIuiv")) && ok;
		ok = !!(gl.transformFeedbackVaryings = GETFN_(proto::TransformFeedbackVaryingsFn, "glTransformFeedbackVaryings")) && ok;
		ok = !!(gl.uniform1ui = GETFN_(proto::Uniform1uiFn, "glUniform1ui")) && ok;
		ok = !!(gl.uniform1uiv = GETFN_(proto::Uniform1uivFn, "glUniform1uiv")) && ok;
		ok = !!(gl.uniform2ui = GETFN_(proto::Uniform2uiFn, "glUniform2ui")) && ok;
		ok = !!(gl.uniform2uiv = GETFN_(proto::Uniform2uivFn, "glUniform2uiv")) && ok;
		ok = !!(gl.uniform3ui = GETFN_(proto::Uniform3uiFn, "glUniform3ui")) && ok;
		ok = !!(gl.uniform3uiv = GETFN_(proto::Uniform3uivFn, "glUniform3uiv")) && ok;
		ok = !!(gl.uniform4ui = GETFN_(proto::Uniform4uiFn, "glUniform4ui")) && ok;
		ok = !!(gl.uniform4uiv = GETFN_(proto::Uniform4uivFn, "glUniform4uiv")) && ok;
		ok = !!(gl.vertexAttribI1i = GETFN_(proto::VertexAttribI1iFn, "glVertexAttribI1i")) && ok;
		ok = !!(gl.vertexAttribI1iv = GETFN_(proto::VertexAttribI1ivFn, "glVertexAttribI1iv")) && ok;
		ok = !!(gl.vertexAttribI1ui = GETFN_(proto::VertexAttribI1uiFn, "glVertexAttribI1ui")) && ok;
		ok = !!(gl.vertexAttribI1uiv = GETFN_(proto::VertexAttribI1uivFn, "glVertexAttribI1uiv")) && ok;
		ok = !!(gl.vertexAttribI2i = GETFN_(proto::VertexAttribI2iFn, "glVertexAttribI2i")) && ok;
		ok = !!(gl.vertexAttribI2iv = GETFN_(proto::VertexAttribI2ivFn, "glVertexAttribI2iv")) && ok;
		ok = !!(gl.vertexAttribI2ui = GETFN_(proto::VertexAttribI2uiFn, "glVertexAttribI2ui")) && ok;
		ok = !!(gl.vertexAttribI2uiv = GETFN_(proto::VertexAttribI2uivFn, "glVertexAttribI2uiv")) && ok;
		ok = !!(gl.vertexAttribI3i = GETFN_(proto::VertexAttribI3iFn, "glVertexAttribI3i")) && ok;
		ok = !!(gl.vertexAttribI3iv = GETFN_(proto::VertexAttribI3ivFn, "glVertexAttribI3iv")) && ok;
		ok = !!(gl.vertexAttribI3ui = GETFN_(proto::VertexAttribI3uiFn, "glVertexAttribI3ui")) && ok;
		ok = !!(gl.vertexAttribI3uiv = GETFN_(proto::VertexAttribI3uivFn, "glVertexAttribI3uiv")) && ok;
		ok = !!(gl.vertexAttribI4bv = GETFN_(proto::VertexAttribI4bvFn, "glVertexAttribI4bv")) && ok;
		ok = !!(gl.vertexAttribI4i = GETFN_(proto::VertexAttribI4iFn, "glVertexAttribI4i")) && ok;
		ok = !!(gl.vertexAttribI4iv = GETFN_(proto::VertexAttribI4ivFn, "glVertexAttribI4iv")) && ok;
		ok = !!(gl.vertexAttribI4sv = GETFN_(proto::VertexAttribI4svFn, "glVertexAttribI4sv")) && ok;
		ok = !!(gl.vertexAttribI4ubv = GETFN_(proto::VertexAttribI4ubvFn, "glVertexAttribI4ubv")) && ok;
		ok = !!(gl.vertexAttribI4ui = GETFN_(proto::VertexAttribI4uiFn, "glVertexAttribI4ui")) && ok;
		ok = !!(gl.vertexAttribI4uiv = GETFN_(proto::VertexAttribI4uivFn, "glVertexAttribI4uiv")) && ok;
		ok = !!(gl.vertexAttribI4usv = GETFN_(proto::VertexAttribI4usvFn, "glVertexAttribI4usv")) && ok;
		ok = !!(gl.vertexAttribIPointer = GETFN_(proto::VertexAttribIPointerFn, "glVertexAttribIPointer")) && ok;
		ok = ok && gl.VERSION_2_1;
		gl.VERSION_3_0 = ok;
		if( ok ) {
			gl.VERSION_loaded_major = 3;
			gl.VERSION_loaded_minor = 0;
		}
	}
	//- end: Core 3.0

	gl.VERSION_3_0_compatibility = gl.VERSION_2_1_compatibility;

	//-//-- 
	//- Core 3.1
	if( arg.major > 3 || (arg.major == 3 && arg.minor >= 1 ) )
	{
		bool ok = true;

		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.bindBufferBase = GETFN_(proto::BindBufferBaseFn, "glBindBufferBase")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.bindBufferRange = GETFN_(proto::BindBufferRangeFn, "glBindBufferRange")) && ok;
		}
		ok = !!(gl.copyBufferSubData = GETFN_(proto::CopyBufferSubDataFn, "glCopyBufferSubData")) && ok;
		ok = !!(gl.drawArraysInstanced = GETFN_(proto::DrawArraysInstancedFn, "glDrawArraysInstanced")) && ok;
		ok = !!(gl.drawElementsInstanced = GETFN_(proto::DrawElementsInstancedFn, "glDrawElementsInstanced")) && ok;
		ok = !!(gl.getActiveUniformBlockName = GETFN_(proto::GetActiveUniformBlockNameFn, "glGetActiveUniformBlockName")) && ok;
		ok = !!(gl.getActiveUniformBlockiv = GETFN_(proto::GetActiveUniformBlockivFn, "glGetActiveUniformBlockiv")) && ok;
		ok = !!(gl.getActiveUniformName = GETFN_(proto::GetActiveUniformNameFn, "glGetActiveUniformName")) && ok;
		ok = !!(gl.getActiveUniformsiv = GETFN_(proto::GetActiveUniformsivFn, "glGetActiveUniformsiv")) && ok;
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.getIntegeri_v = GETFN_(proto::GetIntegeri_vFn, "glGetIntegeri_v")) && ok;
		}
		ok = !!(gl.getUniformBlockIndex = GETFN_(proto::GetUniformBlockIndexFn, "glGetUniformBlockIndex")) && ok;
		ok = !!(gl.getUniformIndices = GETFN_(proto::GetUniformIndicesFn, "glGetUniformIndices")) && ok;
		ok = !!(gl.primitiveRestartIndex = GETFN_(proto::PrimitiveRestartIndexFn, "glPrimitiveRestartIndex")) && ok;
		ok = !!(gl.texBuffer = GETFN_(proto::TexBufferFn, "glTexBuffer")) && ok;
		ok = !!(gl.uniformBlockBinding = GETFN_(proto::UniformBlockBindingFn, "glUniformBlockBinding")) && ok;
		ok = ok && gl.VERSION_3_0;
		gl.VERSION_3_1 = ok;
		if( ok ) {
			gl.VERSION_loaded_major = 3;
			gl.VERSION_loaded_minor = 1;
		}
	}
	//- end: Core 3.1

	gl.VERSION_3_1_compatibility = gl.VERSION_3_0_compatibility;

	//-//-- 
	//- Core 3.2
	if( arg.major > 3 || (arg.major == 3 && arg.minor >= 2 ) )
	{
		bool ok = true;

		ok = !!(gl.clientWaitSync = GETFN_(proto::ClientWaitSyncFn, "glClientWaitSync")) && ok;
		ok = !!(gl.deleteSync = GETFN_(proto::DeleteSyncFn, "glDeleteSync")) && ok;
		ok = !!(gl.drawElementsBaseVertex = GETFN_(proto::DrawElementsBaseVertexFn, "glDrawElementsBaseVertex")) && ok;
		ok = !!(gl.drawElementsInstancedBaseVertex = GETFN_(proto::DrawElementsInstancedBaseVertexFn, "glDrawElementsInstancedBaseVertex")) && ok;
		ok = !!(gl.drawRangeElementsBaseVertex = GETFN_(proto::DrawRangeElementsBaseVertexFn, "glDrawRangeElementsBaseVertex")) && ok;
		ok = !!(gl.fenceSync = GETFN_(proto::FenceSyncFn, "glFenceSync")) && ok;
		ok = !!(gl.framebufferTexture = GETFN_(proto::FramebufferTextureFn, "glFramebufferTexture")) && ok;
		ok = !!(gl.getBufferParameteri64v = GETFN_(proto::GetBufferParameteri64vFn, "glGetBufferParameteri64v")) && ok;
		ok = !!(gl.getInteger64i_v = GETFN_(proto::GetInteger64i_vFn, "glGetInteger64i_v")) && ok;
		ok = !!(gl.getInteger64v = GETFN_(proto::GetInteger64vFn, "glGetInteger64v")) && ok;
		ok = !!(gl.getMultisamplefv = GETFN_(proto::GetMultisamplefvFn, "glGetMultisamplefv")) && ok;
		ok = !!(gl.getSynciv = GETFN_(proto::GetSyncivFn, "glGetSynciv")) && ok;
		ok = !!(gl.isSync = GETFN_(proto::IsSyncFn, "glIsSync")) && ok;
		ok = !!(gl.multiDrawElementsBaseVertex = GETFN_(proto::MultiDrawElementsBaseVertexFn, "glMultiDrawElementsBaseVertex")) && ok;
		ok = !!(gl.provokingVertex = GETFN_(proto::ProvokingVertexFn, "glProvokingVertex")) && ok;
		ok = !!(gl.sampleMaski = GETFN_(proto::SampleMaskiFn, "glSampleMaski")) && ok;
		ok = !!(gl.texImage2DMultisample = GETFN_(proto::TexImage2DMultisampleFn, "glTexImage2DMultisample")) && ok;
		ok = !!(gl.texImage3DMultisample = GETFN_(proto::TexImage3DMultisampleFn, "glTexImage3DMultisample")) && ok;
		ok = !!(gl.waitSync = GETFN_(proto::WaitSyncFn, "glWaitSync")) && ok;
		{
			ok = ok && gl.vertexAttribIPointer;
			ok = ok && gl.vertexAttrib1s;
			ok = ok && gl.depthRange;
			ok = ok && gl.vertexAttribI3uiv;
			ok = ok && gl.disable;
			ok = ok && gl.clearStencil;
			ok = ok && gl.texSubImage1D;
			ok = ok && gl.framebufferTexture1D;
			ok = ok && gl.getAttribLocation;
			ok = ok && gl.pointParameterfv;
			ok = ok && gl.vertexAttrib2dv;
			ok = ok && gl.polygonOffset;
			ok = ok && gl.pixelStorei;
			ok = ok && gl.pointParameteriv;
			ok = ok && gl.vertexAttribI1ui;
			ok = ok && gl.blendEquation;
			ok = ok && gl.genFramebuffers;
			ok = ok && gl.drawArraysInstanced;
			ok = ok && gl.stencilFunc;
			ok = ok && gl.depthMask;
			ok = ok && gl.deleteRenderbuffers;
			ok = ok && gl.primitiveRestartIndex;
			ok = ok && gl.getFragDataLocation;
			ok = ok && gl.genTextures;
			ok = ok && gl.stencilOp;
			ok = ok && gl.getUniformfv;
			ok = ok && gl.texSubImage3D;
			ok = ok && gl.vertexAttribI4i;
			ok = ok && gl.endQuery;
			ok = ok && gl.vertexAttrib2sv;
			ok = ok && gl.getBooleanv;
			ok = ok && gl.vertexAttrib3sv;
			ok = ok && gl.drawBuffers;
			ok = ok && gl.uniform3f;
			ok = ok && gl.getQueryObjectuiv;
			ok = ok && gl.copyTexImage2D;
			ok = ok && gl.uniformBlockBinding;
			ok = ok && gl.vertexAttrib2fv;
			ok = ok && gl.copyBufferSubData;
			ok = ok && gl.vertexAttribI2i;
			ok = ok && gl.uniform4uiv;
			ok = ok && gl.uniform4ui;
			ok = ok && gl.hint;
			ok = ok && gl.blendFuncSeparate;
			ok = ok && gl.viewport;
			ok = ok && gl.getQueryiv;
			ok = ok && gl.getRenderbufferParameteriv;
			ok = ok && gl.shaderSource;
			ok = ok && gl.stencilMask;
			ok = ok && gl.vertexAttrib4Niv;
			ok = ok && gl.framebufferTextureLayer;
			ok = ok && gl.getBufferPointerv;
			ok = ok && gl.getTexImage;
			ok = ok && gl.stencilMaskSeparate;
			ok = ok && gl.getTexParameterIiv;
			ok = ok && gl.isTexture;
			ok = ok && gl.texParameteriv;
			ok = ok && gl.vertexAttribI1i;
			ok = ok && gl.bindFragDataLocation;
			ok = ok && gl.getIntegerv;
			ok = ok && gl.vertexAttribI4usv;
			ok = ok && gl.readPixels;
			ok = ok && gl.compressedTexSubImage1D;
			ok = ok && gl.attachShader;
			ok = ok && gl.getDoublev;
			ok = ok && gl.stencilFuncSeparate;
			ok = ok && gl.vertexAttrib4ubv;
			ok = ok && gl.uniformMatrix3x2fv;
			ok = ok && gl.uniform4fv;
			ok = ok && gl.enableVertexAttribArray;
			ok = ok && gl.vertexAttrib4s;
			ok = ok && gl.vertexAttrib4iv;
			ok = ok && gl.getUniformBlockIndex;
			ok = ok && gl.pointParameterf;
			ok = ok && gl.getVertexAttribdv;
			ok = ok && gl.getString;
			ok = ok && gl.blendEquationSeparate;
			ok = ok && gl.getVertexAttribIuiv;
			ok = ok && gl.vertexAttribI3i;
			ok = ok && gl.uniformMatrix4x2fv;
			ok = ok && gl.vertexAttribI2uiv;
			ok = ok && gl.isFramebuffer;
			ok = ok && gl.copyTexImage1D;
			ok = ok && gl.getShaderInfoLog;
			ok = ok && gl.createProgram;
			ok = ok && gl.multiDrawElements;
			ok = ok && gl.getBufferSubData;
			ok = ok && gl.getError;
			ok = ok && gl.logicOp;
			ok = ok && gl.vertexAttrib4Nub;
			ok = ok && gl.useProgram;
			ok = ok && gl.vertexAttrib4dv;
			ok = ok && gl.colorMask;
			ok = ok && gl.isShader;
			ok = ok && gl.vertexAttrib1fv;
			ok = ok && gl.beginQuery;
			ok = ok && gl.vertexAttribI2iv;
			ok = ok && gl.getFramebufferAttachmentParameteriv;
			ok = ok && gl.texBuffer;
			ok = ok && gl.frontFace;
			ok = ok && gl.vertexAttribI4iv;
			ok = ok && gl.isRenderbuffer;
			ok = ok && gl.deleteShader;
			ok = ok && gl.texSubImage2D;
			ok = ok && gl.pointParameteri;
			ok = ok && gl.copyTexSubImage3D;
			ok = ok && gl.getActiveUniformBlockName;
			ok = ok && gl.lineWidth;
			ok = ok && gl.drawBuffer;
			ok = ok && gl.getShaderSource;
			ok = ok && gl.getProgramiv;
			ok = ok && gl.deleteFramebuffers;
			ok = ok && gl.uniform2i;
			ok = ok && gl.getUniformiv;
			ok = ok && gl.getProgramInfoLog;
			ok = ok && gl.genRenderbuffers;
			ok = ok && gl.getVertexAttribfv;
			ok = ok && gl.renderbufferStorage;
			ok = ok && gl.uniform1f;
			ok = ok && gl.uniform2iv;
			ok = ok && gl.blendFunc;
			ok = ok && gl.uniformMatrix2x3fv;
			ok = ok && gl.stencilOpSeparate;
			ok = ok && gl.getVertexAttribiv;
			ok = ok && gl.getUniformIndices;
			ok = ok && gl.uniformMatrix2x4fv;
			ok = ok && gl.vertexAttrib3f;
			ok = ok && gl.vertexAttrib4f;
			ok = ok && gl.getIntegeri_v;
			ok = ok && gl.disableVertexAttribArray;
			ok = ok && gl.beginTransformFeedback;
			ok = ok && gl.vertexAttrib4Nusv;
			ok = ok && gl.blendColor;
			ok = ok && gl.getBufferParameteriv;
			ok = ok && gl.clearColor;
			ok = ok && gl.genVertexArrays;
			ok = ok && gl.getTexParameterfv;
			ok = ok && gl.getQueryObjectiv;
			ok = ok && gl.compressedTexImage3D;
			ok = ok && gl.uniform2ui;
			ok = ok && gl.vertexAttribI4ubv;
			ok = ok && gl.drawElementsInstanced;
			ok = ok && gl.drawElements;
			ok = ok && gl.copyTexSubImage1D;
			ok = ok && gl.getCompressedTexImage;
			ok = ok && gl.compressedTexImage1D;
			ok = ok && gl.pointSize;
			ok = ok && gl.getTexParameterIuiv;
			ok = ok && gl.deleteVertexArrays;
			ok = ok && gl.getShaderiv;
			ok = ok && gl.uniform1fv;
			ok = ok && gl.drawArrays;
			ok = ok && gl.uniform1iv;
			ok = ok && gl.bufferSubData;
			ok = ok && gl.deleteQueries;
			ok = ok && gl.generateMipmap;
			ok = ok && gl.enable;
			ok = ok && gl.clearBufferiv;
			ok = ok && gl.activeTexture;
			ok = ok && gl.bufferData;
			ok = ok && gl.vertexAttribI3iv;
			ok = ok && gl.getActiveUniformBlockiv;
			ok = ok && gl.uniform2uiv;
			ok = ok && gl.clearBufferfv;
			ok = ok && gl.transformFeedbackVaryings;
			ok = ok && gl.mapBuffer;
			ok = ok && gl.uniform3iv;
			ok = ok && gl.getUniformLocation;
			ok = ok && gl.unmapBuffer;
			ok = ok && gl.bindBufferBase;
			ok = ok && gl.createShader;
			ok = ok && gl.uniform4i;
			ok = ok && gl.validateProgram;
			ok = ok && gl.framebufferRenderbuffer;
			ok = ok && gl.getFloatv;
			ok = ok && gl.endConditionalRender;
			ok = ok && gl.getBooleani_v;
			ok = ok && gl.genBuffers;
			ok = ok && gl.vertexAttribPointer;
			ok = ok && gl.uniformMatrix2fv;
			ok = ok && gl.drawRangeElements;
			ok = ok && gl.getVertexAttribIiv;
			ok = ok && gl.vertexAttribI4uiv;
			ok = ok && gl.vertexAttrib1dv;
			ok = ok && gl.framebufferTexture3D;
			ok = ok && gl.scissor;
			ok = ok && gl.deleteProgram;
			ok = ok && gl.clearBufferfi;
			ok = ok && gl.uniform4iv;
			ok = ok && gl.vertexAttrib2s;
			ok = ok && gl.vertexAttribI3ui;
			ok = ok && gl.texImage1D;
			ok = ok && gl.texImage3D;
			ok = ok && gl.cullFace;
			ok = ok && gl.deleteTextures;
			ok = ok && gl.compressedTexSubImage2D;
			ok = ok && gl.getStringi;
			ok = ok && gl.isBuffer;
			ok = ok && gl.vertexAttrib4uiv;
			ok = ok && gl.vertexAttrib4Nubv;
			ok = ok && gl.checkFramebufferStatus;
			ok = ok && gl.vertexAttrib4usv;
			ok = ok && gl.detachShader;
			ok = ok && gl.vertexAttrib1d;
			ok = ok && gl.renderbufferStorageMultisample;
			ok = ok && gl.texParameterIuiv;
			ok = ok && gl.vertexAttrib3d;
			ok = ok && gl.vertexAttrib3dv;
			ok = ok && gl.vertexAttrib4d;
			ok = ok && gl.vertexAttrib4Nbv;
			ok = ok && gl.texImage2D;
			ok = ok && gl.clampColor;
			ok = ok && gl.bindBuffer;
			ok = ok && gl.uniform3fv;
			ok = ok && gl.endTransformFeedback;
			ok = ok && gl.getActiveUniformName;
			ok = ok && gl.compressedTexImage2D;
			ok = ok && gl.polygonMode;
			ok = ok && gl.vertexAttrib4sv;
			ok = ok && gl.framebufferTexture2D;
			ok = ok && gl.compressedTexSubImage3D;
			ok = ok && gl.getActiveAttrib;
			ok = ok && gl.texParameterf;
			ok = ok && gl.readBuffer;
			ok = ok && gl.getTexParameteriv;
			ok = ok && gl.bindBufferRange;
			ok = ok && gl.vertexAttrib3s;
			ok = ok && gl.uniform2fv;
			ok = ok && gl.vertexAttrib1f;
			ok = ok && gl.enablei;
			ok = ok && gl.compileShader;
			ok = ok && gl.uniform2f;
			ok = ok && gl.uniform3uiv;
			ok = ok && gl.texParameterfv;
			ok = ok && gl.uniform1i;
			ok = ok && gl.clear;
			ok = ok && gl.vertexAttribI2ui;
			ok = ok && gl.uniformMatrix4x3fv;
			ok = ok && gl.linkProgram;
			ok = ok && gl.finish;
			ok = ok && gl.getUniformuiv;
			ok = ok && gl.vertexAttribI4ui;
			ok = ok && gl.uniformMatrix3fv;
			ok = ok && gl.bindFramebuffer;
			ok = ok && gl.vertexAttrib4Nuiv;
			ok = ok && gl.isEnabled;
			ok = ok && gl.vertexAttrib1sv;
			ok = ok && gl.getTexLevelParameterfv;
			ok = ok && gl.vertexAttribI4bv;
			ok = ok && gl.genQueries;
			ok = ok && gl.isEnabledi;
			ok = ok && gl.mapBufferRange;
			ok = ok && gl.sampleCoverage;
			ok = ok && gl.deleteBuffers;
			ok = ok && gl.clearDepth;
			ok = ok && gl.texParameterIiv;
			ok = ok && gl.isQuery;
			ok = ok && gl.disablei;
			ok = ok && gl.uniformMatrix4fv;
			ok = ok && gl.vertexAttrib4Nsv;
			ok = ok && gl.getTransformFeedbackVarying;
			ok = ok && gl.getVertexAttribPointerv;
			ok = ok && gl.multiDrawArrays;
			ok = ok && gl.blitFramebuffer;
			ok = ok && gl.vertexAttribI1iv;
			ok = ok && gl.flushMappedBufferRange;
			ok = ok && gl.vertexAttrib2f;
			ok = ok && gl.bindRenderbuffer;
			ok = ok && gl.pixelStoref;
			ok = ok && gl.getTexLevelParameteriv;
			ok = ok && gl.getActiveUniform;
			ok = ok && gl.vertexAttribI4sv;
			ok = ok && gl.isVertexArray;
			ok = ok && gl.uniform4f;
			ok = ok && gl.uniform1uiv;
			ok = ok && gl.vertexAttribI1uiv;
			ok = ok && gl.texParameteri;
			ok = ok && gl.vertexAttrib4bv;
			ok = ok && gl.uniform1ui;
			ok = ok && gl.depthFunc;
			ok = ok && gl.getAttachedShaders;
			ok = ok && gl.flush;
			ok = ok && gl.bindTexture;
			ok = ok && gl.bindAttribLocation;
			ok = ok && gl.copyTexSubImage2D;
			ok = ok && gl.uniform3ui;
			ok = ok && gl.uniform3i;
			ok = ok && gl.bindVertexArray;
			ok = ok && gl.getActiveUniformsiv;
			ok = ok && gl.vertexAttrib3fv;
			ok = ok && gl.clearBufferuiv;
			ok = ok && gl.beginConditionalRender;
			ok = ok && gl.uniformMatrix3x4fv;
			ok = ok && gl.colorMaski;
			ok = ok && gl.vertexAttrib2d;
			ok = ok && gl.isProgram;
			ok = ok && gl.vertexAttrib4fv;
		}
		gl.VERSION_3_2 = ok;
		if( ok ) {
			gl.VERSION_loaded_major = 3;
			gl.VERSION_loaded_minor = 2;
		}
	}
	//- end: Core 3.2

	//-//-- 
	//- Compatibility 3.2
	if( arg.major > 3 || (arg.major == 3 && arg.minor >= 2 ) )
	{
		bool ok = true;

		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.accum = GETFN_(proto::AccumFn, "glAccum")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.alphaFunc = GETFN_(proto::AlphaFuncFn, "glAlphaFunc")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.areTexturesResident = GETFN_(proto::AreTexturesResidentFn, "glAreTexturesResident")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.arrayElement = GETFN_(proto::ArrayElementFn, "glArrayElement")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.begin = GETFN_(proto::BeginFn, "glBegin")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.bitmap = GETFN_(proto::BitmapFn, "glBitmap")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.callList = GETFN_(proto::CallListFn, "glCallList")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.callLists = GETFN_(proto::CallListsFn, "glCallLists")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.clearAccum = GETFN_(proto::ClearAccumFn, "glClearAccum")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.clearIndex = GETFN_(proto::ClearIndexFn, "glClearIndex")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.clientActiveTexture = GETFN_(proto::ClientActiveTextureFn, "glClientActiveTexture")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.clipPlane = GETFN_(proto::ClipPlaneFn, "glClipPlane")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3b = GETFN_(proto::Color3bFn, "glColor3b")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3bv = GETFN_(proto::Color3bvFn, "glColor3bv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3d = GETFN_(proto::Color3dFn, "glColor3d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3dv = GETFN_(proto::Color3dvFn, "glColor3dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3f = GETFN_(proto::Color3fFn, "glColor3f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3fv = GETFN_(proto::Color3fvFn, "glColor3fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3i = GETFN_(proto::Color3iFn, "glColor3i")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3iv = GETFN_(proto::Color3ivFn, "glColor3iv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3s = GETFN_(proto::Color3sFn, "glColor3s")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3sv = GETFN_(proto::Color3svFn, "glColor3sv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3ub = GETFN_(proto::Color3ubFn, "glColor3ub")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3ubv = GETFN_(proto::Color3ubvFn, "glColor3ubv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3ui = GETFN_(proto::Color3uiFn, "glColor3ui")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3uiv = GETFN_(proto::Color3uivFn, "glColor3uiv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3us = GETFN_(proto::Color3usFn, "glColor3us")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color3usv = GETFN_(proto::Color3usvFn, "glColor3usv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4b = GETFN_(proto::Color4bFn, "glColor4b")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4bv = GETFN_(proto::Color4bvFn, "glColor4bv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4d = GETFN_(proto::Color4dFn, "glColor4d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4dv = GETFN_(proto::Color4dvFn, "glColor4dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4f = GETFN_(proto::Color4fFn, "glColor4f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4fv = GETFN_(proto::Color4fvFn, "glColor4fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4i = GETFN_(proto::Color4iFn, "glColor4i")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4iv = GETFN_(proto::Color4ivFn, "glColor4iv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4s = GETFN_(proto::Color4sFn, "glColor4s")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4sv = GETFN_(proto::Color4svFn, "glColor4sv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4ub = GETFN_(proto::Color4ubFn, "glColor4ub")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4ubv = GETFN_(proto::Color4ubvFn, "glColor4ubv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4ui = GETFN_(proto::Color4uiFn, "glColor4ui")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4uiv = GETFN_(proto::Color4uivFn, "glColor4uiv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4us = GETFN_(proto::Color4usFn, "glColor4us")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.color4usv = GETFN_(proto::Color4usvFn, "glColor4usv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.colorMaterial = GETFN_(proto::ColorMaterialFn, "glColorMaterial")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.colorPointer = GETFN_(proto::ColorPointerFn, "glColorPointer")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.copyPixels = GETFN_(proto::CopyPixelsFn, "glCopyPixels")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.deleteLists = GETFN_(proto::DeleteListsFn, "glDeleteLists")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.disableClientState = GETFN_(proto::DisableClientStateFn, "glDisableClientState")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.drawPixels = GETFN_(proto::DrawPixelsFn, "glDrawPixels")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.edgeFlag = GETFN_(proto::EdgeFlagFn, "glEdgeFlag")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.edgeFlagPointer = GETFN_(proto::EdgeFlagPointerFn, "glEdgeFlagPointer")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.edgeFlagv = GETFN_(proto::EdgeFlagvFn, "glEdgeFlagv")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.enableClientState = GETFN_(proto::EnableClientStateFn, "glEnableClientState")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.end = GETFN_(proto::EndFn, "glEnd")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.endList = GETFN_(proto::EndListFn, "glEndList")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.evalCoord1d = GETFN_(proto::EvalCoord1dFn, "glEvalCoord1d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.evalCoord1dv = GETFN_(proto::EvalCoord1dvFn, "glEvalCoord1dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.evalCoord1f = GETFN_(proto::EvalCoord1fFn, "glEvalCoord1f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.evalCoord1fv = GETFN_(proto::EvalCoord1fvFn, "glEvalCoord1fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.evalCoord2d = GETFN_(proto::EvalCoord2dFn, "glEvalCoord2d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.evalCoord2dv = GETFN_(proto::EvalCoord2dvFn, "glEvalCoord2dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.evalCoord2f = GETFN_(proto::EvalCoord2fFn, "glEvalCoord2f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.evalCoord2fv = GETFN_(proto::EvalCoord2fvFn, "glEvalCoord2fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.evalMesh1 = GETFN_(proto::EvalMesh1Fn, "glEvalMesh1")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.evalMesh2 = GETFN_(proto::EvalMesh2Fn, "glEvalMesh2")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.evalPoint1 = GETFN_(proto::EvalPoint1Fn, "glEvalPoint1")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.evalPoint2 = GETFN_(proto::EvalPoint2Fn, "glEvalPoint2")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.feedbackBuffer = GETFN_(proto::FeedbackBufferFn, "glFeedbackBuffer")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.fogCoordPointer = GETFN_(proto::FogCoordPointerFn, "glFogCoordPointer")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.fogCoordd = GETFN_(proto::FogCoorddFn, "glFogCoordd")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.fogCoorddv = GETFN_(proto::FogCoorddvFn, "glFogCoorddv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.fogCoordf = GETFN_(proto::FogCoordfFn, "glFogCoordf")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.fogCoordfv = GETFN_(proto::FogCoordfvFn, "glFogCoordfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.fogf = GETFN_(proto::FogfFn, "glFogf")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.fogfv = GETFN_(proto::FogfvFn, "glFogfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.fogi = GETFN_(proto::FogiFn, "glFogi")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.fogiv = GETFN_(proto::FogivFn, "glFogiv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.frustum = GETFN_(proto::FrustumFn, "glFrustum")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.genLists = GETFN_(proto::GenListsFn, "glGenLists")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getClipPlane = GETFN_(proto::GetClipPlaneFn, "glGetClipPlane")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getLightfv = GETFN_(proto::GetLightfvFn, "glGetLightfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getLightiv = GETFN_(proto::GetLightivFn, "glGetLightiv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getMapdv = GETFN_(proto::GetMapdvFn, "glGetMapdv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getMapfv = GETFN_(proto::GetMapfvFn, "glGetMapfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getMapiv = GETFN_(proto::GetMapivFn, "glGetMapiv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getMaterialfv = GETFN_(proto::GetMaterialfvFn, "glGetMaterialfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getMaterialiv = GETFN_(proto::GetMaterialivFn, "glGetMaterialiv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getPixelMapfv = GETFN_(proto::GetPixelMapfvFn, "glGetPixelMapfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getPixelMapuiv = GETFN_(proto::GetPixelMapuivFn, "glGetPixelMapuiv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getPixelMapusv = GETFN_(proto::GetPixelMapusvFn, "glGetPixelMapusv")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.getPointerv = GETFN_(proto::GetPointervFn, "glGetPointerv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getPolygonStipple = GETFN_(proto::GetPolygonStippleFn, "glGetPolygonStipple")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getTexEnvfv = GETFN_(proto::GetTexEnvfvFn, "glGetTexEnvfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getTexEnviv = GETFN_(proto::GetTexEnvivFn, "glGetTexEnviv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getTexGendv = GETFN_(proto::GetTexGendvFn, "glGetTexGendv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getTexGenfv = GETFN_(proto::GetTexGenfvFn, "glGetTexGenfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.getTexGeniv = GETFN_(proto::GetTexGenivFn, "glGetTexGeniv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.indexMask = GETFN_(proto::IndexMaskFn, "glIndexMask")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.indexPointer = GETFN_(proto::IndexPointerFn, "glIndexPointer")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.indexd = GETFN_(proto::IndexdFn, "glIndexd")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.indexdv = GETFN_(proto::IndexdvFn, "glIndexdv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.indexf = GETFN_(proto::IndexfFn, "glIndexf")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.indexfv = GETFN_(proto::IndexfvFn, "glIndexfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.indexi = GETFN_(proto::IndexiFn, "glIndexi")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.indexiv = GETFN_(proto::IndexivFn, "glIndexiv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.indexs = GETFN_(proto::IndexsFn, "glIndexs")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.indexsv = GETFN_(proto::IndexsvFn, "glIndexsv")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.indexub = GETFN_(proto::IndexubFn, "glIndexub")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.indexubv = GETFN_(proto::IndexubvFn, "glIndexubv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.initNames = GETFN_(proto::InitNamesFn, "glInitNames")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.interleavedArrays = GETFN_(proto::InterleavedArraysFn, "glInterleavedArrays")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.isList = GETFN_(proto::IsListFn, "glIsList")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.lightModelf = GETFN_(proto::LightModelfFn, "glLightModelf")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.lightModelfv = GETFN_(proto::LightModelfvFn, "glLightModelfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.lightModeli = GETFN_(proto::LightModeliFn, "glLightModeli")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.lightModeliv = GETFN_(proto::LightModelivFn, "glLightModeliv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.lightf = GETFN_(proto::LightfFn, "glLightf")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.lightfv = GETFN_(proto::LightfvFn, "glLightfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.lighti = GETFN_(proto::LightiFn, "glLighti")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.lightiv = GETFN_(proto::LightivFn, "glLightiv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.lineStipple = GETFN_(proto::LineStippleFn, "glLineStipple")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.listBase = GETFN_(proto::ListBaseFn, "glListBase")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.loadIdentity = GETFN_(proto::LoadIdentityFn, "glLoadIdentity")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.loadMatrixd = GETFN_(proto::LoadMatrixdFn, "glLoadMatrixd")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.loadMatrixf = GETFN_(proto::LoadMatrixfFn, "glLoadMatrixf")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.loadName = GETFN_(proto::LoadNameFn, "glLoadName")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.loadTransposeMatrixd = GETFN_(proto::LoadTransposeMatrixdFn, "glLoadTransposeMatrixd")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.loadTransposeMatrixf = GETFN_(proto::LoadTransposeMatrixfFn, "glLoadTransposeMatrixf")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.map1d = GETFN_(proto::Map1dFn, "glMap1d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.map1f = GETFN_(proto::Map1fFn, "glMap1f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.map2d = GETFN_(proto::Map2dFn, "glMap2d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.map2f = GETFN_(proto::Map2fFn, "glMap2f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.mapGrid1d = GETFN_(proto::MapGrid1dFn, "glMapGrid1d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.mapGrid1f = GETFN_(proto::MapGrid1fFn, "glMapGrid1f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.mapGrid2d = GETFN_(proto::MapGrid2dFn, "glMapGrid2d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.mapGrid2f = GETFN_(proto::MapGrid2fFn, "glMapGrid2f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.materialf = GETFN_(proto::MaterialfFn, "glMaterialf")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.materialfv = GETFN_(proto::MaterialfvFn, "glMaterialfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.materiali = GETFN_(proto::MaterialiFn, "glMateriali")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.materialiv = GETFN_(proto::MaterialivFn, "glMaterialiv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.matrixMode = GETFN_(proto::MatrixModeFn, "glMatrixMode")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.multMatrixd = GETFN_(proto::MultMatrixdFn, "glMultMatrixd")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.multMatrixf = GETFN_(proto::MultMatrixfFn, "glMultMatrixf")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multTransposeMatrixd = GETFN_(proto::MultTransposeMatrixdFn, "glMultTransposeMatrixd")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multTransposeMatrixf = GETFN_(proto::MultTransposeMatrixfFn, "glMultTransposeMatrixf")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord1d = GETFN_(proto::MultiTexCoord1dFn, "glMultiTexCoord1d")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord1dv = GETFN_(proto::MultiTexCoord1dvFn, "glMultiTexCoord1dv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord1f = GETFN_(proto::MultiTexCoord1fFn, "glMultiTexCoord1f")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord1fv = GETFN_(proto::MultiTexCoord1fvFn, "glMultiTexCoord1fv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord1i = GETFN_(proto::MultiTexCoord1iFn, "glMultiTexCoord1i")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord1iv = GETFN_(proto::MultiTexCoord1ivFn, "glMultiTexCoord1iv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord1s = GETFN_(proto::MultiTexCoord1sFn, "glMultiTexCoord1s")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord1sv = GETFN_(proto::MultiTexCoord1svFn, "glMultiTexCoord1sv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord2d = GETFN_(proto::MultiTexCoord2dFn, "glMultiTexCoord2d")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord2dv = GETFN_(proto::MultiTexCoord2dvFn, "glMultiTexCoord2dv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord2f = GETFN_(proto::MultiTexCoord2fFn, "glMultiTexCoord2f")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord2fv = GETFN_(proto::MultiTexCoord2fvFn, "glMultiTexCoord2fv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord2i = GETFN_(proto::MultiTexCoord2iFn, "glMultiTexCoord2i")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord2iv = GETFN_(proto::MultiTexCoord2ivFn, "glMultiTexCoord2iv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord2s = GETFN_(proto::MultiTexCoord2sFn, "glMultiTexCoord2s")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord2sv = GETFN_(proto::MultiTexCoord2svFn, "glMultiTexCoord2sv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord3d = GETFN_(proto::MultiTexCoord3dFn, "glMultiTexCoord3d")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord3dv = GETFN_(proto::MultiTexCoord3dvFn, "glMultiTexCoord3dv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord3f = GETFN_(proto::MultiTexCoord3fFn, "glMultiTexCoord3f")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord3fv = GETFN_(proto::MultiTexCoord3fvFn, "glMultiTexCoord3fv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord3i = GETFN_(proto::MultiTexCoord3iFn, "glMultiTexCoord3i")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord3iv = GETFN_(proto::MultiTexCoord3ivFn, "glMultiTexCoord3iv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord3s = GETFN_(proto::MultiTexCoord3sFn, "glMultiTexCoord3s")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord3sv = GETFN_(proto::MultiTexCoord3svFn, "glMultiTexCoord3sv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord4d = GETFN_(proto::MultiTexCoord4dFn, "glMultiTexCoord4d")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord4dv = GETFN_(proto::MultiTexCoord4dvFn, "glMultiTexCoord4dv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord4f = GETFN_(proto::MultiTexCoord4fFn, "glMultiTexCoord4f")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord4fv = GETFN_(proto::MultiTexCoord4fvFn, "glMultiTexCoord4fv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord4i = GETFN_(proto::MultiTexCoord4iFn, "glMultiTexCoord4i")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord4iv = GETFN_(proto::MultiTexCoord4ivFn, "glMultiTexCoord4iv")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord4s = GETFN_(proto::MultiTexCoord4sFn, "glMultiTexCoord4s")) && ok;
		}
		if( !gl.VERSION_1_3 ) {
			ok = !!(gl.multiTexCoord4sv = GETFN_(proto::MultiTexCoord4svFn, "glMultiTexCoord4sv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.newList = GETFN_(proto::NewListFn, "glNewList")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.normal3b = GETFN_(proto::Normal3bFn, "glNormal3b")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.normal3bv = GETFN_(proto::Normal3bvFn, "glNormal3bv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.normal3d = GETFN_(proto::Normal3dFn, "glNormal3d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.normal3dv = GETFN_(proto::Normal3dvFn, "glNormal3dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.normal3f = GETFN_(proto::Normal3fFn, "glNormal3f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.normal3fv = GETFN_(proto::Normal3fvFn, "glNormal3fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.normal3i = GETFN_(proto::Normal3iFn, "glNormal3i")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.normal3iv = GETFN_(proto::Normal3ivFn, "glNormal3iv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.normal3s = GETFN_(proto::Normal3sFn, "glNormal3s")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.normal3sv = GETFN_(proto::Normal3svFn, "glNormal3sv")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.normalPointer = GETFN_(proto::NormalPointerFn, "glNormalPointer")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.ortho = GETFN_(proto::OrthoFn, "glOrtho")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.passThrough = GETFN_(proto::PassThroughFn, "glPassThrough")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.pixelMapfv = GETFN_(proto::PixelMapfvFn, "glPixelMapfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.pixelMapuiv = GETFN_(proto::PixelMapuivFn, "glPixelMapuiv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.pixelMapusv = GETFN_(proto::PixelMapusvFn, "glPixelMapusv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.pixelTransferf = GETFN_(proto::PixelTransferfFn, "glPixelTransferf")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.pixelTransferi = GETFN_(proto::PixelTransferiFn, "glPixelTransferi")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.pixelZoom = GETFN_(proto::PixelZoomFn, "glPixelZoom")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.polygonStipple = GETFN_(proto::PolygonStippleFn, "glPolygonStipple")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.popAttrib = GETFN_(proto::PopAttribFn, "glPopAttrib")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.popClientAttrib = GETFN_(proto::PopClientAttribFn, "glPopClientAttrib")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.popMatrix = GETFN_(proto::PopMatrixFn, "glPopMatrix")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.popName = GETFN_(proto::PopNameFn, "glPopName")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.prioritizeTextures = GETFN_(proto::PrioritizeTexturesFn, "glPrioritizeTextures")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.pushAttrib = GETFN_(proto::PushAttribFn, "glPushAttrib")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.pushClientAttrib = GETFN_(proto::PushClientAttribFn, "glPushClientAttrib")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.pushMatrix = GETFN_(proto::PushMatrixFn, "glPushMatrix")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.pushName = GETFN_(proto::PushNameFn, "glPushName")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos2d = GETFN_(proto::RasterPos2dFn, "glRasterPos2d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos2dv = GETFN_(proto::RasterPos2dvFn, "glRasterPos2dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos2f = GETFN_(proto::RasterPos2fFn, "glRasterPos2f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos2fv = GETFN_(proto::RasterPos2fvFn, "glRasterPos2fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos2i = GETFN_(proto::RasterPos2iFn, "glRasterPos2i")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos2iv = GETFN_(proto::RasterPos2ivFn, "glRasterPos2iv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos2s = GETFN_(proto::RasterPos2sFn, "glRasterPos2s")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos2sv = GETFN_(proto::RasterPos2svFn, "glRasterPos2sv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos3d = GETFN_(proto::RasterPos3dFn, "glRasterPos3d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos3dv = GETFN_(proto::RasterPos3dvFn, "glRasterPos3dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos3f = GETFN_(proto::RasterPos3fFn, "glRasterPos3f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos3fv = GETFN_(proto::RasterPos3fvFn, "glRasterPos3fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos3i = GETFN_(proto::RasterPos3iFn, "glRasterPos3i")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos3iv = GETFN_(proto::RasterPos3ivFn, "glRasterPos3iv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos3s = GETFN_(proto::RasterPos3sFn, "glRasterPos3s")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos3sv = GETFN_(proto::RasterPos3svFn, "glRasterPos3sv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos4d = GETFN_(proto::RasterPos4dFn, "glRasterPos4d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos4dv = GETFN_(proto::RasterPos4dvFn, "glRasterPos4dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos4f = GETFN_(proto::RasterPos4fFn, "glRasterPos4f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos4fv = GETFN_(proto::RasterPos4fvFn, "glRasterPos4fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos4i = GETFN_(proto::RasterPos4iFn, "glRasterPos4i")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos4iv = GETFN_(proto::RasterPos4ivFn, "glRasterPos4iv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos4s = GETFN_(proto::RasterPos4sFn, "glRasterPos4s")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rasterPos4sv = GETFN_(proto::RasterPos4svFn, "glRasterPos4sv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rectd = GETFN_(proto::RectdFn, "glRectd")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rectdv = GETFN_(proto::RectdvFn, "glRectdv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rectf = GETFN_(proto::RectfFn, "glRectf")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rectfv = GETFN_(proto::RectfvFn, "glRectfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.recti = GETFN_(proto::RectiFn, "glRecti")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rectiv = GETFN_(proto::RectivFn, "glRectiv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rects = GETFN_(proto::RectsFn, "glRects")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rectsv = GETFN_(proto::RectsvFn, "glRectsv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.renderMode = GETFN_(proto::RenderModeFn, "glRenderMode")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rotated = GETFN_(proto::RotatedFn, "glRotated")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.rotatef = GETFN_(proto::RotatefFn, "glRotatef")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.scaled = GETFN_(proto::ScaledFn, "glScaled")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.scalef = GETFN_(proto::ScalefFn, "glScalef")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3b = GETFN_(proto::SecondaryColor3bFn, "glSecondaryColor3b")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3bv = GETFN_(proto::SecondaryColor3bvFn, "glSecondaryColor3bv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3d = GETFN_(proto::SecondaryColor3dFn, "glSecondaryColor3d")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3dv = GETFN_(proto::SecondaryColor3dvFn, "glSecondaryColor3dv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3f = GETFN_(proto::SecondaryColor3fFn, "glSecondaryColor3f")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3fv = GETFN_(proto::SecondaryColor3fvFn, "glSecondaryColor3fv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3i = GETFN_(proto::SecondaryColor3iFn, "glSecondaryColor3i")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3iv = GETFN_(proto::SecondaryColor3ivFn, "glSecondaryColor3iv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3s = GETFN_(proto::SecondaryColor3sFn, "glSecondaryColor3s")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3sv = GETFN_(proto::SecondaryColor3svFn, "glSecondaryColor3sv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3ub = GETFN_(proto::SecondaryColor3ubFn, "glSecondaryColor3ub")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3ubv = GETFN_(proto::SecondaryColor3ubvFn, "glSecondaryColor3ubv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3ui = GETFN_(proto::SecondaryColor3uiFn, "glSecondaryColor3ui")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3uiv = GETFN_(proto::SecondaryColor3uivFn, "glSecondaryColor3uiv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3us = GETFN_(proto::SecondaryColor3usFn, "glSecondaryColor3us")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColor3usv = GETFN_(proto::SecondaryColor3usvFn, "glSecondaryColor3usv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.secondaryColorPointer = GETFN_(proto::SecondaryColorPointerFn, "glSecondaryColorPointer")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.selectBuffer = GETFN_(proto::SelectBufferFn, "glSelectBuffer")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.shadeModel = GETFN_(proto::ShadeModelFn, "glShadeModel")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord1d = GETFN_(proto::TexCoord1dFn, "glTexCoord1d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord1dv = GETFN_(proto::TexCoord1dvFn, "glTexCoord1dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord1f = GETFN_(proto::TexCoord1fFn, "glTexCoord1f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord1fv = GETFN_(proto::TexCoord1fvFn, "glTexCoord1fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord1i = GETFN_(proto::TexCoord1iFn, "glTexCoord1i")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord1iv = GETFN_(proto::TexCoord1ivFn, "glTexCoord1iv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord1s = GETFN_(proto::TexCoord1sFn, "glTexCoord1s")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord1sv = GETFN_(proto::TexCoord1svFn, "glTexCoord1sv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord2d = GETFN_(proto::TexCoord2dFn, "glTexCoord2d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord2dv = GETFN_(proto::TexCoord2dvFn, "glTexCoord2dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord2f = GETFN_(proto::TexCoord2fFn, "glTexCoord2f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord2fv = GETFN_(proto::TexCoord2fvFn, "glTexCoord2fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord2i = GETFN_(proto::TexCoord2iFn, "glTexCoord2i")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord2iv = GETFN_(proto::TexCoord2ivFn, "glTexCoord2iv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord2s = GETFN_(proto::TexCoord2sFn, "glTexCoord2s")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord2sv = GETFN_(proto::TexCoord2svFn, "glTexCoord2sv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord3d = GETFN_(proto::TexCoord3dFn, "glTexCoord3d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord3dv = GETFN_(proto::TexCoord3dvFn, "glTexCoord3dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord3f = GETFN_(proto::TexCoord3fFn, "glTexCoord3f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord3fv = GETFN_(proto::TexCoord3fvFn, "glTexCoord3fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord3i = GETFN_(proto::TexCoord3iFn, "glTexCoord3i")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord3iv = GETFN_(proto::TexCoord3ivFn, "glTexCoord3iv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord3s = GETFN_(proto::TexCoord3sFn, "glTexCoord3s")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord3sv = GETFN_(proto::TexCoord3svFn, "glTexCoord3sv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord4d = GETFN_(proto::TexCoord4dFn, "glTexCoord4d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord4dv = GETFN_(proto::TexCoord4dvFn, "glTexCoord4dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord4f = GETFN_(proto::TexCoord4fFn, "glTexCoord4f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord4fv = GETFN_(proto::TexCoord4fvFn, "glTexCoord4fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord4i = GETFN_(proto::TexCoord4iFn, "glTexCoord4i")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord4iv = GETFN_(proto::TexCoord4ivFn, "glTexCoord4iv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord4s = GETFN_(proto::TexCoord4sFn, "glTexCoord4s")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texCoord4sv = GETFN_(proto::TexCoord4svFn, "glTexCoord4sv")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.texCoordPointer = GETFN_(proto::TexCoordPointerFn, "glTexCoordPointer")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texEnvf = GETFN_(proto::TexEnvfFn, "glTexEnvf")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texEnvfv = GETFN_(proto::TexEnvfvFn, "glTexEnvfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texEnvi = GETFN_(proto::TexEnviFn, "glTexEnvi")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texEnviv = GETFN_(proto::TexEnvivFn, "glTexEnviv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texGend = GETFN_(proto::TexGendFn, "glTexGend")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texGendv = GETFN_(proto::TexGendvFn, "glTexGendv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texGenf = GETFN_(proto::TexGenfFn, "glTexGenf")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texGenfv = GETFN_(proto::TexGenfvFn, "glTexGenfv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texGeni = GETFN_(proto::TexGeniFn, "glTexGeni")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.texGeniv = GETFN_(proto::TexGenivFn, "glTexGeniv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.translated = GETFN_(proto::TranslatedFn, "glTranslated")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.translatef = GETFN_(proto::TranslatefFn, "glTranslatef")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex2d = GETFN_(proto::Vertex2dFn, "glVertex2d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex2dv = GETFN_(proto::Vertex2dvFn, "glVertex2dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex2f = GETFN_(proto::Vertex2fFn, "glVertex2f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex2fv = GETFN_(proto::Vertex2fvFn, "glVertex2fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex2i = GETFN_(proto::Vertex2iFn, "glVertex2i")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex2iv = GETFN_(proto::Vertex2ivFn, "glVertex2iv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex2s = GETFN_(proto::Vertex2sFn, "glVertex2s")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex2sv = GETFN_(proto::Vertex2svFn, "glVertex2sv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex3d = GETFN_(proto::Vertex3dFn, "glVertex3d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex3dv = GETFN_(proto::Vertex3dvFn, "glVertex3dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex3f = GETFN_(proto::Vertex3fFn, "glVertex3f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex3fv = GETFN_(proto::Vertex3fvFn, "glVertex3fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex3i = GETFN_(proto::Vertex3iFn, "glVertex3i")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex3iv = GETFN_(proto::Vertex3ivFn, "glVertex3iv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex3s = GETFN_(proto::Vertex3sFn, "glVertex3s")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex3sv = GETFN_(proto::Vertex3svFn, "glVertex3sv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex4d = GETFN_(proto::Vertex4dFn, "glVertex4d")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex4dv = GETFN_(proto::Vertex4dvFn, "glVertex4dv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex4f = GETFN_(proto::Vertex4fFn, "glVertex4f")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex4fv = GETFN_(proto::Vertex4fvFn, "glVertex4fv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex4i = GETFN_(proto::Vertex4iFn, "glVertex4i")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex4iv = GETFN_(proto::Vertex4ivFn, "glVertex4iv")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex4s = GETFN_(proto::Vertex4sFn, "glVertex4s")) && ok;
		}
		if( !gl.VERSION_1_0 ) {
			ok = !!(gl.vertex4sv = GETFN_(proto::Vertex4svFn, "glVertex4sv")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.vertexPointer = GETFN_(proto::VertexPointerFn, "glVertexPointer")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos2d = GETFN_(proto::WindowPos2dFn, "glWindowPos2d")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos2dv = GETFN_(proto::WindowPos2dvFn, "glWindowPos2dv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos2f = GETFN_(proto::WindowPos2fFn, "glWindowPos2f")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos2fv = GETFN_(proto::WindowPos2fvFn, "glWindowPos2fv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos2i = GETFN_(proto::WindowPos2iFn, "glWindowPos2i")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos2iv = GETFN_(proto::WindowPos2ivFn, "glWindowPos2iv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos2s = GETFN_(proto::WindowPos2sFn, "glWindowPos2s")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos2sv = GETFN_(proto::WindowPos2svFn, "glWindowPos2sv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos3d = GETFN_(proto::WindowPos3dFn, "glWindowPos3d")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos3dv = GETFN_(proto::WindowPos3dvFn, "glWindowPos3dv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos3f = GETFN_(proto::WindowPos3fFn, "glWindowPos3f")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos3fv = GETFN_(proto::WindowPos3fvFn, "glWindowPos3fv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos3i = GETFN_(proto::WindowPos3iFn, "glWindowPos3i")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos3iv = GETFN_(proto::WindowPos3ivFn, "glWindowPos3iv")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos3s = GETFN_(proto::WindowPos3sFn, "glWindowPos3s")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.windowPos3sv = GETFN_(proto::WindowPos3svFn, "glWindowPos3sv")) && ok;
		}
		ok = ok && gl.VERSION_3_1_compatibility;
		gl.VERSION_3_2_compatibility = ok;
	}
	//- end: Compat 3.2

	//-//-- 
	//- Core 3.3
	if( arg.major > 3 || (arg.major == 3 && arg.minor >= 3 ) )
	{
		bool ok = true;

		ok = !!(gl.bindFragDataLocationIndexed = GETFN_(proto::BindFragDataLocationIndexedFn, "glBindFragDataLocationIndexed")) && ok;
		ok = !!(gl.bindSampler = GETFN_(proto::BindSamplerFn, "glBindSampler")) && ok;
		ok = !!(gl.deleteSamplers = GETFN_(proto::DeleteSamplersFn, "glDeleteSamplers")) && ok;
		ok = !!(gl.genSamplers = GETFN_(proto::GenSamplersFn, "glGenSamplers")) && ok;
		ok = !!(gl.getFragDataIndex = GETFN_(proto::GetFragDataIndexFn, "glGetFragDataIndex")) && ok;
		ok = !!(gl.getQueryObjecti64v = GETFN_(proto::GetQueryObjecti64vFn, "glGetQueryObjecti64v")) && ok;
		ok = !!(gl.getQueryObjectui64v = GETFN_(proto::GetQueryObjectui64vFn, "glGetQueryObjectui64v")) && ok;
		ok = !!(gl.getSamplerParameterIiv = GETFN_(proto::GetSamplerParameterIivFn, "glGetSamplerParameterIiv")) && ok;
		ok = !!(gl.getSamplerParameterIuiv = GETFN_(proto::GetSamplerParameterIuivFn, "glGetSamplerParameterIuiv")) && ok;
		ok = !!(gl.getSamplerParameterfv = GETFN_(proto::GetSamplerParameterfvFn, "glGetSamplerParameterfv")) && ok;
		ok = !!(gl.getSamplerParameteriv = GETFN_(proto::GetSamplerParameterivFn, "glGetSamplerParameteriv")) && ok;
		ok = !!(gl.isSampler = GETFN_(proto::IsSamplerFn, "glIsSampler")) && ok;
		ok = !!(gl.queryCounter = GETFN_(proto::QueryCounterFn, "glQueryCounter")) && ok;
		ok = !!(gl.samplerParameterIiv = GETFN_(proto::SamplerParameterIivFn, "glSamplerParameterIiv")) && ok;
		ok = !!(gl.samplerParameterIuiv = GETFN_(proto::SamplerParameterIuivFn, "glSamplerParameterIuiv")) && ok;
		ok = !!(gl.samplerParameterf = GETFN_(proto::SamplerParameterfFn, "glSamplerParameterf")) && ok;
		ok = !!(gl.samplerParameterfv = GETFN_(proto::SamplerParameterfvFn, "glSamplerParameterfv")) && ok;
		ok = !!(gl.samplerParameteri = GETFN_(proto::SamplerParameteriFn, "glSamplerParameteri")) && ok;
		ok = !!(gl.samplerParameteriv = GETFN_(proto::SamplerParameterivFn, "glSamplerParameteriv")) && ok;
		ok = !!(gl.vertexAttribDivisor = GETFN_(proto::VertexAttribDivisorFn, "glVertexAttribDivisor")) && ok;
		ok = !!(gl.vertexAttribP1ui = GETFN_(proto::VertexAttribP1uiFn, "glVertexAttribP1ui")) && ok;
		ok = !!(gl.vertexAttribP1uiv = GETFN_(proto::VertexAttribP1uivFn, "glVertexAttribP1uiv")) && ok;
		ok = !!(gl.vertexAttribP2ui = GETFN_(proto::VertexAttribP2uiFn, "glVertexAttribP2ui")) && ok;
		ok = !!(gl.vertexAttribP2uiv = GETFN_(proto::VertexAttribP2uivFn, "glVertexAttribP2uiv")) && ok;
		ok = !!(gl.vertexAttribP3ui = GETFN_(proto::VertexAttribP3uiFn, "glVertexAttribP3ui")) && ok;
		ok = !!(gl.vertexAttribP3uiv = GETFN_(proto::VertexAttribP3uivFn, "glVertexAttribP3uiv")) && ok;
		ok = !!(gl.vertexAttribP4ui = GETFN_(proto::VertexAttribP4uiFn, "glVertexAttribP4ui")) && ok;
		ok = !!(gl.vertexAttribP4uiv = GETFN_(proto::VertexAttribP4uivFn, "glVertexAttribP4uiv")) && ok;
		{
			ok = ok && gl.vertexAttribIPointer;
			ok = ok && gl.vertexAttrib1s;
			ok = ok && gl.depthRange;
			ok = ok && gl.vertexAttribI3uiv;
			ok = ok && gl.disable;
			ok = ok && gl.clearStencil;
			ok = ok && gl.texSubImage1D;
			ok = ok && gl.framebufferTexture1D;
			ok = ok && gl.getAttribLocation;
			ok = ok && gl.pointParameterfv;
			ok = ok && gl.vertexAttrib2dv;
			ok = ok && gl.polygonOffset;
			ok = ok && gl.pixelStorei;
			ok = ok && gl.pointParameteriv;
			ok = ok && gl.vertexAttribI1ui;
			ok = ok && gl.blendEquation;
			ok = ok && gl.genFramebuffers;
			ok = ok && gl.drawArraysInstanced;
			ok = ok && gl.stencilFunc;
			ok = ok && gl.depthMask;
			ok = ok && gl.deleteRenderbuffers;
			ok = ok && gl.getSynciv;
			ok = ok && gl.primitiveRestartIndex;
			ok = ok && gl.getFragDataLocation;
			ok = ok && gl.genTextures;
			ok = ok && gl.stencilOp;
			ok = ok && gl.getUniformfv;
			ok = ok && gl.texSubImage3D;
			ok = ok && gl.vertexAttribI4i;
			ok = ok && gl.endQuery;
			ok = ok && gl.vertexAttrib2sv;
			ok = ok && gl.getBooleanv;
			ok = ok && gl.vertexAttrib3sv;
			ok = ok && gl.drawBuffers;
			ok = ok && gl.uniform3f;
			ok = ok && gl.getQueryObjectuiv;
			ok = ok && gl.copyTexImage2D;
			ok = ok && gl.uniformBlockBinding;
			ok = ok && gl.vertexAttrib2fv;
			ok = ok && gl.provokingVertex;
			ok = ok && gl.copyBufferSubData;
			ok = ok && gl.vertexAttribI2i;
			ok = ok && gl.uniform4uiv;
			ok = ok && gl.uniform4ui;
			ok = ok && gl.hint;
			ok = ok && gl.blendFuncSeparate;
			ok = ok && gl.viewport;
			ok = ok && gl.getQueryiv;
			ok = ok && gl.getRenderbufferParameteriv;
			ok = ok && gl.shaderSource;
			ok = ok && gl.stencilMask;
			ok = ok && gl.vertexAttrib4Niv;
			ok = ok && gl.framebufferTextureLayer;
			ok = ok && gl.getBufferPointerv;
			ok = ok && gl.getTexImage;
			ok = ok && gl.stencilMaskSeparate;
			ok = ok && gl.getTexParameterIiv;
			ok = ok && gl.isTexture;
			ok = ok && gl.texParameteriv;
			ok = ok && gl.vertexAttribI1i;
			ok = ok && gl.bindFragDataLocation;
			ok = ok && gl.getBufferParameteri64v;
			ok = ok && gl.getIntegerv;
			ok = ok && gl.vertexAttribI4usv;
			ok = ok && gl.readPixels;
			ok = ok && gl.compressedTexSubImage1D;
			ok = ok && gl.attachShader;
			ok = ok && gl.getDoublev;
			ok = ok && gl.stencilFuncSeparate;
			ok = ok && gl.vertexAttrib4ubv;
			ok = ok && gl.uniformMatrix3x2fv;
			ok = ok && gl.uniform4fv;
			ok = ok && gl.enableVertexAttribArray;
			ok = ok && gl.vertexAttrib4s;
			ok = ok && gl.vertexAttrib4iv;
			ok = ok && gl.getUniformBlockIndex;
			ok = ok && gl.pointParameterf;
			ok = ok && gl.getVertexAttribdv;
			ok = ok && gl.getString;
			ok = ok && gl.blendEquationSeparate;
			ok = ok && gl.getVertexAttribIuiv;
			ok = ok && gl.vertexAttribI3i;
			ok = ok && gl.uniformMatrix4x2fv;
			ok = ok && gl.vertexAttribI2uiv;
			ok = ok && gl.isFramebuffer;
			ok = ok && gl.copyTexImage1D;
			ok = ok && gl.getShaderInfoLog;
			ok = ok && gl.createProgram;
			ok = ok && gl.multiDrawElements;
			ok = ok && gl.getBufferSubData;
			ok = ok && gl.texImage3DMultisample;
			ok = ok && gl.getError;
			ok = ok && gl.logicOp;
			ok = ok && gl.vertexAttrib4Nub;
			ok = ok && gl.useProgram;
			ok = ok && gl.vertexAttrib4dv;
			ok = ok && gl.colorMask;
			ok = ok && gl.isShader;
			ok = ok && gl.vertexAttrib1fv;
			ok = ok && gl.beginQuery;
			ok = ok && gl.vertexAttribI2iv;
			ok = ok && gl.getFramebufferAttachmentParameteriv;
			ok = ok && gl.texBuffer;
			ok = ok && gl.frontFace;
			ok = ok && gl.vertexAttribI4iv;
			ok = ok && gl.isRenderbuffer;
			ok = ok && gl.deleteShader;
			ok = ok && gl.texSubImage2D;
			ok = ok && gl.pointParameteri;
			ok = ok && gl.getInteger64v;
			ok = ok && gl.copyTexSubImage3D;
			ok = ok && gl.getActiveUniformBlockName;
			ok = ok && gl.lineWidth;
			ok = ok && gl.drawBuffer;
			ok = ok && gl.getShaderSource;
			ok = ok && gl.deleteSync;
			ok = ok && gl.getProgramiv;
			ok = ok && gl.drawElementsInstancedBaseVertex;
			ok = ok && gl.deleteFramebuffers;
			ok = ok && gl.uniform2i;
			ok = ok && gl.getUniformiv;
			ok = ok && gl.getProgramInfoLog;
			ok = ok && gl.genRenderbuffers;
			ok = ok && gl.getVertexAttribfv;
			ok = ok && gl.renderbufferStorage;
			ok = ok && gl.uniform1f;
			ok = ok && gl.uniform2iv;
			ok = ok && gl.blendFunc;
			ok = ok && gl.getMultisamplefv;
			ok = ok && gl.uniformMatrix2x3fv;
			ok = ok && gl.stencilOpSeparate;
			ok = ok && gl.getVertexAttribiv;
			ok = ok && gl.getUniformIndices;
			ok = ok && gl.uniformMatrix2x4fv;
			ok = ok && gl.vertexAttrib3f;
			ok = ok && gl.vertexAttrib4f;
			ok = ok && gl.getIntegeri_v;
			ok = ok && gl.disableVertexAttribArray;
			ok = ok && gl.beginTransformFeedback;
			ok = ok && gl.vertexAttrib4Nusv;
			ok = ok && gl.getInteger64i_v;
			ok = ok && gl.blendColor;
			ok = ok && gl.getBufferParameteriv;
			ok = ok && gl.clearColor;
			ok = ok && gl.genVertexArrays;
			ok = ok && gl.getTexParameterfv;
			ok = ok && gl.getQueryObjectiv;
			ok = ok && gl.compressedTexImage3D;
			ok = ok && gl.uniform2ui;
			ok = ok && gl.vertexAttribI4ubv;
			ok = ok && gl.drawElementsInstanced;
			ok = ok && gl.drawElements;
			ok = ok && gl.drawRangeElementsBaseVertex;
			ok = ok && gl.copyTexSubImage1D;
			ok = ok && gl.getCompressedTexImage;
			ok = ok && gl.compressedTexImage1D;
			ok = ok && gl.pointSize;
			ok = ok && gl.getTexParameterIuiv;
			ok = ok && gl.deleteVertexArrays;
			ok = ok && gl.getShaderiv;
			ok = ok && gl.uniform1fv;
			ok = ok && gl.drawArrays;
			ok = ok && gl.uniform1iv;
			ok = ok && gl.bufferSubData;
			ok = ok && gl.deleteQueries;
			ok = ok && gl.generateMipmap;
			ok = ok && gl.enable;
			ok = ok && gl.clearBufferiv;
			ok = ok && gl.activeTexture;
			ok = ok && gl.bufferData;
			ok = ok && gl.vertexAttribI3iv;
			ok = ok && gl.getActiveUniformBlockiv;
			ok = ok && gl.uniform2uiv;
			ok = ok && gl.clearBufferfv;
			ok = ok && gl.transformFeedbackVaryings;
			ok = ok && gl.mapBuffer;
			ok = ok && gl.uniform3iv;
			ok = ok && gl.drawElementsBaseVertex;
			ok = ok && gl.getUniformLocation;
			ok = ok && gl.unmapBuffer;
			ok = ok && gl.bindBufferBase;
			ok = ok && gl.createShader;
			ok = ok && gl.uniform4i;
			ok = ok && gl.validateProgram;
			ok = ok && gl.framebufferRenderbuffer;
			ok = ok && gl.getFloatv;
			ok = ok && gl.endConditionalRender;
			ok = ok && gl.getBooleani_v;
			ok = ok && gl.genBuffers;
			ok = ok && gl.vertexAttribPointer;
			ok = ok && gl.fenceSync;
			ok = ok && gl.uniformMatrix2fv;
			ok = ok && gl.drawRangeElements;
			ok = ok && gl.getVertexAttribIiv;
			ok = ok && gl.vertexAttribI4uiv;
			ok = ok && gl.vertexAttrib1dv;
			ok = ok && gl.framebufferTexture3D;
			ok = ok && gl.scissor;
			ok = ok && gl.deleteProgram;
			ok = ok && gl.clearBufferfi;
			ok = ok && gl.uniform4iv;
			ok = ok && gl.vertexAttrib2s;
			ok = ok && gl.vertexAttribI3ui;
			ok = ok && gl.texImage1D;
			ok = ok && gl.texImage3D;
			ok = ok && gl.cullFace;
			ok = ok && gl.deleteTextures;
			ok = ok && gl.compressedTexSubImage2D;
			ok = ok && gl.getStringi;
			ok = ok && gl.isBuffer;
			ok = ok && gl.vertexAttrib4uiv;
			ok = ok && gl.vertexAttrib4Nubv;
			ok = ok && gl.checkFramebufferStatus;
			ok = ok && gl.vertexAttrib4usv;
			ok = ok && gl.multiDrawElementsBaseVertex;
			ok = ok && gl.waitSync;
			ok = ok && gl.detachShader;
			ok = ok && gl.vertexAttrib1d;
			ok = ok && gl.renderbufferStorageMultisample;
			ok = ok && gl.texParameterIuiv;
			ok = ok && gl.vertexAttrib3d;
			ok = ok && gl.vertexAttrib3dv;
			ok = ok && gl.vertexAttrib4d;
			ok = ok && gl.vertexAttrib4Nbv;
			ok = ok && gl.texImage2D;
			ok = ok && gl.clampColor;
			ok = ok && gl.bindBuffer;
			ok = ok && gl.uniform3fv;
			ok = ok && gl.clientWaitSync;
			ok = ok && gl.endTransformFeedback;
			ok = ok && gl.getActiveUniformName;
			ok = ok && gl.compressedTexImage2D;
			ok = ok && gl.polygonMode;
			ok = ok && gl.vertexAttrib4sv;
			ok = ok && gl.framebufferTexture2D;
			ok = ok && gl.compressedTexSubImage3D;
			ok = ok && gl.getActiveAttrib;
			ok = ok && gl.texParameterf;
			ok = ok && gl.texImage2DMultisample;
			ok = ok && gl.readBuffer;
			ok = ok && gl.getTexParameteriv;
			ok = ok && gl.bindBufferRange;
			ok = ok && gl.vertexAttrib3s;
			ok = ok && gl.uniform2fv;
			ok = ok && gl.vertexAttrib1f;
			ok = ok && gl.enablei;
			ok = ok && gl.compileShader;
			ok = ok && gl.uniform2f;
			ok = ok && gl.uniform3uiv;
			ok = ok && gl.texParameterfv;
			ok = ok && gl.uniform1i;
			ok = ok && gl.clear;
			ok = ok && gl.vertexAttribI2ui;
			ok = ok && gl.uniformMatrix4x3fv;
			ok = ok && gl.linkProgram;
			ok = ok && gl.finish;
			ok = ok && gl.getUniformuiv;
			ok = ok && gl.vertexAttribI4ui;
			ok = ok && gl.uniformMatrix3fv;
			ok = ok && gl.sampleMaski;
			ok = ok && gl.bindFramebuffer;
			ok = ok && gl.vertexAttrib4Nuiv;
			ok = ok && gl.isEnabled;
			ok = ok && gl.vertexAttrib1sv;
			ok = ok && gl.getTexLevelParameterfv;
			ok = ok && gl.vertexAttribI4bv;
			ok = ok && gl.genQueries;
			ok = ok && gl.isEnabledi;
			ok = ok && gl.mapBufferRange;
			ok = ok && gl.sampleCoverage;
			ok = ok && gl.deleteBuffers;
			ok = ok && gl.clearDepth;
			ok = ok && gl.texParameterIiv;
			ok = ok && gl.isQuery;
			ok = ok && gl.disablei;
			ok = ok && gl.framebufferTexture;
			ok = ok && gl.uniformMatrix4fv;
			ok = ok && gl.vertexAttrib4Nsv;
			ok = ok && gl.getTransformFeedbackVarying;
			ok = ok && gl.getVertexAttribPointerv;
			ok = ok && gl.multiDrawArrays;
			ok = ok && gl.blitFramebuffer;
			ok = ok && gl.vertexAttribI1iv;
			ok = ok && gl.flushMappedBufferRange;
			ok = ok && gl.vertexAttrib2f;
			ok = ok && gl.bindRenderbuffer;
			ok = ok && gl.pixelStoref;
			ok = ok && gl.getTexLevelParameteriv;
			ok = ok && gl.getActiveUniform;
			ok = ok && gl.vertexAttribI4sv;
			ok = ok && gl.isVertexArray;
			ok = ok && gl.isSync;
			ok = ok && gl.uniform4f;
			ok = ok && gl.uniform1uiv;
			ok = ok && gl.vertexAttribI1uiv;
			ok = ok && gl.texParameteri;
			ok = ok && gl.vertexAttrib4bv;
			ok = ok && gl.uniform1ui;
			ok = ok && gl.depthFunc;
			ok = ok && gl.getAttachedShaders;
			ok = ok && gl.flush;
			ok = ok && gl.bindTexture;
			ok = ok && gl.bindAttribLocation;
			ok = ok && gl.copyTexSubImage2D;
			ok = ok && gl.uniform3ui;
			ok = ok && gl.uniform3i;
			ok = ok && gl.bindVertexArray;
			ok = ok && gl.getActiveUniformsiv;
			ok = ok && gl.vertexAttrib3fv;
			ok = ok && gl.clearBufferuiv;
			ok = ok && gl.beginConditionalRender;
			ok = ok && gl.uniformMatrix3x4fv;
			ok = ok && gl.colorMaski;
			ok = ok && gl.vertexAttrib2d;
			ok = ok && gl.isProgram;
			ok = ok && gl.vertexAttrib4fv;
		}
		gl.VERSION_3_3 = ok;
		if( ok ) {
			gl.VERSION_loaded_major = 3;
			gl.VERSION_loaded_minor = 3;
		}
	}
	//- end: Core 3.3

	//-//-- 
	//- Compatibility 3.3
	if( arg.major > 3 || (arg.major == 3 && arg.minor >= 3 ) )
	{
		bool ok = true;

		ok = !!(gl.colorP3ui = GETFN_(proto::ColorP3uiFn, "glColorP3ui")) && ok;
		ok = !!(gl.colorP3uiv = GETFN_(proto::ColorP3uivFn, "glColorP3uiv")) && ok;
		ok = !!(gl.colorP4ui = GETFN_(proto::ColorP4uiFn, "glColorP4ui")) && ok;
		ok = !!(gl.colorP4uiv = GETFN_(proto::ColorP4uivFn, "glColorP4uiv")) && ok;
		ok = !!(gl.multiTexCoordP1ui = GETFN_(proto::MultiTexCoordP1uiFn, "glMultiTexCoordP1ui")) && ok;
		ok = !!(gl.multiTexCoordP1uiv = GETFN_(proto::MultiTexCoordP1uivFn, "glMultiTexCoordP1uiv")) && ok;
		ok = !!(gl.multiTexCoordP2ui = GETFN_(proto::MultiTexCoordP2uiFn, "glMultiTexCoordP2ui")) && ok;
		ok = !!(gl.multiTexCoordP2uiv = GETFN_(proto::MultiTexCoordP2uivFn, "glMultiTexCoordP2uiv")) && ok;
		ok = !!(gl.multiTexCoordP3ui = GETFN_(proto::MultiTexCoordP3uiFn, "glMultiTexCoordP3ui")) && ok;
		ok = !!(gl.multiTexCoordP3uiv = GETFN_(proto::MultiTexCoordP3uivFn, "glMultiTexCoordP3uiv")) && ok;
		ok = !!(gl.multiTexCoordP4ui = GETFN_(proto::MultiTexCoordP4uiFn, "glMultiTexCoordP4ui")) && ok;
		ok = !!(gl.multiTexCoordP4uiv = GETFN_(proto::MultiTexCoordP4uivFn, "glMultiTexCoordP4uiv")) && ok;
		ok = !!(gl.normalP3ui = GETFN_(proto::NormalP3uiFn, "glNormalP3ui")) && ok;
		ok = !!(gl.normalP3uiv = GETFN_(proto::NormalP3uivFn, "glNormalP3uiv")) && ok;
		ok = !!(gl.secondaryColorP3ui = GETFN_(proto::SecondaryColorP3uiFn, "glSecondaryColorP3ui")) && ok;
		ok = !!(gl.secondaryColorP3uiv = GETFN_(proto::SecondaryColorP3uivFn, "glSecondaryColorP3uiv")) && ok;
		ok = !!(gl.texCoordP1ui = GETFN_(proto::TexCoordP1uiFn, "glTexCoordP1ui")) && ok;
		ok = !!(gl.texCoordP1uiv = GETFN_(proto::TexCoordP1uivFn, "glTexCoordP1uiv")) && ok;
		ok = !!(gl.texCoordP2ui = GETFN_(proto::TexCoordP2uiFn, "glTexCoordP2ui")) && ok;
		ok = !!(gl.texCoordP2uiv = GETFN_(proto::TexCoordP2uivFn, "glTexCoordP2uiv")) && ok;
		ok = !!(gl.texCoordP3ui = GETFN_(proto::TexCoordP3uiFn, "glTexCoordP3ui")) && ok;
		ok = !!(gl.texCoordP3uiv = GETFN_(proto::TexCoordP3uivFn, "glTexCoordP3uiv")) && ok;
		ok = !!(gl.texCoordP4ui = GETFN_(proto::TexCoordP4uiFn, "glTexCoordP4ui")) && ok;
		ok = !!(gl.texCoordP4uiv = GETFN_(proto::TexCoordP4uivFn, "glTexCoordP4uiv")) && ok;
		ok = !!(gl.vertexP2ui = GETFN_(proto::VertexP2uiFn, "glVertexP2ui")) && ok;
		ok = !!(gl.vertexP2uiv = GETFN_(proto::VertexP2uivFn, "glVertexP2uiv")) && ok;
		ok = !!(gl.vertexP3ui = GETFN_(proto::VertexP3uiFn, "glVertexP3ui")) && ok;
		ok = !!(gl.vertexP3uiv = GETFN_(proto::VertexP3uivFn, "glVertexP3uiv")) && ok;
		ok = !!(gl.vertexP4ui = GETFN_(proto::VertexP4uiFn, "glVertexP4ui")) && ok;
		ok = !!(gl.vertexP4uiv = GETFN_(proto::VertexP4uivFn, "glVertexP4uiv")) && ok;
		ok = ok && gl.VERSION_3_2_compatibility;
		gl.VERSION_3_3_compatibility = ok;
	}
	//- end: Compat 3.3

}

#undef GETFN_
//--    <<< ~ dlapi::gl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE3(dlapi,gl,detail)
//--//////////////////////////////////////////////////////////////////////////
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
