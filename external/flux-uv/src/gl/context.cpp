/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Context implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/gl/context.hpp>

#include <flux/util/scope_exit.hpp>

#include <flux_dlapi/os/glx.hpp>
#include <flux_dlapi/os/wgl.hpp>
#include <flux_dlapi/os/win32.hpp>

#include "context_glx.hxx"
#include "context_wgl.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	unique_ptr<Context> try_from_glx_();
	unique_ptr<Context> try_from_wgl_();
}

//--    Context                             ///{{{2///////////////////////////

Context::~Context() = default;

//--    create_from_native_current()        ///{{{2///////////////////////////
unique_ptr<Context> create_from_native_current( bool aBindToCat, bool aBindApi )
{
	unique_ptr<Context> ret;

	if( !ret ) ret = try_from_glx_();
	if( !ret ) ret = try_from_wgl_();

	if( ret )
	{
		if( aBindToCat ) base::thread_catalog().configure<ThreadContext>( ret.get() );
		if( aBindApi ) set_gl_api( ret->api(), ret.get() );
	}
	
	return ret;
}

//--    $ try_from_glx_()                   ///{{{2///////////////////////////
namespace
{
	unique_ptr<Context> try_from_glx_()
	{
		if( !dlapi::os::acquire_x11() )
			return nullptr;

		auto const* x11 = dlapi::os::x11();
		FLUX_ASSERT( x11 );

		FLUX_UTIL_ON_SCOPE_EXIT { dlapi::os::release_x11(); };

		auto const* glx = dlapi::os::load_glx();
		if( !glx )
			return nullptr;

		FLUX_UTIL_ON_SCOPE_EXIT { unload( glx ); };

		auto ctx = glx->getCurrentContext();
		auto dpy = glx->getCurrentDisplay();
		auto draw = glx->getCurrentDrawable();
		
		if( !ctx || !dpy || (dlapi::os::X11::None == draw) )
			return nullptr;
		
		return make_unique<detail::CapturedGLXContext>( x11, glx, ctx, draw, dpy );
	}
}

//--    $ try_from_wgl_()                   ///{{{2///////////////////////////
namespace
{
	unique_ptr<Context> try_from_wgl_()
	{
		if( !dlapi::os::acquire_win32() )
			return nullptr;

		auto const* win32 = dlapi::os::win32();
		FLUX_ASSERT( win32 );

		FLUX_UTIL_ON_SCOPE_EXIT { dlapi::os::release_win32(); };

		auto const* wgl = dlapi::os::load_wgl();
		if( !wgl )
			return nullptr;

		FLUX_UTIL_ON_SCOPE_EXIT { unload( wgl ); };

		auto dc = wgl->getCurrentDC();
		auto glrc = wgl->getCurrentContext();
		
		if( !dc || !glrc )
			return nullptr;
		
		return make_unique<detail::CapturedWGLContext>( win32, wgl, dc, glrc );
	}
}

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
