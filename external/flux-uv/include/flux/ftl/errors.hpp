/*-******************************************************* -- HEADER -{{{1- */
/*-	FTL errors
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_ERRORS_HPP_AC23A7AA_4A92_4CFA_A7AA_95223B8A58BA
#define FLUX_FTL_ERRORS_HPP_AC23A7AA_4A92_4CFA_A7AA_95223B8A58BA

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/std/stdexcept.hpp>

//--    >>> namespace = flux::ftl::error >>>        ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,error)
//--    errors - AnyT<>                     ///{{{2///////////////////////////
/** Error: mismatching types in `Any`
 *
 * `AnyTypeMismatch` is thrown when attempting to retrieve a value held by an
 * `AnyT` instance with a type different from the held value's original type.
 * For example:
   \code
   Any x( 55 ); // type = int
   float f = x.get<float>(); // throws AnyTypeMismatch.
   \endcode
 */
struct AnyTypeMismatch : flux::error::LogicError
{};

/** Error: incompatible types in `Any`
 *
 * `AnyTypeIncompatible` is thrown when attempting to copy an `Any` to a
 * different `AnyT` with more features, but without the copied value supporting
 * the additional features. For example:
   \code
   struct NotComparable { ... };
  
   Any x = Any( NotComparable() );

   using Comp_ = meta::make_set< aspect::AnyFeatEqual >;
   AnyT<Comp_> y( x ); // throws AnyTypeIncompatible
   \endcode
 */
struct AnyTypeIncompatible : flux::error::LogicError
{};


//--    errors - IdPool<>                   ///{{{2///////////////////////////
/** Error: all possible IDs allocated
 *
 * No further IDs could be allocated from `IdPool` as the maximum number
 * of allocated IDs has been reached.
 *
 * Thrown by `IdPool::alloc()` & `IdPool::alloc_range()`.
 */
struct IdPoolExhausted : flux::error::RuntimeError
{};

/** Error: `IdPool` memory limit reached
 *
 * The internal state of a `IdPool` reached a memory limit by either
 *   - exceeding the size defined by `limits::kMaxIdAllocBytes`
 *   - memory allocation failing (`realloc()` returned `NULL`)
 *
 * Thrown by `IdPool::release()` and `IdPool::release_range()`.
 *
 * \note The memory limit is largely unrelated to the number of IDs allocated
 * and more to the fragmentation of the allocated IDs. `IdPool<>` uses
 * \f$2*\mbox{sizeof(IdPool::IntType)}\f$ bytes for each *compact* range of 
 * freeIDs.
 */
struct IdPoolMemLimit : flux::error::RuntimeError
{};

/** Error: attempt to free unallocated ID
 *
 * One of the IDs passed to `IdPool::release()` or `IdPool::release_range()`
 * was not currently allocated (never before allocated, already released, or
 * outside of the range of valid IDs of the associated `IdPool`).
 *
 * Thrown by `IdPool::release()` and `IdPool::release_range()`.
 */
struct IdPoolNotAllocated : flux::error::LogicError
{};

//--    errors - Catalog                    ///{{{2///////////////////////////

struct CatalogKeyInvalid : flux::error::RuntimeError
{};

struct CatalogKeyRetained : flux::error::RuntimeError
{};

//--    <<< ~ flux::ftl::error namespace <<<        ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,error)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_FTL_ERRORS_HPP_AC23A7AA_4A92_4CFA_A7AA_95223B8A58BA
