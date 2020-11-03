/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Implementation: FLUX_PP_MESSAGE()
 */
/*-***************************************************************** -}}}1- */

#include "../stringize.hpp"

//--//////////////////////////////////////////////////////////////////////////
//--    MESSAGE                     ///{{{1///////////////////////////////////

#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#	define FLUX_PP_GCC_PRAGMA_(x) _Pragma( FLUX_PP_STRINGIZE(x) )

#	undef FLUX_PP_MESSAGE
#	define FLUX_PP_MESSAGE(msg) FLUX_PP_GCC_PRAGMA_( message( msg ) )

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#	define FLUX_PP_CLANG_PRAGMA_(x) _Pragma( FLUX_PP_STRINGIZE(x) )

#	undef FLUX_PP_MESSAGE
#	define FLUX_PP_MESSAGE(msg) FLUX_PP_CLANG_PRAGMA_( message( msg ) )

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#	define FLUX_PP_MSVC_PRAGMA_(x) __pragma( x )

#	define FLUX_PP_MESSAGE_PREAMBLE_ \
		__FILE__ ":" FLUX_PP_STRINGIZE(__LINE__) ": note: #pragma message: " \
		/*ENDM*/
#	define FLUX_PP_MESSAGE_FMT_(msg) \
		(FLUX_PP_MESSAGE_PREAMBLE_ "\"" msg "\"") \
		/*ENDM*/

#	undef FLUX_PP_MESSAGE
#	define FLUX_PP_MESSAGE(msg) FLUX_PP_MSVC_PRAGMA_( \
		message FLUX_PP_MESSAGE_FMT_(msg) \
	) /*ENDM*/

//#else // other compiler
//FLUX_PP_MESSAGE( "No implementation for FLUX_PP_MESSAGE()" ); // ;-)

#endif // ~ compiler

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
