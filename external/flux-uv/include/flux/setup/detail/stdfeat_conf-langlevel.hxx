/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Standard library features from language level
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_STDFEAT_CONF_LANGLEVEL_HXX_0E828E4D_9A07_41AA_8C5B_B26EC105E4A8
#define DETAIL_STDFEAT_CONF_LANGLEVEL_HXX_0E828E4D_9A07_41AA_8C5B_B26EC105E4A8

//--//////////////////////////////////////////////////////////////////////////
//--    C++11                       ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX11
#	undef FLUX_STDFEAT_CXX11_SYSERROR
#	define FLUX_STDEFAT_CXX11_SYSERROR         1
#endif // ~ LANGLEVEL_CXX11

//--    C++14                       ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX14
#	undef FLUX_STDFEAT_CXX14_MAKE_UNIQUE
#	undef FLUX_STDFEAT_CXX14_TUPLES_BY_TYPE
#	undef FLUX_STDFEAT_CXX14_TYPE_TRAITS
#	undef FLUX_STDFEAT_CXX14_INTEGER_SEQ
#	undef FLUX_STDFEAT_CXX14_EXCHANGE

#	define FLUX_STDFEAT_CXX14_MAKE_UNIQUE      1
#	define FLUX_STDFEAT_CXX14_TUPLES_BY_TYPE   1
#	define FLUX_STDFEAT_CXX14_TYPE_TRAITS      1
#	define FLUX_STDFEAT_CXX14_INTEGER_SEQ      1
#	define FLUX_STDFEAT_CXX14_EXCHANGE         1
#endif // ~ LANGLEVEL_CXX14

//--    C++17                       ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX17
#	undef FLUX_STDFEAT_CXX17_BYTE
#	undef FLUX_STDFEAT_CXX17_CLAMP
#	undef FLUX_STDFEAT_CXX17_VOIDT
#	undef FLUX_STDFEAT_CXX17_OPTIONAL
#	undef FLUX_STDFEAT_CXX17_LAUNDER
#	undef FLUX_STDFEAT_CXX17_FILESYSTEM
#	undef FLUX_STDFEAT_CXX17_TYPE_TRAITS
#	undef FLUX_STDFEAT_CXX17_ITERATOR_TRAITS
#	undef FLUX_STDFEAT_CXX17_UNINITIALIZED
#	undef FLUX_STDFEAT_CXX17_STRING_VIEW
#	undef FLUX_STDFEAT_CXX17_INVOKE
#	undef FLUX_STDFEAT_CXX17_VARIANT
#	undef FLUX_STDFEAT_CXX17_UNCAUGHT_EXCEPTIONS
#	undef FLUX_STDFEAT_CXX17_SHARED_MUTEX

#	define FLUX_STDFEAT_CXX17_BYTE             1
#	define FLUX_STDFEAT_CXX17_CLAMP            1
#	define FLUX_STDFEAT_CXX17_VOIDT            1
#	define FLUX_STDFEAT_CXX17_OPTIONAL         1
#	define FLUX_STDFEAT_CXX17_LAUNDER          1
#	define FLUX_STDFEAT_CXX17_FILESYSTEM       1
#	define FLUX_STDFEAT_CXX17_TYPE_TRAITS      1
#	define FLUX_STDFEAT_CXX17_ITERATOR_TRAITS  1
#	define FLUX_STDFEAT_CXX17_UNINITIALIZED    1
#	define FLUX_STDFEAT_CXX17_STRING_VIEW      1
#	define FLUX_STDFEAT_CXX17_INVOKE           1
#	define FLUX_STDFEAT_CXX17_VARIANT          1
#	define FLUX_STDFEAT_CXX17_UNCAUGHT_EXCEPTIONS  1
#	define FLUX_STDFEAT_CXX17_SHARED_MUTEX     1
#endif // ~ LANGLEVEL_CXX17

//--    C++20                       ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL_CXX >= FLUX_LANGLEVEL_CXX20
#	undef FLUX_STDFEAT_CXX20_SPAN
#	undef FLUX_STDFEAT_CXX20_BIT_CAST
#	undef FLUX_STDFEAT_CXX20_TO_ADDRESS
#	undef FLUX_STDFEAT_CXX20_IS_CONSTEVAL
#	undef FLUX_STDFEAT_CXX20_SOURCE_LOCATION
#	undef FLUX_STDFEAT_CXX20_BARRIER

#	define FLUX_STDFEAT_CXX20_SPAN             1
#	define FLUX_STDFEAT_CXX20_BIT_CAST         1
#	define FLUX_STDFEAT_CXX20_TO_ADDRESS       1
#	define FLUX_STDFEAT_CXX20_IS_CONSTEVAL     1
#	define FLUX_STDFEAT_CXX20_SOURCE_LOCATION  1
#	define FLUX_STDFEAT_CXX20_BARRIER          1
#endif // ~ LANGLEVEL_CXX20

//--    experimental features       ///{{{1///////////////////////////////////

/* Experimental features are not bound to any language level */

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_STDFEAT_CONF_LANGLEVEL_HXX_0E828E4D_9A07_41AA_8C5B_B26EC105E4A8
