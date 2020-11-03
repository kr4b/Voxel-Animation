/*-******************************************************* -- HEADER -{{{1- */
/*- testso.{so,dll}: for testing dynamic loading
 *
 * This loads a specific testso.so.0.0 or testso-0.0.dll. The purpose is mainly
 * to facilitate tests of the autofetching capabilities.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_OS_TESTSO_HPP_9E49F97E_80DA_4A8F_A4B4_DC192401971C
#define FLUXDLAPI_OS_TESTSO_HPP_9E49F97E_80DA_4A8F_A4B4_DC192401971C

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>


#include <flux/sys/dylib.hpp>
#include <flux/std/identity.hpp>
#include <flux/std/inttypes.hpp>

//--    >>> namespace = dlapi::os >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    types                           ///{{{2///////////////////////////////
namespace TestSO
{
	using uint64_t = flux::uint64_t;
}

//--    constants                       ///{{{2///////////////////////////////
namespace TestSO
{
	// (none)
}

//--    prot                            ///{{{2///////////////////////////////
namespace TestSOproto
{
	using TestfuncFn = TestSO::uint64_t (*)(TestSO::uint64_t);
}

//--    TestSOapi                       ///{{{2///////////////////////////////
struct TestSOapi
{
	bool valid;

	TestSOproto::TestfuncFn testfunc;

	std::size_t referenceCount_;
	sys::DylibHandle libHandle_; //TODO: Hide?
};

//--    functions                       ///{{{2///////////////////////////////

TestSOapi const* testso( bool aAutoload = true ) noexcept;

bool acquire_testso( char const* = nullptr );
bool acquire_testso(
	void* (*aGetFn)( char const* aFnName, void* aUser ),
	void* aUser
);

void release_testso();


namespace ex
{
	TestSOapi const* load( Identity<TestSOapi>, char const* = nullptr );
	TestSOapi const* load(
		 Identity<TestSOapi>,
		void* (*aGetFn)( char const* aFnName, void* aUser ),
		void* aUser
	);

	void unload( TestSOapi const* );

	void acquire( TestSOapi const* );
	void release( TestSOapi const* );
}

//--    <<< ~ dlapi::os namespace <<<          ///{{{1////////////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_OS_TESTSO_HPP_9E49F97E_80DA_4A8F_A4B4_DC192401971C
