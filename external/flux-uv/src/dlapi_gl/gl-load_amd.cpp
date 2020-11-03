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

void load_gl_amd_( GLapi& gl, LoaderArg_ const& arg )
{
	//-//-- 
	//- Ext: AMD_debug_output
	static char const* AMD_debug_output = "AMD_debug_output";
	if( listed_( arg.extList, AMD_debug_output ) && arg.checkExt( AMD_debug_output, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.debugMessageCallbackAMD = GETFN_(proto::DebugMessageCallbackAMDFn, "glDebugMessageCallbackAMD")) && ok;
		ok = !!(gl.debugMessageEnableAMD = GETFN_(proto::DebugMessageEnableAMDFn, "glDebugMessageEnableAMD")) && ok;
		ok = !!(gl.debugMessageInsertAMD = GETFN_(proto::DebugMessageInsertAMDFn, "glDebugMessageInsertAMD")) && ok;
		ok = !!(gl.getDebugMessageLogAMD = GETFN_(proto::GetDebugMessageLogAMDFn, "glGetDebugMessageLogAMD")) && ok;
		gl.AMD_debug_output = ok;
	}
	//- end: AMD_debug_output

	//-//-- 
	//- Ext: AMD_draw_buffers_blend
	static char const* AMD_draw_buffers_blend = "AMD_draw_buffers_blend";
	if( listed_( arg.extList, AMD_draw_buffers_blend ) && arg.checkExt( AMD_draw_buffers_blend, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.blendEquationIndexedAMD = GETFN_(proto::BlendEquationIndexedAMDFn, "glBlendEquationIndexedAMD")) && ok;
		ok = !!(gl.blendEquationSeparateIndexedAMD = GETFN_(proto::BlendEquationSeparateIndexedAMDFn, "glBlendEquationSeparateIndexedAMD")) && ok;
		ok = !!(gl.blendFuncIndexedAMD = GETFN_(proto::BlendFuncIndexedAMDFn, "glBlendFuncIndexedAMD")) && ok;
		ok = !!(gl.blendFuncSeparateIndexedAMD = GETFN_(proto::BlendFuncSeparateIndexedAMDFn, "glBlendFuncSeparateIndexedAMD")) && ok;
		gl.AMD_draw_buffers_blend = ok;
	}
	//- end: AMD_draw_buffers_blend

	//-//-- 
	//- Ext: AMD_gpu_shader_int64
	static char const* AMD_gpu_shader_int64 = "AMD_gpu_shader_int64";
	if( listed_( arg.extList, AMD_gpu_shader_int64 ) && arg.checkExt( AMD_gpu_shader_int64, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getUniformi64vNV = GETFN_(proto::GetUniformi64vNVFn, "glGetUniformi64vNV")) && ok;
		ok = !!(gl.getUniformui64vNV = GETFN_(proto::GetUniformui64vNVFn, "glGetUniformui64vNV")) && ok;
		ok = !!(gl.programUniform1i64NV = GETFN_(proto::ProgramUniform1i64NVFn, "glProgramUniform1i64NV")) && ok;
		ok = !!(gl.programUniform1i64vNV = GETFN_(proto::ProgramUniform1i64vNVFn, "glProgramUniform1i64vNV")) && ok;
		ok = !!(gl.programUniform1ui64NV = GETFN_(proto::ProgramUniform1ui64NVFn, "glProgramUniform1ui64NV")) && ok;
		ok = !!(gl.programUniform1ui64vNV = GETFN_(proto::ProgramUniform1ui64vNVFn, "glProgramUniform1ui64vNV")) && ok;
		ok = !!(gl.programUniform2i64NV = GETFN_(proto::ProgramUniform2i64NVFn, "glProgramUniform2i64NV")) && ok;
		ok = !!(gl.programUniform2i64vNV = GETFN_(proto::ProgramUniform2i64vNVFn, "glProgramUniform2i64vNV")) && ok;
		ok = !!(gl.programUniform2ui64NV = GETFN_(proto::ProgramUniform2ui64NVFn, "glProgramUniform2ui64NV")) && ok;
		ok = !!(gl.programUniform2ui64vNV = GETFN_(proto::ProgramUniform2ui64vNVFn, "glProgramUniform2ui64vNV")) && ok;
		ok = !!(gl.programUniform3i64NV = GETFN_(proto::ProgramUniform3i64NVFn, "glProgramUniform3i64NV")) && ok;
		ok = !!(gl.programUniform3i64vNV = GETFN_(proto::ProgramUniform3i64vNVFn, "glProgramUniform3i64vNV")) && ok;
		ok = !!(gl.programUniform3ui64NV = GETFN_(proto::ProgramUniform3ui64NVFn, "glProgramUniform3ui64NV")) && ok;
		ok = !!(gl.programUniform3ui64vNV = GETFN_(proto::ProgramUniform3ui64vNVFn, "glProgramUniform3ui64vNV")) && ok;
		ok = !!(gl.programUniform4i64NV = GETFN_(proto::ProgramUniform4i64NVFn, "glProgramUniform4i64NV")) && ok;
		ok = !!(gl.programUniform4i64vNV = GETFN_(proto::ProgramUniform4i64vNVFn, "glProgramUniform4i64vNV")) && ok;
		ok = !!(gl.programUniform4ui64NV = GETFN_(proto::ProgramUniform4ui64NVFn, "glProgramUniform4ui64NV")) && ok;
		ok = !!(gl.programUniform4ui64vNV = GETFN_(proto::ProgramUniform4ui64vNVFn, "glProgramUniform4ui64vNV")) && ok;
		ok = !!(gl.uniform1i64NV = GETFN_(proto::Uniform1i64NVFn, "glUniform1i64NV")) && ok;
		ok = !!(gl.uniform1i64vNV = GETFN_(proto::Uniform1i64vNVFn, "glUniform1i64vNV")) && ok;
		ok = !!(gl.uniform1ui64NV = GETFN_(proto::Uniform1ui64NVFn, "glUniform1ui64NV")) && ok;
		ok = !!(gl.uniform1ui64vNV = GETFN_(proto::Uniform1ui64vNVFn, "glUniform1ui64vNV")) && ok;
		ok = !!(gl.uniform2i64NV = GETFN_(proto::Uniform2i64NVFn, "glUniform2i64NV")) && ok;
		ok = !!(gl.uniform2i64vNV = GETFN_(proto::Uniform2i64vNVFn, "glUniform2i64vNV")) && ok;
		ok = !!(gl.uniform2ui64NV = GETFN_(proto::Uniform2ui64NVFn, "glUniform2ui64NV")) && ok;
		ok = !!(gl.uniform2ui64vNV = GETFN_(proto::Uniform2ui64vNVFn, "glUniform2ui64vNV")) && ok;
		ok = !!(gl.uniform3i64NV = GETFN_(proto::Uniform3i64NVFn, "glUniform3i64NV")) && ok;
		ok = !!(gl.uniform3i64vNV = GETFN_(proto::Uniform3i64vNVFn, "glUniform3i64vNV")) && ok;
		ok = !!(gl.uniform3ui64NV = GETFN_(proto::Uniform3ui64NVFn, "glUniform3ui64NV")) && ok;
		ok = !!(gl.uniform3ui64vNV = GETFN_(proto::Uniform3ui64vNVFn, "glUniform3ui64vNV")) && ok;
		ok = !!(gl.uniform4i64NV = GETFN_(proto::Uniform4i64NVFn, "glUniform4i64NV")) && ok;
		ok = !!(gl.uniform4i64vNV = GETFN_(proto::Uniform4i64vNVFn, "glUniform4i64vNV")) && ok;
		ok = !!(gl.uniform4ui64NV = GETFN_(proto::Uniform4ui64NVFn, "glUniform4ui64NV")) && ok;
		ok = !!(gl.uniform4ui64vNV = GETFN_(proto::Uniform4ui64vNVFn, "glUniform4ui64vNV")) && ok;
		gl.AMD_gpu_shader_int64 = ok;
	}
	//- end: AMD_gpu_shader_int64

	//-//-- 
	//- Ext: AMD_interleaved_elements
	static char const* AMD_interleaved_elements = "AMD_interleaved_elements";
	if( listed_( arg.extList, AMD_interleaved_elements ) && arg.checkExt( AMD_interleaved_elements, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.vertexAttribParameteriAMD = GETFN_(proto::VertexAttribParameteriAMDFn, "glVertexAttribParameteriAMD")) && ok;
		gl.AMD_interleaved_elements = ok;
	}
	//- end: AMD_interleaved_elements

	//-//-- 
	//- Ext: AMD_multi_draw_indirect
	static char const* AMD_multi_draw_indirect = "AMD_multi_draw_indirect";
	if( listed_( arg.extList, AMD_multi_draw_indirect ) && arg.checkExt( AMD_multi_draw_indirect, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.multiDrawArraysIndirectAMD = GETFN_(proto::MultiDrawArraysIndirectAMDFn, "glMultiDrawArraysIndirectAMD")) && ok;
		ok = !!(gl.multiDrawElementsIndirectAMD = GETFN_(proto::MultiDrawElementsIndirectAMDFn, "glMultiDrawElementsIndirectAMD")) && ok;
		gl.AMD_multi_draw_indirect = ok;
	}
	//- end: AMD_multi_draw_indirect

	//-//-- 
	//- Ext: AMD_name_gen_delete
	static char const* AMD_name_gen_delete = "AMD_name_gen_delete";
	if( listed_( arg.extList, AMD_name_gen_delete ) && arg.checkExt( AMD_name_gen_delete, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.deleteNamesAMD = GETFN_(proto::DeleteNamesAMDFn, "glDeleteNamesAMD")) && ok;
		ok = !!(gl.genNamesAMD = GETFN_(proto::GenNamesAMDFn, "glGenNamesAMD")) && ok;
		ok = !!(gl.isNameAMD = GETFN_(proto::IsNameAMDFn, "glIsNameAMD")) && ok;
		gl.AMD_name_gen_delete = ok;
	}
	//- end: AMD_name_gen_delete

	//-//-- 
	//- Ext: AMD_occlusion_query_event
	static char const* AMD_occlusion_query_event = "AMD_occlusion_query_event";
	if( listed_( arg.extList, AMD_occlusion_query_event ) && arg.checkExt( AMD_occlusion_query_event, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.queryObjectParameteruiAMD = GETFN_(proto::QueryObjectParameteruiAMDFn, "glQueryObjectParameteruiAMD")) && ok;
		gl.AMD_occlusion_query_event = ok;
	}
	//- end: AMD_occlusion_query_event

	//-//-- 
	//- Ext: AMD_performance_monitor
	static char const* AMD_performance_monitor = "AMD_performance_monitor";
	if( listed_( arg.extList, AMD_performance_monitor ) && arg.checkExt( AMD_performance_monitor, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.beginPerfMonitorAMD = GETFN_(proto::BeginPerfMonitorAMDFn, "glBeginPerfMonitorAMD")) && ok;
		ok = !!(gl.deletePerfMonitorsAMD = GETFN_(proto::DeletePerfMonitorsAMDFn, "glDeletePerfMonitorsAMD")) && ok;
		ok = !!(gl.endPerfMonitorAMD = GETFN_(proto::EndPerfMonitorAMDFn, "glEndPerfMonitorAMD")) && ok;
		ok = !!(gl.genPerfMonitorsAMD = GETFN_(proto::GenPerfMonitorsAMDFn, "glGenPerfMonitorsAMD")) && ok;
		ok = !!(gl.getPerfMonitorCounterDataAMD = GETFN_(proto::GetPerfMonitorCounterDataAMDFn, "glGetPerfMonitorCounterDataAMD")) && ok;
		ok = !!(gl.getPerfMonitorCounterInfoAMD = GETFN_(proto::GetPerfMonitorCounterInfoAMDFn, "glGetPerfMonitorCounterInfoAMD")) && ok;
		ok = !!(gl.getPerfMonitorCounterStringAMD = GETFN_(proto::GetPerfMonitorCounterStringAMDFn, "glGetPerfMonitorCounterStringAMD")) && ok;
		ok = !!(gl.getPerfMonitorCountersAMD = GETFN_(proto::GetPerfMonitorCountersAMDFn, "glGetPerfMonitorCountersAMD")) && ok;
		ok = !!(gl.getPerfMonitorGroupStringAMD = GETFN_(proto::GetPerfMonitorGroupStringAMDFn, "glGetPerfMonitorGroupStringAMD")) && ok;
		ok = !!(gl.getPerfMonitorGroupsAMD = GETFN_(proto::GetPerfMonitorGroupsAMDFn, "glGetPerfMonitorGroupsAMD")) && ok;
		ok = !!(gl.selectPerfMonitorCountersAMD = GETFN_(proto::SelectPerfMonitorCountersAMDFn, "glSelectPerfMonitorCountersAMD")) && ok;
		gl.AMD_performance_monitor = ok;
	}
	//- end: AMD_performance_monitor

	//-//-- 
	//- Ext: AMD_sample_positions
	static char const* AMD_sample_positions = "AMD_sample_positions";
	if( listed_( arg.extList, AMD_sample_positions ) && arg.checkExt( AMD_sample_positions, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.setMultisamplefvAMD = GETFN_(proto::SetMultisamplefvAMDFn, "glSetMultisamplefvAMD")) && ok;
		gl.AMD_sample_positions = ok;
	}
	//- end: AMD_sample_positions

	//-//-- 
	//- Ext: AMD_sparse_texture
	static char const* AMD_sparse_texture = "AMD_sparse_texture";
	if( listed_( arg.extList, AMD_sparse_texture ) && arg.checkExt( AMD_sparse_texture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.texStorageSparseAMD = GETFN_(proto::TexStorageSparseAMDFn, "glTexStorageSparseAMD")) && ok;
		ok = !!(gl.textureStorageSparseAMD = GETFN_(proto::TextureStorageSparseAMDFn, "glTextureStorageSparseAMD")) && ok;
		gl.AMD_sparse_texture = ok;
	}
	//- end: AMD_sparse_texture

	//-//-- 
	//- Ext: AMD_stencil_operation_extended
	static char const* AMD_stencil_operation_extended = "AMD_stencil_operation_extended";
	if( listed_( arg.extList, AMD_stencil_operation_extended ) && arg.checkExt( AMD_stencil_operation_extended, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.stencilOpValueAMD = GETFN_(proto::StencilOpValueAMDFn, "glStencilOpValueAMD")) && ok;
		gl.AMD_stencil_operation_extended = ok;
	}
	//- end: AMD_stencil_operation_extended

	//-//-- 
	//- Ext: AMD_vertex_shader_tessellator
	static char const* AMD_vertex_shader_tessellator = "AMD_vertex_shader_tessellator";
	if( listed_( arg.extList, AMD_vertex_shader_tessellator ) && arg.checkExt( AMD_vertex_shader_tessellator, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.tessellationFactorAMD = GETFN_(proto::TessellationFactorAMDFn, "glTessellationFactorAMD")) && ok;
		ok = !!(gl.tessellationModeAMD = GETFN_(proto::TessellationModeAMDFn, "glTessellationModeAMD")) && ok;
		gl.AMD_vertex_shader_tessellator = ok;
	}
	//- end: AMD_vertex_shader_tessellator

	//-//-- 
	//- Ext: ATI_draw_buffers
	static char const* ATI_draw_buffers = "ATI_draw_buffers";
	if( listed_( arg.extList, ATI_draw_buffers ) && arg.checkExt( ATI_draw_buffers, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.drawBuffersATI = GETFN_(proto::DrawBuffersATIFn, "glDrawBuffersATI")) && ok;
		gl.ATI_draw_buffers = ok;
	}
	//- end: ATI_draw_buffers

	//-//-- 
	//- Ext: ATI_element_array
	static char const* ATI_element_array = "ATI_element_array";
	if( listed_( arg.extList, ATI_element_array ) && arg.checkExt( ATI_element_array, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.drawElementArrayATI = GETFN_(proto::DrawElementArrayATIFn, "glDrawElementArrayATI")) && ok;
		ok = !!(gl.drawRangeElementArrayATI = GETFN_(proto::DrawRangeElementArrayATIFn, "glDrawRangeElementArrayATI")) && ok;
		ok = !!(gl.elementPointerATI = GETFN_(proto::ElementPointerATIFn, "glElementPointerATI")) && ok;
		gl.ATI_element_array = ok;
	}
	//- end: ATI_element_array

	//-//-- 
	//- Ext: ATI_envmap_bumpmap
	static char const* ATI_envmap_bumpmap = "ATI_envmap_bumpmap";
	if( listed_( arg.extList, ATI_envmap_bumpmap ) && arg.checkExt( ATI_envmap_bumpmap, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getTexBumpParameterfvATI = GETFN_(proto::GetTexBumpParameterfvATIFn, "glGetTexBumpParameterfvATI")) && ok;
		ok = !!(gl.getTexBumpParameterivATI = GETFN_(proto::GetTexBumpParameterivATIFn, "glGetTexBumpParameterivATI")) && ok;
		ok = !!(gl.texBumpParameterfvATI = GETFN_(proto::TexBumpParameterfvATIFn, "glTexBumpParameterfvATI")) && ok;
		ok = !!(gl.texBumpParameterivATI = GETFN_(proto::TexBumpParameterivATIFn, "glTexBumpParameterivATI")) && ok;
		gl.ATI_envmap_bumpmap = ok;
	}
	//- end: ATI_envmap_bumpmap

	//-//-- 
	//- Ext: ATI_fragment_shader
	static char const* ATI_fragment_shader = "ATI_fragment_shader";
	if( listed_( arg.extList, ATI_fragment_shader ) && arg.checkExt( ATI_fragment_shader, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.alphaFragmentOp1ATI = GETFN_(proto::AlphaFragmentOp1ATIFn, "glAlphaFragmentOp1ATI")) && ok;
		ok = !!(gl.alphaFragmentOp2ATI = GETFN_(proto::AlphaFragmentOp2ATIFn, "glAlphaFragmentOp2ATI")) && ok;
		ok = !!(gl.alphaFragmentOp3ATI = GETFN_(proto::AlphaFragmentOp3ATIFn, "glAlphaFragmentOp3ATI")) && ok;
		ok = !!(gl.beginFragmentShaderATI = GETFN_(proto::BeginFragmentShaderATIFn, "glBeginFragmentShaderATI")) && ok;
		ok = !!(gl.bindFragmentShaderATI = GETFN_(proto::BindFragmentShaderATIFn, "glBindFragmentShaderATI")) && ok;
		ok = !!(gl.colorFragmentOp1ATI = GETFN_(proto::ColorFragmentOp1ATIFn, "glColorFragmentOp1ATI")) && ok;
		ok = !!(gl.colorFragmentOp2ATI = GETFN_(proto::ColorFragmentOp2ATIFn, "glColorFragmentOp2ATI")) && ok;
		ok = !!(gl.colorFragmentOp3ATI = GETFN_(proto::ColorFragmentOp3ATIFn, "glColorFragmentOp3ATI")) && ok;
		ok = !!(gl.deleteFragmentShaderATI = GETFN_(proto::DeleteFragmentShaderATIFn, "glDeleteFragmentShaderATI")) && ok;
		ok = !!(gl.endFragmentShaderATI = GETFN_(proto::EndFragmentShaderATIFn, "glEndFragmentShaderATI")) && ok;
		ok = !!(gl.genFragmentShadersATI = GETFN_(proto::GenFragmentShadersATIFn, "glGenFragmentShadersATI")) && ok;
		ok = !!(gl.passTexCoordATI = GETFN_(proto::PassTexCoordATIFn, "glPassTexCoordATI")) && ok;
		ok = !!(gl.sampleMapATI = GETFN_(proto::SampleMapATIFn, "glSampleMapATI")) && ok;
		ok = !!(gl.setFragmentShaderConstantATI = GETFN_(proto::SetFragmentShaderConstantATIFn, "glSetFragmentShaderConstantATI")) && ok;
		gl.ATI_fragment_shader = ok;
	}
	//- end: ATI_fragment_shader

	//-//-- 
	//- Ext: ATI_map_object_buffer
	static char const* ATI_map_object_buffer = "ATI_map_object_buffer";
	if( listed_( arg.extList, ATI_map_object_buffer ) && arg.checkExt( ATI_map_object_buffer, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.mapObjectBufferATI = GETFN_(proto::MapObjectBufferATIFn, "glMapObjectBufferATI")) && ok;
		ok = !!(gl.unmapObjectBufferATI = GETFN_(proto::UnmapObjectBufferATIFn, "glUnmapObjectBufferATI")) && ok;
		gl.ATI_map_object_buffer = ok;
	}
	//- end: ATI_map_object_buffer

	//-//-- 
	//- Ext: ATI_pn_triangles
	static char const* ATI_pn_triangles = "ATI_pn_triangles";
	if( listed_( arg.extList, ATI_pn_triangles ) && arg.checkExt( ATI_pn_triangles, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.pNTrianglesfATI = GETFN_(proto::PNTrianglesfATIFn, "glPNTrianglesfATI")) && ok;
		ok = !!(gl.pNTrianglesiATI = GETFN_(proto::PNTrianglesiATIFn, "glPNTrianglesiATI")) && ok;
		gl.ATI_pn_triangles = ok;
	}
	//- end: ATI_pn_triangles

	//-//-- 
	//- Ext: ATI_separate_stencil
	static char const* ATI_separate_stencil = "ATI_separate_stencil";
	if( listed_( arg.extList, ATI_separate_stencil ) && arg.checkExt( ATI_separate_stencil, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.stencilFuncSeparateATI = GETFN_(proto::StencilFuncSeparateATIFn, "glStencilFuncSeparateATI")) && ok;
		ok = !!(gl.stencilOpSeparateATI = GETFN_(proto::StencilOpSeparateATIFn, "glStencilOpSeparateATI")) && ok;
		gl.ATI_separate_stencil = ok;
	}
	//- end: ATI_separate_stencil

	//-//-- 
	//- Ext: ATI_vertex_array_object
	static char const* ATI_vertex_array_object = "ATI_vertex_array_object";
	if( listed_( arg.extList, ATI_vertex_array_object ) && arg.checkExt( ATI_vertex_array_object, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.arrayObjectATI = GETFN_(proto::ArrayObjectATIFn, "glArrayObjectATI")) && ok;
		ok = !!(gl.freeObjectBufferATI = GETFN_(proto::FreeObjectBufferATIFn, "glFreeObjectBufferATI")) && ok;
		ok = !!(gl.getArrayObjectfvATI = GETFN_(proto::GetArrayObjectfvATIFn, "glGetArrayObjectfvATI")) && ok;
		ok = !!(gl.getArrayObjectivATI = GETFN_(proto::GetArrayObjectivATIFn, "glGetArrayObjectivATI")) && ok;
		ok = !!(gl.getObjectBufferfvATI = GETFN_(proto::GetObjectBufferfvATIFn, "glGetObjectBufferfvATI")) && ok;
		ok = !!(gl.getObjectBufferivATI = GETFN_(proto::GetObjectBufferivATIFn, "glGetObjectBufferivATI")) && ok;
		ok = !!(gl.getVariantArrayObjectfvATI = GETFN_(proto::GetVariantArrayObjectfvATIFn, "glGetVariantArrayObjectfvATI")) && ok;
		ok = !!(gl.getVariantArrayObjectivATI = GETFN_(proto::GetVariantArrayObjectivATIFn, "glGetVariantArrayObjectivATI")) && ok;
		ok = !!(gl.isObjectBufferATI = GETFN_(proto::IsObjectBufferATIFn, "glIsObjectBufferATI")) && ok;
		ok = !!(gl.newObjectBufferATI = GETFN_(proto::NewObjectBufferATIFn, "glNewObjectBufferATI")) && ok;
		ok = !!(gl.updateObjectBufferATI = GETFN_(proto::UpdateObjectBufferATIFn, "glUpdateObjectBufferATI")) && ok;
		ok = !!(gl.variantArrayObjectATI = GETFN_(proto::VariantArrayObjectATIFn, "glVariantArrayObjectATI")) && ok;
		gl.ATI_vertex_array_object = ok;
	}
	//- end: ATI_vertex_array_object

	//-//-- 
	//- Ext: ATI_vertex_attrib_array_object
	static char const* ATI_vertex_attrib_array_object = "ATI_vertex_attrib_array_object";
	if( listed_( arg.extList, ATI_vertex_attrib_array_object ) && arg.checkExt( ATI_vertex_attrib_array_object, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getVertexAttribArrayObjectfvATI = GETFN_(proto::GetVertexAttribArrayObjectfvATIFn, "glGetVertexAttribArrayObjectfvATI")) && ok;
		ok = !!(gl.getVertexAttribArrayObjectivATI = GETFN_(proto::GetVertexAttribArrayObjectivATIFn, "glGetVertexAttribArrayObjectivATI")) && ok;
		ok = !!(gl.vertexAttribArrayObjectATI = GETFN_(proto::VertexAttribArrayObjectATIFn, "glVertexAttribArrayObjectATI")) && ok;
		gl.ATI_vertex_attrib_array_object = ok;
	}
	//- end: ATI_vertex_attrib_array_object

	//-//-- 
	//- Ext: ATI_vertex_streams
	static char const* ATI_vertex_streams = "ATI_vertex_streams";
	if( listed_( arg.extList, ATI_vertex_streams ) && arg.checkExt( ATI_vertex_streams, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.clientActiveVertexStreamATI = GETFN_(proto::ClientActiveVertexStreamATIFn, "glClientActiveVertexStreamATI")) && ok;
		ok = !!(gl.normalStream3bATI = GETFN_(proto::NormalStream3bATIFn, "glNormalStream3bATI")) && ok;
		ok = !!(gl.normalStream3bvATI = GETFN_(proto::NormalStream3bvATIFn, "glNormalStream3bvATI")) && ok;
		ok = !!(gl.normalStream3dATI = GETFN_(proto::NormalStream3dATIFn, "glNormalStream3dATI")) && ok;
		ok = !!(gl.normalStream3dvATI = GETFN_(proto::NormalStream3dvATIFn, "glNormalStream3dvATI")) && ok;
		ok = !!(gl.normalStream3fATI = GETFN_(proto::NormalStream3fATIFn, "glNormalStream3fATI")) && ok;
		ok = !!(gl.normalStream3fvATI = GETFN_(proto::NormalStream3fvATIFn, "glNormalStream3fvATI")) && ok;
		ok = !!(gl.normalStream3iATI = GETFN_(proto::NormalStream3iATIFn, "glNormalStream3iATI")) && ok;
		ok = !!(gl.normalStream3ivATI = GETFN_(proto::NormalStream3ivATIFn, "glNormalStream3ivATI")) && ok;
		ok = !!(gl.normalStream3sATI = GETFN_(proto::NormalStream3sATIFn, "glNormalStream3sATI")) && ok;
		ok = !!(gl.normalStream3svATI = GETFN_(proto::NormalStream3svATIFn, "glNormalStream3svATI")) && ok;
		ok = !!(gl.vertexBlendEnvfATI = GETFN_(proto::VertexBlendEnvfATIFn, "glVertexBlendEnvfATI")) && ok;
		ok = !!(gl.vertexBlendEnviATI = GETFN_(proto::VertexBlendEnviATIFn, "glVertexBlendEnviATI")) && ok;
		ok = !!(gl.vertexStream1dATI = GETFN_(proto::VertexStream1dATIFn, "glVertexStream1dATI")) && ok;
		ok = !!(gl.vertexStream1dvATI = GETFN_(proto::VertexStream1dvATIFn, "glVertexStream1dvATI")) && ok;
		ok = !!(gl.vertexStream1fATI = GETFN_(proto::VertexStream1fATIFn, "glVertexStream1fATI")) && ok;
		ok = !!(gl.vertexStream1fvATI = GETFN_(proto::VertexStream1fvATIFn, "glVertexStream1fvATI")) && ok;
		ok = !!(gl.vertexStream1iATI = GETFN_(proto::VertexStream1iATIFn, "glVertexStream1iATI")) && ok;
		ok = !!(gl.vertexStream1ivATI = GETFN_(proto::VertexStream1ivATIFn, "glVertexStream1ivATI")) && ok;
		ok = !!(gl.vertexStream1sATI = GETFN_(proto::VertexStream1sATIFn, "glVertexStream1sATI")) && ok;
		ok = !!(gl.vertexStream1svATI = GETFN_(proto::VertexStream1svATIFn, "glVertexStream1svATI")) && ok;
		ok = !!(gl.vertexStream2dATI = GETFN_(proto::VertexStream2dATIFn, "glVertexStream2dATI")) && ok;
		ok = !!(gl.vertexStream2dvATI = GETFN_(proto::VertexStream2dvATIFn, "glVertexStream2dvATI")) && ok;
		ok = !!(gl.vertexStream2fATI = GETFN_(proto::VertexStream2fATIFn, "glVertexStream2fATI")) && ok;
		ok = !!(gl.vertexStream2fvATI = GETFN_(proto::VertexStream2fvATIFn, "glVertexStream2fvATI")) && ok;
		ok = !!(gl.vertexStream2iATI = GETFN_(proto::VertexStream2iATIFn, "glVertexStream2iATI")) && ok;
		ok = !!(gl.vertexStream2ivATI = GETFN_(proto::VertexStream2ivATIFn, "glVertexStream2ivATI")) && ok;
		ok = !!(gl.vertexStream2sATI = GETFN_(proto::VertexStream2sATIFn, "glVertexStream2sATI")) && ok;
		ok = !!(gl.vertexStream2svATI = GETFN_(proto::VertexStream2svATIFn, "glVertexStream2svATI")) && ok;
		ok = !!(gl.vertexStream3dATI = GETFN_(proto::VertexStream3dATIFn, "glVertexStream3dATI")) && ok;
		ok = !!(gl.vertexStream3dvATI = GETFN_(proto::VertexStream3dvATIFn, "glVertexStream3dvATI")) && ok;
		ok = !!(gl.vertexStream3fATI = GETFN_(proto::VertexStream3fATIFn, "glVertexStream3fATI")) && ok;
		ok = !!(gl.vertexStream3fvATI = GETFN_(proto::VertexStream3fvATIFn, "glVertexStream3fvATI")) && ok;
		ok = !!(gl.vertexStream3iATI = GETFN_(proto::VertexStream3iATIFn, "glVertexStream3iATI")) && ok;
		ok = !!(gl.vertexStream3ivATI = GETFN_(proto::VertexStream3ivATIFn, "glVertexStream3ivATI")) && ok;
		ok = !!(gl.vertexStream3sATI = GETFN_(proto::VertexStream3sATIFn, "glVertexStream3sATI")) && ok;
		ok = !!(gl.vertexStream3svATI = GETFN_(proto::VertexStream3svATIFn, "glVertexStream3svATI")) && ok;
		ok = !!(gl.vertexStream4dATI = GETFN_(proto::VertexStream4dATIFn, "glVertexStream4dATI")) && ok;
		ok = !!(gl.vertexStream4dvATI = GETFN_(proto::VertexStream4dvATIFn, "glVertexStream4dvATI")) && ok;
		ok = !!(gl.vertexStream4fATI = GETFN_(proto::VertexStream4fATIFn, "glVertexStream4fATI")) && ok;
		ok = !!(gl.vertexStream4fvATI = GETFN_(proto::VertexStream4fvATIFn, "glVertexStream4fvATI")) && ok;
		ok = !!(gl.vertexStream4iATI = GETFN_(proto::VertexStream4iATIFn, "glVertexStream4iATI")) && ok;
		ok = !!(gl.vertexStream4ivATI = GETFN_(proto::VertexStream4ivATIFn, "glVertexStream4ivATI")) && ok;
		ok = !!(gl.vertexStream4sATI = GETFN_(proto::VertexStream4sATIFn, "glVertexStream4sATI")) && ok;
		ok = !!(gl.vertexStream4svATI = GETFN_(proto::VertexStream4svATIFn, "glVertexStream4svATI")) && ok;
		gl.ATI_vertex_streams = ok;
	}
	//- end: ATI_vertex_streams

}

#undef GETFN_
//--    <<< ~ dlapi::gl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE3(dlapi,gl,detail)
//--//////////////////////////////////////////////////////////////////////////
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
