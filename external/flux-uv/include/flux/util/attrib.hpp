/*-******************************************************* -- HEADER -{{{1- */
/*-	Compiler attributes and hints.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_UTIL_ATTRIB_HPP_74E6AA5E_BF18_455E_8090_B50437188C11
#define FLUX_UTIL_ATTRIB_HPP_74E6AA5E_BF18_455E_8090_B50437188C11

//--//////////////////////////////////////////////////////////////////////////
//--    FLUX_DEPRECATED             ///{{{1///////////////////////////////////

/** Mark entity as deprecated
 *
 * Indicate that an entity is deprecated. Most compilers will emit warnings
 * when a deprecated entity is referenced.
 *
 * Examples:
   \code
   class FLUX_DEPRECATED OldClass {};
   FLUX_DEPRECATED int function();
   FLUX_DEPRECATED int gGlobal = 0;
   FLUX_DEPRECATED typedef Type OtherType;
   \endcode
 *
 * \warning `FLUX_DEPRECATED` pre-dates the standard `[[deprecated]]` 
 * attribute, and does not always automatically translate into it. In 
 * particular, under Visual Studio, when `FLUX_LANGQUIRK_PARTIAL_STDATTRIB` is
 * detected, `FLUX_DEPRECATED` will fall back to `__declspec(deprecated)`. As a
 * consequence, `FLUX_DEPRECATED` cannot be used in all contexts where 
 * `[[deprecated]`` is allowed. In particular, the following are uses are
 * <b>disallowed</b>:
   \code
   using OldAlias FLUX_DEPRECATED = void*; //ERROR in some VisualStudio 2017
   enum EEnum
   {
   	oldEnumerator FLUX_DEPRECATED, //ERROR in some VisualStudio 2017
   	oldEnumerator2 FLUX_DEPRECATED = 2 //ERROR in some VisualStudio 2017
   };
   namespace FLUX_DEPRCATED old_namespace //ERROR in some VisualStudio 2017
   {};
   \endcode
 */
#define FLUX_DEPRECATED                 /*OVERRIDDEN_ELSEWHERE*/
/** Mark entity as deprecated
 *
 * Indicate that an entity is deprecated. Most compilers will emit warnings
 * when a deprecated entity is referenced.
 *
 * Example:
   \code
   class FLUX_DEPRECATED_EX("OldClass is old") OldClass {};
   \endcode
 * 
 * See `FLUX_DEPRECATED` for additional information, examples and usage 
 * restrictions.
 */
#define FLUX_DEPRECATED_EX(msg)         FLUX_DEPRECATED /*OVERRIDDEN_ELSEWHERE*/

//--    attr: maybe unused          ///{{{1///////////////////////////////////

/** Mark variable as possibly-unused
 *
 * Used to suppress warnings from variables, arguments, typedefs and functions
 * that sometimes end up as unused due to e.g., conditional compilation. For
 * example:
   \code
   int foo( FLUX_ATTR_MAYBE_UNUSED void* aPtr ) {
   	FLUX_ASSERT( aPtr );
   	return 43;
   }
   \endcode
 *
 * \see FLUX_ATTR_UNUSED, FLUX_INTENT_UNUSED
 */
#define FLUX_ATTR_MAYBE_UNUSED           /*OVERRIDDEN_ELSEWHERE*/

//--    attr: nodiscard             ///{{{1///////////////////////////////////

/** Warn if return value is discarded/ignored
 *
 * Mark a function's return value as precious, i.e., ask the compiler to emit
 * a warning if it is discarded:
 * \code
 * FLUX_ATTR_NODISCARD void* allocate_expensive_resources();
 *
 * ...
 *
 * allocate_expensive_resources(); // Emit warning here.
 * \endcode
 *
 * \see FLUX_ATTR_NODISCARD_TYPE
 */
#define FLUX_ATTR_NODISCARD              /*OVERRIDDEN_ELSEWHERE*/
/** Warn if return values of this type are discarded/ignored
 *
 * Mark a class as a precious return value. This asks the compiler to always
 * emit warnings if return values of this type are discarded. The attribute
 * may be applied to classes and enums:
 * \code
 * class FLUX_ATTR_NODISCARD_TYPE AirlockState { ... };
 *
 * ...
 *
 * AirlockState airlock_close();
 * 
 * ...
 *
 * airlock_close(); // Emit warning here.
 * vent_atmosphere();
 * \endcode
 *
 * \see FLUX_ATTR_NODISCARD
 */
