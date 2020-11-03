/*-******************************************************* -- HEADER -{{{1- */
/*-	GL errors
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_GL_ERRORS_HPP_A32BE3A3_5506_4240_AEDB_CBC44D566796
#define FLUX_GL_ERRORS_HPP_A32BE3A3_5506_4240_AEDB_CBC44D566796

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/std/stdexcept.hpp>

//--    >>> namespace = flux::gl::error >>>         ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(gl,error)
//--    errors - generic                    ///{{{2///////////////////////////

struct OpenGLError 
	: flux::error::RuntimeError
{};

//--    errors - opengl                     ///{{{2///////////////////////////

/* Errors produced by the OpenGL API */
	
	struct GLCheckpoint
		: OpenGLError
	{};
	struct GLShaderCompileFailed
		: OpenGLError
	{};
	//struct GLFramebufferIncomplete
	//	: OpenGLError
	//{};

//--    errors - context                    ///{{{2///////////////////////////

/* Errors related to handling of the OpenGL context and API */

	struct ContextError
		: OpenGLError
	{};

	struct GLApiError
		: OpenGLError
	{};

		struct NoGLApiBound
			: GLApiError
		{};

		struct InsufficientVersion
			: GLApiError
		{};
		struct MissingExtension
			: GLApiError
		{};

//--    <<< ~ flux::gl::error namespace <<<         ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(gl,error)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_GL_ERRORS_HPP_A32BE3A3_5506_4240_AEDB_CBC44D566796
