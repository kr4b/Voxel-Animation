/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Implementation: FLUX_PP_ENUM*()
 */
/*-***************************************************************** -}}}1- */

#include "../join.hpp"
#include "../identity.hpp"

//--//////////////////////////////////////////////////////////////////////////
//--    ENUM                        ///{{{1///////////////////////////////////

#define FLUX_PP_ENUM_0_(m,s,...) 
#define FLUX_PP_ENUM_1_(m,s,...) m(0,__VA_ARGS__)
#define FLUX_PP_ENUM_2_(m,s,...) m(0,__VA_ARGS__) s() m(1,__VA_ARGS__)

#define FLUX_PP_ENUM_3_(m,s,...)                                   \
	m(0,__VA_ARGS__) s() m(1,__VA_ARGS__) s() m(2,__VA_ARGS__)     \
	/*ENDM*/
#define FLUX_PP_ENUM_4_(m,s,...)                                   \
	m(0,__VA_ARGS__) s() m(1,__VA_ARGS__) s() m(2,__VA_ARGS__) s() \
	m(3,__VA_ARGS__)                                               \
	/*ENDM*/
#define FLUX_PP_ENUM_5_(m,s,...)                                   \
	m(0,__VA_ARGS__) s() m(1,__VA_ARGS__) s() m(2,__VA_ARGS__) s() \
	m(3,__VA_ARGS__) s() m(4,__VA_ARGS__)                          \
	/*ENDM*/
#define FLUX_PP_ENUM_6_(m,s,...)                                   \
	m(0,__VA_ARGS__) s() m(1,__VA_ARGS__) s() m(2,__VA_ARGS__) s() \
	m(3,__VA_ARGS__) s() m(4,__VA_ARGS__) s() m(5,__VA_ARGS__)     \
	/*ENDM*/
#define FLUX_PP_ENUM_7_(m,s,...)                                   \
	m(0,__VA_ARGS__) s() m(1,__VA_ARGS__) s() m(2,__VA_ARGS__) s() \
	m(3,__VA_ARGS__) s() m(4,__VA_ARGS__) s() m(5,__VA_ARGS__) s() \
	m(6,__VA_ARGS__)                                               \
	/*ENDM*/
#define FLUX_PP_ENUM_8_(m,s,...)                                   \
	m(0,__VA_ARGS__) s() m(1,__VA_ARGS__) s() m(2,__VA_ARGS__) s() \
	m(3,__VA_ARGS__) s() m(4,__VA_ARGS__) s() m(5,__VA_ARGS__) s() \
	m(6,__VA_ARGS__) s() m(7,__VA_ARGS__)                          \
	/*ENDM*/
#define FLUX_PP_ENUM_9_(m,s,...)                                   \
	m(0,__VA_ARGS__) s() m(1,__VA_ARGS__) s() m(2,__VA_ARGS__) s() \
	m(3,__VA_ARGS__) s() m(4,__VA_ARGS__) s() m(5,__VA_ARGS__) s() \
	m(6,__VA_ARGS__) s() m(7,__VA_ARGS__) s() m(8,__VA_ARGS__)     \
	/*ENDM*/

#define FLUX_PP_ENUM_10_(m,s,...) FLUX_PP_ENUM_9_(m,s,__VA_ARGS__) s() m(9,__VA_ARGS__)
#define FLUX_PP_ENUM_11_(m,s,...) FLUX_PP_ENUM_10_(m,s,__VA_ARGS__) s() m(10,__VA_ARGS__)
#define FLUX_PP_ENUM_12_(m,s,...) FLUX_PP_ENUM_11_(m,s,__VA_ARGS__) s() m(11,__VA_ARGS__)
#define FLUX_PP_ENUM_13_(m,s,...) FLUX_PP_ENUM_12_(m,s,__VA_ARGS__) s() m(12,__VA_ARGS__)
#define FLUX_PP_ENUM_14_(m,s,...) FLUX_PP_ENUM_13_(m,s,__VA_ARGS__) s() m(13,__VA_ARGS__)
#define FLUX_PP_ENUM_15_(m,s,...) FLUX_PP_ENUM_14_(m,s,__VA_ARGS__) s() m(14,__VA_ARGS__)
#define FLUX_PP_ENUM_16_(m,s,...) FLUX_PP_ENUM_15_(m,s,__VA_ARGS__) s() m(15,__VA_ARGS__)

#undef FLUX_PP_ENUM_EX
#define FLUX_PP_ENUM_EX(count, macro, separator, ... )                    \
	FLUX_PP_IDENTITY(                                                     \
		FLUX_PP_JOIN2(FLUX_PP_ENUM_,count,_)(macro,separator,__VA_ARGS__) \
	) /*ENDM*/

#define FLUX_PP_ENUM_FWD_(n,macro) macro(n)

#undef FLUX_PP_ENUM
#define FLUX_PP_ENUM(count, macro, separator)                                   \
	FLUX_PP_IDENTITY(                                                           \
		FLUX_PP_JOIN2(FLUX_PP_ENUM_,count,_)(FLUX_PP_ENUM_FWD_,separator,macro) \
	) /*ENDM*/

