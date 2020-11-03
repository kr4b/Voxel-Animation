/*-******************************************************* -- HEADER -{{{1- */
/*-	std::get<TYPE>
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_TUPLE_GET_HPP_38AE35D1_0073_4BB3_A102_25986A374164
#define FLUX_COMPAT_TUPLE_GET_HPP_38AE35D1_0073_4BB3_A102_25986A374164

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>

//--    void_t<>                   ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX14_TUPLES_BY_TYPE
#	include <tuple>

FLUX_NAMESPACE_ENTER1(compat)
	using std::get;
FLUX_NAMESPACE_LEAVE1(compat)

#else // fallback
#	include <tuple>
#	include <flux/nih_/tuple_get.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using std::get; // For normal std::get<INTEGER>().
	using nih_::get; // get<TYPE>()
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_TUPLE_GET_HPP_38AE35D1_0073_4BB3_A102_25986A374164
