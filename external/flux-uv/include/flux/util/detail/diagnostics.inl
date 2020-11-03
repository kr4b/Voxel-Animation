/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Diagnostics implementations
 *
 * NVCC makes somewhat of a mess with this. Essentially, if we use NVCC, we
 * have to emit all #pragma:s / _Pragma:s twice. Once for CUDAFE and once for
 * the host compiler (COMPILER_FAMILY).
 *
 * TODO: the above.
 */
/*-***************************************************************** -}}}1- */

#include <flux/pp/if.hpp>
#include <flux/pp/join.hpp>
#include <flux/pp/tuple.hpp>
#include <flux/pp/symbols.hpp>
#include <flux/pp/identity.hpp>
#include <flux/pp/stringize.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    Diagnostic IDs              ///{{{1///////////////////////////////////

/* Unsupported diagnostic. */
#define FLUX_DIAGNOSTIC_UNSUPPORTED                       \
	(                                                     \
		0, ~, /* GCC */                                   \
		0, ~, /* Clang */                                 \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/

/* Various known diagnostics */
#undef FLUX_DIAGNOSTIC_ID_DEPRECATION
#define FLUX_DIAGNOSTIC_ID_DEPRECATION                    \
	(                                                     \
		1, "-Wdeprecated-declarations", /* GCC */         \
		1, "-Wdeprecated", /* Clang */                    \
		1, 4996, /* MSVC */                               \
		1, deprecated_entity, /* NVCC */                  \
	) /*ENDM*/
#undef FLUX_DIAGNOSTIC_ID_UNUSED_ARGUMENT
#define FLUX_DIAGNOSTIC_ID_UNUSED_ARGUMENT                \
	(                                                     \
		1, "-Wunused-parameter", /* GCC */                \
		1, "-Wunused-parameter", /* Clang */              \
		1, 4100, /* MSVC */                               \
		0, ~, /* NVCC doesn't warn about this */          \
	) /*ENDM*/
#undef FLUX_DIAGNOSTIC_ID_UNUSED_VARIABLE
#define FLUX_DIAGNOSTIC_ID_UNUSED_VARIABLE                \
	(                                                     \
		1, "-Wunused-variable", /* GCC */                 \
		1, "-Wunused-variable", /* Clang */               \
		1, 4101, /* MSVC */                               \
		1, declared_but_not_referenced, /* NVCC */        \
	) /*ENDM*/
#undef FLUX_DIAGNOSTIC_ID_UNUSED_FUNCTION
#define FLUX_DIAGNOSTIC_ID_UNUSED_FUNCTION                \
	(                                                     \
		1, "-Wunused-function", /* GCC */                 \
		1, "-Wunused-function", /* Clang */               \
		0, ~, /* MSVC */                                  \
		1, declared_but_not_referenced, /* NVCC; XXX-verify*/        \
	) /*ENDM*/
#undef FLUX_DIAGNOSTIC_ID_UNREACHABLE_CODE
#define FLUX_DIAGNOSTIC_ID_UNREACHABLE_CODE               \
	(                                                     \
		1, "-Wunreachable-code", /* GCC */                \
		1, "-Wunreachable-code", /* Clang */              \
		1, 4702, /* MSVC */                               \
		1, code_is_unreachable, /* NVCC */                \
	) /*ENDM*/
#undef FLUX_DIAGNOSTIC_ID_AMBIGUOUS_ELSE
#define FLUX_DIAGNOSTIC_ID_AMBIGUOUS_ELSE                 \
	(                                                     \
		1, "-Wparentheses", /* GCC */                     \
		1, "-Wdangling-else", /* Clang */                 \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/
#undef FLUX_DIAGNOSTIC_ID_UNNEEDED_INTERNAL
#define FLUX_DIAGNOSTIC_ID_UNNEEDED_INTERNAL              \
	(                                                     \
		0, ~, /* GCC */                                   \
		1, "-Wunneeded-internal-declaration", /* Clang */ \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/
#undef FLUX_DIAGNOSTIC_ID_UNDEFINED_MACRO
#define FLUX_DIAGNOSTIC_ID_UNDEFINED_MACRO                \
	(                                                     \
		1, "-Wundef", /* GCC */                           \
		1, "-Wundef", /* Clang */                         \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/
#undef FLUX_DIAGNOSTIC_ID_SIZEOF_EXPR
#define FLUX_DIAGNOSTIC_ID_SIZEOF_EXPR                    \
	(                                                     \
		0, ~, /* GCC */                                   \
		0, ~, /* Clang */                                 \
		1, 6334, /* MSVC */                               \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/
#undef FLUX_DIAGNOSTIC_ID_COMMA_IN_IF
#define FLUX_DIAGNOSTIC_ID_COMMA_IN_IF                    \
	(                                                     \
		0, ~, /* GCC */                                   \
		0, ~, /* Clang */                                 \
		1, 6319, /* MSVC */                               \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/
#undef FLUX_DIAGNOSTIC_ID_NORETURN_RETURNS
#define FLUX_DIAGNOSTIC_ID_NORETURN_RETURNS               \
	(                                                     \
		1, "-Winvalid-noreturn", /* GCC */                \
		0, ~, /* Clang */                                 \
		0, ~, /* MSVC */                                  \
		1, noreturn_function_does_return, /* NVCC */      \
	) /*ENDM*/
#undef FLUX_DIAGNOSTIC_ID_ZERO_AS_NULLPTR
#define FLUX_DIAGNOSTIC_ID_ZERO_AS_NULLPTR                \
	(                                                     \
		1, "-Wzero-as-null-pointer-constant", /* GCC */   \
		0, ~, /* Clang */                                 \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/

#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC && __GNUC__ >= 8
#	undef FLUX_DIAGNOSTIC_ID_CLASS_MEMACCESS
#	define FLUX_DIAGNOSTIC_ID_CLASS_MEMACCESS                 \
		(                                                     \
			1, "-Wclass-memaccess", /* GCC */                 \
			0, ~, /* Clang */                                 \
			0, ~, /* MSVC */                                  \
			0, ~, /* NVCC */                                  \
		) /*ENDM*/
#else // not GCC 8.x and later
#	undef FLUX_DIAGNOSTIC_ID_CLASS_MEMACCESS
#	define FLUX_DIAGNOSTIC_ID_CLASS_MEMACCESS                 \
		(                                                     \
			0, ~, /* GCC */                                   \
			0, ~, /* Clang */                                 \
			0, ~, /* MSVC */                                  \
			0, ~, /* NVCC */                                  \
		) /*ENDM*/
#endif // ~ GCC 8.x and later

#undef FLUX_DIAGNOSTIC_ID_SIGN_PROMO
#define FLUX_DIAGNOSTIC_ID_SIGN_PROMO                     \
	(                                                     \
		1, "-Wsign-promo", /* GCC */                      \
		0, ~, /* Clang */                                 \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/

#undef FLUX_DIAGNOSTIC_ID_STRICT_ALIASING
#define FLUX_DIAGNOSTIC_ID_STRICT_ALIASING                \
	(                                                     \
		1, "-Wstrict-aliasing", /* GCC */                 \
		0, ~, /* Clang */                                 \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/

#undef FLUX_DIAGNOSTIC_ID_RESTRICT
#define FLUX_DIAGNOSTIC_ID_RESTRICT                       \
	(                                                     \
		1, "-Wrestrict", /* GCC */                        \
		0, ~, /* Clang */                                 \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/

#undef FLUX_DIAGNOSTIC_ID_TYPE_LIMITS
#define FLUX_DIAGNOSTIC_ID_TYPE_LIMITS                    \
	(                                                     \
		1, "-Wtype-limits", /* GCC */                     \
		0, ~, /* Clang */                                 \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/

#undef FLUX_DIAGNOSTIC_ID_MISSING_FIELD_INIT
#define FLUX_DIAGNOSTIC_ID_MISSING_FIELD_INIT             \
	(                                                     \
		0, ~, /* GCC */                                   \
		1, "-Wmissing-field-initializers", /* Clang */    \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/

#undef FLUX_DIAGNOSTIC_ID_EXTRA
#define FLUX_DIAGNOSTIC_ID_EXTRA                          \
	(                                                     \
		1, "-Wextra", /* GCC */                           \
		0, ~, /* Clang */                                 \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/

#undef FLUX_DIAGNOSTIC_ID_IMPLICIT_FALLTHROUGH
#define FLUX_DIAGNOSTIC_ID_IMPLICIT_FALLTHROUGH           \
	(                                                     \
		0, ~, /* GCC */                                   \
		1, "-Wimplicit-fallthrough", /* Clang */          \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/

#undef FLUX_DIAGNOSTIC_ID_CONVERSION
#define FLUX_DIAGNOSTIC_ID_CONVERSION                     \
	(                                                     \
		0, ~, /* GCC */                                   \
		0, ~, /* Clang */                                 \
		1, 4242, /* MSVC */                               \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/
#undef FLUX_DIAGNOSTIC_ID_CONVERSION_SIZET
#define FLUX_DIAGNOSTIC_ID_CONVERSION_SIZET               \
	(                                                     \
		0, ~, /* GCC */                                   \
		0, ~, /* Clang */                                 \
		1, 4267, /* MSVC */                               \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/

#undef FLUX_DIAGNOSTIC_ID_DECLARATION_PARENS
#define FLUX_DIAGNOSTIC_ID_DECLARATION_PARENS             \
	(                                                     \
		1, "-Wparentheses", /* GCC */                     \
		0, ~, /* Clang */                                 \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/
#undef FLUX_DIAGNOSTIC_ID_CAST_TYPE_QUALIFIERS
#define FLUX_DIAGNOSTIC_ID_CAST_TYPE_QUALIFIERS           \
	(                                                     \
		1, "-Wignored-qualifiers", /* GCC */              \
		0, ~, /* Clang */                                 \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/
#undef FLUX_DIAGNOSTIC_ID_SELF_ASSIGN
#define FLUX_DIAGNOSTIC_ID_SELF_ASSIGN                    \
	(                                                     \
		0, ~, /* GCC */                                   \
		1, "-Wself-assign-overloaded", /* Clang */        \
		0, ~, /* MSVC */                                  \
		0, ~, /* NVCC */                                  \
	) /*ENDM*/


//--    GCC                         ///{{{1///////////////////////////////////
#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC

#	define FLUX_DIAGNOSTIC_GCC_PRAGMA_(x)                          \
		_Pragma( FLUX_PP_STRINGIZE(x) )                            \
	/*ENDM*/
#	define FLUX_DIAGNOSTIC_GCC_SET_(mode,id)                       \
		FLUX_DIAGNOSTIC_GCC_PRAGMA_( GCC diagnostic mode id )      \
	/*ENDM*/

	// Diagnostic Modes -{{{2-
#	define FLUX_DIAGNOSTIC_GCC_MODE0_ ignored
#	define FLUX_DIAGNOSTIC_GCC_MODE1_ warning /* Once not supported, so do it always */
#	define FLUX_DIAGNOSTIC_GCC_MODE2_ warning
#	define FLUX_DIAGNOSTIC_GCC_MODE3_ error

	// Manipulate diagnostic state -{{{2-
	/* pushing and popping state is only supported from GCC 4.7 and forward
	 */
#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)
#		define FLUX_DIAGNOSTIC_STATE_PUSH_HOST_() _Pragma( "GCC diagnostic push" )
#		define FLUX_DIAGNOSTIC_STATE_POP_HOST_() _Pragma( "GCC diagnostic pop" )

#		define FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT_HOST_(id)      \
			FLUX_DIAGNOSTIC_STATE_POP_HOST_()                      \
		/*ENDM*/
#	else // GCC <= 4.6
#		define FLUX_DIAGNOSTIC_STATE_PUSH_HOST_()
#		define FLUX_DIAGNOSTIC_STATE_POP_HOST_()

#		define FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT_HOST_(id)      \
			FLUX_PP_IF( FLUX_PP_TUPLE_GETN(0,id),                  \
				FLUX_DIAGNOSTIC_GCC_SET_,                          \
				FLUX_PP_EMPTY,                                     \
				warning /* No "default" choice here. */,           \
				FLUX_PP_TUPLE_GETN(1,id)                           \
			) /*ENDM*/
#	endif // ~ GCC 4.7


#	define FLUX_DIAGNOSTIC_STATE_SET_RAW_HOST_(mode,id)            \
		FLUX_PP_IF( FLUX_PP_TUPLE_GETN(0,id),                      \
			FLUX_DIAGNOSTIC_GCC_SET_,                              \
			FLUX_PP_EMPTY,                                         \
			FLUX_PP_JOIN2(FLUX_DIAGNOSTIC_GCC_MODE,mode,_),        \
			FLUX_PP_TUPLE_GETN(1,id)                               \
		) /*ENDM*/

//--    Clang                       ///{{{1///////////////////////////////////
#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG

#	define FLUX_DIAGNOSTIC_CLANG_PRAGMA_(x)                        \
		_Pragma( FLUX_PP_STRINGIZE(x) )                            \
	/*ENDM*/
#	define FLUX_DIAGNOSTIC_CLANG_SET_(mode,id)                     \
		FLUX_DIAGNOSTIC_CLANG_PRAGMA_( clang diagnostic mode id )  \
	/*ENDM*/

	// Diagnostic Modes -{{{2-
#	define FLUX_DIAGNOSTIC_CLANG_MODE0_ ignored
#	define FLUX_DIAGNOSTIC_CLANG_MODE1_ warning /* Once not supported, so do it always */
#	define FLUX_DIAGNOSTIC_CLANG_MODE2_ warning
#	define FLUX_DIAGNOSTIC_CLANG_MODE3_ error

	// Manipulate diagnostic state -{{{2-
#	define FLUX_DIAGNOSTIC_STATE_PUSH_HOST_() _Pragma( "clang diagnostic push" )
#	define FLUX_DIAGNOSTIC_STATE_POP_HOST_() _Pragma( "clang diagnostic pop" )

#	define FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT_HOST_(id)          \
		FLUX_DIAGNOSTIC_STATE_POP_HOST_()                          \
	/*ENDM*/

#	define FLUX_DIAGNOSTIC_STATE_SET_RAW_HOST_(mode,id)            \
		FLUX_PP_IF( FLUX_PP_TUPLE_GETN(2,id),                      \
			FLUX_DIAGNOSTIC_CLANG_SET_,                            \
			FLUX_PP_EMPTY,                                         \
			FLUX_PP_JOIN2(FLUX_DIAGNOSTIC_CLANG_MODE,mode,_),      \
			FLUX_PP_TUPLE_GETN(3,id)                               \
		) /*ENDM*/

//--    MSVC                        ///{{{1///////////////////////////////////
#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#	define FLUX_DIAGNOSTIC_MSVC_PRAGMA_(x) __pragma( x )
#	define FLUX_DIAGNOSTIC_MSVC_SET_(mode,id)                      \
		FLUX_DIAGNOSTIC_MSVC_PRAGMA_( warning( mode : id ) )       \
	/*ENDM*/

	// Diagnostic Modes -{{{2-
#	define FLUX_DIAGNOSTIC_MSVC_MODE0_ disable
#	define FLUX_DIAGNOSTIC_MSVC_MODE1_ once
#	define FLUX_DIAGNOSTIC_MSVC_MODE2_ 1
#	define FLUX_DIAGNOSTIC_MSVC_MODE3_ error

	// Manipulate diagnostic state -{{{2-
#	define FLUX_DIAGNOSTIC_STATE_PUSH_HOST_() //TODO
#	define FLUX_DIAGNOSTIC_STATE_POP_HOST_() //TODO

#	define FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT_HOST_(id)          \
		FLUX_DIAGNOSTIC_STATE_POP_HOST_()                          \
	/*ENDM*/

#	define FLUX_DIAGNOSTIC_STATE_SET_RAW_HOST_(mode,id)            \
		FLUX_PP_IF( FLUX_PP_TUPLE_GETN(4,id),                      \
			FLUX_DIAGNOSTIC_MSVC_SET_,                             \
			FLUX_PP_EMPTY,                                         \
			FLUX_PP_JOIN2(FLUX_DIAGNOSTIC_MSVC_MODE,mode,_),       \
			FLUX_PP_TUPLE_GETN(5,id)                               \
		) /*ENDM*/

//--    unknown/unhandled           ///{{{1///////////////////////////////////
#else // unknown / unhandled
#	define FLUX_DIAGNOSTIC_STATE_PUSH_HOST_()
#	define FLUX_DIAGNOSTIC_STATE_POP_HOST_()
#	define FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT_HOST_(id)
#	define FLUX_DIAGNOSTIC_STATE_SET_RAW_HOST_(mode,id)

#endif // COMPILER_FAMILY

//--    NVCC                        ///{{{1///////////////////////////////////
#if FLUX_COMPILER_ACTUAL == FLUX_COMPILER_NVCC

#	define FLUX_DIAGNOSTIC_NVCC_PRAGMA_(x)                         \
		_Pragma( FLUX_PP_STRINGIZE(x) )                            \
	/*ENDM*/
#	define FLUX_DIAGNOSTIC_NVCC_SET_(mode,id)                      \
		FLUX_DIAGNOSTIC_NVCC_PRAGMA_( mode = id )                  \
	/*ENDM*/

	/* The docs for this are a bit hard to find. The following stems from
	 *
	 *  - http://www.edg.com/docs/edg_cpp.pdf
	 *  - http://www.ssl.berkeley.edu/~jimm/grizzly_docs/SSL/opt/intel/cc/9.0/lib/locale/en_US/mcpcom.msg
	 */

	// Diagnostic Modes -{{{2-
#	define FLUX_DIAGNOSTIC_NVCC_MODE0_ diag_suppress
#	define FLUX_DIAGNOSTIC_NVCC_MODE1_ diag_once
#	define FLUX_DIAGNOSTIC_NVCC_MODE2_ diag_warning
#	define FLUX_DIAGNOSTIC_NVCC_MODE3_ diag_error

	// Manipulate diagnostic state -{{{2-
#	undef FLUX_DIAGNOSTIC_STATE_PUSH
#	define FLUX_DIAGNOSTIC_STATE_PUSH()                            \
		FLUX_DIAGNOSTIC_STATE_PUSH_HOST_()                         \
		/* NVCC - not supported. */                                \
	/*ENDM*/

#	undef FLUX_DIAGNOSTIC_STATE_POP
#	define FLUX_DIAGNOSTIC_STATE_POP()                             \
		FLUX_DIAGNOSTIC_STATE_POP_HOST_()                          \
		/* NVCC -not supported */                                  \
	/*ENDM*/

#	undef FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT
#	define FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT(id)                \
		FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT_HOST_(id)             \
		FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT_RAW_(                 \
			FLUX_PP_JOIN(FLUX_DIAGNOSTIC_ID_,id)                   \
		) /*ENDM*/

#	define FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT_RAW_(id)           \
		FLUX_PP_IF( FLUX_PP_TUPLE_GETN(6,id),                      \
			FLUX_DIAGNOSTIC_NVCC_SET_,                             \
			FLUX_PP_EMPTY,                                         \
			diag_default,          \
			FLUX_PP_TUPLE_GETN(7,id)                               \
		) /*ENDM*/

#	undef FLUX_DIAGNOSTIC_STATE_SET_RAW
#	define FLUX_DIAGNOSTIC_STATE_SET_RAW(mode,id)                  \
		FLUX_DIAGNOSTIC_STATE_SET_RAW_HOST_(mode,id)               \
		FLUX_PP_IF( FLUX_PP_TUPLE_GETN(6,id),                      \
			FLUX_DIAGNOSTIC_NVCC_SET_,                             \
			FLUX_PP_EMPTY,                                         \
			FLUX_PP_JOIN2(FLUX_DIAGNOSTIC_NVCC_MODE,mode,_),       \
			FLUX_PP_TUPLE_GETN(7,id)                               \
		) /*ENDM*/

//--    !NVCC                        ///{{{1///////////////////////////////////
#else // ! NVCC

#	undef FLUX_DIAGNOSTIC_STATE_PUSH
#	define FLUX_DIAGNOSTIC_STATE_PUSH()                            \
		FLUX_DIAGNOSTIC_STATE_PUSH_HOST_()                         \
	/*ENDM*/

#	undef FLUX_DIAGNOSTIC_STATE_POP
#	define FLUX_DIAGNOSTIC_STATE_POP()                             \
		FLUX_DIAGNOSTIC_STATE_POP_HOST_()                          \
	/*ENDM*/

#	undef FLUX_DIAGNOSTIC_STATE_SET_RAW
#	define FLUX_DIAGNOSTIC_STATE_SET_RAW(mode,id)                  \
		FLUX_DIAGNOSTIC_STATE_SET_RAW_HOST_(mode,id)               \
	/*ENDM*/

#endif // ~ NVCC

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
