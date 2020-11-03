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

void load_gl_nv_( GLapi& gl, LoaderArg_ const& arg )
{
	//-//-- 
	//- Ext: NVX_conditional_render
	static char const* NVX_conditional_render = "NVX_conditional_render";
	if( listed_( arg.extList, NVX_conditional_render ) && arg.checkExt( NVX_conditional_render, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.beginConditionalRenderNVX = GETFN_(proto::BeginConditionalRenderNVXFn, "glBeginConditionalRenderNVX")) && ok;
		ok = !!(gl.endConditionalRenderNVX = GETFN_(proto::EndConditionalRenderNVXFn, "glEndConditionalRenderNVX")) && ok;
		gl.NVX_conditional_render = ok;
	}
	//- end: NVX_conditional_render

	//-//-- 
	//- Ext: NV_bindless_multi_draw_indirect
	static char const* NV_bindless_multi_draw_indirect = "NV_bindless_multi_draw_indirect";
	if( listed_( arg.extList, NV_bindless_multi_draw_indirect ) && arg.checkExt( NV_bindless_multi_draw_indirect, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.multiDrawArraysIndirectBindlessNV = GETFN_(proto::MultiDrawArraysIndirectBindlessNVFn, "glMultiDrawArraysIndirectBindlessNV")) && ok;
		ok = !!(gl.multiDrawElementsIndirectBindlessNV = GETFN_(proto::MultiDrawElementsIndirectBindlessNVFn, "glMultiDrawElementsIndirectBindlessNV")) && ok;
		gl.NV_bindless_multi_draw_indirect = ok;
	}
	//- end: NV_bindless_multi_draw_indirect

	//-//-- 
	//- Ext: NV_bindless_multi_draw_indirect_count
	static char const* NV_bindless_multi_draw_indirect_count = "NV_bindless_multi_draw_indirect_count";
	if( listed_( arg.extList, NV_bindless_multi_draw_indirect_count ) && arg.checkExt( NV_bindless_multi_draw_indirect_count, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.multiDrawArraysIndirectBindlessCountNV = GETFN_(proto::MultiDrawArraysIndirectBindlessCountNVFn, "glMultiDrawArraysIndirectBindlessCountNV")) && ok;
		ok = !!(gl.multiDrawElementsIndirectBindlessCountNV = GETFN_(proto::MultiDrawElementsIndirectBindlessCountNVFn, "glMultiDrawElementsIndirectBindlessCountNV")) && ok;
		gl.NV_bindless_multi_draw_indirect_count = ok;
	}
	//- end: NV_bindless_multi_draw_indirect_count

	//-//-- 
	//- Ext: NV_bindless_texture
	static char const* NV_bindless_texture = "NV_bindless_texture";
	if( listed_( arg.extList, NV_bindless_texture ) && arg.checkExt( NV_bindless_texture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getImageHandleNV = GETFN_(proto::GetImageHandleNVFn, "glGetImageHandleNV")) && ok;
		ok = !!(gl.getTextureHandleNV = GETFN_(proto::GetTextureHandleNVFn, "glGetTextureHandleNV")) && ok;
		ok = !!(gl.getTextureSamplerHandleNV = GETFN_(proto::GetTextureSamplerHandleNVFn, "glGetTextureSamplerHandleNV")) && ok;
		ok = !!(gl.isImageHandleResidentNV = GETFN_(proto::IsImageHandleResidentNVFn, "glIsImageHandleResidentNV")) && ok;
		ok = !!(gl.isTextureHandleResidentNV = GETFN_(proto::IsTextureHandleResidentNVFn, "glIsTextureHandleResidentNV")) && ok;
		ok = !!(gl.makeImageHandleNonResidentNV = GETFN_(proto::MakeImageHandleNonResidentNVFn, "glMakeImageHandleNonResidentNV")) && ok;
		ok = !!(gl.makeImageHandleResidentNV = GETFN_(proto::MakeImageHandleResidentNVFn, "glMakeImageHandleResidentNV")) && ok;
		ok = !!(gl.makeTextureHandleNonResidentNV = GETFN_(proto::MakeTextureHandleNonResidentNVFn, "glMakeTextureHandleNonResidentNV")) && ok;
		ok = !!(gl.makeTextureHandleResidentNV = GETFN_(proto::MakeTextureHandleResidentNVFn, "glMakeTextureHandleResidentNV")) && ok;
		ok = !!(gl.programUniformHandleui64NV = GETFN_(proto::ProgramUniformHandleui64NVFn, "glProgramUniformHandleui64NV")) && ok;
		ok = !!(gl.programUniformHandleui64vNV = GETFN_(proto::ProgramUniformHandleui64vNVFn, "glProgramUniformHandleui64vNV")) && ok;
		ok = !!(gl.uniformHandleui64NV = GETFN_(proto::UniformHandleui64NVFn, "glUniformHandleui64NV")) && ok;
		ok = !!(gl.uniformHandleui64vNV = GETFN_(proto::UniformHandleui64vNVFn, "glUniformHandleui64vNV")) && ok;
		gl.NV_bindless_texture = ok;
	}
	//- end: NV_bindless_texture

	//-//-- 
	//- Ext: NV_blend_equation_advanced
	static char const* NV_blend_equation_advanced = "NV_blend_equation_advanced";
	if( listed_( arg.extList, NV_blend_equation_advanced ) && arg.checkExt( NV_blend_equation_advanced, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.blendBarrierNV = GETFN_(proto::BlendBarrierNVFn, "glBlendBarrierNV")) && ok;
		ok = !!(gl.blendParameteriNV = GETFN_(proto::BlendParameteriNVFn, "glBlendParameteriNV")) && ok;
		gl.NV_blend_equation_advanced = ok;
	}
	//- end: NV_blend_equation_advanced

	//-//-- 
	//- Ext: NV_command_list
	static char const* NV_command_list = "NV_command_list";
	if( listed_( arg.extList, NV_command_list ) && arg.checkExt( NV_command_list, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.callCommandListNV = GETFN_(proto::CallCommandListNVFn, "glCallCommandListNV")) && ok;
		ok = !!(gl.commandListSegmentsNV = GETFN_(proto::CommandListSegmentsNVFn, "glCommandListSegmentsNV")) && ok;
		ok = !!(gl.compileCommandListNV = GETFN_(proto::CompileCommandListNVFn, "glCompileCommandListNV")) && ok;
		ok = !!(gl.createCommandListsNV = GETFN_(proto::CreateCommandListsNVFn, "glCreateCommandListsNV")) && ok;
		ok = !!(gl.createStatesNV = GETFN_(proto::CreateStatesNVFn, "glCreateStatesNV")) && ok;
		ok = !!(gl.deleteCommandListsNV = GETFN_(proto::DeleteCommandListsNVFn, "glDeleteCommandListsNV")) && ok;
		ok = !!(gl.deleteStatesNV = GETFN_(proto::DeleteStatesNVFn, "glDeleteStatesNV")) && ok;
		ok = !!(gl.drawCommandsAddressNV = GETFN_(proto::DrawCommandsAddressNVFn, "glDrawCommandsAddressNV")) && ok;
		ok = !!(gl.drawCommandsNV = GETFN_(proto::DrawCommandsNVFn, "glDrawCommandsNV")) && ok;
		ok = !!(gl.drawCommandsStatesAddressNV = GETFN_(proto::DrawCommandsStatesAddressNVFn, "glDrawCommandsStatesAddressNV")) && ok;
		ok = !!(gl.drawCommandsStatesNV = GETFN_(proto::DrawCommandsStatesNVFn, "glDrawCommandsStatesNV")) && ok;
		ok = !!(gl.getCommandHeaderNV = GETFN_(proto::GetCommandHeaderNVFn, "glGetCommandHeaderNV")) && ok;
		ok = !!(gl.getStageIndexNV = GETFN_(proto::GetStageIndexNVFn, "glGetStageIndexNV")) && ok;
		ok = !!(gl.isCommandListNV = GETFN_(proto::IsCommandListNVFn, "glIsCommandListNV")) && ok;
		ok = !!(gl.isStateNV = GETFN_(proto::IsStateNVFn, "glIsStateNV")) && ok;
		ok = !!(gl.listDrawCommandsStatesClientNV = GETFN_(proto::ListDrawCommandsStatesClientNVFn, "glListDrawCommandsStatesClientNV")) && ok;
		ok = !!(gl.stateCaptureNV = GETFN_(proto::StateCaptureNVFn, "glStateCaptureNV")) && ok;
		gl.NV_command_list = ok;
	}
	//- end: NV_command_list

	//-//-- 
	//- Ext: NV_conditional_render
	static char const* NV_conditional_render = "NV_conditional_render";
	if( listed_( arg.extList, NV_conditional_render ) && arg.checkExt( NV_conditional_render, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.beginConditionalRenderNV = GETFN_(proto::BeginConditionalRenderNVFn, "glBeginConditionalRenderNV")) && ok;
		ok = !!(gl.endConditionalRenderNV = GETFN_(proto::EndConditionalRenderNVFn, "glEndConditionalRenderNV")) && ok;
		gl.NV_conditional_render = ok;
	}
	//- end: NV_conditional_render

	//-//-- 
	//- Ext: NV_conservative_raster
	static char const* NV_conservative_raster = "NV_conservative_raster";
	if( listed_( arg.extList, NV_conservative_raster ) && arg.checkExt( NV_conservative_raster, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.subpixelPrecisionBiasNV = GETFN_(proto::SubpixelPrecisionBiasNVFn, "glSubpixelPrecisionBiasNV")) && ok;
		gl.NV_conservative_raster = ok;
	}
	//- end: NV_conservative_raster

	//-//-- 
	//- Ext: NV_conservative_raster_dilate
	static char const* NV_conservative_raster_dilate = "NV_conservative_raster_dilate";
	if( listed_( arg.extList, NV_conservative_raster_dilate ) && arg.checkExt( NV_conservative_raster_dilate, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.conservativeRasterParameterfNV = GETFN_(proto::ConservativeRasterParameterfNVFn, "glConservativeRasterParameterfNV")) && ok;
		gl.NV_conservative_raster_dilate = ok;
	}
	//- end: NV_conservative_raster_dilate

	//-//-- 
	//- Ext: NV_copy_image
	static char const* NV_copy_image = "NV_copy_image";
	if( listed_( arg.extList, NV_copy_image ) && arg.checkExt( NV_copy_image, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.copyImageSubDataNV = GETFN_(proto::CopyImageSubDataNVFn, "glCopyImageSubDataNV")) && ok;
		gl.NV_copy_image = ok;
	}
	//- end: NV_copy_image

	//-//-- 
	//- Ext: NV_depth_buffer_float
	static char const* NV_depth_buffer_float = "NV_depth_buffer_float";
	if( listed_( arg.extList, NV_depth_buffer_float ) && arg.checkExt( NV_depth_buffer_float, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.clearDepthdNV = GETFN_(proto::ClearDepthdNVFn, "glClearDepthdNV")) && ok;
		ok = !!(gl.depthBoundsdNV = GETFN_(proto::DepthBoundsdNVFn, "glDepthBoundsdNV")) && ok;
		ok = !!(gl.depthRangedNV = GETFN_(proto::DepthRangedNVFn, "glDepthRangedNV")) && ok;
		gl.NV_depth_buffer_float = ok;
	}
	//- end: NV_depth_buffer_float

	//-//-- 
	//- Ext: NV_draw_texture
	static char const* NV_draw_texture = "NV_draw_texture";
	if( listed_( arg.extList, NV_draw_texture ) && arg.checkExt( NV_draw_texture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.drawTextureNV = GETFN_(proto::DrawTextureNVFn, "glDrawTextureNV")) && ok;
		gl.NV_draw_texture = ok;
	}
	//- end: NV_draw_texture

	//-//-- 
	//- Ext: NV_evaluators
	static char const* NV_evaluators = "NV_evaluators";
	if( listed_( arg.extList, NV_evaluators ) && arg.checkExt( NV_evaluators, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.evalMapsNV = GETFN_(proto::EvalMapsNVFn, "glEvalMapsNV")) && ok;
		ok = !!(gl.getMapAttribParameterfvNV = GETFN_(proto::GetMapAttribParameterfvNVFn, "glGetMapAttribParameterfvNV")) && ok;
		ok = !!(gl.getMapAttribParameterivNV = GETFN_(proto::GetMapAttribParameterivNVFn, "glGetMapAttribParameterivNV")) && ok;
		ok = !!(gl.getMapControlPointsNV = GETFN_(proto::GetMapControlPointsNVFn, "glGetMapControlPointsNV")) && ok;
		ok = !!(gl.getMapParameterfvNV = GETFN_(proto::GetMapParameterfvNVFn, "glGetMapParameterfvNV")) && ok;
		ok = !!(gl.getMapParameterivNV = GETFN_(proto::GetMapParameterivNVFn, "glGetMapParameterivNV")) && ok;
		ok = !!(gl.mapControlPointsNV = GETFN_(proto::MapControlPointsNVFn, "glMapControlPointsNV")) && ok;
		ok = !!(gl.mapParameterfvNV = GETFN_(proto::MapParameterfvNVFn, "glMapParameterfvNV")) && ok;
		ok = !!(gl.mapParameterivNV = GETFN_(proto::MapParameterivNVFn, "glMapParameterivNV")) && ok;
		gl.NV_evaluators = ok;
	}
	//- end: NV_evaluators

	//-//-- 
	//- Ext: NV_explicit_multisample
	static char const* NV_explicit_multisample = "NV_explicit_multisample";
	if( listed_( arg.extList, NV_explicit_multisample ) && arg.checkExt( NV_explicit_multisample, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getMultisamplefvNV = GETFN_(proto::GetMultisamplefvNVFn, "glGetMultisamplefvNV")) && ok;
		ok = !!(gl.sampleMaskIndexedNV = GETFN_(proto::SampleMaskIndexedNVFn, "glSampleMaskIndexedNV")) && ok;
		ok = !!(gl.texRenderbufferNV = GETFN_(proto::TexRenderbufferNVFn, "glTexRenderbufferNV")) && ok;
		gl.NV_explicit_multisample = ok;
	}
	//- end: NV_explicit_multisample

	//-//-- 
	//- Ext: NV_fence
	static char const* NV_fence = "NV_fence";
	if( listed_( arg.extList, NV_fence ) && arg.checkExt( NV_fence, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.deleteFencesNV = GETFN_(proto::DeleteFencesNVFn, "glDeleteFencesNV")) && ok;
		ok = !!(gl.finishFenceNV = GETFN_(proto::FinishFenceNVFn, "glFinishFenceNV")) && ok;
		ok = !!(gl.genFencesNV = GETFN_(proto::GenFencesNVFn, "glGenFencesNV")) && ok;
		ok = !!(gl.getFenceivNV = GETFN_(proto::GetFenceivNVFn, "glGetFenceivNV")) && ok;
		ok = !!(gl.isFenceNV = GETFN_(proto::IsFenceNVFn, "glIsFenceNV")) && ok;
		ok = !!(gl.setFenceNV = GETFN_(proto::SetFenceNVFn, "glSetFenceNV")) && ok;
		ok = !!(gl.testFenceNV = GETFN_(proto::TestFenceNVFn, "glTestFenceNV")) && ok;
		gl.NV_fence = ok;
	}
	//- end: NV_fence

	//-//-- 
	//- Ext: NV_fragment_coverage_to_color
	static char const* NV_fragment_coverage_to_color = "NV_fragment_coverage_to_color";
	if( listed_( arg.extList, NV_fragment_coverage_to_color ) && arg.checkExt( NV_fragment_coverage_to_color, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.fragmentCoverageColorNV = GETFN_(proto::FragmentCoverageColorNVFn, "glFragmentCoverageColorNV")) && ok;
		gl.NV_fragment_coverage_to_color = ok;
	}
	//- end: NV_fragment_coverage_to_color

	//-//-- 
	//- Ext: NV_fragment_program
	static char const* NV_fragment_program = "NV_fragment_program";
	if( listed_( arg.extList, NV_fragment_program ) && arg.checkExt( NV_fragment_program, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getProgramNamedParameterdvNV = GETFN_(proto::GetProgramNamedParameterdvNVFn, "glGetProgramNamedParameterdvNV")) && ok;
		ok = !!(gl.getProgramNamedParameterfvNV = GETFN_(proto::GetProgramNamedParameterfvNVFn, "glGetProgramNamedParameterfvNV")) && ok;
		ok = !!(gl.programNamedParameter4dNV = GETFN_(proto::ProgramNamedParameter4dNVFn, "glProgramNamedParameter4dNV")) && ok;
		ok = !!(gl.programNamedParameter4dvNV = GETFN_(proto::ProgramNamedParameter4dvNVFn, "glProgramNamedParameter4dvNV")) && ok;
		ok = !!(gl.programNamedParameter4fNV = GETFN_(proto::ProgramNamedParameter4fNVFn, "glProgramNamedParameter4fNV")) && ok;
		ok = !!(gl.programNamedParameter4fvNV = GETFN_(proto::ProgramNamedParameter4fvNVFn, "glProgramNamedParameter4fvNV")) && ok;
		gl.NV_fragment_program = ok;
	}
	//- end: NV_fragment_program

	//-//-- 
	//- Ext: NV_framebuffer_mixed_samples
	static char const* NV_framebuffer_mixed_samples = "NV_framebuffer_mixed_samples";
	if( listed_( arg.extList, NV_framebuffer_mixed_samples ) && arg.checkExt( NV_framebuffer_mixed_samples, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.coverageModulationNV = GETFN_(proto::CoverageModulationNVFn, "glCoverageModulationNV")) && ok;
		ok = !!(gl.coverageModulationTableNV = GETFN_(proto::CoverageModulationTableNVFn, "glCoverageModulationTableNV")) && ok;
		ok = !!(gl.getCoverageModulationTableNV = GETFN_(proto::GetCoverageModulationTableNVFn, "glGetCoverageModulationTableNV")) && ok;
		if( !gl.EXT_raster_multisample ) {
			ok = !!(gl.rasterSamplesEXT = GETFN_(proto::RasterSamplesEXTFn, "glRasterSamplesEXT")) && ok;
		}
		gl.NV_framebuffer_mixed_samples = ok;
	}
	//- end: NV_framebuffer_mixed_samples

	//-//-- 
	//- Ext: NV_framebuffer_multisample_coverage
	static char const* NV_framebuffer_multisample_coverage = "NV_framebuffer_multisample_coverage";
	if( listed_( arg.extList, NV_framebuffer_multisample_coverage ) && arg.checkExt( NV_framebuffer_multisample_coverage, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.renderbufferStorageMultisampleCoverageNV = GETFN_(proto::RenderbufferStorageMultisampleCoverageNVFn, "glRenderbufferStorageMultisampleCoverageNV")) && ok;
		gl.NV_framebuffer_multisample_coverage = ok;
	}
	//- end: NV_framebuffer_multisample_coverage

	//-//-- 
	//- Ext: NV_geometry_program4
	static char const* NV_geometry_program4 = "NV_geometry_program4";
	if( listed_( arg.extList, NV_geometry_program4 ) && arg.checkExt( NV_geometry_program4, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.framebufferTextureEXT = GETFN_(proto::FramebufferTextureEXTFn, "glFramebufferTextureEXT")) && ok;
		ok = !!(gl.framebufferTextureFaceEXT = GETFN_(proto::FramebufferTextureFaceEXTFn, "glFramebufferTextureFaceEXT")) && ok;
		if( !gl.EXT_texture_array ) {
			ok = !!(gl.framebufferTextureLayerEXT = GETFN_(proto::FramebufferTextureLayerEXTFn, "glFramebufferTextureLayerEXT")) && ok;
		}
		ok = !!(gl.programVertexLimitNV = GETFN_(proto::ProgramVertexLimitNVFn, "glProgramVertexLimitNV")) && ok;
		gl.NV_geometry_program4 = ok;
	}
	//- end: NV_geometry_program4

	//-//-- 
	//- Ext: NV_gpu_program4
	static char const* NV_gpu_program4 = "NV_gpu_program4";
	if( listed_( arg.extList, NV_gpu_program4 ) && arg.checkExt( NV_gpu_program4, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getProgramEnvParameterIivNV = GETFN_(proto::GetProgramEnvParameterIivNVFn, "glGetProgramEnvParameterIivNV")) && ok;
		ok = !!(gl.getProgramEnvParameterIuivNV = GETFN_(proto::GetProgramEnvParameterIuivNVFn, "glGetProgramEnvParameterIuivNV")) && ok;
		ok = !!(gl.getProgramLocalParameterIivNV = GETFN_(proto::GetProgramLocalParameterIivNVFn, "glGetProgramLocalParameterIivNV")) && ok;
		ok = !!(gl.getProgramLocalParameterIuivNV = GETFN_(proto::GetProgramLocalParameterIuivNVFn, "glGetProgramLocalParameterIuivNV")) && ok;
		ok = !!(gl.programEnvParameterI4iNV = GETFN_(proto::ProgramEnvParameterI4iNVFn, "glProgramEnvParameterI4iNV")) && ok;
		ok = !!(gl.programEnvParameterI4ivNV = GETFN_(proto::ProgramEnvParameterI4ivNVFn, "glProgramEnvParameterI4ivNV")) && ok;
		ok = !!(gl.programEnvParameterI4uiNV = GETFN_(proto::ProgramEnvParameterI4uiNVFn, "glProgramEnvParameterI4uiNV")) && ok;
		ok = !!(gl.programEnvParameterI4uivNV = GETFN_(proto::ProgramEnvParameterI4uivNVFn, "glProgramEnvParameterI4uivNV")) && ok;
		ok = !!(gl.programEnvParametersI4ivNV = GETFN_(proto::ProgramEnvParametersI4ivNVFn, "glProgramEnvParametersI4ivNV")) && ok;
		ok = !!(gl.programEnvParametersI4uivNV = GETFN_(proto::ProgramEnvParametersI4uivNVFn, "glProgramEnvParametersI4uivNV")) && ok;
		ok = !!(gl.programLocalParameterI4iNV = GETFN_(proto::ProgramLocalParameterI4iNVFn, "glProgramLocalParameterI4iNV")) && ok;
		ok = !!(gl.programLocalParameterI4ivNV = GETFN_(proto::ProgramLocalParameterI4ivNVFn, "glProgramLocalParameterI4ivNV")) && ok;
		ok = !!(gl.programLocalParameterI4uiNV = GETFN_(proto::ProgramLocalParameterI4uiNVFn, "glProgramLocalParameterI4uiNV")) && ok;
		ok = !!(gl.programLocalParameterI4uivNV = GETFN_(proto::ProgramLocalParameterI4uivNVFn, "glProgramLocalParameterI4uivNV")) && ok;
		ok = !!(gl.programLocalParametersI4ivNV = GETFN_(proto::ProgramLocalParametersI4ivNVFn, "glProgramLocalParametersI4ivNV")) && ok;
		ok = !!(gl.programLocalParametersI4uivNV = GETFN_(proto::ProgramLocalParametersI4uivNVFn, "glProgramLocalParametersI4uivNV")) && ok;
		gl.NV_gpu_program4 = ok;
	}
	//- end: NV_gpu_program4

	//-//-- 
	//- Ext: NV_gpu_program5
	static char const* NV_gpu_program5 = "NV_gpu_program5";
	if( listed_( arg.extList, NV_gpu_program5 ) && arg.checkExt( NV_gpu_program5, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getProgramSubroutineParameteruivNV = GETFN_(proto::GetProgramSubroutineParameteruivNVFn, "glGetProgramSubroutineParameteruivNV")) && ok;
		ok = !!(gl.programSubroutineParametersuivNV = GETFN_(proto::ProgramSubroutineParametersuivNVFn, "glProgramSubroutineParametersuivNV")) && ok;
		gl.NV_gpu_program5 = ok;
	}
	//- end: NV_gpu_program5

	//-//-- 
	//- Ext: NV_gpu_shader5
	static char const* NV_gpu_shader5 = "NV_gpu_shader5";
	if( listed_( arg.extList, NV_gpu_shader5 ) && arg.checkExt( NV_gpu_shader5, arg.user ) )
	{
		bool ok = true;

		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.getUniformi64vNV = GETFN_(proto::GetUniformi64vNVFn, "glGetUniformi64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform1i64NV = GETFN_(proto::ProgramUniform1i64NVFn, "glProgramUniform1i64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform1i64vNV = GETFN_(proto::ProgramUniform1i64vNVFn, "glProgramUniform1i64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform1ui64NV = GETFN_(proto::ProgramUniform1ui64NVFn, "glProgramUniform1ui64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform1ui64vNV = GETFN_(proto::ProgramUniform1ui64vNVFn, "glProgramUniform1ui64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform2i64NV = GETFN_(proto::ProgramUniform2i64NVFn, "glProgramUniform2i64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform2i64vNV = GETFN_(proto::ProgramUniform2i64vNVFn, "glProgramUniform2i64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform2ui64NV = GETFN_(proto::ProgramUniform2ui64NVFn, "glProgramUniform2ui64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform2ui64vNV = GETFN_(proto::ProgramUniform2ui64vNVFn, "glProgramUniform2ui64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform3i64NV = GETFN_(proto::ProgramUniform3i64NVFn, "glProgramUniform3i64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform3i64vNV = GETFN_(proto::ProgramUniform3i64vNVFn, "glProgramUniform3i64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform3ui64NV = GETFN_(proto::ProgramUniform3ui64NVFn, "glProgramUniform3ui64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform3ui64vNV = GETFN_(proto::ProgramUniform3ui64vNVFn, "glProgramUniform3ui64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform4i64NV = GETFN_(proto::ProgramUniform4i64NVFn, "glProgramUniform4i64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform4i64vNV = GETFN_(proto::ProgramUniform4i64vNVFn, "glProgramUniform4i64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform4ui64NV = GETFN_(proto::ProgramUniform4ui64NVFn, "glProgramUniform4ui64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.programUniform4ui64vNV = GETFN_(proto::ProgramUniform4ui64vNVFn, "glProgramUniform4ui64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform1i64NV = GETFN_(proto::Uniform1i64NVFn, "glUniform1i64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform1i64vNV = GETFN_(proto::Uniform1i64vNVFn, "glUniform1i64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform1ui64NV = GETFN_(proto::Uniform1ui64NVFn, "glUniform1ui64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform1ui64vNV = GETFN_(proto::Uniform1ui64vNVFn, "glUniform1ui64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform2i64NV = GETFN_(proto::Uniform2i64NVFn, "glUniform2i64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform2i64vNV = GETFN_(proto::Uniform2i64vNVFn, "glUniform2i64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform2ui64NV = GETFN_(proto::Uniform2ui64NVFn, "glUniform2ui64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform2ui64vNV = GETFN_(proto::Uniform2ui64vNVFn, "glUniform2ui64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform3i64NV = GETFN_(proto::Uniform3i64NVFn, "glUniform3i64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform3i64vNV = GETFN_(proto::Uniform3i64vNVFn, "glUniform3i64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform3ui64NV = GETFN_(proto::Uniform3ui64NVFn, "glUniform3ui64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform3ui64vNV = GETFN_(proto::Uniform3ui64vNVFn, "glUniform3ui64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform4i64NV = GETFN_(proto::Uniform4i64NVFn, "glUniform4i64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform4i64vNV = GETFN_(proto::Uniform4i64vNVFn, "glUniform4i64vNV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform4ui64NV = GETFN_(proto::Uniform4ui64NVFn, "glUniform4ui64NV")) && ok;
		}
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.uniform4ui64vNV = GETFN_(proto::Uniform4ui64vNVFn, "glUniform4ui64vNV")) && ok;
		}
		gl.NV_gpu_shader5 = ok;
	}
	//- end: NV_gpu_shader5

	//-//-- 
	//- Ext: NV_half_float
	static char const* NV_half_float = "NV_half_float";
	if( listed_( arg.extList, NV_half_float ) && arg.checkExt( NV_half_float, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.color3hNV = GETFN_(proto::Color3hNVFn, "glColor3hNV")) && ok;
		ok = !!(gl.color3hvNV = GETFN_(proto::Color3hvNVFn, "glColor3hvNV")) && ok;
		ok = !!(gl.color4hNV = GETFN_(proto::Color4hNVFn, "glColor4hNV")) && ok;
		ok = !!(gl.color4hvNV = GETFN_(proto::Color4hvNVFn, "glColor4hvNV")) && ok;
		ok = !!(gl.fogCoordhNV = GETFN_(proto::FogCoordhNVFn, "glFogCoordhNV")) && ok;
		ok = !!(gl.fogCoordhvNV = GETFN_(proto::FogCoordhvNVFn, "glFogCoordhvNV")) && ok;
		ok = !!(gl.multiTexCoord1hNV = GETFN_(proto::MultiTexCoord1hNVFn, "glMultiTexCoord1hNV")) && ok;
		ok = !!(gl.multiTexCoord1hvNV = GETFN_(proto::MultiTexCoord1hvNVFn, "glMultiTexCoord1hvNV")) && ok;
		ok = !!(gl.multiTexCoord2hNV = GETFN_(proto::MultiTexCoord2hNVFn, "glMultiTexCoord2hNV")) && ok;
		ok = !!(gl.multiTexCoord2hvNV = GETFN_(proto::MultiTexCoord2hvNVFn, "glMultiTexCoord2hvNV")) && ok;
		ok = !!(gl.multiTexCoord3hNV = GETFN_(proto::MultiTexCoord3hNVFn, "glMultiTexCoord3hNV")) && ok;
		ok = !!(gl.multiTexCoord3hvNV = GETFN_(proto::MultiTexCoord3hvNVFn, "glMultiTexCoord3hvNV")) && ok;
		ok = !!(gl.multiTexCoord4hNV = GETFN_(proto::MultiTexCoord4hNVFn, "glMultiTexCoord4hNV")) && ok;
		ok = !!(gl.multiTexCoord4hvNV = GETFN_(proto::MultiTexCoord4hvNVFn, "glMultiTexCoord4hvNV")) && ok;
		ok = !!(gl.normal3hNV = GETFN_(proto::Normal3hNVFn, "glNormal3hNV")) && ok;
		ok = !!(gl.normal3hvNV = GETFN_(proto::Normal3hvNVFn, "glNormal3hvNV")) && ok;
		ok = !!(gl.secondaryColor3hNV = GETFN_(proto::SecondaryColor3hNVFn, "glSecondaryColor3hNV")) && ok;
		ok = !!(gl.secondaryColor3hvNV = GETFN_(proto::SecondaryColor3hvNVFn, "glSecondaryColor3hvNV")) && ok;
		ok = !!(gl.texCoord1hNV = GETFN_(proto::TexCoord1hNVFn, "glTexCoord1hNV")) && ok;
		ok = !!(gl.texCoord1hvNV = GETFN_(proto::TexCoord1hvNVFn, "glTexCoord1hvNV")) && ok;
		ok = !!(gl.texCoord2hNV = GETFN_(proto::TexCoord2hNVFn, "glTexCoord2hNV")) && ok;
		ok = !!(gl.texCoord2hvNV = GETFN_(proto::TexCoord2hvNVFn, "glTexCoord2hvNV")) && ok;
		ok = !!(gl.texCoord3hNV = GETFN_(proto::TexCoord3hNVFn, "glTexCoord3hNV")) && ok;
		ok = !!(gl.texCoord3hvNV = GETFN_(proto::TexCoord3hvNVFn, "glTexCoord3hvNV")) && ok;
		ok = !!(gl.texCoord4hNV = GETFN_(proto::TexCoord4hNVFn, "glTexCoord4hNV")) && ok;
		ok = !!(gl.texCoord4hvNV = GETFN_(proto::TexCoord4hvNVFn, "glTexCoord4hvNV")) && ok;
		ok = !!(gl.vertex2hNV = GETFN_(proto::Vertex2hNVFn, "glVertex2hNV")) && ok;
		ok = !!(gl.vertex2hvNV = GETFN_(proto::Vertex2hvNVFn, "glVertex2hvNV")) && ok;
		ok = !!(gl.vertex3hNV = GETFN_(proto::Vertex3hNVFn, "glVertex3hNV")) && ok;
		ok = !!(gl.vertex3hvNV = GETFN_(proto::Vertex3hvNVFn, "glVertex3hvNV")) && ok;
		ok = !!(gl.vertex4hNV = GETFN_(proto::Vertex4hNVFn, "glVertex4hNV")) && ok;
		ok = !!(gl.vertex4hvNV = GETFN_(proto::Vertex4hvNVFn, "glVertex4hvNV")) && ok;
		ok = !!(gl.vertexAttrib1hNV = GETFN_(proto::VertexAttrib1hNVFn, "glVertexAttrib1hNV")) && ok;
		ok = !!(gl.vertexAttrib1hvNV = GETFN_(proto::VertexAttrib1hvNVFn, "glVertexAttrib1hvNV")) && ok;
		ok = !!(gl.vertexAttrib2hNV = GETFN_(proto::VertexAttrib2hNVFn, "glVertexAttrib2hNV")) && ok;
		ok = !!(gl.vertexAttrib2hvNV = GETFN_(proto::VertexAttrib2hvNVFn, "glVertexAttrib2hvNV")) && ok;
		ok = !!(gl.vertexAttrib3hNV = GETFN_(proto::VertexAttrib3hNVFn, "glVertexAttrib3hNV")) && ok;
		ok = !!(gl.vertexAttrib3hvNV = GETFN_(proto::VertexAttrib3hvNVFn, "glVertexAttrib3hvNV")) && ok;
		ok = !!(gl.vertexAttrib4hNV = GETFN_(proto::VertexAttrib4hNVFn, "glVertexAttrib4hNV")) && ok;
		ok = !!(gl.vertexAttrib4hvNV = GETFN_(proto::VertexAttrib4hvNVFn, "glVertexAttrib4hvNV")) && ok;
		ok = !!(gl.vertexAttribs1hvNV = GETFN_(proto::VertexAttribs1hvNVFn, "glVertexAttribs1hvNV")) && ok;
		ok = !!(gl.vertexAttribs2hvNV = GETFN_(proto::VertexAttribs2hvNVFn, "glVertexAttribs2hvNV")) && ok;
		ok = !!(gl.vertexAttribs3hvNV = GETFN_(proto::VertexAttribs3hvNVFn, "glVertexAttribs3hvNV")) && ok;
		ok = !!(gl.vertexAttribs4hvNV = GETFN_(proto::VertexAttribs4hvNVFn, "glVertexAttribs4hvNV")) && ok;
		ok = !!(gl.vertexWeighthNV = GETFN_(proto::VertexWeighthNVFn, "glVertexWeighthNV")) && ok;
		ok = !!(gl.vertexWeighthvNV = GETFN_(proto::VertexWeighthvNVFn, "glVertexWeighthvNV")) && ok;
		gl.NV_half_float = ok;
	}
	//- end: NV_half_float

	//-//-- 
	//- Ext: NV_internalformat_sample_query
	static char const* NV_internalformat_sample_query = "NV_internalformat_sample_query";
	if( listed_( arg.extList, NV_internalformat_sample_query ) && arg.checkExt( NV_internalformat_sample_query, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getInternalformatSampleivNV = GETFN_(proto::GetInternalformatSampleivNVFn, "glGetInternalformatSampleivNV")) && ok;
		gl.NV_internalformat_sample_query = ok;
	}
	//- end: NV_internalformat_sample_query

	//-//-- 
	//- Ext: NV_occlusion_query
	static char const* NV_occlusion_query = "NV_occlusion_query";
	if( listed_( arg.extList, NV_occlusion_query ) && arg.checkExt( NV_occlusion_query, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.beginOcclusionQueryNV = GETFN_(proto::BeginOcclusionQueryNVFn, "glBeginOcclusionQueryNV")) && ok;
		ok = !!(gl.deleteOcclusionQueriesNV = GETFN_(proto::DeleteOcclusionQueriesNVFn, "glDeleteOcclusionQueriesNV")) && ok;
		ok = !!(gl.endOcclusionQueryNV = GETFN_(proto::EndOcclusionQueryNVFn, "glEndOcclusionQueryNV")) && ok;
		ok = !!(gl.genOcclusionQueriesNV = GETFN_(proto::GenOcclusionQueriesNVFn, "glGenOcclusionQueriesNV")) && ok;
		ok = !!(gl.getOcclusionQueryivNV = GETFN_(proto::GetOcclusionQueryivNVFn, "glGetOcclusionQueryivNV")) && ok;
		ok = !!(gl.getOcclusionQueryuivNV = GETFN_(proto::GetOcclusionQueryuivNVFn, "glGetOcclusionQueryuivNV")) && ok;
		ok = !!(gl.isOcclusionQueryNV = GETFN_(proto::IsOcclusionQueryNVFn, "glIsOcclusionQueryNV")) && ok;
		gl.NV_occlusion_query = ok;
	}
	//- end: NV_occlusion_query

	//-//-- 
	//- Ext: NV_parameter_buffer_object
	static char const* NV_parameter_buffer_object = "NV_parameter_buffer_object";
	if( listed_( arg.extList, NV_parameter_buffer_object ) && arg.checkExt( NV_parameter_buffer_object, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.programBufferParametersIivNV = GETFN_(proto::ProgramBufferParametersIivNVFn, "glProgramBufferParametersIivNV")) && ok;
		ok = !!(gl.programBufferParametersIuivNV = GETFN_(proto::ProgramBufferParametersIuivNVFn, "glProgramBufferParametersIuivNV")) && ok;
		ok = !!(gl.programBufferParametersfvNV = GETFN_(proto::ProgramBufferParametersfvNVFn, "glProgramBufferParametersfvNV")) && ok;
		gl.NV_parameter_buffer_object = ok;
	}
	//- end: NV_parameter_buffer_object

	//-//-- 
	//- Ext: NV_path_rendering
	static char const* NV_path_rendering = "NV_path_rendering";
	if( listed_( arg.extList, NV_path_rendering ) && arg.checkExt( NV_path_rendering, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.copyPathNV = GETFN_(proto::CopyPathNVFn, "glCopyPathNV")) && ok;
		ok = !!(gl.coverFillPathInstancedNV = GETFN_(proto::CoverFillPathInstancedNVFn, "glCoverFillPathInstancedNV")) && ok;
		ok = !!(gl.coverFillPathNV = GETFN_(proto::CoverFillPathNVFn, "glCoverFillPathNV")) && ok;
		ok = !!(gl.coverStrokePathInstancedNV = GETFN_(proto::CoverStrokePathInstancedNVFn, "glCoverStrokePathInstancedNV")) && ok;
		ok = !!(gl.coverStrokePathNV = GETFN_(proto::CoverStrokePathNVFn, "glCoverStrokePathNV")) && ok;
		ok = !!(gl.deletePathsNV = GETFN_(proto::DeletePathsNVFn, "glDeletePathsNV")) && ok;
		ok = !!(gl.genPathsNV = GETFN_(proto::GenPathsNVFn, "glGenPathsNV")) && ok;
		ok = !!(gl.getPathColorGenfvNV = GETFN_(proto::GetPathColorGenfvNVFn, "glGetPathColorGenfvNV")) && ok;
		ok = !!(gl.getPathColorGenivNV = GETFN_(proto::GetPathColorGenivNVFn, "glGetPathColorGenivNV")) && ok;
		ok = !!(gl.getPathCommandsNV = GETFN_(proto::GetPathCommandsNVFn, "glGetPathCommandsNV")) && ok;
		ok = !!(gl.getPathCoordsNV = GETFN_(proto::GetPathCoordsNVFn, "glGetPathCoordsNV")) && ok;
		ok = !!(gl.getPathDashArrayNV = GETFN_(proto::GetPathDashArrayNVFn, "glGetPathDashArrayNV")) && ok;
		ok = !!(gl.getPathLengthNV = GETFN_(proto::GetPathLengthNVFn, "glGetPathLengthNV")) && ok;
		ok = !!(gl.getPathMetricRangeNV = GETFN_(proto::GetPathMetricRangeNVFn, "glGetPathMetricRangeNV")) && ok;
		ok = !!(gl.getPathMetricsNV = GETFN_(proto::GetPathMetricsNVFn, "glGetPathMetricsNV")) && ok;
		ok = !!(gl.getPathParameterfvNV = GETFN_(proto::GetPathParameterfvNVFn, "glGetPathParameterfvNV")) && ok;
		ok = !!(gl.getPathParameterivNV = GETFN_(proto::GetPathParameterivNVFn, "glGetPathParameterivNV")) && ok;
		ok = !!(gl.getPathSpacingNV = GETFN_(proto::GetPathSpacingNVFn, "glGetPathSpacingNV")) && ok;
		ok = !!(gl.getPathTexGenfvNV = GETFN_(proto::GetPathTexGenfvNVFn, "glGetPathTexGenfvNV")) && ok;
		ok = !!(gl.getPathTexGenivNV = GETFN_(proto::GetPathTexGenivNVFn, "glGetPathTexGenivNV")) && ok;
		ok = !!(gl.getProgramResourcefvNV = GETFN_(proto::GetProgramResourcefvNVFn, "glGetProgramResourcefvNV")) && ok;
		ok = !!(gl.interpolatePathsNV = GETFN_(proto::InterpolatePathsNVFn, "glInterpolatePathsNV")) && ok;
		ok = !!(gl.isPathNV = GETFN_(proto::IsPathNVFn, "glIsPathNV")) && ok;
		ok = !!(gl.isPointInFillPathNV = GETFN_(proto::IsPointInFillPathNVFn, "glIsPointInFillPathNV")) && ok;
		ok = !!(gl.isPointInStrokePathNV = GETFN_(proto::IsPointInStrokePathNVFn, "glIsPointInStrokePathNV")) && ok;
		ok = !!(gl.matrixLoad3x2fNV = GETFN_(proto::MatrixLoad3x2fNVFn, "glMatrixLoad3x2fNV")) && ok;
		ok = !!(gl.matrixLoad3x3fNV = GETFN_(proto::MatrixLoad3x3fNVFn, "glMatrixLoad3x3fNV")) && ok;
		ok = !!(gl.matrixLoadTranspose3x3fNV = GETFN_(proto::MatrixLoadTranspose3x3fNVFn, "glMatrixLoadTranspose3x3fNV")) && ok;
		ok = !!(gl.matrixMult3x2fNV = GETFN_(proto::MatrixMult3x2fNVFn, "glMatrixMult3x2fNV")) && ok;
		ok = !!(gl.matrixMult3x3fNV = GETFN_(proto::MatrixMult3x3fNVFn, "glMatrixMult3x3fNV")) && ok;
		ok = !!(gl.matrixMultTranspose3x3fNV = GETFN_(proto::MatrixMultTranspose3x3fNVFn, "glMatrixMultTranspose3x3fNV")) && ok;
		ok = !!(gl.pathColorGenNV = GETFN_(proto::PathColorGenNVFn, "glPathColorGenNV")) && ok;
		ok = !!(gl.pathCommandsNV = GETFN_(proto::PathCommandsNVFn, "glPathCommandsNV")) && ok;
		ok = !!(gl.pathCoordsNV = GETFN_(proto::PathCoordsNVFn, "glPathCoordsNV")) && ok;
		ok = !!(gl.pathCoverDepthFuncNV = GETFN_(proto::PathCoverDepthFuncNVFn, "glPathCoverDepthFuncNV")) && ok;
		ok = !!(gl.pathDashArrayNV = GETFN_(proto::PathDashArrayNVFn, "glPathDashArrayNV")) && ok;
		ok = !!(gl.pathFogGenNV = GETFN_(proto::PathFogGenNVFn, "glPathFogGenNV")) && ok;
		ok = !!(gl.pathGlyphIndexArrayNV = GETFN_(proto::PathGlyphIndexArrayNVFn, "glPathGlyphIndexArrayNV")) && ok;
		ok = !!(gl.pathGlyphIndexRangeNV = GETFN_(proto::PathGlyphIndexRangeNVFn, "glPathGlyphIndexRangeNV")) && ok;
		ok = !!(gl.pathGlyphRangeNV = GETFN_(proto::PathGlyphRangeNVFn, "glPathGlyphRangeNV")) && ok;
		ok = !!(gl.pathGlyphsNV = GETFN_(proto::PathGlyphsNVFn, "glPathGlyphsNV")) && ok;
		ok = !!(gl.pathMemoryGlyphIndexArrayNV = GETFN_(proto::PathMemoryGlyphIndexArrayNVFn, "glPathMemoryGlyphIndexArrayNV")) && ok;
		ok = !!(gl.pathParameterfNV = GETFN_(proto::PathParameterfNVFn, "glPathParameterfNV")) && ok;
		ok = !!(gl.pathParameterfvNV = GETFN_(proto::PathParameterfvNVFn, "glPathParameterfvNV")) && ok;
		ok = !!(gl.pathParameteriNV = GETFN_(proto::PathParameteriNVFn, "glPathParameteriNV")) && ok;
		ok = !!(gl.pathParameterivNV = GETFN_(proto::PathParameterivNVFn, "glPathParameterivNV")) && ok;
		ok = !!(gl.pathStencilDepthOffsetNV = GETFN_(proto::PathStencilDepthOffsetNVFn, "glPathStencilDepthOffsetNV")) && ok;
		ok = !!(gl.pathStencilFuncNV = GETFN_(proto::PathStencilFuncNVFn, "glPathStencilFuncNV")) && ok;
		ok = !!(gl.pathStringNV = GETFN_(proto::PathStringNVFn, "glPathStringNV")) && ok;
		ok = !!(gl.pathSubCommandsNV = GETFN_(proto::PathSubCommandsNVFn, "glPathSubCommandsNV")) && ok;
		ok = !!(gl.pathSubCoordsNV = GETFN_(proto::PathSubCoordsNVFn, "glPathSubCoordsNV")) && ok;
		ok = !!(gl.pathTexGenNV = GETFN_(proto::PathTexGenNVFn, "glPathTexGenNV")) && ok;
		ok = !!(gl.pointAlongPathNV = GETFN_(proto::PointAlongPathNVFn, "glPointAlongPathNV")) && ok;
		ok = !!(gl.programPathFragmentInputGenNV = GETFN_(proto::ProgramPathFragmentInputGenNVFn, "glProgramPathFragmentInputGenNV")) && ok;
		ok = !!(gl.stencilFillPathInstancedNV = GETFN_(proto::StencilFillPathInstancedNVFn, "glStencilFillPathInstancedNV")) && ok;
		ok = !!(gl.stencilFillPathNV = GETFN_(proto::StencilFillPathNVFn, "glStencilFillPathNV")) && ok;
		ok = !!(gl.stencilStrokePathInstancedNV = GETFN_(proto::StencilStrokePathInstancedNVFn, "glStencilStrokePathInstancedNV")) && ok;
		ok = !!(gl.stencilStrokePathNV = GETFN_(proto::StencilStrokePathNVFn, "glStencilStrokePathNV")) && ok;
		ok = !!(gl.stencilThenCoverFillPathInstancedNV = GETFN_(proto::StencilThenCoverFillPathInstancedNVFn, "glStencilThenCoverFillPathInstancedNV")) && ok;
		ok = !!(gl.stencilThenCoverFillPathNV = GETFN_(proto::StencilThenCoverFillPathNVFn, "glStencilThenCoverFillPathNV")) && ok;
		ok = !!(gl.stencilThenCoverStrokePathInstancedNV = GETFN_(proto::StencilThenCoverStrokePathInstancedNVFn, "glStencilThenCoverStrokePathInstancedNV")) && ok;
		ok = !!(gl.stencilThenCoverStrokePathNV = GETFN_(proto::StencilThenCoverStrokePathNVFn, "glStencilThenCoverStrokePathNV")) && ok;
		ok = !!(gl.transformPathNV = GETFN_(proto::TransformPathNVFn, "glTransformPathNV")) && ok;
		ok = !!(gl.weightPathsNV = GETFN_(proto::WeightPathsNVFn, "glWeightPathsNV")) && ok;
		gl.NV_path_rendering = ok;
	}
	//- end: NV_path_rendering

	//-//-- 
	//- Ext: NV_pixel_data_range
	static char const* NV_pixel_data_range = "NV_pixel_data_range";
	if( listed_( arg.extList, NV_pixel_data_range ) && arg.checkExt( NV_pixel_data_range, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.flushPixelDataRangeNV = GETFN_(proto::FlushPixelDataRangeNVFn, "glFlushPixelDataRangeNV")) && ok;
		ok = !!(gl.pixelDataRangeNV = GETFN_(proto::PixelDataRangeNVFn, "glPixelDataRangeNV")) && ok;
		gl.NV_pixel_data_range = ok;
	}
	//- end: NV_pixel_data_range

	//-//-- 
	//- Ext: NV_point_sprite
	static char const* NV_point_sprite = "NV_point_sprite";
	if( listed_( arg.extList, NV_point_sprite ) && arg.checkExt( NV_point_sprite, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.pointParameteriNV = GETFN_(proto::PointParameteriNVFn, "glPointParameteriNV")) && ok;
		ok = !!(gl.pointParameterivNV = GETFN_(proto::PointParameterivNVFn, "glPointParameterivNV")) && ok;
		gl.NV_point_sprite = ok;
	}
	//- end: NV_point_sprite

	//-//-- 
	//- Ext: NV_present_video
	static char const* NV_present_video = "NV_present_video";
	if( listed_( arg.extList, NV_present_video ) && arg.checkExt( NV_present_video, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getVideoi64vNV = GETFN_(proto::GetVideoi64vNVFn, "glGetVideoi64vNV")) && ok;
		ok = !!(gl.getVideoivNV = GETFN_(proto::GetVideoivNVFn, "glGetVideoivNV")) && ok;
		ok = !!(gl.getVideoui64vNV = GETFN_(proto::GetVideoui64vNVFn, "glGetVideoui64vNV")) && ok;
		ok = !!(gl.getVideouivNV = GETFN_(proto::GetVideouivNVFn, "glGetVideouivNV")) && ok;
		ok = !!(gl.presentFrameDualFillNV = GETFN_(proto::PresentFrameDualFillNVFn, "glPresentFrameDualFillNV")) && ok;
		ok = !!(gl.presentFrameKeyedNV = GETFN_(proto::PresentFrameKeyedNVFn, "glPresentFrameKeyedNV")) && ok;
		gl.NV_present_video = ok;
	}
	//- end: NV_present_video

	//-//-- 
	//- Ext: NV_primitive_restart
	static char const* NV_primitive_restart = "NV_primitive_restart";
	if( listed_( arg.extList, NV_primitive_restart ) && arg.checkExt( NV_primitive_restart, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.primitiveRestartIndexNV = GETFN_(proto::PrimitiveRestartIndexNVFn, "glPrimitiveRestartIndexNV")) && ok;
		ok = !!(gl.primitiveRestartNV = GETFN_(proto::PrimitiveRestartNVFn, "glPrimitiveRestartNV")) && ok;
		gl.NV_primitive_restart = ok;
	}
	//- end: NV_primitive_restart

	//-//-- 
	//- Ext: NV_register_combiners
	static char const* NV_register_combiners = "NV_register_combiners";
	if( listed_( arg.extList, NV_register_combiners ) && arg.checkExt( NV_register_combiners, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.combinerInputNV = GETFN_(proto::CombinerInputNVFn, "glCombinerInputNV")) && ok;
		ok = !!(gl.combinerOutputNV = GETFN_(proto::CombinerOutputNVFn, "glCombinerOutputNV")) && ok;
		ok = !!(gl.combinerParameterfNV = GETFN_(proto::CombinerParameterfNVFn, "glCombinerParameterfNV")) && ok;
		ok = !!(gl.combinerParameterfvNV = GETFN_(proto::CombinerParameterfvNVFn, "glCombinerParameterfvNV")) && ok;
		ok = !!(gl.combinerParameteriNV = GETFN_(proto::CombinerParameteriNVFn, "glCombinerParameteriNV")) && ok;
		ok = !!(gl.combinerParameterivNV = GETFN_(proto::CombinerParameterivNVFn, "glCombinerParameterivNV")) && ok;
		ok = !!(gl.finalCombinerInputNV = GETFN_(proto::FinalCombinerInputNVFn, "glFinalCombinerInputNV")) && ok;
		ok = !!(gl.getCombinerInputParameterfvNV = GETFN_(proto::GetCombinerInputParameterfvNVFn, "glGetCombinerInputParameterfvNV")) && ok;
		ok = !!(gl.getCombinerInputParameterivNV = GETFN_(proto::GetCombinerInputParameterivNVFn, "glGetCombinerInputParameterivNV")) && ok;
		ok = !!(gl.getCombinerOutputParameterfvNV = GETFN_(proto::GetCombinerOutputParameterfvNVFn, "glGetCombinerOutputParameterfvNV")) && ok;
		ok = !!(gl.getCombinerOutputParameterivNV = GETFN_(proto::GetCombinerOutputParameterivNVFn, "glGetCombinerOutputParameterivNV")) && ok;
		ok = !!(gl.getFinalCombinerInputParameterfvNV = GETFN_(proto::GetFinalCombinerInputParameterfvNVFn, "glGetFinalCombinerInputParameterfvNV")) && ok;
		ok = !!(gl.getFinalCombinerInputParameterivNV = GETFN_(proto::GetFinalCombinerInputParameterivNVFn, "glGetFinalCombinerInputParameterivNV")) && ok;
		gl.NV_register_combiners = ok;
	}
	//- end: NV_register_combiners

	//-//-- 
	//- Ext: NV_register_combiners2
	static char const* NV_register_combiners2 = "NV_register_combiners2";
	if( listed_( arg.extList, NV_register_combiners2 ) && arg.checkExt( NV_register_combiners2, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.combinerStageParameterfvNV = GETFN_(proto::CombinerStageParameterfvNVFn, "glCombinerStageParameterfvNV")) && ok;
		ok = !!(gl.getCombinerStageParameterfvNV = GETFN_(proto::GetCombinerStageParameterfvNVFn, "glGetCombinerStageParameterfvNV")) && ok;
		gl.NV_register_combiners2 = ok;
	}
	//- end: NV_register_combiners2

	//-//-- 
	//- Ext: NV_sample_locations
	static char const* NV_sample_locations = "NV_sample_locations";
	if( listed_( arg.extList, NV_sample_locations ) && arg.checkExt( NV_sample_locations, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.framebufferSampleLocationsfvNV = GETFN_(proto::FramebufferSampleLocationsfvNVFn, "glFramebufferSampleLocationsfvNV")) && ok;
		ok = !!(gl.namedFramebufferSampleLocationsfvNV = GETFN_(proto::NamedFramebufferSampleLocationsfvNVFn, "glNamedFramebufferSampleLocationsfvNV")) && ok;
		ok = !!(gl.resolveDepthValuesNV = GETFN_(proto::ResolveDepthValuesNVFn, "glResolveDepthValuesNV")) && ok;
		gl.NV_sample_locations = ok;
	}
	//- end: NV_sample_locations

	//-//-- 
	//- Ext: NV_shader_buffer_load
	static char const* NV_shader_buffer_load = "NV_shader_buffer_load";
	if( listed_( arg.extList, NV_shader_buffer_load ) && arg.checkExt( NV_shader_buffer_load, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getBufferParameterui64vNV = GETFN_(proto::GetBufferParameterui64vNVFn, "glGetBufferParameterui64vNV")) && ok;
		ok = !!(gl.getIntegerui64vNV = GETFN_(proto::GetIntegerui64vNVFn, "glGetIntegerui64vNV")) && ok;
		ok = !!(gl.getNamedBufferParameterui64vNV = GETFN_(proto::GetNamedBufferParameterui64vNVFn, "glGetNamedBufferParameterui64vNV")) && ok;
		if( !gl.AMD_gpu_shader_int64 ) {
			ok = !!(gl.getUniformui64vNV = GETFN_(proto::GetUniformui64vNVFn, "glGetUniformui64vNV")) && ok;
		}
		ok = !!(gl.isBufferResidentNV = GETFN_(proto::IsBufferResidentNVFn, "glIsBufferResidentNV")) && ok;
		ok = !!(gl.isNamedBufferResidentNV = GETFN_(proto::IsNamedBufferResidentNVFn, "glIsNamedBufferResidentNV")) && ok;
		ok = !!(gl.makeBufferNonResidentNV = GETFN_(proto::MakeBufferNonResidentNVFn, "glMakeBufferNonResidentNV")) && ok;
		ok = !!(gl.makeBufferResidentNV = GETFN_(proto::MakeBufferResidentNVFn, "glMakeBufferResidentNV")) && ok;
		ok = !!(gl.makeNamedBufferNonResidentNV = GETFN_(proto::MakeNamedBufferNonResidentNVFn, "glMakeNamedBufferNonResidentNV")) && ok;
		ok = !!(gl.makeNamedBufferResidentNV = GETFN_(proto::MakeNamedBufferResidentNVFn, "glMakeNamedBufferResidentNV")) && ok;
		ok = !!(gl.programUniformui64NV = GETFN_(proto::ProgramUniformui64NVFn, "glProgramUniformui64NV")) && ok;
		ok = !!(gl.programUniformui64vNV = GETFN_(proto::ProgramUniformui64vNVFn, "glProgramUniformui64vNV")) && ok;
		ok = !!(gl.uniformui64NV = GETFN_(proto::Uniformui64NVFn, "glUniformui64NV")) && ok;
		ok = !!(gl.uniformui64vNV = GETFN_(proto::Uniformui64vNVFn, "glUniformui64vNV")) && ok;
		gl.NV_shader_buffer_load = ok;
	}
	//- end: NV_shader_buffer_load

	//-//-- 
	//- Ext: NV_texture_barrier
	static char const* NV_texture_barrier = "NV_texture_barrier";
	if( listed_( arg.extList, NV_texture_barrier ) && arg.checkExt( NV_texture_barrier, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.textureBarrierNV = GETFN_(proto::TextureBarrierNVFn, "glTextureBarrierNV")) && ok;
		gl.NV_texture_barrier = ok;
	}
	//- end: NV_texture_barrier

	//-//-- 
	//- Ext: NV_texture_multisample
	static char const* NV_texture_multisample = "NV_texture_multisample";
	if( listed_( arg.extList, NV_texture_multisample ) && arg.checkExt( NV_texture_multisample, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.texImage2DMultisampleCoverageNV = GETFN_(proto::TexImage2DMultisampleCoverageNVFn, "glTexImage2DMultisampleCoverageNV")) && ok;
		ok = !!(gl.texImage3DMultisampleCoverageNV = GETFN_(proto::TexImage3DMultisampleCoverageNVFn, "glTexImage3DMultisampleCoverageNV")) && ok;
		ok = !!(gl.textureImage2DMultisampleCoverageNV = GETFN_(proto::TextureImage2DMultisampleCoverageNVFn, "glTextureImage2DMultisampleCoverageNV")) && ok;
		ok = !!(gl.textureImage2DMultisampleNV = GETFN_(proto::TextureImage2DMultisampleNVFn, "glTextureImage2DMultisampleNV")) && ok;
		ok = !!(gl.textureImage3DMultisampleCoverageNV = GETFN_(proto::TextureImage3DMultisampleCoverageNVFn, "glTextureImage3DMultisampleCoverageNV")) && ok;
		ok = !!(gl.textureImage3DMultisampleNV = GETFN_(proto::TextureImage3DMultisampleNVFn, "glTextureImage3DMultisampleNV")) && ok;
		gl.NV_texture_multisample = ok;
	}
	//- end: NV_texture_multisample

	//-//-- 
	//- Ext: NV_transform_feedback
	static char const* NV_transform_feedback = "NV_transform_feedback";
	if( listed_( arg.extList, NV_transform_feedback ) && arg.checkExt( NV_transform_feedback, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.activeVaryingNV = GETFN_(proto::ActiveVaryingNVFn, "glActiveVaryingNV")) && ok;
		ok = !!(gl.beginTransformFeedbackNV = GETFN_(proto::BeginTransformFeedbackNVFn, "glBeginTransformFeedbackNV")) && ok;
		ok = !!(gl.bindBufferBaseNV = GETFN_(proto::BindBufferBaseNVFn, "glBindBufferBaseNV")) && ok;
		ok = !!(gl.bindBufferOffsetNV = GETFN_(proto::BindBufferOffsetNVFn, "glBindBufferOffsetNV")) && ok;
		ok = !!(gl.bindBufferRangeNV = GETFN_(proto::BindBufferRangeNVFn, "glBindBufferRangeNV")) && ok;
		ok = !!(gl.endTransformFeedbackNV = GETFN_(proto::EndTransformFeedbackNVFn, "glEndTransformFeedbackNV")) && ok;
		ok = !!(gl.getActiveVaryingNV = GETFN_(proto::GetActiveVaryingNVFn, "glGetActiveVaryingNV")) && ok;
		ok = !!(gl.getTransformFeedbackVaryingNV = GETFN_(proto::GetTransformFeedbackVaryingNVFn, "glGetTransformFeedbackVaryingNV")) && ok;
		ok = !!(gl.getVaryingLocationNV = GETFN_(proto::GetVaryingLocationNVFn, "glGetVaryingLocationNV")) && ok;
		ok = !!(gl.transformFeedbackAttribsNV = GETFN_(proto::TransformFeedbackAttribsNVFn, "glTransformFeedbackAttribsNV")) && ok;
		ok = !!(gl.transformFeedbackStreamAttribsNV = GETFN_(proto::TransformFeedbackStreamAttribsNVFn, "glTransformFeedbackStreamAttribsNV")) && ok;
		ok = !!(gl.transformFeedbackVaryingsNV = GETFN_(proto::TransformFeedbackVaryingsNVFn, "glTransformFeedbackVaryingsNV")) && ok;
		gl.NV_transform_feedback = ok;
	}
	//- end: NV_transform_feedback

	//-//-- 
	//- Ext: NV_transform_feedback2
	static char const* NV_transform_feedback2 = "NV_transform_feedback2";
	if( listed_( arg.extList, NV_transform_feedback2 ) && arg.checkExt( NV_transform_feedback2, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.bindTransformFeedbackNV = GETFN_(proto::BindTransformFeedbackNVFn, "glBindTransformFeedbackNV")) && ok;
		ok = !!(gl.deleteTransformFeedbacksNV = GETFN_(proto::DeleteTransformFeedbacksNVFn, "glDeleteTransformFeedbacksNV")) && ok;
		ok = !!(gl.drawTransformFeedbackNV = GETFN_(proto::DrawTransformFeedbackNVFn, "glDrawTransformFeedbackNV")) && ok;
		ok = !!(gl.genTransformFeedbacksNV = GETFN_(proto::GenTransformFeedbacksNVFn, "glGenTransformFeedbacksNV")) && ok;
		ok = !!(gl.isTransformFeedbackNV = GETFN_(proto::IsTransformFeedbackNVFn, "glIsTransformFeedbackNV")) && ok;
		ok = !!(gl.pauseTransformFeedbackNV = GETFN_(proto::PauseTransformFeedbackNVFn, "glPauseTransformFeedbackNV")) && ok;
		ok = !!(gl.resumeTransformFeedbackNV = GETFN_(proto::ResumeTransformFeedbackNVFn, "glResumeTransformFeedbackNV")) && ok;
		gl.NV_transform_feedback2 = ok;
	}
	//- end: NV_transform_feedback2

	//-//-- 
	//- Ext: NV_vdpau_interop
	static char const* NV_vdpau_interop = "NV_vdpau_interop";
	if( listed_( arg.extList, NV_vdpau_interop ) && arg.checkExt( NV_vdpau_interop, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.vDPAUFiniNV = GETFN_(proto::VDPAUFiniNVFn, "glVDPAUFiniNV")) && ok;
		ok = !!(gl.vDPAUGetSurfaceivNV = GETFN_(proto::VDPAUGetSurfaceivNVFn, "glVDPAUGetSurfaceivNV")) && ok;
		ok = !!(gl.vDPAUInitNV = GETFN_(proto::VDPAUInitNVFn, "glVDPAUInitNV")) && ok;
		ok = !!(gl.vDPAUIsSurfaceNV = GETFN_(proto::VDPAUIsSurfaceNVFn, "glVDPAUIsSurfaceNV")) && ok;
		ok = !!(gl.vDPAUMapSurfacesNV = GETFN_(proto::VDPAUMapSurfacesNVFn, "glVDPAUMapSurfacesNV")) && ok;
		ok = !!(gl.vDPAURegisterOutputSurfaceNV = GETFN_(proto::VDPAURegisterOutputSurfaceNVFn, "glVDPAURegisterOutputSurfaceNV")) && ok;
		ok = !!(gl.vDPAURegisterVideoSurfaceNV = GETFN_(proto::VDPAURegisterVideoSurfaceNVFn, "glVDPAURegisterVideoSurfaceNV")) && ok;
		ok = !!(gl.vDPAUSurfaceAccessNV = GETFN_(proto::VDPAUSurfaceAccessNVFn, "glVDPAUSurfaceAccessNV")) && ok;
		ok = !!(gl.vDPAUUnmapSurfacesNV = GETFN_(proto::VDPAUUnmapSurfacesNVFn, "glVDPAUUnmapSurfacesNV")) && ok;
		ok = !!(gl.vDPAUUnregisterSurfaceNV = GETFN_(proto::VDPAUUnregisterSurfaceNVFn, "glVDPAUUnregisterSurfaceNV")) && ok;
		gl.NV_vdpau_interop = ok;
	}
	//- end: NV_vdpau_interop

	//-//-- 
	//- Ext: NV_vertex_array_range
	static char const* NV_vertex_array_range = "NV_vertex_array_range";
	if( listed_( arg.extList, NV_vertex_array_range ) && arg.checkExt( NV_vertex_array_range, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.flushVertexArrayRangeNV = GETFN_(proto::FlushVertexArrayRangeNVFn, "glFlushVertexArrayRangeNV")) && ok;
		ok = !!(gl.vertexArrayRangeNV = GETFN_(proto::VertexArrayRangeNVFn, "glVertexArrayRangeNV")) && ok;
		gl.NV_vertex_array_range = ok;
	}
	//- end: NV_vertex_array_range

	//-//-- 
	//- Ext: NV_vertex_attrib_integer_64bit
	static char const* NV_vertex_attrib_integer_64bit = "NV_vertex_attrib_integer_64bit";
	if( listed_( arg.extList, NV_vertex_attrib_integer_64bit ) && arg.checkExt( NV_vertex_attrib_integer_64bit, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getVertexAttribLi64vNV = GETFN_(proto::GetVertexAttribLi64vNVFn, "glGetVertexAttribLi64vNV")) && ok;
		ok = !!(gl.getVertexAttribLui64vNV = GETFN_(proto::GetVertexAttribLui64vNVFn, "glGetVertexAttribLui64vNV")) && ok;
		ok = !!(gl.vertexAttribL1i64NV = GETFN_(proto::VertexAttribL1i64NVFn, "glVertexAttribL1i64NV")) && ok;
		ok = !!(gl.vertexAttribL1i64vNV = GETFN_(proto::VertexAttribL1i64vNVFn, "glVertexAttribL1i64vNV")) && ok;
		ok = !!(gl.vertexAttribL1ui64NV = GETFN_(proto::VertexAttribL1ui64NVFn, "glVertexAttribL1ui64NV")) && ok;
		ok = !!(gl.vertexAttribL1ui64vNV = GETFN_(proto::VertexAttribL1ui64vNVFn, "glVertexAttribL1ui64vNV")) && ok;
		ok = !!(gl.vertexAttribL2i64NV = GETFN_(proto::VertexAttribL2i64NVFn, "glVertexAttribL2i64NV")) && ok;
		ok = !!(gl.vertexAttribL2i64vNV = GETFN_(proto::VertexAttribL2i64vNVFn, "glVertexAttribL2i64vNV")) && ok;
		ok = !!(gl.vertexAttribL2ui64NV = GETFN_(proto::VertexAttribL2ui64NVFn, "glVertexAttribL2ui64NV")) && ok;
		ok = !!(gl.vertexAttribL2ui64vNV = GETFN_(proto::VertexAttribL2ui64vNVFn, "glVertexAttribL2ui64vNV")) && ok;
		ok = !!(gl.vertexAttribL3i64NV = GETFN_(proto::VertexAttribL3i64NVFn, "glVertexAttribL3i64NV")) && ok;
		ok = !!(gl.vertexAttribL3i64vNV = GETFN_(proto::VertexAttribL3i64vNVFn, "glVertexAttribL3i64vNV")) && ok;
		ok = !!(gl.vertexAttribL3ui64NV = GETFN_(proto::VertexAttribL3ui64NVFn, "glVertexAttribL3ui64NV")) && ok;
		ok = !!(gl.vertexAttribL3ui64vNV = GETFN_(proto::VertexAttribL3ui64vNVFn, "glVertexAttribL3ui64vNV")) && ok;
		ok = !!(gl.vertexAttribL4i64NV = GETFN_(proto::VertexAttribL4i64NVFn, "glVertexAttribL4i64NV")) && ok;
		ok = !!(gl.vertexAttribL4i64vNV = GETFN_(proto::VertexAttribL4i64vNVFn, "glVertexAttribL4i64vNV")) && ok;
		ok = !!(gl.vertexAttribL4ui64NV = GETFN_(proto::VertexAttribL4ui64NVFn, "glVertexAttribL4ui64NV")) && ok;
		ok = !!(gl.vertexAttribL4ui64vNV = GETFN_(proto::VertexAttribL4ui64vNVFn, "glVertexAttribL4ui64vNV")) && ok;
		ok = !!(gl.vertexAttribLFormatNV = GETFN_(proto::VertexAttribLFormatNVFn, "glVertexAttribLFormatNV")) && ok;
		gl.NV_vertex_attrib_integer_64bit = ok;
	}
	//- end: NV_vertex_attrib_integer_64bit

	//-//-- 
	//- Ext: NV_vertex_buffer_unified_memory
	static char const* NV_vertex_buffer_unified_memory = "NV_vertex_buffer_unified_memory";
	if( listed_( arg.extList, NV_vertex_buffer_unified_memory ) && arg.checkExt( NV_vertex_buffer_unified_memory, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.bufferAddressRangeNV = GETFN_(proto::BufferAddressRangeNVFn, "glBufferAddressRangeNV")) && ok;
		ok = !!(gl.colorFormatNV = GETFN_(proto::ColorFormatNVFn, "glColorFormatNV")) && ok;
		ok = !!(gl.edgeFlagFormatNV = GETFN_(proto::EdgeFlagFormatNVFn, "glEdgeFlagFormatNV")) && ok;
		ok = !!(gl.fogCoordFormatNV = GETFN_(proto::FogCoordFormatNVFn, "glFogCoordFormatNV")) && ok;
		ok = !!(gl.getIntegerui64i_vNV = GETFN_(proto::GetIntegerui64i_vNVFn, "glGetIntegerui64i_vNV")) && ok;
		ok = !!(gl.indexFormatNV = GETFN_(proto::IndexFormatNVFn, "glIndexFormatNV")) && ok;
		ok = !!(gl.normalFormatNV = GETFN_(proto::NormalFormatNVFn, "glNormalFormatNV")) && ok;
		ok = !!(gl.secondaryColorFormatNV = GETFN_(proto::SecondaryColorFormatNVFn, "glSecondaryColorFormatNV")) && ok;
		ok = !!(gl.texCoordFormatNV = GETFN_(proto::TexCoordFormatNVFn, "glTexCoordFormatNV")) && ok;
		ok = !!(gl.vertexAttribFormatNV = GETFN_(proto::VertexAttribFormatNVFn, "glVertexAttribFormatNV")) && ok;
		ok = !!(gl.vertexAttribIFormatNV = GETFN_(proto::VertexAttribIFormatNVFn, "glVertexAttribIFormatNV")) && ok;
		ok = !!(gl.vertexFormatNV = GETFN_(proto::VertexFormatNVFn, "glVertexFormatNV")) && ok;
		gl.NV_vertex_buffer_unified_memory = ok;
	}
	//- end: NV_vertex_buffer_unified_memory

	//-//-- 
	//- Ext: NV_vertex_program
	static char const* NV_vertex_program = "NV_vertex_program";
	if( listed_( arg.extList, NV_vertex_program ) && arg.checkExt( NV_vertex_program, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.areProgramsResidentNV = GETFN_(proto::AreProgramsResidentNVFn, "glAreProgramsResidentNV")) && ok;
		ok = !!(gl.bindProgramNV = GETFN_(proto::BindProgramNVFn, "glBindProgramNV")) && ok;
		ok = !!(gl.deleteProgramsNV = GETFN_(proto::DeleteProgramsNVFn, "glDeleteProgramsNV")) && ok;
		ok = !!(gl.executeProgramNV = GETFN_(proto::ExecuteProgramNVFn, "glExecuteProgramNV")) && ok;
		ok = !!(gl.genProgramsNV = GETFN_(proto::GenProgramsNVFn, "glGenProgramsNV")) && ok;
		ok = !!(gl.getProgramParameterdvNV = GETFN_(proto::GetProgramParameterdvNVFn, "glGetProgramParameterdvNV")) && ok;
		ok = !!(gl.getProgramParameterfvNV = GETFN_(proto::GetProgramParameterfvNVFn, "glGetProgramParameterfvNV")) && ok;
		ok = !!(gl.getProgramStringNV = GETFN_(proto::GetProgramStringNVFn, "glGetProgramStringNV")) && ok;
		ok = !!(gl.getProgramivNV = GETFN_(proto::GetProgramivNVFn, "glGetProgramivNV")) && ok;
		ok = !!(gl.getTrackMatrixivNV = GETFN_(proto::GetTrackMatrixivNVFn, "glGetTrackMatrixivNV")) && ok;
		ok = !!(gl.getVertexAttribPointervNV = GETFN_(proto::GetVertexAttribPointervNVFn, "glGetVertexAttribPointervNV")) && ok;
		ok = !!(gl.getVertexAttribdvNV = GETFN_(proto::GetVertexAttribdvNVFn, "glGetVertexAttribdvNV")) && ok;
		ok = !!(gl.getVertexAttribfvNV = GETFN_(proto::GetVertexAttribfvNVFn, "glGetVertexAttribfvNV")) && ok;
		ok = !!(gl.getVertexAttribivNV = GETFN_(proto::GetVertexAttribivNVFn, "glGetVertexAttribivNV")) && ok;
		ok = !!(gl.isProgramNV = GETFN_(proto::IsProgramNVFn, "glIsProgramNV")) && ok;
		ok = !!(gl.loadProgramNV = GETFN_(proto::LoadProgramNVFn, "glLoadProgramNV")) && ok;
		ok = !!(gl.programParameter4dNV = GETFN_(proto::ProgramParameter4dNVFn, "glProgramParameter4dNV")) && ok;
		ok = !!(gl.programParameter4dvNV = GETFN_(proto::ProgramParameter4dvNVFn, "glProgramParameter4dvNV")) && ok;
		ok = !!(gl.programParameter4fNV = GETFN_(proto::ProgramParameter4fNVFn, "glProgramParameter4fNV")) && ok;
		ok = !!(gl.programParameter4fvNV = GETFN_(proto::ProgramParameter4fvNVFn, "glProgramParameter4fvNV")) && ok;
		ok = !!(gl.programParameters4dvNV = GETFN_(proto::ProgramParameters4dvNVFn, "glProgramParameters4dvNV")) && ok;
		ok = !!(gl.programParameters4fvNV = GETFN_(proto::ProgramParameters4fvNVFn, "glProgramParameters4fvNV")) && ok;
		ok = !!(gl.requestResidentProgramsNV = GETFN_(proto::RequestResidentProgramsNVFn, "glRequestResidentProgramsNV")) && ok;
		ok = !!(gl.trackMatrixNV = GETFN_(proto::TrackMatrixNVFn, "glTrackMatrixNV")) && ok;
		ok = !!(gl.vertexAttrib1dNV = GETFN_(proto::VertexAttrib1dNVFn, "glVertexAttrib1dNV")) && ok;
		ok = !!(gl.vertexAttrib1dvNV = GETFN_(proto::VertexAttrib1dvNVFn, "glVertexAttrib1dvNV")) && ok;
		ok = !!(gl.vertexAttrib1fNV = GETFN_(proto::VertexAttrib1fNVFn, "glVertexAttrib1fNV")) && ok;
		ok = !!(gl.vertexAttrib1fvNV = GETFN_(proto::VertexAttrib1fvNVFn, "glVertexAttrib1fvNV")) && ok;
		ok = !!(gl.vertexAttrib1sNV = GETFN_(proto::VertexAttrib1sNVFn, "glVertexAttrib1sNV")) && ok;
		ok = !!(gl.vertexAttrib1svNV = GETFN_(proto::VertexAttrib1svNVFn, "glVertexAttrib1svNV")) && ok;
		ok = !!(gl.vertexAttrib2dNV = GETFN_(proto::VertexAttrib2dNVFn, "glVertexAttrib2dNV")) && ok;
		ok = !!(gl.vertexAttrib2dvNV = GETFN_(proto::VertexAttrib2dvNVFn, "glVertexAttrib2dvNV")) && ok;
		ok = !!(gl.vertexAttrib2fNV = GETFN_(proto::VertexAttrib2fNVFn, "glVertexAttrib2fNV")) && ok;
		ok = !!(gl.vertexAttrib2fvNV = GETFN_(proto::VertexAttrib2fvNVFn, "glVertexAttrib2fvNV")) && ok;
		ok = !!(gl.vertexAttrib2sNV = GETFN_(proto::VertexAttrib2sNVFn, "glVertexAttrib2sNV")) && ok;
		ok = !!(gl.vertexAttrib2svNV = GETFN_(proto::VertexAttrib2svNVFn, "glVertexAttrib2svNV")) && ok;
		ok = !!(gl.vertexAttrib3dNV = GETFN_(proto::VertexAttrib3dNVFn, "glVertexAttrib3dNV")) && ok;
		ok = !!(gl.vertexAttrib3dvNV = GETFN_(proto::VertexAttrib3dvNVFn, "glVertexAttrib3dvNV")) && ok;
		ok = !!(gl.vertexAttrib3fNV = GETFN_(proto::VertexAttrib3fNVFn, "glVertexAttrib3fNV")) && ok;
		ok = !!(gl.vertexAttrib3fvNV = GETFN_(proto::VertexAttrib3fvNVFn, "glVertexAttrib3fvNV")) && ok;
		ok = !!(gl.vertexAttrib3sNV = GETFN_(proto::VertexAttrib3sNVFn, "glVertexAttrib3sNV")) && ok;
		ok = !!(gl.vertexAttrib3svNV = GETFN_(proto::VertexAttrib3svNVFn, "glVertexAttrib3svNV")) && ok;
		ok = !!(gl.vertexAttrib4dNV = GETFN_(proto::VertexAttrib4dNVFn, "glVertexAttrib4dNV")) && ok;
		ok = !!(gl.vertexAttrib4dvNV = GETFN_(proto::VertexAttrib4dvNVFn, "glVertexAttrib4dvNV")) && ok;
		ok = !!(gl.vertexAttrib4fNV = GETFN_(proto::VertexAttrib4fNVFn, "glVertexAttrib4fNV")) && ok;
		ok = !!(gl.vertexAttrib4fvNV = GETFN_(proto::VertexAttrib4fvNVFn, "glVertexAttrib4fvNV")) && ok;
		ok = !!(gl.vertexAttrib4sNV = GETFN_(proto::VertexAttrib4sNVFn, "glVertexAttrib4sNV")) && ok;
		ok = !!(gl.vertexAttrib4svNV = GETFN_(proto::VertexAttrib4svNVFn, "glVertexAttrib4svNV")) && ok;
		ok = !!(gl.vertexAttrib4ubNV = GETFN_(proto::VertexAttrib4ubNVFn, "glVertexAttrib4ubNV")) && ok;
		ok = !!(gl.vertexAttrib4ubvNV = GETFN_(proto::VertexAttrib4ubvNVFn, "glVertexAttrib4ubvNV")) && ok;
		ok = !!(gl.vertexAttribPointerNV = GETFN_(proto::VertexAttribPointerNVFn, "glVertexAttribPointerNV")) && ok;
		ok = !!(gl.vertexAttribs1dvNV = GETFN_(proto::VertexAttribs1dvNVFn, "glVertexAttribs1dvNV")) && ok;
		ok = !!(gl.vertexAttribs1fvNV = GETFN_(proto::VertexAttribs1fvNVFn, "glVertexAttribs1fvNV")) && ok;
		ok = !!(gl.vertexAttribs1svNV = GETFN_(proto::VertexAttribs1svNVFn, "glVertexAttribs1svNV")) && ok;
		ok = !!(gl.vertexAttribs2dvNV = GETFN_(proto::VertexAttribs2dvNVFn, "glVertexAttribs2dvNV")) && ok;
		ok = !!(gl.vertexAttribs2fvNV = GETFN_(proto::VertexAttribs2fvNVFn, "glVertexAttribs2fvNV")) && ok;
		ok = !!(gl.vertexAttribs2svNV = GETFN_(proto::VertexAttribs2svNVFn, "glVertexAttribs2svNV")) && ok;
		ok = !!(gl.vertexAttribs3dvNV = GETFN_(proto::VertexAttribs3dvNVFn, "glVertexAttribs3dvNV")) && ok;
		ok = !!(gl.vertexAttribs3fvNV = GETFN_(proto::VertexAttribs3fvNVFn, "glVertexAttribs3fvNV")) && ok;
		ok = !!(gl.vertexAttribs3svNV = GETFN_(proto::VertexAttribs3svNVFn, "glVertexAttribs3svNV")) && ok;
		ok = !!(gl.vertexAttribs4dvNV = GETFN_(proto::VertexAttribs4dvNVFn, "glVertexAttribs4dvNV")) && ok;
		ok = !!(gl.vertexAttribs4fvNV = GETFN_(proto::VertexAttribs4fvNVFn, "glVertexAttribs4fvNV")) && ok;
		ok = !!(gl.vertexAttribs4svNV = GETFN_(proto::VertexAttribs4svNVFn, "glVertexAttribs4svNV")) && ok;
		ok = !!(gl.vertexAttribs4ubvNV = GETFN_(proto::VertexAttribs4ubvNVFn, "glVertexAttribs4ubvNV")) && ok;
		gl.NV_vertex_program = ok;
	}
	//- end: NV_vertex_program

	//-//-- 
	//- Ext: NV_vertex_program4
	static char const* NV_vertex_program4 = "NV_vertex_program4";
	if( listed_( arg.extList, NV_vertex_program4 ) && arg.checkExt( NV_vertex_program4, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getVertexAttribIivEXT = GETFN_(proto::GetVertexAttribIivEXTFn, "glGetVertexAttribIivEXT")) && ok;
		ok = !!(gl.getVertexAttribIuivEXT = GETFN_(proto::GetVertexAttribIuivEXTFn, "glGetVertexAttribIuivEXT")) && ok;
		ok = !!(gl.vertexAttribI1iEXT = GETFN_(proto::VertexAttribI1iEXTFn, "glVertexAttribI1iEXT")) && ok;
		ok = !!(gl.vertexAttribI1ivEXT = GETFN_(proto::VertexAttribI1ivEXTFn, "glVertexAttribI1ivEXT")) && ok;
		ok = !!(gl.vertexAttribI1uiEXT = GETFN_(proto::VertexAttribI1uiEXTFn, "glVertexAttribI1uiEXT")) && ok;
		ok = !!(gl.vertexAttribI1uivEXT = GETFN_(proto::VertexAttribI1uivEXTFn, "glVertexAttribI1uivEXT")) && ok;
		ok = !!(gl.vertexAttribI2iEXT = GETFN_(proto::VertexAttribI2iEXTFn, "glVertexAttribI2iEXT")) && ok;
		ok = !!(gl.vertexAttribI2ivEXT = GETFN_(proto::VertexAttribI2ivEXTFn, "glVertexAttribI2ivEXT")) && ok;
		ok = !!(gl.vertexAttribI2uiEXT = GETFN_(proto::VertexAttribI2uiEXTFn, "glVertexAttribI2uiEXT")) && ok;
		ok = !!(gl.vertexAttribI2uivEXT = GETFN_(proto::VertexAttribI2uivEXTFn, "glVertexAttribI2uivEXT")) && ok;
		ok = !!(gl.vertexAttribI3iEXT = GETFN_(proto::VertexAttribI3iEXTFn, "glVertexAttribI3iEXT")) && ok;
		ok = !!(gl.vertexAttribI3ivEXT = GETFN_(proto::VertexAttribI3ivEXTFn, "glVertexAttribI3ivEXT")) && ok;
		ok = !!(gl.vertexAttribI3uiEXT = GETFN_(proto::VertexAttribI3uiEXTFn, "glVertexAttribI3uiEXT")) && ok;
		ok = !!(gl.vertexAttribI3uivEXT = GETFN_(proto::VertexAttribI3uivEXTFn, "glVertexAttribI3uivEXT")) && ok;
		ok = !!(gl.vertexAttribI4bvEXT = GETFN_(proto::VertexAttribI4bvEXTFn, "glVertexAttribI4bvEXT")) && ok;
		ok = !!(gl.vertexAttribI4iEXT = GETFN_(proto::VertexAttribI4iEXTFn, "glVertexAttribI4iEXT")) && ok;
		ok = !!(gl.vertexAttribI4ivEXT = GETFN_(proto::VertexAttribI4ivEXTFn, "glVertexAttribI4ivEXT")) && ok;
		ok = !!(gl.vertexAttribI4svEXT = GETFN_(proto::VertexAttribI4svEXTFn, "glVertexAttribI4svEXT")) && ok;
		ok = !!(gl.vertexAttribI4ubvEXT = GETFN_(proto::VertexAttribI4ubvEXTFn, "glVertexAttribI4ubvEXT")) && ok;
		ok = !!(gl.vertexAttribI4uiEXT = GETFN_(proto::VertexAttribI4uiEXTFn, "glVertexAttribI4uiEXT")) && ok;
		ok = !!(gl.vertexAttribI4uivEXT = GETFN_(proto::VertexAttribI4uivEXTFn, "glVertexAttribI4uivEXT")) && ok;
		ok = !!(gl.vertexAttribI4usvEXT = GETFN_(proto::VertexAttribI4usvEXTFn, "glVertexAttribI4usvEXT")) && ok;
		ok = !!(gl.vertexAttribIPointerEXT = GETFN_(proto::VertexAttribIPointerEXTFn, "glVertexAttribIPointerEXT")) && ok;
		gl.NV_vertex_program4 = ok;
	}
	//- end: NV_vertex_program4

	//-//-- 
	//- Ext: NV_video_capture
	static char const* NV_video_capture = "NV_video_capture";
	if( listed_( arg.extList, NV_video_capture ) && arg.checkExt( NV_video_capture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.beginVideoCaptureNV = GETFN_(proto::BeginVideoCaptureNVFn, "glBeginVideoCaptureNV")) && ok;
		ok = !!(gl.bindVideoCaptureStreamBufferNV = GETFN_(proto::BindVideoCaptureStreamBufferNVFn, "glBindVideoCaptureStreamBufferNV")) && ok;
		ok = !!(gl.bindVideoCaptureStreamTextureNV = GETFN_(proto::BindVideoCaptureStreamTextureNVFn, "glBindVideoCaptureStreamTextureNV")) && ok;
		ok = !!(gl.endVideoCaptureNV = GETFN_(proto::EndVideoCaptureNVFn, "glEndVideoCaptureNV")) && ok;
		ok = !!(gl.getVideoCaptureStreamdvNV = GETFN_(proto::GetVideoCaptureStreamdvNVFn, "glGetVideoCaptureStreamdvNV")) && ok;
		ok = !!(gl.getVideoCaptureStreamfvNV = GETFN_(proto::GetVideoCaptureStreamfvNVFn, "glGetVideoCaptureStreamfvNV")) && ok;
		ok = !!(gl.getVideoCaptureStreamivNV = GETFN_(proto::GetVideoCaptureStreamivNVFn, "glGetVideoCaptureStreamivNV")) && ok;
		ok = !!(gl.getVideoCaptureivNV = GETFN_(proto::GetVideoCaptureivNVFn, "glGetVideoCaptureivNV")) && ok;
		ok = !!(gl.videoCaptureNV = GETFN_(proto::VideoCaptureNVFn, "glVideoCaptureNV")) && ok;
		ok = !!(gl.videoCaptureStreamParameterdvNV = GETFN_(proto::VideoCaptureStreamParameterdvNVFn, "glVideoCaptureStreamParameterdvNV")) && ok;
		ok = !!(gl.videoCaptureStreamParameterfvNV = GETFN_(proto::VideoCaptureStreamParameterfvNVFn, "glVideoCaptureStreamParameterfvNV")) && ok;
		ok = !!(gl.videoCaptureStreamParameterivNV = GETFN_(proto::VideoCaptureStreamParameterivNVFn, "glVideoCaptureStreamParameterivNV")) && ok;
		gl.NV_video_capture = ok;
	}
	//- end: NV_video_capture

}

#undef GETFN_
//--    <<< ~ dlapi::gl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE3(dlapi,gl,detail)
//--//////////////////////////////////////////////////////////////////////////
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
