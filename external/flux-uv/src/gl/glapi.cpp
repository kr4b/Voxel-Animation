/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	GLapi implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/gl/glapi.hpp>
#include <flux_dlapi/GL/load.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    detail::ThreadGLApi                 ///{{{2///////////////////////////
namespace detail
{
	ThreadGLApi::ThreadGLApi() noexcept
		: api( nullptr )
		, key( nullptr )
	{}
	ThreadGLApi::~ThreadGLApi()
	{
		if( api ) release_gl( api );
	}

	ThreadGLApi& ThreadGLApi::instance() noexcept
	{
		thread_local ThreadGLApi tlApi;
		return tlApi;
	}
}

//--    gl_api_key()                        ///{{{2///////////////////////////
void const* gl_api_key() noexcept
{
	return detail::ThreadGLApi::instance().key;
}

//--    set_gl_api()                        ///{{{2///////////////////////////
void set_gl_api( GLapi const* aPtr, void const* aKey ) noexcept
{
	auto& inst = detail::ThreadGLApi::instance();
	inst.key = aKey;

	if( inst.api == aPtr )
		return;

	if( inst.api )
		release_gl( inst.api );

	if( (inst.api = aPtr) )
		acquire_gl( inst.api );
}

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
