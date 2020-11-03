/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Implementation: FLUX_PP_IF*()
 */
/*-***************************************************************** -}}}1- */

#include "../join.hpp"
#include "../bool.hpp"

//--//////////////////////////////////////////////////////////////////////////
//--    IF                          ///{{{1///////////////////////////////////

#define FLUX_PP_IF_0_(t,f,...) f(__VA_ARGS__)
#define FLUX_PP_IF_1_(t,f,...) t(__VA_ARGS__)

#undef FLUX_PP_IF
#define FLUX_PP_IF(c,t,f,...)                                             \
	FLUX_PP_JOIN2(FLUX_PP_IF_,FLUX_PP_BOOL(c),_)(t,f,__VA_ARGS__)         \
	/*END*/

//--    IF_SYM                      ///{{{1///////////////////////////////////

#define FLUX_PP_IF_SYM_0_(t,f) f
#define FLUX_PP_IF_SYM_1_(t,f) t

#undef FLUX_PP_IF_SYM
#define FLUX_PP_IF_SYM(c,t,f)                                             \
	FLUX_PP_JOIN2(FLUX_PP_IF_SYM_,FLUX_PP_BOOL(c),_)(t,f)                 \
	/*END*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
