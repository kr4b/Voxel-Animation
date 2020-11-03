/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Required standard library for VisualStudio
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_STDFEAT_REQ_CONF_VSCPP_HXX_C922B378_19E1_4F78_8C5C_601282C420FE
#define DETAIL_EXT_STDFEAT_REQ_CONF_VSCPP_HXX_C922B378_19E1_4F78_8C5C_601282C420FE

//--//////////////////////////////////////////////////////////////////////////
//--    C++11                       ///{{{1///////////////////////////////////

// TODO: is any of this implemented in 2010?

#if _MSC_VER >= 1700 /* Visual Studio 2012 */
/* MSDN: "As for the C++11 Standard Library [...], but Visual C++ in Visual
 * Studio 2010 does implement it, with two exceptions." These are:
 *   - variadic templates (but emulated)
 *   - <initializer_list>
 */
#	undef FLUX_STDFEAT_CXX11_FUNCTIONAL
#	undef FLUX_STDFEAT_CXX11_TYPE_TRAITS
#	undef FLUX_STDFEAT_CXX11_RANDOM
#	undef FLUX_STDFEAT_CXX11_TUPLE
#	undef FLUX_STDFEAT_CXX11_UNORDERED
#	undef FLUX_STDFEAT_CXX11_SHAREDPTR
#	undef FLUX_STDFEAT_CXX11_UNIQUEPTR
#	undef FLUX_STDFEAT_CXX11_THREAD
#	undef FLUX_STDFEAT_CXX11_FUTURE
#	undef FLUX_STDFEAT_CXX11_CHRONO
#	undef FLUX_STDFEAT_CXX11_RATIO
#	undef FLUX_STDFEAT_CXX11_ATOMIC
#	undef FLUX_STDFEAT_CXX11_EXCEPTPTR
#	undef FLUX_STDFEAT_CXX11_PTR_TRAITS

#	define FLUX_STDFEAT_CXX11_FUNCTIONAL    1
#	define FLUX_STDFEAT_CXX11_TYPE_TRAITS   1
#	define FLUX_STDFEAT_CXX11_RANDOM        1
#	define FLUX_STDFEAT_CXX11_TUPLE         1
#	define FLUX_STDFEAT_CXX11_UNORDERED     1
#	define FLUX_STDFEAT_CXX11_SHAREDPTR     1
#	define FLUX_STDFEAT_CXX11_UNIQUEPTR     1
#	define FLUX_STDFEAT_CXX11_THREAD        1
#	define FLUX_STDFEAT_CXX11_FUTURE        1
#	define FLUX_STDFEAT_CXX11_CHRONO        1
#	define FLUX_STDFEAT_CXX11_RATIO         1
#	define FLUX_STDFEAT_CXX11_ATOMIC        1
#	define FLUX_STDFEAT_CXX11_EXCEPTPTR     1
#	define FLUX_STDFEAT_CXX11_PTR_TRAITS    1 //XXX-stdquirk? see below

	// Note on CXX11_PTR_TRAITS: the MSDN docs claim that rebind<> is
	// implemented in a non-standard way up to and including VS2015. For
	// VS2015 this is definitively not the case, so it might also be 
	// available earlier.

#endif // ~ Visual Studio 2012

/* After dropping VS2013 support: */

#undef FLUX_STDQUIRK_CXX11_NO_NESTED_EXCEPT
#define FLUX_STDQUIRK_CXX11_NO_NESTED_EXCEPT 1

// Visual Studio has problematic implementations of `std::steady_clock`
// and `std::high_resolution_clock`.
#undef FLUX_STDQUIRK_CXX11_INACCURATE_CLOCKS
#define FLUX_STDQUIRK_CXX11_INACCURATE_CLOCKS 1

// VS2013 (and presumably earlier versions) use a std::alignment_of that
// tries to instantiate the provided type, which fails for obvious reasons.
#undef FLUX_STDQUIRK_CXX11_ALIGNMENT_OF_ABSTRACT
#define FLUX_STDQUIRK_CXX11_ALIGNMENT_OF_ABSTRACT 1

#if _MSC_VER >= 1700 /* Visual Studio 2012 */
#	undef FLUX_STDFEAT_CXX11_BADARRAYNEW
#	undef FLUX_STDFEAT_CXX11_REGEX

#	define FLUX_STDFEAT_CXX11_BADARRAYNEW   1
#	define FLUX_STDFEAT_CXX11_REGEX         1
#endif // ~ Visual Studio 2012

#if _MSC_VER >= 1900 /* Visual Studio 2015 */

	// Fixed in Visual Studio 2015
#	undef FLUX_STDQUIRK_CXX11_INACCURATE_CLOCKS
#	undef FLUX_STDQUIRK_CXX11_NO_NESTED_EXCEPT
#	undef FLUX_STDQUIRK_CXX11_ALIGNMENT_OF_ABSTRACT

	// Fixed in Visual Studio 2015
	// XXX-TODO-presumably; verify!
#	define FLUX_STDQUIRK_CXX11_INACCURATE_CLOCKS 0
#	define FLUX_STDQUIRK_CXX11_NO_NESTED_EXCEPT 0
#	define FLUX_STDQUIRK_CXX11_ALIGNMENT_OF_ABSTRACT 0
#endif // ~ Visual Studio 2015	

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_STDFEAT_REQ_CONF_VSCPP_HXX_C922B378_19E1_4F78_8C5C_601282C420FE
