/*-******************************************************* -- HEADER -{{{1- */
/*- Award badness points.
 *
 * Experimental.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_BADNESS_HPP_CFA6230E_693E_4DA2_8453_1F8195E18CDB
#define FLUX_REPORT_BADNESS_HPP_CFA6230E_693E_4DA2_8453_1F8195E18CDB

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/pp/join.hpp>
#include <flux/pp/stringize.hpp>
#include <flux/compat/current_func.hpp>


#include "state.hpp"
#include "defaults.hpp"

//--    >>> namespace = flux::report >>>        ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(report)
//--    macros                          ///{{{2///////////////////////////////

// __LINE__ is weird on MSVC: with /Zi it cannot be used in a constexpr 
// context, because it refers to a hidden local variable. This is related to
// Edit and Continue. The following workaround avoids this.
#if FLUX_COMPILER == FLUX_COMPILER_MSVC
#	define FLUX_REPORT_LINE FLUX_PP_JOIN(__LINE__,l)
#else
#	define FLUX_REPORT_LINE __LINE__
#endif // ~ MSVC

//API : badness(), can be called multiple times and accumulates scores
//    - track multiple sources?

//API : unique_badness(), can be called multiple times but only tiggers once
//    - should be relatively cheap to call from e.g. hotspots


// Filter options:
//
//   - state: only report if app state > badness "state"
//   - duration: only report if time > larger than a certain threshold
//      make duration = score.

#if FLUXCFG_REPORT_BADNESS
#	define FLUX_REPORT_BADNESS( score, name, desc )                    \
		FLUX_REPORT_BADNESS_EX( ignored, score, name, desc )           \
		/*ENDM*/
#	define FLUX_REPORT_BADNESS_EX( state, score, name, desc )          \
		do { ::flux::report::badness(                                  \
			::flux::report::EState::state, name, desc,                 \
			__FILE__, FLUX_COMPAT_FUNCTION_NAME, FLUX_REPORT_LINE,     \
			score                                                      \
		); } while(0) /*ENDM*/

#	define FLUX_REPORT_DEFINE_BADNESS( ident, name, desc )             \
		FLUX_REPORT_DEFINE_BADNESS_EX(                                 \
			ident, ignored, name, desc                                 \
		) /*ENDM*/
#	define FLUX_REPORT_DEFINE_BADNESS_EX( ident, state, name, desc )   \
		static constexpr ::flux::report::DeclaredBadness      \
			FLUX_PP_JOIN2(fluxDeclaredBadness,ident,_) = {             \
				::flux::report::EState::state, name, desc,             \
			} /*ENDM*/

#	define FLUX_REPORT_DECLARED_BADNESS( ident, score )                \
		do { ::flux::report::declared_badness(                         \
			&FLUX_PP_JOIN2(fluxDeclaredBadness,ident,_),               \
			score                                                      \
		); } while(0) /*ENDM*/

#	define FLUX_REPORT_UNIQUE_BADNESS( score, name, desc )             \
		do {                                                           \
			FLUX_REPORT_DEFINE_BADNESS( __LINE__, name, desc );        \
			FLUX_REPORT_DECLARED_BADNESS( __LINE__, score );           \
		} while(0) /*ENDM*/
#	define FLUX_REPORT_UNIQUE_BADNESS_EX( state, score, name, desc )   \
		do {                                                           \
			FLUX_REPORT_DEFINE_BADNESS_EX( __LINE__, state, name, desc ); \
			FLUX_REPORT_DECLARED_BADNESS( __LINE__, score );           \
		} while(0) /*ENDM*/


#else // !FLUXCFG_REPORT_BADNESS
#	define FLUX_REPORT_BADNESS( score, name, desc )
#	define FLUX_REPORT_BADNESS_EX( state, store, name, desc )

#	define FLUX_REPORT_DEFINE_BADNESS( ident, name, desc )
#	define FLUX_REPORT_DEFINE_BADNESS_EX( ident, state, name, desc )

