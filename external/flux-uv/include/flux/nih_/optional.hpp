/*-******************************************************* -- HEADER -{{{1- */
/*- optional<> minimal fallback
 *
 * Note: totally unoptimzied (not sure if the standard allows for optional<>
 * to consider a empty state anyway. probably not).
 *
 * INCOMPLETE: missing constexpr all over the place.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_OPTIONAL_HPP_650EF20A_BC50_4DB0_BF02_2AFE2A696E05
#define FLUX_NIH_OPTIONAL_HPP_650EF20A_BC50_4DB0_BF02_2AFE2A696E05

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/enable_if.hpp>

#include <flux/compat/variant.hpp>
#include <flux/compat/type_traits.hpp>

#include <utility>
#include <stdexcept>
#include <functional>
#include <type_traits>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    nullopt                             ///{{{2///////////////////////////
struct nullopt_t
{
	constexpr explicit nullopt_t(int) {}
};

constexpr nullopt_t nullopt{0};

//--    in_place                            ///{{{2///////////////////////////

using in_place_t = compat::in_place_t;
using compat::in_place;

//--    optional                            ///{{{2///////////////////////////
namespace detail
{
	template< typename tType >
	struct OptionalProp
	{
		constexpr static bool nothrowDest =
			std::is_nothrow_destructible<tType>::value
		;
		constexpr static bool nothrowSwap = 
			// std::is_nothrow_swappable<tType>::value && /* C++17 only */
			std::is_nothrow_move_assignable<tType>::value &&
			std::is_nothrow_move_constructible<tType>::value
		;
	};
};

template< typename tType >
class optional
{
	private:
		using Prop_ = detail::OptionalProp<tType>;

	public:
		optional() noexcept
			: mEngaged(false)
		{}
		optional( nullopt_t ) noexcept
			: mEngaged(false)
		{}

		//INCOMPLETE: following should be explicit under certain conditions
		template< 
			typename tOther,
			typename = FLUX_ENABLE_IF(std::is_constructible<tType, tOther const&>)(int),
			typename = FLUX_DISABLE_IF_C(
				std::is_constructible<tType, optional<tOther>&>::value ||
				std::is_constructible<tType, optional<tOther> const&>::value ||
				std::is_constructible<tType, optional<tOther>&&>::value ||
				std::is_constructible<tType, optional<tOther> const&>::value ||
				std::is_convertible<optional<tOther>&, tType>::value ||
				std::is_convertible<optional<tOther> const&, tType>::value ||
				std::is_convertible<optional<tOther>&&, tType>::value ||
				std::is_convertible<optional<tOther> const&&, tType>::value
			)(int)
		> 
		optional( optional<tOther> const& aOther )
			: mEngaged(false)
		{
			init_(aOther);
		}

		template< 
			typename tOther,
			typename = FLUX_ENABLE_IF(std::is_constructible<tType, tOther const&>)(int),
			typename = FLUX_DISABLE_IF_C(
				std::is_constructible<tType, optional<tOther>&>::value ||
				std::is_constructible<tType, optional<tOther> const&>::value ||
				std::is_constructible<tType, optional<tOther>&&>::value ||
				std::is_constructible<tType, optional<tOther> const&>::value ||
				std::is_convertible<optional<tOther>&, tType>::value ||
				std::is_convertible<optional<tOther> const&, tType>::value ||
				std::is_convertible<optional<tOther>&&, tType>::value ||
				std::is_convertible<optional<tOther> const&&, tType>::value
			)(int)
		> 
		optional( optional<tOther>&& aOther )
			: mEngaged(false)
		{
			init_( std::move(aOther) );
		}

		template< typename... tArgs > 
		explicit optional( in_place_t const&, tArgs&&... aArgs )
			: mEngaged(false)
		{
			emplace_( std::forward<tArgs>(aArgs)... );
		}

		//MISSING: in_place_t constructor with initializer_list
		
		//INCOMPLETE: following should be explicit under certain conditions
		template< 
			typename tOther = tType,
			typename = FLUX_ENABLE_IF(std::is_constructible<tType,tOther&&>)(int),
			typename = FLUX_DISABLE_IF_C(
				std::is_same<compat::decay_t<tOther>, in_place_t>::value ||
				std::is_same<compat::decay_t<tOther>, optional<tType>>::value
			)(int)
		> 
		optional( tOther&& aOther )
			: mEngaged(false)
		{
			emplace_( std::forward<tOther>(aOther) );
		}

