/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	WGL-WGL api loader implementation
 * 
 */
/*-***************************************************************** -}}}1- */

#include <flux_dlapi/os/wgl.hpp>

#include <flux/std/assert.hpp>
#include <flux/sys/dylib.hpp>

#include <cassert>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = dlapi::os >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
	// Only on Windows.
	constexpr char const* kDefaultWGLLibName = "opengl32.dll";

	WGLapi* load_api_( void* (*)(char const*,void*), void* );

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
void unload( WGLapi const* aWGL )
{
	WGLapi* mptr = const_cast<WGLapi*>(aWGL);
	assert( mptr );

	mptr->valid = false;
	
	release( aWGL );
}

//--    acquire()                       ///{{{2///////////////////////////////
void acquire( WGLapi const* aWGL )
{
	assert( aWGL );
	++const_cast<WGLapi*>(aWGL)->referenceCount_;
}

//--    release()                       ///{{{2///////////////////////////////
void release( WGLapi const* aWGL )
{
	assert( aWGL );
	if( 0 == --const_cast<WGLapi*>(aWGL)->referenceCount_ )
	{
		if( aWGL->libHandle_ )
			sys::dylib_close( aWGL->libHandle_ );

		delete aWGL;
	}
}

//--    load_wgl()                    ///{{{2///////////////////////////////
WGLapi const* load_wgl( void* (*aGetFn)(char const*,void*), void* aUser )
{
	return load_api_( aGetFn, aUser );
}

WGLapi const* load_wgl( char const* aDlibName )
{
	if( !aDlibName ) aDlibName = kDefaultWGLLibName;

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

//--    load_wgl_extensions()           ///{{{2///////////////////////////////
bool load_wgl_extensions( WGLapi const* aWGL )
{
	FLUX_ASSERT( aWGL && aWGL->getProcAddress );

	if( !aWGL->getCurrentContext() )
		return false;

	auto load = [&] (char const* aName) {
		return FnPtrProxy_(reinterpret_cast<void*>(aWGL->getProcAddress( aName )));
	};

	bool any = false;
	auto* wgl = const_cast<WGLapi*>(aWGL);
	
	// TODO: should we check the extension string somehow?
	if( (wgl->createContextAttribsARB = load( "wglCreateContextAttribsARB" )) )
	{
		any = true;
		wgl->ARB_create_context = true;
	}

	return any;
}

//--    $ load_api_()                   ///{{{2///////////////////////////////
namespace
{
	WGLapi* load_api_( void* (*aGetFn)(char const*,void*), void* aUser )
	{
		assert( aGetFn );
		auto load = [&] (char const* aName) {
			return FnPtrProxy_(aGetFn( aName, aUser ));
		};

		WGLapi* wgl = new WGLapi();
		wgl->libHandle_ = nullptr;

		bool ok = true;

#		define LOADFN_( fld, name ) ok = !!(wgl->fld = load( name )) && ok
		LOADFN_( getProcAddress, "wglGetProcAddress" );

		LOADFN_( getCurrentDC, "wglGetCurrentDC" );
		LOADFN_( getCurrentContext, "wglGetCurrentContext" );
		LOADFN_( createContext, "wglCreateContext" );
		LOADFN_( deleteContext, "wglDeleteContext" );
		LOADFN_( makeCurrent, "wglMakeCurrent" );
		LOADFN_( shareLists, "wglShareLists" );
#		undef LOADFN_

		// Can't load extensions at this point...
		wgl->ARB_create_context = false;
		wgl->createContextAttribsARB = nullptr;

		// done.
		if( ok )
		{
			wgl->valid = true;
			wgl->referenceCount_ = 1;
			return wgl;
		}

		delete wgl;
		return nullptr;
	}
}

//--    <<< ~ dlapi::os namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
