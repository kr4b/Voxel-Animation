/*-******************************************************* -- HEADER -{{{1- */
/*-	Vector<>
 *
 * A vector (container, not math) that implements small-vector optimization via
 * a buffer embedded directly into the vector instance.
 *
 * Vector<> is inspired from several sources. 
 *  - CPPCon 2016 talk: TODO-FIXME-link to talk
 * This talk presents the LLVM vector class, which also implements the small
 * vector optimization. Folly (the Facebook C++ library) contains a similar
 * class: 
 *  - https://github.com/facebook/folly/blob/master/folly/docs/FBVector.md
 *
 * This implementation (unlike the versions above) attempts to be allocator-
 * aware, and may be instantiated with custom C++ allocators. The following
 * resources are helpful in this regard:
 *  - http://stackoverflow.com/a/27472502
 *  - https://rawgit.com/google/cxx-std-draft/allocator-paper/allocator_user_guide.html
 *
 * Further, Vector<> uses the `is_bitwise_*<>` traits. The default allocator
 * is replaced by a special `realloc()`-aware one, should the value type be
 * bitwise relocatable (see `is_bitwise_relocatable<>`).
 *
 * TODO-FIXME-CHANGES:
 *   - resize(size) shouldn't value construct, but default construct
 *     (difference from std::vector); otherwise, use either resize(size,val) or
 *     maybe a custom version resize(size, value_construct_t{})
 *   - similar for constructors and other methods that resize the vector
 *   - NOEXCEPT/NOEXCEPT_IF (in particular: the move operations!)
 *   - uninitialized_*_rev() when I've writtenthese
 *
 * TODO:
 *   - maybe use C++17 Allocator::is_always_equal where applicable.
 *
 * TODO:
 *   - introduce Elements<tCount> and Bytes<tByteSize> extra arguments for in-place size
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_VECTOR_HPP_44E512A3_348A_4B11_99B3_97DA5217125E
#define FLUX_FTL_VECTOR_HPP_44E512A3_348A_4B11_99B3_97DA5217125E

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/ftl/auxiliary.hpp>
#include <flux/ftl/ptr_and_flag.hpp>

#include <flux/compat/iterator_traits.hpp>

#include <memory>
#include <iterator>
#include <type_traits>

#include <cstddef>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl);
//--    forward                     ///{{{1///////////////////////////////////
namespace detail
{
	template< typename > struct VecDefaultAllocator;
	template< typename, class > struct VecTraits;
};

namespace aspect
{
	/** Vector default allocator
	 *
	 * Default allocator for `ftl::VectorBase` and `ftl::Vector`. Evaluates to
	 * `std::allocator` by default, unless the type is marked as relocatable 
	 * by `is_bitwise_relocatable<>`, in which case it uses a special allocator
	 * based around `std::realloc()`.
	 */
	template< typename tType > 
	using VecDefaultAllocator = typename ftl::detail::VecDefaultAllocator<tType>::type;
}

//--    VectorBase<>                ///{{{1///////////////////////////////////
/** Vector class with support for in-place storage
 *
 * Variable-sized array ("vector") with support for in-place storage. See 
 * `Vector<>` for the latter.
 * 
 * \note In most instances, the `Vector<>` template should be used instead.
 *   Only use `VectorBase<>` when accepting vectors as arguments to functions, 
 *   as this won't tie the vector to a certain fixed in-place size.
 * 
 * `VectorBase` and `Vector` closely match the interface of `std::vector`,
 * with the following divergences:
 *   - Strong exception guarantees only if moving elements will not throw;
 *     basic guarantees otherwise. (TODO: revisit this)
 *   - TODO: no value init, but default init? doesn't zero out large allocs...
 *
 * `VectorBase` and `Vector` respect and the `is_bitwise_*` traits, and may use
 * these to perform raw `memcpy()` and similar operations.
 * 
 * <b>Assumptions & Exception guarantees:</b>
 *  - Requirement: \a tType will not throw from its destructor
 *  - Assumption: \a tType will not throw when moved
 *  - If so, then strong exception guarantee
 *  - Otherwise, if \a tType throws when moved, then only basic exception guarantee.
 *  - `VectorBase` itself throws `std::bad_alloc` if allocations fail, and
 *    `std::out_of_range` from `.at()` if the index is out of range.
 *
 * TODO:<i>Is that actually still true? When don't we provide strong
 * exception guarantee? + TODO: write tests where elements throw exceptions on
 * occasion.</i>
 *
 * \note `VectorBase` and `Vector` are inspired from and based on several 
 * sources, including:
 *   - Facebook's Folly: https://github.com/facebook/folly/blob/master/folly/docs/FBVector.md
 *   - LLVM's SmallVector: http://llvm.org/docs/doxygen/html/classllvm_1_1SmallVector.html and the talk presenting it (TODO-youtube/CPPCON link)
 *
 * \see Vector
 */
