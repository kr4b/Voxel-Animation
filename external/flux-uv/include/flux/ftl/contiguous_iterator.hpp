/*-******************************************************* -- HEADER -{{{1- */
/*-	Helpers for contiguous iterators
 *
 * Attempts at detecting contiguous iterators and helpers to convert these into
 * pointers. Some dragons ahead -- detection relies on platform-specific 
 * details.
 *
 * See FLUXCFG_FTL_FRAGILE_CONTIGUOUS_DETECTION
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_CONTIGUOUS_ITERATOR_HPP_D55D16B8_4046_4A9C_B820_7353BA79F1A6
#define FLUX_FTL_CONTIGUOUS_ITERATOR_HPP_D55D16B8_4046_4A9C_B820_7353BA79F1A6

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/enable_if.hpp>

#include <type_traits>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    is_contiguous_iterator<>            ///{{{2///////////////////////////

/** \brief Trait: identify contiguous iterators.
 *
 * `is_contiguous_iterator<>` may be used to identify if \a typename is a 
 * contiguous iterator. Contiguous iterators are random access iterators that
 * return, when iterated, elements placed in a contiguous block of memory.
 * Thus, the element `*(it+1)` is found at `*(std::addressof(*it)+1)`.
 *
 * Raw pointers are always considered to be contiguous iterators. Iterators
 * originating from certain containers (e.g., `std::array`, `std::string` and
 * `std::vector`) are considered contiguous for a subset of standard library
 * implementations, as follows:
 * 
 *   - `STDLIB_STDCPP`: some containers (`std::array`) just use raw pointers;
 *   others return a `__gnu_cxx::__normal_iterator` which is detected. If
 *   `_GLIBCXX_DEBUG` is enabled, non-pointer iterators will be considered 
 *   <b>non-contiguous</b>.
 *   - `STDLIB_LIBCPP`: contiguous containers return either raw pointers or 
 *   `std::__wrap_iter`. These are detected and considered contiguous.
 *   - `STDLIB_VSCPP`: each container returns a custom type of iterator; the
 *   iterators from `std::string`, `std::array` and `std::vector` are
 *   recognized and considered contiguous (regardless of the setting of
 *   `_ITERATOR_DEBUG_LEVEL`.)
 *
 * \note If `FLUXCFG_FTL_FRAGILE_CONTIGUOUS_DETECTION` is disabled, then only
 * pointers will be considered to be contiguous iterators.
 *
 * See `#to_pointer()` for converting contiguous iterators to pointers.
 * `to_pointer()` handles `end()` iterators (which may not be dereferenced).
 */
template< typename >
struct is_contiguous_iterator
	: std::false_type
{};

//--    contiguous_iterator_pointer<>       ///{{{2///////////////////////////

/** \brief Meta-function: get an iterator's corresponding pointer type
 *
 * Gets the iterator \a typename's corresponding pointer type. This rougly
 * matches `std::add_pointer< typename std::iterator_traits<It>::%value_type >`
 * if the type `It` is a contiguous iterator. `contiguous_iterator_pointer` is
 * incomplete for non--contiguous-iterator types.
 */
template< typename >
struct contiguous_iterator_pointer;

/** \copydoc contiguous_iterator_pointer */
template< typename tIt >
using contiguous_iterator_pointer_t = typename contiguous_iterator_pointer<tIt>::type;

//--    to_pointer()                        ///{{{2///////////////////////////
/** \brief Convert contiguous iterator to pointer
 *
 * Converts the contiguous iterator \a tIt to a raw pointer. It is permissible
 * to call `to_pointer()` on an `end()`-iterator (but the returned pointer may
 * not be dereferenced).
 *
 * \note The implementation relies on `std::addressof(*it)` by default. This
 * seems to work unless iterator debugging is enabled (which is handled
 * separately). Ensure that the unit tests have been run to verify this!
 */
template< typename tIt >
contiguous_iterator_pointer_t<tIt> to_pointer( tIt const& ) noexcept;

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/contiguous_iterator.inl"
#endif // FLUX_FTL_CONTIGUOUS_ITERATOR_HPP_D55D16B8_4046_4A9C_B820_7353BA79F1A6
