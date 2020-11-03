/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	observer_ptr implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>
#include <flux/compat/constexpr.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    observer_ptr                        ///{{{2///////////////////////////
template< class tW > inline constexpr
observer_ptr<tW>::observer_ptr() noexcept
	: mPtr(nullptr)
{}
template< class tW > inline constexpr
observer_ptr<tW>::observer_ptr( std::nullptr_t ) noexcept
	: mPtr(nullptr)
{}

template< class tW > inline FLUX_CONSTEXPR_EX
observer_ptr<tW>::observer_ptr( element_type* aPtr ) noexcept
	: mPtr(aPtr)
{}

template< class tW > template< class tOther > inline FLUX_CONSTEXPR_EX
observer_ptr<tW>::observer_ptr( observer_ptr<tOther> aOther ) noexcept
	: mPtr(aOther.get())
{}


template< class tW > inline constexpr
observer_ptr<tW>::operator bool() const noexcept
{
	return !!mPtr;
}
template< class tW > inline constexpr
observer_ptr<tW>::operator element_type*() const noexcept
{
	return mPtr;
}

template< class tW > inline constexpr
auto observer_ptr<tW>::operator*() const -> Ref_
{
	return FLUX_ASSERT( mPtr ), *mPtr;
}

template< class tW > inline constexpr
auto observer_ptr<tW>::operator->() const noexcept -> element_type*
{
	return mPtr;
}


template< class tW > inline FLUX_CONSTEXPR_EX
auto observer_ptr<tW>::release() noexcept -> element_type*
{
	auto ret = mPtr;
	mPtr = nullptr;
	return ret;
}
template< class tW > inline FLUX_CONSTEXPR_EX
void observer_ptr<tW>::reset( element_type* aPtr ) noexcept
{
	mPtr = aPtr;
}
template< class tW > inline FLUX_CONSTEXPR_EX
void observer_ptr<tW>::swap( observer_ptr& aOther ) noexcept
{
	std::swap( mPtr, aOther.mPtr );
}

template< class tW > inline constexpr
auto observer_ptr<tW>::get() const noexcept -> element_type*
{
	return mPtr;
}

//--    make_observer()                     ///{{{2///////////////////////////
template< class tW > inline
observer_ptr<tW> make_observer( tW* aPtr ) noexcept
{
	return observer_ptr<tW>(aPtr);
}

//--    swap()                              ///{{{2///////////////////////////
template< class tW > inline
void swap( observer_ptr<tW>& aX, observer_ptr<tW>& aY ) noexcept
{
	aX.swap( aY );
}


//--    comparsions                         ///{{{2///////////////////////////
template< class tX, class tY > inline
bool operator== (observer_ptr<tX> const& aX, observer_ptr<tY> const& aY) noexcept
{
	return aX.get() == aY.get();
}
template< class tX, class tY > inline
bool operator!= (observer_ptr<tX> const& aX, observer_ptr<tY> const& aY) noexcept
{
	return aX.get() != aY.get();
}

template< class tX > inline
bool operator== (observer_ptr<tX> const& aX, std::nullptr_t) noexcept
{
	return !aX.get();
}
template< class tX > inline
bool operator== (std::nullptr_t, observer_ptr<tX> const& aX) noexcept
{
	return !aX.get();
}
template< class tX > inline
bool operator!= (observer_ptr<tX> const& aX, std::nullptr_t) noexcept
{
	return !!aX.get();
}
template< class tX > inline
bool operator!= (std::nullptr_t, observer_ptr<tX> const& aX) noexcept
{
	return !!aX.get();
}

template< class tX, class tY > inline
bool operator< (observer_ptr<tX> const& aX, observer_ptr<tY> const& aY) noexcept
{
	// Z_ should be the "composite pointer type", but that seems to be a bit of
	// a pain. I'm not 100% sure that the common_type<> is the correct thing 
	// here.
	using Z_ = typename std::common_type<tX*,tY*>::type;
	return std::less<Z_>{}( aX.get(), aY.get() );
}
template< class tX, class tY > inline
bool operator> (observer_ptr<tX> const& aX, observer_ptr<tY> const& aY) noexcept
{
	return aY < aX;
}
template< class tX, class tY > inline
bool operator<= (observer_ptr<tX> const& aX, observer_ptr<tY> const& aY) noexcept
{
	return !(aY < aX);
}
template< class tX, class tY > inline
bool operator>= (observer_ptr<tX> const& aX, observer_ptr<tY> const& aY) noexcept
{
	return !(aX < aY);
}

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
