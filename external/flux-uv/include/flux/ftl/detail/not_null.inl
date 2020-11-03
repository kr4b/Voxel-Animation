/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- not_null implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/util/hint.hpp>
#include <flux/util/intent.hpp>

#include <memory>
#include <utility>
#include <functional>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    $ local                             ///{{{2///////////////////////////
namespace detail
{
	template< bool > 
	struct OptCheckHelper
	{
		template< class tAspect, typename tType > inline
		static void check_not_null( tType const& aPointer )
		{
			tAspect::check( aPointer );
		}
	};
	template<> struct OptCheckHelper<false> {
		template< class tAspect, typename tType > inline
		static void check_not_null( tType const& aPointer )
		{
			tAspect::access( aPointer );
		}
	};
};

//--    detail::UncheckedHelper             ///{{{2///////////////////////////
namespace detail
{
	template< bool > struct UncheckedHelper
	{
		template< typename tPtr, typename tType > inline
		static void reset( tPtr& aPtr, tType&& aType )
		{
			aPtr.reset_checked_( std::forward<tType>(aType) );
		}
	};
	template<> struct UncheckedHelper<true> {
		template< typename tPtr, typename tType > inline
		static void reset( tPtr& aPtr, tType&& aType )
		{
			aPtr.reset_unchecked_( std::forward<tType>(aType) );
		}
	};
}

//--    aspect::NotNull*                    ///{{{2///////////////////////////
namespace aspect
{
	struct NotNullUnsafe
	{
#		if FLUX_DEBUG
		static constexpr bool safe = true;
#		else
		static constexpr bool safe = false;
#		endif

		template< typename tPtrType >
		static inline void check( tPtrType const& aPtr ) noexcept
		{
#			if FLUX_DEBUG
			if( FLUX_HINT_UNLIKELY(!aPtr) )
				ftl::detail::null_handler_assert();
#			else
			FLUX_INTENT_UNUSED(aPtr);
#			endif // ~ DEBUG
		}
		template< typename tPtrType >
		static inline void access( tPtrType const& aPtr ) noexcept
		{
#			if FLUX_DEBUG
			if( FLUX_HINT_UNLIKELY(!aPtr) )
				ftl::detail::null_handler_assert();
#			else
			FLUX_INTENT_UNUSED(aPtr);
#			endif // ~ DEBUG
		}
	};

	struct NotNullExcept
	{
		static constexpr bool safe = true;

		template< typename tPtrType >
		static inline void check( tPtrType const& aPtr )
		{
			if( FLUX_HINT_UNLIKELY(!aPtr) )
				ftl::detail::null_handler_throw();
		}
		template< typename tPtrType >
		static inline void access( tPtrType const& aPtr )
		{
#			if FLUX_DEBUG
			if( FLUX_HINT_UNLIKELY(!aPtr) )
				ftl::detail::null_handler_assert();
#			else
			FLUX_INTENT_UNUSED(aPtr);
#			endif // ~ DEBUG
		}
	};
	struct NotNullExceptAssert
	{
		static constexpr bool safe = true;

		template< typename tPtrType >
		static inline void check( tPtrType const& aPtr )
		{
			if( FLUX_HINT_UNLIKELY(!aPtr) )
			{
#				if FLUX_DEBUG
				ftl::detail::null_handler_assert();
#				endif // ~ DEBUG
				ftl::detail::null_handler_throw();
			}
		}
		template< typename tPtrType >
		static inline void access( tPtrType const& aPtr )
		{
#			if FLUX_DEBUG
			if( FLUX_HINT_UNLIKELY(!aPtr) )
				ftl::detail::null_handler_assert();
#			else
			FLUX_INTENT_UNUSED(aPtr);
#			endif // ~ DEBUG
		}
	};
}

//--    is_not_null<>                       ///{{{2///////////////////////////
namespace detail
{
	template< typename tType >
	struct is_not_null_Impl_ : std::false_type
	{};

	template< typename tType, class tAspect >
	struct is_not_null_Impl_< not_null<tType,tAspect> > : std::true_type
	{};
}

template< typename tType > 
struct is_not_null : detail::is_not_null_Impl_<
	typename compat::decay_t< tType >
> {};

//--    remove_not_null<>                   ///{{{2///////////////////////////
template< typename tType > struct remove_not_null
{
	typedef tType type;
};
template< typename tType, class tAspect > 
struct remove_not_null< not_null<tType,tAspect> > {
	typedef typename remove_not_null<tType>::type type;
};

//--    not_null                            ///{{{2///////////////////////////

	// [cd]tor -{{{3-
template< typename tType, class tAspect > inline
not_null<tType,tAspect>::not_null( not_null const& aOther )
	: mPointer(aOther.mPointer)
{
	tAspect::access( mPointer );
}

