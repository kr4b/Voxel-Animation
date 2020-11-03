/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Implementation: FLUX_PP_BOOL()
 */
/*-***************************************************************** -}}}1- */

#include "../join.hpp"

//--//////////////////////////////////////////////////////////////////////////
//--    BOOL                        ///{{{1///////////////////////////////////

#define FLUX_PP_BOOL_0_ 0
#define FLUX_PP_BOOL_1_ 1
#define FLUX_PP_BOOL_2_ 1
#define FLUX_PP_BOOL_3_ 1
#define FLUX_PP_BOOL_4_ 1
#define FLUX_PP_BOOL_5_ 1
#define FLUX_PP_BOOL_6_ 1
#define FLUX_PP_BOOL_7_ 1
#define FLUX_PP_BOOL_8_ 1
#define FLUX_PP_BOOL_9_ 1
#define FLUX_PP_BOOL_10_ 1
#define FLUX_PP_BOOL_11_ 1
#define FLUX_PP_BOOL_12_ 1
#define FLUX_PP_BOOL_13_ 1
#define FLUX_PP_BOOL_14_ 1
#define FLUX_PP_BOOL_15_ 1
#define FLUX_PP_BOOL_16_ 1

#define FLUX_PP_BOOLb_(a) a
#define FLUX_PP_BOOLa_(a) FLUX_PP_BOOLb_(a)

#undef FLUX_PP_BOOL
#define FLUX_PP_BOOL(a) FLUX_PP_BOOLa_(FLUX_PP_JOIN2(FLUX_PP_BOOL_,a,_))

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
