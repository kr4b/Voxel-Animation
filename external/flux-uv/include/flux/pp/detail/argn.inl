/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Implementation: FLUX_PP_ARGN()
 */
/*-***************************************************************** -}}}1- */

#include "../join.hpp"

//--//////////////////////////////////////////////////////////////////////////
//--    ARGN                        ///{{{1///////////////////////////////////

#define FLUX_PP_ARG_0_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _0
#define FLUX_PP_ARG_1_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _1
#define FLUX_PP_ARG_2_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _2
#define FLUX_PP_ARG_3_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _3
#define FLUX_PP_ARG_4_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _4
#define FLUX_PP_ARG_5_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _5
#define FLUX_PP_ARG_6_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _6
#define FLUX_PP_ARG_7_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _7
#define FLUX_PP_ARG_8_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _8
#define FLUX_PP_ARG_9_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _9
#define FLUX_PP_ARG_10_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _A
#define FLUX_PP_ARG_11_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _B
#define FLUX_PP_ARG_12_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _C
#define FLUX_PP_ARG_13_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _D
#define FLUX_PP_ARG_14_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _E
#define FLUX_PP_ARG_15_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _F
#define FLUX_PP_ARG_16_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F,_G,...) _G


#define FLUX_PP_ARGNb_(...) __VA_ARGS__
#define FLUX_PP_ARGNa_(...) FLUX_PP_ARGNb_(__VA_ARGS__)

#undef FLUX_PP_ARGN
#define FLUX_PP_ARGN(n,...) FLUX_PP_ARGNa_( \
		FLUX_PP_JOIN2(FLUX_PP_ARG_,n,_)(__VA_ARGS__,,,,,,,,,,,,,,,,,) \
	) /*ENDM*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
