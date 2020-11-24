/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Implementation: FLUX_PP_REPEAT*()
 */
/*-***************************************************************** -}}}1- */

#include "../join.hpp"

//--//////////////////////////////////////////////////////////////////////////
//--    REPEAT                      ///{{{1///////////////////////////////////

/* See enum-msvc.inl regarding FLUX_SYM_PP_EMPTY */

#define FLUX_PP_REPEAT_0_(m,...)
#define FLUX_PP_REPEAT_1_(m,...) m FLUX_PP_SYM_EMPTY (0,__VA_ARGS__)

#define FLUX_PP_REPEAT_2_(m,...)                                             \
	m FLUX_PP_SYM_EMPTY (0,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__)  \
	/*ENDM*/
#define FLUX_PP_REPEAT_3_(m,...)                                             \
	m FLUX_PP_SYM_EMPTY (0,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__)                                      \
	/*ENDM*/
#define FLUX_PP_REPEAT_4_(m,...)                                             \
	m FLUX_PP_SYM_EMPTY (0,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__)  \
	/*ENDM*/
#define FLUX_PP_REPEAT_5_(m,...)                                             \
	m FLUX_PP_SYM_EMPTY (0,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (4,__VA_ARGS__)                                      \
	/*ENDM*/
#define FLUX_PP_REPEAT_6_(m,...)                                             \
	m FLUX_PP_SYM_EMPTY (0,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (4,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (5,__VA_ARGS__)  \
	/*ENDM*/
#define FLUX_PP_REPEAT_7_(m,...)                                             \
	m FLUX_PP_SYM_EMPTY (0,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (4,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (5,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (6,__VA_ARGS__)                                      \
	/*ENDM*/
#define FLUX_PP_REPEAT_8_(m,...)                                             \
	m FLUX_PP_SYM_EMPTY (0,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (4,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (5,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (6,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (7,__VA_ARGS__)  \
	/*ENDM*/
#define FLUX_PP_REPEAT_9_(m,...)                                             \
	m FLUX_PP_SYM_EMPTY (0,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (4,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (5,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (6,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (7,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (8,__VA_ARGS__)                                      \
	/*ENDM*/
#define FLUX_PP_REPEAT_10_(m,...)                                            \
	m FLUX_PP_SYM_EMPTY (0,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (4,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (5,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (6,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (7,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (8,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (9,__VA_ARGS__)  \
	/*ENDM*/

#define FLUX_PP_REPEAT_11_(m,...) FLUX_PP_REPEAT_10_(m,__VA_ARGS__)          \
	m FLUX_PP_SYM_EMPTY (10,__VA_ARGS__)                                     \
	/*ENDM*/
#define FLUX_PP_REPEAT_12_(m,...) FLUX_PP_REPEAT_11_(m,__VA_ARGS__)          \
	m FLUX_PP_SYM_EMPTY (11,__VA_ARGS__)                                     \
	/*ENDM*/
#define FLUX_PP_REPEAT_13_(m,...) FLUX_PP_REPEAT_12_(m,__VA_ARGS__)          \
	m FLUX_PP_SYM_EMPTY (12,__VA_ARGS__)                                     \
	/*ENDM*/
#define FLUX_PP_REPEAT_14_(m,...) FLUX_PP_REPEAT_13_(m,__VA_ARGS__)          \
	m FLUX_PP_SYM_EMPTY (13,__VA_ARGS__)                                     \
	/*ENDM*/
#define FLUX_PP_REPEAT_15_(m,...) FLUX_PP_REPEAT_14_(m,__VA_ARGS__)          \
	m FLUX_PP_SYM_EMPTY (14,__VA_ARGS__)                                     \
	/*ENDM*/
#define FLUX_PP_REPEAT_16_(m,...) FLUX_PP_REPEAT_15_(m,__VA_ARGS__)          \
	m FLUX_PP_SYM_EMPTY (15,__VA_ARGS__)                                     \
	/*ENDM*/

#undef FLUX_PP_REPEAT_EX
#define FLUX_PP_REPEAT_EX(count,macro,...)                              \
	FLUX_PP_JOIN2(FLUX_PP_REPEAT_,count,_)(macro,__VA_ARGS__)           \
	/*END*/

#define FLUX_PP_REPEAT_FWD_(n,m) m(n)

#undef FLUX_PP_REPEAT
#define FLUX_PP_REPEAT(count,macro)                                    \
	FLUX_PP_JOIN2(FLUX_PP_REPEAT_,count,_)(FLUX_PP_REPEAT_FWD_,macro)  \
	/*END*/

//--    REPEAT_NZ                   ///{{{1///////////////////////////////////

#define FLUX_PP_REPEATNZ_0_(m,...)
#define FLUX_PP_REPEATNZ_1_(m,...)
#define FLUX_PP_REPEATNZ_2_(m,...) m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__)

#define FLUX_PP_REPEATNZ_3_(m,...)                                           \
	m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__)  \
	/*ENDM*/
#define FLUX_PP_REPEATNZ_4_(m,...)                                           \
	m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__)                                      \
	/*ENDM*/
#define FLUX_PP_REPEATNZ_5_(m,...)                                           \
	m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (4,__VA_ARGS__)  \
	/*ENDM*/
#define FLUX_PP_REPEATNZ_6_(m,...)                                           \
	m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (4,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (5,__VA_ARGS__)                                      \
	/*ENDM*/
#define FLUX_PP_REPEATNZ_7_(m,...)                                           \
	m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (4,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (5,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (6,__VA_ARGS__)  \
	/*ENDM*/
#define FLUX_PP_REPEATNZ_8_(m,...)                                           \
	m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (4,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (5,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (6,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (7,__VA_ARGS__)                                      \
	/*ENDM*/
#define FLUX_PP_REPEATNZ_9_(m,...)                                           \
	m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (4,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (5,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (6,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (7,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (8,__VA_ARGS__)  \
	/*ENDM*/
#define FLUX_PP_REPEATNZ_10_(m,...)                                          \
	m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (4,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (5,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (6,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (7,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (8,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (9,__VA_ARGS__)                                      \
	/*ENDM*/
#define FLUX_PP_REPEATNZ_11_(m,...)                                          \
	m FLUX_PP_SYM_EMPTY (1,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (2,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (3,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (4,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (5,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (6,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (7,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (8,__VA_ARGS__)  \
	m FLUX_PP_SYM_EMPTY (9,__VA_ARGS__) m FLUX_PP_SYM_EMPTY (10,__VA_ARGS__) \
	/*ENDM*/

#define FLUX_PP_REPEATNZ_12_(m,...) FLUX_PP_REPEATNZ_11_(m,__VA_ARGS__)      \
	m FLUX_PP_SYM_EMPTY (11,__VA_ARGS__)                                     \
	/*ENDM*/
#define FLUX_PP_REPEATNZ_13_(m,...) FLUX_PP_REPEATNZ_12_(m,__VA_ARGS__)      \
	m FLUX_PP_SYM_EMPTY (12,__VA_ARGS__)                                     \
	/*ENDM*/
#define FLUX_PP_REPEATNZ_14_(m,...) FLUX_PP_REPEATNZ_13_(m,__VA_ARGS__)      \
	m FLUX_PP_SYM_EMPTY (13,__VA_ARGS__)                                     \
	/*ENDM*/
#define FLUX_PP_REPEATNZ_15_(m,...) FLUX_PP_REPEATNZ_14_(m,__VA_ARGS__)      \
	m FLUX_PP_SYM_EMPTY (14,__VA_ARGS__)                                     \
	/*ENDM*/
#define FLUX_PP_REPEATNZ_16_(m,...) FLUX_PP_REPEATNZ_15_(m,__VA_ARGS__)      \
	m FLUX_PP_SYM_EMPTY (15,__VA_ARGS__)                                     \
	/*ENDM*/

#undef FLUX_PP_REPEAT_NZ_EX
#define FLUX_PP_REPEAT_NZ_EX(count,macro,...)                              \
	FLUX_PP_JOIN2(FLUX_PP_REPEATNZ_,count,_)(macro,__VA_ARGS__)            \
	/*END*/

#define FLUX_PP_REPEATNZ_FWD_(n,m) m(n)

#undef FLUX_PP_REPEAT_NZ
#define FLUX_PP_REPEAT_NZ(count,macro)                                     \
	FLUX_PP_JOIN2(FLUX_PP_REPEATNZ_,count,_)(FLUX_PP_REPEATNZ_FWD_,macro)  \
	/*END*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 