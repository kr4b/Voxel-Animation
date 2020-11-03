/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	TestSO api loader implementation
 *
 * Note: autofetching doesn't set the execute bit on the fetched .so. This
 * is apparently not needed under linux, but other unixes may require it.
 */
/*-***************************************************************** -}}}1- */

#include <flux_dlapi/os/testso.hpp>

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
	detail::ApiRecord<TestSOapi> gTestSOApi_;

	TestSOapi* load_api_( void* (*)(char const*,void*), void* );
}

//--    testso()                        ///{{{2///////////////////////////////
TestSOapi const* testso( bool aAutoload ) noexcept
{
	if( auto ptr = gTestSOApi_.api() )
		return ptr;

	return aAutoload ? gTestSOApi_.try_autoload() : nullptr;
}

//--    acquire_testso()                ///{{{2///////////////////////////////
bool acquire_testso( char const* aDlibName )
{
	return gTestSOApi_.acquire( aDlibName );
}
bool acquire_testso( void* (*aGetFn)(char const*,void*), void* aUser )
{
	return gTestSOApi_.acquire( aGetFn, aUser );
}

//--    release_testso()                ///{{{2///////////////////////////////
void release_testso()
{
	return gTestSOApi_.release();
}

//--    ex::unload()                    ///{{{2///////////////////////////////
namespace ex
{
	void unload( TestSOapi const* aGLFW )
	{
		TestSOapi* mptr = const_cast<TestSOapi*>(aGLFW);
		assert( mptr );

		mptr->valid = false;

		release( aGLFW );
	}
}

//--    ex::acquire()                   ///{{{2///////////////////////////////
namespace ex
{
	void acquire( TestSOapi const* aGLFW )
	{
		assert( aGLFW );
		++const_cast<TestSOapi*>(aGLFW)->referenceCount_;
	}
}

//--    ex::release()                   ///{{{2///////////////////////////////
namespace ex
{
	void release( TestSOapi const* aGLFW )
	{
		assert( aGLFW );
		if( 0 == --const_cast<TestSOapi*>(aGLFW)->referenceCount_ )
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
	TestSOapi const* load( Identity<TestSOapi>, void* (*aGetFn)(char const*,void*), void* aUser )
	{
		return load_api_( aGetFn, aUser );
	}

	TestSOapi const* load( Identity<TestSOapi>, char const* /*aDlibName*/ )
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
				EExtendedOptions::cached | EExtendedOptions::autofetch,
				nullptr,
				nullptr /*"testso.so.0.0"*/,
				"testso", "0.0"
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
	TestSOapi* load_api_( void* (*aGetFn)(char const*,void*), void* aUser )
	{
		assert( aGetFn );
		auto load = [&] (char const* aName) {
			return detail::FnPtrProxy(aGetFn( aName, aUser ));
		};

		TestSOapi* testso = new TestSOapi();
		testso->libHandle_ = nullptr;

		bool ok = true;

#		define LOADFN_( fld, name ) ok = !!(testso->fld = load( name )) && ok
		LOADFN_( testfunc, "testfunc" );
#		undef LOADFN_

		// done.
		if( ok )
		{
			testso->valid = true;
			testso->referenceCount_ = 1;
			return testso;
		}

		delete testso;
		return nullptr;
	}
}

//--    <<< ~ dlapi::os namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
