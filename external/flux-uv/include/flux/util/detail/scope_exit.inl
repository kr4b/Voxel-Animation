/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- ON_SCOPE_EXIT() implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/setup.pkg>

#include <flux/pp/unique.hpp>
#include <flux/util/attrib.hpp>

#include <flux/compat/type_traits.hpp>

#include <utility>

//--//////////////////////////////////////////////////////////////////////////
//--    ON_SCOPE_EXIT()             ///{{{1///////////////////////////////////

#undef FLUX_UTIL_ON_SCOPE_EXIT
#define FLUX_UTIL_ON_SCOPE_EXIT                                             \
	auto FLUX_PP_UNIQUE_IDENT(fluxScopeExitHelper) FLUX_ATTR_UNUSED =       \
		::flux::util::detail::ScopeExitFactory{} ->* [&] ()                 \
	/*ENDM*/

//--    >>> namespace = flux::util::detail >>>      ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(util,detail)
//--    ScopeExitHelper                     ///{{{2///////////////////////////
template< class tCallable >
struct ScopeExitHelper
{
	ScopeExitHelper( tCallable&& aCallable )
		: mCallable( std::move(aCallable) )
	{}

	~ScopeExitHelper() noexcept(false)
	{
		mCallable();
	}

	private: tCallable mCallable;
};

//--    ScopeExitFactory                    ///{{{2///////////////////////////
struct ScopeExitFactory
{
	template< class tCallable >
	ScopeExitHelper<tCallable> operator ->* (tCallable&& aCallable)
	{
		return ScopeExitHelper<tCallable>{ std::move(aCallable) };
	}
};

//--    <<< ~ flux::util::detail namespace <<<      ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(util,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
