/*-******************************************************* -- HEADER -{{{1- */
/*- X11 api subset
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_OS_X11_HPP_8EA627EC_61CE_4384_85CB_E5ADF42A112D
#define FLUXDLAPI_OS_X11_HPP_8EA627EC_61CE_4384_85CB_E5ADF42A112D

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/sys/dylib.hpp>
#include <flux/std/identity.hpp>

#include <cstddef>

//--    >>> namespace = dlapi::os >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    types                           ///{{{2///////////////////////////////
namespace X11
{
	typedef struct DisplayOpaque_ Display;

	using Bool = int;
	using XID = unsigned long;
}

//--    constants                       ///{{{2///////////////////////////////
namespace X11
{
	constexpr long None = 0L;
}

//--    prot                            ///{{{2///////////////////////////////
namespace X11proto
{
	using OpenDisplayFn = X11::Display* (*)( char* );
	using CloseDisplayFn = int (*)( X11::Display* );

	using FreeFn = int (*)( void* );
}

//--    X11api                          ///{{{2///////////////////////////////
struct X11api
{
	bool valid;

	X11proto::OpenDisplayFn openDisplay;
	X11proto::CloseDisplayFn closeDisplay;

	X11proto::FreeFn free;

	std::size_t referenceCount_;
	sys::DylibHandle libHandle_; //TODO: Hide?
};

//--    functions                       ///{{{2///////////////////////////////

X11api const* x11( bool aAutoload = true ) noexcept;

bool acquire_x11( char const* = nullptr );
bool acquire_x11( 	
	void* (*aGetFn)( char const* aFnName, void* aUser ),
	void* aUser
);

void release_x11();

namespace ex
{
	X11api const* load( Identity<X11api>, char const* = nullptr );
	X11api const* load(
		Identity<X11api>,
		void* (*aGetFn)( char const* aFnName, void* aUser ),
		void* aUser
	);

	void unload( X11api const* );

	void acquire( X11api const* );
	void release( X11api const* );
}

//--    <<< ~ dlapi::os namespace <<<          ///{{{1////////////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_OS_X11_HPP_8EA627EC_61CE_4384_85CB_E5ADF42A112D
