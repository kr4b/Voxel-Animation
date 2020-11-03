/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- uninitialialized_*() implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/ftl/defaults.hpp>
#include <flux/compat/to_address.hpp>

#if FLUXCFG_FTL_FRAGILE_CONTIGUOUS_DETECTION
#	include <flux/setup/platform.hpp>

#	if FLUX_STDLIB == FLUX_STDLIB_STDCPP
#		include <vector> // make sure __normal_iterator is defined
#	elif FLUX_STDLIB == FLUX_STDLIB_LIBCPP
#		include <vector> // make sure __wrap_iterator is defined
#	elif FLUX_STDLIB == FLUX_STDLIB_VSCPP
#		include <array> // make sure _Array*_iterator is defined
#		include <string> // make sure _String*_iterator is defined
#		include <vector> // make sure _Vector*_iterator is defined
#	endif // ~ STDLIB
#endif // ~ CFG_FTL_FRAGILE_CONTIGUOUS_DETECTION

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    is_contiguous_iterator<>            ///{{{2///////////////////////////

/* Pointers are contiguous iterators.
 */
template< typename tType >
struct is_contiguous_iterator< tType* >
	: std::true_type
{};

/* "Real" iterators are a bit more tricky. AFAIK there's no really good way to
 * detect these at the moment (the "contiguous_iterator_tag" never made it into
 * the standard).
 *
 * The current code attempts to enumerate known contiguous iterators from the
 * various standard libraries. This is (of course) rather fragile.
 */
#if FLUXCFG_FTL_FRAGILE_CONTIGUOUS_DETECTION
#	if FLUX_STDLIB == FLUX_STDLIB_STDCPP
	/* libstdc++ uses __gnu_cxx::__normal_iterator<> for the std::vector and
	 * std::string classes. std::array (and std::valarray) use raw pointers 
	 * from the get-go.
	 */
	template< typename tType, class tContainer >
	struct is_contiguous_iterator< __gnu_cxx::__normal_iterator<tType*,tContainer> >
		: std::true_type
	{};

#	elif FLUX_STDLIB == FLUX_STDLIB_LIBCPP
	/* libstdc++ uses std::__wrap_iter<> for std::vector, std::string. The
	 * std::array uses raw pointers from the get.go.
	 */
	template< typename tType >
	struct is_contiguous_iterator< std::__wrap_iter<tType*> >
		: std::true_type
	{};

#	elif FLUX_STDLIB == FLUX_STDLIB_VSCPP
	/* The VS standard library uses std::_Vector[_const]_iterator<> with a lot
	 * of nested types for std::vector<>. Similar exist for std::string and
	 * std::array.
	 */
	template< typename tType, std::size_t tSize >
	struct is_contiguous_iterator< std::_Array_iterator<tType,tSize> >
		: std::true_type
	{};
	template< typename tType, std::size_t tSize >
	struct is_contiguous_iterator< std::_Array_const_iterator<tType,tSize> >
		: std::true_type
	{};

	template< class tWhatever >
	struct is_contiguous_iterator< std::_String_iterator< tWhatever > >
		: std::true_type
	{};
	template< class tWhatever >
	struct is_contiguous_iterator< std::_String_const_iterator< tWhatever > >
		: std::true_type
	{};

	template< class tWhatever >
	struct is_contiguous_iterator< std::_Vector_iterator< tWhatever > >
		: std::true_type
	{};
	template< class tWhatever >
	struct is_contiguous_iterator< std::_Vector_const_iterator< tWhatever > >
		: std::true_type
	{};

#	endif // ~ STDLIB
#endif // ~ CFG_FTL_FRAGILE_CONTIGUOUS_DETECTION

//--    contiguous_iterator_pointer<>       ///{{{2///////////////////////////

template< typename tType >
struct contiguous_iterator_pointer< tType* >
{
	using type = tType*;
};

#if FLUXCFG_FTL_FRAGILE_CONTIGUOUS_DETECTION
#	if FLUX_STDLIB == FLUX_STDLIB_STDCPP
	template< typename tType, class tContainer >
	struct contiguous_iterator_pointer< __gnu_cxx::__normal_iterator<tType*,tContainer> >
	{
		using type = tType*;
	};

#	elif FLUX_STDLIB == FLUX_STDLIB_LIBCPP
	template< typename tType >
	struct contiguous_iterator_pointer< std::__wrap_iter<tType*> >
	{
		using type = tType*;
	};

