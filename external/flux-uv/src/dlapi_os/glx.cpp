/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	GLX api loader implementation
 * 
 */
/*-***************************************************************** -}}}1- */

#include <flux_dlapi/os/glx.hpp>

#include <flux/std/assert.hpp>

#include <flux/sys/dylib.hpp>

#include <cstring>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = dlapi::os >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
	// Only on Linux ATM.
	constexpr char const* kDefaultGLXLibName = "libGL.so";

	GLXapi* load_api_( void* (*)(char const*,void*), void* );

	// Warning: moderate evil ahead (but we have to cast things either way).
	struct FnPtrProxy_
	{
		inline FnPtrProxy_( void* aPtr )
			: mPtr(aPtr)
		{}

		template< typename tType > inline
		operator tType*() const {
			return reinterpret_cast<tType*>(mPtr);
		}

		void* mPtr;
	};
}

//--    unload()                        ///{{{2///////////////////////////////
void unload( GLXapi const* aGLX )
{
	GLXapi* mptr = const_cast<GLXapi*>(aGLX);
	FLUX_ASSERT( mptr );

	mptr->valid = false;
	
	release( aGLX );
}

//--    acquire()                       ///{{{2///////////////////////////////
void acquire( GLXapi const* aGLX )
{
	FLUX_ASSERT( aGLX );
	++const_cast<GLXapi*>(aGLX)->referenceCount_;
}

//--    release()                       ///{{{2///////////////////////////////
void release( GLXapi const* aGLX )
{
	FLUX_ASSERT( aGLX );
	if( 0 == --const_cast<GLXapi*>(aGLX)->referenceCount_ )
	{
		if( aGLX->libHandle_ )
			sys::dylib_close( aGLX->libHandle_ );

		delete aGLX;
	}
}

//--    load_glx()                      ///{{{2///////////////////////////////
GLXapi const* load_glx( void* (*aGetFn)(char const*,void*), void* aUser )
{
	return load_api_( aGetFn, aUser );
}

GLXapi const* load_glx( char const* aDlibName )
{
	if( !aDlibName ) aDlibName = kDefaultGLXLibName;
	if( auto const handle = sys::dylib_open( aDlibName ) )
	{
		using FnType_ = void* (*)(char const*, void*);
		FnType_ const get = [] (char const* aName, void* aHandle) {
			auto const handle = static_cast<sys::DylibHandle>(aHandle);
			return (void*)sys::dylib_get_proc( handle, aName );
		};

		if( auto const ret = load_api_( get, handle ) )
		{
			ret->libHandle_ = handle;
			return ret;
		}

		sys::dylib_close( handle );
	}

	return nullptr;
}

//--    load_glx_extensions()           ///{{{2///////////////////////////////
bool load_glx_extensions( GLXapi const* aGLX )
{
	FLUX_ASSERT( aGLX && aGLX->getProcAddress );

	namespace GLX_ = dlapi::os::GLX;
	namespace X11_ = dlapi::os::X11;

	// Get the GLX extensions string (this requires some additional gymnastics)
	GLX_::Context ctx;
	if( !(ctx = aGLX->getCurrentContext()) )
		return false;

	X11::Display* dpy;
	if( !(dpy = aGLX->getCurrentDisplay()) )
		return false;

	int screen = 0;
	if( 0 != aGLX->queryContext( dpy, ctx, GLX_::SCREEN, &screen ) )
		return false;

	char const* const exts = aGLX->queryExtensionsString( dpy, screen );
	if( !exts )
		return false; // No extensions supported...

	// try to load the extensions
	auto check = [&] (char const* aExtName) {
		if( char const* pp = std::strstr( exts, aExtName ) )
		{
			auto const len = std::strlen(aExtName);
			return ' ' == pp[len] || '\0' == pp[len];
		}
		return false;
	};
	auto load = [&] (char const* aFnName) {
		return FnPtrProxy_(reinterpret_cast<void*>(aGLX->getProcAddress( reinterpret_cast<GLX::GLubyte const*>(aFnName) )));
	};

	bool any = false;
	auto* glx = const_cast<GLXapi*>(aGLX);
	
	if( check( "GLX_ARB_create_context" ) )
	{
		bool ok = true;
		if( !(glx->createContextAttribsARB = load( "glXCreateContextAttribsARB" )) )
			ok = false;
			
		if( ok ) 
		{
			any = true;
			glx->ARB_create_context = true;
		}
	}

	return any;
}

//--    $ load_api_()                   ///{{{2///////////////////////////////
namespace
{
	GLXapi* load_api_( void* (*aGetFn)(char const*,void*), void* aUser )
	{
		FLUX_ASSERT( aGetFn );
		auto load = [&] (char const* aName) {
			return FnPtrProxy_(aGetFn( aName, aUser ));
		};

		GLXapi* glx = new GLXapi();
		glx->libHandle_ = nullptr;

		bool ok = true;

#		define LOADFN_( fld, name ) ok = !!(glx->fld = load( name )) && ok
		//XXX- some of these are GLX 1.3 (or other versions). Currently assumes that these are available.
		LOADFN_( queryVersion, "glXQueryVersion" );
		LOADFN_( queryExtensionsString, "glXQueryExtensionsString" );
		LOADFN_( isDirect, "glXIsDirect" );
		LOADFN_( getProcAddress, "glXGetProcAddress" );

		LOADFN_( getCurrentContext, "glXGetCurrentContext" );
		LOADFN_( getCurrentDisplay, "glXGetCurrentDisplay" );
		LOADFN_( getCurrentDrawable, "glXGetCurrentDrawable" );
		LOADFN_( createNewContext, "glXCreateNewContext" );
		LOADFN_( destroyContext, "glXDestroyContext" );
		LOADFN_( makeCurrent, "glXMakeCurrent" );

		LOADFN_( queryContext, "glXQueryContext" );
		LOADFN_( queryDrawable, "glXQueryDrawable" );
		LOADFN_( chooseFBConfig, "glXChooseFBConfig" );
		LOADFN_( getFBConfigAttrib, "glXGetFBConfigAttrib" );

		LOADFN_( swapBuffers, "glXSwapBuffers" );
#		undef LOADFN_

		glx->ARB_create_context = false;
		glx->createContextAttribsARB = nullptr;

		// done.
		if( ok )
		{
			glx->valid = true;
			glx->referenceCount_ = 1;
			return glx;
		}

		delete glx;
		return nullptr;
	}
}

//--    <<< ~ dlapi::os namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
