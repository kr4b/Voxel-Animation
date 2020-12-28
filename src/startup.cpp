#include "startup.hpp"

#include <flux/std/log.hpp>
#include <flux/std/assert.hpp>

#include <flux/base/def/vfs.hpp>
#include <flux/base/default_catalogs.hpp>
namespace base = flux::base;

#include <flux/gl/glapi.hpp>
#include <flux/gl/context.hpp>
#include <flux/gl/checkpoint.hpp>
namespace gl = flux::gl;

#include <flux/util/scope_exit.hpp>

#include <flux/compat/exchange.hpp>
namespace compat = flux::compat;

#include <flux_dlapi/os/glfw.hpp>
namespace GLFW = flux::dlapi::os::GLFW;


#include "defaults.hpp"


namespace
{
	void glfw_cb_error_( int, char const* );
}


bool global_startup()
{
	// Make ready some basic stuff.
	base::thread_catalog().acquire< base::def::ProcessVfs >();
	
	// Load GLFW
	if( !flux::dlapi::os::acquire_glfw() )
	{
		FLUX_LOG( FATAL, "Unable to load GLFW api." );
		return false;
	}

	return true;
}

void global_shutdown()
{
	flux::dlapi::os::release_glfw();
}


GfxContext::GfxContext( GfxContext&& aOther ) noexcept
	: win( compat::exchange( aOther.win, nullptr ) )
	, context( std::move( aOther.context ) )
{}
GfxContext& GfxContext::operator= (GfxContext&& aOther ) noexcept
{
	std::swap( win, aOther.win );
	std::swap( context, aOther.context );
	return *this;
}

bool GfxContext::valid() noexcept
{
	return !!win && !!context;
}


GfxContext gfx_startup()
{
	// Initialize GLFW and create a windpw
	auto const* glfw = flux::dlapi::os::glfw();
	glfw->setErrorCallback( glfw_cb_error_ );
	
	if( !glfw->init() )
	{
		FLUX_LOG( FATAL, "GLFW initialization failed.\n" );
		return {};
	}

	glfw->windowHint( GLFW::CONTEXT_VERSION_MAJOR, 4 );
	glfw->windowHint( GLFW::CONTEXT_VERSION_MINOR, 3 );
	glfw->windowHint( GLFW::SRGB_CAPABLE, 1 );
	glfw->windowHint( GLFW::OPENGL_DEBUG_CONTEXT, 1 );
	glfw->windowHint( GLFW::OPENGL_PROFILE, GLFW::OPENGL_CORE_PROFILE );

	auto win = glfw->createWindow( kWindowDefaults.width, kWindowDefaults.height, kWindowDefaults.name, nullptr, nullptr );
	if( !win )
	{
		FLUX_LOG( FATAL, "GLFW: no window for us\n" );
		glfw->terminate();
		return {};
	}

	FLUX_UTIL_ON_SCOPE_EXIT
	{
		if( win )
			glfw->destroyWindow( win );
	};

	glfw->setWindowUserPointer( win, nullptr ); 

	glfw->makeContextCurrent( win );
	glfw->swapInterval( 1 );

	// Initialize OpenGL
	auto ctx = [] {
		FLUX_LOG_SCOPE( INFO, "flux: abduct the GL context" );
		auto c = gl::create_from_native_current();
		{
			auto const& caps = c->caps();
			FLUX_LOG( INFO, "Context: GL: {:d}.{:d}, {}/{} [{}]", caps.major, caps.minor, caps.vendor.c_str(), caps.renderer.c_str(), caps.osapi.c_str() );
		}
		return c;
	}();

	namespace GL = gl::GL;
	auto const gl = gl::gl_api();
	FLUX_ASSERT( gl );

	// Configure OpenGL defaults
	gl->enable( GL::FRAMEBUFFER_SRGB );
	gl->clearColor( 0.0f, 0.3f, 0.3f, 0.0f );

	gl->enable(GL::DEPTH_TEST);
	gl->depthFunc(GL::LEQUAL);

	// Return context
	GfxContext ret;
	ret.win = compat::exchange( win, nullptr );
	ret.context = std::move(ctx);
	return ret;
}

void gfx_shutdown( GfxContext aContext )
{
	aContext.context.reset();
	
	if( auto const* glfw = flux::dlapi::os::glfw() )
	{
		if( aContext.win )
			glfw->destroyWindow( aContext.win );

		glfw->terminate();
	}
}

void gfx_set_callbacks( GLFW::Window* aWin, GfxCallbacks const& aCb, void* aUser )
{
	auto const* glfw = flux::dlapi::os::glfw();
	FLUX_ASSERT( glfw );
	
	glfw->setWindowUserPointer( aWin, aUser ); 
	glfw->setKeyCallback( aWin, aCb.keyCallback );
	glfw->setMouseButtonCallback( aWin, aCb.buttonCallback );
	glfw->setScrollCallback( aWin, aCb.scrollCallback );
	glfw->setCursorPosCallback( aWin, aCb.cursorCallback );
}


namespace
{
	void glfw_cb_error_( int aErrNum, char const* aDesc )
	{
		FLUX_LOG( ERROR, "GLFW: {} ({})", aDesc, aErrNum );
	}
}