#	elif FLUX_STDLIB == FLUX_STDLIB_VSCPP
	template< typename tType, std::size_t tSize >
	struct contiguous_iterator_pointer< std::_Array_iterator<tType,tSize> >
	{
		using type = tType*;
	};
	template< typename tType, std::size_t tSize >
	struct contiguous_iterator_pointer< std::_Array_const_iterator<tType,tSize> >
	{
		using type = tType const*;
	};

	template< class tWhatever >
	struct contiguous_iterator_pointer< std::_String_iterator< tWhatever > >
	{
		using Iter_ = std::_String_iterator<tWhatever>;
		//using type = decltype(std::addressof(*std::declval<Iter_>()));
		using type = typename std::iterator_traits<Iter_>::value_type*;
	};
	template< class tWhatever >
	struct contiguous_iterator_pointer< std::_String_const_iterator< tWhatever > >
	{
		using Iter_ = std::_String_const_iterator<tWhatever>;
		using type = typename std::iterator_traits<Iter_>::value_type const*;
	};

	template< class tWhatever >
	struct contiguous_iterator_pointer< std::_Vector_iterator< tWhatever > >
	{
		using Iter_ = std::_Vector_iterator<tWhatever>;
		using type = typename std::iterator_traits<Iter_>::value_type*;
	};
	template< class tWhatever >
	struct contiguous_iterator_pointer< std::_Vector_const_iterator< tWhatever > >
	{
		using Iter_ = std::_Vector_const_iterator<tWhatever>;
		using type = typename std::iterator_traits<Iter_>::value_type const*;
	};
#	endif // ~ STDLIB
#endif // ~ CFG_FTL_FRAGILE_CONTIGUOUS_DETECTION

//--    to_pointer()                        ///{{{2///////////////////////////
namespace detail
{
#	if FLUX_STDLIB == FLUX_STDLIB_VSCPP && _ITERATOR_DEBUG_LEVEL > 0
	template< typename tIter > inline
	contiguous_iterator_pointer_t<tIter> vs_iter_ptr_( tIter const& aIt ) noexcept
	{
#		if _MSC_VER < 1915
		// Yeah, this is really really really fishy. (But _Ptr is public, 
		// so it's not technically illegal? Right?)
		return const_cast<contiguous_iterator_pointer_t<tIter>>(aIt._Ptr);
#		else // _MSC_VER >= 1915
		// They made it private. They also changed the interface, so that's
		// good. This is probably still fishy, though.
		return const_cast<contiguous_iterator_pointer_t<tIter>>(aIt._Unwrapped());
#		endif
	}

	template< typename tType > inline
	tType* vs_iter_ptr_( tType* aPtr ) noexcept
	{
		return aPtr;
	}

#		if _MSC_VER <= 1800 // VS2013
	/* VS2013 stores the std::array iterators differently when iterator debugging
	 * is enabled. The array's base address is then stored in _Ptr, and the 
	 * current element's index in _Idx.
	 */
	template< typename tType, std::size_t tSize > inline
	tType* vs_iter_ptr_( std::_Array_iterator<tType,tSize> const& aIt ) noexcept
	{
		return const_cast<tType*>(aIt._Ptr + aIt._Idx);
	}
	template< typename tType, std::size_t tSize > inline
	tType const* vs_iter_ptr_( std::_Array_const_iterator<tType,tSize> const& aIt ) noexcept
	{
		return aIt._Ptr + aIt._Idx;
	}
#		endif // ~ VS2013
#	endif // ~ STDLIB_VSCPP
}

template< typename tIt > inline
contiguous_iterator_pointer_t<tIt> to_pointer( tIt const& aIt ) noexcept
{
#	if FLUX_STDLIB == FLUX_STDLIB_VSCPP
	/* Deal with checked iterators. We can't dereference end(), since that 
	 * aborts at runtime. We can't do begin()-1, because that too aborts at
	 * runtime.
	 */
#		if _ITERATOR_DEBUG_LEVEL > 0 
	return detail::vs_iter_ptr_(aIt);
#		else // iterator debugging disabled
	return compat::to_address(aIt); // addressof(*aIt)
#		endif

#	else // !STDLIB_VSCPP
	//TODO: check _GLIBCXX_DEBUG; see
	// https://gcc.gnu.org/onlinedocs/libstdc++/manual/debug_mode_using.html
	return compat::to_address(aIt); // addressof(*aIt)
#	endif // ~ STDLIB
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
