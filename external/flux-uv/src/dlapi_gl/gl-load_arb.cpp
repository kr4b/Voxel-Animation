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

void load_gl_arb_( GLapi& gl, LoaderArg_ const& arg )
{
	//-//-- 
	//- Ext: ARB_ES2_compatibility
	static char const* ARB_ES2_compatibility = "ARB_ES2_compatibility";
	if( listed_( arg.extList, ARB_ES2_compatibility ) && arg.checkExt( ARB_ES2_compatibility, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.clearDepthf = GETFN_(proto::ClearDepthfFn, "glClearDepthf")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.depthRangef = GETFN_(proto::DepthRangefFn, "glDepthRangef")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.getShaderPrecisionFormat = GETFN_(proto::GetShaderPrecisionFormatFn, "glGetShaderPrecisionFormat")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.releaseShaderCompiler = GETFN_(proto::ReleaseShaderCompilerFn, "glReleaseShaderCompiler")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.shaderBinary = GETFN_(proto::ShaderBinaryFn, "glShaderBinary")) && ok;
		}
		gl.ARB_ES2_compatibility = ok;
	}
	//- end: ARB_ES2_compatibility

	//-//-- 
	//- Ext: ARB_ES3_1_compatibility
	static char const* ARB_ES3_1_compatibility = "ARB_ES3_1_compatibility";
	if( listed_( arg.extList, ARB_ES3_1_compatibility ) && arg.checkExt( ARB_ES3_1_compatibility, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.memoryBarrierByRegion = GETFN_(proto::MemoryBarrierByRegionFn, "glMemoryBarrierByRegion")) && ok;
		}
		gl.ARB_ES3_1_compatibility = ok;
	}
	//- end: ARB_ES3_1_compatibility

	//-//-- 
	//- Ext: ARB_ES3_2_compatibility
	static char const* ARB_ES3_2_compatibility = "ARB_ES3_2_compatibility";
	if( listed_( arg.extList, ARB_ES3_2_compatibility ) && arg.checkExt( ARB_ES3_2_compatibility, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.primitiveBoundingBoxARB = GETFN_(proto::PrimitiveBoundingBoxARBFn, "glPrimitiveBoundingBoxARB")) && ok;
		gl.ARB_ES3_2_compatibility = ok;
	}
	//- end: ARB_ES3_2_compatibility

	//-//-- 
	//- Ext: ARB_base_instance
	static char const* ARB_base_instance = "ARB_base_instance";
	if( listed_( arg.extList, ARB_base_instance ) && arg.checkExt( ARB_base_instance, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_2 ) {
			ok = !!(gl.drawArraysInstancedBaseInstance = GETFN_(proto::DrawArraysInstancedBaseInstanceFn, "glDrawArraysInstancedBaseInstance")) && ok;
		}
		if( !gl.VERSION_4_2 ) {
			ok = !!(gl.drawElementsInstancedBaseInstance = GETFN_(proto::DrawElementsInstancedBaseInstanceFn, "glDrawElementsInstancedBaseInstance")) && ok;
		}
		if( !gl.VERSION_4_2 ) {
			ok = !!(gl.drawElementsInstancedBaseVertexBaseInstance = GETFN_(proto::DrawElementsInstancedBaseVertexBaseInstanceFn, "glDrawElementsInstancedBaseVertexBaseInstance")) && ok;
		}
		gl.ARB_base_instance = ok;
	}
	//- end: ARB_base_instance

	//-//-- 
	//- Ext: ARB_bindless_texture
	static char const* ARB_bindless_texture = "ARB_bindless_texture";
	if( listed_( arg.extList, ARB_bindless_texture ) && arg.checkExt( ARB_bindless_texture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getImageHandleARB = GETFN_(proto::GetImageHandleARBFn, "glGetImageHandleARB")) && ok;
		ok = !!(gl.getTextureHandleARB = GETFN_(proto::GetTextureHandleARBFn, "glGetTextureHandleARB")) && ok;
		ok = !!(gl.getTextureSamplerHandleARB = GETFN_(proto::GetTextureSamplerHandleARBFn, "glGetTextureSamplerHandleARB")) && ok;
		ok = !!(gl.getVertexAttribLui64vARB = GETFN_(proto::GetVertexAttribLui64vARBFn, "glGetVertexAttribLui64vARB")) && ok;
		ok = !!(gl.isImageHandleResidentARB = GETFN_(proto::IsImageHandleResidentARBFn, "glIsImageHandleResidentARB")) && ok;
		ok = !!(gl.isTextureHandleResidentARB = GETFN_(proto::IsTextureHandleResidentARBFn, "glIsTextureHandleResidentARB")) && ok;
		ok = !!(gl.makeImageHandleNonResidentARB = GETFN_(proto::MakeImageHandleNonResidentARBFn, "glMakeImageHandleNonResidentARB")) && ok;
		ok = !!(gl.makeImageHandleResidentARB = GETFN_(proto::MakeImageHandleResidentARBFn, "glMakeImageHandleResidentARB")) && ok;
		ok = !!(gl.makeTextureHandleNonResidentARB = GETFN_(proto::MakeTextureHandleNonResidentARBFn, "glMakeTextureHandleNonResidentARB")) && ok;
		ok = !!(gl.makeTextureHandleResidentARB = GETFN_(proto::MakeTextureHandleResidentARBFn, "glMakeTextureHandleResidentARB")) && ok;
		ok = !!(gl.programUniformHandleui64ARB = GETFN_(proto::ProgramUniformHandleui64ARBFn, "glProgramUniformHandleui64ARB")) && ok;
		ok = !!(gl.programUniformHandleui64vARB = GETFN_(proto::ProgramUniformHandleui64vARBFn, "glProgramUniformHandleui64vARB")) && ok;
		ok = !!(gl.uniformHandleui64ARB = GETFN_(proto::UniformHandleui64ARBFn, "glUniformHandleui64ARB")) && ok;
		ok = !!(gl.uniformHandleui64vARB = GETFN_(proto::UniformHandleui64vARBFn, "glUniformHandleui64vARB")) && ok;
		ok = !!(gl.vertexAttribL1ui64ARB = GETFN_(proto::VertexAttribL1ui64ARBFn, "glVertexAttribL1ui64ARB")) && ok;
		ok = !!(gl.vertexAttribL1ui64vARB = GETFN_(proto::VertexAttribL1ui64vARBFn, "glVertexAttribL1ui64vARB")) && ok;
		gl.ARB_bindless_texture = ok;
	}
	//- end: ARB_bindless_texture

	//-//-- 
	//- Ext: ARB_blend_func_extended
	static char const* ARB_blend_func_extended = "ARB_blend_func_extended";
	if( listed_( arg.extList, ARB_blend_func_extended ) && arg.checkExt( ARB_blend_func_extended, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.bindFragDataLocationIndexed = GETFN_(proto::BindFragDataLocationIndexedFn, "glBindFragDataLocationIndexed")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.getFragDataIndex = GETFN_(proto::GetFragDataIndexFn, "glGetFragDataIndex")) && ok;
		}
		gl.ARB_blend_func_extended = ok;
	}
	//- end: ARB_blend_func_extended

	//-//-- 
	//- Ext: ARB_buffer_storage
	static char const* ARB_buffer_storage = "ARB_buffer_storage";
	if( listed_( arg.extList, ARB_buffer_storage ) && arg.checkExt( ARB_buffer_storage, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_4 ) {
			ok = !!(gl.bufferStorage = GETFN_(proto::BufferStorageFn, "glBufferStorage")) && ok;
		}
		gl.ARB_buffer_storage = ok;
	}
	//- end: ARB_buffer_storage

	//-//-- 
	//- Ext: ARB_cl_event
	static char const* ARB_cl_event = "ARB_cl_event";
	if( listed_( arg.extList, ARB_cl_event ) && arg.checkExt( ARB_cl_event, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.createSyncFromCLeventARB = GETFN_(proto::CreateSyncFromCLeventARBFn, "glCreateSyncFromCLeventARB")) && ok;
		gl.ARB_cl_event = ok;
	}
	//- end: ARB_cl_event

	//-//-- 
	//- Ext: ARB_clear_buffer_object
	static char const* ARB_clear_buffer_object = "ARB_clear_buffer_object";
	if( listed_( arg.extList, ARB_clear_buffer_object ) && arg.checkExt( ARB_clear_buffer_object, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.clearBufferData = GETFN_(proto::ClearBufferDataFn, "glClearBufferData")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.clearBufferSubData = GETFN_(proto::ClearBufferSubDataFn, "glClearBufferSubData")) && ok;
		}
		gl.ARB_clear_buffer_object = ok;
	}
	//- end: ARB_clear_buffer_object

	//-//-- 
	//- Ext: ARB_clear_texture
	static char const* ARB_clear_texture = "ARB_clear_texture";
	if( listed_( arg.extList, ARB_clear_texture ) && arg.checkExt( ARB_clear_texture, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_4 ) {
			ok = !!(gl.clearTexImage = GETFN_(proto::ClearTexImageFn, "glClearTexImage")) && ok;
		}
		if( !gl.VERSION_4_4 ) {
			ok = !!(gl.clearTexSubImage = GETFN_(proto::ClearTexSubImageFn, "glClearTexSubImage")) && ok;
		}
		gl.ARB_clear_texture = ok;
	}
	//- end: ARB_clear_texture

	//-//-- 
	//- Ext: ARB_clip_control
	static char const* ARB_clip_control = "ARB_clip_control";
	if( listed_( arg.extList, ARB_clip_control ) && arg.checkExt( ARB_clip_control, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.clipControl = GETFN_(proto::ClipControlFn, "glClipControl")) && ok;
		}
		gl.ARB_clip_control = ok;
	}
	//- end: ARB_clip_control

	//-//-- 
	//- Ext: ARB_color_buffer_float
	static char const* ARB_color_buffer_float = "ARB_color_buffer_float";
	if( listed_( arg.extList, ARB_color_buffer_float ) && arg.checkExt( ARB_color_buffer_float, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.clampColorARB = GETFN_(proto::ClampColorARBFn, "glClampColorARB")) && ok;
		gl.ARB_color_buffer_float = ok;
	}
	//- end: ARB_color_buffer_float

	//-//-- 
	//- Ext: ARB_compute_shader
	static char const* ARB_compute_shader = "ARB_compute_shader";
	if( listed_( arg.extList, ARB_compute_shader ) && arg.checkExt( ARB_compute_shader, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.dispatchCompute = GETFN_(proto::DispatchComputeFn, "glDispatchCompute")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.dispatchComputeIndirect = GETFN_(proto::DispatchComputeIndirectFn, "glDispatchComputeIndirect")) && ok;
		}
		gl.ARB_compute_shader = ok;
	}
	//- end: ARB_compute_shader

	//-//-- 
	//- Ext: ARB_compute_variable_group_size
	static char const* ARB_compute_variable_group_size = "ARB_compute_variable_group_size";
	if( listed_( arg.extList, ARB_compute_variable_group_size ) && arg.checkExt( ARB_compute_variable_group_size, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.dispatchComputeGroupSizeARB = GETFN_(proto::DispatchComputeGroupSizeARBFn, "glDispatchComputeGroupSizeARB")) && ok;
		gl.ARB_compute_variable_group_size = ok;
	}
	//- end: ARB_compute_variable_group_size

	//-//-- 
	//- Ext: ARB_copy_buffer
	static char const* ARB_copy_buffer = "ARB_copy_buffer";
	if( listed_( arg.extList, ARB_copy_buffer ) && arg.checkExt( ARB_copy_buffer, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_3_1 ) {
			ok = !!(gl.copyBufferSubData = GETFN_(proto::CopyBufferSubDataFn, "glCopyBufferSubData")) && ok;
		}
		gl.ARB_copy_buffer = ok;
	}
	//- end: ARB_copy_buffer

	//-//-- 
	//- Ext: ARB_copy_image
	static char const* ARB_copy_image = "ARB_copy_image";
	if( listed_( arg.extList, ARB_copy_image ) && arg.checkExt( ARB_copy_image, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.copyImageSubData = GETFN_(proto::CopyImageSubDataFn, "glCopyImageSubData")) && ok;
		}
		gl.ARB_copy_image = ok;
	}
	//- end: ARB_copy_image

	//-//-- 
	//- Ext: ARB_debug_output
	static char const* ARB_debug_output = "ARB_debug_output";
	if( listed_( arg.extList, ARB_debug_output ) && arg.checkExt( ARB_debug_output, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.debugMessageCallbackARB = GETFN_(proto::DebugMessageCallbackARBFn, "glDebugMessageCallbackARB")) && ok;
		ok = !!(gl.debugMessageControlARB = GETFN_(proto::DebugMessageControlARBFn, "glDebugMessageControlARB")) && ok;
		ok = !!(gl.debugMessageInsertARB = GETFN_(proto::DebugMessageInsertARBFn, "glDebugMessageInsertARB")) && ok;
		ok = !!(gl.getDebugMessageLogARB = GETFN_(proto::GetDebugMessageLogARBFn, "glGetDebugMessageLogARB")) && ok;
		gl.ARB_debug_output = ok;
	}
	//- end: ARB_debug_output

	//-//-- 
	//- Ext: ARB_direct_state_access
	static char const* ARB_direct_state_access = "ARB_direct_state_access";
	if( listed_( arg.extList, ARB_direct_state_access ) && arg.checkExt( ARB_direct_state_access, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.bindTextureUnit = GETFN_(proto::BindTextureUnitFn, "glBindTextureUnit")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.blitNamedFramebuffer = GETFN_(proto::BlitNamedFramebufferFn, "glBlitNamedFramebuffer")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.checkNamedFramebufferStatus = GETFN_(proto::CheckNamedFramebufferStatusFn, "glCheckNamedFramebufferStatus")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.clearNamedBufferData = GETFN_(proto::ClearNamedBufferDataFn, "glClearNamedBufferData")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.clearNamedBufferSubData = GETFN_(proto::ClearNamedBufferSubDataFn, "glClearNamedBufferSubData")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.clearNamedFramebufferfi = GETFN_(proto::ClearNamedFramebufferfiFn, "glClearNamedFramebufferfi")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.clearNamedFramebufferfv = GETFN_(proto::ClearNamedFramebufferfvFn, "glClearNamedFramebufferfv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.clearNamedFramebufferiv = GETFN_(proto::ClearNamedFramebufferivFn, "glClearNamedFramebufferiv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.clearNamedFramebufferuiv = GETFN_(proto::ClearNamedFramebufferuivFn, "glClearNamedFramebufferuiv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.compressedTextureSubImage1D = GETFN_(proto::CompressedTextureSubImage1DFn, "glCompressedTextureSubImage1D")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.compressedTextureSubImage2D = GETFN_(proto::CompressedTextureSubImage2DFn, "glCompressedTextureSubImage2D")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.compressedTextureSubImage3D = GETFN_(proto::CompressedTextureSubImage3DFn, "glCompressedTextureSubImage3D")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.copyNamedBufferSubData = GETFN_(proto::CopyNamedBufferSubDataFn, "glCopyNamedBufferSubData")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.copyTextureSubImage1D = GETFN_(proto::CopyTextureSubImage1DFn, "glCopyTextureSubImage1D")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.copyTextureSubImage2D = GETFN_(proto::CopyTextureSubImage2DFn, "glCopyTextureSubImage2D")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.copyTextureSubImage3D = GETFN_(proto::CopyTextureSubImage3DFn, "glCopyTextureSubImage3D")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.createBuffers = GETFN_(proto::CreateBuffersFn, "glCreateBuffers")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.createFramebuffers = GETFN_(proto::CreateFramebuffersFn, "glCreateFramebuffers")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.createProgramPipelines = GETFN_(proto::CreateProgramPipelinesFn, "glCreateProgramPipelines")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.createQueries = GETFN_(proto::CreateQueriesFn, "glCreateQueries")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.createRenderbuffers = GETFN_(proto::CreateRenderbuffersFn, "glCreateRenderbuffers")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.createSamplers = GETFN_(proto::CreateSamplersFn, "glCreateSamplers")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.createTextures = GETFN_(proto::CreateTexturesFn, "glCreateTextures")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.createTransformFeedbacks = GETFN_(proto::CreateTransformFeedbacksFn, "glCreateTransformFeedbacks")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.createVertexArrays = GETFN_(proto::CreateVertexArraysFn, "glCreateVertexArrays")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.disableVertexArrayAttrib = GETFN_(proto::DisableVertexArrayAttribFn, "glDisableVertexArrayAttrib")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.enableVertexArrayAttrib = GETFN_(proto::EnableVertexArrayAttribFn, "glEnableVertexArrayAttrib")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.flushMappedNamedBufferRange = GETFN_(proto::FlushMappedNamedBufferRangeFn, "glFlushMappedNamedBufferRange")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.generateTextureMipmap = GETFN_(proto::GenerateTextureMipmapFn, "glGenerateTextureMipmap")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getCompressedTextureImage = GETFN_(proto::GetCompressedTextureImageFn, "glGetCompressedTextureImage")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getNamedBufferParameteri64v = GETFN_(proto::GetNamedBufferParameteri64vFn, "glGetNamedBufferParameteri64v")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getNamedBufferParameteriv = GETFN_(proto::GetNamedBufferParameterivFn, "glGetNamedBufferParameteriv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getNamedBufferPointerv = GETFN_(proto::GetNamedBufferPointervFn, "glGetNamedBufferPointerv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getNamedBufferSubData = GETFN_(proto::GetNamedBufferSubDataFn, "glGetNamedBufferSubData")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getNamedFramebufferAttachmentParameteriv = GETFN_(proto::GetNamedFramebufferAttachmentParameterivFn, "glGetNamedFramebufferAttachmentParameteriv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getNamedFramebufferParameteriv = GETFN_(proto::GetNamedFramebufferParameterivFn, "glGetNamedFramebufferParameteriv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getNamedRenderbufferParameteriv = GETFN_(proto::GetNamedRenderbufferParameterivFn, "glGetNamedRenderbufferParameteriv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getQueryBufferObjecti64v = GETFN_(proto::GetQueryBufferObjecti64vFn, "glGetQueryBufferObjecti64v")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getQueryBufferObjectiv = GETFN_(proto::GetQueryBufferObjectivFn, "glGetQueryBufferObjectiv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getQueryBufferObjectui64v = GETFN_(proto::GetQueryBufferObjectui64vFn, "glGetQueryBufferObjectui64v")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getQueryBufferObjectuiv = GETFN_(proto::GetQueryBufferObjectuivFn, "glGetQueryBufferObjectuiv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getTextureImage = GETFN_(proto::GetTextureImageFn, "glGetTextureImage")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getTextureLevelParameterfv = GETFN_(proto::GetTextureLevelParameterfvFn, "glGetTextureLevelParameterfv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getTextureLevelParameteriv = GETFN_(proto::GetTextureLevelParameterivFn, "glGetTextureLevelParameteriv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getTextureParameterIiv = GETFN_(proto::GetTextureParameterIivFn, "glGetTextureParameterIiv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getTextureParameterIuiv = GETFN_(proto::GetTextureParameterIuivFn, "glGetTextureParameterIuiv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getTextureParameterfv = GETFN_(proto::GetTextureParameterfvFn, "glGetTextureParameterfv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getTextureParameteriv = GETFN_(proto::GetTextureParameterivFn, "glGetTextureParameteriv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getTransformFeedbacki64_v = GETFN_(proto::GetTransformFeedbacki64_vFn, "glGetTransformFeedbacki64_v")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getTransformFeedbacki_v = GETFN_(proto::GetTransformFeedbacki_vFn, "glGetTransformFeedbacki_v")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getTransformFeedbackiv = GETFN_(proto::GetTransformFeedbackivFn, "glGetTransformFeedbackiv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getVertexArrayIndexed64iv = GETFN_(proto::GetVertexArrayIndexed64ivFn, "glGetVertexArrayIndexed64iv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getVertexArrayIndexediv = GETFN_(proto::GetVertexArrayIndexedivFn, "glGetVertexArrayIndexediv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getVertexArrayiv = GETFN_(proto::GetVertexArrayivFn, "glGetVertexArrayiv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.invalidateNamedFramebufferData = GETFN_(proto::InvalidateNamedFramebufferDataFn, "glInvalidateNamedFramebufferData")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.invalidateNamedFramebufferSubData = GETFN_(proto::InvalidateNamedFramebufferSubDataFn, "glInvalidateNamedFramebufferSubData")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.mapNamedBuffer = GETFN_(proto::MapNamedBufferFn, "glMapNamedBuffer")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.mapNamedBufferRange = GETFN_(proto::MapNamedBufferRangeFn, "glMapNamedBufferRange")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.namedBufferData = GETFN_(proto::NamedBufferDataFn, "glNamedBufferData")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.namedBufferStorage = GETFN_(proto::NamedBufferStorageFn, "glNamedBufferStorage")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.namedBufferSubData = GETFN_(proto::NamedBufferSubDataFn, "glNamedBufferSubData")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.namedFramebufferDrawBuffer = GETFN_(proto::NamedFramebufferDrawBufferFn, "glNamedFramebufferDrawBuffer")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.namedFramebufferDrawBuffers = GETFN_(proto::NamedFramebufferDrawBuffersFn, "glNamedFramebufferDrawBuffers")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.namedFramebufferParameteri = GETFN_(proto::NamedFramebufferParameteriFn, "glNamedFramebufferParameteri")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.namedFramebufferReadBuffer = GETFN_(proto::NamedFramebufferReadBufferFn, "glNamedFramebufferReadBuffer")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.namedFramebufferRenderbuffer = GETFN_(proto::NamedFramebufferRenderbufferFn, "glNamedFramebufferRenderbuffer")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.namedFramebufferTexture = GETFN_(proto::NamedFramebufferTextureFn, "glNamedFramebufferTexture")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.namedFramebufferTextureLayer = GETFN_(proto::NamedFramebufferTextureLayerFn, "glNamedFramebufferTextureLayer")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.namedRenderbufferStorage = GETFN_(proto::NamedRenderbufferStorageFn, "glNamedRenderbufferStorage")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.namedRenderbufferStorageMultisample = GETFN_(proto::NamedRenderbufferStorageMultisampleFn, "glNamedRenderbufferStorageMultisample")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureBuffer = GETFN_(proto::TextureBufferFn, "glTextureBuffer")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureBufferRange = GETFN_(proto::TextureBufferRangeFn, "glTextureBufferRange")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureParameterIiv = GETFN_(proto::TextureParameterIivFn, "glTextureParameterIiv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureParameterIuiv = GETFN_(proto::TextureParameterIuivFn, "glTextureParameterIuiv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureParameterf = GETFN_(proto::TextureParameterfFn, "glTextureParameterf")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureParameterfv = GETFN_(proto::TextureParameterfvFn, "glTextureParameterfv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureParameteri = GETFN_(proto::TextureParameteriFn, "glTextureParameteri")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureParameteriv = GETFN_(proto::TextureParameterivFn, "glTextureParameteriv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureStorage1D = GETFN_(proto::TextureStorage1DFn, "glTextureStorage1D")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureStorage2D = GETFN_(proto::TextureStorage2DFn, "glTextureStorage2D")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureStorage2DMultisample = GETFN_(proto::TextureStorage2DMultisampleFn, "glTextureStorage2DMultisample")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureStorage3D = GETFN_(proto::TextureStorage3DFn, "glTextureStorage3D")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureStorage3DMultisample = GETFN_(proto::TextureStorage3DMultisampleFn, "glTextureStorage3DMultisample")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureSubImage1D = GETFN_(proto::TextureSubImage1DFn, "glTextureSubImage1D")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureSubImage2D = GETFN_(proto::TextureSubImage2DFn, "glTextureSubImage2D")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureSubImage3D = GETFN_(proto::TextureSubImage3DFn, "glTextureSubImage3D")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.transformFeedbackBufferBase = GETFN_(proto::TransformFeedbackBufferBaseFn, "glTransformFeedbackBufferBase")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.transformFeedbackBufferRange = GETFN_(proto::TransformFeedbackBufferRangeFn, "glTransformFeedbackBufferRange")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.unmapNamedBuffer = GETFN_(proto::UnmapNamedBufferFn, "glUnmapNamedBuffer")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.vertexArrayAttribBinding = GETFN_(proto::VertexArrayAttribBindingFn, "glVertexArrayAttribBinding")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.vertexArrayAttribFormat = GETFN_(proto::VertexArrayAttribFormatFn, "glVertexArrayAttribFormat")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.vertexArrayAttribIFormat = GETFN_(proto::VertexArrayAttribIFormatFn, "glVertexArrayAttribIFormat")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.vertexArrayAttribLFormat = GETFN_(proto::VertexArrayAttribLFormatFn, "glVertexArrayAttribLFormat")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.vertexArrayBindingDivisor = GETFN_(proto::VertexArrayBindingDivisorFn, "glVertexArrayBindingDivisor")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.vertexArrayElementBuffer = GETFN_(proto::VertexArrayElementBufferFn, "glVertexArrayElementBuffer")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.vertexArrayVertexBuffer = GETFN_(proto::VertexArrayVertexBufferFn, "glVertexArrayVertexBuffer")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.vertexArrayVertexBuffers = GETFN_(proto::VertexArrayVertexBuffersFn, "glVertexArrayVertexBuffers")) && ok;
		}
		gl.ARB_direct_state_access = ok;
	}
	//- end: ARB_direct_state_access

	//-//-- 
	//- Ext: ARB_draw_buffers
	static char const* ARB_draw_buffers = "ARB_draw_buffers";
	if( listed_( arg.extList, ARB_draw_buffers ) && arg.checkExt( ARB_draw_buffers, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.drawBuffersARB = GETFN_(proto::DrawBuffersARBFn, "glDrawBuffersARB")) && ok;
		gl.ARB_draw_buffers = ok;
	}
	//- end: ARB_draw_buffers

	//-//-- 
	//- Ext: ARB_draw_buffers_blend
	static char const* ARB_draw_buffers_blend = "ARB_draw_buffers_blend";
	if( listed_( arg.extList, ARB_draw_buffers_blend ) && arg.checkExt( ARB_draw_buffers_blend, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.blendEquationSeparateiARB = GETFN_(proto::BlendEquationSeparateiARBFn, "glBlendEquationSeparateiARB")) && ok;
		ok = !!(gl.blendEquationiARB = GETFN_(proto::BlendEquationiARBFn, "glBlendEquationiARB")) && ok;
		ok = !!(gl.blendFuncSeparateiARB = GETFN_(proto::BlendFuncSeparateiARBFn, "glBlendFuncSeparateiARB")) && ok;
		ok = !!(gl.blendFunciARB = GETFN_(proto::BlendFunciARBFn, "glBlendFunciARB")) && ok;
		gl.ARB_draw_buffers_blend = ok;
	}
	//- end: ARB_draw_buffers_blend

	//-//-- 
	//- Ext: ARB_draw_elements_base_vertex
	static char const* ARB_draw_elements_base_vertex = "ARB_draw_elements_base_vertex";
	if( listed_( arg.extList, ARB_draw_elements_base_vertex ) && arg.checkExt( ARB_draw_elements_base_vertex, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.drawElementsBaseVertex = GETFN_(proto::DrawElementsBaseVertexFn, "glDrawElementsBaseVertex")) && ok;
		}
		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.drawElementsInstancedBaseVertex = GETFN_(proto::DrawElementsInstancedBaseVertexFn, "glDrawElementsInstancedBaseVertex")) && ok;
		}
		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.drawRangeElementsBaseVertex = GETFN_(proto::DrawRangeElementsBaseVertexFn, "glDrawRangeElementsBaseVertex")) && ok;
		}
		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.multiDrawElementsBaseVertex = GETFN_(proto::MultiDrawElementsBaseVertexFn, "glMultiDrawElementsBaseVertex")) && ok;
		}
		gl.ARB_draw_elements_base_vertex = ok;
	}
	//- end: ARB_draw_elements_base_vertex

	//-//-- 
	//- Ext: ARB_draw_indirect
	static char const* ARB_draw_indirect = "ARB_draw_indirect";
	if( listed_( arg.extList, ARB_draw_indirect ) && arg.checkExt( ARB_draw_indirect, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.drawArraysIndirect = GETFN_(proto::DrawArraysIndirectFn, "glDrawArraysIndirect")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.drawElementsIndirect = GETFN_(proto::DrawElementsIndirectFn, "glDrawElementsIndirect")) && ok;
		}
		gl.ARB_draw_indirect = ok;
	}
	//- end: ARB_draw_indirect

	//-//-- 
	//- Ext: ARB_draw_instanced
	static char const* ARB_draw_instanced = "ARB_draw_instanced";
	if( listed_( arg.extList, ARB_draw_instanced ) && arg.checkExt( ARB_draw_instanced, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.drawArraysInstancedARB = GETFN_(proto::DrawArraysInstancedARBFn, "glDrawArraysInstancedARB")) && ok;
		ok = !!(gl.drawElementsInstancedARB = GETFN_(proto::DrawElementsInstancedARBFn, "glDrawElementsInstancedARB")) && ok;
		gl.ARB_draw_instanced = ok;
	}
	//- end: ARB_draw_instanced

	//-//-- 
	//- Ext: ARB_fragment_program
	static char const* ARB_fragment_program = "ARB_fragment_program";
	if( listed_( arg.extList, ARB_fragment_program ) && arg.checkExt( ARB_fragment_program, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.bindProgramARB = GETFN_(proto::BindProgramARBFn, "glBindProgramARB")) && ok;
		ok = !!(gl.deleteProgramsARB = GETFN_(proto::DeleteProgramsARBFn, "glDeleteProgramsARB")) && ok;
		ok = !!(gl.genProgramsARB = GETFN_(proto::GenProgramsARBFn, "glGenProgramsARB")) && ok;
		ok = !!(gl.getProgramEnvParameterdvARB = GETFN_(proto::GetProgramEnvParameterdvARBFn, "glGetProgramEnvParameterdvARB")) && ok;
		ok = !!(gl.getProgramEnvParameterfvARB = GETFN_(proto::GetProgramEnvParameterfvARBFn, "glGetProgramEnvParameterfvARB")) && ok;
		ok = !!(gl.getProgramLocalParameterdvARB = GETFN_(proto::GetProgramLocalParameterdvARBFn, "glGetProgramLocalParameterdvARB")) && ok;
		ok = !!(gl.getProgramLocalParameterfvARB = GETFN_(proto::GetProgramLocalParameterfvARBFn, "glGetProgramLocalParameterfvARB")) && ok;
		ok = !!(gl.getProgramStringARB = GETFN_(proto::GetProgramStringARBFn, "glGetProgramStringARB")) && ok;
		ok = !!(gl.getProgramivARB = GETFN_(proto::GetProgramivARBFn, "glGetProgramivARB")) && ok;
		ok = !!(gl.isProgramARB = GETFN_(proto::IsProgramARBFn, "glIsProgramARB")) && ok;
		ok = !!(gl.programEnvParameter4dARB = GETFN_(proto::ProgramEnvParameter4dARBFn, "glProgramEnvParameter4dARB")) && ok;
		ok = !!(gl.programEnvParameter4dvARB = GETFN_(proto::ProgramEnvParameter4dvARBFn, "glProgramEnvParameter4dvARB")) && ok;
		ok = !!(gl.programEnvParameter4fARB = GETFN_(proto::ProgramEnvParameter4fARBFn, "glProgramEnvParameter4fARB")) && ok;
		ok = !!(gl.programEnvParameter4fvARB = GETFN_(proto::ProgramEnvParameter4fvARBFn, "glProgramEnvParameter4fvARB")) && ok;
		ok = !!(gl.programLocalParameter4dARB = GETFN_(proto::ProgramLocalParameter4dARBFn, "glProgramLocalParameter4dARB")) && ok;
		ok = !!(gl.programLocalParameter4dvARB = GETFN_(proto::ProgramLocalParameter4dvARBFn, "glProgramLocalParameter4dvARB")) && ok;
		ok = !!(gl.programLocalParameter4fARB = GETFN_(proto::ProgramLocalParameter4fARBFn, "glProgramLocalParameter4fARB")) && ok;
		ok = !!(gl.programLocalParameter4fvARB = GETFN_(proto::ProgramLocalParameter4fvARBFn, "glProgramLocalParameter4fvARB")) && ok;
		ok = !!(gl.programStringARB = GETFN_(proto::ProgramStringARBFn, "glProgramStringARB")) && ok;
		gl.ARB_fragment_program = ok;
	}
	//- end: ARB_fragment_program

	//-//-- 
	//- Ext: ARB_framebuffer_no_attachments
	static char const* ARB_framebuffer_no_attachments = "ARB_framebuffer_no_attachments";
	if( listed_( arg.extList, ARB_framebuffer_no_attachments ) && arg.checkExt( ARB_framebuffer_no_attachments, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.framebufferParameteri = GETFN_(proto::FramebufferParameteriFn, "glFramebufferParameteri")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.getFramebufferParameteriv = GETFN_(proto::GetFramebufferParameterivFn, "glGetFramebufferParameteriv")) && ok;
		}
		gl.ARB_framebuffer_no_attachments = ok;
	}
	//- end: ARB_framebuffer_no_attachments

	//-//-- 
	//- Ext: ARB_framebuffer_object
	static char const* ARB_framebuffer_object = "ARB_framebuffer_object";
	if( listed_( arg.extList, ARB_framebuffer_object ) && arg.checkExt( ARB_framebuffer_object, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.bindFramebuffer = GETFN_(proto::BindFramebufferFn, "glBindFramebuffer")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.bindRenderbuffer = GETFN_(proto::BindRenderbufferFn, "glBindRenderbuffer")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.blitFramebuffer = GETFN_(proto::BlitFramebufferFn, "glBlitFramebuffer")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.checkFramebufferStatus = GETFN_(proto::CheckFramebufferStatusFn, "glCheckFramebufferStatus")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.deleteFramebuffers = GETFN_(proto::DeleteFramebuffersFn, "glDeleteFramebuffers")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.deleteRenderbuffers = GETFN_(proto::DeleteRenderbuffersFn, "glDeleteRenderbuffers")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.framebufferRenderbuffer = GETFN_(proto::FramebufferRenderbufferFn, "glFramebufferRenderbuffer")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.framebufferTexture1D = GETFN_(proto::FramebufferTexture1DFn, "glFramebufferTexture1D")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.framebufferTexture2D = GETFN_(proto::FramebufferTexture2DFn, "glFramebufferTexture2D")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.framebufferTexture3D = GETFN_(proto::FramebufferTexture3DFn, "glFramebufferTexture3D")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.framebufferTextureLayer = GETFN_(proto::FramebufferTextureLayerFn, "glFramebufferTextureLayer")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.genFramebuffers = GETFN_(proto::GenFramebuffersFn, "glGenFramebuffers")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.genRenderbuffers = GETFN_(proto::GenRenderbuffersFn, "glGenRenderbuffers")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.generateMipmap = GETFN_(proto::GenerateMipmapFn, "glGenerateMipmap")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.getFramebufferAttachmentParameteriv = GETFN_(proto::GetFramebufferAttachmentParameterivFn, "glGetFramebufferAttachmentParameteriv")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.getRenderbufferParameteriv = GETFN_(proto::GetRenderbufferParameterivFn, "glGetRenderbufferParameteriv")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.isFramebuffer = GETFN_(proto::IsFramebufferFn, "glIsFramebuffer")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.isRenderbuffer = GETFN_(proto::IsRenderbufferFn, "glIsRenderbuffer")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.renderbufferStorage = GETFN_(proto::RenderbufferStorageFn, "glRenderbufferStorage")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.renderbufferStorageMultisample = GETFN_(proto::RenderbufferStorageMultisampleFn, "glRenderbufferStorageMultisample")) && ok;
		}
		gl.ARB_framebuffer_object = ok;
	}
	//- end: ARB_framebuffer_object

	//-//-- 
	//- Ext: ARB_geometry_shader4
	static char const* ARB_geometry_shader4 = "ARB_geometry_shader4";
	if( listed_( arg.extList, ARB_geometry_shader4 ) && arg.checkExt( ARB_geometry_shader4, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.framebufferTextureARB = GETFN_(proto::FramebufferTextureARBFn, "glFramebufferTextureARB")) && ok;
		ok = !!(gl.framebufferTextureFaceARB = GETFN_(proto::FramebufferTextureFaceARBFn, "glFramebufferTextureFaceARB")) && ok;
		ok = !!(gl.framebufferTextureLayerARB = GETFN_(proto::FramebufferTextureLayerARBFn, "glFramebufferTextureLayerARB")) && ok;
		ok = !!(gl.programParameteriARB = GETFN_(proto::ProgramParameteriARBFn, "glProgramParameteriARB")) && ok;
		gl.ARB_geometry_shader4 = ok;
	}
	//- end: ARB_geometry_shader4

	//-//-- 
	//- Ext: ARB_get_program_binary
	static char const* ARB_get_program_binary = "ARB_get_program_binary";
	if( listed_( arg.extList, ARB_get_program_binary ) && arg.checkExt( ARB_get_program_binary, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.getProgramBinary = GETFN_(proto::GetProgramBinaryFn, "glGetProgramBinary")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programBinary = GETFN_(proto::ProgramBinaryFn, "glProgramBinary")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programParameteri = GETFN_(proto::ProgramParameteriFn, "glProgramParameteri")) && ok;
		}
		gl.ARB_get_program_binary = ok;
	}
	//- end: ARB_get_program_binary

	//-//-- 
	//- Ext: ARB_get_texture_sub_image
	static char const* ARB_get_texture_sub_image = "ARB_get_texture_sub_image";
	if( listed_( arg.extList, ARB_get_texture_sub_image ) && arg.checkExt( ARB_get_texture_sub_image, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getCompressedTextureSubImage = GETFN_(proto::GetCompressedTextureSubImageFn, "glGetCompressedTextureSubImage")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getTextureSubImage = GETFN_(proto::GetTextureSubImageFn, "glGetTextureSubImage")) && ok;
		}
		gl.ARB_get_texture_sub_image = ok;
	}
	//- end: ARB_get_texture_sub_image

	//-//-- 
	//- Ext: ARB_gpu_shader_fp64
	static char const* ARB_gpu_shader_fp64 = "ARB_gpu_shader_fp64";
	if( listed_( arg.extList, ARB_gpu_shader_fp64 ) && arg.checkExt( ARB_gpu_shader_fp64, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.getUniformdv = GETFN_(proto::GetUniformdvFn, "glGetUniformdv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniform1d = GETFN_(proto::Uniform1dFn, "glUniform1d")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniform1dv = GETFN_(proto::Uniform1dvFn, "glUniform1dv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniform2d = GETFN_(proto::Uniform2dFn, "glUniform2d")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniform2dv = GETFN_(proto::Uniform2dvFn, "glUniform2dv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniform3d = GETFN_(proto::Uniform3dFn, "glUniform3d")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniform3dv = GETFN_(proto::Uniform3dvFn, "glUniform3dv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniform4d = GETFN_(proto::Uniform4dFn, "glUniform4d")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniform4dv = GETFN_(proto::Uniform4dvFn, "glUniform4dv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniformMatrix2dv = GETFN_(proto::UniformMatrix2dvFn, "glUniformMatrix2dv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniformMatrix2x3dv = GETFN_(proto::UniformMatrix2x3dvFn, "glUniformMatrix2x3dv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniformMatrix2x4dv = GETFN_(proto::UniformMatrix2x4dvFn, "glUniformMatrix2x4dv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniformMatrix3dv = GETFN_(proto::UniformMatrix3dvFn, "glUniformMatrix3dv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniformMatrix3x2dv = GETFN_(proto::UniformMatrix3x2dvFn, "glUniformMatrix3x2dv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniformMatrix3x4dv = GETFN_(proto::UniformMatrix3x4dvFn, "glUniformMatrix3x4dv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniformMatrix4dv = GETFN_(proto::UniformMatrix4dvFn, "glUniformMatrix4dv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniformMatrix4x2dv = GETFN_(proto::UniformMatrix4x2dvFn, "glUniformMatrix4x2dv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniformMatrix4x3dv = GETFN_(proto::UniformMatrix4x3dvFn, "glUniformMatrix4x3dv")) && ok;
		}
		gl.ARB_gpu_shader_fp64 = ok;
	}
	//- end: ARB_gpu_shader_fp64

	//-//-- 
	//- Ext: ARB_gpu_shader_int64
	static char const* ARB_gpu_shader_int64 = "ARB_gpu_shader_int64";
	if( listed_( arg.extList, ARB_gpu_shader_int64 ) && arg.checkExt( ARB_gpu_shader_int64, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getUniformi64vARB = GETFN_(proto::GetUniformi64vARBFn, "glGetUniformi64vARB")) && ok;
		ok = !!(gl.getUniformui64vARB = GETFN_(proto::GetUniformui64vARBFn, "glGetUniformui64vARB")) && ok;
		ok = !!(gl.getnUniformi64vARB = GETFN_(proto::GetnUniformi64vARBFn, "glGetnUniformi64vARB")) && ok;
		ok = !!(gl.getnUniformui64vARB = GETFN_(proto::GetnUniformui64vARBFn, "glGetnUniformui64vARB")) && ok;
		ok = !!(gl.programUniform1i64ARB = GETFN_(proto::ProgramUniform1i64ARBFn, "glProgramUniform1i64ARB")) && ok;
		ok = !!(gl.programUniform1i64vARB = GETFN_(proto::ProgramUniform1i64vARBFn, "glProgramUniform1i64vARB")) && ok;
		ok = !!(gl.programUniform1ui64ARB = GETFN_(proto::ProgramUniform1ui64ARBFn, "glProgramUniform1ui64ARB")) && ok;
		ok = !!(gl.programUniform1ui64vARB = GETFN_(proto::ProgramUniform1ui64vARBFn, "glProgramUniform1ui64vARB")) && ok;
		ok = !!(gl.programUniform2i64ARB = GETFN_(proto::ProgramUniform2i64ARBFn, "glProgramUniform2i64ARB")) && ok;
		ok = !!(gl.programUniform2i64vARB = GETFN_(proto::ProgramUniform2i64vARBFn, "glProgramUniform2i64vARB")) && ok;
		ok = !!(gl.programUniform2ui64ARB = GETFN_(proto::ProgramUniform2ui64ARBFn, "glProgramUniform2ui64ARB")) && ok;
		ok = !!(gl.programUniform2ui64vARB = GETFN_(proto::ProgramUniform2ui64vARBFn, "glProgramUniform2ui64vARB")) && ok;
		ok = !!(gl.programUniform3i64ARB = GETFN_(proto::ProgramUniform3i64ARBFn, "glProgramUniform3i64ARB")) && ok;
		ok = !!(gl.programUniform3i64vARB = GETFN_(proto::ProgramUniform3i64vARBFn, "glProgramUniform3i64vARB")) && ok;
		ok = !!(gl.programUniform3ui64ARB = GETFN_(proto::ProgramUniform3ui64ARBFn, "glProgramUniform3ui64ARB")) && ok;
		ok = !!(gl.programUniform3ui64vARB = GETFN_(proto::ProgramUniform3ui64vARBFn, "glProgramUniform3ui64vARB")) && ok;
		ok = !!(gl.programUniform4i64ARB = GETFN_(proto::ProgramUniform4i64ARBFn, "glProgramUniform4i64ARB")) && ok;
		ok = !!(gl.programUniform4i64vARB = GETFN_(proto::ProgramUniform4i64vARBFn, "glProgramUniform4i64vARB")) && ok;
		ok = !!(gl.programUniform4ui64ARB = GETFN_(proto::ProgramUniform4ui64ARBFn, "glProgramUniform4ui64ARB")) && ok;
		ok = !!(gl.programUniform4ui64vARB = GETFN_(proto::ProgramUniform4ui64vARBFn, "glProgramUniform4ui64vARB")) && ok;
		ok = !!(gl.uniform1i64ARB = GETFN_(proto::Uniform1i64ARBFn, "glUniform1i64ARB")) && ok;
		ok = !!(gl.uniform1i64vARB = GETFN_(proto::Uniform1i64vARBFn, "glUniform1i64vARB")) && ok;
		ok = !!(gl.uniform1ui64ARB = GETFN_(proto::Uniform1ui64ARBFn, "glUniform1ui64ARB")) && ok;
		ok = !!(gl.uniform1ui64vARB = GETFN_(proto::Uniform1ui64vARBFn, "glUniform1ui64vARB")) && ok;
		ok = !!(gl.uniform2i64ARB = GETFN_(proto::Uniform2i64ARBFn, "glUniform2i64ARB")) && ok;
		ok = !!(gl.uniform2i64vARB = GETFN_(proto::Uniform2i64vARBFn, "glUniform2i64vARB")) && ok;
		ok = !!(gl.uniform2ui64ARB = GETFN_(proto::Uniform2ui64ARBFn, "glUniform2ui64ARB")) && ok;
		ok = !!(gl.uniform2ui64vARB = GETFN_(proto::Uniform2ui64vARBFn, "glUniform2ui64vARB")) && ok;
		ok = !!(gl.uniform3i64ARB = GETFN_(proto::Uniform3i64ARBFn, "glUniform3i64ARB")) && ok;
		ok = !!(gl.uniform3i64vARB = GETFN_(proto::Uniform3i64vARBFn, "glUniform3i64vARB")) && ok;
		ok = !!(gl.uniform3ui64ARB = GETFN_(proto::Uniform3ui64ARBFn, "glUniform3ui64ARB")) && ok;
		ok = !!(gl.uniform3ui64vARB = GETFN_(proto::Uniform3ui64vARBFn, "glUniform3ui64vARB")) && ok;
		ok = !!(gl.uniform4i64ARB = GETFN_(proto::Uniform4i64ARBFn, "glUniform4i64ARB")) && ok;
		ok = !!(gl.uniform4i64vARB = GETFN_(proto::Uniform4i64vARBFn, "glUniform4i64vARB")) && ok;
		ok = !!(gl.uniform4ui64ARB = GETFN_(proto::Uniform4ui64ARBFn, "glUniform4ui64ARB")) && ok;
		ok = !!(gl.uniform4ui64vARB = GETFN_(proto::Uniform4ui64vARBFn, "glUniform4ui64vARB")) && ok;
		gl.ARB_gpu_shader_int64 = ok;
	}
	//- end: ARB_gpu_shader_int64

	//-//-- 
	//- Ext: ARB_imaging
	static char const* ARB_imaging = "ARB_imaging";
	if( listed_( arg.extList, ARB_imaging ) && arg.checkExt( ARB_imaging, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.blendColor = GETFN_(proto::BlendColorFn, "glBlendColor")) && ok;
		}
		if( !gl.VERSION_1_4 ) {
			ok = !!(gl.blendEquation = GETFN_(proto::BlendEquationFn, "glBlendEquation")) && ok;
		}
		ok = !!(gl.colorSubTable = GETFN_(proto::ColorSubTableFn, "glColorSubTable")) && ok;
		ok = !!(gl.colorTable = GETFN_(proto::ColorTableFn, "glColorTable")) && ok;
		ok = !!(gl.colorTableParameterfv = GETFN_(proto::ColorTableParameterfvFn, "glColorTableParameterfv")) && ok;
		ok = !!(gl.colorTableParameteriv = GETFN_(proto::ColorTableParameterivFn, "glColorTableParameteriv")) && ok;
		ok = !!(gl.convolutionFilter1D = GETFN_(proto::ConvolutionFilter1DFn, "glConvolutionFilter1D")) && ok;
		ok = !!(gl.convolutionFilter2D = GETFN_(proto::ConvolutionFilter2DFn, "glConvolutionFilter2D")) && ok;
		ok = !!(gl.convolutionParameterf = GETFN_(proto::ConvolutionParameterfFn, "glConvolutionParameterf")) && ok;
		ok = !!(gl.convolutionParameterfv = GETFN_(proto::ConvolutionParameterfvFn, "glConvolutionParameterfv")) && ok;
		ok = !!(gl.convolutionParameteri = GETFN_(proto::ConvolutionParameteriFn, "glConvolutionParameteri")) && ok;
		ok = !!(gl.convolutionParameteriv = GETFN_(proto::ConvolutionParameterivFn, "glConvolutionParameteriv")) && ok;
		ok = !!(gl.copyColorSubTable = GETFN_(proto::CopyColorSubTableFn, "glCopyColorSubTable")) && ok;
		ok = !!(gl.copyColorTable = GETFN_(proto::CopyColorTableFn, "glCopyColorTable")) && ok;
		ok = !!(gl.copyConvolutionFilter1D = GETFN_(proto::CopyConvolutionFilter1DFn, "glCopyConvolutionFilter1D")) && ok;
		ok = !!(gl.copyConvolutionFilter2D = GETFN_(proto::CopyConvolutionFilter2DFn, "glCopyConvolutionFilter2D")) && ok;
		ok = !!(gl.getColorTable = GETFN_(proto::GetColorTableFn, "glGetColorTable")) && ok;
		ok = !!(gl.getColorTableParameterfv = GETFN_(proto::GetColorTableParameterfvFn, "glGetColorTableParameterfv")) && ok;
		ok = !!(gl.getColorTableParameteriv = GETFN_(proto::GetColorTableParameterivFn, "glGetColorTableParameteriv")) && ok;
		ok = !!(gl.getConvolutionFilter = GETFN_(proto::GetConvolutionFilterFn, "glGetConvolutionFilter")) && ok;
		ok = !!(gl.getConvolutionParameterfv = GETFN_(proto::GetConvolutionParameterfvFn, "glGetConvolutionParameterfv")) && ok;
		ok = !!(gl.getConvolutionParameteriv = GETFN_(proto::GetConvolutionParameterivFn, "glGetConvolutionParameteriv")) && ok;
		ok = !!(gl.getHistogram = GETFN_(proto::GetHistogramFn, "glGetHistogram")) && ok;
		ok = !!(gl.getHistogramParameterfv = GETFN_(proto::GetHistogramParameterfvFn, "glGetHistogramParameterfv")) && ok;
		ok = !!(gl.getHistogramParameteriv = GETFN_(proto::GetHistogramParameterivFn, "glGetHistogramParameteriv")) && ok;
		ok = !!(gl.getMinmax = GETFN_(proto::GetMinmaxFn, "glGetMinmax")) && ok;
		ok = !!(gl.getMinmaxParameterfv = GETFN_(proto::GetMinmaxParameterfvFn, "glGetMinmaxParameterfv")) && ok;
		ok = !!(gl.getMinmaxParameteriv = GETFN_(proto::GetMinmaxParameterivFn, "glGetMinmaxParameteriv")) && ok;
		ok = !!(gl.getSeparableFilter = GETFN_(proto::GetSeparableFilterFn, "glGetSeparableFilter")) && ok;
		ok = !!(gl.histogram = GETFN_(proto::HistogramFn, "glHistogram")) && ok;
		ok = !!(gl.minmax = GETFN_(proto::MinmaxFn, "glMinmax")) && ok;
		ok = !!(gl.resetHistogram = GETFN_(proto::ResetHistogramFn, "glResetHistogram")) && ok;
		ok = !!(gl.resetMinmax = GETFN_(proto::ResetMinmaxFn, "glResetMinmax")) && ok;
		ok = !!(gl.separableFilter2D = GETFN_(proto::SeparableFilter2DFn, "glSeparableFilter2D")) && ok;
		gl.ARB_imaging = ok;
	}
	//- end: ARB_imaging

	//-//-- 
	//- Ext: ARB_indirect_parameters
	static char const* ARB_indirect_parameters = "ARB_indirect_parameters";
	if( listed_( arg.extList, ARB_indirect_parameters ) && arg.checkExt( ARB_indirect_parameters, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.multiDrawArraysIndirectCountARB = GETFN_(proto::MultiDrawArraysIndirectCountARBFn, "glMultiDrawArraysIndirectCountARB")) && ok;
		ok = !!(gl.multiDrawElementsIndirectCountARB = GETFN_(proto::MultiDrawElementsIndirectCountARBFn, "glMultiDrawElementsIndirectCountARB")) && ok;
		gl.ARB_indirect_parameters = ok;
	}
	//- end: ARB_indirect_parameters

	//-//-- 
	//- Ext: ARB_instanced_arrays
	static char const* ARB_instanced_arrays = "ARB_instanced_arrays";
	if( listed_( arg.extList, ARB_instanced_arrays ) && arg.checkExt( ARB_instanced_arrays, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.vertexAttribDivisorARB = GETFN_(proto::VertexAttribDivisorARBFn, "glVertexAttribDivisorARB")) && ok;
		gl.ARB_instanced_arrays = ok;
	}
	//- end: ARB_instanced_arrays

	//-//-- 
	//- Ext: ARB_internalformat_query
	static char const* ARB_internalformat_query = "ARB_internalformat_query";
	if( listed_( arg.extList, ARB_internalformat_query ) && arg.checkExt( ARB_internalformat_query, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_2 ) {
			ok = !!(gl.getInternalformativ = GETFN_(proto::GetInternalformativFn, "glGetInternalformativ")) && ok;
		}
		gl.ARB_internalformat_query = ok;
	}
	//- end: ARB_internalformat_query

	//-//-- 
	//- Ext: ARB_internalformat_query2
	static char const* ARB_internalformat_query2 = "ARB_internalformat_query2";
	if( listed_( arg.extList, ARB_internalformat_query2 ) && arg.checkExt( ARB_internalformat_query2, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.getInternalformati64v = GETFN_(proto::GetInternalformati64vFn, "glGetInternalformati64v")) && ok;
		}
		gl.ARB_internalformat_query2 = ok;
	}
	//- end: ARB_internalformat_query2

	//-//-- 
	//- Ext: ARB_invalidate_subdata
	static char const* ARB_invalidate_subdata = "ARB_invalidate_subdata";
	if( listed_( arg.extList, ARB_invalidate_subdata ) && arg.checkExt( ARB_invalidate_subdata, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.invalidateBufferData = GETFN_(proto::InvalidateBufferDataFn, "glInvalidateBufferData")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.invalidateBufferSubData = GETFN_(proto::InvalidateBufferSubDataFn, "glInvalidateBufferSubData")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.invalidateFramebuffer = GETFN_(proto::InvalidateFramebufferFn, "glInvalidateFramebuffer")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.invalidateSubFramebuffer = GETFN_(proto::InvalidateSubFramebufferFn, "glInvalidateSubFramebuffer")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.invalidateTexImage = GETFN_(proto::InvalidateTexImageFn, "glInvalidateTexImage")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.invalidateTexSubImage = GETFN_(proto::InvalidateTexSubImageFn, "glInvalidateTexSubImage")) && ok;
		}
		gl.ARB_invalidate_subdata = ok;
	}
	//- end: ARB_invalidate_subdata

	//-//-- 
	//- Ext: ARB_map_buffer_range
	static char const* ARB_map_buffer_range = "ARB_map_buffer_range";
	if( listed_( arg.extList, ARB_map_buffer_range ) && arg.checkExt( ARB_map_buffer_range, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.flushMappedBufferRange = GETFN_(proto::FlushMappedBufferRangeFn, "glFlushMappedBufferRange")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.mapBufferRange = GETFN_(proto::MapBufferRangeFn, "glMapBufferRange")) && ok;
		}
		gl.ARB_map_buffer_range = ok;
	}
	//- end: ARB_map_buffer_range

	//-//-- 
	//- Ext: ARB_matrix_palette
	static char const* ARB_matrix_palette = "ARB_matrix_palette";
	if( listed_( arg.extList, ARB_matrix_palette ) && arg.checkExt( ARB_matrix_palette, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.currentPaletteMatrixARB = GETFN_(proto::CurrentPaletteMatrixARBFn, "glCurrentPaletteMatrixARB")) && ok;
		ok = !!(gl.matrixIndexPointerARB = GETFN_(proto::MatrixIndexPointerARBFn, "glMatrixIndexPointerARB")) && ok;
		ok = !!(gl.matrixIndexubvARB = GETFN_(proto::MatrixIndexubvARBFn, "glMatrixIndexubvARB")) && ok;
		ok = !!(gl.matrixIndexuivARB = GETFN_(proto::MatrixIndexuivARBFn, "glMatrixIndexuivARB")) && ok;
		ok = !!(gl.matrixIndexusvARB = GETFN_(proto::MatrixIndexusvARBFn, "glMatrixIndexusvARB")) && ok;
		gl.ARB_matrix_palette = ok;
	}
	//- end: ARB_matrix_palette

	//-//-- 
	//- Ext: ARB_multi_bind
	static char const* ARB_multi_bind = "ARB_multi_bind";
	if( listed_( arg.extList, ARB_multi_bind ) && arg.checkExt( ARB_multi_bind, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_4 ) {
			ok = !!(gl.bindBuffersBase = GETFN_(proto::BindBuffersBaseFn, "glBindBuffersBase")) && ok;
		}
		if( !gl.VERSION_4_4 ) {
			ok = !!(gl.bindBuffersRange = GETFN_(proto::BindBuffersRangeFn, "glBindBuffersRange")) && ok;
		}
		if( !gl.VERSION_4_4 ) {
			ok = !!(gl.bindImageTextures = GETFN_(proto::BindImageTexturesFn, "glBindImageTextures")) && ok;
		}
		if( !gl.VERSION_4_4 ) {
			ok = !!(gl.bindSamplers = GETFN_(proto::BindSamplersFn, "glBindSamplers")) && ok;
		}
		if( !gl.VERSION_4_4 ) {
			ok = !!(gl.bindTextures = GETFN_(proto::BindTexturesFn, "glBindTextures")) && ok;
		}
		if( !gl.VERSION_4_4 ) {
			ok = !!(gl.bindVertexBuffers = GETFN_(proto::BindVertexBuffersFn, "glBindVertexBuffers")) && ok;
		}
		gl.ARB_multi_bind = ok;
	}
	//- end: ARB_multi_bind

	//-//-- 
	//- Ext: ARB_multi_draw_indirect
	static char const* ARB_multi_draw_indirect = "ARB_multi_draw_indirect";
	if( listed_( arg.extList, ARB_multi_draw_indirect ) && arg.checkExt( ARB_multi_draw_indirect, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.multiDrawArraysIndirect = GETFN_(proto::MultiDrawArraysIndirectFn, "glMultiDrawArraysIndirect")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.multiDrawElementsIndirect = GETFN_(proto::MultiDrawElementsIndirectFn, "glMultiDrawElementsIndirect")) && ok;
		}
		gl.ARB_multi_draw_indirect = ok;
	}
	//- end: ARB_multi_draw_indirect

	//-//-- 
	//- Ext: ARB_multisample
	static char const* ARB_multisample = "ARB_multisample";
	if( listed_( arg.extList, ARB_multisample ) && arg.checkExt( ARB_multisample, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.sampleCoverageARB = GETFN_(proto::SampleCoverageARBFn, "glSampleCoverageARB")) && ok;
		gl.ARB_multisample = ok;
	}
	//- end: ARB_multisample

	//-//-- 
	//- Ext: ARB_multitexture
	static char const* ARB_multitexture = "ARB_multitexture";
	if( listed_( arg.extList, ARB_multitexture ) && arg.checkExt( ARB_multitexture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.activeTextureARB = GETFN_(proto::ActiveTextureARBFn, "glActiveTextureARB")) && ok;
		ok = !!(gl.clientActiveTextureARB = GETFN_(proto::ClientActiveTextureARBFn, "glClientActiveTextureARB")) && ok;
		ok = !!(gl.multiTexCoord1dARB = GETFN_(proto::MultiTexCoord1dARBFn, "glMultiTexCoord1dARB")) && ok;
		ok = !!(gl.multiTexCoord1dvARB = GETFN_(proto::MultiTexCoord1dvARBFn, "glMultiTexCoord1dvARB")) && ok;
		ok = !!(gl.multiTexCoord1fARB = GETFN_(proto::MultiTexCoord1fARBFn, "glMultiTexCoord1fARB")) && ok;
		ok = !!(gl.multiTexCoord1fvARB = GETFN_(proto::MultiTexCoord1fvARBFn, "glMultiTexCoord1fvARB")) && ok;
		ok = !!(gl.multiTexCoord1iARB = GETFN_(proto::MultiTexCoord1iARBFn, "glMultiTexCoord1iARB")) && ok;
		ok = !!(gl.multiTexCoord1ivARB = GETFN_(proto::MultiTexCoord1ivARBFn, "glMultiTexCoord1ivARB")) && ok;
		ok = !!(gl.multiTexCoord1sARB = GETFN_(proto::MultiTexCoord1sARBFn, "glMultiTexCoord1sARB")) && ok;
		ok = !!(gl.multiTexCoord1svARB = GETFN_(proto::MultiTexCoord1svARBFn, "glMultiTexCoord1svARB")) && ok;
		ok = !!(gl.multiTexCoord2dARB = GETFN_(proto::MultiTexCoord2dARBFn, "glMultiTexCoord2dARB")) && ok;
		ok = !!(gl.multiTexCoord2dvARB = GETFN_(proto::MultiTexCoord2dvARBFn, "glMultiTexCoord2dvARB")) && ok;
		ok = !!(gl.multiTexCoord2fARB = GETFN_(proto::MultiTexCoord2fARBFn, "glMultiTexCoord2fARB")) && ok;
		ok = !!(gl.multiTexCoord2fvARB = GETFN_(proto::MultiTexCoord2fvARBFn, "glMultiTexCoord2fvARB")) && ok;
		ok = !!(gl.multiTexCoord2iARB = GETFN_(proto::MultiTexCoord2iARBFn, "glMultiTexCoord2iARB")) && ok;
		ok = !!(gl.multiTexCoord2ivARB = GETFN_(proto::MultiTexCoord2ivARBFn, "glMultiTexCoord2ivARB")) && ok;
		ok = !!(gl.multiTexCoord2sARB = GETFN_(proto::MultiTexCoord2sARBFn, "glMultiTexCoord2sARB")) && ok;
		ok = !!(gl.multiTexCoord2svARB = GETFN_(proto::MultiTexCoord2svARBFn, "glMultiTexCoord2svARB")) && ok;
		ok = !!(gl.multiTexCoord3dARB = GETFN_(proto::MultiTexCoord3dARBFn, "glMultiTexCoord3dARB")) && ok;
		ok = !!(gl.multiTexCoord3dvARB = GETFN_(proto::MultiTexCoord3dvARBFn, "glMultiTexCoord3dvARB")) && ok;
		ok = !!(gl.multiTexCoord3fARB = GETFN_(proto::MultiTexCoord3fARBFn, "glMultiTexCoord3fARB")) && ok;
		ok = !!(gl.multiTexCoord3fvARB = GETFN_(proto::MultiTexCoord3fvARBFn, "glMultiTexCoord3fvARB")) && ok;
		ok = !!(gl.multiTexCoord3iARB = GETFN_(proto::MultiTexCoord3iARBFn, "glMultiTexCoord3iARB")) && ok;
		ok = !!(gl.multiTexCoord3ivARB = GETFN_(proto::MultiTexCoord3ivARBFn, "glMultiTexCoord3ivARB")) && ok;
		ok = !!(gl.multiTexCoord3sARB = GETFN_(proto::MultiTexCoord3sARBFn, "glMultiTexCoord3sARB")) && ok;
		ok = !!(gl.multiTexCoord3svARB = GETFN_(proto::MultiTexCoord3svARBFn, "glMultiTexCoord3svARB")) && ok;
		ok = !!(gl.multiTexCoord4dARB = GETFN_(proto::MultiTexCoord4dARBFn, "glMultiTexCoord4dARB")) && ok;
		ok = !!(gl.multiTexCoord4dvARB = GETFN_(proto::MultiTexCoord4dvARBFn, "glMultiTexCoord4dvARB")) && ok;
		ok = !!(gl.multiTexCoord4fARB = GETFN_(proto::MultiTexCoord4fARBFn, "glMultiTexCoord4fARB")) && ok;
		ok = !!(gl.multiTexCoord4fvARB = GETFN_(proto::MultiTexCoord4fvARBFn, "glMultiTexCoord4fvARB")) && ok;
		ok = !!(gl.multiTexCoord4iARB = GETFN_(proto::MultiTexCoord4iARBFn, "glMultiTexCoord4iARB")) && ok;
		ok = !!(gl.multiTexCoord4ivARB = GETFN_(proto::MultiTexCoord4ivARBFn, "glMultiTexCoord4ivARB")) && ok;
		ok = !!(gl.multiTexCoord4sARB = GETFN_(proto::MultiTexCoord4sARBFn, "glMultiTexCoord4sARB")) && ok;
		ok = !!(gl.multiTexCoord4svARB = GETFN_(proto::MultiTexCoord4svARBFn, "glMultiTexCoord4svARB")) && ok;
		gl.ARB_multitexture = ok;
	}
	//- end: ARB_multitexture

	//-//-- 
	//- Ext: ARB_occlusion_query
	static char const* ARB_occlusion_query = "ARB_occlusion_query";
	if( listed_( arg.extList, ARB_occlusion_query ) && arg.checkExt( ARB_occlusion_query, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.beginQueryARB = GETFN_(proto::BeginQueryARBFn, "glBeginQueryARB")) && ok;
		ok = !!(gl.deleteQueriesARB = GETFN_(proto::DeleteQueriesARBFn, "glDeleteQueriesARB")) && ok;
		ok = !!(gl.endQueryARB = GETFN_(proto::EndQueryARBFn, "glEndQueryARB")) && ok;
		ok = !!(gl.genQueriesARB = GETFN_(proto::GenQueriesARBFn, "glGenQueriesARB")) && ok;
		ok = !!(gl.getQueryObjectivARB = GETFN_(proto::GetQueryObjectivARBFn, "glGetQueryObjectivARB")) && ok;
		ok = !!(gl.getQueryObjectuivARB = GETFN_(proto::GetQueryObjectuivARBFn, "glGetQueryObjectuivARB")) && ok;
		ok = !!(gl.getQueryivARB = GETFN_(proto::GetQueryivARBFn, "glGetQueryivARB")) && ok;
		ok = !!(gl.isQueryARB = GETFN_(proto::IsQueryARBFn, "glIsQueryARB")) && ok;
		gl.ARB_occlusion_query = ok;
	}
	//- end: ARB_occlusion_query

	//-//-- 
	//- Ext: ARB_parallel_shader_compile
	static char const* ARB_parallel_shader_compile = "ARB_parallel_shader_compile";
	if( listed_( arg.extList, ARB_parallel_shader_compile ) && arg.checkExt( ARB_parallel_shader_compile, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.maxShaderCompilerThreadsARB = GETFN_(proto::MaxShaderCompilerThreadsARBFn, "glMaxShaderCompilerThreadsARB")) && ok;
		gl.ARB_parallel_shader_compile = ok;
	}
	//- end: ARB_parallel_shader_compile

	//-//-- 
	//- Ext: ARB_point_parameters
	static char const* ARB_point_parameters = "ARB_point_parameters";
	if( listed_( arg.extList, ARB_point_parameters ) && arg.checkExt( ARB_point_parameters, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.pointParameterfARB = GETFN_(proto::PointParameterfARBFn, "glPointParameterfARB")) && ok;
		ok = !!(gl.pointParameterfvARB = GETFN_(proto::PointParameterfvARBFn, "glPointParameterfvARB")) && ok;
		gl.ARB_point_parameters = ok;
	}
	//- end: ARB_point_parameters

	//-//-- 
	//- Ext: ARB_program_interface_query
	static char const* ARB_program_interface_query = "ARB_program_interface_query";
	if( listed_( arg.extList, ARB_program_interface_query ) && arg.checkExt( ARB_program_interface_query, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.getProgramInterfaceiv = GETFN_(proto::GetProgramInterfaceivFn, "glGetProgramInterfaceiv")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.getProgramResourceIndex = GETFN_(proto::GetProgramResourceIndexFn, "glGetProgramResourceIndex")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.getProgramResourceLocation = GETFN_(proto::GetProgramResourceLocationFn, "glGetProgramResourceLocation")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.getProgramResourceLocationIndex = GETFN_(proto::GetProgramResourceLocationIndexFn, "glGetProgramResourceLocationIndex")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.getProgramResourceName = GETFN_(proto::GetProgramResourceNameFn, "glGetProgramResourceName")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.getProgramResourceiv = GETFN_(proto::GetProgramResourceivFn, "glGetProgramResourceiv")) && ok;
		}
		gl.ARB_program_interface_query = ok;
	}
	//- end: ARB_program_interface_query

	//-//-- 
	//- Ext: ARB_provoking_vertex
	static char const* ARB_provoking_vertex = "ARB_provoking_vertex";
	if( listed_( arg.extList, ARB_provoking_vertex ) && arg.checkExt( ARB_provoking_vertex, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.provokingVertex = GETFN_(proto::ProvokingVertexFn, "glProvokingVertex")) && ok;
		}
		gl.ARB_provoking_vertex = ok;
	}
	//- end: ARB_provoking_vertex

	//-//-- 
	//- Ext: ARB_robustness
	static char const* ARB_robustness = "ARB_robustness";
	if( listed_( arg.extList, ARB_robustness ) && arg.checkExt( ARB_robustness, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getGraphicsResetStatusARB = GETFN_(proto::GetGraphicsResetStatusARBFn, "glGetGraphicsResetStatusARB")) && ok;
		ok = !!(gl.getnColorTableARB = GETFN_(proto::GetnColorTableARBFn, "glGetnColorTableARB")) && ok;
		ok = !!(gl.getnCompressedTexImageARB = GETFN_(proto::GetnCompressedTexImageARBFn, "glGetnCompressedTexImageARB")) && ok;
		ok = !!(gl.getnConvolutionFilterARB = GETFN_(proto::GetnConvolutionFilterARBFn, "glGetnConvolutionFilterARB")) && ok;
		ok = !!(gl.getnHistogramARB = GETFN_(proto::GetnHistogramARBFn, "glGetnHistogramARB")) && ok;
		ok = !!(gl.getnMapdvARB = GETFN_(proto::GetnMapdvARBFn, "glGetnMapdvARB")) && ok;
		ok = !!(gl.getnMapfvARB = GETFN_(proto::GetnMapfvARBFn, "glGetnMapfvARB")) && ok;
		ok = !!(gl.getnMapivARB = GETFN_(proto::GetnMapivARBFn, "glGetnMapivARB")) && ok;
		ok = !!(gl.getnMinmaxARB = GETFN_(proto::GetnMinmaxARBFn, "glGetnMinmaxARB")) && ok;
		ok = !!(gl.getnPixelMapfvARB = GETFN_(proto::GetnPixelMapfvARBFn, "glGetnPixelMapfvARB")) && ok;
		ok = !!(gl.getnPixelMapuivARB = GETFN_(proto::GetnPixelMapuivARBFn, "glGetnPixelMapuivARB")) && ok;
		ok = !!(gl.getnPixelMapusvARB = GETFN_(proto::GetnPixelMapusvARBFn, "glGetnPixelMapusvARB")) && ok;
		ok = !!(gl.getnPolygonStippleARB = GETFN_(proto::GetnPolygonStippleARBFn, "glGetnPolygonStippleARB")) && ok;
		ok = !!(gl.getnSeparableFilterARB = GETFN_(proto::GetnSeparableFilterARBFn, "glGetnSeparableFilterARB")) && ok;
		ok = !!(gl.getnTexImageARB = GETFN_(proto::GetnTexImageARBFn, "glGetnTexImageARB")) && ok;
		ok = !!(gl.getnUniformdvARB = GETFN_(proto::GetnUniformdvARBFn, "glGetnUniformdvARB")) && ok;
		ok = !!(gl.getnUniformfvARB = GETFN_(proto::GetnUniformfvARBFn, "glGetnUniformfvARB")) && ok;
		ok = !!(gl.getnUniformivARB = GETFN_(proto::GetnUniformivARBFn, "glGetnUniformivARB")) && ok;
		ok = !!(gl.getnUniformuivARB = GETFN_(proto::GetnUniformuivARBFn, "glGetnUniformuivARB")) && ok;
		ok = !!(gl.readnPixelsARB = GETFN_(proto::ReadnPixelsARBFn, "glReadnPixelsARB")) && ok;
		gl.ARB_robustness = ok;
	}
	//- end: ARB_robustness

	//-//-- 
	//- Ext: ARB_sample_locations
	static char const* ARB_sample_locations = "ARB_sample_locations";
	if( listed_( arg.extList, ARB_sample_locations ) && arg.checkExt( ARB_sample_locations, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.evaluateDepthValuesARB = GETFN_(proto::EvaluateDepthValuesARBFn, "glEvaluateDepthValuesARB")) && ok;
		ok = !!(gl.framebufferSampleLocationsfvARB = GETFN_(proto::FramebufferSampleLocationsfvARBFn, "glFramebufferSampleLocationsfvARB")) && ok;
		ok = !!(gl.namedFramebufferSampleLocationsfvARB = GETFN_(proto::NamedFramebufferSampleLocationsfvARBFn, "glNamedFramebufferSampleLocationsfvARB")) && ok;
		gl.ARB_sample_locations = ok;
	}
	//- end: ARB_sample_locations

	//-//-- 
	//- Ext: ARB_sample_shading
	static char const* ARB_sample_shading = "ARB_sample_shading";
	if( listed_( arg.extList, ARB_sample_shading ) && arg.checkExt( ARB_sample_shading, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.minSampleShadingARB = GETFN_(proto::MinSampleShadingARBFn, "glMinSampleShadingARB")) && ok;
		gl.ARB_sample_shading = ok;
	}
	//- end: ARB_sample_shading

	//-//-- 
	//- Ext: ARB_sampler_objects
	static char const* ARB_sampler_objects = "ARB_sampler_objects";
	if( listed_( arg.extList, ARB_sampler_objects ) && arg.checkExt( ARB_sampler_objects, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.bindSampler = GETFN_(proto::BindSamplerFn, "glBindSampler")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.deleteSamplers = GETFN_(proto::DeleteSamplersFn, "glDeleteSamplers")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.genSamplers = GETFN_(proto::GenSamplersFn, "glGenSamplers")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.getSamplerParameterIiv = GETFN_(proto::GetSamplerParameterIivFn, "glGetSamplerParameterIiv")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.getSamplerParameterIuiv = GETFN_(proto::GetSamplerParameterIuivFn, "glGetSamplerParameterIuiv")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.getSamplerParameterfv = GETFN_(proto::GetSamplerParameterfvFn, "glGetSamplerParameterfv")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.getSamplerParameteriv = GETFN_(proto::GetSamplerParameterivFn, "glGetSamplerParameteriv")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.isSampler = GETFN_(proto::IsSamplerFn, "glIsSampler")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.samplerParameterIiv = GETFN_(proto::SamplerParameterIivFn, "glSamplerParameterIiv")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.samplerParameterIuiv = GETFN_(proto::SamplerParameterIuivFn, "glSamplerParameterIuiv")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.samplerParameterf = GETFN_(proto::SamplerParameterfFn, "glSamplerParameterf")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.samplerParameterfv = GETFN_(proto::SamplerParameterfvFn, "glSamplerParameterfv")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.samplerParameteri = GETFN_(proto::SamplerParameteriFn, "glSamplerParameteri")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.samplerParameteriv = GETFN_(proto::SamplerParameterivFn, "glSamplerParameteriv")) && ok;
		}
		gl.ARB_sampler_objects = ok;
	}
	//- end: ARB_sampler_objects

	//-//-- 
	//- Ext: ARB_separate_shader_objects
	static char const* ARB_separate_shader_objects = "ARB_separate_shader_objects";
	if( listed_( arg.extList, ARB_separate_shader_objects ) && arg.checkExt( ARB_separate_shader_objects, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.activeShaderProgram = GETFN_(proto::ActiveShaderProgramFn, "glActiveShaderProgram")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.bindProgramPipeline = GETFN_(proto::BindProgramPipelineFn, "glBindProgramPipeline")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.createShaderProgramv = GETFN_(proto::CreateShaderProgramvFn, "glCreateShaderProgramv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.deleteProgramPipelines = GETFN_(proto::DeleteProgramPipelinesFn, "glDeleteProgramPipelines")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.genProgramPipelines = GETFN_(proto::GenProgramPipelinesFn, "glGenProgramPipelines")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.getProgramPipelineInfoLog = GETFN_(proto::GetProgramPipelineInfoLogFn, "glGetProgramPipelineInfoLog")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.getProgramPipelineiv = GETFN_(proto::GetProgramPipelineivFn, "glGetProgramPipelineiv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.isProgramPipeline = GETFN_(proto::IsProgramPipelineFn, "glIsProgramPipeline")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform1d = GETFN_(proto::ProgramUniform1dFn, "glProgramUniform1d")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform1dv = GETFN_(proto::ProgramUniform1dvFn, "glProgramUniform1dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform1f = GETFN_(proto::ProgramUniform1fFn, "glProgramUniform1f")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform1fv = GETFN_(proto::ProgramUniform1fvFn, "glProgramUniform1fv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform1i = GETFN_(proto::ProgramUniform1iFn, "glProgramUniform1i")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform1iv = GETFN_(proto::ProgramUniform1ivFn, "glProgramUniform1iv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform1ui = GETFN_(proto::ProgramUniform1uiFn, "glProgramUniform1ui")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform1uiv = GETFN_(proto::ProgramUniform1uivFn, "glProgramUniform1uiv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform2d = GETFN_(proto::ProgramUniform2dFn, "glProgramUniform2d")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform2dv = GETFN_(proto::ProgramUniform2dvFn, "glProgramUniform2dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform2f = GETFN_(proto::ProgramUniform2fFn, "glProgramUniform2f")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform2fv = GETFN_(proto::ProgramUniform2fvFn, "glProgramUniform2fv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform2i = GETFN_(proto::ProgramUniform2iFn, "glProgramUniform2i")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform2iv = GETFN_(proto::ProgramUniform2ivFn, "glProgramUniform2iv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform2ui = GETFN_(proto::ProgramUniform2uiFn, "glProgramUniform2ui")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform2uiv = GETFN_(proto::ProgramUniform2uivFn, "glProgramUniform2uiv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform3d = GETFN_(proto::ProgramUniform3dFn, "glProgramUniform3d")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform3dv = GETFN_(proto::ProgramUniform3dvFn, "glProgramUniform3dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform3f = GETFN_(proto::ProgramUniform3fFn, "glProgramUniform3f")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform3fv = GETFN_(proto::ProgramUniform3fvFn, "glProgramUniform3fv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform3i = GETFN_(proto::ProgramUniform3iFn, "glProgramUniform3i")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform3iv = GETFN_(proto::ProgramUniform3ivFn, "glProgramUniform3iv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform3ui = GETFN_(proto::ProgramUniform3uiFn, "glProgramUniform3ui")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform3uiv = GETFN_(proto::ProgramUniform3uivFn, "glProgramUniform3uiv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform4d = GETFN_(proto::ProgramUniform4dFn, "glProgramUniform4d")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform4dv = GETFN_(proto::ProgramUniform4dvFn, "glProgramUniform4dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform4f = GETFN_(proto::ProgramUniform4fFn, "glProgramUniform4f")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform4fv = GETFN_(proto::ProgramUniform4fvFn, "glProgramUniform4fv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform4i = GETFN_(proto::ProgramUniform4iFn, "glProgramUniform4i")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform4iv = GETFN_(proto::ProgramUniform4ivFn, "glProgramUniform4iv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform4ui = GETFN_(proto::ProgramUniform4uiFn, "glProgramUniform4ui")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniform4uiv = GETFN_(proto::ProgramUniform4uivFn, "glProgramUniform4uiv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix2dv = GETFN_(proto::ProgramUniformMatrix2dvFn, "glProgramUniformMatrix2dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix2fv = GETFN_(proto::ProgramUniformMatrix2fvFn, "glProgramUniformMatrix2fv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix2x3dv = GETFN_(proto::ProgramUniformMatrix2x3dvFn, "glProgramUniformMatrix2x3dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix2x3fv = GETFN_(proto::ProgramUniformMatrix2x3fvFn, "glProgramUniformMatrix2x3fv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix2x4dv = GETFN_(proto::ProgramUniformMatrix2x4dvFn, "glProgramUniformMatrix2x4dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix2x4fv = GETFN_(proto::ProgramUniformMatrix2x4fvFn, "glProgramUniformMatrix2x4fv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix3dv = GETFN_(proto::ProgramUniformMatrix3dvFn, "glProgramUniformMatrix3dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix3fv = GETFN_(proto::ProgramUniformMatrix3fvFn, "glProgramUniformMatrix3fv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix3x2dv = GETFN_(proto::ProgramUniformMatrix3x2dvFn, "glProgramUniformMatrix3x2dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix3x2fv = GETFN_(proto::ProgramUniformMatrix3x2fvFn, "glProgramUniformMatrix3x2fv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix3x4dv = GETFN_(proto::ProgramUniformMatrix3x4dvFn, "glProgramUniformMatrix3x4dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix3x4fv = GETFN_(proto::ProgramUniformMatrix3x4fvFn, "glProgramUniformMatrix3x4fv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix4dv = GETFN_(proto::ProgramUniformMatrix4dvFn, "glProgramUniformMatrix4dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix4fv = GETFN_(proto::ProgramUniformMatrix4fvFn, "glProgramUniformMatrix4fv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix4x2dv = GETFN_(proto::ProgramUniformMatrix4x2dvFn, "glProgramUniformMatrix4x2dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix4x2fv = GETFN_(proto::ProgramUniformMatrix4x2fvFn, "glProgramUniformMatrix4x2fv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix4x3dv = GETFN_(proto::ProgramUniformMatrix4x3dvFn, "glProgramUniformMatrix4x3dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.programUniformMatrix4x3fv = GETFN_(proto::ProgramUniformMatrix4x3fvFn, "glProgramUniformMatrix4x3fv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.useProgramStages = GETFN_(proto::UseProgramStagesFn, "glUseProgramStages")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.validateProgramPipeline = GETFN_(proto::ValidateProgramPipelineFn, "glValidateProgramPipeline")) && ok;
		}
		gl.ARB_separate_shader_objects = ok;
	}
	//- end: ARB_separate_shader_objects

	//-//-- 
	//- Ext: ARB_shader_atomic_counters
	static char const* ARB_shader_atomic_counters = "ARB_shader_atomic_counters";
	if( listed_( arg.extList, ARB_shader_atomic_counters ) && arg.checkExt( ARB_shader_atomic_counters, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_2 ) {
			ok = !!(gl.getActiveAtomicCounterBufferiv = GETFN_(proto::GetActiveAtomicCounterBufferivFn, "glGetActiveAtomicCounterBufferiv")) && ok;
		}
		gl.ARB_shader_atomic_counters = ok;
	}
	//- end: ARB_shader_atomic_counters

	//-//-- 
	//- Ext: ARB_shader_image_load_store
	static char const* ARB_shader_image_load_store = "ARB_shader_image_load_store";
	if( listed_( arg.extList, ARB_shader_image_load_store ) && arg.checkExt( ARB_shader_image_load_store, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_2 ) {
			ok = !!(gl.bindImageTexture = GETFN_(proto::BindImageTextureFn, "glBindImageTexture")) && ok;
		}
		if( !gl.VERSION_4_2 ) {
			ok = !!(gl.memoryBarrier = GETFN_(proto::MemoryBarrierFn, "glMemoryBarrier")) && ok;
		}
		gl.ARB_shader_image_load_store = ok;
	}
	//- end: ARB_shader_image_load_store

	//-//-- 
	//- Ext: ARB_shader_objects
	static char const* ARB_shader_objects = "ARB_shader_objects";
	if( listed_( arg.extList, ARB_shader_objects ) && arg.checkExt( ARB_shader_objects, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.attachObjectARB = GETFN_(proto::AttachObjectARBFn, "glAttachObjectARB")) && ok;
		ok = !!(gl.compileShaderARB = GETFN_(proto::CompileShaderARBFn, "glCompileShaderARB")) && ok;
		ok = !!(gl.createProgramObjectARB = GETFN_(proto::CreateProgramObjectARBFn, "glCreateProgramObjectARB")) && ok;
		ok = !!(gl.createShaderObjectARB = GETFN_(proto::CreateShaderObjectARBFn, "glCreateShaderObjectARB")) && ok;
		ok = !!(gl.deleteObjectARB = GETFN_(proto::DeleteObjectARBFn, "glDeleteObjectARB")) && ok;
		ok = !!(gl.detachObjectARB = GETFN_(proto::DetachObjectARBFn, "glDetachObjectARB")) && ok;
		ok = !!(gl.getActiveUniformARB = GETFN_(proto::GetActiveUniformARBFn, "glGetActiveUniformARB")) && ok;
		ok = !!(gl.getAttachedObjectsARB = GETFN_(proto::GetAttachedObjectsARBFn, "glGetAttachedObjectsARB")) && ok;
		ok = !!(gl.getHandleARB = GETFN_(proto::GetHandleARBFn, "glGetHandleARB")) && ok;
		ok = !!(gl.getInfoLogARB = GETFN_(proto::GetInfoLogARBFn, "glGetInfoLogARB")) && ok;
		ok = !!(gl.getObjectParameterfvARB = GETFN_(proto::GetObjectParameterfvARBFn, "glGetObjectParameterfvARB")) && ok;
		ok = !!(gl.getObjectParameterivARB = GETFN_(proto::GetObjectParameterivARBFn, "glGetObjectParameterivARB")) && ok;
		ok = !!(gl.getShaderSourceARB = GETFN_(proto::GetShaderSourceARBFn, "glGetShaderSourceARB")) && ok;
		ok = !!(gl.getUniformLocationARB = GETFN_(proto::GetUniformLocationARBFn, "glGetUniformLocationARB")) && ok;
		ok = !!(gl.getUniformfvARB = GETFN_(proto::GetUniformfvARBFn, "glGetUniformfvARB")) && ok;
		ok = !!(gl.getUniformivARB = GETFN_(proto::GetUniformivARBFn, "glGetUniformivARB")) && ok;
		ok = !!(gl.linkProgramARB = GETFN_(proto::LinkProgramARBFn, "glLinkProgramARB")) && ok;
		ok = !!(gl.shaderSourceARB = GETFN_(proto::ShaderSourceARBFn, "glShaderSourceARB")) && ok;
		ok = !!(gl.uniform1fARB = GETFN_(proto::Uniform1fARBFn, "glUniform1fARB")) && ok;
		ok = !!(gl.uniform1fvARB = GETFN_(proto::Uniform1fvARBFn, "glUniform1fvARB")) && ok;
		ok = !!(gl.uniform1iARB = GETFN_(proto::Uniform1iARBFn, "glUniform1iARB")) && ok;
		ok = !!(gl.uniform1ivARB = GETFN_(proto::Uniform1ivARBFn, "glUniform1ivARB")) && ok;
		ok = !!(gl.uniform2fARB = GETFN_(proto::Uniform2fARBFn, "glUniform2fARB")) && ok;
		ok = !!(gl.uniform2fvARB = GETFN_(proto::Uniform2fvARBFn, "glUniform2fvARB")) && ok;
		ok = !!(gl.uniform2iARB = GETFN_(proto::Uniform2iARBFn, "glUniform2iARB")) && ok;
		ok = !!(gl.uniform2ivARB = GETFN_(proto::Uniform2ivARBFn, "glUniform2ivARB")) && ok;
		ok = !!(gl.uniform3fARB = GETFN_(proto::Uniform3fARBFn, "glUniform3fARB")) && ok;
		ok = !!(gl.uniform3fvARB = GETFN_(proto::Uniform3fvARBFn, "glUniform3fvARB")) && ok;
		ok = !!(gl.uniform3iARB = GETFN_(proto::Uniform3iARBFn, "glUniform3iARB")) && ok;
		ok = !!(gl.uniform3ivARB = GETFN_(proto::Uniform3ivARBFn, "glUniform3ivARB")) && ok;
		ok = !!(gl.uniform4fARB = GETFN_(proto::Uniform4fARBFn, "glUniform4fARB")) && ok;
		ok = !!(gl.uniform4fvARB = GETFN_(proto::Uniform4fvARBFn, "glUniform4fvARB")) && ok;
		ok = !!(gl.uniform4iARB = GETFN_(proto::Uniform4iARBFn, "glUniform4iARB")) && ok;
		ok = !!(gl.uniform4ivARB = GETFN_(proto::Uniform4ivARBFn, "glUniform4ivARB")) && ok;
		ok = !!(gl.uniformMatrix2fvARB = GETFN_(proto::UniformMatrix2fvARBFn, "glUniformMatrix2fvARB")) && ok;
		ok = !!(gl.uniformMatrix3fvARB = GETFN_(proto::UniformMatrix3fvARBFn, "glUniformMatrix3fvARB")) && ok;
		ok = !!(gl.uniformMatrix4fvARB = GETFN_(proto::UniformMatrix4fvARBFn, "glUniformMatrix4fvARB")) && ok;
		ok = !!(gl.useProgramObjectARB = GETFN_(proto::UseProgramObjectARBFn, "glUseProgramObjectARB")) && ok;
		ok = !!(gl.validateProgramARB = GETFN_(proto::ValidateProgramARBFn, "glValidateProgramARB")) && ok;
		gl.ARB_shader_objects = ok;
	}
	//- end: ARB_shader_objects

	//-//-- 
	//- Ext: ARB_shader_storage_buffer_object
	static char const* ARB_shader_storage_buffer_object = "ARB_shader_storage_buffer_object";
	if( listed_( arg.extList, ARB_shader_storage_buffer_object ) && arg.checkExt( ARB_shader_storage_buffer_object, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.shaderStorageBlockBinding = GETFN_(proto::ShaderStorageBlockBindingFn, "glShaderStorageBlockBinding")) && ok;
		}
		gl.ARB_shader_storage_buffer_object = ok;
	}
	//- end: ARB_shader_storage_buffer_object

	//-//-- 
	//- Ext: ARB_shader_subroutine
	static char const* ARB_shader_subroutine = "ARB_shader_subroutine";
	if( listed_( arg.extList, ARB_shader_subroutine ) && arg.checkExt( ARB_shader_subroutine, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.getActiveSubroutineName = GETFN_(proto::GetActiveSubroutineNameFn, "glGetActiveSubroutineName")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.getActiveSubroutineUniformName = GETFN_(proto::GetActiveSubroutineUniformNameFn, "glGetActiveSubroutineUniformName")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.getActiveSubroutineUniformiv = GETFN_(proto::GetActiveSubroutineUniformivFn, "glGetActiveSubroutineUniformiv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.getProgramStageiv = GETFN_(proto::GetProgramStageivFn, "glGetProgramStageiv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.getSubroutineIndex = GETFN_(proto::GetSubroutineIndexFn, "glGetSubroutineIndex")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.getSubroutineUniformLocation = GETFN_(proto::GetSubroutineUniformLocationFn, "glGetSubroutineUniformLocation")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.getUniformSubroutineuiv = GETFN_(proto::GetUniformSubroutineuivFn, "glGetUniformSubroutineuiv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.uniformSubroutinesuiv = GETFN_(proto::UniformSubroutinesuivFn, "glUniformSubroutinesuiv")) && ok;
		}
		gl.ARB_shader_subroutine = ok;
	}
	//- end: ARB_shader_subroutine

	//-//-- 
	//- Ext: ARB_shading_language_include
	static char const* ARB_shading_language_include = "ARB_shading_language_include";
	if( listed_( arg.extList, ARB_shading_language_include ) && arg.checkExt( ARB_shading_language_include, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.compileShaderIncludeARB = GETFN_(proto::CompileShaderIncludeARBFn, "glCompileShaderIncludeARB")) && ok;
		ok = !!(gl.deleteNamedStringARB = GETFN_(proto::DeleteNamedStringARBFn, "glDeleteNamedStringARB")) && ok;
		ok = !!(gl.getNamedStringARB = GETFN_(proto::GetNamedStringARBFn, "glGetNamedStringARB")) && ok;
		ok = !!(gl.getNamedStringivARB = GETFN_(proto::GetNamedStringivARBFn, "glGetNamedStringivARB")) && ok;
		ok = !!(gl.isNamedStringARB = GETFN_(proto::IsNamedStringARBFn, "glIsNamedStringARB")) && ok;
		ok = !!(gl.namedStringARB = GETFN_(proto::NamedStringARBFn, "glNamedStringARB")) && ok;
		gl.ARB_shading_language_include = ok;
	}
	//- end: ARB_shading_language_include

	//-//-- 
	//- Ext: ARB_sparse_buffer
	static char const* ARB_sparse_buffer = "ARB_sparse_buffer";
	if( listed_( arg.extList, ARB_sparse_buffer ) && arg.checkExt( ARB_sparse_buffer, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.bufferPageCommitmentARB = GETFN_(proto::BufferPageCommitmentARBFn, "glBufferPageCommitmentARB")) && ok;
		ok = !!(gl.namedBufferPageCommitmentARB = GETFN_(proto::NamedBufferPageCommitmentARBFn, "glNamedBufferPageCommitmentARB")) && ok;
		ok = !!(gl.namedBufferPageCommitmentEXT = GETFN_(proto::NamedBufferPageCommitmentEXTFn, "glNamedBufferPageCommitmentEXT")) && ok;
		gl.ARB_sparse_buffer = ok;
	}
	//- end: ARB_sparse_buffer

	//-//-- 
	//- Ext: ARB_sparse_texture
	static char const* ARB_sparse_texture = "ARB_sparse_texture";
	if( listed_( arg.extList, ARB_sparse_texture ) && arg.checkExt( ARB_sparse_texture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.texPageCommitmentARB = GETFN_(proto::TexPageCommitmentARBFn, "glTexPageCommitmentARB")) && ok;
		gl.ARB_sparse_texture = ok;
	}
	//- end: ARB_sparse_texture

	//-//-- 
	//- Ext: ARB_sync
	static char const* ARB_sync = "ARB_sync";
	if( listed_( arg.extList, ARB_sync ) && arg.checkExt( ARB_sync, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.clientWaitSync = GETFN_(proto::ClientWaitSyncFn, "glClientWaitSync")) && ok;
		}
		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.deleteSync = GETFN_(proto::DeleteSyncFn, "glDeleteSync")) && ok;
		}
		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.fenceSync = GETFN_(proto::FenceSyncFn, "glFenceSync")) && ok;
		}
		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.getInteger64v = GETFN_(proto::GetInteger64vFn, "glGetInteger64v")) && ok;
		}
		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.getSynciv = GETFN_(proto::GetSyncivFn, "glGetSynciv")) && ok;
		}
		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.isSync = GETFN_(proto::IsSyncFn, "glIsSync")) && ok;
		}
		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.waitSync = GETFN_(proto::WaitSyncFn, "glWaitSync")) && ok;
		}
		gl.ARB_sync = ok;
	}
	//- end: ARB_sync

	//-//-- 
	//- Ext: ARB_tessellation_shader
	static char const* ARB_tessellation_shader = "ARB_tessellation_shader";
	if( listed_( arg.extList, ARB_tessellation_shader ) && arg.checkExt( ARB_tessellation_shader, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.patchParameterfv = GETFN_(proto::PatchParameterfvFn, "glPatchParameterfv")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.patchParameteri = GETFN_(proto::PatchParameteriFn, "glPatchParameteri")) && ok;
		}
		gl.ARB_tessellation_shader = ok;
	}
	//- end: ARB_tessellation_shader

	//-//-- 
	//- Ext: ARB_texture_barrier
	static char const* ARB_texture_barrier = "ARB_texture_barrier";
	if( listed_( arg.extList, ARB_texture_barrier ) && arg.checkExt( ARB_texture_barrier, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.textureBarrier = GETFN_(proto::TextureBarrierFn, "glTextureBarrier")) && ok;
		}
		gl.ARB_texture_barrier = ok;
	}
	//- end: ARB_texture_barrier

	//-//-- 
	//- Ext: ARB_texture_buffer_object
	static char const* ARB_texture_buffer_object = "ARB_texture_buffer_object";
	if( listed_( arg.extList, ARB_texture_buffer_object ) && arg.checkExt( ARB_texture_buffer_object, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.texBufferARB = GETFN_(proto::TexBufferARBFn, "glTexBufferARB")) && ok;
		gl.ARB_texture_buffer_object = ok;
	}
	//- end: ARB_texture_buffer_object

	//-//-- 
	//- Ext: ARB_texture_buffer_range
	static char const* ARB_texture_buffer_range = "ARB_texture_buffer_range";
	if( listed_( arg.extList, ARB_texture_buffer_range ) && arg.checkExt( ARB_texture_buffer_range, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.texBufferRange = GETFN_(proto::TexBufferRangeFn, "glTexBufferRange")) && ok;
		}
		gl.ARB_texture_buffer_range = ok;
	}
	//- end: ARB_texture_buffer_range

	//-//-- 
	//- Ext: ARB_texture_compression
	static char const* ARB_texture_compression = "ARB_texture_compression";
	if( listed_( arg.extList, ARB_texture_compression ) && arg.checkExt( ARB_texture_compression, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.compressedTexImage1DARB = GETFN_(proto::CompressedTexImage1DARBFn, "glCompressedTexImage1DARB")) && ok;
		ok = !!(gl.compressedTexImage2DARB = GETFN_(proto::CompressedTexImage2DARBFn, "glCompressedTexImage2DARB")) && ok;
		ok = !!(gl.compressedTexImage3DARB = GETFN_(proto::CompressedTexImage3DARBFn, "glCompressedTexImage3DARB")) && ok;
		ok = !!(gl.compressedTexSubImage1DARB = GETFN_(proto::CompressedTexSubImage1DARBFn, "glCompressedTexSubImage1DARB")) && ok;
		ok = !!(gl.compressedTexSubImage2DARB = GETFN_(proto::CompressedTexSubImage2DARBFn, "glCompressedTexSubImage2DARB")) && ok;
		ok = !!(gl.compressedTexSubImage3DARB = GETFN_(proto::CompressedTexSubImage3DARBFn, "glCompressedTexSubImage3DARB")) && ok;
		ok = !!(gl.getCompressedTexImageARB = GETFN_(proto::GetCompressedTexImageARBFn, "glGetCompressedTexImageARB")) && ok;
		gl.ARB_texture_compression = ok;
	}
	//- end: ARB_texture_compression

	//-//-- 
	//- Ext: ARB_texture_multisample
	static char const* ARB_texture_multisample = "ARB_texture_multisample";
	if( listed_( arg.extList, ARB_texture_multisample ) && arg.checkExt( ARB_texture_multisample, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.getMultisamplefv = GETFN_(proto::GetMultisamplefvFn, "glGetMultisamplefv")) && ok;
		}
		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.sampleMaski = GETFN_(proto::SampleMaskiFn, "glSampleMaski")) && ok;
		}
		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.texImage2DMultisample = GETFN_(proto::TexImage2DMultisampleFn, "glTexImage2DMultisample")) && ok;
		}
		if( !gl.VERSION_3_2 ) {
			ok = !!(gl.texImage3DMultisample = GETFN_(proto::TexImage3DMultisampleFn, "glTexImage3DMultisample")) && ok;
		}
		gl.ARB_texture_multisample = ok;
	}
	//- end: ARB_texture_multisample

	//-//-- 
	//- Ext: ARB_texture_storage
	static char const* ARB_texture_storage = "ARB_texture_storage";
	if( listed_( arg.extList, ARB_texture_storage ) && arg.checkExt( ARB_texture_storage, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_2 ) {
			ok = !!(gl.texStorage1D = GETFN_(proto::TexStorage1DFn, "glTexStorage1D")) && ok;
		}
		if( !gl.VERSION_4_2 ) {
			ok = !!(gl.texStorage2D = GETFN_(proto::TexStorage2DFn, "glTexStorage2D")) && ok;
		}
		if( !gl.VERSION_4_2 ) {
			ok = !!(gl.texStorage3D = GETFN_(proto::TexStorage3DFn, "glTexStorage3D")) && ok;
		}
		gl.ARB_texture_storage = ok;
	}
	//- end: ARB_texture_storage

	//-//-- 
	//- Ext: ARB_texture_storage_multisample
	static char const* ARB_texture_storage_multisample = "ARB_texture_storage_multisample";
	if( listed_( arg.extList, ARB_texture_storage_multisample ) && arg.checkExt( ARB_texture_storage_multisample, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.texStorage2DMultisample = GETFN_(proto::TexStorage2DMultisampleFn, "glTexStorage2DMultisample")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.texStorage3DMultisample = GETFN_(proto::TexStorage3DMultisampleFn, "glTexStorage3DMultisample")) && ok;
		}
		gl.ARB_texture_storage_multisample = ok;
	}
	//- end: ARB_texture_storage_multisample

	//-//-- 
	//- Ext: ARB_texture_view
	static char const* ARB_texture_view = "ARB_texture_view";
	if( listed_( arg.extList, ARB_texture_view ) && arg.checkExt( ARB_texture_view, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.textureView = GETFN_(proto::TextureViewFn, "glTextureView")) && ok;
		}
		gl.ARB_texture_view = ok;
	}
	//- end: ARB_texture_view

	//-//-- 
	//- Ext: ARB_timer_query
	static char const* ARB_timer_query = "ARB_timer_query";
	if( listed_( arg.extList, ARB_timer_query ) && arg.checkExt( ARB_timer_query, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.getQueryObjecti64v = GETFN_(proto::GetQueryObjecti64vFn, "glGetQueryObjecti64v")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.getQueryObjectui64v = GETFN_(proto::GetQueryObjectui64vFn, "glGetQueryObjectui64v")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.queryCounter = GETFN_(proto::QueryCounterFn, "glQueryCounter")) && ok;
		}
		gl.ARB_timer_query = ok;
	}
	//- end: ARB_timer_query

	//-//-- 
	//- Ext: ARB_transform_feedback2
	static char const* ARB_transform_feedback2 = "ARB_transform_feedback2";
	if( listed_( arg.extList, ARB_transform_feedback2 ) && arg.checkExt( ARB_transform_feedback2, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.bindTransformFeedback = GETFN_(proto::BindTransformFeedbackFn, "glBindTransformFeedback")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.deleteTransformFeedbacks = GETFN_(proto::DeleteTransformFeedbacksFn, "glDeleteTransformFeedbacks")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.drawTransformFeedback = GETFN_(proto::DrawTransformFeedbackFn, "glDrawTransformFeedback")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.genTransformFeedbacks = GETFN_(proto::GenTransformFeedbacksFn, "glGenTransformFeedbacks")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.isTransformFeedback = GETFN_(proto::IsTransformFeedbackFn, "glIsTransformFeedback")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.pauseTransformFeedback = GETFN_(proto::PauseTransformFeedbackFn, "glPauseTransformFeedback")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.resumeTransformFeedback = GETFN_(proto::ResumeTransformFeedbackFn, "glResumeTransformFeedback")) && ok;
		}
		gl.ARB_transform_feedback2 = ok;
	}
	//- end: ARB_transform_feedback2

	//-//-- 
	//- Ext: ARB_transform_feedback3
	static char const* ARB_transform_feedback3 = "ARB_transform_feedback3";
	if( listed_( arg.extList, ARB_transform_feedback3 ) && arg.checkExt( ARB_transform_feedback3, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.beginQueryIndexed = GETFN_(proto::BeginQueryIndexedFn, "glBeginQueryIndexed")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.drawTransformFeedbackStream = GETFN_(proto::DrawTransformFeedbackStreamFn, "glDrawTransformFeedbackStream")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.endQueryIndexed = GETFN_(proto::EndQueryIndexedFn, "glEndQueryIndexed")) && ok;
		}
		if( !gl.VERSION_4_0 ) {
			ok = !!(gl.getQueryIndexediv = GETFN_(proto::GetQueryIndexedivFn, "glGetQueryIndexediv")) && ok;
		}
		gl.ARB_transform_feedback3 = ok;
	}
	//- end: ARB_transform_feedback3

	//-//-- 
	//- Ext: ARB_transform_feedback_instanced
	static char const* ARB_transform_feedback_instanced = "ARB_transform_feedback_instanced";
	if( listed_( arg.extList, ARB_transform_feedback_instanced ) && arg.checkExt( ARB_transform_feedback_instanced, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_2 ) {
			ok = !!(gl.drawTransformFeedbackInstanced = GETFN_(proto::DrawTransformFeedbackInstancedFn, "glDrawTransformFeedbackInstanced")) && ok;
		}
		if( !gl.VERSION_4_2 ) {
			ok = !!(gl.drawTransformFeedbackStreamInstanced = GETFN_(proto::DrawTransformFeedbackStreamInstancedFn, "glDrawTransformFeedbackStreamInstanced")) && ok;
		}
		gl.ARB_transform_feedback_instanced = ok;
	}
	//- end: ARB_transform_feedback_instanced

	//-//-- 
	//- Ext: ARB_transpose_matrix
	static char const* ARB_transpose_matrix = "ARB_transpose_matrix";
	if( listed_( arg.extList, ARB_transpose_matrix ) && arg.checkExt( ARB_transpose_matrix, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.loadTransposeMatrixdARB = GETFN_(proto::LoadTransposeMatrixdARBFn, "glLoadTransposeMatrixdARB")) && ok;
		ok = !!(gl.loadTransposeMatrixfARB = GETFN_(proto::LoadTransposeMatrixfARBFn, "glLoadTransposeMatrixfARB")) && ok;
		ok = !!(gl.multTransposeMatrixdARB = GETFN_(proto::MultTransposeMatrixdARBFn, "glMultTransposeMatrixdARB")) && ok;
		ok = !!(gl.multTransposeMatrixfARB = GETFN_(proto::MultTransposeMatrixfARBFn, "glMultTransposeMatrixfARB")) && ok;
		gl.ARB_transpose_matrix = ok;
	}
	//- end: ARB_transpose_matrix

	//-//-- 
	//- Ext: ARB_uniform_buffer_object
	static char const* ARB_uniform_buffer_object = "ARB_uniform_buffer_object";
	if( listed_( arg.extList, ARB_uniform_buffer_object ) && arg.checkExt( ARB_uniform_buffer_object, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.bindBufferBase = GETFN_(proto::BindBufferBaseFn, "glBindBufferBase")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.bindBufferRange = GETFN_(proto::BindBufferRangeFn, "glBindBufferRange")) && ok;
		}
		if( !gl.VERSION_3_1 ) {
			ok = !!(gl.getActiveUniformBlockName = GETFN_(proto::GetActiveUniformBlockNameFn, "glGetActiveUniformBlockName")) && ok;
		}
		if( !gl.VERSION_3_1 ) {
			ok = !!(gl.getActiveUniformBlockiv = GETFN_(proto::GetActiveUniformBlockivFn, "glGetActiveUniformBlockiv")) && ok;
		}
		if( !gl.VERSION_3_1 ) {
			ok = !!(gl.getActiveUniformName = GETFN_(proto::GetActiveUniformNameFn, "glGetActiveUniformName")) && ok;
		}
		if( !gl.VERSION_3_1 ) {
			ok = !!(gl.getActiveUniformsiv = GETFN_(proto::GetActiveUniformsivFn, "glGetActiveUniformsiv")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.getIntegeri_v = GETFN_(proto::GetIntegeri_vFn, "glGetIntegeri_v")) && ok;
		}
		if( !gl.VERSION_3_1 ) {
			ok = !!(gl.getUniformBlockIndex = GETFN_(proto::GetUniformBlockIndexFn, "glGetUniformBlockIndex")) && ok;
		}
		if( !gl.VERSION_3_1 ) {
			ok = !!(gl.getUniformIndices = GETFN_(proto::GetUniformIndicesFn, "glGetUniformIndices")) && ok;
		}
		if( !gl.VERSION_3_1 ) {
			ok = !!(gl.uniformBlockBinding = GETFN_(proto::UniformBlockBindingFn, "glUniformBlockBinding")) && ok;
		}
		gl.ARB_uniform_buffer_object = ok;
	}
	//- end: ARB_uniform_buffer_object

	//-//-- 
	//- Ext: ARB_vertex_array_object
	static char const* ARB_vertex_array_object = "ARB_vertex_array_object";
	if( listed_( arg.extList, ARB_vertex_array_object ) && arg.checkExt( ARB_vertex_array_object, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.bindVertexArray = GETFN_(proto::BindVertexArrayFn, "glBindVertexArray")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.deleteVertexArrays = GETFN_(proto::DeleteVertexArraysFn, "glDeleteVertexArrays")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.genVertexArrays = GETFN_(proto::GenVertexArraysFn, "glGenVertexArrays")) && ok;
		}
		if( !gl.VERSION_3_0 ) {
			ok = !!(gl.isVertexArray = GETFN_(proto::IsVertexArrayFn, "glIsVertexArray")) && ok;
		}
		gl.ARB_vertex_array_object = ok;
	}
	//- end: ARB_vertex_array_object

	//-//-- 
	//- Ext: ARB_vertex_attrib_64bit
	static char const* ARB_vertex_attrib_64bit = "ARB_vertex_attrib_64bit";
	if( listed_( arg.extList, ARB_vertex_attrib_64bit ) && arg.checkExt( ARB_vertex_attrib_64bit, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.getVertexAttribLdv = GETFN_(proto::GetVertexAttribLdvFn, "glGetVertexAttribLdv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.vertexAttribL1d = GETFN_(proto::VertexAttribL1dFn, "glVertexAttribL1d")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.vertexAttribL1dv = GETFN_(proto::VertexAttribL1dvFn, "glVertexAttribL1dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.vertexAttribL2d = GETFN_(proto::VertexAttribL2dFn, "glVertexAttribL2d")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.vertexAttribL2dv = GETFN_(proto::VertexAttribL2dvFn, "glVertexAttribL2dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.vertexAttribL3d = GETFN_(proto::VertexAttribL3dFn, "glVertexAttribL3d")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.vertexAttribL3dv = GETFN_(proto::VertexAttribL3dvFn, "glVertexAttribL3dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.vertexAttribL4d = GETFN_(proto::VertexAttribL4dFn, "glVertexAttribL4d")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.vertexAttribL4dv = GETFN_(proto::VertexAttribL4dvFn, "glVertexAttribL4dv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.vertexAttribLPointer = GETFN_(proto::VertexAttribLPointerFn, "glVertexAttribLPointer")) && ok;
		}
		gl.ARB_vertex_attrib_64bit = ok;
	}
	//- end: ARB_vertex_attrib_64bit

	//-//-- 
	//- Ext: ARB_vertex_attrib_binding
	static char const* ARB_vertex_attrib_binding = "ARB_vertex_attrib_binding";
	if( listed_( arg.extList, ARB_vertex_attrib_binding ) && arg.checkExt( ARB_vertex_attrib_binding, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.bindVertexBuffer = GETFN_(proto::BindVertexBufferFn, "glBindVertexBuffer")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.vertexAttribBinding = GETFN_(proto::VertexAttribBindingFn, "glVertexAttribBinding")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.vertexAttribFormat = GETFN_(proto::VertexAttribFormatFn, "glVertexAttribFormat")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.vertexAttribIFormat = GETFN_(proto::VertexAttribIFormatFn, "glVertexAttribIFormat")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.vertexAttribLFormat = GETFN_(proto::VertexAttribLFormatFn, "glVertexAttribLFormat")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.vertexBindingDivisor = GETFN_(proto::VertexBindingDivisorFn, "glVertexBindingDivisor")) && ok;
		}
		gl.ARB_vertex_attrib_binding = ok;
	}
	//- end: ARB_vertex_attrib_binding

	//-//-- 
	//- Ext: ARB_vertex_blend
	static char const* ARB_vertex_blend = "ARB_vertex_blend";
	if( listed_( arg.extList, ARB_vertex_blend ) && arg.checkExt( ARB_vertex_blend, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.vertexBlendARB = GETFN_(proto::VertexBlendARBFn, "glVertexBlendARB")) && ok;
		ok = !!(gl.weightPointerARB = GETFN_(proto::WeightPointerARBFn, "glWeightPointerARB")) && ok;
		ok = !!(gl.weightbvARB = GETFN_(proto::WeightbvARBFn, "glWeightbvARB")) && ok;
		ok = !!(gl.weightdvARB = GETFN_(proto::WeightdvARBFn, "glWeightdvARB")) && ok;
		ok = !!(gl.weightfvARB = GETFN_(proto::WeightfvARBFn, "glWeightfvARB")) && ok;
		ok = !!(gl.weightivARB = GETFN_(proto::WeightivARBFn, "glWeightivARB")) && ok;
		ok = !!(gl.weightsvARB = GETFN_(proto::WeightsvARBFn, "glWeightsvARB")) && ok;
		ok = !!(gl.weightubvARB = GETFN_(proto::WeightubvARBFn, "glWeightubvARB")) && ok;
		ok = !!(gl.weightuivARB = GETFN_(proto::WeightuivARBFn, "glWeightuivARB")) && ok;
		ok = !!(gl.weightusvARB = GETFN_(proto::WeightusvARBFn, "glWeightusvARB")) && ok;
		gl.ARB_vertex_blend = ok;
	}
	//- end: ARB_vertex_blend

	//-//-- 
	//- Ext: ARB_vertex_buffer_object
	static char const* ARB_vertex_buffer_object = "ARB_vertex_buffer_object";
	if( listed_( arg.extList, ARB_vertex_buffer_object ) && arg.checkExt( ARB_vertex_buffer_object, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.bindBufferARB = GETFN_(proto::BindBufferARBFn, "glBindBufferARB")) && ok;
		ok = !!(gl.bufferDataARB = GETFN_(proto::BufferDataARBFn, "glBufferDataARB")) && ok;
		ok = !!(gl.bufferSubDataARB = GETFN_(proto::BufferSubDataARBFn, "glBufferSubDataARB")) && ok;
		ok = !!(gl.deleteBuffersARB = GETFN_(proto::DeleteBuffersARBFn, "glDeleteBuffersARB")) && ok;
		ok = !!(gl.genBuffersARB = GETFN_(proto::GenBuffersARBFn, "glGenBuffersARB")) && ok;
		ok = !!(gl.getBufferParameterivARB = GETFN_(proto::GetBufferParameterivARBFn, "glGetBufferParameterivARB")) && ok;
		ok = !!(gl.getBufferPointervARB = GETFN_(proto::GetBufferPointervARBFn, "glGetBufferPointervARB")) && ok;
		ok = !!(gl.getBufferSubDataARB = GETFN_(proto::GetBufferSubDataARBFn, "glGetBufferSubDataARB")) && ok;
		ok = !!(gl.isBufferARB = GETFN_(proto::IsBufferARBFn, "glIsBufferARB")) && ok;
		ok = !!(gl.mapBufferARB = GETFN_(proto::MapBufferARBFn, "glMapBufferARB")) && ok;
		ok = !!(gl.unmapBufferARB = GETFN_(proto::UnmapBufferARBFn, "glUnmapBufferARB")) && ok;
		gl.ARB_vertex_buffer_object = ok;
	}
	//- end: ARB_vertex_buffer_object

	//-//-- 
	//- Ext: ARB_vertex_program
	static char const* ARB_vertex_program = "ARB_vertex_program";
	if( listed_( arg.extList, ARB_vertex_program ) && arg.checkExt( ARB_vertex_program, arg.user ) )
	{
		bool ok = true;

		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.bindProgramARB = GETFN_(proto::BindProgramARBFn, "glBindProgramARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.deleteProgramsARB = GETFN_(proto::DeleteProgramsARBFn, "glDeleteProgramsARB")) && ok;
		}
		ok = !!(gl.disableVertexAttribArrayARB = GETFN_(proto::DisableVertexAttribArrayARBFn, "glDisableVertexAttribArrayARB")) && ok;
		ok = !!(gl.enableVertexAttribArrayARB = GETFN_(proto::EnableVertexAttribArrayARBFn, "glEnableVertexAttribArrayARB")) && ok;
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.genProgramsARB = GETFN_(proto::GenProgramsARBFn, "glGenProgramsARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.getProgramEnvParameterdvARB = GETFN_(proto::GetProgramEnvParameterdvARBFn, "glGetProgramEnvParameterdvARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.getProgramEnvParameterfvARB = GETFN_(proto::GetProgramEnvParameterfvARBFn, "glGetProgramEnvParameterfvARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.getProgramLocalParameterdvARB = GETFN_(proto::GetProgramLocalParameterdvARBFn, "glGetProgramLocalParameterdvARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.getProgramLocalParameterfvARB = GETFN_(proto::GetProgramLocalParameterfvARBFn, "glGetProgramLocalParameterfvARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.getProgramStringARB = GETFN_(proto::GetProgramStringARBFn, "glGetProgramStringARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.getProgramivARB = GETFN_(proto::GetProgramivARBFn, "glGetProgramivARB")) && ok;
		}
		ok = !!(gl.getVertexAttribPointervARB = GETFN_(proto::GetVertexAttribPointervARBFn, "glGetVertexAttribPointervARB")) && ok;
		ok = !!(gl.getVertexAttribdvARB = GETFN_(proto::GetVertexAttribdvARBFn, "glGetVertexAttribdvARB")) && ok;
		ok = !!(gl.getVertexAttribfvARB = GETFN_(proto::GetVertexAttribfvARBFn, "glGetVertexAttribfvARB")) && ok;
		ok = !!(gl.getVertexAttribivARB = GETFN_(proto::GetVertexAttribivARBFn, "glGetVertexAttribivARB")) && ok;
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.isProgramARB = GETFN_(proto::IsProgramARBFn, "glIsProgramARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.programEnvParameter4dARB = GETFN_(proto::ProgramEnvParameter4dARBFn, "glProgramEnvParameter4dARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.programEnvParameter4dvARB = GETFN_(proto::ProgramEnvParameter4dvARBFn, "glProgramEnvParameter4dvARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.programEnvParameter4fARB = GETFN_(proto::ProgramEnvParameter4fARBFn, "glProgramEnvParameter4fARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.programEnvParameter4fvARB = GETFN_(proto::ProgramEnvParameter4fvARBFn, "glProgramEnvParameter4fvARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.programLocalParameter4dARB = GETFN_(proto::ProgramLocalParameter4dARBFn, "glProgramLocalParameter4dARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.programLocalParameter4dvARB = GETFN_(proto::ProgramLocalParameter4dvARBFn, "glProgramLocalParameter4dvARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.programLocalParameter4fARB = GETFN_(proto::ProgramLocalParameter4fARBFn, "glProgramLocalParameter4fARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.programLocalParameter4fvARB = GETFN_(proto::ProgramLocalParameter4fvARBFn, "glProgramLocalParameter4fvARB")) && ok;
		}
		if( !gl.ARB_fragment_program ) {
			ok = !!(gl.programStringARB = GETFN_(proto::ProgramStringARBFn, "glProgramStringARB")) && ok;
		}
		ok = !!(gl.vertexAttrib1dARB = GETFN_(proto::VertexAttrib1dARBFn, "glVertexAttrib1dARB")) && ok;
		ok = !!(gl.vertexAttrib1dvARB = GETFN_(proto::VertexAttrib1dvARBFn, "glVertexAttrib1dvARB")) && ok;
		ok = !!(gl.vertexAttrib1fARB = GETFN_(proto::VertexAttrib1fARBFn, "glVertexAttrib1fARB")) && ok;
		ok = !!(gl.vertexAttrib1fvARB = GETFN_(proto::VertexAttrib1fvARBFn, "glVertexAttrib1fvARB")) && ok;
		ok = !!(gl.vertexAttrib1sARB = GETFN_(proto::VertexAttrib1sARBFn, "glVertexAttrib1sARB")) && ok;
		ok = !!(gl.vertexAttrib1svARB = GETFN_(proto::VertexAttrib1svARBFn, "glVertexAttrib1svARB")) && ok;
		ok = !!(gl.vertexAttrib2dARB = GETFN_(proto::VertexAttrib2dARBFn, "glVertexAttrib2dARB")) && ok;
		ok = !!(gl.vertexAttrib2dvARB = GETFN_(proto::VertexAttrib2dvARBFn, "glVertexAttrib2dvARB")) && ok;
		ok = !!(gl.vertexAttrib2fARB = GETFN_(proto::VertexAttrib2fARBFn, "glVertexAttrib2fARB")) && ok;
		ok = !!(gl.vertexAttrib2fvARB = GETFN_(proto::VertexAttrib2fvARBFn, "glVertexAttrib2fvARB")) && ok;
		ok = !!(gl.vertexAttrib2sARB = GETFN_(proto::VertexAttrib2sARBFn, "glVertexAttrib2sARB")) && ok;
		ok = !!(gl.vertexAttrib2svARB = GETFN_(proto::VertexAttrib2svARBFn, "glVertexAttrib2svARB")) && ok;
		ok = !!(gl.vertexAttrib3dARB = GETFN_(proto::VertexAttrib3dARBFn, "glVertexAttrib3dARB")) && ok;
		ok = !!(gl.vertexAttrib3dvARB = GETFN_(proto::VertexAttrib3dvARBFn, "glVertexAttrib3dvARB")) && ok;
		ok = !!(gl.vertexAttrib3fARB = GETFN_(proto::VertexAttrib3fARBFn, "glVertexAttrib3fARB")) && ok;
		ok = !!(gl.vertexAttrib3fvARB = GETFN_(proto::VertexAttrib3fvARBFn, "glVertexAttrib3fvARB")) && ok;
		ok = !!(gl.vertexAttrib3sARB = GETFN_(proto::VertexAttrib3sARBFn, "glVertexAttrib3sARB")) && ok;
		ok = !!(gl.vertexAttrib3svARB = GETFN_(proto::VertexAttrib3svARBFn, "glVertexAttrib3svARB")) && ok;
		ok = !!(gl.vertexAttrib4NbvARB = GETFN_(proto::VertexAttrib4NbvARBFn, "glVertexAttrib4NbvARB")) && ok;
		ok = !!(gl.vertexAttrib4NivARB = GETFN_(proto::VertexAttrib4NivARBFn, "glVertexAttrib4NivARB")) && ok;
		ok = !!(gl.vertexAttrib4NsvARB = GETFN_(proto::VertexAttrib4NsvARBFn, "glVertexAttrib4NsvARB")) && ok;
		ok = !!(gl.vertexAttrib4NubARB = GETFN_(proto::VertexAttrib4NubARBFn, "glVertexAttrib4NubARB")) && ok;
		ok = !!(gl.vertexAttrib4NubvARB = GETFN_(proto::VertexAttrib4NubvARBFn, "glVertexAttrib4NubvARB")) && ok;
		ok = !!(gl.vertexAttrib4NuivARB = GETFN_(proto::VertexAttrib4NuivARBFn, "glVertexAttrib4NuivARB")) && ok;
		ok = !!(gl.vertexAttrib4NusvARB = GETFN_(proto::VertexAttrib4NusvARBFn, "glVertexAttrib4NusvARB")) && ok;
		ok = !!(gl.vertexAttrib4bvARB = GETFN_(proto::VertexAttrib4bvARBFn, "glVertexAttrib4bvARB")) && ok;
		ok = !!(gl.vertexAttrib4dARB = GETFN_(proto::VertexAttrib4dARBFn, "glVertexAttrib4dARB")) && ok;
		ok = !!(gl.vertexAttrib4dvARB = GETFN_(proto::VertexAttrib4dvARBFn, "glVertexAttrib4dvARB")) && ok;
		ok = !!(gl.vertexAttrib4fARB = GETFN_(proto::VertexAttrib4fARBFn, "glVertexAttrib4fARB")) && ok;
		ok = !!(gl.vertexAttrib4fvARB = GETFN_(proto::VertexAttrib4fvARBFn, "glVertexAttrib4fvARB")) && ok;
		ok = !!(gl.vertexAttrib4ivARB = GETFN_(proto::VertexAttrib4ivARBFn, "glVertexAttrib4ivARB")) && ok;
		ok = !!(gl.vertexAttrib4sARB = GETFN_(proto::VertexAttrib4sARBFn, "glVertexAttrib4sARB")) && ok;
		ok = !!(gl.vertexAttrib4svARB = GETFN_(proto::VertexAttrib4svARBFn, "glVertexAttrib4svARB")) && ok;
		ok = !!(gl.vertexAttrib4ubvARB = GETFN_(proto::VertexAttrib4ubvARBFn, "glVertexAttrib4ubvARB")) && ok;
		ok = !!(gl.vertexAttrib4uivARB = GETFN_(proto::VertexAttrib4uivARBFn, "glVertexAttrib4uivARB")) && ok;
		ok = !!(gl.vertexAttrib4usvARB = GETFN_(proto::VertexAttrib4usvARBFn, "glVertexAttrib4usvARB")) && ok;
		ok = !!(gl.vertexAttribPointerARB = GETFN_(proto::VertexAttribPointerARBFn, "glVertexAttribPointerARB")) && ok;
		gl.ARB_vertex_program = ok;
	}
	//- end: ARB_vertex_program

	//-//-- 
	//- Ext: ARB_vertex_shader
	static char const* ARB_vertex_shader = "ARB_vertex_shader";
	if( listed_( arg.extList, ARB_vertex_shader ) && arg.checkExt( ARB_vertex_shader, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.bindAttribLocationARB = GETFN_(proto::BindAttribLocationARBFn, "glBindAttribLocationARB")) && ok;
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.disableVertexAttribArrayARB = GETFN_(proto::DisableVertexAttribArrayARBFn, "glDisableVertexAttribArrayARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.enableVertexAttribArrayARB = GETFN_(proto::EnableVertexAttribArrayARBFn, "glEnableVertexAttribArrayARB")) && ok;
		}
		ok = !!(gl.getActiveAttribARB = GETFN_(proto::GetActiveAttribARBFn, "glGetActiveAttribARB")) && ok;
		ok = !!(gl.getAttribLocationARB = GETFN_(proto::GetAttribLocationARBFn, "glGetAttribLocationARB")) && ok;
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.getVertexAttribPointervARB = GETFN_(proto::GetVertexAttribPointervARBFn, "glGetVertexAttribPointervARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.getVertexAttribdvARB = GETFN_(proto::GetVertexAttribdvARBFn, "glGetVertexAttribdvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.getVertexAttribfvARB = GETFN_(proto::GetVertexAttribfvARBFn, "glGetVertexAttribfvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.getVertexAttribivARB = GETFN_(proto::GetVertexAttribivARBFn, "glGetVertexAttribivARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib1dARB = GETFN_(proto::VertexAttrib1dARBFn, "glVertexAttrib1dARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib1dvARB = GETFN_(proto::VertexAttrib1dvARBFn, "glVertexAttrib1dvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib1fARB = GETFN_(proto::VertexAttrib1fARBFn, "glVertexAttrib1fARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib1fvARB = GETFN_(proto::VertexAttrib1fvARBFn, "glVertexAttrib1fvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib1sARB = GETFN_(proto::VertexAttrib1sARBFn, "glVertexAttrib1sARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib1svARB = GETFN_(proto::VertexAttrib1svARBFn, "glVertexAttrib1svARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib2dARB = GETFN_(proto::VertexAttrib2dARBFn, "glVertexAttrib2dARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib2dvARB = GETFN_(proto::VertexAttrib2dvARBFn, "glVertexAttrib2dvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib2fARB = GETFN_(proto::VertexAttrib2fARBFn, "glVertexAttrib2fARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib2fvARB = GETFN_(proto::VertexAttrib2fvARBFn, "glVertexAttrib2fvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib2sARB = GETFN_(proto::VertexAttrib2sARBFn, "glVertexAttrib2sARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib2svARB = GETFN_(proto::VertexAttrib2svARBFn, "glVertexAttrib2svARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib3dARB = GETFN_(proto::VertexAttrib3dARBFn, "glVertexAttrib3dARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib3dvARB = GETFN_(proto::VertexAttrib3dvARBFn, "glVertexAttrib3dvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib3fARB = GETFN_(proto::VertexAttrib3fARBFn, "glVertexAttrib3fARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib3fvARB = GETFN_(proto::VertexAttrib3fvARBFn, "glVertexAttrib3fvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib3sARB = GETFN_(proto::VertexAttrib3sARBFn, "glVertexAttrib3sARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib3svARB = GETFN_(proto::VertexAttrib3svARBFn, "glVertexAttrib3svARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4NbvARB = GETFN_(proto::VertexAttrib4NbvARBFn, "glVertexAttrib4NbvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4NivARB = GETFN_(proto::VertexAttrib4NivARBFn, "glVertexAttrib4NivARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4NsvARB = GETFN_(proto::VertexAttrib4NsvARBFn, "glVertexAttrib4NsvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4NubARB = GETFN_(proto::VertexAttrib4NubARBFn, "glVertexAttrib4NubARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4NubvARB = GETFN_(proto::VertexAttrib4NubvARBFn, "glVertexAttrib4NubvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4NuivARB = GETFN_(proto::VertexAttrib4NuivARBFn, "glVertexAttrib4NuivARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4NusvARB = GETFN_(proto::VertexAttrib4NusvARBFn, "glVertexAttrib4NusvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4bvARB = GETFN_(proto::VertexAttrib4bvARBFn, "glVertexAttrib4bvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4dARB = GETFN_(proto::VertexAttrib4dARBFn, "glVertexAttrib4dARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4dvARB = GETFN_(proto::VertexAttrib4dvARBFn, "glVertexAttrib4dvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4fARB = GETFN_(proto::VertexAttrib4fARBFn, "glVertexAttrib4fARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4fvARB = GETFN_(proto::VertexAttrib4fvARBFn, "glVertexAttrib4fvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4ivARB = GETFN_(proto::VertexAttrib4ivARBFn, "glVertexAttrib4ivARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4sARB = GETFN_(proto::VertexAttrib4sARBFn, "glVertexAttrib4sARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4svARB = GETFN_(proto::VertexAttrib4svARBFn, "glVertexAttrib4svARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4ubvARB = GETFN_(proto::VertexAttrib4ubvARBFn, "glVertexAttrib4ubvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4uivARB = GETFN_(proto::VertexAttrib4uivARBFn, "glVertexAttrib4uivARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttrib4usvARB = GETFN_(proto::VertexAttrib4usvARBFn, "glVertexAttrib4usvARB")) && ok;
		}
		if( !gl.ARB_vertex_program ) {
			ok = !!(gl.vertexAttribPointerARB = GETFN_(proto::VertexAttribPointerARBFn, "glVertexAttribPointerARB")) && ok;
		}
		gl.ARB_vertex_shader = ok;
	}
	//- end: ARB_vertex_shader

	//-//-- 
	//- Ext: ARB_vertex_type_2_10_10_10_rev
	static char const* ARB_vertex_type_2_10_10_10_rev = "ARB_vertex_type_2_10_10_10_rev";
	if( listed_( arg.extList, ARB_vertex_type_2_10_10_10_rev ) && arg.checkExt( ARB_vertex_type_2_10_10_10_rev, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.colorP3ui = GETFN_(proto::ColorP3uiFn, "glColorP3ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.colorP3uiv = GETFN_(proto::ColorP3uivFn, "glColorP3uiv")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.colorP4ui = GETFN_(proto::ColorP4uiFn, "glColorP4ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.colorP4uiv = GETFN_(proto::ColorP4uivFn, "glColorP4uiv")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.multiTexCoordP1ui = GETFN_(proto::MultiTexCoordP1uiFn, "glMultiTexCoordP1ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.multiTexCoordP1uiv = GETFN_(proto::MultiTexCoordP1uivFn, "glMultiTexCoordP1uiv")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.multiTexCoordP2ui = GETFN_(proto::MultiTexCoordP2uiFn, "glMultiTexCoordP2ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.multiTexCoordP2uiv = GETFN_(proto::MultiTexCoordP2uivFn, "glMultiTexCoordP2uiv")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.multiTexCoordP3ui = GETFN_(proto::MultiTexCoordP3uiFn, "glMultiTexCoordP3ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.multiTexCoordP3uiv = GETFN_(proto::MultiTexCoordP3uivFn, "glMultiTexCoordP3uiv")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.multiTexCoordP4ui = GETFN_(proto::MultiTexCoordP4uiFn, "glMultiTexCoordP4ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.multiTexCoordP4uiv = GETFN_(proto::MultiTexCoordP4uivFn, "glMultiTexCoordP4uiv")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.normalP3ui = GETFN_(proto::NormalP3uiFn, "glNormalP3ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.normalP3uiv = GETFN_(proto::NormalP3uivFn, "glNormalP3uiv")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.secondaryColorP3ui = GETFN_(proto::SecondaryColorP3uiFn, "glSecondaryColorP3ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.secondaryColorP3uiv = GETFN_(proto::SecondaryColorP3uivFn, "glSecondaryColorP3uiv")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.texCoordP1ui = GETFN_(proto::TexCoordP1uiFn, "glTexCoordP1ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.texCoordP1uiv = GETFN_(proto::TexCoordP1uivFn, "glTexCoordP1uiv")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.texCoordP2ui = GETFN_(proto::TexCoordP2uiFn, "glTexCoordP2ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.texCoordP2uiv = GETFN_(proto::TexCoordP2uivFn, "glTexCoordP2uiv")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.texCoordP3ui = GETFN_(proto::TexCoordP3uiFn, "glTexCoordP3ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.texCoordP3uiv = GETFN_(proto::TexCoordP3uivFn, "glTexCoordP3uiv")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.texCoordP4ui = GETFN_(proto::TexCoordP4uiFn, "glTexCoordP4ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.texCoordP4uiv = GETFN_(proto::TexCoordP4uivFn, "glTexCoordP4uiv")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.vertexAttribP1ui = GETFN_(proto::VertexAttribP1uiFn, "glVertexAttribP1ui")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.vertexAttribP1uiv = GETFN_(proto::VertexAttribP1uivFn, "glVertexAttribP1uiv")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.vertexAttribP2ui = GETFN_(proto::VertexAttribP2uiFn, "glVertexAttribP2ui")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.vertexAttribP2uiv = GETFN_(proto::VertexAttribP2uivFn, "glVertexAttribP2uiv")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.vertexAttribP3ui = GETFN_(proto::VertexAttribP3uiFn, "glVertexAttribP3ui")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.vertexAttribP3uiv = GETFN_(proto::VertexAttribP3uivFn, "glVertexAttribP3uiv")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.vertexAttribP4ui = GETFN_(proto::VertexAttribP4uiFn, "glVertexAttribP4ui")) && ok;
		}
		if( !gl.VERSION_3_3 ) {
			ok = !!(gl.vertexAttribP4uiv = GETFN_(proto::VertexAttribP4uivFn, "glVertexAttribP4uiv")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.vertexP2ui = GETFN_(proto::VertexP2uiFn, "glVertexP2ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.vertexP2uiv = GETFN_(proto::VertexP2uivFn, "glVertexP2uiv")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.vertexP3ui = GETFN_(proto::VertexP3uiFn, "glVertexP3ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.vertexP3uiv = GETFN_(proto::VertexP3uivFn, "glVertexP3uiv")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.vertexP4ui = GETFN_(proto::VertexP4uiFn, "glVertexP4ui")) && ok;
		}
		if( !gl.VERSION_3_3_compatibility ) {
			ok = !!(gl.vertexP4uiv = GETFN_(proto::VertexP4uivFn, "glVertexP4uiv")) && ok;
		}
		gl.ARB_vertex_type_2_10_10_10_rev = ok;
	}
	//- end: ARB_vertex_type_2_10_10_10_rev

	//-//-- 
	//- Ext: ARB_viewport_array
	static char const* ARB_viewport_array = "ARB_viewport_array";
	if( listed_( arg.extList, ARB_viewport_array ) && arg.checkExt( ARB_viewport_array, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.depthRangeArrayv = GETFN_(proto::DepthRangeArrayvFn, "glDepthRangeArrayv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.depthRangeIndexed = GETFN_(proto::DepthRangeIndexedFn, "glDepthRangeIndexed")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.getDoublei_v = GETFN_(proto::GetDoublei_vFn, "glGetDoublei_v")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.getFloati_v = GETFN_(proto::GetFloati_vFn, "glGetFloati_v")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.scissorArrayv = GETFN_(proto::ScissorArrayvFn, "glScissorArrayv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.scissorIndexed = GETFN_(proto::ScissorIndexedFn, "glScissorIndexed")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.scissorIndexedv = GETFN_(proto::ScissorIndexedvFn, "glScissorIndexedv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.viewportArrayv = GETFN_(proto::ViewportArrayvFn, "glViewportArrayv")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.viewportIndexedf = GETFN_(proto::ViewportIndexedfFn, "glViewportIndexedf")) && ok;
		}
		if( !gl.VERSION_4_1 ) {
			ok = !!(gl.viewportIndexedfv = GETFN_(proto::ViewportIndexedfvFn, "glViewportIndexedfv")) && ok;
		}
		gl.ARB_viewport_array = ok;
	}
	//- end: ARB_viewport_array

	//-//-- 
	//- Ext: ARB_window_pos
	static char const* ARB_window_pos = "ARB_window_pos";
	if( listed_( arg.extList, ARB_window_pos ) && arg.checkExt( ARB_window_pos, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.windowPos2dARB = GETFN_(proto::WindowPos2dARBFn, "glWindowPos2dARB")) && ok;
		ok = !!(gl.windowPos2dvARB = GETFN_(proto::WindowPos2dvARBFn, "glWindowPos2dvARB")) && ok;
		ok = !!(gl.windowPos2fARB = GETFN_(proto::WindowPos2fARBFn, "glWindowPos2fARB")) && ok;
		ok = !!(gl.windowPos2fvARB = GETFN_(proto::WindowPos2fvARBFn, "glWindowPos2fvARB")) && ok;
		ok = !!(gl.windowPos2iARB = GETFN_(proto::WindowPos2iARBFn, "glWindowPos2iARB")) && ok;
		ok = !!(gl.windowPos2ivARB = GETFN_(proto::WindowPos2ivARBFn, "glWindowPos2ivARB")) && ok;
		ok = !!(gl.windowPos2sARB = GETFN_(proto::WindowPos2sARBFn, "glWindowPos2sARB")) && ok;
		ok = !!(gl.windowPos2svARB = GETFN_(proto::WindowPos2svARBFn, "glWindowPos2svARB")) && ok;
		ok = !!(gl.windowPos3dARB = GETFN_(proto::WindowPos3dARBFn, "glWindowPos3dARB")) && ok;
		ok = !!(gl.windowPos3dvARB = GETFN_(proto::WindowPos3dvARBFn, "glWindowPos3dvARB")) && ok;
		ok = !!(gl.windowPos3fARB = GETFN_(proto::WindowPos3fARBFn, "glWindowPos3fARB")) && ok;
		ok = !!(gl.windowPos3fvARB = GETFN_(proto::WindowPos3fvARBFn, "glWindowPos3fvARB")) && ok;
		ok = !!(gl.windowPos3iARB = GETFN_(proto::WindowPos3iARBFn, "glWindowPos3iARB")) && ok;
		ok = !!(gl.windowPos3ivARB = GETFN_(proto::WindowPos3ivARBFn, "glWindowPos3ivARB")) && ok;
		ok = !!(gl.windowPos3sARB = GETFN_(proto::WindowPos3sARBFn, "glWindowPos3sARB")) && ok;
		ok = !!(gl.windowPos3svARB = GETFN_(proto::WindowPos3svARBFn, "glWindowPos3svARB")) && ok;
		gl.ARB_window_pos = ok;
	}
	//- end: ARB_window_pos

	//-//-- 
	//- Ext: ARB_gl_spirv
	static char const* ARB_gl_spirv = "ARB_gl_spirv";
	if( listed_( arg.extList, ARB_gl_spirv ) && arg.checkExt( ARB_gl_spirv, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.specializeShaderARB = GETFN_(proto::SpecializeShaderARBFn, "glSpecializeShaderARB")) && ok;
		gl.ARB_gl_spirv = ok;
	}
	//- end: ARB_gl_spirv

}

#undef GETFN_
//--    <<< ~ dlapi::gl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE3(dlapi,gl,detail)
//--//////////////////////////////////////////////////////////////////////////
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
