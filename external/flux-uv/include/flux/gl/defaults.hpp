/*-******************************************************* -- HEADER -{{{1- */
/*-	flux.gl configuration
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_GL_DEFAULTS_HPP_4481B1B7_0C16_422C_85F5_343970B1C6F8
#define FLUX_GL_DEFAULTS_HPP_4481B1B7_0C16_422C_85F5_343970B1C6F8

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

//--    defaults                    ///{{{1///////////////////////////////////

/* Config: enable flux-io based helpers
 *
 * Controls whether helper functions that use flux-io are present. For example,
 * when enabled, GLSL and Spir-V shaders can be directly loaded using the VFS
 * from flux-io. 
 */
#if !defined(FLUXCFG_GL_WITH_FLUXIO)
#	define FLUXCFG_GL_WITH_FLUXIO FLUX_WITH_FLUXIO
#endif // ~ CFG_GL_WITH_FLUXIO

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_GL_DEFAULTS_HPP_4481B1B7_0C16_422C_85F5_343970B1C6F8
