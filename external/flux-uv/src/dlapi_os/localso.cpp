/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Localso api loader implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux_dlapi/os/localso.hpp>

#include <flux/sys/dylib.hpp>

#include <cassert>

#include "load_util.hxx"
#include "load_extended.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = dlapi::os >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
	detail::ApiRecord<LocalSOapi> gLocalSOApi_;

	LocalSOapi* load_api_( void* (*)(char const*,void*), void* );
}

//--    localso()                       ///{{{2///////////////////////////////
LocalSOapi const* localso( bool aAutoload ) noexcept
{
	if( auto ptr = gLocalSOApi_.api() )
		return ptr;

	return aAutoload ? gLocalSOApi_.try_autoload() : nullptr;
}

//--    acquire_localso()               ///{{{2///////////////////////////////
bool acquire_localso( char const* aDlibName )
{
	return gLocalSOApi_.acquire( aDlibName );
}
bool acquire_localso( void* (*aGetFn)(char const*,void*), void* aUser )
{
	return gLocalSOApi_.acquire( aGetFn, aUser );
}

//--    release_localso()               ///{{{2///////////////////////////////
void release_localso()
{
	return gLocalSOApi_.release();
}

//--    ex::unload()                    ///{{{2///////////////////////////////
namespace ex
{
	void unload( LocalSOapi const* aGLFW )
	{
		LocalSOapi* mptr = const_cast<LocalSOapi*>(aGLFW);
		assert( mptr );

		mptr->valid = false;

		release( aGLFW );
	}
}

//--    ex::acquire()                   ///{{{2///////////////////////////////
namespace ex
{
	void acquire( LocalSOapi const* aGLFW )
	{
		assert( aGLFW );
		++const_cast<LocalSOapi*>(aGLFW)->referenceCount_;
	}
}

//--    ex::release()                   ///{{{2///////////////////////////////
namespace ex
{
	void release( LocalSOapi const* aGLFW )
	{
		assert( aGLFW );
		if( 0 == --const_cast<LocalSOapi*>(aGLFW)->referenceCount_ )
		{
			if( aGLFW->libHandle_ )
				sys::dylib_close( aGLFW->libHandle_ );

			delete aGLFW;
		}
	}
}

//--    ex::load()                      ///{{{2///////////////////////////////
namespace ex
{
	LocalSOapi const* load( Identity<LocalSOapi>, void* (*aGetFn)(char const*,void*), void* aUser )
	{
		return load_api_( aGetFn, aUser );
	}

	LocalSOapi const* load( Identity<LocalSOapi>, char const* /*aDlibName*/ )
	{
		using detail::EExtendedOptions;

		sys::DylibHandle handle = nullptr;
#		if 0
		if( aDlibName )
		{
			handle = sys::dylib_open( aDlibName );
		}
		else
#		endif
		{
			handle = detail::extended_search(
				EExtendedOptions::cached | EExtendedOptions::bindist,
				nullptr,
				nullptr /*"localso.so.0.0"*/,
				"localso", "0.0"
			);
		}

		if( handle )
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
	LocalSOapi* load_api_( void* (*aGetFn)(char const*,void*), void* aUser )
	{
		assert( aGetFn );
		auto load = [&] (char const* aName) {
			return detail::FnPtrProxy(aGetFn( aName, aUser ));
		};

		LocalSOapi* localso = new LocalSOapi();
		localso->libHandle_ = nullptr;

		bool ok = true;

#		define LOADFN_( fld, name ) ok = !!(localso->fld = load( name )) && ok
		LOADFN_( localfunc, "localfunc" );
#		undef LOADFN_

		// done.
		if( ok )
		{
			localso->valid = true;
			localso->referenceCount_ = 1;
			return localso;
		}

		delete localso;
		return nullptr;
	}
}

//--    <<< ~ dlapi::os namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
