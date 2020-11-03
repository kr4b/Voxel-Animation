/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	optional<>
 */
/*-***************************************************************** -}}}1- */

#include <flux/compat/launder.hpp>
#include <cassert>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    optional<>                          ///{{{2///////////////////////////

	// [cd]tor -{{{3-
template< typename tType > inline
optional<tType>::~optional()
{
	if( mEngaged ) destroy_();
}
	
	// assignment -{{{3-
template< typename tType > inline
optional<tType>::optional( optional const& aOther )
	: mEngaged( false )
{
	init_( aOther );
}
template< typename tType > inline
optional<tType>& optional<tType>::operator= (optional const& aOther )
{
	if( this == &aOther )
		return *this;
	
	if( mEngaged )
	{
		destroy_();
		mEngaged = false;
	}

	init_( aOther );
	return *this;
}

template< typename tType > inline
optional<tType>::optional( optional&& aOther )
	: mEngaged( false )
{
	init_( std::move(aOther) );
}
template< typename tType > inline
optional<tType>& optional<tType>::operator= (optional&& aOther)
{
	if( mEngaged )
	{
		destroy_();
		mEngaged = false;
	}

	init_( std::move(aOther) );
	return *this;
}

template< typename tType > inline
optional<tType>& optional<tType>::operator= (nullopt_t)
{
	if( mEngaged )
	{
		destroy_();
		mEngaged = false;
	}

	return *this;
}

	// operators -{{{3-
template< typename tType > inline 
tType* optional<tType>::operator-> ()
{
	return ptr_();
}
template< typename tType > inline 
tType const* optional<tType>::operator-> () const
{
	return ptr_();
}

template< typename tType > inline 
tType& optional<tType>::operator* () &
{
	return *ptr_();
}
template< typename tType > inline 
tType&& optional<tType>::operator* () &&
{
	return std::move(*ptr_());
}
template< typename tType > inline 
tType const& optional<tType>::operator* () const&
{
	return *ptr_();
}

template< typename tType > inline 
optional<tType>::operator bool() const noexcept
{
	return mEngaged;
}

	// public -{{{3-
template< typename tType > inline 
bool optional<tType>::has_value() const noexcept
{
	return mEngaged;
}

template< typename tType > inline 
tType& optional<tType>::value() &
{
	if( !has_value() ) throw bad_optional_access();
	return *ptr_();
}
template< typename tType > inline 
tType const& optional<tType>::value() const&
{
	if( !has_value() ) throw bad_optional_access();
	return *ptr_();
}
template< typename tType > inline 
tType&& optional<tType>::value() &&
{
	if( !has_value() ) throw bad_optional_access();
	return std::move(*ptr_());
}
template< typename tType > inline 
tType const&& optional<tType>::value() const&&
{
	if( !has_value() ) throw bad_optional_access();
	return *ptr_();
}

template< typename tType > template< typename tOther > inline 
tType optional<tType>::value_or( tOther&& aOther ) &&
{
	return mEngaged ? std::move(*ptr_()) : static_cast<tType>(std::forward<tOther>(aOther));
}
template< typename tType > template< typename tOther > inline 
tType optional<tType>::value_or( tOther&& aOther ) const&
{
	return mEngaged ? *ptr_() : static_cast<tType>(std::forward<tOther>(aOther));
}

template< typename tType > inline
void optional<tType>::swap( optional& aOther ) noexcept(Prop_::nothrowSwap)
{
	if( !has_value() && !aOther.has_value() )
		return;

	if( !aOther.has_value() )
	{
		aOther.emplace_( std::move(**this) );
		destroy_();
		mEngaged = false;
	}
	else if( !has_value() ) 
	{
		emplace_( std::move(*aOther) );
		aOther.destroy_();
		aOther.mEngaged = false;
	}
	else
	{
		using std::swap;
		swap( **this, *aOther );
	}
}

template< typename tType > inline
void optional<tType>::reset() noexcept(Prop_::nothrowDest)
{
	if( mEngaged )
	{
		destroy_();
		mEngaged = false;
	}
}

template< typename tType > template< typename... tArgs > inline
void optional<tType>::emplace( tArgs&&... aArgs )
{
	if( mEngaged )
	{
		destroy_();
		mEngaged = false;
	}

	emplace_( std::forward<tArgs>(aArgs)... );
}

	// private -{{{3-
template< typename tType > template< typename tOther > inline
void optional<tType>::init_( optional<tOther>&& aOther )
{
	assert( !mEngaged );
	
	if( aOther )
	{
		new (&mStore) tType(std::move(*aOther));
		mEngaged = true;
	}
}
template< typename tType > template< typename tOther > inline
void optional<tType>::init_( optional<tOther> const& aOther )
{
	assert( !mEngaged );

	if( aOther )
	{
		new (&mStore) tType(*aOther);
		mEngaged = true;
	}
}

template< typename tType > inline
tType* optional<tType>::ptr_() noexcept
{
	return compat::launder( reinterpret_cast<tType*>(&mStore) );
}
template< typename tType > inline
tType const* optional<tType>::ptr_() const noexcept
{
	return compat::launder( reinterpret_cast<tType const*>(&mStore) );
}

template< typename tType > template< typename... tArgs > inline
void optional<tType>::emplace_( tArgs&&... aArgs )
{
	assert( !mEngaged );

	new (&mStore) tType(std::forward<tArgs>(aArgs)...);
	mEngaged = true;
}

template< typename tType > inline
void optional<tType>::destroy_() noexcept(Prop_::nothrowDest)
{
	assert( mEngaged );
	ptr_()->~tType();
}