template< typename tType, class tAlloc = aspect::VecDefaultAllocator<tType> >
class VectorBase 
{
	/* TODO:
	 *   - check TODOs here and in the .inl files
	 *   - uses is_trivially_copyable + is_trivially_constructible + is_trivially destructible. Make these to `is_bitwise_*`. Maybe need a new trait there, `is_bitwise_zero_conjurable` (or redefine the conjurable one to be more strict: type should be in a "neutral" state if zeroed out).
	 */

	/* TODO:
	 *
	 * try to make traits for whether or not certain operations (e.g., moves) are NOEXCEPT. mark appropriate methods appropriately
	 */
	
	using ATraits_ = std::allocator_traits<tAlloc>;
	using VTraits_ = detail::VecTraits<tType,tAlloc>;

	using VoidPtr_ = typename ATraits_::void_pointer;
	
	public:
		using value_type = tType;
		using pointer = typename ATraits_::pointer;
		using const_pointer = typename ATraits_::const_pointer;
		using reference = value_type&;
		using const_reference = value_type const&;
		using size_type = typename ATraits_::size_type;
		using difference_type = typename ATraits_::difference_type;

		using iterator = pointer;
		using const_iterator = const_pointer;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		using allocator_type = tAlloc;

	public:
		VectorBase(), ~VectorBase();
		
		VectorBase( size_type, value_type const&, tAlloc const& = tAlloc() );

		explicit VectorBase( tAlloc const& );
		explicit VectorBase( size_type, tAlloc const& = tAlloc() );
		explicit VectorBase( size_type, DefaultInitTag, tAlloc const& = tAlloc() ); //TODO-test

		template< typename tIter, class tCat = typename compat::iterator_traits<tIter>::iterator_category >
		VectorBase( tIter, tIter, tAlloc const& = tAlloc() );
		
		VectorBase( std::initializer_list<value_type>, tAlloc const& = tAlloc() );


	public:
		/** \name Moving
		 * \brief Move vector
		 *
		 * Move vector. This will attempt to move the buffer of the source
		 * vector to the destination vector. This is impossible under the
		 * following circumstances:
		 *   - the source vector uses in place storage (see `in_place()`)
		 *   - `propagate_on_container_move_assignment` is false and the 
		 *     allocators are incompatible (for `operator=), or an incompatible
		 *     allocator was provided (constructor with \a tAlloc argument).
		 * Thus, one must assume that references, pointers and iterators are
		 * invalidated when a vector is moved (unlike the proposed behaviour of
		 * LWG 2321).
		 */
		//@{
		VectorBase( VectorBase&& );
		VectorBase( VectorBase&&, tAlloc const& );

		VectorBase& operator= (VectorBase&&);
		//@}
		
		/** \name Copying
		 * \brief Copy vector
		 *
		 * //TODO
		 */
		//@{
		VectorBase( VectorBase const& );
		VectorBase( VectorBase const&, tAlloc const& );

		VectorBase& operator= (VectorBase const&);
		//@}

		VectorBase& operator= (std::initializer_list<value_type>);
	
	protected:
		struct InplaceStorage_ {};
		VectorBase( InplaceStorage_, VoidPtr_, VoidPtr_, size_type = 0, tAlloc const& = tAlloc() );
		VectorBase( InplaceStorage_, VoidPtr_, VoidPtr_, size_type, DefaultInitTag, tAlloc const& = tAlloc() );
		VectorBase( InplaceStorage_, VoidPtr_, VoidPtr_, size_type, value_type const&, tAlloc const& );

		template< typename tIter, class tCat = typename compat::iterator_traits<tIter>::iterator_category >
		VectorBase( InplaceStorage_, VoidPtr_, VoidPtr_, tIter, tIter, tAlloc const& );

	public:
		reference operator[] (size_type);
		const_reference operator[] (size_type) const;