#define FLUX_ATTR_NODISCARD_TYPE         /*OVERRIDDEN_ELSEWHERE*/

//--    attr: extended              ///{{{1///////////////////////////////////

/** Mark variable as unused
 *
 * Used to suppress warnings from e.g., unused variables and arguments.
 *
 * Example:
   \code
   int foo FLUX_ATTR_UNUSED = 0;
   void f( int aX FLUX_ATTR_UNUSED ) {}
   \endcode
 *
 * \note Prefer `FLUX_INTENT_UNUSED` where possible, since that doesn't 
 * have to be inline in the declaration.
 *
 * \see FLUX_INTENT_UNUSED
 */
#define FLUX_ATTR_UNUSED                /*OVERRIDDEN_ELSEWHERE*/

/** Mark function as pure
 *
 * A pure function may not have any side effects and its results should only
 * depend on its arguments and on memory pointed to by its arguments. For
 * example, `strlen()` is a pure function - it only inspects the memory pointed
 * to by its argument, but nothing else.
 *
 * Example:
   \code
   FLUX_ATTR_PUREFN float sum( size_ aCount, float const* aElements );
   \endcode
 *
 * \note `FLUX_ATTR_PUREFN` is a bit more restrictive than GCC's
 * 	`__attribute__((pure))`. It corresponds to MSVC's `__declspec(noalias)`.
 *
 * \see FLUX_ATTR_CONSTFN
 */
#define FLUX_ATTR_PUREFN             /*OVERRIDDEN_ELSEWHERE*/
/** Mark function as constant
 *
 * A constant function may not have any side effects and may only inspect its
 * arguments (which must be passed by value). The function must not examine
 * global memory. For example, most elementary mathematical (`sin()`, ...)
 * functions are constant functions.
 *
 * Example:
   \code
   FLUX_ATTR_CONSTFN float reciprocal_sqrt( float aX ),
   \endcode
 *
 * \see FLUX_ATTR_PUREFN
 */
#define FLUX_ATTR_CONSTFN            /*OVERRIDDEN_ELSEWHERE*/

/** Mark a function to never return
 *
 * Functions that never return include, for example, `std::exit()` and
 * `std::abort()`. Functions that end up calling such a method unconditionally
 * should be marked themselves as not returning.
 *
 * Example:
   \code
   FLUX_ATTR_NORETURN void terminate_process_with_extreme_prejudice();
   \endcode
 */
#define FLUX_ATTR_NORETURN              /*OVERRIDDEN_ELSEWHERE*/

/** Mark a function's return value as precious
 *
 * DEPRECATED. See `FLUX_ATTR_NODISCARD`.
 * 
 * This may cause the compiler to emit diagnostics if the function's return
 * value is ignored.
 *
 * Example:
   \code
   FLUX_ATTR_PRECIOUS_RESULT void* allocate_huge_chunk_of_memory();
   \endcode
 */
#define FLUX_ATTR_PRECIOUS_RESULT       /*OVERRIDDEN_ELSEWHERE*/

/** Always inline function
 *
 * Tell compiler to always inline a function. Example:
   \code
   FLUX_ATTR_ALWAYS_INLINE void foo() { ... }
   \endcode
 * 
 * `FLUX_ATTR_ALWAYS_INLINE` implies `inline`.
 *
 * \note This uses e.g., `__attribute__((always_inline))` or `__forceinline`
 *   where available and falls back to a plain inline. Even when a compiler
 *   specific attribute is available, the compiler may *still* refuse to
 *   inline certain functions, as described on e.g.,
 *   https://msdn.microsoft.com/en-us/library/z8y1yy88.aspx
 */
#define FLUX_ATTR_ALWAYS_INLINE         inline /*OVERRIDDEN_ELSEWHERE*/

/** Returned pointer is not null
 *
 * Indicates that a function will return a pointer that may always be assumed
 * to be non-null:
   \code
   FLUX_ATTR_RET_NONNULL int* get_iptr() { ... }
  
   int* ip = get_iptr();
   if( ip )
   {
      ...
   }
   else
   {
      ...
   }
   \endcode
 * In this case, the compiler could safely remove the branch and discard the
 * `else`-side of the code, since `ip` is guaranteed to be non-null.
 *
 * \note This only works on raw pointers -- the attribute cannot be applied to
 * functions returning smart pointers (for obvious reasons).
 */
