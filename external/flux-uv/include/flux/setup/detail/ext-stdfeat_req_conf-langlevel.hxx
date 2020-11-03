/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	(old) standard library features from langlevel
 *
 * WARNING: untested!
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_EXT_STDFEAT_REQ_CONF_LANGLEVEL_HXX_21FF71D7_BBD7_4F68_A7B2_B053B03796EE
#define DETAIL_EXT_EXT_STDFEAT_REQ_CONF_LANGLEVEL_HXX_21FF71D7_BBD7_4F68_A7B2_B053B03796EE

//--//////////////////////////////////////////////////////////////////////////
//--    C++11                       ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX11
#	undef FLUX_STDFEAT_CXX11_TYPE_TRAITS
#	define FLUX_STDFEAT_CXX11_TYPE_TRAITS   1

#	undef FLUX_STDFEAT_CXX11_FUNCTIONAL
#	define FLUX_STDFEAT_CXX11_FUNCTIONAL    1

#	undef FLUX_STDFEAT_CXX11_TUPLE
#	define FLUX_STDFEAT_CXX11_TUPLE         1

#	undef FLUX_STDFEAT_CXX11_UNIQUEPTR
#	define FLUX_STDFEAT_CXX11_UNIQUEPTR     1

#	undef FLUX_STDFEAT_CXX11_SHAREDPTR
#	define FLUX_STDFEAT_CXX11_SHAREDPTR     1

#	undef FLUX_STDFEAT_CXX11_UNORDERED
#	define FLUX_STDFEAT_CXX11_UNORDERED     1

#	undef FLUX_STDFEAT_CXX11_RANDOM
#	define FLUX_STDFEAT_CXX11_RANDOM        1

#	undef FLUX_STDFEAT_CXX11_CHRONO
#	define FLUX_STDFEAT_CXX11_CHRONO        1

#	undef FLUX_STDFEAT_CXX11_THREAD
#	define FLUX_STDFEAT_CXX11_THREAD        1

#	undef FLUX_STDFEAT_CXX11_FUTURE
#	define FLUX_STDFEAT_CXX11_FUTURE        1

#	undef FLUX_STDFEAT_CXX11_RATIO
#	define FLUX_STDFEAT_CXX11_RATIO         1

#	undef FLUX_STDFEAT_CXX11_EXCEPTPTR
#	define FLUX_STDFEAT_CXX11_EXCEPTPTR     1

#	undef FLUX_STDFEAT_CXX11_ATOMIC
#	define FLUX_STDFEAT_CXX11_ATOMIC 1

#	undef FLUX_STDQUIRK_CXX11_NO_UNDERLYING_TYPE
#	define FLUX_STDQUIRK_CXX11_NO_UNDERLYING_TYPE 0

#	undef FLUX_STDFEAT_CXX11_PTR_TRAITS
#	define FLUX_STDFEAT_CXX11_PTR_TRAITS 1
#endif // ~ C++11 language support

/* Added after dropping VS2013 support: */

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX11
#	undef FLUX_STDFEAT_CXX11_BADARRAYNEW
#	define FLUX_STDFEAT_CXX11_BADARRAYNEW 1

#	undef FLUX_STDFEAT_CXX11_REGEX
#	define FLUX_STDFEAT_CXX11_REGEX 1
#endif // ~ C++11 language support

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_EXT_STDFEAT_REQ_CONF_LANGLEVEL_HXX_21FF71D7_BBD7_4F68_A7B2_B053B03796EE
