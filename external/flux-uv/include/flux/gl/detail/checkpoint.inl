/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Checkpoint implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/setup/buildcfg.hpp>
#include <flux/compat/current_func.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    $ local                             ///{{{2///////////////////////////

#define FLUX_GL_CHECKPOINT_IMPL_()                                  \
	for( auto err = ::flux::gl::gl_api()->getError();               \
		::flux::gl::GL::GLNO_ERROR != err;                          \
		err = ::flux::gl::GL::GLNO_ERROR )                          \
		::flux::gl::detail::throw_gl_checkpoint_error(              \
			__FILE__,                                               \
			FLUX_COMPAT_FUNCTION_NAME,                              \
			__LINE__,                                               \
			err                                                     \
		)                                                           \
	/*ENDM*/

//--    CHECKPOINT_ALWAYS()                 ///{{{2///////////////////////////

#undef FLUX_GL_CHECKPOINT_ALWAYS
#define FLUX_GL_CHECKPOINT_ALWAYS() FLUX_GL_CHECKPOINT_IMPL_()

//--    CHECKPOINT_DEBUG()                  ///{{{2///////////////////////////

#if FLUX_DEBUG
#	undef FLUX_GL_CHECKPOINT_DEBUG
#	define FLUX_GL_CHECKPOINT_DEBUG() FLUX_GL_CHECKPOINT_IMPL_()

#else
#	undef FLUX_GL_CHECKPOINT_DEBUG
#	define FLUX_GL_CHECKPOINT_DEBUG() do {} while(false)

#endif // ~ FLUX_DEBUG

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