#define FLUX_ATTR_RET_NONNULL          /*OVERRIDE_ELSEWHERE*/
/** Argument pointer is not null
 *
 * Indicates that a function expects a (raw) pointer to be non-null:
   \code
   void copy_from_a_to_b( 
   	void* FLUX_ATTR_ARG_NONNULL b, 
	void const* FLUX_ATTR_ARG_NONNULL a 
   );
   \endcode
 * This enables compilers to emit warnings if they detect that null is passed 
 * to such a function. It also enables compilers to optimze further on the
 * assumption that an argument is never null
 *
 * \note This only works on raw pointers -- the attribute cannot be applied to
 * functions returning smart pointers (for obvious reasons).
 *
 * Use of the `flux::std::not_null<>` (and `flux::std::not_null_unsafe<>`)
 * wrapper should, in general, be preferred over the `FLUX_ATTR_RET_NONNULL`
 * attribute.
 *
 * \warning Specifying this attribute <b>will allow</b> the compiler to assume
 * that arguments are <b>not null</b> while compiling and optimizing the code
 * in question. <b>This means that the compiler may elide explicit null-checks
 * from the code!</b>
 *
 * \see flux::std::not_null
 */
#define FLUX_ATTR_ARG_NONNULL          /*OVERRIDE_ELSEWHERE*/

/** Atttribute: Type may alias
 *
 * The GCC documentation states:
 * > Access to objects with types with this attribute are not subject to type-
 * > based alias analysis, but are instead assumed to be abe to alias any other
 * > type of objects, just like the `char` type.
 *
 * `FLUX_ATTR_MAY_ALIAS` is applied to types:
   \code
   struct FLUX_ATTR_MAY_ALIAS StructType
   {
   	...
   };
  
   using Alias_ = int FLUX_ATTR_MAY_ALIAS;
   \endcode
 */
#define FLUX_ATTR_MAY_ALIAS            /*OVERRIDDEN_ELSEWHERE*/

/** Clang's `trivial_abi` attribute
 *
 * Clang's `trivial_abi` attribute declares a type as "trivial", even if does
 * not technically fullfil the requirements. Doing so may avoid some overheads
 * related to caller-cleanup requirements by the Itanium ABI, by essentially
 * enabling callee-cleanup behaviour for these types. (This also introduces
 * some subtle changes with respect to lifetime.) See e.g., [`[[trivial_abi]]`
 * 101](https://quuxplusone.github.io/blog/2018/05/02/trivial-abi-101/).
 *
 * Only works on Clang at the moment. However, would have less of an effect on
 * Windows, where callee-cleanup is already the default.
 *
 * `FLUX_ATTR_TRIVIAL_ABI` is applied to type definitions/declarations (but 
 * cannot be used on type aliases, since it changes the semantics of the type):
   \code
   struct FLUX_ATTR_TRIVIAL_ABI StructType
   {
   	...
   };
   \endcode
 *
 * \note `FLUX_ATTR_TRIVIAL_ABI` is ignored on classes that have any base
 * classes or non-static data members that are non-trivial. It causes no error
 * or diagnostic to do so.
 */
#define FLUX_ATTR_TRIVIAL_ABI          /*OVERRIDDEN_ELSEWHERE*/

//--    attr: CUDA                  ///{{{1///////////////////////////////////

/** Make function callable from CUDA device code
 *
 * Marks a function as `__device__ __host__` when compiled under NVCC, does
 * nothing. This enables the function to be called from other CUDA `__device__`
 * and `__global__` methods.
 *
 * \note For a pure `__device__` or a `__kernel__` method, use a `#%ifdef`.
 *    These functions could confuse compilers other than NVCC due to use
 *    of special built-ins, and are not really usable from "normal" C++
 *    anyway.
 */
#define FLUX_ATTR_CUDA_DEVICEFN         /*OVERRIDDEN_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/attrib.inl"
#endif // FLUX_UTIL_ATTRIB_HPP_74E6AA5E_BF18_455E_8090_B50437188C11
