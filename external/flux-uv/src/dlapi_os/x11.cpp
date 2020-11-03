/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	X11 api loader implementation
 * 
 */
/*-***************************************************************** -}}}1- */

#include <flux_dlapi/os/x11.hpp>

#include <flux/sys/dylib.hpp>

#include <cassert>

#include "load_util.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = dlapi::os >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
	// Only on Linux ATM.
	constexpr char const* kDefaultX11LibName = "libX11.so";

	detail::ApiRecord<X11api> gX11Api_;

	X11api* load_api_( void* (*)(char const*,void*), void* );
}

//--    x11()                           ///{{{2///////////////////////////////
X11api const* x11( bool aAutoload ) noexcept
{
	if( auto ptr = gX11Api_.api() )
		return ptr;

	return aAutoload ? gX11Api_.try_autoload() : nullptr;
}

//--    acquire_x11()                   ///{{{2///////////////////////////////
bool acquire_x11( char const* aDlibName )
{
	return gX11Api_.acquire( aDlibName );
}
bool acquire_x11( void* (*aGetFn)(char const*,void*), void* aUser )
{
	return gX11Api_.acquire( aGetFn, aUser );
}

//--    release_x11()                   ///{{{2///////////////////////////////
void release_x11()
{
	gX11Api_.release();
}

//--    ex::unload()                    ///{{{2///////////////////////////////
namespace ex
{
	void unload( X11api const* aX11 )
	{
		X11api* mptr = const_cast<X11api*>(aX11);
		assert( mptr );

		mptr->valid = false;
		
		release( aX11 );
	}
}

//--    ex::acquire()                   ///{{{2///////////////////////////////
namespace ex
{
	void acquire( X11api const* aX11 )
	{
		assert( aX11 );
		++const_cast<X11api*>(aX11)->referenceCount_;
	}
}

//--    ex::release()                   ///{{{2///////////////////////////////
namespace ex
{
	void release( X11api const* aX11 )
	{
		assert( aX11 );
		if( 0 == --const_cast<X11api*>(aX11)->referenceCount_ )
		{
			if( aX11->libHandle_ )
				sys::dylib_close( aX11->libHandle_ );

			delete aX11;
		}
	}
}

//--    ex::load_x11()                  ///{{{2///////////////////////////////
namespace ex
{
	X11api const* load( Identity<X11api>, void* (*aGetFn)(char const*,void*), void* aUser )
	{
		return load_api_( aGetFn, aUser );
	}

	X11api const* load( Identity<X11api>, char const* aDlibName )
	{
		if( !aDlibName )
			aDlibName = kDefaultX11LibName;

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
}

//--    $ load_api_()                   ///{{{2///////////////////////////////
namespace
{
	X11api* load_api_( void* (*aGetFn)(char const*,void*), void* aUser )
	{
		assert( aGetFn );
		auto load = [&] (char const* aName) {
			return detail::FnPtrProxy(aGetFn( aName, aUser ));
		};

		X11api* x11 = new X11api();
		x11->libHandle_ = nullptr;

		bool ok = true;

#		define LOADFN_( fld, name ) ok = !!(x11->fld = load( name )) && ok
		LOADFN_( openDisplay, "XOpenDisplay" );
		LOADFN_( closeDisplay, "XCloseDisplay" );
		LOADFN_( free, "XFree" );
#		undef LOADFN_

		// done.
		if( ok )
		{
			x11->valid = true;
			x11->referenceCount_ = 1;
			return x11;
		}

		delete x11;
		return nullptr;
	}
}

//--    <<< ~ dlapi::os namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
