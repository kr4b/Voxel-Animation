/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Implementation: FLUX_PP_STRINGIZE()
 */
/*-***************************************************************** -}}}1- */

//--//////////////////////////////////////////////////////////////////////////
//--    STRINGIZE                   ///{{{1///////////////////////////////////

/* Passes extra unused argument to STRINGIZE_IMPL_. This is a workaround for
 * MSVC, which otherwise tends to complain about insufficient arguments when
 * the argument `a` expands to nothing.
 */

#undef FLUX_PP_STRINGIZE
#define FLUX_PP_STRINGIZE(a) FLUX_PP_STRINGIZE_IMPL_(a,)

#define FLUX_PP_STRINGIZE_IMPL_(a,b) #a

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