template< typename tType, class tAspect >
template< typename tOtherType, class tOtherAspect, typename > inline
not_null<tType,tAspect>::not_null( not_null<tOtherType,tOtherAspect> const& aOther )
	: mPointer(aOther.mPointer)
{
	detail::OptCheckHelper<tOtherAspect::safe>::template check_not_null<tAspect>(mPointer);
}

template< typename tType, class tAspect > inline
not_null<tType,tAspect>::not_null( not_null&& aOther )
	: mPointer(std::move(aOther.mPointer))
{
	tAspect::access( mPointer );
}

template< typename tType, class tAspect >
template< typename tOtherType, class tOtherAspect, typename > inline
not_null<tType,tAspect>::not_null( not_null<tOtherType,tOtherAspect>&& aOther )
	: mPointer(std::move(aOther.mPointer))
{
	detail::OptCheckHelper<tOtherAspect::safe>::template check_not_null<tAspect>(mPointer);
}

template< typename tType, class tAspect >
template< typename tPointer, typename > inline
not_null<tType,tAspect>::not_null( tPointer&& aPointer )
	: mPointer(std::forward<tPointer>(aPointer))
{
	tAspect::check( mPointer );
}

template< typename tType, class tAspect >
template< typename tOtherType, class tOtherAspect, typename, typename > inline
not_null<tType,tAspect>::not_null( not_null<tOtherType,tOtherAspect> const& aOther )
	: mPointer(static_cast<pointer>(aOther.mPointer))
{
	detail::OptCheckHelper<tOtherAspect::safe>::template check_not_null<tAspect>(mPointer);
}
template< typename tType, class tAspect >
template< typename tOtherType, class tOtherAspect, typename, typename > inline
not_null<tType,tAspect>::not_null( not_null<tOtherType,tOtherAspect>&& aOther )
	: mPointer(static_cast<pointer>(aOther.mPointer))
{
	detail::OptCheckHelper<tOtherAspect::safe>::template check_not_null<tAspect>(mPointer);
}

	// public - operators -{{{3-
template< typename tType, class tAspect > inline
not_null<tType,tAspect>& not_null<tType,tAspect>::operator= (not_null const& aOther) 
{
	mPointer = aOther.mPointer;
	tAspect::access( mPointer );
	return *this;
}

template< typename tType, class tAspect >
template< typename tOtherType, class tOtherAspect, typename > inline
not_null<tType,tAspect>& not_null<tType,tAspect>::operator= (not_null<tOtherType,tOtherAspect> const& aOther)
{
	mPointer = aOther.mPointer;
	detail::OptCheckHelper<tOtherAspect::safe>::template check_not_null<tAspect>(mPointer);
	return *this;
}

template< typename tType, class tAspect > inline
not_null<tType,tAspect>& not_null<tType,tAspect>::operator= (not_null&& aOther) 
{
	mPointer = std::move(aOther.mPointer);
	tAspect::access( mPointer );
	return *this;
}

template< typename tType, class tAspect >
template< typename tOtherType, class tOtherAspect, typename > inline
not_null<tType,tAspect>& not_null<tType,tAspect>::operator= (not_null<tOtherType,tOtherAspect>&& aOther)
{
	mPointer = std::move(aOther.mPointer);
	detail::OptCheckHelper<tOtherAspect::safe>::template check_not_null<tAspect>(mPointer);
	return *this;
}

template< typename tType, class tAspect > inline
not_null<tType,tAspect>::operator pointer&&() &&
{
	tAspect::access( mPointer );
	return std::move(mPointer);
}
template< typename tType, class tAspect > inline
not_null<tType,tAspect>::operator pointer const&&() const&
{
	tAspect::access( mPointer );
	return std::move(mPointer);
}

template< typename tType, class tAspect >
template< typename tOtherPtr, typename > inline
not_null<tType,tAspect>::operator tOtherPtr() &&
{
	tAspect::access( mPointer );
	return std::move(mPointer);
}
template< typename tType, class tAspect >
template< typename tOtherPtr, typename > inline
not_null<tType,tAspect>::operator tOtherPtr() const&
{
	tAspect::access( mPointer );
	return mPointer;
}

template< typename tType, class tAspect >
not_null<tType,tAspect>::operator bool() const
{
	tAspect::access( mPointer );
	return true;
}

template< typename tType, class tAspect > inline
bool not_null<tType,tAspect>::operator== (std::nullptr_t) const
{
	// Never equal to the null pointer!
	tAspect::access( mPointer );
	return false;
}
template< typename tType, class tAspect > inline
bool not_null<tType,tAspect>::operator!= (std::nullptr_t) const
{
	// Never equal to the null pointer!
	tAspect::access( mPointer );
	return true;
}

