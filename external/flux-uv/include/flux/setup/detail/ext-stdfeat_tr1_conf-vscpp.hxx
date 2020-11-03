/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	TR1 library features from language level
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_STDFEAT_TR1_CONF_VSCPP_HXX_FC443141_FBAB_450E_902B_5903B23240BF
#define DETAIL_EXT_STDFEAT_TR1_CONF_VSCPP_HXX_FC443141_FBAB_450E_902B_5903B23240BF

//--//////////////////////////////////////////////////////////////////////////
//--    TR1                         ///{{{1///////////////////////////////////

#if defined(_MSVC_LANG) && _MSVC_LANG > 201402
/* 2017.3 + /std:c++latest doesn't have the TR1 stuff anymore, apparently.
 */

#else // !_MSVC_LANG
#	if _MSC_VER >= 1500 // Visual Studio 2008
#		undef FLUX_STDFEAT_TR1_FUNCTIONAL
#		undef FLUX_STDFEAT_TR1_TYPE_TRAITS
#		undef FLUX_STDFEAT_TR1_RANDOM
//#		undef FLUX_STDFEAT_TR1_EXTRAMATH // Various C99 functions missing
#		undef FLUX_STDFEAT_TR1_TUPLE
#		undef FLUX_STDFEAT_TR1_UNORDERED
#		undef FLUX_STDFEAT_TR1_REGEX
#		undef FLUX_STDFEAT_TR1_SHAREDPTR

#		define FLUX_STDFEAT_TR1_FUNCTIONAL      1
#		define FLUX_STDFEAT_TR1_TYPE_TRAITS     1
#		define FLUX_STDFEAT_TR1_RANDOM          1
//#		define FLUX_STDFEAT_TR1_EXTRAMATH       1
#		define FLUX_STDFEAT_TR1_TUPLE           1
#		define FLUX_STDFEAT_TR1_UNORDERED       1
#		define FLUX_STDFEAT_TR1_REGEX           1
#		define FLUX_STDFEAT_TR1_SHAREDPTR       1
#	endif // ~ Visual Studio 2008

#	if _MSC_VER >= 1900 // Visual Studio 2015
#		undef FLUX_STDQUIRK_TR1_NO_ADD_REFERENCE
#		define FLUX_STDQUIRK_TR1_NO_ADD_REFERENCE 1
#	endif // ~ Visual Studio 2015
#endif // ~ _MSVC_LANG

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_STDFEAT_TR1_CONF_VSCPP_HXX_FC443141_FBAB_450E_902B_5903B23240BF
