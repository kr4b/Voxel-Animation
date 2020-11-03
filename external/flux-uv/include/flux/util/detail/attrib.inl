/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Attribute implementations
 */
/*-***************************************************************** -}}}1- */

#include <flux/setup.pkg>
#include <flux/setup/compiler.hpp>
#include <flux/setup/langfeat.hpp>

#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC && _MSC_VER >= 1600 /* VS 2010+ */
	/* We need <sal.h> for Visual Studio source annotations, i.e., the likes
	 * of _Check_return, _Ret_notnull_ and so on. We also need to specify 
	 * "/analyze" to the compiler, as these will be ignored otherwise.
	 */
#	include <sal.h>
#endif // ~ MSVC 2010+


//--//////////////////////////////////////////////////////////////////////////
//--    DEPRECATED                  ///{{{1///////////////////////////////////

#if !FLUX_LANGQUIRK_PARTIAL_STDATTRIB
#	undef FLUX_DEPRECATED
#	define FLUX_DEPRECATED [[deprecated]]

#	undef FLUX_DEPRECATED_EX
#	define FLUX_DEPRECATED_EX(msg) [[deprecated(msg)]]

#else // LANGQUIRK_PARTIAL_STDATTRIB
#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#		undef FLUX_DEPRECATED
#		define FLUX_DEPRECATED __declspec(deprecated)

#		undef FLUX_DEPRECATED_EX
#		define FLUX_DEPRECATED_EX(msg) __declspec(deprecated)
#	else // ~ other compilers
#		error "TODO: handle LANGQUIRK_PARTIAL_STDATTRIB"
#	endif // ~ COMPILER_FAMILY
#endif // ~ LANGQUIRK_PARTIAL_STDATTRIB

//--    ATTR_MAYBE_UNUSED           ///{{{1///////////////////////////////////

#if FLUX_LANGFEAT_ATTR_MAYBE_UNUSED
#	undef FLUX_ATTR_MAYBE_UNUSED
#	define FLUX_ATTR_MAYBE_UNUSED [[maybe_unused]]

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#	undef FLUX_ATTR_MAYBE_UNUSED
#	define FLUX_ATTR_MAYBE_UNUSED __attribute__((unused))

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#	undef FLUX_ATTR_MAYBE_UNUSED

#	if defined(__has_cpp_attribute)
#		if __has_cpp_attribute(gnu::unused)
#			define FLUX_ATTR_MAYBE_UNUSED [[gnu::unused]]
#		elif __has_cpp_attribute(unused)
#			define FLUX_ATTR_MAYBE_UNUSED [[unused]]
#		endif // ~ C++11 attributes
#	endif // ~ defined(__has_cpp_attribute)

#	if !defined(FLUX_ATTR_MAYBE_UNUSED)
#		define FLUX_ATTR_MAYBE_UNUSED __attribute__((unused))
#	endif // ~ fallback

/* NOTE: no Visual Studio-specific fallback */

#endif // ~ implementations


//--    ATTR_NODISCARD*             ///{{{1///////////////////////////////////

#if FLUX_LANGFEAT_ATTR_NODISCARD
#	undef FLUX_ATTR_NODISCARD
#	define FLUX_ATTR_NODISCARD [[nodiscard]]

#	undef FLUX_ATTR_NODISCARD_TYPE
#	define FLUX_ATTR_NODISCARD_TYPE [[nodiscard]]

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#	undef FLUX_ATTR_NODISCARD
#	define FLUX_ATTR_NODISCARD __attribute__((__warn_unused_result__))

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#	undef FLUX_ATTR_NODISCARD

#	if defined(__has_cpp_attribute)
#		if __has_cpp_attribute(clang::warn_unused_result)
#			define FLUX_ATTR_NODISCARD [[clang::warn_unused_result]]
#		elif __has_cpp_attribute(warn_unused_result)
#			define FLUX_ATTR_NODISCARD [[warn_unused_result]]
#		endif // ~ C++11 attributes
#	endif // ~ defined(__has_cpp_attribute)

#	if !defined(FLUX_ATTR_NODISCARD)
#		define FLUX_ATTR_NODISCARD __attribute__((warn_unused_result))
#	endif // ~ fallback

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#	if _MSC_VER >= 1700 // Visual Studio 2012
	/* Can choose between
	 *   - _Must_inspect_result_
	 *   - _Check_return_
	 *
	 * The former is more strict. For example, storing the return value in
	 * a variable that is not used further will still produce a warning. The
	 * latter (_Check_return_) is however satisfied by that. Either can be
	 * manually cast to (void), which will suppress warnings in both cases.
	 *
	 * I'm sticking to the more annoying one for now. :-)
	 *
	 * Both require <sal.h>, and the "/analyze" compiler command line option.
	 */
#		undef FLUX_ATTR_NODISCARD
#		define FLUX_ATTR_NODISCARD _Must_inspect_result_
#	endif // ~ Visualt Studio 2012