template< typename tType, class tAspect > inline
bool not_null<tType,tAspect>::operator== (pointer const& aPtr) const
{
	tAspect::access( mPointer );
	return mPointer == aPtr;
}
template< typename tType, class tAspect > inline
bool not_null<tType,tAspect>::operator== (not_null const& aPtr) const
{
	tAspect::access( mPointer );
	return mPointer == aPtr.mPointer;
}
template< typename tType, class tAspect > inline
bool not_null<tType,tAspect>::operator!= (pointer const& aPtr) const
{
	tAspect::access( mPointer );
	return mPointer != aPtr;
}
template< typename tType, class tAspect > inline
bool not_null<tType,tAspect>::operator!= (not_null const& aPtr) const
{
	tAspect::access( mPointer );
	return mPointer != aPtr.mPointer;
}

template< typename tType, class tAspect > inline
bool not_null<tType,tAspect>::operator< (not_null const& aPtr) const
{
	tAspect::access( mPointer );
	return std::less<tType>{}( mPointer, aPtr.mPointer );
}
template< typename tType, class tAspect > inline
bool not_null<tType,tAspect>::operator<= (not_null const& aPtr) const
{
	tAspect::access( mPointer );
	return !std::less<tType>{}( aPtr.mPointer, mPointer );
}
template< typename tType, class tAspect > inline
bool not_null<tType,tAspect>::operator> (not_null const& aPtr) const
{
	tAspect::access( mPointer );
	return std::less<tType>{}( aPtr.mPointer, mPointer );
}
template< typename tType, class tAspect > inline
bool not_null<tType,tAspect>::operator>= (not_null const& aPtr) const
{
	tAspect::access( mPointer );
	return !std::less<tType>{}( mPointer, aPtr.mPointer );
}

template< typename tType, class tAspect > inline
compat::add_pointer_t< typename not_null<tType,tAspect>::element_type > not_null<tType,tAspect>::operator->() const
{
	tAspect::access( mPointer );
	return &*mPointer;
}
template< typename tType, class tAspect > inline
compat::add_lvalue_reference_t< typename not_null<tType,tAspect>::element_type > not_null<tType,tAspect>::operator*() const
{
	tAspect::access( mPointer );
	return *mPointer;
}

	// public - functions -{{{3-
template< typename tType, class tAspect > 
template< typename tPtr, typename > inline
not_null<decltype(std::declval<tPtr>().get()),tAspect> not_null<tType, tAspect>::get() const
{
	typedef not_null<decltype(std::declval<pointer>().get()),tAspect> NotNull_;
	return NotNull_( mPointer.get(), typename NotNull_::Unchecked_() );
}
template< typename tType, class tAspect >
template< typename tNewPtr, typename tPtr, typename > inline
void not_null<tType,tAspect>::reset( tNewPtr&& aPtr )
{
	detail::UncheckedHelper< 
		is_not_null<tNewPtr>::value
	>::reset( *this, std::forward<tNewPtr>(aPtr) );
}

	// private -{{{3-
template< typename tType, class tAspect > inline
not_null<tType,tAspect>::not_null( pointer aPtr, Unchecked_ const& )
	: mPointer( aPtr )
{
	tAspect::access( mPointer );
}

template< typename tType, class tAspect >
template< typename tNewPtr > inline
void not_null<tType,tAspect>::reset_checked_( tNewPtr&& aPtr )
{
	pointer ptr( std::forward<tNewPtr>(aPtr) );
	tAspect::check( ptr );

	using std::swap;
	swap( mPointer, ptr );
}
template< typename tType, class tAspect >
template< typename tNewPtr > inline
void not_null<tType,tAspect>::reset_unchecked_( tNewPtr&& aPtr )
{
	mPointer.reset( std::forward<tNewPtr>(aPtr) );
	tAspect::access( mPointer );
}

//--    functions                           ///{{{2///////////////////////////
template< typename tPointer > inline
not_null<tPointer> make_not_null( tPointer&& aPointer )
{
	return not_null<tPointer>( std::forward<tPointer>(aPointer) );
}
template< typename tPointer > inline
not_null_unsafe<tPointer> make_not_null_unsafe( tPointer&& aPointer )
{
	return not_null<tPointer>( std::forward<tPointer>(aPointer) );
}

//--    operators                           ///{{{2///////////////////////////
template< typename tPointer, class tAspect > inline
bool operator== (std::nullptr_t, not_null<tPointer, tAspect> const& aQ)
{
	return aQ == nullptr; // potentially checks with tAspect::access()
}
template< typename tPointer, class tAspect > inline
bool operator!= (std::nullptr_t, not_null<tPointer, tAspect> const& aQ)
{
	return aQ == nullptr; // potentially checks with tAspect::access()
}

template< typename tPointer, class tAspect > inline
bool operator== (tPointer const& aP, not_null<tPointer, tAspect> const& aQ)
{
	return aQ == aP;
}
template< typename tPointer, class tAspect > inline
bool operator!= (tPointer const& aP, not_null<tPointer, tAspect> const& aQ)
{
	return aQ != aP;
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
