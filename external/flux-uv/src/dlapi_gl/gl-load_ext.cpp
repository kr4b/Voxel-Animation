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

void load_gl_ext_( GLapi& gl, LoaderArg_ const& arg )
{
	//-//-- 
	//- Ext: EXT_bindable_uniform
	static char const* EXT_bindable_uniform = "EXT_bindable_uniform";
	if( listed_( arg.extList, EXT_bindable_uniform ) && arg.checkExt( EXT_bindable_uniform, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getUniformBufferSizeEXT = GETFN_(proto::GetUniformBufferSizeEXTFn, "glGetUniformBufferSizeEXT")) && ok;
		ok = !!(gl.getUniformOffsetEXT = GETFN_(proto::GetUniformOffsetEXTFn, "glGetUniformOffsetEXT")) && ok;
		ok = !!(gl.uniformBufferEXT = GETFN_(proto::UniformBufferEXTFn, "glUniformBufferEXT")) && ok;
		gl.EXT_bindable_uniform = ok;
	}
	//- end: EXT_bindable_uniform

	//-//-- 
	//- Ext: EXT_blend_color
	static char const* EXT_blend_color = "EXT_blend_color";
	if( listed_( arg.extList, EXT_blend_color ) && arg.checkExt( EXT_blend_color, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.blendColorEXT = GETFN_(proto::BlendColorEXTFn, "glBlendColorEXT")) && ok;
		gl.EXT_blend_color = ok;
	}
	//- end: EXT_blend_color

	//-//-- 
	//- Ext: EXT_blend_equation_separate
	static char const* EXT_blend_equation_separate = "EXT_blend_equation_separate";
	if( listed_( arg.extList, EXT_blend_equation_separate ) && arg.checkExt( EXT_blend_equation_separate, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.blendEquationSeparateEXT = GETFN_(proto::BlendEquationSeparateEXTFn, "glBlendEquationSeparateEXT")) && ok;
		gl.EXT_blend_equation_separate = ok;
	}
	//- end: EXT_blend_equation_separate

	//-//-- 
	//- Ext: EXT_blend_func_separate
	static char const* EXT_blend_func_separate = "EXT_blend_func_separate";
	if( listed_( arg.extList, EXT_blend_func_separate ) && arg.checkExt( EXT_blend_func_separate, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.blendFuncSeparateEXT = GETFN_(proto::BlendFuncSeparateEXTFn, "glBlendFuncSeparateEXT")) && ok;
		gl.EXT_blend_func_separate = ok;
	}
	//- end: EXT_blend_func_separate

	//-//-- 
	//- Ext: EXT_blend_minmax
	static char const* EXT_blend_minmax = "EXT_blend_minmax";
	if( listed_( arg.extList, EXT_blend_minmax ) && arg.checkExt( EXT_blend_minmax, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.blendEquationEXT = GETFN_(proto::BlendEquationEXTFn, "glBlendEquationEXT")) && ok;
		gl.EXT_blend_minmax = ok;
	}
	//- end: EXT_blend_minmax

	//-//-- 
	//- Ext: EXT_color_subtable
	static char const* EXT_color_subtable = "EXT_color_subtable";
	if( listed_( arg.extList, EXT_color_subtable ) && arg.checkExt( EXT_color_subtable, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.colorSubTableEXT = GETFN_(proto::ColorSubTableEXTFn, "glColorSubTableEXT")) && ok;
		ok = !!(gl.copyColorSubTableEXT = GETFN_(proto::CopyColorSubTableEXTFn, "glCopyColorSubTableEXT")) && ok;
		gl.EXT_color_subtable = ok;
	}
	//- end: EXT_color_subtable

	//-//-- 
	//- Ext: EXT_compiled_vertex_array
	static char const* EXT_compiled_vertex_array = "EXT_compiled_vertex_array";
	if( listed_( arg.extList, EXT_compiled_vertex_array ) && arg.checkExt( EXT_compiled_vertex_array, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.lockArraysEXT = GETFN_(proto::LockArraysEXTFn, "glLockArraysEXT")) && ok;
		ok = !!(gl.unlockArraysEXT = GETFN_(proto::UnlockArraysEXTFn, "glUnlockArraysEXT")) && ok;
		gl.EXT_compiled_vertex_array = ok;
	}
	//- end: EXT_compiled_vertex_array

	//-//-- 
	//- Ext: EXT_convolution
	static char const* EXT_convolution = "EXT_convolution";
	if( listed_( arg.extList, EXT_convolution ) && arg.checkExt( EXT_convolution, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.convolutionFilter1DEXT = GETFN_(proto::ConvolutionFilter1DEXTFn, "glConvolutionFilter1DEXT")) && ok;
		ok = !!(gl.convolutionFilter2DEXT = GETFN_(proto::ConvolutionFilter2DEXTFn, "glConvolutionFilter2DEXT")) && ok;
		ok = !!(gl.convolutionParameterfEXT = GETFN_(proto::ConvolutionParameterfEXTFn, "glConvolutionParameterfEXT")) && ok;
		ok = !!(gl.convolutionParameterfvEXT = GETFN_(proto::ConvolutionParameterfvEXTFn, "glConvolutionParameterfvEXT")) && ok;
		ok = !!(gl.convolutionParameteriEXT = GETFN_(proto::ConvolutionParameteriEXTFn, "glConvolutionParameteriEXT")) && ok;
		ok = !!(gl.convolutionParameterivEXT = GETFN_(proto::ConvolutionParameterivEXTFn, "glConvolutionParameterivEXT")) && ok;
		ok = !!(gl.copyConvolutionFilter1DEXT = GETFN_(proto::CopyConvolutionFilter1DEXTFn, "glCopyConvolutionFilter1DEXT")) && ok;
		ok = !!(gl.copyConvolutionFilter2DEXT = GETFN_(proto::CopyConvolutionFilter2DEXTFn, "glCopyConvolutionFilter2DEXT")) && ok;
		ok = !!(gl.getConvolutionFilterEXT = GETFN_(proto::GetConvolutionFilterEXTFn, "glGetConvolutionFilterEXT")) && ok;
		ok = !!(gl.getConvolutionParameterfvEXT = GETFN_(proto::GetConvolutionParameterfvEXTFn, "glGetConvolutionParameterfvEXT")) && ok;
		ok = !!(gl.getConvolutionParameterivEXT = GETFN_(proto::GetConvolutionParameterivEXTFn, "glGetConvolutionParameterivEXT")) && ok;
		ok = !!(gl.getSeparableFilterEXT = GETFN_(proto::GetSeparableFilterEXTFn, "glGetSeparableFilterEXT")) && ok;
		ok = !!(gl.separableFilter2DEXT = GETFN_(proto::SeparableFilter2DEXTFn, "glSeparableFilter2DEXT")) && ok;
		gl.EXT_convolution = ok;
	}
	//- end: EXT_convolution

	//-//-- 
	//- Ext: EXT_coordinate_frame
	static char const* EXT_coordinate_frame = "EXT_coordinate_frame";
	if( listed_( arg.extList, EXT_coordinate_frame ) && arg.checkExt( EXT_coordinate_frame, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.binormal3bEXT = GETFN_(proto::Binormal3bEXTFn, "glBinormal3bEXT")) && ok;
		ok = !!(gl.binormal3bvEXT = GETFN_(proto::Binormal3bvEXTFn, "glBinormal3bvEXT")) && ok;
		ok = !!(gl.binormal3dEXT = GETFN_(proto::Binormal3dEXTFn, "glBinormal3dEXT")) && ok;
		ok = !!(gl.binormal3dvEXT = GETFN_(proto::Binormal3dvEXTFn, "glBinormal3dvEXT")) && ok;
		ok = !!(gl.binormal3fEXT = GETFN_(proto::Binormal3fEXTFn, "glBinormal3fEXT")) && ok;
		ok = !!(gl.binormal3fvEXT = GETFN_(proto::Binormal3fvEXTFn, "glBinormal3fvEXT")) && ok;
		ok = !!(gl.binormal3iEXT = GETFN_(proto::Binormal3iEXTFn, "glBinormal3iEXT")) && ok;
		ok = !!(gl.binormal3ivEXT = GETFN_(proto::Binormal3ivEXTFn, "glBinormal3ivEXT")) && ok;
		ok = !!(gl.binormal3sEXT = GETFN_(proto::Binormal3sEXTFn, "glBinormal3sEXT")) && ok;
		ok = !!(gl.binormal3svEXT = GETFN_(proto::Binormal3svEXTFn, "glBinormal3svEXT")) && ok;
		ok = !!(gl.binormalPointerEXT = GETFN_(proto::BinormalPointerEXTFn, "glBinormalPointerEXT")) && ok;
		ok = !!(gl.tangent3bEXT = GETFN_(proto::Tangent3bEXTFn, "glTangent3bEXT")) && ok;
		ok = !!(gl.tangent3bvEXT = GETFN_(proto::Tangent3bvEXTFn, "glTangent3bvEXT")) && ok;
		ok = !!(gl.tangent3dEXT = GETFN_(proto::Tangent3dEXTFn, "glTangent3dEXT")) && ok;
		ok = !!(gl.tangent3dvEXT = GETFN_(proto::Tangent3dvEXTFn, "glTangent3dvEXT")) && ok;
		ok = !!(gl.tangent3fEXT = GETFN_(proto::Tangent3fEXTFn, "glTangent3fEXT")) && ok;
		ok = !!(gl.tangent3fvEXT = GETFN_(proto::Tangent3fvEXTFn, "glTangent3fvEXT")) && ok;
		ok = !!(gl.tangent3iEXT = GETFN_(proto::Tangent3iEXTFn, "glTangent3iEXT")) && ok;
		ok = !!(gl.tangent3ivEXT = GETFN_(proto::Tangent3ivEXTFn, "glTangent3ivEXT")) && ok;
		ok = !!(gl.tangent3sEXT = GETFN_(proto::Tangent3sEXTFn, "glTangent3sEXT")) && ok;
		ok = !!(gl.tangent3svEXT = GETFN_(proto::Tangent3svEXTFn, "glTangent3svEXT")) && ok;
		ok = !!(gl.tangentPointerEXT = GETFN_(proto::TangentPointerEXTFn, "glTangentPointerEXT")) && ok;
		gl.EXT_coordinate_frame = ok;
	}
	//- end: EXT_coordinate_frame

	//-//-- 
	//- Ext: EXT_copy_texture
	static char const* EXT_copy_texture = "EXT_copy_texture";
	if( listed_( arg.extList, EXT_copy_texture ) && arg.checkExt( EXT_copy_texture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.copyTexImage1DEXT = GETFN_(proto::CopyTexImage1DEXTFn, "glCopyTexImage1DEXT")) && ok;
		ok = !!(gl.copyTexImage2DEXT = GETFN_(proto::CopyTexImage2DEXTFn, "glCopyTexImage2DEXT")) && ok;
		ok = !!(gl.copyTexSubImage1DEXT = GETFN_(proto::CopyTexSubImage1DEXTFn, "glCopyTexSubImage1DEXT")) && ok;
		ok = !!(gl.copyTexSubImage2DEXT = GETFN_(proto::CopyTexSubImage2DEXTFn, "glCopyTexSubImage2DEXT")) && ok;
		ok = !!(gl.copyTexSubImage3DEXT = GETFN_(proto::CopyTexSubImage3DEXTFn, "glCopyTexSubImage3DEXT")) && ok;
		gl.EXT_copy_texture = ok;
	}
	//- end: EXT_copy_texture

	//-//-- 
	//- Ext: EXT_cull_vertex
	static char const* EXT_cull_vertex = "EXT_cull_vertex";
	if( listed_( arg.extList, EXT_cull_vertex ) && arg.checkExt( EXT_cull_vertex, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.cullParameterdvEXT = GETFN_(proto::CullParameterdvEXTFn, "glCullParameterdvEXT")) && ok;
		ok = !!(gl.cullParameterfvEXT = GETFN_(proto::CullParameterfvEXTFn, "glCullParameterfvEXT")) && ok;
		gl.EXT_cull_vertex = ok;
	}
	//- end: EXT_cull_vertex

	//-//-- 
	//- Ext: EXT_debug_label
	static char const* EXT_debug_label = "EXT_debug_label";
	if( listed_( arg.extList, EXT_debug_label ) && arg.checkExt( EXT_debug_label, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getObjectLabelEXT = GETFN_(proto::GetObjectLabelEXTFn, "glGetObjectLabelEXT")) && ok;
		ok = !!(gl.labelObjectEXT = GETFN_(proto::LabelObjectEXTFn, "glLabelObjectEXT")) && ok;
		gl.EXT_debug_label = ok;
	}
	//- end: EXT_debug_label

	//-//-- 
	//- Ext: EXT_debug_marker
	static char const* EXT_debug_marker = "EXT_debug_marker";
	if( listed_( arg.extList, EXT_debug_marker ) && arg.checkExt( EXT_debug_marker, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.insertEventMarkerEXT = GETFN_(proto::InsertEventMarkerEXTFn, "glInsertEventMarkerEXT")) && ok;
		ok = !!(gl.popGroupMarkerEXT = GETFN_(proto::PopGroupMarkerEXTFn, "glPopGroupMarkerEXT")) && ok;
		ok = !!(gl.pushGroupMarkerEXT = GETFN_(proto::PushGroupMarkerEXTFn, "glPushGroupMarkerEXT")) && ok;
		gl.EXT_debug_marker = ok;
	}
	//- end: EXT_debug_marker

	//-//-- 
	//- Ext: EXT_depth_bounds_test
	static char const* EXT_depth_bounds_test = "EXT_depth_bounds_test";
	if( listed_( arg.extList, EXT_depth_bounds_test ) && arg.checkExt( EXT_depth_bounds_test, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.depthBoundsEXT = GETFN_(proto::DepthBoundsEXTFn, "glDepthBoundsEXT")) && ok;
		gl.EXT_depth_bounds_test = ok;
	}
	//- end: EXT_depth_bounds_test

	//-//-- 
	//- Ext: EXT_direct_state_access
	static char const* EXT_direct_state_access = "EXT_direct_state_access";
	if( listed_( arg.extList, EXT_direct_state_access ) && arg.checkExt( EXT_direct_state_access, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.bindMultiTextureEXT = GETFN_(proto::BindMultiTextureEXTFn, "glBindMultiTextureEXT")) && ok;
		ok = !!(gl.checkNamedFramebufferStatusEXT = GETFN_(proto::CheckNamedFramebufferStatusEXTFn, "glCheckNamedFramebufferStatusEXT")) && ok;
		ok = !!(gl.clearNamedBufferDataEXT = GETFN_(proto::ClearNamedBufferDataEXTFn, "glClearNamedBufferDataEXT")) && ok;
		ok = !!(gl.clearNamedBufferSubDataEXT = GETFN_(proto::ClearNamedBufferSubDataEXTFn, "glClearNamedBufferSubDataEXT")) && ok;
		ok = !!(gl.clientAttribDefaultEXT = GETFN_(proto::ClientAttribDefaultEXTFn, "glClientAttribDefaultEXT")) && ok;
		ok = !!(gl.compressedMultiTexImage1DEXT = GETFN_(proto::CompressedMultiTexImage1DEXTFn, "glCompressedMultiTexImage1DEXT")) && ok;
		ok = !!(gl.compressedMultiTexImage2DEXT = GETFN_(proto::CompressedMultiTexImage2DEXTFn, "glCompressedMultiTexImage2DEXT")) && ok;
		ok = !!(gl.compressedMultiTexImage3DEXT = GETFN_(proto::CompressedMultiTexImage3DEXTFn, "glCompressedMultiTexImage3DEXT")) && ok;
		ok = !!(gl.compressedMultiTexSubImage1DEXT = GETFN_(proto::CompressedMultiTexSubImage1DEXTFn, "glCompressedMultiTexSubImage1DEXT")) && ok;
		ok = !!(gl.compressedMultiTexSubImage2DEXT = GETFN_(proto::CompressedMultiTexSubImage2DEXTFn, "glCompressedMultiTexSubImage2DEXT")) && ok;
		ok = !!(gl.compressedMultiTexSubImage3DEXT = GETFN_(proto::CompressedMultiTexSubImage3DEXTFn, "glCompressedMultiTexSubImage3DEXT")) && ok;
		ok = !!(gl.compressedTextureImage1DEXT = GETFN_(proto::CompressedTextureImage1DEXTFn, "glCompressedTextureImage1DEXT")) && ok;
		ok = !!(gl.compressedTextureImage2DEXT = GETFN_(proto::CompressedTextureImage2DEXTFn, "glCompressedTextureImage2DEXT")) && ok;
		ok = !!(gl.compressedTextureImage3DEXT = GETFN_(proto::CompressedTextureImage3DEXTFn, "glCompressedTextureImage3DEXT")) && ok;
		ok = !!(gl.compressedTextureSubImage1DEXT = GETFN_(proto::CompressedTextureSubImage1DEXTFn, "glCompressedTextureSubImage1DEXT")) && ok;
		ok = !!(gl.compressedTextureSubImage2DEXT = GETFN_(proto::CompressedTextureSubImage2DEXTFn, "glCompressedTextureSubImage2DEXT")) && ok;
		ok = !!(gl.compressedTextureSubImage3DEXT = GETFN_(proto::CompressedTextureSubImage3DEXTFn, "glCompressedTextureSubImage3DEXT")) && ok;
		ok = !!(gl.copyMultiTexImage1DEXT = GETFN_(proto::CopyMultiTexImage1DEXTFn, "glCopyMultiTexImage1DEXT")) && ok;
		ok = !!(gl.copyMultiTexImage2DEXT = GETFN_(proto::CopyMultiTexImage2DEXTFn, "glCopyMultiTexImage2DEXT")) && ok;
		ok = !!(gl.copyMultiTexSubImage1DEXT = GETFN_(proto::CopyMultiTexSubImage1DEXTFn, "glCopyMultiTexSubImage1DEXT")) && ok;
		ok = !!(gl.copyMultiTexSubImage2DEXT = GETFN_(proto::CopyMultiTexSubImage2DEXTFn, "glCopyMultiTexSubImage2DEXT")) && ok;
		ok = !!(gl.copyMultiTexSubImage3DEXT = GETFN_(proto::CopyMultiTexSubImage3DEXTFn, "glCopyMultiTexSubImage3DEXT")) && ok;
		ok = !!(gl.copyTextureImage1DEXT = GETFN_(proto::CopyTextureImage1DEXTFn, "glCopyTextureImage1DEXT")) && ok;
		ok = !!(gl.copyTextureImage2DEXT = GETFN_(proto::CopyTextureImage2DEXTFn, "glCopyTextureImage2DEXT")) && ok;
		ok = !!(gl.copyTextureSubImage1DEXT = GETFN_(proto::CopyTextureSubImage1DEXTFn, "glCopyTextureSubImage1DEXT")) && ok;
		ok = !!(gl.copyTextureSubImage2DEXT = GETFN_(proto::CopyTextureSubImage2DEXTFn, "glCopyTextureSubImage2DEXT")) && ok;
		ok = !!(gl.copyTextureSubImage3DEXT = GETFN_(proto::CopyTextureSubImage3DEXTFn, "glCopyTextureSubImage3DEXT")) && ok;
		ok = !!(gl.disableClientStateIndexedEXT = GETFN_(proto::DisableClientStateIndexedEXTFn, "glDisableClientStateIndexedEXT")) && ok;
		ok = !!(gl.disableClientStateiEXT = GETFN_(proto::DisableClientStateiEXTFn, "glDisableClientStateiEXT")) && ok;
		ok = !!(gl.disableIndexedEXT = GETFN_(proto::DisableIndexedEXTFn, "glDisableIndexedEXT")) && ok;
		ok = !!(gl.disableVertexArrayAttribEXT = GETFN_(proto::DisableVertexArrayAttribEXTFn, "glDisableVertexArrayAttribEXT")) && ok;
		ok = !!(gl.disableVertexArrayEXT = GETFN_(proto::DisableVertexArrayEXTFn, "glDisableVertexArrayEXT")) && ok;
		ok = !!(gl.enableClientStateIndexedEXT = GETFN_(proto::EnableClientStateIndexedEXTFn, "glEnableClientStateIndexedEXT")) && ok;
		ok = !!(gl.enableClientStateiEXT = GETFN_(proto::EnableClientStateiEXTFn, "glEnableClientStateiEXT")) && ok;
		ok = !!(gl.enableIndexedEXT = GETFN_(proto::EnableIndexedEXTFn, "glEnableIndexedEXT")) && ok;
		ok = !!(gl.enableVertexArrayAttribEXT = GETFN_(proto::EnableVertexArrayAttribEXTFn, "glEnableVertexArrayAttribEXT")) && ok;
		ok = !!(gl.enableVertexArrayEXT = GETFN_(proto::EnableVertexArrayEXTFn, "glEnableVertexArrayEXT")) && ok;
		ok = !!(gl.flushMappedNamedBufferRangeEXT = GETFN_(proto::FlushMappedNamedBufferRangeEXTFn, "glFlushMappedNamedBufferRangeEXT")) && ok;
		ok = !!(gl.framebufferDrawBufferEXT = GETFN_(proto::FramebufferDrawBufferEXTFn, "glFramebufferDrawBufferEXT")) && ok;
		ok = !!(gl.framebufferDrawBuffersEXT = GETFN_(proto::FramebufferDrawBuffersEXTFn, "glFramebufferDrawBuffersEXT")) && ok;
		ok = !!(gl.framebufferReadBufferEXT = GETFN_(proto::FramebufferReadBufferEXTFn, "glFramebufferReadBufferEXT")) && ok;
		ok = !!(gl.generateMultiTexMipmapEXT = GETFN_(proto::GenerateMultiTexMipmapEXTFn, "glGenerateMultiTexMipmapEXT")) && ok;
		ok = !!(gl.generateTextureMipmapEXT = GETFN_(proto::GenerateTextureMipmapEXTFn, "glGenerateTextureMipmapEXT")) && ok;
		ok = !!(gl.getBooleanIndexedvEXT = GETFN_(proto::GetBooleanIndexedvEXTFn, "glGetBooleanIndexedvEXT")) && ok;
		ok = !!(gl.getCompressedMultiTexImageEXT = GETFN_(proto::GetCompressedMultiTexImageEXTFn, "glGetCompressedMultiTexImageEXT")) && ok;
		ok = !!(gl.getCompressedTextureImageEXT = GETFN_(proto::GetCompressedTextureImageEXTFn, "glGetCompressedTextureImageEXT")) && ok;
		ok = !!(gl.getDoubleIndexedvEXT = GETFN_(proto::GetDoubleIndexedvEXTFn, "glGetDoubleIndexedvEXT")) && ok;
		ok = !!(gl.getDoublei_vEXT = GETFN_(proto::GetDoublei_vEXTFn, "glGetDoublei_vEXT")) && ok;
		ok = !!(gl.getFloatIndexedvEXT = GETFN_(proto::GetFloatIndexedvEXTFn, "glGetFloatIndexedvEXT")) && ok;
		ok = !!(gl.getFloati_vEXT = GETFN_(proto::GetFloati_vEXTFn, "glGetFloati_vEXT")) && ok;
		ok = !!(gl.getFramebufferParameterivEXT = GETFN_(proto::GetFramebufferParameterivEXTFn, "glGetFramebufferParameterivEXT")) && ok;
		ok = !!(gl.getIntegerIndexedvEXT = GETFN_(proto::GetIntegerIndexedvEXTFn, "glGetIntegerIndexedvEXT")) && ok;
		ok = !!(gl.getMultiTexEnvfvEXT = GETFN_(proto::GetMultiTexEnvfvEXTFn, "glGetMultiTexEnvfvEXT")) && ok;
		ok = !!(gl.getMultiTexEnvivEXT = GETFN_(proto::GetMultiTexEnvivEXTFn, "glGetMultiTexEnvivEXT")) && ok;
		ok = !!(gl.getMultiTexGendvEXT = GETFN_(proto::GetMultiTexGendvEXTFn, "glGetMultiTexGendvEXT")) && ok;
		ok = !!(gl.getMultiTexGenfvEXT = GETFN_(proto::GetMultiTexGenfvEXTFn, "glGetMultiTexGenfvEXT")) && ok;
		ok = !!(gl.getMultiTexGenivEXT = GETFN_(proto::GetMultiTexGenivEXTFn, "glGetMultiTexGenivEXT")) && ok;
		ok = !!(gl.getMultiTexImageEXT = GETFN_(proto::GetMultiTexImageEXTFn, "glGetMultiTexImageEXT")) && ok;
		ok = !!(gl.getMultiTexLevelParameterfvEXT = GETFN_(proto::GetMultiTexLevelParameterfvEXTFn, "glGetMultiTexLevelParameterfvEXT")) && ok;
		ok = !!(gl.getMultiTexLevelParameterivEXT = GETFN_(proto::GetMultiTexLevelParameterivEXTFn, "glGetMultiTexLevelParameterivEXT")) && ok;
		ok = !!(gl.getMultiTexParameterIivEXT = GETFN_(proto::GetMultiTexParameterIivEXTFn, "glGetMultiTexParameterIivEXT")) && ok;
		ok = !!(gl.getMultiTexParameterIuivEXT = GETFN_(proto::GetMultiTexParameterIuivEXTFn, "glGetMultiTexParameterIuivEXT")) && ok;
		ok = !!(gl.getMultiTexParameterfvEXT = GETFN_(proto::GetMultiTexParameterfvEXTFn, "glGetMultiTexParameterfvEXT")) && ok;
		ok = !!(gl.getMultiTexParameterivEXT = GETFN_(proto::GetMultiTexParameterivEXTFn, "glGetMultiTexParameterivEXT")) && ok;
		ok = !!(gl.getNamedBufferParameterivEXT = GETFN_(proto::GetNamedBufferParameterivEXTFn, "glGetNamedBufferParameterivEXT")) && ok;
		ok = !!(gl.getNamedBufferPointervEXT = GETFN_(proto::GetNamedBufferPointervEXTFn, "glGetNamedBufferPointervEXT")) && ok;
		ok = !!(gl.getNamedBufferSubDataEXT = GETFN_(proto::GetNamedBufferSubDataEXTFn, "glGetNamedBufferSubDataEXT")) && ok;
		ok = !!(gl.getNamedFramebufferAttachmentParameterivEXT = GETFN_(proto::GetNamedFramebufferAttachmentParameterivEXTFn, "glGetNamedFramebufferAttachmentParameterivEXT")) && ok;
		ok = !!(gl.getNamedFramebufferParameterivEXT = GETFN_(proto::GetNamedFramebufferParameterivEXTFn, "glGetNamedFramebufferParameterivEXT")) && ok;
		ok = !!(gl.getNamedProgramLocalParameterIivEXT = GETFN_(proto::GetNamedProgramLocalParameterIivEXTFn, "glGetNamedProgramLocalParameterIivEXT")) && ok;
		ok = !!(gl.getNamedProgramLocalParameterIuivEXT = GETFN_(proto::GetNamedProgramLocalParameterIuivEXTFn, "glGetNamedProgramLocalParameterIuivEXT")) && ok;
		ok = !!(gl.getNamedProgramLocalParameterdvEXT = GETFN_(proto::GetNamedProgramLocalParameterdvEXTFn, "glGetNamedProgramLocalParameterdvEXT")) && ok;
		ok = !!(gl.getNamedProgramLocalParameterfvEXT = GETFN_(proto::GetNamedProgramLocalParameterfvEXTFn, "glGetNamedProgramLocalParameterfvEXT")) && ok;
		ok = !!(gl.getNamedProgramStringEXT = GETFN_(proto::GetNamedProgramStringEXTFn, "glGetNamedProgramStringEXT")) && ok;
		ok = !!(gl.getNamedProgramivEXT = GETFN_(proto::GetNamedProgramivEXTFn, "glGetNamedProgramivEXT")) && ok;
		ok = !!(gl.getNamedRenderbufferParameterivEXT = GETFN_(proto::GetNamedRenderbufferParameterivEXTFn, "glGetNamedRenderbufferParameterivEXT")) && ok;
		ok = !!(gl.getPointerIndexedvEXT = GETFN_(proto::GetPointerIndexedvEXTFn, "glGetPointerIndexedvEXT")) && ok;
		ok = !!(gl.getPointeri_vEXT = GETFN_(proto::GetPointeri_vEXTFn, "glGetPointeri_vEXT")) && ok;
		ok = !!(gl.getTextureImageEXT = GETFN_(proto::GetTextureImageEXTFn, "glGetTextureImageEXT")) && ok;
		ok = !!(gl.getTextureLevelParameterfvEXT = GETFN_(proto::GetTextureLevelParameterfvEXTFn, "glGetTextureLevelParameterfvEXT")) && ok;
		ok = !!(gl.getTextureLevelParameterivEXT = GETFN_(proto::GetTextureLevelParameterivEXTFn, "glGetTextureLevelParameterivEXT")) && ok;
		ok = !!(gl.getTextureParameterIivEXT = GETFN_(proto::GetTextureParameterIivEXTFn, "glGetTextureParameterIivEXT")) && ok;
		ok = !!(gl.getTextureParameterIuivEXT = GETFN_(proto::GetTextureParameterIuivEXTFn, "glGetTextureParameterIuivEXT")) && ok;
		ok = !!(gl.getTextureParameterfvEXT = GETFN_(proto::GetTextureParameterfvEXTFn, "glGetTextureParameterfvEXT")) && ok;
		ok = !!(gl.getTextureParameterivEXT = GETFN_(proto::GetTextureParameterivEXTFn, "glGetTextureParameterivEXT")) && ok;
		ok = !!(gl.getVertexArrayIntegeri_vEXT = GETFN_(proto::GetVertexArrayIntegeri_vEXTFn, "glGetVertexArrayIntegeri_vEXT")) && ok;
		ok = !!(gl.getVertexArrayIntegervEXT = GETFN_(proto::GetVertexArrayIntegervEXTFn, "glGetVertexArrayIntegervEXT")) && ok;
		ok = !!(gl.getVertexArrayPointeri_vEXT = GETFN_(proto::GetVertexArrayPointeri_vEXTFn, "glGetVertexArrayPointeri_vEXT")) && ok;
		ok = !!(gl.getVertexArrayPointervEXT = GETFN_(proto::GetVertexArrayPointervEXTFn, "glGetVertexArrayPointervEXT")) && ok;
		ok = !!(gl.isEnabledIndexedEXT = GETFN_(proto::IsEnabledIndexedEXTFn, "glIsEnabledIndexedEXT")) && ok;
		ok = !!(gl.mapNamedBufferEXT = GETFN_(proto::MapNamedBufferEXTFn, "glMapNamedBufferEXT")) && ok;
		ok = !!(gl.mapNamedBufferRangeEXT = GETFN_(proto::MapNamedBufferRangeEXTFn, "glMapNamedBufferRangeEXT")) && ok;
		ok = !!(gl.matrixFrustumEXT = GETFN_(proto::MatrixFrustumEXTFn, "glMatrixFrustumEXT")) && ok;
		ok = !!(gl.matrixLoadIdentityEXT = GETFN_(proto::MatrixLoadIdentityEXTFn, "glMatrixLoadIdentityEXT")) && ok;
		ok = !!(gl.matrixLoadTransposedEXT = GETFN_(proto::MatrixLoadTransposedEXTFn, "glMatrixLoadTransposedEXT")) && ok;
		ok = !!(gl.matrixLoadTransposefEXT = GETFN_(proto::MatrixLoadTransposefEXTFn, "glMatrixLoadTransposefEXT")) && ok;
		ok = !!(gl.matrixLoaddEXT = GETFN_(proto::MatrixLoaddEXTFn, "glMatrixLoaddEXT")) && ok;
		ok = !!(gl.matrixLoadfEXT = GETFN_(proto::MatrixLoadfEXTFn, "glMatrixLoadfEXT")) && ok;
		ok = !!(gl.matrixMultTransposedEXT = GETFN_(proto::MatrixMultTransposedEXTFn, "glMatrixMultTransposedEXT")) && ok;
		ok = !!(gl.matrixMultTransposefEXT = GETFN_(proto::MatrixMultTransposefEXTFn, "glMatrixMultTransposefEXT")) && ok;
		ok = !!(gl.matrixMultdEXT = GETFN_(proto::MatrixMultdEXTFn, "glMatrixMultdEXT")) && ok;
		ok = !!(gl.matrixMultfEXT = GETFN_(proto::MatrixMultfEXTFn, "glMatrixMultfEXT")) && ok;
		ok = !!(gl.matrixOrthoEXT = GETFN_(proto::MatrixOrthoEXTFn, "glMatrixOrthoEXT")) && ok;
		ok = !!(gl.matrixPopEXT = GETFN_(proto::MatrixPopEXTFn, "glMatrixPopEXT")) && ok;
		ok = !!(gl.matrixPushEXT = GETFN_(proto::MatrixPushEXTFn, "glMatrixPushEXT")) && ok;
		ok = !!(gl.matrixRotatedEXT = GETFN_(proto::MatrixRotatedEXTFn, "glMatrixRotatedEXT")) && ok;
		ok = !!(gl.matrixRotatefEXT = GETFN_(proto::MatrixRotatefEXTFn, "glMatrixRotatefEXT")) && ok;
		ok = !!(gl.matrixScaledEXT = GETFN_(proto::MatrixScaledEXTFn, "glMatrixScaledEXT")) && ok;
		ok = !!(gl.matrixScalefEXT = GETFN_(proto::MatrixScalefEXTFn, "glMatrixScalefEXT")) && ok;
		ok = !!(gl.matrixTranslatedEXT = GETFN_(proto::MatrixTranslatedEXTFn, "glMatrixTranslatedEXT")) && ok;
		ok = !!(gl.matrixTranslatefEXT = GETFN_(proto::MatrixTranslatefEXTFn, "glMatrixTranslatefEXT")) && ok;
		ok = !!(gl.multiTexBufferEXT = GETFN_(proto::MultiTexBufferEXTFn, "glMultiTexBufferEXT")) && ok;
		ok = !!(gl.multiTexCoordPointerEXT = GETFN_(proto::MultiTexCoordPointerEXTFn, "glMultiTexCoordPointerEXT")) && ok;
		ok = !!(gl.multiTexEnvfEXT = GETFN_(proto::MultiTexEnvfEXTFn, "glMultiTexEnvfEXT")) && ok;
		ok = !!(gl.multiTexEnvfvEXT = GETFN_(proto::MultiTexEnvfvEXTFn, "glMultiTexEnvfvEXT")) && ok;
		ok = !!(gl.multiTexEnviEXT = GETFN_(proto::MultiTexEnviEXTFn, "glMultiTexEnviEXT")) && ok;
		ok = !!(gl.multiTexEnvivEXT = GETFN_(proto::MultiTexEnvivEXTFn, "glMultiTexEnvivEXT")) && ok;
		ok = !!(gl.multiTexGendEXT = GETFN_(proto::MultiTexGendEXTFn, "glMultiTexGendEXT")) && ok;
		ok = !!(gl.multiTexGendvEXT = GETFN_(proto::MultiTexGendvEXTFn, "glMultiTexGendvEXT")) && ok;
		ok = !!(gl.multiTexGenfEXT = GETFN_(proto::MultiTexGenfEXTFn, "glMultiTexGenfEXT")) && ok;
		ok = !!(gl.multiTexGenfvEXT = GETFN_(proto::MultiTexGenfvEXTFn, "glMultiTexGenfvEXT")) && ok;
		ok = !!(gl.multiTexGeniEXT = GETFN_(proto::MultiTexGeniEXTFn, "glMultiTexGeniEXT")) && ok;
		ok = !!(gl.multiTexGenivEXT = GETFN_(proto::MultiTexGenivEXTFn, "glMultiTexGenivEXT")) && ok;
		ok = !!(gl.multiTexImage1DEXT = GETFN_(proto::MultiTexImage1DEXTFn, "glMultiTexImage1DEXT")) && ok;
		ok = !!(gl.multiTexImage2DEXT = GETFN_(proto::MultiTexImage2DEXTFn, "glMultiTexImage2DEXT")) && ok;
		ok = !!(gl.multiTexImage3DEXT = GETFN_(proto::MultiTexImage3DEXTFn, "glMultiTexImage3DEXT")) && ok;
		ok = !!(gl.multiTexParameterIivEXT = GETFN_(proto::MultiTexParameterIivEXTFn, "glMultiTexParameterIivEXT")) && ok;
		ok = !!(gl.multiTexParameterIuivEXT = GETFN_(proto::MultiTexParameterIuivEXTFn, "glMultiTexParameterIuivEXT")) && ok;
		ok = !!(gl.multiTexParameterfEXT = GETFN_(proto::MultiTexParameterfEXTFn, "glMultiTexParameterfEXT")) && ok;
		ok = !!(gl.multiTexParameterfvEXT = GETFN_(proto::MultiTexParameterfvEXTFn, "glMultiTexParameterfvEXT")) && ok;
		ok = !!(gl.multiTexParameteriEXT = GETFN_(proto::MultiTexParameteriEXTFn, "glMultiTexParameteriEXT")) && ok;
		ok = !!(gl.multiTexParameterivEXT = GETFN_(proto::MultiTexParameterivEXTFn, "glMultiTexParameterivEXT")) && ok;
		ok = !!(gl.multiTexRenderbufferEXT = GETFN_(proto::MultiTexRenderbufferEXTFn, "glMultiTexRenderbufferEXT")) && ok;
		ok = !!(gl.multiTexSubImage1DEXT = GETFN_(proto::MultiTexSubImage1DEXTFn, "glMultiTexSubImage1DEXT")) && ok;
		ok = !!(gl.multiTexSubImage2DEXT = GETFN_(proto::MultiTexSubImage2DEXTFn, "glMultiTexSubImage2DEXT")) && ok;
		ok = !!(gl.multiTexSubImage3DEXT = GETFN_(proto::MultiTexSubImage3DEXTFn, "glMultiTexSubImage3DEXT")) && ok;
		ok = !!(gl.namedBufferDataEXT = GETFN_(proto::NamedBufferDataEXTFn, "glNamedBufferDataEXT")) && ok;
		ok = !!(gl.namedBufferStorageEXT = GETFN_(proto::NamedBufferStorageEXTFn, "glNamedBufferStorageEXT")) && ok;
		ok = !!(gl.namedBufferSubDataEXT = GETFN_(proto::NamedBufferSubDataEXTFn, "glNamedBufferSubDataEXT")) && ok;
		ok = !!(gl.namedCopyBufferSubDataEXT = GETFN_(proto::NamedCopyBufferSubDataEXTFn, "glNamedCopyBufferSubDataEXT")) && ok;
		ok = !!(gl.namedFramebufferParameteriEXT = GETFN_(proto::NamedFramebufferParameteriEXTFn, "glNamedFramebufferParameteriEXT")) && ok;
		ok = !!(gl.namedFramebufferRenderbufferEXT = GETFN_(proto::NamedFramebufferRenderbufferEXTFn, "glNamedFramebufferRenderbufferEXT")) && ok;
		ok = !!(gl.namedFramebufferTexture1DEXT = GETFN_(proto::NamedFramebufferTexture1DEXTFn, "glNamedFramebufferTexture1DEXT")) && ok;
		ok = !!(gl.namedFramebufferTexture2DEXT = GETFN_(proto::NamedFramebufferTexture2DEXTFn, "glNamedFramebufferTexture2DEXT")) && ok;
		ok = !!(gl.namedFramebufferTexture3DEXT = GETFN_(proto::NamedFramebufferTexture3DEXTFn, "glNamedFramebufferTexture3DEXT")) && ok;
		ok = !!(gl.namedFramebufferTextureEXT = GETFN_(proto::NamedFramebufferTextureEXTFn, "glNamedFramebufferTextureEXT")) && ok;
		ok = !!(gl.namedFramebufferTextureFaceEXT = GETFN_(proto::NamedFramebufferTextureFaceEXTFn, "glNamedFramebufferTextureFaceEXT")) && ok;
		ok = !!(gl.namedFramebufferTextureLayerEXT = GETFN_(proto::NamedFramebufferTextureLayerEXTFn, "glNamedFramebufferTextureLayerEXT")) && ok;
		ok = !!(gl.namedProgramLocalParameter4dEXT = GETFN_(proto::NamedProgramLocalParameter4dEXTFn, "glNamedProgramLocalParameter4dEXT")) && ok;
		ok = !!(gl.namedProgramLocalParameter4dvEXT = GETFN_(proto::NamedProgramLocalParameter4dvEXTFn, "glNamedProgramLocalParameter4dvEXT")) && ok;
		ok = !!(gl.namedProgramLocalParameter4fEXT = GETFN_(proto::NamedProgramLocalParameter4fEXTFn, "glNamedProgramLocalParameter4fEXT")) && ok;
		ok = !!(gl.namedProgramLocalParameter4fvEXT = GETFN_(proto::NamedProgramLocalParameter4fvEXTFn, "glNamedProgramLocalParameter4fvEXT")) && ok;
		ok = !!(gl.namedProgramLocalParameterI4iEXT = GETFN_(proto::NamedProgramLocalParameterI4iEXTFn, "glNamedProgramLocalParameterI4iEXT")) && ok;
		ok = !!(gl.namedProgramLocalParameterI4ivEXT = GETFN_(proto::NamedProgramLocalParameterI4ivEXTFn, "glNamedProgramLocalParameterI4ivEXT")) && ok;
		ok = !!(gl.namedProgramLocalParameterI4uiEXT = GETFN_(proto::NamedProgramLocalParameterI4uiEXTFn, "glNamedProgramLocalParameterI4uiEXT")) && ok;
		ok = !!(gl.namedProgramLocalParameterI4uivEXT = GETFN_(proto::NamedProgramLocalParameterI4uivEXTFn, "glNamedProgramLocalParameterI4uivEXT")) && ok;
		ok = !!(gl.namedProgramLocalParameters4fvEXT = GETFN_(proto::NamedProgramLocalParameters4fvEXTFn, "glNamedProgramLocalParameters4fvEXT")) && ok;
		ok = !!(gl.namedProgramLocalParametersI4ivEXT = GETFN_(proto::NamedProgramLocalParametersI4ivEXTFn, "glNamedProgramLocalParametersI4ivEXT")) && ok;
		ok = !!(gl.namedProgramLocalParametersI4uivEXT = GETFN_(proto::NamedProgramLocalParametersI4uivEXTFn, "glNamedProgramLocalParametersI4uivEXT")) && ok;
		ok = !!(gl.namedProgramStringEXT = GETFN_(proto::NamedProgramStringEXTFn, "glNamedProgramStringEXT")) && ok;
		ok = !!(gl.namedRenderbufferStorageEXT = GETFN_(proto::NamedRenderbufferStorageEXTFn, "glNamedRenderbufferStorageEXT")) && ok;
		ok = !!(gl.namedRenderbufferStorageMultisampleCoverageEXT = GETFN_(proto::NamedRenderbufferStorageMultisampleCoverageEXTFn, "glNamedRenderbufferStorageMultisampleCoverageEXT")) && ok;
		ok = !!(gl.namedRenderbufferStorageMultisampleEXT = GETFN_(proto::NamedRenderbufferStorageMultisampleEXTFn, "glNamedRenderbufferStorageMultisampleEXT")) && ok;
		ok = !!(gl.programUniform1dEXT = GETFN_(proto::ProgramUniform1dEXTFn, "glProgramUniform1dEXT")) && ok;
		ok = !!(gl.programUniform1dvEXT = GETFN_(proto::ProgramUniform1dvEXTFn, "glProgramUniform1dvEXT")) && ok;
		ok = !!(gl.programUniform1fEXT = GETFN_(proto::ProgramUniform1fEXTFn, "glProgramUniform1fEXT")) && ok;
		ok = !!(gl.programUniform1fvEXT = GETFN_(proto::ProgramUniform1fvEXTFn, "glProgramUniform1fvEXT")) && ok;
		ok = !!(gl.programUniform1iEXT = GETFN_(proto::ProgramUniform1iEXTFn, "glProgramUniform1iEXT")) && ok;
		ok = !!(gl.programUniform1ivEXT = GETFN_(proto::ProgramUniform1ivEXTFn, "glProgramUniform1ivEXT")) && ok;
		ok = !!(gl.programUniform1uiEXT = GETFN_(proto::ProgramUniform1uiEXTFn, "glProgramUniform1uiEXT")) && ok;
		ok = !!(gl.programUniform1uivEXT = GETFN_(proto::ProgramUniform1uivEXTFn, "glProgramUniform1uivEXT")) && ok;
		ok = !!(gl.programUniform2dEXT = GETFN_(proto::ProgramUniform2dEXTFn, "glProgramUniform2dEXT")) && ok;
		ok = !!(gl.programUniform2dvEXT = GETFN_(proto::ProgramUniform2dvEXTFn, "glProgramUniform2dvEXT")) && ok;
		ok = !!(gl.programUniform2fEXT = GETFN_(proto::ProgramUniform2fEXTFn, "glProgramUniform2fEXT")) && ok;
		ok = !!(gl.programUniform2fvEXT = GETFN_(proto::ProgramUniform2fvEXTFn, "glProgramUniform2fvEXT")) && ok;
		ok = !!(gl.programUniform2iEXT = GETFN_(proto::ProgramUniform2iEXTFn, "glProgramUniform2iEXT")) && ok;
		ok = !!(gl.programUniform2ivEXT = GETFN_(proto::ProgramUniform2ivEXTFn, "glProgramUniform2ivEXT")) && ok;
		ok = !!(gl.programUniform2uiEXT = GETFN_(proto::ProgramUniform2uiEXTFn, "glProgramUniform2uiEXT")) && ok;
		ok = !!(gl.programUniform2uivEXT = GETFN_(proto::ProgramUniform2uivEXTFn, "glProgramUniform2uivEXT")) && ok;
		ok = !!(gl.programUniform3dEXT = GETFN_(proto::ProgramUniform3dEXTFn, "glProgramUniform3dEXT")) && ok;
		ok = !!(gl.programUniform3dvEXT = GETFN_(proto::ProgramUniform3dvEXTFn, "glProgramUniform3dvEXT")) && ok;
		ok = !!(gl.programUniform3fEXT = GETFN_(proto::ProgramUniform3fEXTFn, "glProgramUniform3fEXT")) && ok;
		ok = !!(gl.programUniform3fvEXT = GETFN_(proto::ProgramUniform3fvEXTFn, "glProgramUniform3fvEXT")) && ok;
		ok = !!(gl.programUniform3iEXT = GETFN_(proto::ProgramUniform3iEXTFn, "glProgramUniform3iEXT")) && ok;
		ok = !!(gl.programUniform3ivEXT = GETFN_(proto::ProgramUniform3ivEXTFn, "glProgramUniform3ivEXT")) && ok;
		ok = !!(gl.programUniform3uiEXT = GETFN_(proto::ProgramUniform3uiEXTFn, "glProgramUniform3uiEXT")) && ok;
		ok = !!(gl.programUniform3uivEXT = GETFN_(proto::ProgramUniform3uivEXTFn, "glProgramUniform3uivEXT")) && ok;
		ok = !!(gl.programUniform4dEXT = GETFN_(proto::ProgramUniform4dEXTFn, "glProgramUniform4dEXT")) && ok;
		ok = !!(gl.programUniform4dvEXT = GETFN_(proto::ProgramUniform4dvEXTFn, "glProgramUniform4dvEXT")) && ok;
		ok = !!(gl.programUniform4fEXT = GETFN_(proto::ProgramUniform4fEXTFn, "glProgramUniform4fEXT")) && ok;
		ok = !!(gl.programUniform4fvEXT = GETFN_(proto::ProgramUniform4fvEXTFn, "glProgramUniform4fvEXT")) && ok;
		ok = !!(gl.programUniform4iEXT = GETFN_(proto::ProgramUniform4iEXTFn, "glProgramUniform4iEXT")) && ok;
		ok = !!(gl.programUniform4ivEXT = GETFN_(proto::ProgramUniform4ivEXTFn, "glProgramUniform4ivEXT")) && ok;
		ok = !!(gl.programUniform4uiEXT = GETFN_(proto::ProgramUniform4uiEXTFn, "glProgramUniform4uiEXT")) && ok;
		ok = !!(gl.programUniform4uivEXT = GETFN_(proto::ProgramUniform4uivEXTFn, "glProgramUniform4uivEXT")) && ok;
		ok = !!(gl.programUniformMatrix2dvEXT = GETFN_(proto::ProgramUniformMatrix2dvEXTFn, "glProgramUniformMatrix2dvEXT")) && ok;
		ok = !!(gl.programUniformMatrix2fvEXT = GETFN_(proto::ProgramUniformMatrix2fvEXTFn, "glProgramUniformMatrix2fvEXT")) && ok;
		ok = !!(gl.programUniformMatrix2x3dvEXT = GETFN_(proto::ProgramUniformMatrix2x3dvEXTFn, "glProgramUniformMatrix2x3dvEXT")) && ok;
		ok = !!(gl.programUniformMatrix2x3fvEXT = GETFN_(proto::ProgramUniformMatrix2x3fvEXTFn, "glProgramUniformMatrix2x3fvEXT")) && ok;
		ok = !!(gl.programUniformMatrix2x4dvEXT = GETFN_(proto::ProgramUniformMatrix2x4dvEXTFn, "glProgramUniformMatrix2x4dvEXT")) && ok;
		ok = !!(gl.programUniformMatrix2x4fvEXT = GETFN_(proto::ProgramUniformMatrix2x4fvEXTFn, "glProgramUniformMatrix2x4fvEXT")) && ok;
		ok = !!(gl.programUniformMatrix3dvEXT = GETFN_(proto::ProgramUniformMatrix3dvEXTFn, "glProgramUniformMatrix3dvEXT")) && ok;
		ok = !!(gl.programUniformMatrix3fvEXT = GETFN_(proto::ProgramUniformMatrix3fvEXTFn, "glProgramUniformMatrix3fvEXT")) && ok;
		ok = !!(gl.programUniformMatrix3x2dvEXT = GETFN_(proto::ProgramUniformMatrix3x2dvEXTFn, "glProgramUniformMatrix3x2dvEXT")) && ok;
		ok = !!(gl.programUniformMatrix3x2fvEXT = GETFN_(proto::ProgramUniformMatrix3x2fvEXTFn, "glProgramUniformMatrix3x2fvEXT")) && ok;
		ok = !!(gl.programUniformMatrix3x4dvEXT = GETFN_(proto::ProgramUniformMatrix3x4dvEXTFn, "glProgramUniformMatrix3x4dvEXT")) && ok;
		ok = !!(gl.programUniformMatrix3x4fvEXT = GETFN_(proto::ProgramUniformMatrix3x4fvEXTFn, "glProgramUniformMatrix3x4fvEXT")) && ok;
		ok = !!(gl.programUniformMatrix4dvEXT = GETFN_(proto::ProgramUniformMatrix4dvEXTFn, "glProgramUniformMatrix4dvEXT")) && ok;
		ok = !!(gl.programUniformMatrix4fvEXT = GETFN_(proto::ProgramUniformMatrix4fvEXTFn, "glProgramUniformMatrix4fvEXT")) && ok;
		ok = !!(gl.programUniformMatrix4x2dvEXT = GETFN_(proto::ProgramUniformMatrix4x2dvEXTFn, "glProgramUniformMatrix4x2dvEXT")) && ok;
		ok = !!(gl.programUniformMatrix4x2fvEXT = GETFN_(proto::ProgramUniformMatrix4x2fvEXTFn, "glProgramUniformMatrix4x2fvEXT")) && ok;
		ok = !!(gl.programUniformMatrix4x3dvEXT = GETFN_(proto::ProgramUniformMatrix4x3dvEXTFn, "glProgramUniformMatrix4x3dvEXT")) && ok;
		ok = !!(gl.programUniformMatrix4x3fvEXT = GETFN_(proto::ProgramUniformMatrix4x3fvEXTFn, "glProgramUniformMatrix4x3fvEXT")) && ok;
		ok = !!(gl.pushClientAttribDefaultEXT = GETFN_(proto::PushClientAttribDefaultEXTFn, "glPushClientAttribDefaultEXT")) && ok;
		ok = !!(gl.textureBufferEXT = GETFN_(proto::TextureBufferEXTFn, "glTextureBufferEXT")) && ok;
		ok = !!(gl.textureBufferRangeEXT = GETFN_(proto::TextureBufferRangeEXTFn, "glTextureBufferRangeEXT")) && ok;
		ok = !!(gl.textureImage1DEXT = GETFN_(proto::TextureImage1DEXTFn, "glTextureImage1DEXT")) && ok;
		ok = !!(gl.textureImage2DEXT = GETFN_(proto::TextureImage2DEXTFn, "glTextureImage2DEXT")) && ok;
		ok = !!(gl.textureImage3DEXT = GETFN_(proto::TextureImage3DEXTFn, "glTextureImage3DEXT")) && ok;
		ok = !!(gl.texturePageCommitmentEXT = GETFN_(proto::TexturePageCommitmentEXTFn, "glTexturePageCommitmentEXT")) && ok;
		ok = !!(gl.textureParameterIivEXT = GETFN_(proto::TextureParameterIivEXTFn, "glTextureParameterIivEXT")) && ok;
		ok = !!(gl.textureParameterIuivEXT = GETFN_(proto::TextureParameterIuivEXTFn, "glTextureParameterIuivEXT")) && ok;
		ok = !!(gl.textureParameterfEXT = GETFN_(proto::TextureParameterfEXTFn, "glTextureParameterfEXT")) && ok;
		ok = !!(gl.textureParameterfvEXT = GETFN_(proto::TextureParameterfvEXTFn, "glTextureParameterfvEXT")) && ok;
		ok = !!(gl.textureParameteriEXT = GETFN_(proto::TextureParameteriEXTFn, "glTextureParameteriEXT")) && ok;
		ok = !!(gl.textureParameterivEXT = GETFN_(proto::TextureParameterivEXTFn, "glTextureParameterivEXT")) && ok;
		ok = !!(gl.textureRenderbufferEXT = GETFN_(proto::TextureRenderbufferEXTFn, "glTextureRenderbufferEXT")) && ok;
		ok = !!(gl.textureStorage1DEXT = GETFN_(proto::TextureStorage1DEXTFn, "glTextureStorage1DEXT")) && ok;
		ok = !!(gl.textureStorage2DEXT = GETFN_(proto::TextureStorage2DEXTFn, "glTextureStorage2DEXT")) && ok;
		ok = !!(gl.textureStorage2DMultisampleEXT = GETFN_(proto::TextureStorage2DMultisampleEXTFn, "glTextureStorage2DMultisampleEXT")) && ok;
		ok = !!(gl.textureStorage3DEXT = GETFN_(proto::TextureStorage3DEXTFn, "glTextureStorage3DEXT")) && ok;
		ok = !!(gl.textureStorage3DMultisampleEXT = GETFN_(proto::TextureStorage3DMultisampleEXTFn, "glTextureStorage3DMultisampleEXT")) && ok;
		ok = !!(gl.textureSubImage1DEXT = GETFN_(proto::TextureSubImage1DEXTFn, "glTextureSubImage1DEXT")) && ok;
		ok = !!(gl.textureSubImage2DEXT = GETFN_(proto::TextureSubImage2DEXTFn, "glTextureSubImage2DEXT")) && ok;
		ok = !!(gl.textureSubImage3DEXT = GETFN_(proto::TextureSubImage3DEXTFn, "glTextureSubImage3DEXT")) && ok;
		ok = !!(gl.unmapNamedBufferEXT = GETFN_(proto::UnmapNamedBufferEXTFn, "glUnmapNamedBufferEXT")) && ok;
		ok = !!(gl.vertexArrayBindVertexBufferEXT = GETFN_(proto::VertexArrayBindVertexBufferEXTFn, "glVertexArrayBindVertexBufferEXT")) && ok;
		ok = !!(gl.vertexArrayColorOffsetEXT = GETFN_(proto::VertexArrayColorOffsetEXTFn, "glVertexArrayColorOffsetEXT")) && ok;
		ok = !!(gl.vertexArrayEdgeFlagOffsetEXT = GETFN_(proto::VertexArrayEdgeFlagOffsetEXTFn, "glVertexArrayEdgeFlagOffsetEXT")) && ok;
		ok = !!(gl.vertexArrayFogCoordOffsetEXT = GETFN_(proto::VertexArrayFogCoordOffsetEXTFn, "glVertexArrayFogCoordOffsetEXT")) && ok;
		ok = !!(gl.vertexArrayIndexOffsetEXT = GETFN_(proto::VertexArrayIndexOffsetEXTFn, "glVertexArrayIndexOffsetEXT")) && ok;
		ok = !!(gl.vertexArrayMultiTexCoordOffsetEXT = GETFN_(proto::VertexArrayMultiTexCoordOffsetEXTFn, "glVertexArrayMultiTexCoordOffsetEXT")) && ok;
		ok = !!(gl.vertexArrayNormalOffsetEXT = GETFN_(proto::VertexArrayNormalOffsetEXTFn, "glVertexArrayNormalOffsetEXT")) && ok;
		ok = !!(gl.vertexArraySecondaryColorOffsetEXT = GETFN_(proto::VertexArraySecondaryColorOffsetEXTFn, "glVertexArraySecondaryColorOffsetEXT")) && ok;
		ok = !!(gl.vertexArrayTexCoordOffsetEXT = GETFN_(proto::VertexArrayTexCoordOffsetEXTFn, "glVertexArrayTexCoordOffsetEXT")) && ok;
		ok = !!(gl.vertexArrayVertexAttribBindingEXT = GETFN_(proto::VertexArrayVertexAttribBindingEXTFn, "glVertexArrayVertexAttribBindingEXT")) && ok;
		ok = !!(gl.vertexArrayVertexAttribDivisorEXT = GETFN_(proto::VertexArrayVertexAttribDivisorEXTFn, "glVertexArrayVertexAttribDivisorEXT")) && ok;
		ok = !!(gl.vertexArrayVertexAttribFormatEXT = GETFN_(proto::VertexArrayVertexAttribFormatEXTFn, "glVertexArrayVertexAttribFormatEXT")) && ok;
		ok = !!(gl.vertexArrayVertexAttribIFormatEXT = GETFN_(proto::VertexArrayVertexAttribIFormatEXTFn, "glVertexArrayVertexAttribIFormatEXT")) && ok;
		ok = !!(gl.vertexArrayVertexAttribIOffsetEXT = GETFN_(proto::VertexArrayVertexAttribIOffsetEXTFn, "glVertexArrayVertexAttribIOffsetEXT")) && ok;
		ok = !!(gl.vertexArrayVertexAttribLFormatEXT = GETFN_(proto::VertexArrayVertexAttribLFormatEXTFn, "glVertexArrayVertexAttribLFormatEXT")) && ok;
		ok = !!(gl.vertexArrayVertexAttribLOffsetEXT = GETFN_(proto::VertexArrayVertexAttribLOffsetEXTFn, "glVertexArrayVertexAttribLOffsetEXT")) && ok;
		ok = !!(gl.vertexArrayVertexAttribOffsetEXT = GETFN_(proto::VertexArrayVertexAttribOffsetEXTFn, "glVertexArrayVertexAttribOffsetEXT")) && ok;
		ok = !!(gl.vertexArrayVertexBindingDivisorEXT = GETFN_(proto::VertexArrayVertexBindingDivisorEXTFn, "glVertexArrayVertexBindingDivisorEXT")) && ok;
		ok = !!(gl.vertexArrayVertexOffsetEXT = GETFN_(proto::VertexArrayVertexOffsetEXTFn, "glVertexArrayVertexOffsetEXT")) && ok;
		gl.EXT_direct_state_access = ok;
	}
	//- end: EXT_direct_state_access

	//-//-- 
	//- Ext: EXT_draw_buffers2
	static char const* EXT_draw_buffers2 = "EXT_draw_buffers2";
	if( listed_( arg.extList, EXT_draw_buffers2 ) && arg.checkExt( EXT_draw_buffers2, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.colorMaskIndexedEXT = GETFN_(proto::ColorMaskIndexedEXTFn, "glColorMaskIndexedEXT")) && ok;
		if( !gl.EXT_direct_state_access ) {
			ok = !!(gl.disableIndexedEXT = GETFN_(proto::DisableIndexedEXTFn, "glDisableIndexedEXT")) && ok;
		}
		if( !gl.EXT_direct_state_access ) {
			ok = !!(gl.enableIndexedEXT = GETFN_(proto::EnableIndexedEXTFn, "glEnableIndexedEXT")) && ok;
		}
		if( !gl.EXT_direct_state_access ) {
			ok = !!(gl.getBooleanIndexedvEXT = GETFN_(proto::GetBooleanIndexedvEXTFn, "glGetBooleanIndexedvEXT")) && ok;
		}
		if( !gl.EXT_direct_state_access ) {
			ok = !!(gl.getIntegerIndexedvEXT = GETFN_(proto::GetIntegerIndexedvEXTFn, "glGetIntegerIndexedvEXT")) && ok;
		}
		if( !gl.EXT_direct_state_access ) {
			ok = !!(gl.isEnabledIndexedEXT = GETFN_(proto::IsEnabledIndexedEXTFn, "glIsEnabledIndexedEXT")) && ok;
		}
		gl.EXT_draw_buffers2 = ok;
	}
	//- end: EXT_draw_buffers2

	//-//-- 
	//- Ext: EXT_draw_instanced
	static char const* EXT_draw_instanced = "EXT_draw_instanced";
	if( listed_( arg.extList, EXT_draw_instanced ) && arg.checkExt( EXT_draw_instanced, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.drawArraysInstancedEXT = GETFN_(proto::DrawArraysInstancedEXTFn, "glDrawArraysInstancedEXT")) && ok;
		ok = !!(gl.drawElementsInstancedEXT = GETFN_(proto::DrawElementsInstancedEXTFn, "glDrawElementsInstancedEXT")) && ok;
		gl.EXT_draw_instanced = ok;
	}
	//- end: EXT_draw_instanced

	//-//-- 
	//- Ext: EXT_draw_range_elements
	static char const* EXT_draw_range_elements = "EXT_draw_range_elements";
	if( listed_( arg.extList, EXT_draw_range_elements ) && arg.checkExt( EXT_draw_range_elements, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.drawRangeElementsEXT = GETFN_(proto::DrawRangeElementsEXTFn, "glDrawRangeElementsEXT")) && ok;
		gl.EXT_draw_range_elements = ok;
	}
	//- end: EXT_draw_range_elements

	//-//-- 
	//- Ext: EXT_fog_coord
	static char const* EXT_fog_coord = "EXT_fog_coord";
	if( listed_( arg.extList, EXT_fog_coord ) && arg.checkExt( EXT_fog_coord, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.fogCoordPointerEXT = GETFN_(proto::FogCoordPointerEXTFn, "glFogCoordPointerEXT")) && ok;
		ok = !!(gl.fogCoorddEXT = GETFN_(proto::FogCoorddEXTFn, "glFogCoorddEXT")) && ok;
		ok = !!(gl.fogCoorddvEXT = GETFN_(proto::FogCoorddvEXTFn, "glFogCoorddvEXT")) && ok;
		ok = !!(gl.fogCoordfEXT = GETFN_(proto::FogCoordfEXTFn, "glFogCoordfEXT")) && ok;
		ok = !!(gl.fogCoordfvEXT = GETFN_(proto::FogCoordfvEXTFn, "glFogCoordfvEXT")) && ok;
		gl.EXT_fog_coord = ok;
	}
	//- end: EXT_fog_coord

	//-//-- 
	//- Ext: EXT_framebuffer_blit
	static char const* EXT_framebuffer_blit = "EXT_framebuffer_blit";
	if( listed_( arg.extList, EXT_framebuffer_blit ) && arg.checkExt( EXT_framebuffer_blit, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.blitFramebufferEXT = GETFN_(proto::BlitFramebufferEXTFn, "glBlitFramebufferEXT")) && ok;
		gl.EXT_framebuffer_blit = ok;
	}
	//- end: EXT_framebuffer_blit

	//-//-- 
	//- Ext: EXT_framebuffer_multisample
	static char const* EXT_framebuffer_multisample = "EXT_framebuffer_multisample";
	if( listed_( arg.extList, EXT_framebuffer_multisample ) && arg.checkExt( EXT_framebuffer_multisample, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.renderbufferStorageMultisampleEXT = GETFN_(proto::RenderbufferStorageMultisampleEXTFn, "glRenderbufferStorageMultisampleEXT")) && ok;
		gl.EXT_framebuffer_multisample = ok;
	}
	//- end: EXT_framebuffer_multisample

	//-//-- 
	//- Ext: EXT_framebuffer_object
	static char const* EXT_framebuffer_object = "EXT_framebuffer_object";
	if( listed_( arg.extList, EXT_framebuffer_object ) && arg.checkExt( EXT_framebuffer_object, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.bindFramebufferEXT = GETFN_(proto::BindFramebufferEXTFn, "glBindFramebufferEXT")) && ok;
		ok = !!(gl.bindRenderbufferEXT = GETFN_(proto::BindRenderbufferEXTFn, "glBindRenderbufferEXT")) && ok;
		ok = !!(gl.checkFramebufferStatusEXT = GETFN_(proto::CheckFramebufferStatusEXTFn, "glCheckFramebufferStatusEXT")) && ok;
		ok = !!(gl.deleteFramebuffersEXT = GETFN_(proto::DeleteFramebuffersEXTFn, "glDeleteFramebuffersEXT")) && ok;
		ok = !!(gl.deleteRenderbuffersEXT = GETFN_(proto::DeleteRenderbuffersEXTFn, "glDeleteRenderbuffersEXT")) && ok;
		ok = !!(gl.framebufferRenderbufferEXT = GETFN_(proto::FramebufferRenderbufferEXTFn, "glFramebufferRenderbufferEXT")) && ok;
		ok = !!(gl.framebufferTexture1DEXT = GETFN_(proto::FramebufferTexture1DEXTFn, "glFramebufferTexture1DEXT")) && ok;
		ok = !!(gl.framebufferTexture2DEXT = GETFN_(proto::FramebufferTexture2DEXTFn, "glFramebufferTexture2DEXT")) && ok;
		ok = !!(gl.framebufferTexture3DEXT = GETFN_(proto::FramebufferTexture3DEXTFn, "glFramebufferTexture3DEXT")) && ok;
		ok = !!(gl.genFramebuffersEXT = GETFN_(proto::GenFramebuffersEXTFn, "glGenFramebuffersEXT")) && ok;
		ok = !!(gl.genRenderbuffersEXT = GETFN_(proto::GenRenderbuffersEXTFn, "glGenRenderbuffersEXT")) && ok;
		ok = !!(gl.generateMipmapEXT = GETFN_(proto::GenerateMipmapEXTFn, "glGenerateMipmapEXT")) && ok;
		ok = !!(gl.getFramebufferAttachmentParameterivEXT = GETFN_(proto::GetFramebufferAttachmentParameterivEXTFn, "glGetFramebufferAttachmentParameterivEXT")) && ok;
		ok = !!(gl.getRenderbufferParameterivEXT = GETFN_(proto::GetRenderbufferParameterivEXTFn, "glGetRenderbufferParameterivEXT")) && ok;
		ok = !!(gl.isFramebufferEXT = GETFN_(proto::IsFramebufferEXTFn, "glIsFramebufferEXT")) && ok;
		ok = !!(gl.isRenderbufferEXT = GETFN_(proto::IsRenderbufferEXTFn, "glIsRenderbufferEXT")) && ok;
		ok = !!(gl.renderbufferStorageEXT = GETFN_(proto::RenderbufferStorageEXTFn, "glRenderbufferStorageEXT")) && ok;
		gl.EXT_framebuffer_object = ok;
	}
	//- end: EXT_framebuffer_object

	//-//-- 
	//- Ext: EXT_geometry_shader4
	static char const* EXT_geometry_shader4 = "EXT_geometry_shader4";
	if( listed_( arg.extList, EXT_geometry_shader4 ) && arg.checkExt( EXT_geometry_shader4, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.programParameteriEXT = GETFN_(proto::ProgramParameteriEXTFn, "glProgramParameteriEXT")) && ok;
		gl.EXT_geometry_shader4 = ok;
	}
	//- end: EXT_geometry_shader4

	//-//-- 
	//- Ext: EXT_gpu_program_parameters
	static char const* EXT_gpu_program_parameters = "EXT_gpu_program_parameters";
	if( listed_( arg.extList, EXT_gpu_program_parameters ) && arg.checkExt( EXT_gpu_program_parameters, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.programEnvParameters4fvEXT = GETFN_(proto::ProgramEnvParameters4fvEXTFn, "glProgramEnvParameters4fvEXT")) && ok;
		ok = !!(gl.programLocalParameters4fvEXT = GETFN_(proto::ProgramLocalParameters4fvEXTFn, "glProgramLocalParameters4fvEXT")) && ok;
		gl.EXT_gpu_program_parameters = ok;
	}
	//- end: EXT_gpu_program_parameters

	//-//-- 
	//- Ext: EXT_gpu_shader4
	static char const* EXT_gpu_shader4 = "EXT_gpu_shader4";
	if( listed_( arg.extList, EXT_gpu_shader4 ) && arg.checkExt( EXT_gpu_shader4, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.bindFragDataLocationEXT = GETFN_(proto::BindFragDataLocationEXTFn, "glBindFragDataLocationEXT")) && ok;
		ok = !!(gl.getFragDataLocationEXT = GETFN_(proto::GetFragDataLocationEXTFn, "glGetFragDataLocationEXT")) && ok;
		ok = !!(gl.getUniformuivEXT = GETFN_(proto::GetUniformuivEXTFn, "glGetUniformuivEXT")) && ok;
		ok = !!(gl.uniform1uiEXT = GETFN_(proto::Uniform1uiEXTFn, "glUniform1uiEXT")) && ok;
		ok = !!(gl.uniform1uivEXT = GETFN_(proto::Uniform1uivEXTFn, "glUniform1uivEXT")) && ok;
		ok = !!(gl.uniform2uiEXT = GETFN_(proto::Uniform2uiEXTFn, "glUniform2uiEXT")) && ok;
		ok = !!(gl.uniform2uivEXT = GETFN_(proto::Uniform2uivEXTFn, "glUniform2uivEXT")) && ok;
		ok = !!(gl.uniform3uiEXT = GETFN_(proto::Uniform3uiEXTFn, "glUniform3uiEXT")) && ok;
		ok = !!(gl.uniform3uivEXT = GETFN_(proto::Uniform3uivEXTFn, "glUniform3uivEXT")) && ok;
		ok = !!(gl.uniform4uiEXT = GETFN_(proto::Uniform4uiEXTFn, "glUniform4uiEXT")) && ok;
		ok = !!(gl.uniform4uivEXT = GETFN_(proto::Uniform4uivEXTFn, "glUniform4uivEXT")) && ok;
		gl.EXT_gpu_shader4 = ok;
	}
	//- end: EXT_gpu_shader4

	//-//-- 
	//- Ext: EXT_histogram
	static char const* EXT_histogram = "EXT_histogram";
	if( listed_( arg.extList, EXT_histogram ) && arg.checkExt( EXT_histogram, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getHistogramEXT = GETFN_(proto::GetHistogramEXTFn, "glGetHistogramEXT")) && ok;
		ok = !!(gl.getHistogramParameterfvEXT = GETFN_(proto::GetHistogramParameterfvEXTFn, "glGetHistogramParameterfvEXT")) && ok;
		ok = !!(gl.getHistogramParameterivEXT = GETFN_(proto::GetHistogramParameterivEXTFn, "glGetHistogramParameterivEXT")) && ok;
		ok = !!(gl.getMinmaxEXT = GETFN_(proto::GetMinmaxEXTFn, "glGetMinmaxEXT")) && ok;
		ok = !!(gl.getMinmaxParameterfvEXT = GETFN_(proto::GetMinmaxParameterfvEXTFn, "glGetMinmaxParameterfvEXT")) && ok;
		ok = !!(gl.getMinmaxParameterivEXT = GETFN_(proto::GetMinmaxParameterivEXTFn, "glGetMinmaxParameterivEXT")) && ok;
		ok = !!(gl.histogramEXT = GETFN_(proto::HistogramEXTFn, "glHistogramEXT")) && ok;
		ok = !!(gl.minmaxEXT = GETFN_(proto::MinmaxEXTFn, "glMinmaxEXT")) && ok;
		ok = !!(gl.resetHistogramEXT = GETFN_(proto::ResetHistogramEXTFn, "glResetHistogramEXT")) && ok;
		ok = !!(gl.resetMinmaxEXT = GETFN_(proto::ResetMinmaxEXTFn, "glResetMinmaxEXT")) && ok;
		gl.EXT_histogram = ok;
	}
	//- end: EXT_histogram

	//-//-- 
	//- Ext: EXT_index_func
	static char const* EXT_index_func = "EXT_index_func";
	if( listed_( arg.extList, EXT_index_func ) && arg.checkExt( EXT_index_func, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.indexFuncEXT = GETFN_(proto::IndexFuncEXTFn, "glIndexFuncEXT")) && ok;
		gl.EXT_index_func = ok;
	}
	//- end: EXT_index_func

	//-//-- 
	//- Ext: EXT_index_material
	static char const* EXT_index_material = "EXT_index_material";
	if( listed_( arg.extList, EXT_index_material ) && arg.checkExt( EXT_index_material, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.indexMaterialEXT = GETFN_(proto::IndexMaterialEXTFn, "glIndexMaterialEXT")) && ok;
		gl.EXT_index_material = ok;
	}
	//- end: EXT_index_material

	//-//-- 
	//- Ext: EXT_light_texture
	static char const* EXT_light_texture = "EXT_light_texture";
	if( listed_( arg.extList, EXT_light_texture ) && arg.checkExt( EXT_light_texture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.applyTextureEXT = GETFN_(proto::ApplyTextureEXTFn, "glApplyTextureEXT")) && ok;
		ok = !!(gl.textureLightEXT = GETFN_(proto::TextureLightEXTFn, "glTextureLightEXT")) && ok;
		ok = !!(gl.textureMaterialEXT = GETFN_(proto::TextureMaterialEXTFn, "glTextureMaterialEXT")) && ok;
		gl.EXT_light_texture = ok;
	}
	//- end: EXT_light_texture

	//-//-- 
	//- Ext: EXT_multi_draw_arrays
	static char const* EXT_multi_draw_arrays = "EXT_multi_draw_arrays";
	if( listed_( arg.extList, EXT_multi_draw_arrays ) && arg.checkExt( EXT_multi_draw_arrays, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.multiDrawArraysEXT = GETFN_(proto::MultiDrawArraysEXTFn, "glMultiDrawArraysEXT")) && ok;
		ok = !!(gl.multiDrawElementsEXT = GETFN_(proto::MultiDrawElementsEXTFn, "glMultiDrawElementsEXT")) && ok;
		gl.EXT_multi_draw_arrays = ok;
	}
	//- end: EXT_multi_draw_arrays

	//-//-- 
	//- Ext: EXT_multisample
	static char const* EXT_multisample = "EXT_multisample";
	if( listed_( arg.extList, EXT_multisample ) && arg.checkExt( EXT_multisample, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.sampleMaskEXT = GETFN_(proto::SampleMaskEXTFn, "glSampleMaskEXT")) && ok;
		ok = !!(gl.samplePatternEXT = GETFN_(proto::SamplePatternEXTFn, "glSamplePatternEXT")) && ok;
		gl.EXT_multisample = ok;
	}
	//- end: EXT_multisample

	//-//-- 
	//- Ext: EXT_paletted_texture
	static char const* EXT_paletted_texture = "EXT_paletted_texture";
	if( listed_( arg.extList, EXT_paletted_texture ) && arg.checkExt( EXT_paletted_texture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.colorTableEXT = GETFN_(proto::ColorTableEXTFn, "glColorTableEXT")) && ok;
		ok = !!(gl.getColorTableEXT = GETFN_(proto::GetColorTableEXTFn, "glGetColorTableEXT")) && ok;
		ok = !!(gl.getColorTableParameterfvEXT = GETFN_(proto::GetColorTableParameterfvEXTFn, "glGetColorTableParameterfvEXT")) && ok;
		ok = !!(gl.getColorTableParameterivEXT = GETFN_(proto::GetColorTableParameterivEXTFn, "glGetColorTableParameterivEXT")) && ok;
		gl.EXT_paletted_texture = ok;
	}
	//- end: EXT_paletted_texture

	//-//-- 
	//- Ext: EXT_pixel_transform
	static char const* EXT_pixel_transform = "EXT_pixel_transform";
	if( listed_( arg.extList, EXT_pixel_transform ) && arg.checkExt( EXT_pixel_transform, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getPixelTransformParameterfvEXT = GETFN_(proto::GetPixelTransformParameterfvEXTFn, "glGetPixelTransformParameterfvEXT")) && ok;
		ok = !!(gl.getPixelTransformParameterivEXT = GETFN_(proto::GetPixelTransformParameterivEXTFn, "glGetPixelTransformParameterivEXT")) && ok;
		ok = !!(gl.pixelTransformParameterfEXT = GETFN_(proto::PixelTransformParameterfEXTFn, "glPixelTransformParameterfEXT")) && ok;
		ok = !!(gl.pixelTransformParameterfvEXT = GETFN_(proto::PixelTransformParameterfvEXTFn, "glPixelTransformParameterfvEXT")) && ok;
		ok = !!(gl.pixelTransformParameteriEXT = GETFN_(proto::PixelTransformParameteriEXTFn, "glPixelTransformParameteriEXT")) && ok;
		ok = !!(gl.pixelTransformParameterivEXT = GETFN_(proto::PixelTransformParameterivEXTFn, "glPixelTransformParameterivEXT")) && ok;
		gl.EXT_pixel_transform = ok;
	}
	//- end: EXT_pixel_transform

	//-//-- 
	//- Ext: EXT_point_parameters
	static char const* EXT_point_parameters = "EXT_point_parameters";
	if( listed_( arg.extList, EXT_point_parameters ) && arg.checkExt( EXT_point_parameters, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.pointParameterfEXT = GETFN_(proto::PointParameterfEXTFn, "glPointParameterfEXT")) && ok;
		ok = !!(gl.pointParameterfvEXT = GETFN_(proto::PointParameterfvEXTFn, "glPointParameterfvEXT")) && ok;
		gl.EXT_point_parameters = ok;
	}
	//- end: EXT_point_parameters

	//-//-- 
	//- Ext: EXT_polygon_offset
	static char const* EXT_polygon_offset = "EXT_polygon_offset";
	if( listed_( arg.extList, EXT_polygon_offset ) && arg.checkExt( EXT_polygon_offset, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.polygonOffsetEXT = GETFN_(proto::PolygonOffsetEXTFn, "glPolygonOffsetEXT")) && ok;
		gl.EXT_polygon_offset = ok;
	}
	//- end: EXT_polygon_offset

	//-//-- 
	//- Ext: EXT_polygon_offset_clamp
	static char const* EXT_polygon_offset_clamp = "EXT_polygon_offset_clamp";
	if( listed_( arg.extList, EXT_polygon_offset_clamp ) && arg.checkExt( EXT_polygon_offset_clamp, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.polygonOffsetClampEXT = GETFN_(proto::PolygonOffsetClampEXTFn, "glPolygonOffsetClampEXT")) && ok;
		gl.EXT_polygon_offset_clamp = ok;
	}
	//- end: EXT_polygon_offset_clamp

	//-//-- 
	//- Ext: EXT_provoking_vertex
	static char const* EXT_provoking_vertex = "EXT_provoking_vertex";
	if( listed_( arg.extList, EXT_provoking_vertex ) && arg.checkExt( EXT_provoking_vertex, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.provokingVertexEXT = GETFN_(proto::ProvokingVertexEXTFn, "glProvokingVertexEXT")) && ok;
		gl.EXT_provoking_vertex = ok;
	}
	//- end: EXT_provoking_vertex

	//-//-- 
	//- Ext: EXT_raster_multisample
	static char const* EXT_raster_multisample = "EXT_raster_multisample";
	if( listed_( arg.extList, EXT_raster_multisample ) && arg.checkExt( EXT_raster_multisample, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.rasterSamplesEXT = GETFN_(proto::RasterSamplesEXTFn, "glRasterSamplesEXT")) && ok;
		gl.EXT_raster_multisample = ok;
	}
	//- end: EXT_raster_multisample

	//-//-- 
	//- Ext: EXT_secondary_color
	static char const* EXT_secondary_color = "EXT_secondary_color";
	if( listed_( arg.extList, EXT_secondary_color ) && arg.checkExt( EXT_secondary_color, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.secondaryColor3bEXT = GETFN_(proto::SecondaryColor3bEXTFn, "glSecondaryColor3bEXT")) && ok;
		ok = !!(gl.secondaryColor3bvEXT = GETFN_(proto::SecondaryColor3bvEXTFn, "glSecondaryColor3bvEXT")) && ok;
		ok = !!(gl.secondaryColor3dEXT = GETFN_(proto::SecondaryColor3dEXTFn, "glSecondaryColor3dEXT")) && ok;
		ok = !!(gl.secondaryColor3dvEXT = GETFN_(proto::SecondaryColor3dvEXTFn, "glSecondaryColor3dvEXT")) && ok;
		ok = !!(gl.secondaryColor3fEXT = GETFN_(proto::SecondaryColor3fEXTFn, "glSecondaryColor3fEXT")) && ok;
		ok = !!(gl.secondaryColor3fvEXT = GETFN_(proto::SecondaryColor3fvEXTFn, "glSecondaryColor3fvEXT")) && ok;
		ok = !!(gl.secondaryColor3iEXT = GETFN_(proto::SecondaryColor3iEXTFn, "glSecondaryColor3iEXT")) && ok;
		ok = !!(gl.secondaryColor3ivEXT = GETFN_(proto::SecondaryColor3ivEXTFn, "glSecondaryColor3ivEXT")) && ok;
		ok = !!(gl.secondaryColor3sEXT = GETFN_(proto::SecondaryColor3sEXTFn, "glSecondaryColor3sEXT")) && ok;
		ok = !!(gl.secondaryColor3svEXT = GETFN_(proto::SecondaryColor3svEXTFn, "glSecondaryColor3svEXT")) && ok;
		ok = !!(gl.secondaryColor3ubEXT = GETFN_(proto::SecondaryColor3ubEXTFn, "glSecondaryColor3ubEXT")) && ok;
		ok = !!(gl.secondaryColor3ubvEXT = GETFN_(proto::SecondaryColor3ubvEXTFn, "glSecondaryColor3ubvEXT")) && ok;
		ok = !!(gl.secondaryColor3uiEXT = GETFN_(proto::SecondaryColor3uiEXTFn, "glSecondaryColor3uiEXT")) && ok;
		ok = !!(gl.secondaryColor3uivEXT = GETFN_(proto::SecondaryColor3uivEXTFn, "glSecondaryColor3uivEXT")) && ok;
		ok = !!(gl.secondaryColor3usEXT = GETFN_(proto::SecondaryColor3usEXTFn, "glSecondaryColor3usEXT")) && ok;
		ok = !!(gl.secondaryColor3usvEXT = GETFN_(proto::SecondaryColor3usvEXTFn, "glSecondaryColor3usvEXT")) && ok;
		ok = !!(gl.secondaryColorPointerEXT = GETFN_(proto::SecondaryColorPointerEXTFn, "glSecondaryColorPointerEXT")) && ok;
		gl.EXT_secondary_color = ok;
	}
	//- end: EXT_secondary_color

	//-//-- 
	//- Ext: EXT_separate_shader_objects
	static char const* EXT_separate_shader_objects = "EXT_separate_shader_objects";
	if( listed_( arg.extList, EXT_separate_shader_objects ) && arg.checkExt( EXT_separate_shader_objects, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.activeProgramEXT = GETFN_(proto::ActiveProgramEXTFn, "glActiveProgramEXT")) && ok;
		ok = !!(gl.createShaderProgramEXT = GETFN_(proto::CreateShaderProgramEXTFn, "glCreateShaderProgramEXT")) && ok;
		ok = !!(gl.useShaderProgramEXT = GETFN_(proto::UseShaderProgramEXTFn, "glUseShaderProgramEXT")) && ok;
		gl.EXT_separate_shader_objects = ok;
	}
	//- end: EXT_separate_shader_objects

	//-//-- 
	//- Ext: EXT_shader_image_load_store
	static char const* EXT_shader_image_load_store = "EXT_shader_image_load_store";
	if( listed_( arg.extList, EXT_shader_image_load_store ) && arg.checkExt( EXT_shader_image_load_store, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.bindImageTextureEXT = GETFN_(proto::BindImageTextureEXTFn, "glBindImageTextureEXT")) && ok;
		ok = !!(gl.memoryBarrierEXT = GETFN_(proto::MemoryBarrierEXTFn, "glMemoryBarrierEXT")) && ok;
		gl.EXT_shader_image_load_store = ok;
	}
	//- end: EXT_shader_image_load_store

	//-//-- 
	//- Ext: EXT_stencil_clear_tag
	static char const* EXT_stencil_clear_tag = "EXT_stencil_clear_tag";
	if( listed_( arg.extList, EXT_stencil_clear_tag ) && arg.checkExt( EXT_stencil_clear_tag, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.stencilClearTagEXT = GETFN_(proto::StencilClearTagEXTFn, "glStencilClearTagEXT")) && ok;
		gl.EXT_stencil_clear_tag = ok;
	}
	//- end: EXT_stencil_clear_tag

	//-//-- 
	//- Ext: EXT_stencil_two_side
	static char const* EXT_stencil_two_side = "EXT_stencil_two_side";
	if( listed_( arg.extList, EXT_stencil_two_side ) && arg.checkExt( EXT_stencil_two_side, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.activeStencilFaceEXT = GETFN_(proto::ActiveStencilFaceEXTFn, "glActiveStencilFaceEXT")) && ok;
		gl.EXT_stencil_two_side = ok;
	}
	//- end: EXT_stencil_two_side

	//-//-- 
	//- Ext: EXT_subtexture
	static char const* EXT_subtexture = "EXT_subtexture";
	if( listed_( arg.extList, EXT_subtexture ) && arg.checkExt( EXT_subtexture, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.texSubImage1DEXT = GETFN_(proto::TexSubImage1DEXTFn, "glTexSubImage1DEXT")) && ok;
		ok = !!(gl.texSubImage2DEXT = GETFN_(proto::TexSubImage2DEXTFn, "glTexSubImage2DEXT")) && ok;
		gl.EXT_subtexture = ok;
	}
	//- end: EXT_subtexture

	//-//-- 
	//- Ext: EXT_texture3D
	static char const* EXT_texture3D = "EXT_texture3D";
	if( listed_( arg.extList, EXT_texture3D ) && arg.checkExt( EXT_texture3D, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.texImage3DEXT = GETFN_(proto::TexImage3DEXTFn, "glTexImage3DEXT")) && ok;
		ok = !!(gl.texSubImage3DEXT = GETFN_(proto::TexSubImage3DEXTFn, "glTexSubImage3DEXT")) && ok;
		gl.EXT_texture3D = ok;
	}
	//- end: EXT_texture3D

	//-//-- 
	//- Ext: EXT_texture_array
	static char const* EXT_texture_array = "EXT_texture_array";
	if( listed_( arg.extList, EXT_texture_array ) && arg.checkExt( EXT_texture_array, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.framebufferTextureLayerEXT = GETFN_(proto::FramebufferTextureLayerEXTFn, "glFramebufferTextureLayerEXT")) && ok;
		gl.EXT_texture_array = ok;
	}
	//- end: EXT_texture_array

	//-//-- 
	//- Ext: EXT_texture_buffer_object
	static char const* EXT_texture_buffer_object = "EXT_texture_buffer_object";
	if( listed_( arg.extList, EXT_texture_buffer_object ) && arg.checkExt( EXT_texture_buffer_object, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.texBufferEXT = GETFN_(proto::TexBufferEXTFn, "glTexBufferEXT")) && ok;
		gl.EXT_texture_buffer_object = ok;
	}
	//- end: EXT_texture_buffer_object

	//-//-- 
	//- Ext: EXT_texture_filter_minmax
	static char const* EXT_texture_filter_minmax = "EXT_texture_filter_minmax";
	if( listed_( arg.extList, EXT_texture_filter_minmax ) && arg.checkExt( EXT_texture_filter_minmax, arg.user ) )
	{
		bool ok = true;

		if( !gl.EXT_raster_multisample ) {
			ok = !!(gl.rasterSamplesEXT = GETFN_(proto::RasterSamplesEXTFn, "glRasterSamplesEXT")) && ok;
		}
		gl.EXT_texture_filter_minmax = ok;
	}
	//- end: EXT_texture_filter_minmax

	//-//-- 
	//- Ext: EXT_texture_integer
	static char const* EXT_texture_integer = "EXT_texture_integer";
	if( listed_( arg.extList, EXT_texture_integer ) && arg.checkExt( EXT_texture_integer, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.clearColorIiEXT = GETFN_(proto::ClearColorIiEXTFn, "glClearColorIiEXT")) && ok;
		ok = !!(gl.clearColorIuiEXT = GETFN_(proto::ClearColorIuiEXTFn, "glClearColorIuiEXT")) && ok;
		ok = !!(gl.getTexParameterIivEXT = GETFN_(proto::GetTexParameterIivEXTFn, "glGetTexParameterIivEXT")) && ok;
		ok = !!(gl.getTexParameterIuivEXT = GETFN_(proto::GetTexParameterIuivEXTFn, "glGetTexParameterIuivEXT")) && ok;
		ok = !!(gl.texParameterIivEXT = GETFN_(proto::TexParameterIivEXTFn, "glTexParameterIivEXT")) && ok;
		ok = !!(gl.texParameterIuivEXT = GETFN_(proto::TexParameterIuivEXTFn, "glTexParameterIuivEXT")) && ok;
		gl.EXT_texture_integer = ok;
	}
	//- end: EXT_texture_integer

	//-//-- 
	//- Ext: EXT_texture_object
	static char const* EXT_texture_object = "EXT_texture_object";
	if( listed_( arg.extList, EXT_texture_object ) && arg.checkExt( EXT_texture_object, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.areTexturesResidentEXT = GETFN_(proto::AreTexturesResidentEXTFn, "glAreTexturesResidentEXT")) && ok;
		ok = !!(gl.bindTextureEXT = GETFN_(proto::BindTextureEXTFn, "glBindTextureEXT")) && ok;
		ok = !!(gl.deleteTexturesEXT = GETFN_(proto::DeleteTexturesEXTFn, "glDeleteTexturesEXT")) && ok;
		ok = !!(gl.genTexturesEXT = GETFN_(proto::GenTexturesEXTFn, "glGenTexturesEXT")) && ok;
		ok = !!(gl.isTextureEXT = GETFN_(proto::IsTextureEXTFn, "glIsTextureEXT")) && ok;
		ok = !!(gl.prioritizeTexturesEXT = GETFN_(proto::PrioritizeTexturesEXTFn, "glPrioritizeTexturesEXT")) && ok;
		gl.EXT_texture_object = ok;
	}
	//- end: EXT_texture_object

	//-//-- 
	//- Ext: EXT_texture_perturb_normal
	static char const* EXT_texture_perturb_normal = "EXT_texture_perturb_normal";
	if( listed_( arg.extList, EXT_texture_perturb_normal ) && arg.checkExt( EXT_texture_perturb_normal, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.textureNormalEXT = GETFN_(proto::TextureNormalEXTFn, "glTextureNormalEXT")) && ok;
		gl.EXT_texture_perturb_normal = ok;
	}
	//- end: EXT_texture_perturb_normal

	//-//-- 
	//- Ext: EXT_timer_query
	static char const* EXT_timer_query = "EXT_timer_query";
	if( listed_( arg.extList, EXT_timer_query ) && arg.checkExt( EXT_timer_query, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getQueryObjecti64vEXT = GETFN_(proto::GetQueryObjecti64vEXTFn, "glGetQueryObjecti64vEXT")) && ok;
		ok = !!(gl.getQueryObjectui64vEXT = GETFN_(proto::GetQueryObjectui64vEXTFn, "glGetQueryObjectui64vEXT")) && ok;
		gl.EXT_timer_query = ok;
	}
	//- end: EXT_timer_query

	//-//-- 
	//- Ext: EXT_transform_feedback
	static char const* EXT_transform_feedback = "EXT_transform_feedback";
	if( listed_( arg.extList, EXT_transform_feedback ) && arg.checkExt( EXT_transform_feedback, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.beginTransformFeedbackEXT = GETFN_(proto::BeginTransformFeedbackEXTFn, "glBeginTransformFeedbackEXT")) && ok;
		ok = !!(gl.bindBufferBaseEXT = GETFN_(proto::BindBufferBaseEXTFn, "glBindBufferBaseEXT")) && ok;
		ok = !!(gl.bindBufferOffsetEXT = GETFN_(proto::BindBufferOffsetEXTFn, "glBindBufferOffsetEXT")) && ok;
		ok = !!(gl.bindBufferRangeEXT = GETFN_(proto::BindBufferRangeEXTFn, "glBindBufferRangeEXT")) && ok;
		ok = !!(gl.endTransformFeedbackEXT = GETFN_(proto::EndTransformFeedbackEXTFn, "glEndTransformFeedbackEXT")) && ok;
		ok = !!(gl.getTransformFeedbackVaryingEXT = GETFN_(proto::GetTransformFeedbackVaryingEXTFn, "glGetTransformFeedbackVaryingEXT")) && ok;
		ok = !!(gl.transformFeedbackVaryingsEXT = GETFN_(proto::TransformFeedbackVaryingsEXTFn, "glTransformFeedbackVaryingsEXT")) && ok;
		gl.EXT_transform_feedback = ok;
	}
	//- end: EXT_transform_feedback

	//-//-- 
	//- Ext: EXT_vertex_array
	static char const* EXT_vertex_array = "EXT_vertex_array";
	if( listed_( arg.extList, EXT_vertex_array ) && arg.checkExt( EXT_vertex_array, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.arrayElementEXT = GETFN_(proto::ArrayElementEXTFn, "glArrayElementEXT")) && ok;
		ok = !!(gl.colorPointerEXT = GETFN_(proto::ColorPointerEXTFn, "glColorPointerEXT")) && ok;
		ok = !!(gl.drawArraysEXT = GETFN_(proto::DrawArraysEXTFn, "glDrawArraysEXT")) && ok;
		ok = !!(gl.edgeFlagPointerEXT = GETFN_(proto::EdgeFlagPointerEXTFn, "glEdgeFlagPointerEXT")) && ok;
		ok = !!(gl.getPointervEXT = GETFN_(proto::GetPointervEXTFn, "glGetPointervEXT")) && ok;
		ok = !!(gl.indexPointerEXT = GETFN_(proto::IndexPointerEXTFn, "glIndexPointerEXT")) && ok;
		ok = !!(gl.normalPointerEXT = GETFN_(proto::NormalPointerEXTFn, "glNormalPointerEXT")) && ok;
		ok = !!(gl.texCoordPointerEXT = GETFN_(proto::TexCoordPointerEXTFn, "glTexCoordPointerEXT")) && ok;
		ok = !!(gl.vertexPointerEXT = GETFN_(proto::VertexPointerEXTFn, "glVertexPointerEXT")) && ok;
		gl.EXT_vertex_array = ok;
	}
	//- end: EXT_vertex_array

	//-//-- 
	//- Ext: EXT_vertex_attrib_64bit
	static char const* EXT_vertex_attrib_64bit = "EXT_vertex_attrib_64bit";
	if( listed_( arg.extList, EXT_vertex_attrib_64bit ) && arg.checkExt( EXT_vertex_attrib_64bit, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.getVertexAttribLdvEXT = GETFN_(proto::GetVertexAttribLdvEXTFn, "glGetVertexAttribLdvEXT")) && ok;
		ok = !!(gl.vertexAttribL1dEXT = GETFN_(proto::VertexAttribL1dEXTFn, "glVertexAttribL1dEXT")) && ok;
		ok = !!(gl.vertexAttribL1dvEXT = GETFN_(proto::VertexAttribL1dvEXTFn, "glVertexAttribL1dvEXT")) && ok;
		ok = !!(gl.vertexAttribL2dEXT = GETFN_(proto::VertexAttribL2dEXTFn, "glVertexAttribL2dEXT")) && ok;
		ok = !!(gl.vertexAttribL2dvEXT = GETFN_(proto::VertexAttribL2dvEXTFn, "glVertexAttribL2dvEXT")) && ok;
		ok = !!(gl.vertexAttribL3dEXT = GETFN_(proto::VertexAttribL3dEXTFn, "glVertexAttribL3dEXT")) && ok;
		ok = !!(gl.vertexAttribL3dvEXT = GETFN_(proto::VertexAttribL3dvEXTFn, "glVertexAttribL3dvEXT")) && ok;
		ok = !!(gl.vertexAttribL4dEXT = GETFN_(proto::VertexAttribL4dEXTFn, "glVertexAttribL4dEXT")) && ok;
		ok = !!(gl.vertexAttribL4dvEXT = GETFN_(proto::VertexAttribL4dvEXTFn, "glVertexAttribL4dvEXT")) && ok;
		ok = !!(gl.vertexAttribLPointerEXT = GETFN_(proto::VertexAttribLPointerEXTFn, "glVertexAttribLPointerEXT")) && ok;
		gl.EXT_vertex_attrib_64bit = ok;
	}
	//- end: EXT_vertex_attrib_64bit

	//-//-- 
	//- Ext: EXT_vertex_shader
	static char const* EXT_vertex_shader = "EXT_vertex_shader";
	if( listed_( arg.extList, EXT_vertex_shader ) && arg.checkExt( EXT_vertex_shader, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.beginVertexShaderEXT = GETFN_(proto::BeginVertexShaderEXTFn, "glBeginVertexShaderEXT")) && ok;
		ok = !!(gl.bindLightParameterEXT = GETFN_(proto::BindLightParameterEXTFn, "glBindLightParameterEXT")) && ok;
		ok = !!(gl.bindMaterialParameterEXT = GETFN_(proto::BindMaterialParameterEXTFn, "glBindMaterialParameterEXT")) && ok;
		ok = !!(gl.bindParameterEXT = GETFN_(proto::BindParameterEXTFn, "glBindParameterEXT")) && ok;
		ok = !!(gl.bindTexGenParameterEXT = GETFN_(proto::BindTexGenParameterEXTFn, "glBindTexGenParameterEXT")) && ok;
		ok = !!(gl.bindTextureUnitParameterEXT = GETFN_(proto::BindTextureUnitParameterEXTFn, "glBindTextureUnitParameterEXT")) && ok;
		ok = !!(gl.bindVertexShaderEXT = GETFN_(proto::BindVertexShaderEXTFn, "glBindVertexShaderEXT")) && ok;
		ok = !!(gl.deleteVertexShaderEXT = GETFN_(proto::DeleteVertexShaderEXTFn, "glDeleteVertexShaderEXT")) && ok;
		ok = !!(gl.disableVariantClientStateEXT = GETFN_(proto::DisableVariantClientStateEXTFn, "glDisableVariantClientStateEXT")) && ok;
		ok = !!(gl.enableVariantClientStateEXT = GETFN_(proto::EnableVariantClientStateEXTFn, "glEnableVariantClientStateEXT")) && ok;
		ok = !!(gl.endVertexShaderEXT = GETFN_(proto::EndVertexShaderEXTFn, "glEndVertexShaderEXT")) && ok;
		ok = !!(gl.extractComponentEXT = GETFN_(proto::ExtractComponentEXTFn, "glExtractComponentEXT")) && ok;
		ok = !!(gl.genSymbolsEXT = GETFN_(proto::GenSymbolsEXTFn, "glGenSymbolsEXT")) && ok;
		ok = !!(gl.genVertexShadersEXT = GETFN_(proto::GenVertexShadersEXTFn, "glGenVertexShadersEXT")) && ok;
		ok = !!(gl.getInvariantBooleanvEXT = GETFN_(proto::GetInvariantBooleanvEXTFn, "glGetInvariantBooleanvEXT")) && ok;
		ok = !!(gl.getInvariantFloatvEXT = GETFN_(proto::GetInvariantFloatvEXTFn, "glGetInvariantFloatvEXT")) && ok;
		ok = !!(gl.getInvariantIntegervEXT = GETFN_(proto::GetInvariantIntegervEXTFn, "glGetInvariantIntegervEXT")) && ok;
		ok = !!(gl.getLocalConstantBooleanvEXT = GETFN_(proto::GetLocalConstantBooleanvEXTFn, "glGetLocalConstantBooleanvEXT")) && ok;
		ok = !!(gl.getLocalConstantFloatvEXT = GETFN_(proto::GetLocalConstantFloatvEXTFn, "glGetLocalConstantFloatvEXT")) && ok;
		ok = !!(gl.getLocalConstantIntegervEXT = GETFN_(proto::GetLocalConstantIntegervEXTFn, "glGetLocalConstantIntegervEXT")) && ok;
		ok = !!(gl.getVariantBooleanvEXT = GETFN_(proto::GetVariantBooleanvEXTFn, "glGetVariantBooleanvEXT")) && ok;
		ok = !!(gl.getVariantFloatvEXT = GETFN_(proto::GetVariantFloatvEXTFn, "glGetVariantFloatvEXT")) && ok;
		ok = !!(gl.getVariantIntegervEXT = GETFN_(proto::GetVariantIntegervEXTFn, "glGetVariantIntegervEXT")) && ok;
		ok = !!(gl.getVariantPointervEXT = GETFN_(proto::GetVariantPointervEXTFn, "glGetVariantPointervEXT")) && ok;
		ok = !!(gl.insertComponentEXT = GETFN_(proto::InsertComponentEXTFn, "glInsertComponentEXT")) && ok;
		ok = !!(gl.isVariantEnabledEXT = GETFN_(proto::IsVariantEnabledEXTFn, "glIsVariantEnabledEXT")) && ok;
		ok = !!(gl.setInvariantEXT = GETFN_(proto::SetInvariantEXTFn, "glSetInvariantEXT")) && ok;
		ok = !!(gl.setLocalConstantEXT = GETFN_(proto::SetLocalConstantEXTFn, "glSetLocalConstantEXT")) && ok;
		ok = !!(gl.shaderOp1EXT = GETFN_(proto::ShaderOp1EXTFn, "glShaderOp1EXT")) && ok;
		ok = !!(gl.shaderOp2EXT = GETFN_(proto::ShaderOp2EXTFn, "glShaderOp2EXT")) && ok;
		ok = !!(gl.shaderOp3EXT = GETFN_(proto::ShaderOp3EXTFn, "glShaderOp3EXT")) && ok;
		ok = !!(gl.swizzleEXT = GETFN_(proto::SwizzleEXTFn, "glSwizzleEXT")) && ok;
		ok = !!(gl.variantPointerEXT = GETFN_(proto::VariantPointerEXTFn, "glVariantPointerEXT")) && ok;
		ok = !!(gl.variantbvEXT = GETFN_(proto::VariantbvEXTFn, "glVariantbvEXT")) && ok;
		ok = !!(gl.variantdvEXT = GETFN_(proto::VariantdvEXTFn, "glVariantdvEXT")) && ok;
		ok = !!(gl.variantfvEXT = GETFN_(proto::VariantfvEXTFn, "glVariantfvEXT")) && ok;
		ok = !!(gl.variantivEXT = GETFN_(proto::VariantivEXTFn, "glVariantivEXT")) && ok;
		ok = !!(gl.variantsvEXT = GETFN_(proto::VariantsvEXTFn, "glVariantsvEXT")) && ok;
		ok = !!(gl.variantubvEXT = GETFN_(proto::VariantubvEXTFn, "glVariantubvEXT")) && ok;
		ok = !!(gl.variantuivEXT = GETFN_(proto::VariantuivEXTFn, "glVariantuivEXT")) && ok;
		ok = !!(gl.variantusvEXT = GETFN_(proto::VariantusvEXTFn, "glVariantusvEXT")) && ok;
		ok = !!(gl.writeMaskEXT = GETFN_(proto::WriteMaskEXTFn, "glWriteMaskEXT")) && ok;
		gl.EXT_vertex_shader = ok;
	}
	//- end: EXT_vertex_shader

	//-//-- 
	//- Ext: EXT_vertex_weighting
	static char const* EXT_vertex_weighting = "EXT_vertex_weighting";
	if( listed_( arg.extList, EXT_vertex_weighting ) && arg.checkExt( EXT_vertex_weighting, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.vertexWeightPointerEXT = GETFN_(proto::VertexWeightPointerEXTFn, "glVertexWeightPointerEXT")) && ok;
		ok = !!(gl.vertexWeightfEXT = GETFN_(proto::VertexWeightfEXTFn, "glVertexWeightfEXT")) && ok;
		ok = !!(gl.vertexWeightfvEXT = GETFN_(proto::VertexWeightfvEXTFn, "glVertexWeightfvEXT")) && ok;
		gl.EXT_vertex_weighting = ok;
	}
	//- end: EXT_vertex_weighting

	//-//-- 
	//- Ext: EXT_x11_sync_object
	static char const* EXT_x11_sync_object = "EXT_x11_sync_object";
	if( listed_( arg.extList, EXT_x11_sync_object ) && arg.checkExt( EXT_x11_sync_object, arg.user ) )
	{
		bool ok = true;

		ok = !!(gl.importSyncEXT = GETFN_(proto::ImportSyncEXTFn, "glImportSyncEXT")) && ok;
		gl.EXT_x11_sync_object = ok;
	}
	//- end: EXT_x11_sync_object

}

#undef GETFN_
//--    <<< ~ dlapi::gl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE3(dlapi,gl,detail)
//--//////////////////////////////////////////////////////////////////////////
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
