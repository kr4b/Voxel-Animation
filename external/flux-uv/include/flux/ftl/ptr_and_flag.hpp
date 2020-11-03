/*-******************************************************* -- HEADER -{{{1- */
/*-	Pointer and a boolean flag
 *
 * Class that holds a (raw) pointer and a flag. The flag is embedded in the
 * pointer if alignment makes such an embedding possible.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_PTR_AND_FLAG_HPP_134B9302_F7ED_4B35_B484_7A3AA3E4ABE0
#define FLUX_FTL_PTR_AND_FLAG_HPP_134B9302_F7ED_4B35_B484_7A3AA3E4ABE0

//--//////////////////////////////////////////////////////////////////////////
//--    includes            ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/setup/stdfeat.hpp>
#include <flux/std/enable_if.hpp>
#include <flux/ftl/meta_bool.hpp>

#include <flux/compat/constexpr.hpp>
#include <flux/compat/type_traits.hpp>

#if FLUX_STDQUIRK_CXX11_NO_TRIVIALLY_TRAITS
#	include <flux/ftl/is_bitwise.hpp>
#endif // ~ NO_TRIVIALLY_TRAITS

#include <cstddef>
#include <utility>
#include <type_traits>

//--    >>> namespace = flux::ftl >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    PtrAndFlag                      ///{{{2///////////////////////////////
namespace detail
{
	template< typename, bool >
	class PtrAndFlagImpl;

	template< typename tPtr, std::size_t tAlign >
	struct Embed_Flag_In_Ptr;
	template< typename tPtr >
	struct Ptr_Type_Alignment; // handles void*; gives it alignment = 1
}

#if !defined(__DOXYGEN__)
template< 
	typename tPtr, 
	std::size_t = detail::Ptr_Type_Alignment<compat::remove_cv_t<tPtr>>::value >
struct PtrAndFlag;
#endif

/** Pointer and flag
 *
 * A pointer-and-flag pair. Attempts to embed the flag in the LSB of the
 * pointer, should alignment requirements allow for such (i.e., if alignment
 * greater than two bytes is required by the pointed-to type of \a tPtr, the 
 * LSB of the pointers is guaranteed to be zero in any valid pointer).
 *
 * For one-byte aligned pointers (and void-pointers), the flag is stored as a
 * separate bool field.
 *
 * \note Only supports raw pointers. `PtrAndFlag` are `bitwise_everything`,
 * just like raw pointers.
 */
template< typename tType, std::size_t tAlign >
struct PtrAndFlag<tType*, tAlign>
	: detail::PtrAndFlagImpl<tType*, 
		detail::Embed_Flag_In_Ptr<tType*,tAlign>::value
	>
{
	using Pointer = tType*;
	using element_type = tType;
	
	PtrAndFlag() = default;

	PtrAndFlag( Pointer, bool = false ) noexcept;
	PtrAndFlag( std::nullptr_t, bool = false ) noexcept;

	/* Public API, implemented in PtrAndFlagImpl<>:
	 */
#	if defined(__DOXYGEN__)
	/** Return current value of the flag
	 *
	 * \see set_flag()
	 */
	bool flag() const noexcept;
	/** Return pointer
	 *
	 * \see set_pointer()
	 */
	Pointer pointer() const noexcept;

	/** Set flag
	 *
	 * \see flag()
	 */
	void set_flag( bool ) noexcept;
	/** Set pointer
	 *
	 * \see pointer()
	 */
	void set_pointer( Pointer ) noexcept;
	/** Set pointer to `nullptr`
	 *
	 * \see pointer()
	 */
	void set_pointer( std::nullptr_t ) noexcept;
#	endif // ~ __DOXYGEN__
};

/** Pointer and flag, const specialization
 * 
 * See `PtrAndFlag<tType*,tAlign>` for non-constant pointers.
 */
template< typename tType, std::size_t tAlign >
struct PtrAndFlag<tType* const, tAlign >
	: detail::PtrAndFlagImpl< tType* const, 
		detail::Embed_Flag_In_Ptr<tType*,tAlign>::value
	>
{
	using Pointer = tType* const;
	using element_type = tType;
	
	PtrAndFlag() = default;

	PtrAndFlag( Pointer, bool = false ) noexcept;
	PtrAndFlag( std::nullptr_t, bool = false ) noexcept;
};

template< typename tType >
struct PtrAndFlag< tType*, 0 >;

//--    functions                       ///{{{2///////////////////////////////

template< typename tType, typename tPtr, std::size_t tAlign > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( std::is_same<compat::remove_cv_t<tType>,bool> )
(bool) get( PtrAndFlag<tPtr,tAlign> const& ) noexcept;
template< typename tType, typename tPtr, std::size_t tAlign > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( meta::All< std::is_convertible<tPtr,tType>, meta::Not<std::is_same<compat::remove_cv_t<tType>,bool>> > )
(tType) get( PtrAndFlag<tPtr,tAlign> const& ) noexcept;

// TODO: modifiable versions with a proxy?

template< typename tType, std::size_t tSize > FLUX_CONSTEXPR_EX
void swap( PtrAndFlag<tType,tSize>&, PtrAndFlag<tType,tSize>& ) noexcept;

//--    is_bitwise_*                    ///{{{2///////////////////////////////
#if FLUX_STDQUIRK_CXX11_NO_TRIVIALLY_TRAITS
/* is_bitwise_*<> rely on (among others) std::is_trivially_*<>; when the latter
 * are missing, PtrAndFlag<> may not be correctly identified. Manually override
 * the values in such a case.
 */
template< typename tPtr, std::size_t tAlign >
std::true_type tag_is_bitwise_conjurable( PtrAndFlag<tPtr,tAlign> const& );
template< typename tPtr, std::size_t tAlign >
std::true_type tag_is_bitwise_awakeable( PtrAndFlag<tPtr,tAlign> const& );
template< typename tPtr, std::size_t tAlign >
std::true_type tag_is_bitwise_duplicable( PtrAndFlag<tPtr,tAlign> const& );
template< typename tPtr, std::size_t tAlign >
std::true_type tag_is_bitwise_relocatable( PtrAndFlag<tPtr,tAlign> const& );
template< typename tPtr, std::size_t tAlign >
std::true_type tag_is_bitwise_dismissible( PtrAndFlag<tPtr,tAlign> const& );
#endif // ~ NO_TRIVIALLY_TRAITS

//--    <<< ~ flux::ftl namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/ptr_and_flag.inl"
#endif // FLUX_FTL_PTR_AND_FLAG_HPP_134B9302_F7ED_4B35_B484_7A3AA3E4ABE0
