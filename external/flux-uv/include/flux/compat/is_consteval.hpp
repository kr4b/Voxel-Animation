/*-******************************************************* -- HEADER -{{{1- */
/*-	is_constant_evaluated()
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_IS_CONSTEVAL_HPP_50DAD9BE_5ECD_4FCC_9760_904DD26286E5
#define FLUX_COMPAT_IS_CONSTEVAL_HPP_50DAD9BE_5ECD_4FCC_9760_904DD26286E5

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>


//--    clamp()                     ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX20_IS_CONSTEVAL
#	include <type_traits>

FLUX_NAMESPACE_ENTER1(compat)
	using std::is_constant_evaluated;
FLUX_NAMESPACE_LEAVE1(compat)

#else // fallback
FLUX_NAMESPACE_ENTER1(compat)
	constexpr
	bool is_constant_evaluated() noexcept
	{
		return false;
	}
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_IS_CONSTEVAL_HPP_50DAD9BE_5ECD_4FCC_9760_904DD26286E5