#	define FLUX_REPORT_DECLARED_BADNESS( ident, score )

#	define FLUX_REPORT_UNIQUE_BADNESS( score, name, desc )
#	define FLUX_REPORT_UNIQUE_BADNESS_EX( state, score, name, desc )

#endif // ~ FLUXCFG_REPORT_BADNESS

// --- OLD ---
#if 0
#if FLUXCFG_REPORT_BADNESS
#	define FLUX_REPORT_DECLARE_BADNESS( state, ident, desc )           \
		static constexpr ::flux::report::Badness              \
			FLUX_PP_JOIN2(fluxBadness,ident,_) = {                     \
				::flux::report::EState::state,                         \
				FLUX_PP_STRINGIZE(ident),                              \
				desc                                                   \
			}; /*ENDM*/
#	define FLUX_REPORT_BADNESS( ident, score )                         \
		static constexpr ::flux::report::BadnessSource        \
			FLUX_PP_JOIN2(fluxUniqueBadnessSrc,__LINE__,_) = {         \
				FLUX_COMPAT_FUNCTION_NAME, __FILE__, FLUX_REPORT_LINE  \
			};                                                         \
		::flux::report::badness(                                       \
			FLUX_PP_JOIN2(fluxBadness,ident,_),                        \
			FLUX_PP_JOIN2(fluxUniqueBadnessSrc,__LINE__,_),            \
			score                                                      \
		); /*ENDM*/
		
#	define FLUX_REPORT_UNIQUE_BADNESS( state, name, desc, score )      \
		static constexpr ::flux::report::Badness              \
			FLUX_PP_JOIN2(fluxUniqueBadness,__LINE__,_) = {            \
				::flux::report::EState::state, name, desc              \
			};                                                         \
		static constexpr ::flux::report::BadnessSource        \
			FLUX_PP_JOIN2(fluxUniqueBadnessSrc,__LINE__,_) = {         \
				FLUX_COMPAT_FUNCTION_NAME, __FILE__, FLUX_REPORT_LINE  \
			};                                                         \
		::flux::report::badness(                                       \
			FLUX_PP_JOIN2(fluxUniqueBadness,__LINE__,_),               \
			FLUX_PP_JOIN2(fluxUniqueBadnessSrc,__LINE__,_),            \
			score                                                      \
		); /*ENDM*/

#else // !REPORT_BADNESS
#	define FLUX_REPORT_DECLARE_BADNESS(s,i,d)  /* nothing */
#	define FLUX_REPORT_BADNESS(i,s)            /* nothing */

#	define FLUX_REPORT_UNIQUE_BADNESS(s,n,d,s) /* nothing */

#endif // ~ REPORT_BADNESS
#endif

//TODO: BADNESS_IF() ?
//TODO: SCOPED_BADNESS_IF( test() )   // report badness on e.g., stall

//--    types                           ///{{{2///////////////////////////////

struct DeclaredBadness
{
	EState const state;
	char const* const name;
	char const* const desc;
};

#if 0
struct Badness
{
	EState state;
	char const* const name;
	char const* const desc;
};
struct BadnessSource
{
	char const* const func;
	char const* const file;
	int line;
};
#endif

//--    functions                       ///{{{2///////////////////////////////

void badness(
	EState,
	char const* aName,
	char const* aDesc,
	char const* aSrcFile,
	char const* aSrcFunc,
	int aSrcLine,
	double aScore
) noexcept;

void declared_badness(
	DeclaredBadness const*, // Warning: must remain valid until process exits!
	double aScore
) noexcept;

#if 0
//XXX-NOTE: the Badness and BadnessSouce objects must remain valid until the
//process exits. Thus they cannot be automatic/stack allocations.
void badness( 
	Badness const&,
	BadnessSource const&,
	double aScore = 1.
) noexcept;
#endif

//--    <<< ~ flux::report namespace <<<        ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(report)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_BADNESS_HPP_CFA6230E_693E_4DA2_8453_1F8195E18CDB
