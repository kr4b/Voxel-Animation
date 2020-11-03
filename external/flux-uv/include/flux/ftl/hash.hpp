/*-******************************************************* -- HEADER -{{{1- */
/*-	Hash
 *
 * Custom hash() function. `std::hash<>` is a bit icky; until C++17 it's not
 * really possible to test if a certain type is hashable or not (see e.g., 
 * http://stackoverflow.com/questions/12753997/check-if-type-is-hashable).
 *
 * ftl::hash() used to wrap std::hash(), but no longer does so (the reason is
 * partly differences between the standard libraries). Either way, ftl::hash()
 * works on most built-in types, and a few default types. The expecatation is
 * that other types will provide custom hash() functions that are available via
 * ADL.
 *
 * Note that hash() only guarantees the same result for the same input within
 * a single execution of the program (similar to the C++14 std::hash<>).
 *
 * WARNING: these are not cryptographic hashes, but rather hashes that may be
 * usable for hash tables and similar.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_HASH_HPP_50C046E7_F0A6_44AF_A1C6_2537BAB484A5
#define FLUX_FTL_HASH_HPP_50C046E7_F0A6_44AF_A1C6_2537BAB484A5

//--//////////////////////////////////////////////////////////////////////////
//--    includes            ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/std/enable_if.hpp>

#include <flux/compat/type_traits.hpp>

#include <string>
#include <type_traits>

//--    >>> namespace = flux::ftl >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    $ detail                        ///{{{2///////////////////////////////
namespace detail
{
	template< typename tType >
	struct Has_Std_Hash : std::false_type
	{};
}

//--    types                           ///{{{2///////////////////////////////

using Hash = std::size_t;

//--    hash()                          ///{{{2///////////////////////////////

/** Compute hash
 *
 * Computes hashes for the following types.
 *  - bool
 *  - char, signed char, unsigned char
 *  - wchar_t
 *  - signed/unsigned {short, int, long, long long}
 *  - float, double, long double
 *  - pointer types except `char*`, `char const*`
 *
 * \note `char*`, `char const*` and `std::string` have special overloads. 
 *
 * Note that `hash()` is not guaranteed to compute the same value across 
 * different executions of the program. `hash()` does <b>not</b> return the
 * same value as `std::hash<>` for these types.
 */
template< typename tType > 
FLUX_ENABLE_IF( detail::Has_Std_Hash< compat::decay_t<tType> > )
(Hash) hash( tType const& ) noexcept;

/** Compute hash of enum
 *
 * Computes the hash of the enum \a tType, by coverting the hash to its
 * underlying type and then invoking `hash()` on the result.
 */
template< typename tType >
FLUX_ENABLE_IF( std::is_enum<tType> )
(Hash) hash( tType const& ) noexcept;

/** Compute hash of a string
 *
 * Computes the hash of the nul-terminated string pointed to by the function's
 * argument. `hash()` inspects each byte; for large strings better hashes may
 * exist.
 */
Hash hash( char const* ) noexcept;
/** Compute hash of a string
 *
 * Computes the hash of \a std::string. `hash()` inspects each byte; for large
 * strings better hashes may exist.
 */
Hash hash( std::string const& ) noexcept;

//--    hash_combine()                  ///{{{2///////////////////////////////

constexpr
Hash hash_combine( Hash, Hash ) noexcept;

//--    <<< ~ flux::ftl namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/hash.inl"
#endif // FLUX_FTL_HASH_HPP_50C046E7_F0A6_44AF_A1C6_2537BAB484A5
