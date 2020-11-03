/*-******************************************************* -- HEADER -{{{1- */
/*-	std::void_t<>
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_VOIDT_HPP_536D42DF_7960_4ED8_A6AC_E00855D6A4E7
#define FLUX_COMPAT_VOIDT_HPP_536D42DF_7960_4ED8_A6AC_E00855D6A4E7

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>

//--    void_t<>                   ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX17_VOIDT
#	include <type_traits>

FLUX_NAMESPACE_ENTER1(compat)
	using std::void_t;
FLUX_NAMESPACE_LEAVE1(compat)

#else // fallback
FLUX_NAMESPACE_ENTER1(compat)
	//template< class... > using void_t = void;
	
	// According to cppreference, the following should be more safe with 
	// various compilers under C++14 and before CWG1558 (where unused
	// template parameters were not guaranteed to trigger SFINAE).
	namespace detail
	{
		template< class... tArgs > 
		struct VoidTImpl 
		{ 
			using type = void; 
		};
	}

	template< class... tArgs > 
	using void_t = typename detail::VoidTImpl<tArgs...>::type;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_VOIDT_HPP_536D42DF_7960_4ED8_A6AC_E00855D6A4E7