		~optional();

	public:
		optional( optional const& );
		optional& operator= (optional const&);

		optional( optional&& );
		optional& operator= (optional&&);

		optional& operator= (nullopt_t);

		template< 
			typename tOther = tType,
			typename = FLUX_ENABLE_IF(std::is_constructible<tType,tOther>)(int),
			typename = FLUX_ENABLE_IF(std::is_assignable<tType&,tOther>)(int),
			typename = FLUX_DISABLE_IF(std::is_same<compat::decay_t<tOther>,optional<tType>>)(int)
		> 
		optional& operator= (tOther&& aValue)
		{
			if( mEngaged ) { destroy_(); mEngaged = false; }
			emplace_( std::move(aValue) );
			return *this;
		}

		template< 
			typename tOther,
			typename = FLUX_ENABLE_IF(std::is_constructible<tType,tOther const&>)(int),
			typename = FLUX_ENABLE_IF(std::is_assignable<tType&,tOther const&>)(int),
			typename = FLUX_DISABLE_IF_C(
				std::is_constructible<tType, optional<tOther>&>::value ||
				std::is_constructible<tType, optional<tOther> const&>::value ||
				std::is_constructible<tType, optional<tOther>&&>::value ||
				std::is_constructible<tType, optional<tOther> const&>::value ||
				std::is_convertible<optional<tOther>&, tType>::value ||
				std::is_convertible<optional<tOther> const&, tType>::value ||
				std::is_convertible<optional<tOther>&&, tType>::value ||
				std::is_convertible<optional<tOther> const&&, tType>::value
			)(int)
		> 
		optional& operator= (optional<tOther> const& aOther )
		{
			if( this == &aOther ) 
				return *this;

			if( mEngaged ) { destroy_(); mEngaged = false; }
			init_( aOther );
			return *this;
		}
		
		template< 
			typename tOther,
			typename = FLUX_ENABLE_IF(std::is_constructible<tType,tOther>)(int),
			typename = FLUX_ENABLE_IF(std::is_assignable<tType&,tOther>)(int),
			typename = FLUX_DISABLE_IF_C(
				std::is_constructible<tType, optional<tOther>&>::value ||
				std::is_constructible<tType, optional<tOther> const&>::value ||
				std::is_constructible<tType, optional<tOther>&&>::value ||
				std::is_constructible<tType, optional<tOther> const&>::value ||
				std::is_convertible<optional<tOther>&, tType>::value ||
				std::is_convertible<optional<tOther> const&, tType>::value ||
				std::is_convertible<optional<tOther>&&, tType>::value ||
				std::is_convertible<optional<tOther> const&&, tType>::value
			)(int)
		> 
		optional& operator= (optional<tOther>&& aOther)
		{
			if( mEngaged ) { destroy_(); mEngaged = false; }
			init_( std::move(aOther) );
			return *this;
		}

	public:
		tType* operator-> ();
		tType const* operator-> () const;

		tType& operator* () &;
		tType&& operator* () &&;
		tType const& operator* () const&;

		explicit operator bool() const noexcept;

	public:	
		bool has_value() const noexcept;

		tType& value() &;
		tType const& value() const&;
		tType&& value() &&;
		tType const&& value() const&&;

		template< typename tOther >
		tType value_or( tOther&& ) &&;
		template< typename tOther >
		tType value_or( tOther&& ) const&;

		void swap( optional& ) noexcept(Prop_::nothrowSwap);
		void reset() noexcept(Prop_::nothrowDest);

		template< typename... tArgs >
		void emplace( tArgs&&... );

		//MISSING: emplace with initializer_list
	
	private:
		template< typename tOther > 
		void init_( optional<tOther>&& );
		template< typename tOther > 
		void init_( optional<tOther> const& );

		tType* ptr_() noexcept;
		tType const* ptr_() const noexcept;

		template< typename... tArgs > 
		void emplace_( tArgs&&... );