#endif // ~ implementations


//--    GCC                         ///{{{1///////////////////////////////////
#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#	undef FLUX_ATTR_UNUSED
#	define FLUX_ATTR_UNUSED __attribute__((unused))

#	undef FLUX_ATTR_PUREFN
#	define FLUX_ATTR_PUREFN __attribute__((pure))

#	undef FLUX_ATTR_CONSTFN
#	define FLUX_ATTR_CONSTFN __attribute__((const))

#	undef FLUX_ATTR_NORETURN 
#	define FLUX_ATTR_NORETURN __attribute__((noreturn))

#	undef FLUX_ATTR_PRECIOUS_RESULT
#	define FLUX_ATTR_PRECIOUS_RESULT __attribute__((__warn_unused_result__))

#	undef FLUX_ATTR_ALWAYS_INLINE
#	define FLUX_ATTR_ALWAYS_INLINE __attribute__((always_inline)) inline

	/* GCC 4.8 and earlier ignore this attribute (and warn about doing so if
	 * it is placed in front of the function declaration. So, only support
	 * this attribute on GCC 4.9 and later.
	 */
#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9)
#		undef FLUX_ATTR_RET_NONNULL
#		define FLUX_ATTR_RET_NONNULL __attribute__((returns_nonnull))
#	endif // ~ GCC 4.9 and newer

	/* GCC's __attribute__((nonnull)) binds to the function, and optionally
	 * specifies parameters by numeric index. That's not the way we want to
	 * use the attribute, though.
	 */

#	undef FLUX_ATTR_MAY_ALIAS
#	define FLUX_ATTR_MAY_ALIAS __attribute__((__may_alias__))

#endif // ~ FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC

//--    Clang                       ///{{{1///////////////////////////////////
#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#	undef FLUX_ATTR_UNUSED
#	define FLUX_ATTR_UNUSED __attribute__((unused))

#	undef FLUX_ATTR_PUREFN
#	define FLUX_ATTR_PUREFN __attribute__((pure))

#	undef FLUX_ATTR_CONSTFN
#	define FLUX_ATTR_CONSTFN __attribute__((const))

#	undef FLUX_ATTR_NORETURN 
#	define FLUX_ATTR_NORETURN __attribute__((noreturn))

#	undef FLUX_ATTR_PRECIOUS_RESULT
#	define FLUX_ATTR_PRECIOUS_RESULT __attribute__((__warn_unused_result__))

#	undef FLUX_ATTR_ALWAYS_INLINE
#	define FLUX_ATTR_ALWAYS_INLINE __attribute__((always_inline)) inline

#	undef FLUX_ATTR_RET_NONNULL
#	define FLUX_ATTR_RET_NONNULL __attribute__((returns_nonnull))

#	undef FLUX_ATTR_ARG_NONNULL
#	define FLUX_ATTR_ARG_NONNULL __attribute__((nonnull))

#	undef FLUX_ATTR_TRIVIAL_ABI
#	define FLUX_ATTR_TRIVIAL_ABI __attribute__((trivial_abi))

#endif // ~ FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC

//--    MSVC                        ///{{{1///////////////////////////////////
#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
// missing: FLUX_ATTR_UNUSED

#	undef FLUX_ATTR_PUREFN
#	define FLUX_ATTR_PUREFN __declspec(noalias)

#	undef FLUX_ATTR_CONSTFN
#	define FLUX_ATTR_CONSTFN FLUX_ATTR_PUREFN

#	undef FLUX_ATTR_NORETURN
#	define FLUX_ATTR_NORETURN __declspec(noreturn)

#	if _MSC_VER >= 1600 /* Visual Studio 2010+ */
	/* For this to have any effect, we maybe need the /analyze command line
	 * option too.
	 *
	 * See
	 *  - http://msdn.microsoft.com/en-us/library/hh916383.aspx
	 *  - http://msdn.microsoft.com/en-us/library/jj159529.aspx
	 *
	 *  NOTE: requires <sal.h> to compile, and "/analyze" to actually matter
	 *  during compilation.
	 */
#		undef FLUX_ATTR_PRECIOUS_RESULT
#		undef FLUX_ATTR_RET_NONNULL

#		define FLUX_ATTR_PRECIOUS_RESULT _Check_return_
#		define FLUX_ATTR_RET_NONNULL _Ret_notnull_
#	endif // ~ _MSC_VER

#	undef FLUX_ATTR_ALWAYS_INLINE
#	define FLUX_ATTR_ALWAYS_INLINE __forceinline

#endif // ~ FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC

//--    NVCC                        ///{{{1///////////////////////////////////
#if FLUX_COMPILER_ACTUAL == FLUX_COMPILER_NVCC
#	undef FLUX_ATTR_CUDA_DEVICEFN
#	define FLUX_ATTR_CUDA_DEVICEFN __device__ __host__

#endif // ~ FLUX_COMPILER_ACTUAL == FLUX_COMPILER_NVCC

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
