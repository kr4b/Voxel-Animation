/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Switch annotations implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/setup.pkg>
#include <flux/setup/compiler.hpp>
#include <flux/setup/langfeat.hpp>

#include <flux/std/assert.hpp>
#include <flux/util/intent.hpp>
#include <flux/pp/stringize.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    SWITCH_FALLTHROUGH          ///{{{1///////////////////////////////////

#if FLUX_LANGFEAT_ATTR_FALLTHROUGH
#	undef FLUX_SWITCH_FALLTHROUGH
#	define FLUX_SWITCH_FALLTHROUGH() [[fallthrough]]

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#	undef FLUX_SWITCH_FALLTHROUGH

	/* So ...  some clang versions seem to be funny. 3.5.0 generates warnings
	 * about fallthrough, but doesn't have `__has_cpp_attribute()`. It *does*
	 * however support [[clang::tallthrough]], and seems to advertise this via
	 * __has_attribute(fallthrough)). However, this requires C++11 attributes,
	 * so we also need to check for __has_extension/_has_feature(cxx_attributes).
	 */
#	if defined(__has_cpp_attribute) && __has_cpp_attribute(clang::fallthrough)
#		define FLUX_SWITCH_FALLTHROUGH() [[clang::fallthrough]]
#	else
		/* Check for "undocumented" [[clang::fallthrough]] */
#		if ((defined(__has_feature) && __has_feature(cxx_attributes)) || (defined(__has_extension) && __has_extension(cxx_attributes))) && defined(__has_attribute) && __has_attribute(fallthrough)
#			define FLUX_SWITCH_FALLTHROUGH() [[clang::fallthrough]]
#		else
#			define FLUX_SWITCH_FALLTHROUGH() /* not supported */
#		endif
#	endif // ~ has [[clang::fallthrough]]


#else // !LANGFEAT_ATTR_FALLTHROUGH
#	undef FLUX_SWITCH_FALLTHROUGH
#	define FLUX_SWITCH_FALLTHROUGH() /* not supported */

#endif // ~ LANGFEAT_ATTR_FALLTHROUGH

//--    SWITCH_UNREACHABLE          ///{{{1///////////////////////////////////

#if FLUX_DEBUG
#	undef FLUX_SWITCH_UNREACHABLE
#	define FLUX_SWITCH_UNREACHABLE(x) \
		case x: FLUX_ASSERT_TRIGGER( "Case: " FLUX_PP_STRINGIZE(x) \
			" declared unreachable" ); break \
		/*ENDM*/

#else // ! FLUX_DEBUG
#	undef FLUX_SWITCH_UNREACHABLE
#	define FLUX_SWITCH_UNREACHABLE(x) case x: FLUX_INTENT_UNREACHABLE()

#endif // ~ FLUX_DEBUG

//--    SWITCH_UNREACHABLE_DEFAULT()    ///{{{1///////////////////////////////

#if FLUX_DEBUG
#	undef FLUX_SWITCH_UNREACHABLE_DEFAULT
#	define FLUX_SWITCH_UNREACHABLE_DEFAULT() \
		default: FLUX_ASSERT_TRIGGER( "Default case declared unreachable" ); break \
		/*ENDM*/

#else // ! FLUX_DEBUG
#	undef FLUX_SWITCH_UNREACHABLE_DEFAULT
#	define FLUX_SWITCH_UNREACHABLE_DEFAULT() default: FLUX_INTENT_UNREACHABLE()

#endif // ~ FLUX_DEBUG

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
