/*-******************************************************* -- HEADER -{{{1- */
/*- localso.{so,dll}: for testing dynamic loading
 *
 * This loads a specific localso.so.0.0 or localso-0.0.dll. The purpose is to
 * facilitate tests of "local" distribution via `/@flux/opt/system/bindist`.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_OS_LOCALSO_HPP_7A3738B5_F234_4BDD_B9C1_482533DCE4D5
#define FLUXDLAPI_OS_LOCALSO_HPP_7A3738B5_F234_4BDD_B9C1_482533DCE4D5

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/sys/dylib.hpp>
#include <flux/std/identity.hpp>
#include <flux/std/inttypes.hpp>

//--    >>> namespace = dlapi::os >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    types                           ///{{{2///////////////////////////////
namespace LocalSO
{
	using uint64_t = flux::uint64_t;
}

//--    constants                       ///{{{2///////////////////////////////
namespace LocalSO
{
	// (none)
}

//--    prot                            ///{{{2///////////////////////////////
namespace LocalSOproto
{
	using LocalfuncFn = LocalSO::uint64_t (*)(LocalSO::uint64_t);
}

//--    LocalSOapi                       ///{{{2///////////////////////////////
struct LocalSOapi
{
	bool valid;

	LocalSOproto::LocalfuncFn localfunc;

	std::size_t referenceCount_;
	sys::DylibHandle libHandle_; //TODO: Hide?
};

//--    functions                       ///{{{2///////////////////////////////

LocalSOapi const* localso( bool aAutoload = true ) noexcept;

bool acquire_localso( char const* = nullptr );
bool acquire_localso(
	void* (*aGetFn)( char const* aFnName, void* aUser ),
	void* aUser
);

void release_localso();


namespace ex
{
	LocalSOapi const* load( Identity<LocalSOapi>, char const* = nullptr );
	LocalSOapi const* load(
		 Identity<LocalSOapi>,
		void* (*aGetFn)( char const* aFnName, void* aUser ),
		void* aUser
	);

	void unload( LocalSOapi const* );

	void acquire( LocalSOapi const* );
	void release( LocalSOapi const* );
}

//--    <<< ~ dlapi::os namespace <<<          ///{{{1////////////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_OS_LOCALSO_HPP_7A3738B5_F234_4BDD_B9C1_482533DCE4D5
