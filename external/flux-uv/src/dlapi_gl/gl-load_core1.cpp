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

void load_gl_core1_( GLapi& gl, LoaderArg_ const& arg )
{
	//-//-- 
	//- Core 1.0
	if( arg.major > 1 || (arg.major == 1 && arg.minor >= 0 ) )
	{
		bool ok = true;

		ok = !!(gl.accum = GETFN_(proto::AccumFn, "glAccum")) && ok;
		ok = !!(gl.alphaFunc = GETFN_(proto::AlphaFuncFn, "glAlphaFunc")) && ok;
		ok = !!(gl.begin = GETFN_(proto::BeginFn, "glBegin")) && ok;
		ok = !!(gl.bitmap = GETFN_(proto::BitmapFn, "glBitmap")) && ok;
		ok = !!(gl.blendFunc = GETFN_(proto::BlendFuncFn, "glBlendFunc")) && ok;
		ok = !!(gl.callList = GETFN_(proto::CallListFn, "glCallList")) && ok;
		ok = !!(gl.callLists = GETFN_(proto::CallListsFn, "glCallLists")) && ok;
		ok = !!(gl.clear = GETFN_(proto::ClearFn, "glClear")) && ok;
		ok = !!(gl.clearAccum = GETFN_(proto::ClearAccumFn, "glClearAccum")) && ok;
		ok = !!(gl.clearColor = GETFN_(proto::ClearColorFn, "glClearColor")) && ok;
		ok = !!(gl.clearDepth = GETFN_(proto::ClearDepthFn, "glClearDepth")) && ok;
		ok = !!(gl.clearIndex = GETFN_(proto::ClearIndexFn, "glClearIndex")) && ok;
		ok = !!(gl.clearStencil = GETFN_(proto::ClearStencilFn, "glClearStencil")) && ok;
		ok = !!(gl.clipPlane = GETFN_(proto::ClipPlaneFn, "glClipPlane")) && ok;
		ok = !!(gl.color3b = GETFN_(proto::Color3bFn, "glColor3b")) && ok;
		ok = !!(gl.color3bv = GETFN_(proto::Color3bvFn, "glColor3bv")) && ok;
		ok = !!(gl.color3d = GETFN_(proto::Color3dFn, "glColor3d")) && ok;
		ok = !!(gl.color3dv = GETFN_(proto::Color3dvFn, "glColor3dv")) && ok;
		ok = !!(gl.color3f = GETFN_(proto::Color3fFn, "glColor3f")) && ok;
		ok = !!(gl.color3fv = GETFN_(proto::Color3fvFn, "glColor3fv")) && ok;
		ok = !!(gl.color3i = GETFN_(proto::Color3iFn, "glColor3i")) && ok;
		ok = !!(gl.color3iv = GETFN_(proto::Color3ivFn, "glColor3iv")) && ok;
		ok = !!(gl.color3s = GETFN_(proto::Color3sFn, "glColor3s")) && ok;
		ok = !!(gl.color3sv = GETFN_(proto::Color3svFn, "glColor3sv")) && ok;
		ok = !!(gl.color3ub = GETFN_(proto::Color3ubFn, "glColor3ub")) && ok;
		ok = !!(gl.color3ubv = GETFN_(proto::Color3ubvFn, "glColor3ubv")) && ok;
		ok = !!(gl.color3ui = GETFN_(proto::Color3uiFn, "glColor3ui")) && ok;
		ok = !!(gl.color3uiv = GETFN_(proto::Color3uivFn, "glColor3uiv")) && ok;
		ok = !!(gl.color3us = GETFN_(proto::Color3usFn, "glColor3us")) && ok;
		ok = !!(gl.color3usv = GETFN_(proto::Color3usvFn, "glColor3usv")) && ok;
		ok = !!(gl.color4b = GETFN_(proto::Color4bFn, "glColor4b")) && ok;
		ok = !!(gl.color4bv = GETFN_(proto::Color4bvFn, "glColor4bv")) && ok;
		ok = !!(gl.color4d = GETFN_(proto::Color4dFn, "glColor4d")) && ok;
		ok = !!(gl.color4dv = GETFN_(proto::Color4dvFn, "glColor4dv")) && ok;
		ok = !!(gl.color4f = GETFN_(proto::Color4fFn, "glColor4f")) && ok;
		ok = !!(gl.color4fv = GETFN_(proto::Color4fvFn, "glColor4fv")) && ok;
		ok = !!(gl.color4i = GETFN_(proto::Color4iFn, "glColor4i")) && ok;
		ok = !!(gl.color4iv = GETFN_(proto::Color4ivFn, "glColor4iv")) && ok;
		ok = !!(gl.color4s = GETFN_(proto::Color4sFn, "glColor4s")) && ok;
		ok = !!(gl.color4sv = GETFN_(proto::Color4svFn, "glColor4sv")) && ok;
		ok = !!(gl.color4ub = GETFN_(proto::Color4ubFn, "glColor4ub")) && ok;
		ok = !!(gl.color4ubv = GETFN_(proto::Color4ubvFn, "glColor4ubv")) && ok;
		ok = !!(gl.color4ui = GETFN_(proto::Color4uiFn, "glColor4ui")) && ok;
		ok = !!(gl.color4uiv = GETFN_(proto::Color4uivFn, "glColor4uiv")) && ok;
		ok = !!(gl.color4us = GETFN_(proto::Color4usFn, "glColor4us")) && ok;
		ok = !!(gl.color4usv = GETFN_(proto::Color4usvFn, "glColor4usv")) && ok;
		ok = !!(gl.colorMask = GETFN_(proto::ColorMaskFn, "glColorMask")) && ok;
		ok = !!(gl.colorMaterial = GETFN_(proto::ColorMaterialFn, "glColorMaterial")) && ok;
		ok = !!(gl.copyPixels = GETFN_(proto::CopyPixelsFn, "glCopyPixels")) && ok;
		ok = !!(gl.cullFace = GETFN_(proto::CullFaceFn, "glCullFace")) && ok;
		ok = !!(gl.deleteLists = GETFN_(proto::DeleteListsFn, "glDeleteLists")) && ok;
		ok = !!(gl.depthFunc = GETFN_(proto::DepthFuncFn, "glDepthFunc")) && ok;
		ok = !!(gl.depthMask = GETFN_(proto::DepthMaskFn, "glDepthMask")) && ok;
		ok = !!(gl.depthRange = GETFN_(proto::DepthRangeFn, "glDepthRange")) && ok;
		ok = !!(gl.disable = GETFN_(proto::DisableFn, "glDisable")) && ok;
		ok = !!(gl.drawBuffer = GETFN_(proto::DrawBufferFn, "glDrawBuffer")) && ok;
		ok = !!(gl.drawPixels = GETFN_(proto::DrawPixelsFn, "glDrawPixels")) && ok;
		ok = !!(gl.edgeFlag = GETFN_(proto::EdgeFlagFn, "glEdgeFlag")) && ok;
		ok = !!(gl.edgeFlagv = GETFN_(proto::EdgeFlagvFn, "glEdgeFlagv")) && ok;
		ok = !!(gl.enable = GETFN_(proto::EnableFn, "glEnable")) && ok;
		ok = !!(gl.end = GETFN_(proto::EndFn, "glEnd")) && ok;
		ok = !!(gl.endList = GETFN_(proto::EndListFn, "glEndList")) && ok;
		ok = !!(gl.evalCoord1d = GETFN_(proto::EvalCoord1dFn, "glEvalCoord1d")) && ok;
		ok = !!(gl.evalCoord1dv = GETFN_(proto::EvalCoord1dvFn, "glEvalCoord1dv")) && ok;
		ok = !!(gl.evalCoord1f = GETFN_(proto::EvalCoord1fFn, "glEvalCoord1f")) && ok;
		ok = !!(gl.evalCoord1fv = GETFN_(proto::EvalCoord1fvFn, "glEvalCoord1fv")) && ok;
		ok = !!(gl.evalCoord2d = GETFN_(proto::EvalCoord2dFn, "glEvalCoord2d")) && ok;
		ok = !!(gl.evalCoord2dv = GETFN_(proto::EvalCoord2dvFn, "glEvalCoord2dv")) && ok;
		ok = !!(gl.evalCoord2f = GETFN_(proto::EvalCoord2fFn, "glEvalCoord2f")) && ok;
		ok = !!(gl.evalCoord2fv = GETFN_(proto::EvalCoord2fvFn, "glEvalCoord2fv")) && ok;
		ok = !!(gl.evalMesh1 = GETFN_(proto::EvalMesh1Fn, "glEvalMesh1")) && ok;
		ok = !!(gl.evalMesh2 = GETFN_(proto::EvalMesh2Fn, "glEvalMesh2")) && ok;
		ok = !!(gl.evalPoint1 = GETFN_(proto::EvalPoint1Fn, "glEvalPoint1")) && ok;
		ok = !!(gl.evalPoint2 = GETFN_(proto::EvalPoint2Fn, "glEvalPoint2")) && ok;
		ok = !!(gl.feedbackBuffer = GETFN_(proto::FeedbackBufferFn, "glFeedbackBuffer")) && ok;
		ok = !!(gl.finish = GETFN_(proto::FinishFn, "glFinish")) && ok;
		ok = !!(gl.flush = GETFN_(proto::FlushFn, "glFlush")) && ok;
		ok = !!(gl.fogf = GETFN_(proto::FogfFn, "glFogf")) && ok;
		ok = !!(gl.fogfv = GETFN_(proto::FogfvFn, "glFogfv")) && ok;
		ok = !!(gl.fogi = GETFN_(proto::FogiFn, "glFogi")) && ok;
		ok = !!(gl.fogiv = GETFN_(proto::FogivFn, "glFogiv")) && ok;
		ok = !!(gl.frontFace = GETFN_(proto::FrontFaceFn, "glFrontFace")) && ok;
		ok = !!(gl.frustum = GETFN_(proto::FrustumFn, "glFrustum")) && ok;
		ok = !!(gl.genLists = GETFN_(proto::GenListsFn, "glGenLists")) && ok;
		ok = !!(gl.getBooleanv = GETFN_(proto::GetBooleanvFn, "glGetBooleanv")) && ok;
		ok = !!(gl.getClipPlane = GETFN_(proto::GetClipPlaneFn, "glGetClipPlane")) && ok;
		ok = !!(gl.getDoublev = GETFN_(proto::GetDoublevFn, "glGetDoublev")) && ok;
		ok = !!(gl.getError = GETFN_(proto::GetErrorFn, "glGetError")) && ok;
		ok = !!(gl.getFloatv = GETFN_(proto::GetFloatvFn, "glGetFloatv")) && ok;
		ok = !!(gl.getIntegerv = GETFN_(proto::GetIntegervFn, "glGetIntegerv")) && ok;
		ok = !!(gl.getLightfv = GETFN_(proto::GetLightfvFn, "glGetLightfv")) && ok;
		ok = !!(gl.getLightiv = GETFN_(proto::GetLightivFn, "glGetLightiv")) && ok;
		ok = !!(gl.getMapdv = GETFN_(proto::GetMapdvFn, "glGetMapdv")) && ok;
		ok = !!(gl.getMapfv = GETFN_(proto::GetMapfvFn, "glGetMapfv")) && ok;
		ok = !!(gl.getMapiv = GETFN_(proto::GetMapivFn, "glGetMapiv")) && ok;
		ok = !!(gl.getMaterialfv = GETFN_(proto::GetMaterialfvFn, "glGetMaterialfv")) && ok;
		ok = !!(gl.getMaterialiv = GETFN_(proto::GetMaterialivFn, "glGetMaterialiv")) && ok;
		ok = !!(gl.getPixelMapfv = GETFN_(proto::GetPixelMapfvFn, "glGetPixelMapfv")) && ok;
		ok = !!(gl.getPixelMapuiv = GETFN_(proto::GetPixelMapuivFn, "glGetPixelMapuiv")) && ok;
		ok = !!(gl.getPixelMapusv = GETFN_(proto::GetPixelMapusvFn, "glGetPixelMapusv")) && ok;
		ok = !!(gl.getPolygonStipple = GETFN_(proto::GetPolygonStippleFn, "glGetPolygonStipple")) && ok;
		ok = !!(gl.getString = GETFN_(proto::GetStringFn, "glGetString")) && ok;
		ok = !!(gl.getTexEnvfv = GETFN_(proto::GetTexEnvfvFn, "glGetTexEnvfv")) && ok;
		ok = !!(gl.getTexEnviv = GETFN_(proto::GetTexEnvivFn, "glGetTexEnviv")) && ok;
		ok = !!(gl.getTexGendv = GETFN_(proto::GetTexGendvFn, "glGetTexGendv")) && ok;
		ok = !!(gl.getTexGenfv = GETFN_(proto::GetTexGenfvFn, "glGetTexGenfv")) && ok;
		ok = !!(gl.getTexGeniv = GETFN_(proto::GetTexGenivFn, "glGetTexGeniv")) && ok;
		ok = !!(gl.getTexImage = GETFN_(proto::GetTexImageFn, "glGetTexImage")) && ok;
		ok = !!(gl.getTexLevelParameterfv = GETFN_(proto::GetTexLevelParameterfvFn, "glGetTexLevelParameterfv")) && ok;
		ok = !!(gl.getTexLevelParameteriv = GETFN_(proto::GetTexLevelParameterivFn, "glGetTexLevelParameteriv")) && ok;
		ok = !!(gl.getTexParameterfv = GETFN_(proto::GetTexParameterfvFn, "glGetTexParameterfv")) && ok;
		ok = !!(gl.getTexParameteriv = GETFN_(proto::GetTexParameterivFn, "glGetTexParameteriv")) && ok;
		ok = !!(gl.hint = GETFN_(proto::HintFn, "glHint")) && ok;
		ok = !!(gl.indexMask = GETFN_(proto::IndexMaskFn, "glIndexMask")) && ok;
		ok = !!(gl.indexd = GETFN_(proto::IndexdFn, "glIndexd")) && ok;
		ok = !!(gl.indexdv = GETFN_(proto::IndexdvFn, "glIndexdv")) && ok;
		ok = !!(gl.indexf = GETFN_(proto::IndexfFn, "glIndexf")) && ok;
		ok = !!(gl.indexfv = GETFN_(proto::IndexfvFn, "glIndexfv")) && ok;
		ok = !!(gl.indexi = GETFN_(proto::IndexiFn, "glIndexi")) && ok;
		ok = !!(gl.indexiv = GETFN_(proto::IndexivFn, "glIndexiv")) && ok;
		ok = !!(gl.indexs = GETFN_(proto::IndexsFn, "glIndexs")) && ok;
		ok = !!(gl.indexsv = GETFN_(proto::IndexsvFn, "glIndexsv")) && ok;
		ok = !!(gl.initNames = GETFN_(proto::InitNamesFn, "glInitNames")) && ok;
		ok = !!(gl.isEnabled = GETFN_(proto::IsEnabledFn, "glIsEnabled")) && ok;
		ok = !!(gl.isList = GETFN_(proto::IsListFn, "glIsList")) && ok;
		ok = !!(gl.lightModelf = GETFN_(proto::LightModelfFn, "glLightModelf")) && ok;
		ok = !!(gl.lightModelfv = GETFN_(proto::LightModelfvFn, "glLightModelfv")) && ok;
		ok = !!(gl.lightModeli = GETFN_(proto::LightModeliFn, "glLightModeli")) && ok;
		ok = !!(gl.lightModeliv = GETFN_(proto::LightModelivFn, "glLightModeliv")) && ok;
		ok = !!(gl.lightf = GETFN_(proto::LightfFn, "glLightf")) && ok;
		ok = !!(gl.lightfv = GETFN_(proto::LightfvFn, "glLightfv")) && ok;
		ok = !!(gl.lighti = GETFN_(proto::LightiFn, "glLighti")) && ok;
		ok = !!(gl.lightiv = GETFN_(proto::LightivFn, "glLightiv")) && ok;
		ok = !!(gl.lineStipple = GETFN_(proto::LineStippleFn, "glLineStipple")) && ok;
		ok = !!(gl.lineWidth = GETFN_(proto::LineWidthFn, "glLineWidth")) && ok;
		ok = !!(gl.listBase = GETFN_(proto::ListBaseFn, "glListBase")) && ok;
		ok = !!(gl.loadIdentity = GETFN_(proto::LoadIdentityFn, "glLoadIdentity")) && ok;
		ok = !!(gl.loadMatrixd = GETFN_(proto::LoadMatrixdFn, "glLoadMatrixd")) && ok;
		ok = !!(gl.loadMatrixf = GETFN_(proto::LoadMatrixfFn, "glLoadMatrixf")) && ok;
		ok = !!(gl.loadName = GETFN_(proto::LoadNameFn, "glLoadName")) && ok;
		ok = !!(gl.logicOp = GETFN_(proto::LogicOpFn, "glLogicOp")) && ok;
		ok = !!(gl.map1d = GETFN_(proto::Map1dFn, "glMap1d")) && ok;
		ok = !!(gl.map1f = GETFN_(proto::Map1fFn, "glMap1f")) && ok;
		ok = !!(gl.map2d = GETFN_(proto::Map2dFn, "glMap2d")) && ok;
		ok = !!(gl.map2f = GETFN_(proto::Map2fFn, "glMap2f")) && ok;
		ok = !!(gl.mapGrid1d = GETFN_(proto::MapGrid1dFn, "glMapGrid1d")) && ok;
		ok = !!(gl.mapGrid1f = GETFN_(proto::MapGrid1fFn, "glMapGrid1f")) && ok;
		ok = !!(gl.mapGrid2d = GETFN_(proto::MapGrid2dFn, "glMapGrid2d")) && ok;
		ok = !!(gl.mapGrid2f = GETFN_(proto::MapGrid2fFn, "glMapGrid2f")) && ok;
		ok = !!(gl.materialf = GETFN_(proto::MaterialfFn, "glMaterialf")) && ok;
		ok = !!(gl.materialfv = GETFN_(proto::MaterialfvFn, "glMaterialfv")) && ok;
		ok = !!(gl.materiali = GETFN_(proto::MaterialiFn, "glMateriali")) && ok;
		ok = !!(gl.materialiv = GETFN_(proto::MaterialivFn, "glMaterialiv")) && ok;
		ok = !!(gl.matrixMode = GETFN_(proto::MatrixModeFn, "glMatrixMode")) && ok;
		ok = !!(gl.multMatrixd = GETFN_(proto::MultMatrixdFn, "glMultMatrixd")) && ok;
		ok = !!(gl.multMatrixf = GETFN_(proto::MultMatrixfFn, "glMultMatrixf")) && ok;
		ok = !!(gl.newList = GETFN_(proto::NewListFn, "glNewList")) && ok;
		ok = !!(gl.normal3b = GETFN_(proto::Normal3bFn, "glNormal3b")) && ok;
		ok = !!(gl.normal3bv = GETFN_(proto::Normal3bvFn, "glNormal3bv")) && ok;
		ok = !!(gl.normal3d = GETFN_(proto::Normal3dFn, "glNormal3d")) && ok;
		ok = !!(gl.normal3dv = GETFN_(proto::Normal3dvFn, "glNormal3dv")) && ok;
		ok = !!(gl.normal3f = GETFN_(proto::Normal3fFn, "glNormal3f")) && ok;
		ok = !!(gl.normal3fv = GETFN_(proto::Normal3fvFn, "glNormal3fv")) && ok;
		ok = !!(gl.normal3i = GETFN_(proto::Normal3iFn, "glNormal3i")) && ok;
		ok = !!(gl.normal3iv = GETFN_(proto::Normal3ivFn, "glNormal3iv")) && ok;
		ok = !!(gl.normal3s = GETFN_(proto::Normal3sFn, "glNormal3s")) && ok;
		ok = !!(gl.normal3sv = GETFN_(proto::Normal3svFn, "glNormal3sv")) && ok;
		ok = !!(gl.ortho = GETFN_(proto::OrthoFn, "glOrtho")) && ok;
		ok = !!(gl.passThrough = GETFN_(proto::PassThroughFn, "glPassThrough")) && ok;
		ok = !!(gl.pixelMapfv = GETFN_(proto::PixelMapfvFn, "glPixelMapfv")) && ok;
		ok = !!(gl.pixelMapuiv = GETFN_(proto::PixelMapuivFn, "glPixelMapuiv")) && ok;
		ok = !!(gl.pixelMapusv = GETFN_(proto::PixelMapusvFn, "glPixelMapusv")) && ok;
		ok = !!(gl.pixelStoref = GETFN_(proto::PixelStorefFn, "glPixelStoref")) && ok;
		ok = !!(gl.pixelStorei = GETFN_(proto::PixelStoreiFn, "glPixelStorei")) && ok;
		ok = !!(gl.pixelTransferf = GETFN_(proto::PixelTransferfFn, "glPixelTransferf")) && ok;
		ok = !!(gl.pixelTransferi = GETFN_(proto::PixelTransferiFn, "glPixelTransferi")) && ok;
		ok = !!(gl.pixelZoom = GETFN_(proto::PixelZoomFn, "glPixelZoom")) && ok;
		ok = !!(gl.pointSize = GETFN_(proto::PointSizeFn, "glPointSize")) && ok;
		ok = !!(gl.polygonMode = GETFN_(proto::PolygonModeFn, "glPolygonMode")) && ok;
		ok = !!(gl.polygonStipple = GETFN_(proto::PolygonStippleFn, "glPolygonStipple")) && ok;
		ok = !!(gl.popAttrib = GETFN_(proto::PopAttribFn, "glPopAttrib")) && ok;
		ok = !!(gl.popMatrix = GETFN_(proto::PopMatrixFn, "glPopMatrix")) && ok;
		ok = !!(gl.popName = GETFN_(proto::PopNameFn, "glPopName")) && ok;
		ok = !!(gl.pushAttrib = GETFN_(proto::PushAttribFn, "glPushAttrib")) && ok;
		ok = !!(gl.pushMatrix = GETFN_(proto::PushMatrixFn, "glPushMatrix")) && ok;
		ok = !!(gl.pushName = GETFN_(proto::PushNameFn, "glPushName")) && ok;
		ok = !!(gl.rasterPos2d = GETFN_(proto::RasterPos2dFn, "glRasterPos2d")) && ok;
		ok = !!(gl.rasterPos2dv = GETFN_(proto::RasterPos2dvFn, "glRasterPos2dv")) && ok;
		ok = !!(gl.rasterPos2f = GETFN_(proto::RasterPos2fFn, "glRasterPos2f")) && ok;
		ok = !!(gl.rasterPos2fv = GETFN_(proto::RasterPos2fvFn, "glRasterPos2fv")) && ok;
		ok = !!(gl.rasterPos2i = GETFN_(proto::RasterPos2iFn, "glRasterPos2i")) && ok;
		ok = !!(gl.rasterPos2iv = GETFN_(proto::RasterPos2ivFn, "glRasterPos2iv")) && ok;
		ok = !!(gl.rasterPos2s = GETFN_(proto::RasterPos2sFn, "glRasterPos2s")) && ok;
		ok = !!(gl.rasterPos2sv = GETFN_(proto::RasterPos2svFn, "glRasterPos2sv")) && ok;
		ok = !!(gl.rasterPos3d = GETFN_(proto::RasterPos3dFn, "glRasterPos3d")) && ok;
		ok = !!(gl.rasterPos3dv = GETFN_(proto::RasterPos3dvFn, "glRasterPos3dv")) && ok;
		ok = !!(gl.rasterPos3f = GETFN_(proto::RasterPos3fFn, "glRasterPos3f")) && ok;
		ok = !!(gl.rasterPos3fv = GETFN_(proto::RasterPos3fvFn, "glRasterPos3fv")) && ok;
		ok = !!(gl.rasterPos3i = GETFN_(proto::RasterPos3iFn, "glRasterPos3i")) && ok;
		ok = !!(gl.rasterPos3iv = GETFN_(proto::RasterPos3ivFn, "glRasterPos3iv")) && ok;
		ok = !!(gl.rasterPos3s = GETFN_(proto::RasterPos3sFn, "glRasterPos3s")) && ok;
		ok = !!(gl.rasterPos3sv = GETFN_(proto::RasterPos3svFn, "glRasterPos3sv")) && ok;
		ok = !!(gl.rasterPos4d = GETFN_(proto::RasterPos4dFn, "glRasterPos4d")) && ok;
		ok = !!(gl.rasterPos4dv = GETFN_(proto::RasterPos4dvFn, "glRasterPos4dv")) && ok;
		ok = !!(gl.rasterPos4f = GETFN_(proto::RasterPos4fFn, "glRasterPos4f")) && ok;
		ok = !!(gl.rasterPos4fv = GETFN_(proto::RasterPos4fvFn, "glRasterPos4fv")) && ok;
		ok = !!(gl.rasterPos4i = GETFN_(proto::RasterPos4iFn, "glRasterPos4i")) && ok;
		ok = !!(gl.rasterPos4iv = GETFN_(proto::RasterPos4ivFn, "glRasterPos4iv")) && ok;
		ok = !!(gl.rasterPos4s = GETFN_(proto::RasterPos4sFn, "glRasterPos4s")) && ok;
		ok = !!(gl.rasterPos4sv = GETFN_(proto::RasterPos4svFn, "glRasterPos4sv")) && ok;
		ok = !!(gl.readBuffer = GETFN_(proto::ReadBufferFn, "glReadBuffer")) && ok;
		ok = !!(gl.readPixels = GETFN_(proto::ReadPixelsFn, "glReadPixels")) && ok;
		ok = !!(gl.rectd = GETFN_(proto::RectdFn, "glRectd")) && ok;
		ok = !!(gl.rectdv = GETFN_(proto::RectdvFn, "glRectdv")) && ok;
		ok = !!(gl.rectf = GETFN_(proto::RectfFn, "glRectf")) && ok;
		ok = !!(gl.rectfv = GETFN_(proto::RectfvFn, "glRectfv")) && ok;
		ok = !!(gl.recti = GETFN_(proto::RectiFn, "glRecti")) && ok;
		ok = !!(gl.rectiv = GETFN_(proto::RectivFn, "glRectiv")) && ok;
		ok = !!(gl.rects = GETFN_(proto::RectsFn, "glRects")) && ok;
		ok = !!(gl.rectsv = GETFN_(proto::RectsvFn, "glRectsv")) && ok;
		ok = !!(gl.renderMode = GETFN_(proto::RenderModeFn, "glRenderMode")) && ok;
		ok = !!(gl.rotated = GETFN_(proto::RotatedFn, "glRotated")) && ok;
		ok = !!(gl.rotatef = GETFN_(proto::RotatefFn, "glRotatef")) && ok;
		ok = !!(gl.scaled = GETFN_(proto::ScaledFn, "glScaled")) && ok;
		ok = !!(gl.scalef = GETFN_(proto::ScalefFn, "glScalef")) && ok;
		ok = !!(gl.scissor = GETFN_(proto::ScissorFn, "glScissor")) && ok;
		ok = !!(gl.selectBuffer = GETFN_(proto::SelectBufferFn, "glSelectBuffer")) && ok;
		ok = !!(gl.shadeModel = GETFN_(proto::ShadeModelFn, "glShadeModel")) && ok;
		ok = !!(gl.stencilFunc = GETFN_(proto::StencilFuncFn, "glStencilFunc")) && ok;
		ok = !!(gl.stencilMask = GETFN_(proto::StencilMaskFn, "glStencilMask")) && ok;
		ok = !!(gl.stencilOp = GETFN_(proto::StencilOpFn, "glStencilOp")) && ok;
		ok = !!(gl.texCoord1d = GETFN_(proto::TexCoord1dFn, "glTexCoord1d")) && ok;
		ok = !!(gl.texCoord1dv = GETFN_(proto::TexCoord1dvFn, "glTexCoord1dv")) && ok;
		ok = !!(gl.texCoord1f = GETFN_(proto::TexCoord1fFn, "glTexCoord1f")) && ok;
		ok = !!(gl.texCoord1fv = GETFN_(proto::TexCoord1fvFn, "glTexCoord1fv")) && ok;
		ok = !!(gl.texCoord1i = GETFN_(proto::TexCoord1iFn, "glTexCoord1i")) && ok;
		ok = !!(gl.texCoord1iv = GETFN_(proto::TexCoord1ivFn, "glTexCoord1iv")) && ok;
		ok = !!(gl.texCoord1s = GETFN_(proto::TexCoord1sFn, "glTexCoord1s")) && ok;
		ok = !!(gl.texCoord1sv = GETFN_(proto::TexCoord1svFn, "glTexCoord1sv")) && ok;
		ok = !!(gl.texCoord2d = GETFN_(proto::TexCoord2dFn, "glTexCoord2d")) && ok;
		ok = !!(gl.texCoord2dv = GETFN_(proto::TexCoord2dvFn, "glTexCoord2dv")) && ok;
		ok = !!(gl.texCoord2f = GETFN_(proto::TexCoord2fFn, "glTexCoord2f")) && ok;
		ok = !!(gl.texCoord2fv = GETFN_(proto::TexCoord2fvFn, "glTexCoord2fv")) && ok;
		ok = !!(gl.texCoord2i = GETFN_(proto::TexCoord2iFn, "glTexCoord2i")) && ok;
		ok = !!(gl.texCoord2iv = GETFN_(proto::TexCoord2ivFn, "glTexCoord2iv")) && ok;
		ok = !!(gl.texCoord2s = GETFN_(proto::TexCoord2sFn, "glTexCoord2s")) && ok;
		ok = !!(gl.texCoord2sv = GETFN_(proto::TexCoord2svFn, "glTexCoord2sv")) && ok;
		ok = !!(gl.texCoord3d = GETFN_(proto::TexCoord3dFn, "glTexCoord3d")) && ok;
		ok = !!(gl.texCoord3dv = GETFN_(proto::TexCoord3dvFn, "glTexCoord3dv")) && ok;
		ok = !!(gl.texCoord3f = GETFN_(proto::TexCoord3fFn, "glTexCoord3f")) && ok;
		ok = !!(gl.texCoord3fv = GETFN_(proto::TexCoord3fvFn, "glTexCoord3fv")) && ok;
		ok = !!(gl.texCoord3i = GETFN_(proto::TexCoord3iFn, "glTexCoord3i")) && ok;
		ok = !!(gl.texCoord3iv = GETFN_(proto::TexCoord3ivFn, "glTexCoord3iv")) && ok;
		ok = !!(gl.texCoord3s = GETFN_(proto::TexCoord3sFn, "glTexCoord3s")) && ok;
		ok = !!(gl.texCoord3sv = GETFN_(proto::TexCoord3svFn, "glTexCoord3sv")) && ok;
		ok = !!(gl.texCoord4d = GETFN_(proto::TexCoord4dFn, "glTexCoord4d")) && ok;
		ok = !!(gl.texCoord4dv = GETFN_(proto::TexCoord4dvFn, "glTexCoord4dv")) && ok;
		ok = !!(gl.texCoord4f = GETFN_(proto::TexCoord4fFn, "glTexCoord4f")) && ok;
		ok = !!(gl.texCoord4fv = GETFN_(proto::TexCoord4fvFn, "glTexCoord4fv")) && ok;
		ok = !!(gl.texCoord4i = GETFN_(proto::TexCoord4iFn, "glTexCoord4i")) && ok;
		ok = !!(gl.texCoord4iv = GETFN_(proto::TexCoord4ivFn, "glTexCoord4iv")) && ok;
		ok = !!(gl.texCoord4s = GETFN_(proto::TexCoord4sFn, "glTexCoord4s")) && ok;
		ok = !!(gl.texCoord4sv = GETFN_(proto::TexCoord4svFn, "glTexCoord4sv")) && ok;
		ok = !!(gl.texEnvf = GETFN_(proto::TexEnvfFn, "glTexEnvf")) && ok;
		ok = !!(gl.texEnvfv = GETFN_(proto::TexEnvfvFn, "glTexEnvfv")) && ok;
		ok = !!(gl.texEnvi = GETFN_(proto::TexEnviFn, "glTexEnvi")) && ok;
		ok = !!(gl.texEnviv = GETFN_(proto::TexEnvivFn, "glTexEnviv")) && ok;
		ok = !!(gl.texGend = GETFN_(proto::TexGendFn, "glTexGend")) && ok;
		ok = !!(gl.texGendv = GETFN_(proto::TexGendvFn, "glTexGendv")) && ok;
		ok = !!(gl.texGenf = GETFN_(proto::TexGenfFn, "glTexGenf")) && ok;
		ok = !!(gl.texGenfv = GETFN_(proto::TexGenfvFn, "glTexGenfv")) && ok;
		ok = !!(gl.texGeni = GETFN_(proto::TexGeniFn, "glTexGeni")) && ok;
		ok = !!(gl.texGeniv = GETFN_(proto::TexGenivFn, "glTexGeniv")) && ok;
		ok = !!(gl.texImage1D = GETFN_(proto::TexImage1DFn, "glTexImage1D")) && ok;
		ok = !!(gl.texImage2D = GETFN_(proto::TexImage2DFn, "glTexImage2D")) && ok;
		ok = !!(gl.texParameterf = GETFN_(proto::TexParameterfFn, "glTexParameterf")) && ok;
		ok = !!(gl.texParameterfv = GETFN_(proto::TexParameterfvFn, "glTexParameterfv")) && ok;
		ok = !!(gl.texParameteri = GETFN_(proto::TexParameteriFn, "glTexParameteri")) && ok;
		ok = !!(gl.texParameteriv = GETFN_(proto::TexParameterivFn, "glTexParameteriv")) && ok;
		ok = !!(gl.translated = GETFN_(proto::TranslatedFn, "glTranslated")) && ok;
		ok = !!(gl.translatef = GETFN_(proto::TranslatefFn, "glTranslatef")) && ok;
		ok = !!(gl.vertex2d = GETFN_(proto::Vertex2dFn, "glVertex2d")) && ok;
		ok = !!(gl.vertex2dv = GETFN_(proto::Vertex2dvFn, "glVertex2dv")) && ok;
		ok = !!(gl.vertex2f = GETFN_(proto::Vertex2fFn, "glVertex2f")) && ok;
		ok = !!(gl.vertex2fv = GETFN_(proto::Vertex2fvFn, "glVertex2fv")) && ok;
		ok = !!(gl.vertex2i = GETFN_(proto::Vertex2iFn, "glVertex2i")) && ok;
		ok = !!(gl.vertex2iv = GETFN_(proto::Vertex2ivFn, "glVertex2iv")) && ok;
		ok = !!(gl.vertex2s = GETFN_(proto::Vertex2sFn, "glVertex2s")) && ok;
		ok = !!(gl.vertex2sv = GETFN_(proto::Vertex2svFn, "glVertex2sv")) && ok;
		ok = !!(gl.vertex3d = GETFN_(proto::Vertex3dFn, "glVertex3d")) && ok;
		ok = !!(gl.vertex3dv = GETFN_(proto::Vertex3dvFn, "glVertex3dv")) && ok;
		ok = !!(gl.vertex3f = GETFN_(proto::Vertex3fFn, "glVertex3f")) && ok;
		ok = !!(gl.vertex3fv = GETFN_(proto::Vertex3fvFn, "glVertex3fv")) && ok;
		ok = !!(gl.vertex3i = GETFN_(proto::Vertex3iFn, "glVertex3i")) && ok;
		ok = !!(gl.vertex3iv = GETFN_(proto::Vertex3ivFn, "glVertex3iv")) && ok;
		ok = !!(gl.vertex3s = GETFN_(proto::Vertex3sFn, "glVertex3s")) && ok;
		ok = !!(gl.vertex3sv = GETFN_(proto::Vertex3svFn, "glVertex3sv")) && ok;
		ok = !!(gl.vertex4d = GETFN_(proto::Vertex4dFn, "glVertex4d")) && ok;
		ok = !!(gl.vertex4dv = GETFN_(proto::Vertex4dvFn, "glVertex4dv")) && ok;
		ok = !!(gl.vertex4f = GETFN_(proto::Vertex4fFn, "glVertex4f")) && ok;
		ok = !!(gl.vertex4fv = GETFN_(proto::Vertex4fvFn, "glVertex4fv")) && ok;
		ok = !!(gl.vertex4i = GETFN_(proto::Vertex4iFn, "glVertex4i")) && ok;
		ok = !!(gl.vertex4iv = GETFN_(proto::Vertex4ivFn, "glVertex4iv")) && ok;
		ok = !!(gl.vertex4s = GETFN_(proto::Vertex4sFn, "glVertex4s")) && ok;
		ok = !!(gl.vertex4sv = GETFN_(proto::Vertex4svFn, "glVertex4sv")) && ok;
		ok = !!(gl.viewport = GETFN_(proto::ViewportFn, "glViewport")) && ok;
		gl.VERSION_1_0 = ok;
		if( ok ) {
			gl.VERSION_loaded_major = 1;
			gl.VERSION_loaded_minor = 0;
		}
	}
	//- end: Core 1.0

	gl.VERSION_1_0_compatibility = gl.VERSION_1_0;

	//-//-- 
	//- Core 1.1
	if( arg.major > 1 || (arg.major == 1 && arg.minor >= 1 ) )
	{
		bool ok = true;

		ok = !!(gl.areTexturesResident = GETFN_(proto::AreTexturesResidentFn, "glAreTexturesResident")) && ok;
		ok = !!(gl.arrayElement = GETFN_(proto::ArrayElementFn, "glArrayElement")) && ok;
		ok = !!(gl.bindTexture = GETFN_(proto::BindTextureFn, "glBindTexture")) && ok;
		ok = !!(gl.colorPointer = GETFN_(proto::ColorPointerFn, "glColorPointer")) && ok;
		ok = !!(gl.copyTexImage1D = GETFN_(proto::CopyTexImage1DFn, "glCopyTexImage1D")) && ok;
		ok = !!(gl.copyTexImage2D = GETFN_(proto::CopyTexImage2DFn, "glCopyTexImage2D")) && ok;
		ok = !!(gl.copyTexSubImage1D = GETFN_(proto::CopyTexSubImage1DFn, "glCopyTexSubImage1D")) && ok;
		ok = !!(gl.copyTexSubImage2D = GETFN_(proto::CopyTexSubImage2DFn, "glCopyTexSubImage2D")) && ok;
		ok = !!(gl.deleteTextures = GETFN_(proto::DeleteTexturesFn, "glDeleteTextures")) && ok;
		ok = !!(gl.disableClientState = GETFN_(proto::DisableClientStateFn, "glDisableClientState")) && ok;
		ok = !!(gl.drawArrays = GETFN_(proto::DrawArraysFn, "glDrawArrays")) && ok;
		ok = !!(gl.drawElements = GETFN_(proto::DrawElementsFn, "glDrawElements")) && ok;
		ok = !!(gl.edgeFlagPointer = GETFN_(proto::EdgeFlagPointerFn, "glEdgeFlagPointer")) && ok;
		ok = !!(gl.enableClientState = GETFN_(proto::EnableClientStateFn, "glEnableClientState")) && ok;
		ok = !!(gl.genTextures = GETFN_(proto::GenTexturesFn, "glGenTextures")) && ok;
		ok = !!(gl.getPointerv = GETFN_(proto::GetPointervFn, "glGetPointerv")) && ok;
		ok = !!(gl.indexPointer = GETFN_(proto::IndexPointerFn, "glIndexPointer")) && ok;
		ok = !!(gl.indexub = GETFN_(proto::IndexubFn, "glIndexub")) && ok;
		ok = !!(gl.indexubv = GETFN_(proto::IndexubvFn, "glIndexubv")) && ok;
		ok = !!(gl.interleavedArrays = GETFN_(proto::InterleavedArraysFn, "glInterleavedArrays")) && ok;
		ok = !!(gl.isTexture = GETFN_(proto::IsTextureFn, "glIsTexture")) && ok;
		ok = !!(gl.normalPointer = GETFN_(proto::NormalPointerFn, "glNormalPointer")) && ok;
		ok = !!(gl.polygonOffset = GETFN_(proto::PolygonOffsetFn, "glPolygonOffset")) && ok;
		ok = !!(gl.popClientAttrib = GETFN_(proto::PopClientAttribFn, "glPopClientAttrib")) && ok;
		ok = !!(gl.prioritizeTextures = GETFN_(proto::PrioritizeTexturesFn, "glPrioritizeTextures")) && ok;
		ok = !!(gl.pushClientAttrib = GETFN_(proto::PushClientAttribFn, "glPushClientAttrib")) && ok;
		ok = !!(gl.texCoordPointer = GETFN_(proto::TexCoordPointerFn, "glTexCoordPointer")) && ok;
		ok = !!(gl.texSubImage1D = GETFN_(proto::TexSubImage1DFn, "glTexSubImage1D")) && ok;
		ok = !!(gl.texSubImage2D = GETFN_(proto::TexSubImage2DFn, "glTexSubImage2D")) && ok;
		ok = !!(gl.vertexPointer = GETFN_(proto::VertexPointerFn, "glVertexPointer")) && ok;
		ok = ok && gl.VERSION_1_0;
		gl.VERSION_1_1 = ok;
		if( ok ) {
			gl.VERSION_loaded_major = 1;
			gl.VERSION_loaded_minor = 1;
		}
	}
	//- end: Core 1.1

	gl.VERSION_1_1_compatibility = gl.VERSION_1_0_compatibility;

	//-//-- 
	//- Core 1.2
	if( arg.major > 1 || (arg.major == 1 && arg.minor >= 2 ) )
	{
		bool ok = true;

		ok = !!(gl.copyTexSubImage3D = GETFN_(proto::CopyTexSubImage3DFn, "glCopyTexSubImage3D")) && ok;
		ok = !!(gl.drawRangeElements = GETFN_(proto::DrawRangeElementsFn, "glDrawRangeElements")) && ok;
		ok = !!(gl.texImage3D = GETFN_(proto::TexImage3DFn, "glTexImage3D")) && ok;
		ok = !!(gl.texSubImage3D = GETFN_(proto::TexSubImage3DFn, "glTexSubImage3D")) && ok;
		ok = ok && gl.VERSION_1_1;
		gl.VERSION_1_2 = ok;
		if( ok ) {
			gl.VERSION_loaded_major = 1;
			gl.VERSION_loaded_minor = 2;
		}
	}
	//- end: Core 1.2

	gl.VERSION_1_2_compatibility = gl.VERSION_1_1_compatibility;

	//-//-- 
	//- Core 1.3
	if( arg.major > 1 || (arg.major == 1 && arg.minor >= 3 ) )
	{
		bool ok = true;

		ok = !!(gl.activeTexture = GETFN_(proto::ActiveTextureFn, "glActiveTexture")) && ok;
		ok = !!(gl.clientActiveTexture = GETFN_(proto::ClientActiveTextureFn, "glClientActiveTexture")) && ok;
		ok = !!(gl.compressedTexImage1D = GETFN_(proto::CompressedTexImage1DFn, "glCompressedTexImage1D")) && ok;
		ok = !!(gl.compressedTexImage2D = GETFN_(proto::CompressedTexImage2DFn, "glCompressedTexImage2D")) && ok;
		ok = !!(gl.compressedTexImage3D = GETFN_(proto::CompressedTexImage3DFn, "glCompressedTexImage3D")) && ok;
		ok = !!(gl.compressedTexSubImage1D = GETFN_(proto::CompressedTexSubImage1DFn, "glCompressedTexSubImage1D")) && ok;
		ok = !!(gl.compressedTexSubImage2D = GETFN_(proto::CompressedTexSubImage2DFn, "glCompressedTexSubImage2D")) && ok;
		ok = !!(gl.compressedTexSubImage3D = GETFN_(proto::CompressedTexSubImage3DFn, "glCompressedTexSubImage3D")) && ok;
		ok = !!(gl.getCompressedTexImage = GETFN_(proto::GetCompressedTexImageFn, "glGetCompressedTexImage")) && ok;
		ok = !!(gl.loadTransposeMatrixd = GETFN_(proto::LoadTransposeMatrixdFn, "glLoadTransposeMatrixd")) && ok;
		ok = !!(gl.loadTransposeMatrixf = GETFN_(proto::LoadTransposeMatrixfFn, "glLoadTransposeMatrixf")) && ok;
		ok = !!(gl.multTransposeMatrixd = GETFN_(proto::MultTransposeMatrixdFn, "glMultTransposeMatrixd")) && ok;
		ok = !!(gl.multTransposeMatrixf = GETFN_(proto::MultTransposeMatrixfFn, "glMultTransposeMatrixf")) && ok;
		ok = !!(gl.multiTexCoord1d = GETFN_(proto::MultiTexCoord1dFn, "glMultiTexCoord1d")) && ok;
		ok = !!(gl.multiTexCoord1dv = GETFN_(proto::MultiTexCoord1dvFn, "glMultiTexCoord1dv")) && ok;
		ok = !!(gl.multiTexCoord1f = GETFN_(proto::MultiTexCoord1fFn, "glMultiTexCoord1f")) && ok;
		ok = !!(gl.multiTexCoord1fv = GETFN_(proto::MultiTexCoord1fvFn, "glMultiTexCoord1fv")) && ok;
		ok = !!(gl.multiTexCoord1i = GETFN_(proto::MultiTexCoord1iFn, "glMultiTexCoord1i")) && ok;
		ok = !!(gl.multiTexCoord1iv = GETFN_(proto::MultiTexCoord1ivFn, "glMultiTexCoord1iv")) && ok;
		ok = !!(gl.multiTexCoord1s = GETFN_(proto::MultiTexCoord1sFn, "glMultiTexCoord1s")) && ok;
		ok = !!(gl.multiTexCoord1sv = GETFN_(proto::MultiTexCoord1svFn, "glMultiTexCoord1sv")) && ok;
		ok = !!(gl.multiTexCoord2d = GETFN_(proto::MultiTexCoord2dFn, "glMultiTexCoord2d")) && ok;
		ok = !!(gl.multiTexCoord2dv = GETFN_(proto::MultiTexCoord2dvFn, "glMultiTexCoord2dv")) && ok;
		ok = !!(gl.multiTexCoord2f = GETFN_(proto::MultiTexCoord2fFn, "glMultiTexCoord2f")) && ok;
		ok = !!(gl.multiTexCoord2fv = GETFN_(proto::MultiTexCoord2fvFn, "glMultiTexCoord2fv")) && ok;
		ok = !!(gl.multiTexCoord2i = GETFN_(proto::MultiTexCoord2iFn, "glMultiTexCoord2i")) && ok;
		ok = !!(gl.multiTexCoord2iv = GETFN_(proto::MultiTexCoord2ivFn, "glMultiTexCoord2iv")) && ok;
		ok = !!(gl.multiTexCoord2s = GETFN_(proto::MultiTexCoord2sFn, "glMultiTexCoord2s")) && ok;
		ok = !!(gl.multiTexCoord2sv = GETFN_(proto::MultiTexCoord2svFn, "glMultiTexCoord2sv")) && ok;
		ok = !!(gl.multiTexCoord3d = GETFN_(proto::MultiTexCoord3dFn, "glMultiTexCoord3d")) && ok;
		ok = !!(gl.multiTexCoord3dv = GETFN_(proto::MultiTexCoord3dvFn, "glMultiTexCoord3dv")) && ok;
		ok = !!(gl.multiTexCoord3f = GETFN_(proto::MultiTexCoord3fFn, "glMultiTexCoord3f")) && ok;
		ok = !!(gl.multiTexCoord3fv = GETFN_(proto::MultiTexCoord3fvFn, "glMultiTexCoord3fv")) && ok;
		ok = !!(gl.multiTexCoord3i = GETFN_(proto::MultiTexCoord3iFn, "glMultiTexCoord3i")) && ok;
		ok = !!(gl.multiTexCoord3iv = GETFN_(proto::MultiTexCoord3ivFn, "glMultiTexCoord3iv")) && ok;
		ok = !!(gl.multiTexCoord3s = GETFN_(proto::MultiTexCoord3sFn, "glMultiTexCoord3s")) && ok;
		ok = !!(gl.multiTexCoord3sv = GETFN_(proto::MultiTexCoord3svFn, "glMultiTexCoord3sv")) && ok;
		ok = !!(gl.multiTexCoord4d = GETFN_(proto::MultiTexCoord4dFn, "glMultiTexCoord4d")) && ok;
		ok = !!(gl.multiTexCoord4dv = GETFN_(proto::MultiTexCoord4dvFn, "glMultiTexCoord4dv")) && ok;
		ok = !!(gl.multiTexCoord4f = GETFN_(proto::MultiTexCoord4fFn, "glMultiTexCoord4f")) && ok;
		ok = !!(gl.multiTexCoord4fv = GETFN_(proto::MultiTexCoord4fvFn, "glMultiTexCoord4fv")) && ok;
		ok = !!(gl.multiTexCoord4i = GETFN_(proto::MultiTexCoord4iFn, "glMultiTexCoord4i")) && ok;
		ok = !!(gl.multiTexCoord4iv = GETFN_(proto::MultiTexCoord4ivFn, "glMultiTexCoord4iv")) && ok;
		ok = !!(gl.multiTexCoord4s = GETFN_(proto::MultiTexCoord4sFn, "glMultiTexCoord4s")) && ok;
		ok = !!(gl.multiTexCoord4sv = GETFN_(proto::MultiTexCoord4svFn, "glMultiTexCoord4sv")) && ok;
		ok = !!(gl.sampleCoverage = GETFN_(proto::SampleCoverageFn, "glSampleCoverage")) && ok;
		ok = ok && gl.VERSION_1_2;
		gl.VERSION_1_3 = ok;
		if( ok ) {
			gl.VERSION_loaded_major = 1;
			gl.VERSION_loaded_minor = 3;
		}
	}
	//- end: Core 1.3

	gl.VERSION_1_3_compatibility = gl.VERSION_1_2_compatibility;

	//-//-- 
	//- Core 1.4
	if( arg.major > 1 || (arg.major == 1 && arg.minor >= 4 ) )
	{
		bool ok = true;

		ok = !!(gl.blendColor = GETFN_(proto::BlendColorFn, "glBlendColor")) && ok;
		ok = !!(gl.blendEquation = GETFN_(proto::BlendEquationFn, "glBlendEquation")) && ok;
		ok = !!(gl.blendFuncSeparate = GETFN_(proto::BlendFuncSeparateFn, "glBlendFuncSeparate")) && ok;
		ok = !!(gl.fogCoordPointer = GETFN_(proto::FogCoordPointerFn, "glFogCoordPointer")) && ok;
		ok = !!(gl.fogCoordd = GETFN_(proto::FogCoorddFn, "glFogCoordd")) && ok;
		ok = !!(gl.fogCoorddv = GETFN_(proto::FogCoorddvFn, "glFogCoorddv")) && ok;
		ok = !!(gl.fogCoordf = GETFN_(proto::FogCoordfFn, "glFogCoordf")) && ok;
		ok = !!(gl.fogCoordfv = GETFN_(proto::FogCoordfvFn, "glFogCoordfv")) && ok;
		ok = !!(gl.multiDrawArrays = GETFN_(proto::MultiDrawArraysFn, "glMultiDrawArrays")) && ok;
		ok = !!(gl.multiDrawElements = GETFN_(proto::MultiDrawElementsFn, "glMultiDrawElements")) && ok;
		ok = !!(gl.pointParameterf = GETFN_(proto::PointParameterfFn, "glPointParameterf")) && ok;
		ok = !!(gl.pointParameterfv = GETFN_(proto::PointParameterfvFn, "glPointParameterfv")) && ok;
		ok = !!(gl.pointParameteri = GETFN_(proto::PointParameteriFn, "glPointParameteri")) && ok;
		ok = !!(gl.pointParameteriv = GETFN_(proto::PointParameterivFn, "glPointParameteriv")) && ok;
		ok = !!(gl.secondaryColor3b = GETFN_(proto::SecondaryColor3bFn, "glSecondaryColor3b")) && ok;
		ok = !!(gl.secondaryColor3bv = GETFN_(proto::SecondaryColor3bvFn, "glSecondaryColor3bv")) && ok;
		ok = !!(gl.secondaryColor3d = GETFN_(proto::SecondaryColor3dFn, "glSecondaryColor3d")) && ok;
		ok = !!(gl.secondaryColor3dv = GETFN_(proto::SecondaryColor3dvFn, "glSecondaryColor3dv")) && ok;
		ok = !!(gl.secondaryColor3f = GETFN_(proto::SecondaryColor3fFn, "glSecondaryColor3f")) && ok;
		ok = !!(gl.secondaryColor3fv = GETFN_(proto::SecondaryColor3fvFn, "glSecondaryColor3fv")) && ok;
		ok = !!(gl.secondaryColor3i = GETFN_(proto::SecondaryColor3iFn, "glSecondaryColor3i")) && ok;
		ok = !!(gl.secondaryColor3iv = GETFN_(proto::SecondaryColor3ivFn, "glSecondaryColor3iv")) && ok;
		ok = !!(gl.secondaryColor3s = GETFN_(proto::SecondaryColor3sFn, "glSecondaryColor3s")) && ok;
		ok = !!(gl.secondaryColor3sv = GETFN_(proto::SecondaryColor3svFn, "glSecondaryColor3sv")) && ok;
		ok = !!(gl.secondaryColor3ub = GETFN_(proto::SecondaryColor3ubFn, "glSecondaryColor3ub")) && ok;
		ok = !!(gl.secondaryColor3ubv = GETFN_(proto::SecondaryColor3ubvFn, "glSecondaryColor3ubv")) && ok;
		ok = !!(gl.secondaryColor3ui = GETFN_(proto::SecondaryColor3uiFn, "glSecondaryColor3ui")) && ok;
		ok = !!(gl.secondaryColor3uiv = GETFN_(proto::SecondaryColor3uivFn, "glSecondaryColor3uiv")) && ok;
		ok = !!(gl.secondaryColor3us = GETFN_(proto::SecondaryColor3usFn, "glSecondaryColor3us")) && ok;
		ok = !!(gl.secondaryColor3usv = GETFN_(proto::SecondaryColor3usvFn, "glSecondaryColor3usv")) && ok;
		ok = !!(gl.secondaryColorPointer = GETFN_(proto::SecondaryColorPointerFn, "glSecondaryColorPointer")) && ok;
		ok = !!(gl.windowPos2d = GETFN_(proto::WindowPos2dFn, "glWindowPos2d")) && ok;
		ok = !!(gl.windowPos2dv = GETFN_(proto::WindowPos2dvFn, "glWindowPos2dv")) && ok;
		ok = !!(gl.windowPos2f = GETFN_(proto::WindowPos2fFn, "glWindowPos2f")) && ok;
		ok = !!(gl.windowPos2fv = GETFN_(proto::WindowPos2fvFn, "glWindowPos2fv")) && ok;
		ok = !!(gl.windowPos2i = GETFN_(proto::WindowPos2iFn, "glWindowPos2i")) && ok;
		ok = !!(gl.windowPos2iv = GETFN_(proto::WindowPos2ivFn, "glWindowPos2iv")) && ok;
		ok = !!(gl.windowPos2s = GETFN_(proto::WindowPos2sFn, "glWindowPos2s")) && ok;
		ok = !!(gl.windowPos2sv = GETFN_(proto::WindowPos2svFn, "glWindowPos2sv")) && ok;
		ok = !!(gl.windowPos3d = GETFN_(proto::WindowPos3dFn, "glWindowPos3d")) && ok;
		ok = !!(gl.windowPos3dv = GETFN_(proto::WindowPos3dvFn, "glWindowPos3dv")) && ok;
		ok = !!(gl.windowPos3f = GETFN_(proto::WindowPos3fFn, "glWindowPos3f")) && ok;
		ok = !!(gl.windowPos3fv = GETFN_(proto::WindowPos3fvFn, "glWindowPos3fv")) && ok;
		ok = !!(gl.windowPos3i = GETFN_(proto::WindowPos3iFn, "glWindowPos3i")) && ok;
		ok = !!(gl.windowPos3iv = GETFN_(proto::WindowPos3ivFn, "glWindowPos3iv")) && ok;
		ok = !!(gl.windowPos3s = GETFN_(proto::WindowPos3sFn, "glWindowPos3s")) && ok;
		ok = !!(gl.windowPos3sv = GETFN_(proto::WindowPos3svFn, "glWindowPos3sv")) && ok;
		ok = ok && gl.VERSION_1_3;
		gl.VERSION_1_4 = ok;
		if( ok ) {
			gl.VERSION_loaded_major = 1;
			gl.VERSION_loaded_minor = 4;
		}
	}
	//- end: Core 1.4

	gl.VERSION_1_4_compatibility = gl.VERSION_1_3_compatibility;

	//-//-- 
	//- Core 1.5
	if( arg.major > 1 || (arg.major == 1 && arg.minor >= 5 ) )
	{
		bool ok = true;

		ok = !!(gl.beginQuery = GETFN_(proto::BeginQueryFn, "glBeginQuery")) && ok;
		ok = !!(gl.bindBuffer = GETFN_(proto::BindBufferFn, "glBindBuffer")) && ok;
		ok = !!(gl.bufferData = GETFN_(proto::BufferDataFn, "glBufferData")) && ok;
		ok = !!(gl.bufferSubData = GETFN_(proto::BufferSubDataFn, "glBufferSubData")) && ok;
		ok = !!(gl.deleteBuffers = GETFN_(proto::DeleteBuffersFn, "glDeleteBuffers")) && ok;
		ok = !!(gl.deleteQueries = GETFN_(proto::DeleteQueriesFn, "glDeleteQueries")) && ok;
		ok = !!(gl.endQuery = GETFN_(proto::EndQueryFn, "glEndQuery")) && ok;
		ok = !!(gl.genBuffers = GETFN_(proto::GenBuffersFn, "glGenBuffers")) && ok;
		ok = !!(gl.genQueries = GETFN_(proto::GenQueriesFn, "glGenQueries")) && ok;
		ok = !!(gl.getBufferParameteriv = GETFN_(proto::GetBufferParameterivFn, "glGetBufferParameteriv")) && ok;
		ok = !!(gl.getBufferPointerv = GETFN_(proto::GetBufferPointervFn, "glGetBufferPointerv")) && ok;
		ok = !!(gl.getBufferSubData = GETFN_(proto::GetBufferSubDataFn, "glGetBufferSubData")) && ok;
		ok = !!(gl.getQueryObjectiv = GETFN_(proto::GetQueryObjectivFn, "glGetQueryObjectiv")) && ok;
		ok = !!(gl.getQueryObjectuiv = GETFN_(proto::GetQueryObjectuivFn, "glGetQueryObjectuiv")) && ok;
		ok = !!(gl.getQueryiv = GETFN_(proto::GetQueryivFn, "glGetQueryiv")) && ok;
		ok = !!(gl.isBuffer = GETFN_(proto::IsBufferFn, "glIsBuffer")) && ok;
		ok = !!(gl.isQuery = GETFN_(proto::IsQueryFn, "glIsQuery")) && ok;
		ok = !!(gl.mapBuffer = GETFN_(proto::MapBufferFn, "glMapBuffer")) && ok;
		ok = !!(gl.unmapBuffer = GETFN_(proto::UnmapBufferFn, "glUnmapBuffer")) && ok;
		ok = ok && gl.VERSION_1_4;
		gl.VERSION_1_5 = ok;
		if( ok ) {
			gl.VERSION_loaded_major = 1;
			gl.VERSION_loaded_minor = 5;
		}
	}
	//- end: Core 1.5

	gl.VERSION_1_5_compatibility = gl.VERSION_1_4_compatibility;

}

#undef GETFN_
//--    <<< ~ dlapi::gl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE3(dlapi,gl,detail)
//--//////////////////////////////////////////////////////////////////////////
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