//--    make_optional()                     ///{{{2///////////////////////////
template< typename tType > inline
optional<compat::decay_t<tType>> make_optional( tType&& aValue )
{
	return optional<compat::decay_t<tType>>(std::forward<tType>(aValue));
}
template< typename tType, typename... tArgs > inline
optional<tType> make_optional( tArgs&&... aArgs )
{
	return optional<tType>( in_place, std::forward<tArgs>(aArgs)... );
}

//--    swap()                              ///{{{2///////////////////////////
template< typename tType > inline
void swap( optional<tType>& aX, optional<tType>& aY ) noexcept(noexcept(aX.swap(aY)))
{
	aX.swap( aY );
}

//--    non-member operators                ///{{{2///////////////////////////

	// two optionals -{{{3-
template< typename tType > inline
bool operator== ( optional<tType> const& aX, optional<tType> const& aY )
{
	if( bool(aX) != bool(aY) ) return false;
	if( !bool(aX) ) return true;
	return *aX == *aY;
}
template< typename tType > inline
bool operator!= ( optional<tType> const& aX, optional<tType> const& aY )
{
	if( bool(aX) != bool(aY) ) return true;
	if( !bool(aX) ) return false;
	return *aX != *aY;
}
template< typename tType > inline
bool operator< ( optional<tType> const& aX, optional<tType> const& aY )
{
	if( !bool(aY) ) return false;
	if( !bool(aX) ) return true;
	return *aX < *aY;
}
template< typename tType > inline
bool operator<= ( optional<tType> const& aX, optional<tType> const& aY )
{
	if( !bool(aX) ) return true;
	if( !bool(aY) ) return false;
	return *aX <= *aY;
}
template< typename tType > inline
bool operator> ( optional<tType> const& aX, optional<tType> const& aY )
{
	if( !bool(aX) ) return false;
	if( !bool(aY) ) return true;
	return *aX > *aY;
}
template< typename tType > inline
bool operator>= ( optional<tType> const& aX, optional<tType> const& aY )
{
	if( !bool(aY) ) return true;
	if( !bool(aX) ) return false;
	return *aX >= *aY;
}

	// with nullopt_t -{{{3-
template< typename tType > inline
bool operator== ( optional<tType> const& aX, nullopt_t ) noexcept
{
	return !aX;
}
template< typename tType > inline
bool operator== ( nullopt_t, optional<tType> const& aX ) noexcept
{
	return !aX;
}
template< typename tType > inline
bool operator!= ( optional<tType> const& aX, nullopt_t ) noexcept
{
	return !!aX;
}
template< typename tType > inline
bool operator!= ( nullopt_t, optional<tType> const& aX ) noexcept
{
	return !!aX;
}
template< typename tType > inline 
bool operator< ( optional<tType> const&, nullopt_t ) noexcept
{
	return false;
}
template< typename tType > inline
bool operator< ( nullopt_t, optional<tType> const& aX ) noexcept
{
	return !!aX;
}
template< typename tType > inline
bool operator<= ( optional<tType> const& aX, nullopt_t ) noexcept
{
	return !aX;
}
template< typename tType > inline
bool operator<= ( nullopt_t, optional<tType> const& ) noexcept
{
	return true;
}
template< typename tType > inline
bool operator> ( optional<tType> const& aX, nullopt_t ) noexcept
{
	return !!aX;
}
template< typename tType > inline
bool operator> ( nullopt_t, optional<tType> const& ) noexcept
{
	return false;
}
template< typename tType > inline 
bool operator>= ( optional<tType> const&, nullopt_t ) noexcept
{
	return true;
}
template< typename tType > inline
bool operator>= ( nullopt_t, optional<tType> const& aX ) noexcept
{
	return !aX;
}

	// with non-optional -{{{3-
template< typename tType > inline
bool operator== ( optional<tType> const& aX, tType const& aY )
{
	return aX.has_value() ? *aX == aY : false;
}
template< typename tType > inline
bool operator== ( tType const& aX, optional<tType> const& aY )
{
	return aY.has_value() ? aX == *aY : false;
}
template< typename tType > inline
bool operator!= ( optional<tType> const& aX, tType const& aY )
{
	return aX.has_value() ? *aX != aY : true;
}
template< typename tType > inline
bool operator!= ( tType const& aX, optional<tType> const& aY )
{
	return aY.has_value() ? aX != *aY : true;
}
template< typename tType > inline 
bool operator< ( optional<tType> const& aX, tType const& aY )
{
	return aX.has_value() ? *aX < aY : true;
}
template< typename tType > inline
bool operator< ( tType const& aX, optional<tType> const& aY )
{
	return aY.has_value() ? aX < *aY : false;
}
template< typename tType > inline
bool operator<= ( optional<tType> const& aX, tType const& aY )
{
	return aX.has_value() ? *aX <= aY : true;
}
template< typename tType > inline
bool operator<= ( tType const& aX, optional<tType> const& aY )
{
	return aY.has_value() ? aX <= *aY : false;
}
template< typename tType > inline
bool operator> ( optional<tType> const& aX, tType const& aY )
{
	return aX.has_value() ? *aX > aY : false;
}
template< typename tType > inline
bool operator> ( tType const& aX, optional<tType> const& aY )
{
	return aY.has_value() ? aX > *aY : true;
}
template< typename tType > inline 
bool operator>= ( optional<tType> const& aX, tType const& aY )
{
	return aX.has_value() ? *aX >= aY : false;
}
template< typename tType > inline
bool operator>= ( tType const& aX, optional<tType> const& aY )
{
	return aY.has_value() ? aX >= *aY : true;
}

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