		void destroy_() noexcept(Prop_::nothrowDest);

	private:
		typename std::aligned_storage< 
			sizeof(tType), 
			alignof(tType)
		>::type mStore; /* Probably more aligned than bool ... */

		bool mEngaged;
};

//--    functions                           ///{{{2///////////////////////////
template< typename tType >
optional<compat::decay_t<tType>> make_optional( tType&& );

template< typename tType, typename... tArgs >
optional<tType> make_optional( tArgs&&... );

//MISSING: make_optional with initializer_list


template< typename tType >
void swap( optional<tType>& aX, optional<tType>& aY ) noexcept(noexcept(aX.swap(aY)));

//--    non-member operators                ///{{{2///////////////////////////

	// two optional objects
template< typename tType >
bool operator== ( optional<tType> const&, optional<tType> const& );
template< typename tType >
bool operator!= ( optional<tType> const&, optional<tType> const& );
template< typename tType >
bool operator< ( optional<tType> const&, optional<tType> const& );
template< typename tType >
bool operator<= ( optional<tType> const&, optional<tType> const& );
template< typename tType >
bool operator> ( optional<tType> const&, optional<tType> const& );
template< typename tType >
bool operator>= ( optional<tType> const&, optional<tType> const& );

	// with nullopt_t
template< typename tType >
bool operator== ( optional<tType> const&, nullopt_t ) noexcept;
template< typename tType >
bool operator== ( nullopt_t, optional<tType> const& ) noexcept;
template< typename tType >
bool operator!= ( optional<tType> const&, nullopt_t ) noexcept;
template< typename tType >
bool operator!= ( nullopt_t, optional<tType> const& ) noexcept;
template< typename tType >
bool operator< ( optional<tType> const&, nullopt_t ) noexcept;
template< typename tType >
bool operator< ( nullopt_t, optional<tType> const& ) noexcept;
template< typename tType >
bool operator<= ( optional<tType> const&, nullopt_t ) noexcept;
template< typename tType >
bool operator<= ( nullopt_t, optional<tType> const& ) noexcept;
template< typename tType >
bool operator> ( optional<tType> const&, nullopt_t ) noexcept;
template< typename tType >
bool operator> ( nullopt_t, optional<tType> const& ) noexcept;
template< typename tType >
bool operator>= ( optional<tType> const&, nullopt_t ) noexcept;
template< typename tType >
bool operator>= ( nullopt_t, optional<tType> const& ) noexcept;

	// with a non-optional
template< typename tType >
bool operator== ( optional<tType> const&, tType const& );
template< typename tType >
bool operator== ( tType const&, optional<tType> const& );
template< typename tType >
bool operator!= ( optional<tType> const&, tType const& );
template< typename tType >
bool operator!= ( tType const&, optional<tType> const& );
template< typename tType >
bool operator< ( optional<tType> const&, tType const& );
template< typename tType >
bool operator< ( tType const&, optional<tType> const& );
template< typename tType >
bool operator<= ( optional<tType> const&, tType const& );
template< typename tType >
bool operator<= ( tType const&, optional<tType> const& );
template< typename tType >
bool operator> ( optional<tType> const&, tType const& );
template< typename tType >
bool operator> ( tType const&, optional<tType> const& );
template< typename tType >
bool operator>= ( optional<tType> const&, tType const& );
template< typename tType >
bool operator>= ( tType const&, optional<tType> const& );

//--    bad_optional_access                 ///{{{2///////////////////////////

struct bad_optional_access
	: std::logic_error
{
	bad_optional_access()
		: std::logic_error( "optional not engaged" )
	{}
};

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--    std::hash<> specialization  ///{{{1///////////////////////////////////
namespace std
{
	template< typename tType >
	struct hash< flux::nih_::optional<tType> > : hash<tType>
	{
		typedef std::size_t result_type;
		typedef flux::nih_::optional<tType> argument_type;

		result_type operator() (argument_type const& aX) const
		{
			if( !aX ) return 0x3333;
			return hash<tType>::operator() (aX.value() );
		}
	};
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/optional.inl"
#endif // FLUX_NIH_OPTIONAL_HPP_650EF20A_BC50_4DB0_BF02_2AFE2A696E05
