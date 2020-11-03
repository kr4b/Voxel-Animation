/*-******************************************************* -- HEADER -{{{1- */
/*-	OpenGL std::error_category
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_GL_ERROR_CATEGORY_HPP_ED8815CD_BEAA_4021_A71E_C35E3D0E5D44
#define FLUX_GL_ERROR_CATEGORY_HPP_ED8815CD_BEAA_4021_A71E_C35E3D0E5D44

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/system_error.hpp>

//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    functions                           ///{{{2///////////////////////////

compat::error_code make_gl_error( int ) noexcept;
compat::error_category const& gl_error_category() noexcept;

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_GL_ERROR_CATEGORY_HPP_ED8815CD_BEAA_4021_A71E_C35E3D0E5D44