	public:
		/** Is vector empty?
		 *
		 * Returns `true` if vector contains no elements, `false` otherwise.
		 * \f$\mathcal{O}\left(1\right)\f$.
		 */
		bool empty() const noexcept;
		/** Does the vector currently use an in-place buffer?
		 *
		 * Returns `true` if the storage currently held by the vector is
		 * refering to an in-place buffer, and `false` otherwise. 
		 * \f$\mathcal{O}\left(1\right)\f$.
		 */
		bool in_place() const noexcept;

		/** Size of the vector
		 * 
		 * Returns the number of elements stored in the vector. 
		 * \f$\mathcal{O}\left(1\right)\f$.
		 */
		size_type size() const noexcept;
		/** Capacity of the vector
		 *
		 * Returns the capacity of the storage currently held by the vector.
		 * \f$\mathcal{O}\left(1\right)\f$.
		 */
		size_type capacity() const noexcept;
		/** Maximal size of the vector
		 *
		 * Returns a theoretical upper bound on the size of this vector.
		 * The theoretical upper bound involves varying degrees of wishful 
		 * thinking. \f$\mathcal{O}\left(1\right)\f$.
		 */
		size_type max_size() const noexcept;

		/** Reserve storage
		 *
		 * Reserve \a size_type elements of storage. Does nothing of the 
		 * requested size is smaller than the current `capacity()` of the
		 * vector. \f$\mathcal{O}\left(N\right)\f$.
		 */
		void reserve( size_type );
		/** Shrink storage
		 *
		 * Shrink the currently held storage to exactly fit the number of items
		 * in the vector. May reallocate storage. Ignored if an in-place buffer
		 * (`in_place()`) is in use. \f$\mathcal{O}\left(N\right)\f$.
		 */
		void shrink_to_fit();

		/** Resize vector
		 *
		 * Resize vector to \a size_type elements, destroying or value-
		 * constructing elements as needed. \f$\mathcal{O}\left(N\right)\f$.
		 */
		void resize( size_type );
		/** Resize vector
		 *
		 * Resize vector to \a size_type elements, destroying or copy-
		 * constructing elements from \a value_type as needed.
		 * \f$\mathcal{O}\left(N\right)\f$.
		 */
		void resize( size_type, value_type const& ); //TODO-test: ref-to-self.
		/** Resize vector
		 *
		 * Resize vector to \a size_type elements, destroying or default-
		 * constructing elements as needed. \f$\mathcal{O}\left(N\right)\f$.
		 */
		void resize( size_type, DefaultInitTag ); //TODO-tests

		/** Clear vector
		 *
		 * Destroy all currently held elements. Does not free/release storage.
		 * \f$\mathcal{O}\left(N\right)\f$ (\f$\mathcal{O}\left(1\right)\f$ if
		 * `tType` `is_bitwise_dismissible<>`).
		 */
		void clear() noexcept;

		void assign( size_type, value_type const& );
		void assign( std::initializer_list<value_type> );

		template< typename tIter, class tCat = typename compat::iterator_traits<tIter>::iterator_category >
		void assign( tIter, tIter );


		/** \brief Insert elements at location
		 *
		 * Insert elements before \a const_iterator. \a value_type may refer to
		 * an element in the current vector. \f$\mathcal{O}\left(N\right)\f$.
		 *
		 * *Exceptions:* Generally, weak exception guarantee. Strong exception
		 * guarantee in the following cases:
		 *   - Elements are insert at the end
		 *   - TODO (nothrow move? copy?)
		 */
		iterator insert( const_iterator, value_type&& );
		/** \copydoc insert() */
		iterator insert( const_iterator, value_type const& );
		/** \copydoc insert() */
		iterator insert( const_iterator, size_type, value_type const& );
		/** \copydoc insert() */
		iterator insert( const_iterator, std::initializer_list<value_type> );

		/** Insert elements
		 *
		 * Insert elements in range [\a tIter, \a tIter)`into vector before \a
		 * const_iterator. The iterators \a tIter may not be iterators into the
		 * current vector (similar to `std::vector`, see 23.2.3 <i>Sequence
		 * containers</i> [sequence.reqmts]).  "True"
		 * \f$\mathcal{O}\left(N\right)\f$ if \a tIter is a random access
		 * iterator, and amortized \f$\mathcal{O}\left(N\right)\f$ otherwise
		 * (i.e., multiple relocations may occur). 
		 */
		template< typename tIter, class tCat = typename compat::iterator_traits<tIter>::iterator_category >
		iterator insert( const_iterator, tIter, tIter ); //TODO-tests

