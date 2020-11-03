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

void load_gl_other_( GLapi& gl, LoaderArg_ const& arg )
{
	//-//-- 
	//- Ext: APPLE_element_array
	static char const* APPLE_element_array = "APPLE_element_array";
	if( listed_( arg.extList, APPLE_element_array ) && arg.checkExt( APPLE_element_array, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.drawElementArrayAPPLE = GETFN_(proto::DrawElementArrayAPPLEFn, "glDrawElementArrayAPPLE")) && ok;
		ok = !!(gl.drawRangeElementArrayAPPLE = GETFN_(proto::DrawRangeElementArrayAPPLEFn, "glDrawRangeElementArrayAPPLE")) && ok;
		ok = !!(gl.elementPointerAPPLE = GETFN_(proto::ElementPointerAPPLEFn, "glElementPointerAPPLE")) && ok;
		ok = !!(gl.multiDrawElementArrayAPPLE = GETFN_(proto::MultiDrawElementArrayAPPLEFn, "glMultiDrawElementArrayAPPLE")) && ok;
		ok = !!(gl.multiDrawRangeElementArrayAPPLE = GETFN_(proto::MultiDrawRangeElementArrayAPPLEFn, "glMultiDrawRangeElementArrayAPPLE")) && ok;
		gl.APPLE_element_array = ok;
	}
	//- end: APPLE_element_array

	//-//-- 
	//- Ext: APPLE_fence
	static char const* APPLE_fence = "APPLE_fence";
	if( listed_( arg.extList, APPLE_fence ) && arg.checkExt( APPLE_fence, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.deleteFencesAPPLE = GETFN_(proto::DeleteFencesAPPLEFn, "glDeleteFencesAPPLE")) && ok;
		ok = !!(gl.finishFenceAPPLE = GETFN_(proto::FinishFenceAPPLEFn, "glFinishFenceAPPLE")) && ok;
		ok = !!(gl.finishObjectAPPLE = GETFN_(proto::FinishObjectAPPLEFn, "glFinishObjectAPPLE")) && ok;
		ok = !!(gl.genFencesAPPLE = GETFN_(proto::GenFencesAPPLEFn, "glGenFencesAPPLE")) && ok;
		ok = !!(gl.isFenceAPPLE = GETFN_(proto::IsFenceAPPLEFn, "glIsFenceAPPLE")) && ok;
		ok = !!(gl.setFenceAPPLE = GETFN_(proto::SetFenceAPPLEFn, "glSetFenceAPPLE")) && ok;
		ok = !!(gl.testFenceAPPLE = GETFN_(proto::TestFenceAPPLEFn, "glTestFenceAPPLE")) && ok;
		ok = !!(gl.testObjectAPPLE = GETFN_(proto::TestObjectAPPLEFn, "glTestObjectAPPLE")) && ok;
		gl.APPLE_fence = ok;
	}
	//- end: APPLE_fence

	//-//-- 
	//- Ext: APPLE_flush_buffer_range
	static char const* APPLE_flush_buffer_range = "APPLE_flush_buffer_range";
	if( listed_( arg.extList, APPLE_flush_buffer_range ) && arg.checkExt( APPLE_flush_buffer_range, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.bufferParameteriAPPLE = GETFN_(proto::BufferParameteriAPPLEFn, "glBufferParameteriAPPLE")) && ok;
		ok = !!(gl.flushMappedBufferRangeAPPLE = GETFN_(proto::FlushMappedBufferRangeAPPLEFn, "glFlushMappedBufferRangeAPPLE")) && ok;
		gl.APPLE_flush_buffer_range = ok;
	}
	//- end: APPLE_flush_buffer_range

	//-//-- 
	//- Ext: APPLE_object_purgeable
	static char const* APPLE_object_purgeable = "APPLE_object_purgeable";
	if( listed_( arg.extList, APPLE_object_purgeable ) && arg.checkExt( APPLE_object_purgeable, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getObjectParameterivAPPLE = GETFN_(proto::GetObjectParameterivAPPLEFn, "glGetObjectParameterivAPPLE")) && ok;
		ok = !!(gl.objectPurgeableAPPLE = GETFN_(proto::ObjectPurgeableAPPLEFn, "glObjectPurgeableAPPLE")) && ok;
		ok = !!(gl.objectUnpurgeableAPPLE = GETFN_(proto::ObjectUnpurgeableAPPLEFn, "glObjectUnpurgeableAPPLE")) && ok;
		gl.APPLE_object_purgeable = ok;
	}
	//- end: APPLE_object_purgeable

	//-//-- 
	//- Ext: APPLE_texture_range
	static char const* APPLE_texture_range = "APPLE_texture_range";
	if( listed_( arg.extList, APPLE_texture_range ) && arg.checkExt( APPLE_texture_range, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getTexParameterPointervAPPLE = GETFN_(proto::GetTexParameterPointervAPPLEFn, "glGetTexParameterPointervAPPLE")) && ok;
		ok = !!(gl.textureRangeAPPLE = GETFN_(proto::TextureRangeAPPLEFn, "glTextureRangeAPPLE")) && ok;
		gl.APPLE_texture_range = ok;
	}
	//- end: APPLE_texture_range

	//-//-- 
	//- Ext: APPLE_vertex_array_object
	static char const* APPLE_vertex_array_object = "APPLE_vertex_array_object";
	if( listed_( arg.extList, APPLE_vertex_array_object ) && arg.checkExt( APPLE_vertex_array_object, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.bindVertexArrayAPPLE = GETFN_(proto::BindVertexArrayAPPLEFn, "glBindVertexArrayAPPLE")) && ok;
		ok = !!(gl.deleteVertexArraysAPPLE = GETFN_(proto::DeleteVertexArraysAPPLEFn, "glDeleteVertexArraysAPPLE")) && ok;
		ok = !!(gl.genVertexArraysAPPLE = GETFN_(proto::GenVertexArraysAPPLEFn, "glGenVertexArraysAPPLE")) && ok;
		ok = !!(gl.isVertexArrayAPPLE = GETFN_(proto::IsVertexArrayAPPLEFn, "glIsVertexArrayAPPLE")) && ok;
		gl.APPLE_vertex_array_object = ok;
	}
	//- end: APPLE_vertex_array_object

	//-//-- 
	//- Ext: APPLE_vertex_array_range
	static char const* APPLE_vertex_array_range = "APPLE_vertex_array_range";
	if( listed_( arg.extList, APPLE_vertex_array_range ) && arg.checkExt( APPLE_vertex_array_range, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.flushVertexArrayRangeAPPLE = GETFN_(proto::FlushVertexArrayRangeAPPLEFn, "glFlushVertexArrayRangeAPPLE")) && ok;
		ok = !!(gl.vertexArrayParameteriAPPLE = GETFN_(proto::VertexArrayParameteriAPPLEFn, "glVertexArrayParameteriAPPLE")) && ok;
		ok = !!(gl.vertexArrayRangeAPPLE = GETFN_(proto::VertexArrayRangeAPPLEFn, "glVertexArrayRangeAPPLE")) && ok;
		gl.APPLE_vertex_array_range = ok;
	}
	//- end: APPLE_vertex_array_range

	//-//-- 
	//- Ext: APPLE_vertex_program_evaluators
	static char const* APPLE_vertex_program_evaluators = "APPLE_vertex_program_evaluators";
	if( listed_( arg.extList, APPLE_vertex_program_evaluators ) && arg.checkExt( APPLE_vertex_program_evaluators, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.disableVertexAttribAPPLE = GETFN_(proto::DisableVertexAttribAPPLEFn, "glDisableVertexAttribAPPLE")) && ok;
		ok = !!(gl.enableVertexAttribAPPLE = GETFN_(proto::EnableVertexAttribAPPLEFn, "glEnableVertexAttribAPPLE")) && ok;
		ok = !!(gl.isVertexAttribEnabledAPPLE = GETFN_(proto::IsVertexAttribEnabledAPPLEFn, "glIsVertexAttribEnabledAPPLE")) && ok;
		ok = !!(gl.mapVertexAttrib1dAPPLE = GETFN_(proto::MapVertexAttrib1dAPPLEFn, "glMapVertexAttrib1dAPPLE")) && ok;
		ok = !!(gl.mapVertexAttrib1fAPPLE = GETFN_(proto::MapVertexAttrib1fAPPLEFn, "glMapVertexAttrib1fAPPLE")) && ok;
		ok = !!(gl.mapVertexAttrib2dAPPLE = GETFN_(proto::MapVertexAttrib2dAPPLEFn, "glMapVertexAttrib2dAPPLE")) && ok;
		ok = !!(gl.mapVertexAttrib2fAPPLE = GETFN_(proto::MapVertexAttrib2fAPPLEFn, "glMapVertexAttrib2fAPPLE")) && ok;
		gl.APPLE_vertex_program_evaluators = ok;
	}
	//- end: APPLE_vertex_program_evaluators

	//-//-- 
	//- Ext: GREMEDY_frame_terminator
	static char const* GREMEDY_frame_terminator = "GREMEDY_frame_terminator";
	if( listed_( arg.extList, GREMEDY_frame_terminator ) && arg.checkExt( GREMEDY_frame_terminator, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.frameTerminatorGREMEDY = GETFN_(proto::FrameTerminatorGREMEDYFn, "glFrameTerminatorGREMEDY")) && ok;
		gl.GREMEDY_frame_terminator = ok;
	}
	//- end: GREMEDY_frame_terminator

	//-//-- 
	//- Ext: GREMEDY_string_marker
	static char const* GREMEDY_string_marker = "GREMEDY_string_marker";
	if( listed_( arg.extList, GREMEDY_string_marker ) && arg.checkExt( GREMEDY_string_marker, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.stringMarkerGREMEDY = GETFN_(proto::StringMarkerGREMEDYFn, "glStringMarkerGREMEDY")) && ok;
		gl.GREMEDY_string_marker = ok;
	}
	//- end: GREMEDY_string_marker

	//-//-- 
	//- Ext: HP_image_transform
	static char const* HP_image_transform = "HP_image_transform";
	if( listed_( arg.extList, HP_image_transform ) && arg.checkExt( HP_image_transform, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getImageTransformParameterfvHP = GETFN_(proto::GetImageTransformParameterfvHPFn, "glGetImageTransformParameterfvHP")) && ok;
		ok = !!(gl.getImageTransformParameterivHP = GETFN_(proto::GetImageTransformParameterivHPFn, "glGetImageTransformParameterivHP")) && ok;
		ok = !!(gl.imageTransformParameterfHP = GETFN_(proto::ImageTransformParameterfHPFn, "glImageTransformParameterfHP")) && ok;
		ok = !!(gl.imageTransformParameterfvHP = GETFN_(proto::ImageTransformParameterfvHPFn, "glImageTransformParameterfvHP")) && ok;
		ok = !!(gl.imageTransformParameteriHP = GETFN_(proto::ImageTransformParameteriHPFn, "glImageTransformParameteriHP")) && ok;
		ok = !!(gl.imageTransformParameterivHP = GETFN_(proto::ImageTransformParameterivHPFn, "glImageTransformParameterivHP")) && ok;
		gl.HP_image_transform = ok;
	}
	//- end: HP_image_transform

	//-//-- 
	//- Ext: IBM_multimode_draw_arrays
	static char const* IBM_multimode_draw_arrays = "IBM_multimode_draw_arrays";
	if( listed_( arg.extList, IBM_multimode_draw_arrays ) && arg.checkExt( IBM_multimode_draw_arrays, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.multiModeDrawArraysIBM = GETFN_(proto::MultiModeDrawArraysIBMFn, "glMultiModeDrawArraysIBM")) && ok;
		ok = !!(gl.multiModeDrawElementsIBM = GETFN_(proto::MultiModeDrawElementsIBMFn, "glMultiModeDrawElementsIBM")) && ok;
		gl.IBM_multimode_draw_arrays = ok;
	}
	//- end: IBM_multimode_draw_arrays

	//-//-- 
	//- Ext: IBM_static_data
	static char const* IBM_static_data = "IBM_static_data";
	if( listed_( arg.extList, IBM_static_data ) && arg.checkExt( IBM_static_data, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.flushStaticDataIBM = GETFN_(proto::FlushStaticDataIBMFn, "glFlushStaticDataIBM")) && ok;
		gl.IBM_static_data = ok;
	}
	//- end: IBM_static_data

	//-//-- 
	//- Ext: IBM_vertex_array_lists
	static char const* IBM_vertex_array_lists = "IBM_vertex_array_lists";
	if( listed_( arg.extList, IBM_vertex_array_lists ) && arg.checkExt( IBM_vertex_array_lists, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.colorPointerListIBM = GETFN_(proto::ColorPointerListIBMFn, "glColorPointerListIBM")) && ok;
		ok = !!(gl.edgeFlagPointerListIBM = GETFN_(proto::EdgeFlagPointerListIBMFn, "glEdgeFlagPointerListIBM")) && ok;
		ok = !!(gl.fogCoordPointerListIBM = GETFN_(proto::FogCoordPointerListIBMFn, "glFogCoordPointerListIBM")) && ok;
		ok = !!(gl.indexPointerListIBM = GETFN_(proto::IndexPointerListIBMFn, "glIndexPointerListIBM")) && ok;
		ok = !!(gl.normalPointerListIBM = GETFN_(proto::NormalPointerListIBMFn, "glNormalPointerListIBM")) && ok;
		ok = !!(gl.secondaryColorPointerListIBM = GETFN_(proto::SecondaryColorPointerListIBMFn, "glSecondaryColorPointerListIBM")) && ok;
		ok = !!(gl.texCoordPointerListIBM = GETFN_(proto::TexCoordPointerListIBMFn, "glTexCoordPointerListIBM")) && ok;
		ok = !!(gl.vertexPointerListIBM = GETFN_(proto::VertexPointerListIBMFn, "glVertexPointerListIBM")) && ok;
		gl.IBM_vertex_array_lists = ok;
	}
	//- end: IBM_vertex_array_lists

	//-//-- 
	//- Ext: INTEL_framebuffer_CMAA
	static char const* INTEL_framebuffer_CMAA = "INTEL_framebuffer_CMAA";
	if( listed_( arg.extList, INTEL_framebuffer_CMAA ) && arg.checkExt( INTEL_framebuffer_CMAA, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.applyFramebufferAttachmentCMAAINTEL = GETFN_(proto::ApplyFramebufferAttachmentCMAAINTELFn, "glApplyFramebufferAttachmentCMAAINTEL")) && ok;
		gl.INTEL_framebuffer_CMAA = ok;
	}
	//- end: INTEL_framebuffer_CMAA

	//-//-- 
	//- Ext: INTEL_map_texture
	static char const* INTEL_map_texture = "INTEL_map_texture";
	if( listed_( arg.extList, INTEL_map_texture ) && arg.checkExt( INTEL_map_texture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.mapTexture2DINTEL = GETFN_(proto::MapTexture2DINTELFn, "glMapTexture2DINTEL")) && ok;
		ok = !!(gl.syncTextureINTEL = GETFN_(proto::SyncTextureINTELFn, "glSyncTextureINTEL")) && ok;
		ok = !!(gl.unmapTexture2DINTEL = GETFN_(proto::UnmapTexture2DINTELFn, "glUnmapTexture2DINTEL")) && ok;
		gl.INTEL_map_texture = ok;
	}
	//- end: INTEL_map_texture

	//-//-- 
	//- Ext: INTEL_parallel_arrays
	static char const* INTEL_parallel_arrays = "INTEL_parallel_arrays";
	if( listed_( arg.extList, INTEL_parallel_arrays ) && arg.checkExt( INTEL_parallel_arrays, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.colorPointervINTEL = GETFN_(proto::ColorPointervINTELFn, "glColorPointervINTEL")) && ok;
		ok = !!(gl.normalPointervINTEL = GETFN_(proto::NormalPointervINTELFn, "glNormalPointervINTEL")) && ok;
		ok = !!(gl.texCoordPointervINTEL = GETFN_(proto::TexCoordPointervINTELFn, "glTexCoordPointervINTEL")) && ok;
		ok = !!(gl.vertexPointervINTEL = GETFN_(proto::VertexPointervINTELFn, "glVertexPointervINTEL")) && ok;
		gl.INTEL_parallel_arrays = ok;
	}
	//- end: INTEL_parallel_arrays

	//-//-- 
	//- Ext: INTEL_performance_query
	static char const* INTEL_performance_query = "INTEL_performance_query";
	if( listed_( arg.extList, INTEL_performance_query ) && arg.checkExt( INTEL_performance_query, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.beginPerfQueryINTEL = GETFN_(proto::BeginPerfQueryINTELFn, "glBeginPerfQueryINTEL")) && ok;
		ok = !!(gl.createPerfQueryINTEL = GETFN_(proto::CreatePerfQueryINTELFn, "glCreatePerfQueryINTEL")) && ok;
		ok = !!(gl.deletePerfQueryINTEL = GETFN_(proto::DeletePerfQueryINTELFn, "glDeletePerfQueryINTEL")) && ok;
		ok = !!(gl.endPerfQueryINTEL = GETFN_(proto::EndPerfQueryINTELFn, "glEndPerfQueryINTEL")) && ok;
		ok = !!(gl.getFirstPerfQueryIdINTEL = GETFN_(proto::GetFirstPerfQueryIdINTELFn, "glGetFirstPerfQueryIdINTEL")) && ok;
		ok = !!(gl.getNextPerfQueryIdINTEL = GETFN_(proto::GetNextPerfQueryIdINTELFn, "glGetNextPerfQueryIdINTEL")) && ok;
		ok = !!(gl.getPerfCounterInfoINTEL = GETFN_(proto::GetPerfCounterInfoINTELFn, "glGetPerfCounterInfoINTEL")) && ok;
		ok = !!(gl.getPerfQueryDataINTEL = GETFN_(proto::GetPerfQueryDataINTELFn, "glGetPerfQueryDataINTEL")) && ok;
		ok = !!(gl.getPerfQueryIdByNameINTEL = GETFN_(proto::GetPerfQueryIdByNameINTELFn, "glGetPerfQueryIdByNameINTEL")) && ok;
		ok = !!(gl.getPerfQueryInfoINTEL = GETFN_(proto::GetPerfQueryInfoINTELFn, "glGetPerfQueryInfoINTEL")) && ok;
		gl.INTEL_performance_query = ok;
	}
	//- end: INTEL_performance_query

	//-//-- 
	//- Ext: KHR_blend_equation_advanced
	static char const* KHR_blend_equation_advanced = "KHR_blend_equation_advanced";
	if( listed_( arg.extList, KHR_blend_equation_advanced ) && arg.checkExt( KHR_blend_equation_advanced, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.blendBarrierKHR = GETFN_(proto::BlendBarrierKHRFn, "glBlendBarrierKHR")) && ok;
		gl.KHR_blend_equation_advanced = ok;
	}
	//- end: KHR_blend_equation_advanced

	//-//-- 
	//- Ext: KHR_debug
	static char const* KHR_debug = "KHR_debug";
	if( listed_( arg.extList, KHR_debug ) && arg.checkExt( KHR_debug, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.debugMessageCallback = GETFN_(proto::DebugMessageCallbackFn, "glDebugMessageCallback")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.debugMessageControl = GETFN_(proto::DebugMessageControlFn, "glDebugMessageControl")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.debugMessageInsert = GETFN_(proto::DebugMessageInsertFn, "glDebugMessageInsert")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.getDebugMessageLog = GETFN_(proto::GetDebugMessageLogFn, "glGetDebugMessageLog")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.getObjectLabel = GETFN_(proto::GetObjectLabelFn, "glGetObjectLabel")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.getObjectPtrLabel = GETFN_(proto::GetObjectPtrLabelFn, "glGetObjectPtrLabel")) && ok;
		}
		if( !gl.VERSION_1_1 ) {
			ok = !!(gl.getPointerv = GETFN_(proto::GetPointervFn, "glGetPointerv")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.objectLabel = GETFN_(proto::ObjectLabelFn, "glObjectLabel")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.objectPtrLabel = GETFN_(proto::ObjectPtrLabelFn, "glObjectPtrLabel")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.popDebugGroup = GETFN_(proto::PopDebugGroupFn, "glPopDebugGroup")) && ok;
		}
		if( !gl.VERSION_4_3 ) {
			ok = !!(gl.pushDebugGroup = GETFN_(proto::PushDebugGroupFn, "glPushDebugGroup")) && ok;
		}
		gl.KHR_debug = ok;
	}
	//- end: KHR_debug

	//-//-- 
	//- Ext: KHR_robustness
	static char const* KHR_robustness = "KHR_robustness";
	if( listed_( arg.extList, KHR_robustness ) && arg.checkExt( KHR_robustness, arg.user ) )
	{
		bool ok = true;

		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getGraphicsResetStatus = GETFN_(proto::GetGraphicsResetStatusFn, "glGetGraphicsResetStatus")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getnUniformfv = GETFN_(proto::GetnUniformfvFn, "glGetnUniformfv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getnUniformiv = GETFN_(proto::GetnUniformivFn, "glGetnUniformiv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.getnUniformuiv = GETFN_(proto::GetnUniformuivFn, "glGetnUniformuiv")) && ok;
		}
		if( !gl.VERSION_4_5 ) {
			ok = !!(gl.readnPixels = GETFN_(proto::ReadnPixelsFn, "glReadnPixels")) && ok;
		}
		gl.KHR_robustness = ok;
	}
	//- end: KHR_robustness

	//-//-- 
	//- Ext: MESA_resize_buffers
	static char const* MESA_resize_buffers = "MESA_resize_buffers";
	if( listed_( arg.extList, MESA_resize_buffers ) && arg.checkExt( MESA_resize_buffers, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.resizeBuffersMESA = GETFN_(proto::ResizeBuffersMESAFn, "glResizeBuffersMESA")) && ok;
		gl.MESA_resize_buffers = ok;
	}
	//- end: MESA_resize_buffers

	//-//-- 
	//- Ext: MESA_window_pos
	static char const* MESA_window_pos = "MESA_window_pos";
	if( listed_( arg.extList, MESA_window_pos ) && arg.checkExt( MESA_window_pos, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.windowPos2dMESA = GETFN_(proto::WindowPos2dMESAFn, "glWindowPos2dMESA")) && ok;
		ok = !!(gl.windowPos2dvMESA = GETFN_(proto::WindowPos2dvMESAFn, "glWindowPos2dvMESA")) && ok;
		ok = !!(gl.windowPos2fMESA = GETFN_(proto::WindowPos2fMESAFn, "glWindowPos2fMESA")) && ok;
		ok = !!(gl.windowPos2fvMESA = GETFN_(proto::WindowPos2fvMESAFn, "glWindowPos2fvMESA")) && ok;
		ok = !!(gl.windowPos2iMESA = GETFN_(proto::WindowPos2iMESAFn, "glWindowPos2iMESA")) && ok;
		ok = !!(gl.windowPos2ivMESA = GETFN_(proto::WindowPos2ivMESAFn, "glWindowPos2ivMESA")) && ok;
		ok = !!(gl.windowPos2sMESA = GETFN_(proto::WindowPos2sMESAFn, "glWindowPos2sMESA")) && ok;
		ok = !!(gl.windowPos2svMESA = GETFN_(proto::WindowPos2svMESAFn, "glWindowPos2svMESA")) && ok;
		ok = !!(gl.windowPos3dMESA = GETFN_(proto::WindowPos3dMESAFn, "glWindowPos3dMESA")) && ok;
		ok = !!(gl.windowPos3dvMESA = GETFN_(proto::WindowPos3dvMESAFn, "glWindowPos3dvMESA")) && ok;
		ok = !!(gl.windowPos3fMESA = GETFN_(proto::WindowPos3fMESAFn, "glWindowPos3fMESA")) && ok;
		ok = !!(gl.windowPos3fvMESA = GETFN_(proto::WindowPos3fvMESAFn, "glWindowPos3fvMESA")) && ok;
		ok = !!(gl.windowPos3iMESA = GETFN_(proto::WindowPos3iMESAFn, "glWindowPos3iMESA")) && ok;
		ok = !!(gl.windowPos3ivMESA = GETFN_(proto::WindowPos3ivMESAFn, "glWindowPos3ivMESA")) && ok;
		ok = !!(gl.windowPos3sMESA = GETFN_(proto::WindowPos3sMESAFn, "glWindowPos3sMESA")) && ok;
		ok = !!(gl.windowPos3svMESA = GETFN_(proto::WindowPos3svMESAFn, "glWindowPos3svMESA")) && ok;
		ok = !!(gl.windowPos4dMESA = GETFN_(proto::WindowPos4dMESAFn, "glWindowPos4dMESA")) && ok;
		ok = !!(gl.windowPos4dvMESA = GETFN_(proto::WindowPos4dvMESAFn, "glWindowPos4dvMESA")) && ok;
		ok = !!(gl.windowPos4fMESA = GETFN_(proto::WindowPos4fMESAFn, "glWindowPos4fMESA")) && ok;
		ok = !!(gl.windowPos4fvMESA = GETFN_(proto::WindowPos4fvMESAFn, "glWindowPos4fvMESA")) && ok;
		ok = !!(gl.windowPos4iMESA = GETFN_(proto::WindowPos4iMESAFn, "glWindowPos4iMESA")) && ok;
		ok = !!(gl.windowPos4ivMESA = GETFN_(proto::WindowPos4ivMESAFn, "glWindowPos4ivMESA")) && ok;
		ok = !!(gl.windowPos4sMESA = GETFN_(proto::WindowPos4sMESAFn, "glWindowPos4sMESA")) && ok;
		ok = !!(gl.windowPos4svMESA = GETFN_(proto::WindowPos4svMESAFn, "glWindowPos4svMESA")) && ok;
		gl.MESA_window_pos = ok;
	}
	//- end: MESA_window_pos

	//-//-- 
	//- Ext: OES_byte_coordinates
	static char const* OES_byte_coordinates = "OES_byte_coordinates";
	if( listed_( arg.extList, OES_byte_coordinates ) && arg.checkExt( OES_byte_coordinates, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.multiTexCoord1bOES = GETFN_(proto::MultiTexCoord1bOESFn, "glMultiTexCoord1bOES")) && ok;
		ok = !!(gl.multiTexCoord1bvOES = GETFN_(proto::MultiTexCoord1bvOESFn, "glMultiTexCoord1bvOES")) && ok;
		ok = !!(gl.multiTexCoord2bOES = GETFN_(proto::MultiTexCoord2bOESFn, "glMultiTexCoord2bOES")) && ok;
		ok = !!(gl.multiTexCoord2bvOES = GETFN_(proto::MultiTexCoord2bvOESFn, "glMultiTexCoord2bvOES")) && ok;
		ok = !!(gl.multiTexCoord3bOES = GETFN_(proto::MultiTexCoord3bOESFn, "glMultiTexCoord3bOES")) && ok;
		ok = !!(gl.multiTexCoord3bvOES = GETFN_(proto::MultiTexCoord3bvOESFn, "glMultiTexCoord3bvOES")) && ok;
		ok = !!(gl.multiTexCoord4bOES = GETFN_(proto::MultiTexCoord4bOESFn, "glMultiTexCoord4bOES")) && ok;
		ok = !!(gl.multiTexCoord4bvOES = GETFN_(proto::MultiTexCoord4bvOESFn, "glMultiTexCoord4bvOES")) && ok;
		ok = !!(gl.texCoord1bOES = GETFN_(proto::TexCoord1bOESFn, "glTexCoord1bOES")) && ok;
		ok = !!(gl.texCoord1bvOES = GETFN_(proto::TexCoord1bvOESFn, "glTexCoord1bvOES")) && ok;
		ok = !!(gl.texCoord2bOES = GETFN_(proto::TexCoord2bOESFn, "glTexCoord2bOES")) && ok;
		ok = !!(gl.texCoord2bvOES = GETFN_(proto::TexCoord2bvOESFn, "glTexCoord2bvOES")) && ok;
		ok = !!(gl.texCoord3bOES = GETFN_(proto::TexCoord3bOESFn, "glTexCoord3bOES")) && ok;
		ok = !!(gl.texCoord3bvOES = GETFN_(proto::TexCoord3bvOESFn, "glTexCoord3bvOES")) && ok;
		ok = !!(gl.texCoord4bOES = GETFN_(proto::TexCoord4bOESFn, "glTexCoord4bOES")) && ok;
		ok = !!(gl.texCoord4bvOES = GETFN_(proto::TexCoord4bvOESFn, "glTexCoord4bvOES")) && ok;
		ok = !!(gl.vertex2bOES = GETFN_(proto::Vertex2bOESFn, "glVertex2bOES")) && ok;
		ok = !!(gl.vertex2bvOES = GETFN_(proto::Vertex2bvOESFn, "glVertex2bvOES")) && ok;
		ok = !!(gl.vertex3bOES = GETFN_(proto::Vertex3bOESFn, "glVertex3bOES")) && ok;
		ok = !!(gl.vertex3bvOES = GETFN_(proto::Vertex3bvOESFn, "glVertex3bvOES")) && ok;
		ok = !!(gl.vertex4bOES = GETFN_(proto::Vertex4bOESFn, "glVertex4bOES")) && ok;
		ok = !!(gl.vertex4bvOES = GETFN_(proto::Vertex4bvOESFn, "glVertex4bvOES")) && ok;
		gl.OES_byte_coordinates = ok;
	}
	//- end: OES_byte_coordinates

	//-//-- 
	//- Ext: OES_fixed_point
	static char const* OES_fixed_point = "OES_fixed_point";
	if( listed_( arg.extList, OES_fixed_point ) && arg.checkExt( OES_fixed_point, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.accumxOES = GETFN_(proto::AccumxOESFn, "glAccumxOES")) && ok;
		ok = !!(gl.alphaFuncxOES = GETFN_(proto::AlphaFuncxOESFn, "glAlphaFuncxOES")) && ok;
		ok = !!(gl.bitmapxOES = GETFN_(proto::BitmapxOESFn, "glBitmapxOES")) && ok;
		ok = !!(gl.blendColorxOES = GETFN_(proto::BlendColorxOESFn, "glBlendColorxOES")) && ok;
		ok = !!(gl.clearAccumxOES = GETFN_(proto::ClearAccumxOESFn, "glClearAccumxOES")) && ok;
		ok = !!(gl.clearColorxOES = GETFN_(proto::ClearColorxOESFn, "glClearColorxOES")) && ok;
		ok = !!(gl.clearDepthxOES = GETFN_(proto::ClearDepthxOESFn, "glClearDepthxOES")) && ok;
		ok = !!(gl.clipPlanexOES = GETFN_(proto::ClipPlanexOESFn, "glClipPlanexOES")) && ok;
		ok = !!(gl.color3xOES = GETFN_(proto::Color3xOESFn, "glColor3xOES")) && ok;
		ok = !!(gl.color3xvOES = GETFN_(proto::Color3xvOESFn, "glColor3xvOES")) && ok;
		ok = !!(gl.color4xOES = GETFN_(proto::Color4xOESFn, "glColor4xOES")) && ok;
		ok = !!(gl.color4xvOES = GETFN_(proto::Color4xvOESFn, "glColor4xvOES")) && ok;
		ok = !!(gl.convolutionParameterxOES = GETFN_(proto::ConvolutionParameterxOESFn, "glConvolutionParameterxOES")) && ok;
		ok = !!(gl.convolutionParameterxvOES = GETFN_(proto::ConvolutionParameterxvOESFn, "glConvolutionParameterxvOES")) && ok;
		ok = !!(gl.depthRangexOES = GETFN_(proto::DepthRangexOESFn, "glDepthRangexOES")) && ok;
		ok = !!(gl.evalCoord1xOES = GETFN_(proto::EvalCoord1xOESFn, "glEvalCoord1xOES")) && ok;
		ok = !!(gl.evalCoord1xvOES = GETFN_(proto::EvalCoord1xvOESFn, "glEvalCoord1xvOES")) && ok;
		ok = !!(gl.evalCoord2xOES = GETFN_(proto::EvalCoord2xOESFn, "glEvalCoord2xOES")) && ok;
		ok = !!(gl.evalCoord2xvOES = GETFN_(proto::EvalCoord2xvOESFn, "glEvalCoord2xvOES")) && ok;
		ok = !!(gl.feedbackBufferxOES = GETFN_(proto::FeedbackBufferxOESFn, "glFeedbackBufferxOES")) && ok;
		ok = !!(gl.fogxOES = GETFN_(proto::FogxOESFn, "glFogxOES")) && ok;
		ok = !!(gl.fogxvOES = GETFN_(proto::FogxvOESFn, "glFogxvOES")) && ok;
		ok = !!(gl.frustumxOES = GETFN_(proto::FrustumxOESFn, "glFrustumxOES")) && ok;
		ok = !!(gl.getClipPlanexOES = GETFN_(proto::GetClipPlanexOESFn, "glGetClipPlanexOES")) && ok;
		ok = !!(gl.getConvolutionParameterxvOES = GETFN_(proto::GetConvolutionParameterxvOESFn, "glGetConvolutionParameterxvOES")) && ok;
		ok = !!(gl.getFixedvOES = GETFN_(proto::GetFixedvOESFn, "glGetFixedvOES")) && ok;
		ok = !!(gl.getHistogramParameterxvOES = GETFN_(proto::GetHistogramParameterxvOESFn, "glGetHistogramParameterxvOES")) && ok;
		ok = !!(gl.getLightxOES = GETFN_(proto::GetLightxOESFn, "glGetLightxOES")) && ok;
		ok = !!(gl.getMapxvOES = GETFN_(proto::GetMapxvOESFn, "glGetMapxvOES")) && ok;
		ok = !!(gl.getMaterialxOES = GETFN_(proto::GetMaterialxOESFn, "glGetMaterialxOES")) && ok;
		ok = !!(gl.getPixelMapxv = GETFN_(proto::GetPixelMapxvFn, "glGetPixelMapxv")) && ok;
		ok = !!(gl.getTexEnvxvOES = GETFN_(proto::GetTexEnvxvOESFn, "glGetTexEnvxvOES")) && ok;
		ok = !!(gl.getTexGenxvOES = GETFN_(proto::GetTexGenxvOESFn, "glGetTexGenxvOES")) && ok;
		ok = !!(gl.getTexLevelParameterxvOES = GETFN_(proto::GetTexLevelParameterxvOESFn, "glGetTexLevelParameterxvOES")) && ok;
		ok = !!(gl.getTexParameterxvOES = GETFN_(proto::GetTexParameterxvOESFn, "glGetTexParameterxvOES")) && ok;
		ok = !!(gl.indexxOES = GETFN_(proto::IndexxOESFn, "glIndexxOES")) && ok;
		ok = !!(gl.indexxvOES = GETFN_(proto::IndexxvOESFn, "glIndexxvOES")) && ok;
		ok = !!(gl.lightModelxOES = GETFN_(proto::LightModelxOESFn, "glLightModelxOES")) && ok;
		ok = !!(gl.lightModelxvOES = GETFN_(proto::LightModelxvOESFn, "glLightModelxvOES")) && ok;
		ok = !!(gl.lightxOES = GETFN_(proto::LightxOESFn, "glLightxOES")) && ok;
		ok = !!(gl.lightxvOES = GETFN_(proto::LightxvOESFn, "glLightxvOES")) && ok;
		ok = !!(gl.lineWidthxOES = GETFN_(proto::LineWidthxOESFn, "glLineWidthxOES")) && ok;
		ok = !!(gl.loadMatrixxOES = GETFN_(proto::LoadMatrixxOESFn, "glLoadMatrixxOES")) && ok;
		ok = !!(gl.loadTransposeMatrixxOES = GETFN_(proto::LoadTransposeMatrixxOESFn, "glLoadTransposeMatrixxOES")) && ok;
		ok = !!(gl.map1xOES = GETFN_(proto::Map1xOESFn, "glMap1xOES")) && ok;
		ok = !!(gl.map2xOES = GETFN_(proto::Map2xOESFn, "glMap2xOES")) && ok;
		ok = !!(gl.mapGrid1xOES = GETFN_(proto::MapGrid1xOESFn, "glMapGrid1xOES")) && ok;
		ok = !!(gl.mapGrid2xOES = GETFN_(proto::MapGrid2xOESFn, "glMapGrid2xOES")) && ok;
		ok = !!(gl.materialxOES = GETFN_(proto::MaterialxOESFn, "glMaterialxOES")) && ok;
		ok = !!(gl.materialxvOES = GETFN_(proto::MaterialxvOESFn, "glMaterialxvOES")) && ok;
		ok = !!(gl.multMatrixxOES = GETFN_(proto::MultMatrixxOESFn, "glMultMatrixxOES")) && ok;
		ok = !!(gl.multTransposeMatrixxOES = GETFN_(proto::MultTransposeMatrixxOESFn, "glMultTransposeMatrixxOES")) && ok;
		ok = !!(gl.multiTexCoord1xOES = GETFN_(proto::MultiTexCoord1xOESFn, "glMultiTexCoord1xOES")) && ok;
		ok = !!(gl.multiTexCoord1xvOES = GETFN_(proto::MultiTexCoord1xvOESFn, "glMultiTexCoord1xvOES")) && ok;
		ok = !!(gl.multiTexCoord2xOES = GETFN_(proto::MultiTexCoord2xOESFn, "glMultiTexCoord2xOES")) && ok;
		ok = !!(gl.multiTexCoord2xvOES = GETFN_(proto::MultiTexCoord2xvOESFn, "glMultiTexCoord2xvOES")) && ok;
		ok = !!(gl.multiTexCoord3xOES = GETFN_(proto::MultiTexCoord3xOESFn, "glMultiTexCoord3xOES")) && ok;
		ok = !!(gl.multiTexCoord3xvOES = GETFN_(proto::MultiTexCoord3xvOESFn, "glMultiTexCoord3xvOES")) && ok;
		ok = !!(gl.multiTexCoord4xOES = GETFN_(proto::MultiTexCoord4xOESFn, "glMultiTexCoord4xOES")) && ok;
		ok = !!(gl.multiTexCoord4xvOES = GETFN_(proto::MultiTexCoord4xvOESFn, "glMultiTexCoord4xvOES")) && ok;
		ok = !!(gl.normal3xOES = GETFN_(proto::Normal3xOESFn, "glNormal3xOES")) && ok;
		ok = !!(gl.normal3xvOES = GETFN_(proto::Normal3xvOESFn, "glNormal3xvOES")) && ok;
		ok = !!(gl.orthoxOES = GETFN_(proto::OrthoxOESFn, "glOrthoxOES")) && ok;
		ok = !!(gl.passThroughxOES = GETFN_(proto::PassThroughxOESFn, "glPassThroughxOES")) && ok;
		ok = !!(gl.pixelMapx = GETFN_(proto::PixelMapxFn, "glPixelMapx")) && ok;
		ok = !!(gl.pixelStorex = GETFN_(proto::PixelStorexFn, "glPixelStorex")) && ok;
		ok = !!(gl.pixelTransferxOES = GETFN_(proto::PixelTransferxOESFn, "glPixelTransferxOES")) && ok;
		ok = !!(gl.pixelZoomxOES = GETFN_(proto::PixelZoomxOESFn, "glPixelZoomxOES")) && ok;
		ok = !!(gl.pointParameterxvOES = GETFN_(proto::PointParameterxvOESFn, "glPointParameterxvOES")) && ok;
		ok = !!(gl.pointSizexOES = GETFN_(proto::PointSizexOESFn, "glPointSizexOES")) && ok;
		ok = !!(gl.polygonOffsetxOES = GETFN_(proto::PolygonOffsetxOESFn, "glPolygonOffsetxOES")) && ok;
		ok = !!(gl.prioritizeTexturesxOES = GETFN_(proto::PrioritizeTexturesxOESFn, "glPrioritizeTexturesxOES")) && ok;
		ok = !!(gl.rasterPos2xOES = GETFN_(proto::RasterPos2xOESFn, "glRasterPos2xOES")) && ok;
		ok = !!(gl.rasterPos2xvOES = GETFN_(proto::RasterPos2xvOESFn, "glRasterPos2xvOES")) && ok;
		ok = !!(gl.rasterPos3xOES = GETFN_(proto::RasterPos3xOESFn, "glRasterPos3xOES")) && ok;
		ok = !!(gl.rasterPos3xvOES = GETFN_(proto::RasterPos3xvOESFn, "glRasterPos3xvOES")) && ok;
		ok = !!(gl.rasterPos4xOES = GETFN_(proto::RasterPos4xOESFn, "glRasterPos4xOES")) && ok;
		ok = !!(gl.rasterPos4xvOES = GETFN_(proto::RasterPos4xvOESFn, "glRasterPos4xvOES")) && ok;
		ok = !!(gl.rectxOES = GETFN_(proto::RectxOESFn, "glRectxOES")) && ok;
		ok = !!(gl.rectxvOES = GETFN_(proto::RectxvOESFn, "glRectxvOES")) && ok;
		ok = !!(gl.rotatexOES = GETFN_(proto::RotatexOESFn, "glRotatexOES")) && ok;
		ok = !!(gl.scalexOES = GETFN_(proto::ScalexOESFn, "glScalexOES")) && ok;
		ok = !!(gl.texCoord1xOES = GETFN_(proto::TexCoord1xOESFn, "glTexCoord1xOES")) && ok;
		ok = !!(gl.texCoord1xvOES = GETFN_(proto::TexCoord1xvOESFn, "glTexCoord1xvOES")) && ok;
		ok = !!(gl.texCoord2xOES = GETFN_(proto::TexCoord2xOESFn, "glTexCoord2xOES")) && ok;
		ok = !!(gl.texCoord2xvOES = GETFN_(proto::TexCoord2xvOESFn, "glTexCoord2xvOES")) && ok;
		ok = !!(gl.texCoord3xOES = GETFN_(proto::TexCoord3xOESFn, "glTexCoord3xOES")) && ok;
		ok = !!(gl.texCoord3xvOES = GETFN_(proto::TexCoord3xvOESFn, "glTexCoord3xvOES")) && ok;
		ok = !!(gl.texCoord4xOES = GETFN_(proto::TexCoord4xOESFn, "glTexCoord4xOES")) && ok;
		ok = !!(gl.texCoord4xvOES = GETFN_(proto::TexCoord4xvOESFn, "glTexCoord4xvOES")) && ok;
		ok = !!(gl.texEnvxOES = GETFN_(proto::TexEnvxOESFn, "glTexEnvxOES")) && ok;
		ok = !!(gl.texEnvxvOES = GETFN_(proto::TexEnvxvOESFn, "glTexEnvxvOES")) && ok;
		ok = !!(gl.texGenxOES = GETFN_(proto::TexGenxOESFn, "glTexGenxOES")) && ok;
		ok = !!(gl.texGenxvOES = GETFN_(proto::TexGenxvOESFn, "glTexGenxvOES")) && ok;
		ok = !!(gl.texParameterxOES = GETFN_(proto::TexParameterxOESFn, "glTexParameterxOES")) && ok;
		ok = !!(gl.texParameterxvOES = GETFN_(proto::TexParameterxvOESFn, "glTexParameterxvOES")) && ok;
		ok = !!(gl.translatexOES = GETFN_(proto::TranslatexOESFn, "glTranslatexOES")) && ok;
		ok = !!(gl.vertex2xOES = GETFN_(proto::Vertex2xOESFn, "glVertex2xOES")) && ok;
		ok = !!(gl.vertex2xvOES = GETFN_(proto::Vertex2xvOESFn, "glVertex2xvOES")) && ok;
		ok = !!(gl.vertex3xOES = GETFN_(proto::Vertex3xOESFn, "glVertex3xOES")) && ok;
		ok = !!(gl.vertex3xvOES = GETFN_(proto::Vertex3xvOESFn, "glVertex3xvOES")) && ok;
		ok = !!(gl.vertex4xOES = GETFN_(proto::Vertex4xOESFn, "glVertex4xOES")) && ok;
		ok = !!(gl.vertex4xvOES = GETFN_(proto::Vertex4xvOESFn, "glVertex4xvOES")) && ok;
		gl.OES_fixed_point = ok;
	}
	//- end: OES_fixed_point

	//-//-- 
	//- Ext: OES_query_matrix
	static char const* OES_query_matrix = "OES_query_matrix";
	if( listed_( arg.extList, OES_query_matrix ) && arg.checkExt( OES_query_matrix, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.queryMatrixxOES = GETFN_(proto::QueryMatrixxOESFn, "glQueryMatrixxOES")) && ok;
		gl.OES_query_matrix = ok;
	}
	//- end: OES_query_matrix

	//-//-- 
	//- Ext: OES_single_precision
	static char const* OES_single_precision = "OES_single_precision";
	if( listed_( arg.extList, OES_single_precision ) && arg.checkExt( OES_single_precision, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.clearDepthfOES = GETFN_(proto::ClearDepthfOESFn, "glClearDepthfOES")) && ok;
		ok = !!(gl.clipPlanefOES = GETFN_(proto::ClipPlanefOESFn, "glClipPlanefOES")) && ok;
		ok = !!(gl.depthRangefOES = GETFN_(proto::DepthRangefOESFn, "glDepthRangefOES")) && ok;
		ok = !!(gl.frustumfOES = GETFN_(proto::FrustumfOESFn, "glFrustumfOES")) && ok;
		ok = !!(gl.getClipPlanefOES = GETFN_(proto::GetClipPlanefOESFn, "glGetClipPlanefOES")) && ok;
		ok = !!(gl.orthofOES = GETFN_(proto::OrthofOESFn, "glOrthofOES")) && ok;
		gl.OES_single_precision = ok;
	}
	//- end: OES_single_precision

	//-//-- 
	//- Ext: OVR_multiview
	static char const* OVR_multiview = "OVR_multiview";
	if( listed_( arg.extList, OVR_multiview ) && arg.checkExt( OVR_multiview, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.framebufferTextureMultiviewOVR = GETFN_(proto::FramebufferTextureMultiviewOVRFn, "glFramebufferTextureMultiviewOVR")) && ok;
		gl.OVR_multiview = ok;
	}
	//- end: OVR_multiview

	//-//-- 
	//- Ext: PGI_misc_hints
	static char const* PGI_misc_hints = "PGI_misc_hints";
	if( listed_( arg.extList, PGI_misc_hints ) && arg.checkExt( PGI_misc_hints, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.hintPGI = GETFN_(proto::HintPGIFn, "glHintPGI")) && ok;
		gl.PGI_misc_hints = ok;
	}
	//- end: PGI_misc_hints

	//-//-- 
	//- Ext: SGIS_detail_texture
	static char const* SGIS_detail_texture = "SGIS_detail_texture";
	if( listed_( arg.extList, SGIS_detail_texture ) && arg.checkExt( SGIS_detail_texture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.detailTexFuncSGIS = GETFN_(proto::DetailTexFuncSGISFn, "glDetailTexFuncSGIS")) && ok;
		ok = !!(gl.getDetailTexFuncSGIS = GETFN_(proto::GetDetailTexFuncSGISFn, "glGetDetailTexFuncSGIS")) && ok;
		gl.SGIS_detail_texture = ok;
	}
	//- end: SGIS_detail_texture

	//-//-- 
	//- Ext: SGIS_fog_function
	static char const* SGIS_fog_function = "SGIS_fog_function";
	if( listed_( arg.extList, SGIS_fog_function ) && arg.checkExt( SGIS_fog_function, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.fogFuncSGIS = GETFN_(proto::FogFuncSGISFn, "glFogFuncSGIS")) && ok;
		ok = !!(gl.getFogFuncSGIS = GETFN_(proto::GetFogFuncSGISFn, "glGetFogFuncSGIS")) && ok;
		gl.SGIS_fog_function = ok;
	}
	//- end: SGIS_fog_function

	//-//-- 
	//- Ext: SGIS_multisample
	static char const* SGIS_multisample = "SGIS_multisample";
	if( listed_( arg.extList, SGIS_multisample ) && arg.checkExt( SGIS_multisample, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.sampleMaskSGIS = GETFN_(proto::SampleMaskSGISFn, "glSampleMaskSGIS")) && ok;
		ok = !!(gl.samplePatternSGIS = GETFN_(proto::SamplePatternSGISFn, "glSamplePatternSGIS")) && ok;
		gl.SGIS_multisample = ok;
	}
	//- end: SGIS_multisample

	//-//-- 
	//- Ext: SGIS_pixel_texture
	static char const* SGIS_pixel_texture = "SGIS_pixel_texture";
	if( listed_( arg.extList, SGIS_pixel_texture ) && arg.checkExt( SGIS_pixel_texture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getPixelTexGenParameterfvSGIS = GETFN_(proto::GetPixelTexGenParameterfvSGISFn, "glGetPixelTexGenParameterfvSGIS")) && ok;
		ok = !!(gl.getPixelTexGenParameterivSGIS = GETFN_(proto::GetPixelTexGenParameterivSGISFn, "glGetPixelTexGenParameterivSGIS")) && ok;
		ok = !!(gl.pixelTexGenParameterfSGIS = GETFN_(proto::PixelTexGenParameterfSGISFn, "glPixelTexGenParameterfSGIS")) && ok;
		ok = !!(gl.pixelTexGenParameterfvSGIS = GETFN_(proto::PixelTexGenParameterfvSGISFn, "glPixelTexGenParameterfvSGIS")) && ok;
		ok = !!(gl.pixelTexGenParameteriSGIS = GETFN_(proto::PixelTexGenParameteriSGISFn, "glPixelTexGenParameteriSGIS")) && ok;
		ok = !!(gl.pixelTexGenParameterivSGIS = GETFN_(proto::PixelTexGenParameterivSGISFn, "glPixelTexGenParameterivSGIS")) && ok;
		gl.SGIS_pixel_texture = ok;
	}
	//- end: SGIS_pixel_texture

	//-//-- 
	//- Ext: SGIS_point_parameters
	static char const* SGIS_point_parameters = "SGIS_point_parameters";
	if( listed_( arg.extList, SGIS_point_parameters ) && arg.checkExt( SGIS_point_parameters, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.pointParameterfSGIS = GETFN_(proto::PointParameterfSGISFn, "glPointParameterfSGIS")) && ok;
		ok = !!(gl.pointParameterfvSGIS = GETFN_(proto::PointParameterfvSGISFn, "glPointParameterfvSGIS")) && ok;
		gl.SGIS_point_parameters = ok;
	}
	//- end: SGIS_point_parameters

	//-//-- 
	//- Ext: SGIS_sharpen_texture
	static char const* SGIS_sharpen_texture = "SGIS_sharpen_texture";
	if( listed_( arg.extList, SGIS_sharpen_texture ) && arg.checkExt( SGIS_sharpen_texture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getSharpenTexFuncSGIS = GETFN_(proto::GetSharpenTexFuncSGISFn, "glGetSharpenTexFuncSGIS")) && ok;
		ok = !!(gl.sharpenTexFuncSGIS = GETFN_(proto::SharpenTexFuncSGISFn, "glSharpenTexFuncSGIS")) && ok;
		gl.SGIS_sharpen_texture = ok;
	}
	//- end: SGIS_sharpen_texture

	//-//-- 
	//- Ext: SGIS_texture4D
	static char const* SGIS_texture4D = "SGIS_texture4D";
	if( listed_( arg.extList, SGIS_texture4D ) && arg.checkExt( SGIS_texture4D, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.texImage4DSGIS = GETFN_(proto::TexImage4DSGISFn, "glTexImage4DSGIS")) && ok;
		ok = !!(gl.texSubImage4DSGIS = GETFN_(proto::TexSubImage4DSGISFn, "glTexSubImage4DSGIS")) && ok;
		gl.SGIS_texture4D = ok;
	}
	//- end: SGIS_texture4D

	//-//-- 
	//- Ext: SGIS_texture_color_mask
	static char const* SGIS_texture_color_mask = "SGIS_texture_color_mask";
	if( listed_( arg.extList, SGIS_texture_color_mask ) && arg.checkExt( SGIS_texture_color_mask, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.textureColorMaskSGIS = GETFN_(proto::TextureColorMaskSGISFn, "glTextureColorMaskSGIS")) && ok;
		gl.SGIS_texture_color_mask = ok;
	}
	//- end: SGIS_texture_color_mask

	//-//-- 
	//- Ext: SGIS_texture_filter4
	static char const* SGIS_texture_filter4 = "SGIS_texture_filter4";
	if( listed_( arg.extList, SGIS_texture_filter4 ) && arg.checkExt( SGIS_texture_filter4, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getTexFilterFuncSGIS = GETFN_(proto::GetTexFilterFuncSGISFn, "glGetTexFilterFuncSGIS")) && ok;
		ok = !!(gl.texFilterFuncSGIS = GETFN_(proto::TexFilterFuncSGISFn, "glTexFilterFuncSGIS")) && ok;
		gl.SGIS_texture_filter4 = ok;
	}
	//- end: SGIS_texture_filter4

	//-//-- 
	//- Ext: SGIX_async
	static char const* SGIX_async = "SGIX_async";
	if( listed_( arg.extList, SGIX_async ) && arg.checkExt( SGIX_async, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.asyncMarkerSGIX = GETFN_(proto::AsyncMarkerSGIXFn, "glAsyncMarkerSGIX")) && ok;
		ok = !!(gl.deleteAsyncMarkersSGIX = GETFN_(proto::DeleteAsyncMarkersSGIXFn, "glDeleteAsyncMarkersSGIX")) && ok;
		ok = !!(gl.finishAsyncSGIX = GETFN_(proto::FinishAsyncSGIXFn, "glFinishAsyncSGIX")) && ok;
		ok = !!(gl.genAsyncMarkersSGIX = GETFN_(proto::GenAsyncMarkersSGIXFn, "glGenAsyncMarkersSGIX")) && ok;
		ok = !!(gl.isAsyncMarkerSGIX = GETFN_(proto::IsAsyncMarkerSGIXFn, "glIsAsyncMarkerSGIX")) && ok;
		ok = !!(gl.pollAsyncSGIX = GETFN_(proto::PollAsyncSGIXFn, "glPollAsyncSGIX")) && ok;
		gl.SGIX_async = ok;
	}
	//- end: SGIX_async

	//-//-- 
	//- Ext: SGIX_flush_raster
	static char const* SGIX_flush_raster = "SGIX_flush_raster";
	if( listed_( arg.extList, SGIX_flush_raster ) && arg.checkExt( SGIX_flush_raster, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.flushRasterSGIX = GETFN_(proto::FlushRasterSGIXFn, "glFlushRasterSGIX")) && ok;
		gl.SGIX_flush_raster = ok;
	}
	//- end: SGIX_flush_raster

	//-//-- 
	//- Ext: SGIX_fragment_lighting
	static char const* SGIX_fragment_lighting = "SGIX_fragment_lighting";
	if( listed_( arg.extList, SGIX_fragment_lighting ) && arg.checkExt( SGIX_fragment_lighting, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.fragmentColorMaterialSGIX = GETFN_(proto::FragmentColorMaterialSGIXFn, "glFragmentColorMaterialSGIX")) && ok;
		ok = !!(gl.fragmentLightModelfSGIX = GETFN_(proto::FragmentLightModelfSGIXFn, "glFragmentLightModelfSGIX")) && ok;
		ok = !!(gl.fragmentLightModelfvSGIX = GETFN_(proto::FragmentLightModelfvSGIXFn, "glFragmentLightModelfvSGIX")) && ok;
		ok = !!(gl.fragmentLightModeliSGIX = GETFN_(proto::FragmentLightModeliSGIXFn, "glFragmentLightModeliSGIX")) && ok;
		ok = !!(gl.fragmentLightModelivSGIX = GETFN_(proto::FragmentLightModelivSGIXFn, "glFragmentLightModelivSGIX")) && ok;
		ok = !!(gl.fragmentLightfSGIX = GETFN_(proto::FragmentLightfSGIXFn, "glFragmentLightfSGIX")) && ok;
		ok = !!(gl.fragmentLightfvSGIX = GETFN_(proto::FragmentLightfvSGIXFn, "glFragmentLightfvSGIX")) && ok;
		ok = !!(gl.fragmentLightiSGIX = GETFN_(proto::FragmentLightiSGIXFn, "glFragmentLightiSGIX")) && ok;
		ok = !!(gl.fragmentLightivSGIX = GETFN_(proto::FragmentLightivSGIXFn, "glFragmentLightivSGIX")) && ok;
		ok = !!(gl.fragmentMaterialfSGIX = GETFN_(proto::FragmentMaterialfSGIXFn, "glFragmentMaterialfSGIX")) && ok;
		ok = !!(gl.fragmentMaterialfvSGIX = GETFN_(proto::FragmentMaterialfvSGIXFn, "glFragmentMaterialfvSGIX")) && ok;
		ok = !!(gl.fragmentMaterialiSGIX = GETFN_(proto::FragmentMaterialiSGIXFn, "glFragmentMaterialiSGIX")) && ok;
		ok = !!(gl.fragmentMaterialivSGIX = GETFN_(proto::FragmentMaterialivSGIXFn, "glFragmentMaterialivSGIX")) && ok;
		ok = !!(gl.getFragmentLightfvSGIX = GETFN_(proto::GetFragmentLightfvSGIXFn, "glGetFragmentLightfvSGIX")) && ok;
		ok = !!(gl.getFragmentLightivSGIX = GETFN_(proto::GetFragmentLightivSGIXFn, "glGetFragmentLightivSGIX")) && ok;
		ok = !!(gl.getFragmentMaterialfvSGIX = GETFN_(proto::GetFragmentMaterialfvSGIXFn, "glGetFragmentMaterialfvSGIX")) && ok;
		ok = !!(gl.getFragmentMaterialivSGIX = GETFN_(proto::GetFragmentMaterialivSGIXFn, "glGetFragmentMaterialivSGIX")) && ok;
		ok = !!(gl.lightEnviSGIX = GETFN_(proto::LightEnviSGIXFn, "glLightEnviSGIX")) && ok;
		gl.SGIX_fragment_lighting = ok;
	}
	//- end: SGIX_fragment_lighting

	//-//-- 
	//- Ext: SGIX_framezoom
	static char const* SGIX_framezoom = "SGIX_framezoom";
	if( listed_( arg.extList, SGIX_framezoom ) && arg.checkExt( SGIX_framezoom, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.frameZoomSGIX = GETFN_(proto::FrameZoomSGIXFn, "glFrameZoomSGIX")) && ok;
		gl.SGIX_framezoom = ok;
	}
	//- end: SGIX_framezoom

	//-//-- 
	//- Ext: SGIX_igloo_interface
	static char const* SGIX_igloo_interface = "SGIX_igloo_interface";
	if( listed_( arg.extList, SGIX_igloo_interface ) && arg.checkExt( SGIX_igloo_interface, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.iglooInterfaceSGIX = GETFN_(proto::IglooInterfaceSGIXFn, "glIglooInterfaceSGIX")) && ok;
		gl.SGIX_igloo_interface = ok;
	}
	//- end: SGIX_igloo_interface

	//-//-- 
	//- Ext: SGIX_instruments
	static char const* SGIX_instruments = "SGIX_instruments";
	if( listed_( arg.extList, SGIX_instruments ) && arg.checkExt( SGIX_instruments, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getInstrumentsSGIX = GETFN_(proto::GetInstrumentsSGIXFn, "glGetInstrumentsSGIX")) && ok;
		ok = !!(gl.instrumentsBufferSGIX = GETFN_(proto::InstrumentsBufferSGIXFn, "glInstrumentsBufferSGIX")) && ok;
		ok = !!(gl.pollInstrumentsSGIX = GETFN_(proto::PollInstrumentsSGIXFn, "glPollInstrumentsSGIX")) && ok;
		ok = !!(gl.readInstrumentsSGIX = GETFN_(proto::ReadInstrumentsSGIXFn, "glReadInstrumentsSGIX")) && ok;
		ok = !!(gl.startInstrumentsSGIX = GETFN_(proto::StartInstrumentsSGIXFn, "glStartInstrumentsSGIX")) && ok;
		ok = !!(gl.stopInstrumentsSGIX = GETFN_(proto::StopInstrumentsSGIXFn, "glStopInstrumentsSGIX")) && ok;
		gl.SGIX_instruments = ok;
	}
	//- end: SGIX_instruments

	//-//-- 
	//- Ext: SGIX_list_priority
	static char const* SGIX_list_priority = "SGIX_list_priority";
	if( listed_( arg.extList, SGIX_list_priority ) && arg.checkExt( SGIX_list_priority, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getListParameterfvSGIX = GETFN_(proto::GetListParameterfvSGIXFn, "glGetListParameterfvSGIX")) && ok;
		ok = !!(gl.getListParameterivSGIX = GETFN_(proto::GetListParameterivSGIXFn, "glGetListParameterivSGIX")) && ok;
		ok = !!(gl.listParameterfSGIX = GETFN_(proto::ListParameterfSGIXFn, "glListParameterfSGIX")) && ok;
		ok = !!(gl.listParameterfvSGIX = GETFN_(proto::ListParameterfvSGIXFn, "glListParameterfvSGIX")) && ok;
		ok = !!(gl.listParameteriSGIX = GETFN_(proto::ListParameteriSGIXFn, "glListParameteriSGIX")) && ok;
		ok = !!(gl.listParameterivSGIX = GETFN_(proto::ListParameterivSGIXFn, "glListParameterivSGIX")) && ok;
		gl.SGIX_list_priority = ok;
	}
	//- end: SGIX_list_priority

	//-//-- 
	//- Ext: SGIX_pixel_texture
	static char const* SGIX_pixel_texture = "SGIX_pixel_texture";
	if( listed_( arg.extList, SGIX_pixel_texture ) && arg.checkExt( SGIX_pixel_texture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.pixelTexGenSGIX = GETFN_(proto::PixelTexGenSGIXFn, "glPixelTexGenSGIX")) && ok;
		gl.SGIX_pixel_texture = ok;
	}
	//- end: SGIX_pixel_texture

	//-//-- 
	//- Ext: SGIX_polynomial_ffd
	static char const* SGIX_polynomial_ffd = "SGIX_polynomial_ffd";
	if( listed_( arg.extList, SGIX_polynomial_ffd ) && arg.checkExt( SGIX_polynomial_ffd, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.deformSGIX = GETFN_(proto::DeformSGIXFn, "glDeformSGIX")) && ok;
		ok = !!(gl.deformationMap3dSGIX = GETFN_(proto::DeformationMap3dSGIXFn, "glDeformationMap3dSGIX")) && ok;
		ok = !!(gl.deformationMap3fSGIX = GETFN_(proto::DeformationMap3fSGIXFn, "glDeformationMap3fSGIX")) && ok;
		ok = !!(gl.loadIdentityDeformationMapSGIX = GETFN_(proto::LoadIdentityDeformationMapSGIXFn, "glLoadIdentityDeformationMapSGIX")) && ok;
		gl.SGIX_polynomial_ffd = ok;
	}
	//- end: SGIX_polynomial_ffd

	//-//-- 
	//- Ext: SGIX_reference_plane
	static char const* SGIX_reference_plane = "SGIX_reference_plane";
	if( listed_( arg.extList, SGIX_reference_plane ) && arg.checkExt( SGIX_reference_plane, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.referencePlaneSGIX = GETFN_(proto::ReferencePlaneSGIXFn, "glReferencePlaneSGIX")) && ok;
		gl.SGIX_reference_plane = ok;
	}
	//- end: SGIX_reference_plane

	//-//-- 
	//- Ext: SGIX_sprite
	static char const* SGIX_sprite = "SGIX_sprite";
	if( listed_( arg.extList, SGIX_sprite ) && arg.checkExt( SGIX_sprite, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.spriteParameterfSGIX = GETFN_(proto::SpriteParameterfSGIXFn, "glSpriteParameterfSGIX")) && ok;
		ok = !!(gl.spriteParameterfvSGIX = GETFN_(proto::SpriteParameterfvSGIXFn, "glSpriteParameterfvSGIX")) && ok;
		ok = !!(gl.spriteParameteriSGIX = GETFN_(proto::SpriteParameteriSGIXFn, "glSpriteParameteriSGIX")) && ok;
		ok = !!(gl.spriteParameterivSGIX = GETFN_(proto::SpriteParameterivSGIXFn, "glSpriteParameterivSGIX")) && ok;
		gl.SGIX_sprite = ok;
	}
	//- end: SGIX_sprite

	//-//-- 
	//- Ext: SGIX_tag_sample_buffer
	static char const* SGIX_tag_sample_buffer = "SGIX_tag_sample_buffer";
	if( listed_( arg.extList, SGIX_tag_sample_buffer ) && arg.checkExt( SGIX_tag_sample_buffer, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.tagSampleBufferSGIX = GETFN_(proto::TagSampleBufferSGIXFn, "glTagSampleBufferSGIX")) && ok;
		gl.SGIX_tag_sample_buffer = ok;
	}
	//- end: SGIX_tag_sample_buffer

	//-//-- 
	//- Ext: SGI_color_table
	static char const* SGI_color_table = "SGI_color_table";
	if( listed_( arg.extList, SGI_color_table ) && arg.checkExt( SGI_color_table, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.colorTableParameterfvSGI = GETFN_(proto::ColorTableParameterfvSGIFn, "glColorTableParameterfvSGI")) && ok;
		ok = !!(gl.colorTableParameterivSGI = GETFN_(proto::ColorTableParameterivSGIFn, "glColorTableParameterivSGI")) && ok;
		ok = !!(gl.colorTableSGI = GETFN_(proto::ColorTableSGIFn, "glColorTableSGI")) && ok;
		ok = !!(gl.copyColorTableSGI = GETFN_(proto::CopyColorTableSGIFn, "glCopyColorTableSGI")) && ok;
		ok = !!(gl.getColorTableParameterfvSGI = GETFN_(proto::GetColorTableParameterfvSGIFn, "glGetColorTableParameterfvSGI")) && ok;
		ok = !!(gl.getColorTableParameterivSGI = GETFN_(proto::GetColorTableParameterivSGIFn, "glGetColorTableParameterivSGI")) && ok;
		ok = !!(gl.getColorTableSGI = GETFN_(proto::GetColorTableSGIFn, "glGetColorTableSGI")) && ok;
		gl.SGI_color_table = ok;
	}
	//- end: SGI_color_table

}

#undef GETFN_
//--    <<< ~ dlapi::gl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE3(dlapi,gl,detail)
//--//////////////////////////////////////////////////////////////////////////
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