//--    ENUM_NZ                     ///{{{1///////////////////////////////////

#define FLUX_PP_ENUMEZ_0_(m,s,...) 
#define FLUX_PP_ENUMEZ_1_(m,s,...) 
#define FLUX_PP_ENUMEZ_2_(m,s,...) m(1,__VA_ARGS__)
#define FLUX_PP_ENUMEZ_3_(m,s,...) m(1,__VA_ARGS__) s() m(2,__VA_ARGS__)

#define FLUX_PP_ENUMEZ_4_(m,s,...)                                 \
	m(1,__VA_ARGS__) s() m(2,__VA_ARGS__) s() m(3,__VA_ARGS__)     \
	/*ENDM*/
#define FLUX_PP_ENUMEZ_5_(m,s,...)                                 \
	m(1,__VA_ARGS__) s() m(2,__VA_ARGS__) s() m(3,__VA_ARGS__) s() \
	m(4,__VA_ARGS__)                                               \
	/*ENDM*/
#define FLUX_PP_ENUMEZ_6_(m,s,...)                                 \
	m(1,__VA_ARGS__) s() m(2,__VA_ARGS__) s() m(3,__VA_ARGS__) s() \
	m(4,__VA_ARGS__) s() m(5,__VA_ARGS__)                          \
	/*ENDM*/
#define FLUX_PP_ENUMEZ_7_(m,s,...)                                 \
	m(1,__VA_ARGS__) s() m(2,__VA_ARGS__) s() m(3,__VA_ARGS__) s() \
	m(4,__VA_ARGS__) s() m(5,__VA_ARGS__) s() m(6,__VA_ARGS__)     \
	/*ENDM*/
#define FLUX_PP_ENUMEZ_8_(m,s,...)                                 \
	m(1,__VA_ARGS__) s() m(2,__VA_ARGS__) s() m(3,__VA_ARGS__) s() \
	m(4,__VA_ARGS__) s() m(5,__VA_ARGS__) s() m(6,__VA_ARGS__) s() \
	m(7,__VA_ARGS__)                                               \
	/*ENDM*/
#define FLUX_PP_ENUMEZ_9_(m,s,...)                                 \
	m(1,__VA_ARGS__) s() m(2,__VA_ARGS__) s() m(3,__VA_ARGS__) s() \
	m(4,__VA_ARGS__) s() m(5,__VA_ARGS__) s() m(6,__VA_ARGS__) s() \
	m(7,__VA_ARGS__) s() m(8,__VA_ARGS__)                          \
	/*ENDM*/
#define FLUX_PP_ENUMEZ_10_(m,s,...)                                \
	m(1,__VA_ARGS__) s() m(2,__VA_ARGS__) s() m(3,__VA_ARGS__) s() \
	m(4,__VA_ARGS__) s() m(5,__VA_ARGS__) s() m(6,__VA_ARGS__) s() \
	m(7,__VA_ARGS__) s() m(8,__VA_ARGS__) s() m(9,__VA_ARGS__)     \
	/*ENDM*/

#define FLUX_PP_ENUMEZ_11_(m,s,...) FLUX_PP_ENUMEZ_10_(m,s,__VA_ARGS__) s() m(10,__VA_ARGS__)
#define FLUX_PP_ENUMEZ_12_(m,s,...) FLUX_PP_ENUMEZ_11_(m,s,__VA_ARGS__) s() m(11,__VA_ARGS__)
#define FLUX_PP_ENUMEZ_13_(m,s,...) FLUX_PP_ENUMEZ_12_(m,s,__VA_ARGS__) s() m(12,__VA_ARGS__)
#define FLUX_PP_ENUMEZ_14_(m,s,...) FLUX_PP_ENUMEZ_13_(m,s,__VA_ARGS__) s() m(13,__VA_ARGS__)
#define FLUX_PP_ENUMEZ_15_(m,s,...) FLUX_PP_ENUMEZ_14_(m,s,__VA_ARGS__) s() m(14,__VA_ARGS__)
#define FLUX_PP_ENUMEZ_16_(m,s,...) FLUX_PP_ENUMEZ_15_(m,s,__VA_ARGS__) s() m(15,__VA_ARGS__)

#undef FLUX_PP_ENUM_NZ_EX
#define FLUX_PP_ENUM_NZ_EX(count, macro, separator, ... )                   \
	FLUX_PP_IDENTITY(                                                       \
		FLUX_PP_JOIN2(FLUX_PP_ENUMEZ_,count,_)(macro,separator,__VA_ARGS__) \
	) /*ENDM*/

#define FLUX_PP_ENUM_FWD_(n,macro) macro(n)

#undef FLUX_PP_ENUM_NZ
#define FLUX_PP_ENUM_NZ(count, macro, separator)                                  \
	FLUX_PP_IDENTITY(                                                             \
		FLUX_PP_JOIN2(FLUX_PP_ENUMEZ_,count,_)(FLUX_PP_ENUM_FWD_,separator,macro) \
	) /*ENDM*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