		/** Emplace element at location
		 *
		 * Constructs element before \a const_iterator. Arguments \a tAargs may
		 * refer back into the current vector. \f$\mathcal{O}\left(N\right)\f$.
		 */
		template< typename... tArgs > inline
		iterator emplace( const_iterator, tArgs&&... );

		/** \brief Erase element(s)
		 *
		 * Erase element \a const_iterator or elements in the range 
		 * [\a const_titerator, \a const_iterator), respectively. 
		 * \f$\mathcal{O}\left(N\right)\f$.
		 */
		iterator erase( const_iterator );
		/** \copydoc erase() */
		iterator erase( const_iterator, const_iterator );
		

		void push_back( value_type&& );
		void push_back( value_type const& );

		template< typename... tArgs > inline
		reference emplace_back( tArgs&&... );

		
		void pop_back() noexcept;


		/** \name Retrieve elements
		 * \brief Retrieve element
		 *
		 * Retrive element at index \a size_type, at the front and at the back,
		 * respectively. `at()` throws `std::out_of_range` if the requested
		 * index is outside of the current bounds. `front()` and `back()` do
		 * <b>not</b> perform any checks - calling these on an empty vector
		 * results in undefined behaviour.  Strictly
		 * \f$\mathcal{O}\left(1\right)\f$.
		 */
		//@{
		reference at( size_type );
		const_reference at( size_type ) const;

		reference front() noexcept;
		const_reference front() const noexcept;
		reference back() noexcept;
		const_reference back() const noexcept;
		//@}

		/** \name Forward iterators
		 * \brief Forward/normal-order iterators
		 *
		 * Returns a "normal" iterator. The normal iterator is a 
		 * RandomAccessIterator.
		 */
		//@{
		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		const_iterator cbegin() const noexcept;

		iterator end() noexcept;
		const_iterator end() const noexcept;
		const_iterator cend() const noexcept;
		//@}

		/** \name Reverse iterators
		 * \brief Reverse-order iterators
		 */
		//@{
		reverse_iterator rbegin() noexcept;
		const_reverse_iterator rbegin() const noexcept;
		const_reverse_iterator crbegin() const noexcept;

		reverse_iterator rend() noexcept;
		const_reverse_iterator rend() const noexcept;
		const_reverse_iterator crend() const noexcept;
		//@}

		/** \name data(): pointers to continuous memory
		 * \brief Pointer to continuous memory
		 * 
		 * Get a pointer to continuous memory. The pointer is such that the
		 * range [`data()`, `data()`+`size()`) is valid, even if the container is
		 * empty. 
		 *
		 * \warning `data()` is not required to return a `nullptr` when empty!
		 */
		//@{
		value_type* data() noexcept;
		value_type const* data() const noexcept;
		//@}


		void swap( VectorBase& ); //TODO-tests; TODO: noexcept() => is_nothrow_swappable

		/** Get allocator
		 */
		allocator_type get_allocator() const;

	private:
		void fill_def_( size_type, ValueInitTag );
		void fill_def_( size_type, DefaultInitTag );
		void fill_val_( size_type, value_type const& );

		template< typename tIter >
		void assign_copy_( tIter, tIter, std::input_iterator_tag );
		template< typename tIter > 
		void assign_copy_( tIter, tIter, std::random_access_iterator_tag );

		void assign_move_other_( VectorBase&&, std::true_type, bool = true ); 
		void assign_move_other_( VectorBase&&, std::false_type );
	
		void grow_();
		void reserve_( size_type );
	
		template< typename... tArgs >
		reference emplace_back_( std::true_type, tArgs&&... );
		template< typename... tArgs >
		reference emplace_back_( std::false_type, tArgs&&... );

		iterator insert_back_( size_type, value_type const& );

		template< typename tIter > 
		iterator insert_( const_iterator, tIter, tIter, std::input_iterator_tag );
		template< typename tIter >
		iterator insert_( const_iterator, tIter, tIter, std::random_access_iterator_tag );

	private:
		using Pointer_ = pointer;
		struct Impl_ : tAlloc
		{
			using pointer = Pointer_;

			pointer beg, end;
			PtrAndFlag<pointer> capacity;

			explicit Impl_( tAlloc&&, pointer = nullptr, pointer = nullptr );
			explicit Impl_( tAlloc const&, pointer = nullptr, pointer = nullptr );
		};

	protected:
		Impl_ mImpl;
};

