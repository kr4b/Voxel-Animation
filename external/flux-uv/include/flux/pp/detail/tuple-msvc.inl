/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Implementation: pre-processor tuples
 */
/*-***************************************************************** -}}}1- */

#include "../argn.hpp"
#include "../symbols.hpp"

//--//////////////////////////////////////////////////////////////////////////
//--    local                       ///{{{1///////////////////////////////////

/* See enum-msvc.inl for discussion re FLUX_PP_SYM_EMPTY */

#define FLUX_PP_TUPLE_EVALa_(...) __VA_ARGS__ 
#define FLUX_PP_TUPLE_EVAL_(...) FLUX_PP_TUPLE_EVALa_(__VA_ARGS__)

#define FLUX_PP_TUPLE_FLATTENa_(...) (__VA_ARGS__)
#define FLUX_PP_TUPLE_FLATTEN_(t) FLUX_PP_TUPLE_FLATTENa_ FLUX_PP_SYM_EMPTY t

//--    TO_LIST                     ///{{{1///////////////////////////////////

#define FLUX_PP_TUPLE_TO_LIST_(...) __VA_ARGS__

#undef FLUX_PP_TUPLE_TO_LIST
#define FLUX_PP_TUPLE_TO_LIST(t) FLUX_PP_TUPLE_EVAL_(                      \
		FLUX_PP_TUPLE_TO_LIST_ t                                           \
	) /*ENDM*/

//--    HEAD/TAIL                   ///{{{1///////////////////////////////////

/* NOTE: If you're here regarding Warning C4003 "not enough actual parameters
 * for macro FLUX_PP_TUPLE_{HEAD,TAIL}_", then don't worry. The result is 
 * correct despite the warning, but I don't want to suppress the warning by 
 * default, since it's generally a useful warning.
 *
 * (Tested with VisualStudio 2013)
 */
#define FLUX_PP_TUPLE_HEAD_(a,...) a
#define FLUX_PP_TUPLE_TAIL_(a,...) (__VA_ARGS__)

#undef FLUX_PP_TUPLE_HEAD
#define FLUX_PP_TUPLE_HEAD(t) FLUX_PP_TUPLE_EVAL_(                         \
		FLUX_PP_TUPLE_HEAD_ FLUX_PP_TUPLE_FLATTEN_(t) \
	) /*ENDM*/

#undef FLUX_PP_TUPLE_TAIL
#define FLUX_PP_TUPLE_TAIL(t) FLUX_PP_TUPLE_EVAL_(                         \
		FLUX_PP_TUPLE_TAIL_ FLUX_PP_TUPLE_FLATTEN_(t) \
	) /*ENDM*/

//--    GETN                        ///{{{1///////////////////////////////////

#undef FLUX_PP_TUPLE_GETN
#define FLUX_PP_TUPLE_GETN(n,t) FLUX_PP_ARGN(n, FLUX_PP_TUPLE_TO_LIST(t))

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
