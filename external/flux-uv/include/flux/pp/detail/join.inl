/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Implementation: FLUX_PP_JOIN*()
 */
/*-***************************************************************** -}}}1- */

//--//////////////////////////////////////////////////////////////////////////
//--    JOIN                        ///{{{1///////////////////////////////////

#define FLUX_PP_JOIN_IMPL0_(a,b) a##b
#define FLUX_PP_JOIN_IMPL_(a,b) FLUX_PP_JOIN_IMPL0_(a,b)

//--    JOIN2                       ///{{{1///////////////////////////////////

#define FLUX_PP_JOIN2_IMPL0_(a,b,c) a##b##c
#define FLUX_PP_JOIN2_IMPL_(a,b,c) FLUX_PP_JOIN2_IMPL0_(a,b,c)

//--    REV_JOIN                    ///{{{1///////////////////////////////////

#define FLUX_PP_REV_JOIN_IMPL0_(a,...) __VA_ARGS__##a
#define FLUX_PP_REV_JOIN_IMPL_(a,...) FLUX_PP_REV_JOIN_IMPL0_(a,__VA_ARGS__)

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