// TODO: tag_is_bitwise
//   - duplicable: no
//   - relocatable : yes, if underlying type is relocatable (**) AND if tAlloc is
//   - conjurable: ??? no (yes, but a pessimization for the inplace storage)
//   - awakeable: no
//   - dismissible: no
//
// WARNING: all of these depend on tAlloc as well!
//
// (**) without the small-vector optimization it would be so regardless
//
// ISSUE: is_relocatable only from Vector -> Vector, or VectorBase to VectorBase
//   but never across these! (Can't detect if Vector is downcast to VectorBase, so
//   VectorBase can never be relocatable??)

//--    Vector<>                    ///{{{1///////////////////////////////////
namespace detail
{
	template< bool, std::size_t, std::size_t >
	struct VecInlineStore
	{
		static constexpr
		std::nullptr_t store_beg_( VecInlineStore* ) noexcept;
		static constexpr
		std::nullptr_t store_end_( VecInlineStore* ) noexcept;
	};

	template< std::size_t tBytes, std::size_t tAlign >
	struct VecInlineStore<true,tBytes,tAlign>
	{
		static constexpr
		void* store_beg_( VecInlineStore* ) noexcept;
		static constexpr
		void* store_end_( VecInlineStore* ) noexcept;

		private: typename std::aligned_storage<tBytes,tAlign>::type mStorage;
	};
}

/** Vector with in place storage
 *
 * `Vector<>` class that provides in place storage of at most `tBytes`.
 *
 * \copydoc VectorBase
 *
 * \note One `Vector<>` is `noexcept' movable to an identical `Vector<>` if the
 * contained type is `noexcept` moveable  This is a contrast to `VectorBase`,
 * which never be statically guaranteed to be `noexcept` movable.
 */
template< typename tType, std::size_t tBytes = 64, typename tAlloc = aspect::VecDefaultAllocator<tType> >
class Vector
	: public VectorBase< tType, tAlloc >
	, private detail::VecInlineStore< (sizeof(tType) <= tBytes), (tBytes/sizeof(tType))*sizeof(tType), alignof(tType) >
{
	using Base_ = VectorBase<tType, tAlloc>;
	using Init_ = typename Base_::InplaceStorage_;

	public:
		using size_type = typename Base_::size_type;
		using value_type = typename Base_::value_type;

		static constexpr bool isNothrowMovable = std::is_nothrow_move_constructible<tType>::value && std::is_nothrow_move_assignable<tType>::value; // TODO: use is_nothrow_relocatable() and similar.

	public:
		Vector();
		Vector( size_type, value_type const&, tAlloc const& = tAlloc() );

		explicit Vector( tAlloc const& );
		explicit Vector( size_type, tAlloc const& = tAlloc() );
		explicit Vector( size_type, DefaultInitTag, tAlloc const& = tAlloc() ); //TODO-tests

		template< typename tIter, class tCat = typename compat::iterator_traits<tIter>::iterator_category >
		Vector( tIter, tIter, tAlloc const& = tAlloc() );

		Vector( Base_&&, tAlloc const& );
		Vector( Base_ const&, tAlloc const& );

		Vector( std::initializer_list<value_type>, tAlloc const& = tAlloc() );
	
	public:
		Vector( Vector&& ) noexcept(isNothrowMovable);
		Vector& operator= (Vector&&) noexcept(isNothrowMovable);

		Vector( Vector const& );
		Vector& operator= (Vector const&);

		Vector( Base_&& );
		Vector& operator= (Base_&&);
	
		Vector( Base_ const& );
		Vector& operator= (Base_ const&);

		Vector& operator= (std::initializer_list<value_type> );
};

//--    functions                   ///{{{1///////////////////////////////////

/** Compute the size of a vector as it grows
 *
 * Compute the new capacity of a `ftl::Vector<>` given a current capacity of
 * `aCap` elements and an element size of `aElemSize`.
 *
 * Currently, the growth rate corresponds to \f$\left\lceil x +
 * \frac{x}{2}\right\rceil\f$.
 *
 * \note the `aElemSize` affects the size returned initially (i.e., when the
 * current capacity \a aCap is zero). It ensures that the smallest capacity
 * corresponds to at least 64 bytes.
 */
std::size_t vector_grow( std::size_t aCap, std::size_t aElemSize  );


//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl);
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/vector.inl"
#endif // FLUX_FTL_VECTOR_HPP_44E512A3_348A_4B11_99B3_97DA5217125E
