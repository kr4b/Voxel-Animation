/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- AnyT<> implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>

#include <flux/util/intent.hpp>
#include <flux/compat/type_traits.hpp>

#include <cstring>

#include "any-util.hxx"
#include "any-aspect.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    is_any<>                            ///{{{2///////////////////////////
template< typename tType >
struct is_any
	: std::false_type
{};

template< class tFeat, std::size_t tSize >
struct is_any< AnyT<tFeat,tSize> >
	: std::true_type
{};

//--    AnyT                                ///{{{2///////////////////////////
#define FLUXTMP_ANY_TEMPL_ template< class tFeatures, std::size_t tSize >
#define FLUXTMP_ANY_CLASS_ AnyT<tFeatures,tSize>

	// [cd]tor & assign --{{{3--
FLUXTMP_ANY_TEMPL_ inline
FLUXTMP_ANY_CLASS_::AnyT() noexcept
	: mMeta(nullptr, true)
{}

FLUXTMP_ANY_TEMPL_ inline
FLUXTMP_ANY_CLASS_::AnyT( AnyT&& aOther ) noexcept
	: mMeta(nullptr, true)
{
	if( aOther.mMeta.flag() )
	{
		std::memcpy( &mStore, &aOther.mStore, sizeof(void*) );

		using std::swap;
		swap( mMeta, aOther.mMeta );
	}
	else
	{
		aOther.mMeta.pointer()->move( &mStore, &aOther.mStore );
		mMeta = aOther.mMeta;
	}
}
FLUXTMP_ANY_TEMPL_ inline
auto FLUXTMP_ANY_CLASS_::operator= (AnyT&& aOther) noexcept -> AnyT&
{
	copy( std::move(aOther) );
	return *this;
}


FLUXTMP_ANY_TEMPL_ inline
FLUXTMP_ANY_CLASS_::AnyT( AnyT const& aOther )
	: mMeta(nullptr,true)
{
	copy( aOther );
}
FLUXTMP_ANY_TEMPL_ inline
auto FLUXTMP_ANY_CLASS_::operator= (AnyT const& aOther) -> AnyT&
{
	copy( aOther );
	return *this;
}


FLUXTMP_ANY_TEMPL_ template< typename tType, typename > inline
FLUXTMP_ANY_CLASS_::AnyT( tType&& aValue )
	: mMeta(nullptr,true)
{
	assign( std::forward<tType>(aValue) );
}
FLUXTMP_ANY_TEMPL_ template< typename tType, class tAspect, typename > inline
FLUXTMP_ANY_CLASS_::AnyT( tType&& aValue, Identity<tAspect> )
	: mMeta(nullptr,true)
{
	assign_with_aspect( std::forward<tType>(aValue), Identity<tAspect>{} );
}
FLUXTMP_ANY_TEMPL_ template< typename tType, typename > inline
auto FLUXTMP_ANY_CLASS_::operator= (tType&& aOther) -> AnyT&
{
	assign( std::forward<tType>(aOther) );
	return *this;
}


FLUXTMP_ANY_TEMPL_ template< class tOtherFeat, std::size_t tOtherSize > inline
FLUXTMP_ANY_CLASS_::AnyT( AnyT<tOtherFeat,tOtherSize>&& aOther )
	: mMeta(nullptr,true)
{
	copy( std::move(aOther) );
}
FLUXTMP_ANY_TEMPL_ template< class tOtherFeat, std::size_t tOtherSize > inline
auto FLUXTMP_ANY_CLASS_::operator= ( AnyT<tOtherFeat,tOtherSize>&& aOther ) -> AnyT&
{
	copy( std::move(aOther) );
	return *this;
}

FLUXTMP_ANY_TEMPL_ template< class tOtherFeat, std::size_t tOtherSize > inline 
FLUXTMP_ANY_CLASS_::AnyT( AnyT<tOtherFeat,tOtherSize> const& aOther )
	: mMeta(nullptr,true)
{
	copy( aOther );
}
FLUXTMP_ANY_TEMPL_ template< class tOtherFeat, std::size_t tOtherSize > inline
auto FLUXTMP_ANY_CLASS_::operator= ( AnyT<tOtherFeat,tOtherSize> const& aOther ) -> AnyT&
{
	copy( aOther );
	return *this;
}


FLUXTMP_ANY_TEMPL_ template< typename tType, typename... tArgs > inline
FLUXTMP_ANY_CLASS_::AnyT( Identity<tType>, tArgs&&... aArgs )
	: mMeta(nullptr,true)
{
	emplace<tType>( std::forward<tArgs>(aArgs)... );
}

FLUXTMP_ANY_TEMPL_ inline
FLUXTMP_ANY_CLASS_::~AnyT()
{
	clear();
}

	// public --{{{3--
FLUXTMP_ANY_TEMPL_ inline
void FLUXTMP_ANY_CLASS_::clear() 
{
	if( !empty() )
	{
		meta_()->destroy( ptr_() );
		mMeta.set_pointer( nullptr );

		if( mMeta.flag() )
		{
			::operator delete( ptr_() );
			mMeta.set_flag( false );
		}
	}
}

FLUXTMP_ANY_TEMPL_ inline
bool FLUXTMP_ANY_CLASS_::empty() const noexcept
{
	return !mMeta.pointer();
}
FLUXTMP_ANY_TEMPL_ inline
std::size_t FLUXTMP_ANY_CLASS_::size() const noexcept
{
	return empty() ? 0 : meta_()->size();
}
FLUXTMP_ANY_TEMPL_ inline
std::type_info const& FLUXTMP_ANY_CLASS_::type_info() const noexcept
{
	return empty() ? typeid(void) : meta_()->type_info();
}
FLUXTMP_ANY_TEMPL_ inline
std::type_index FLUXTMP_ANY_CLASS_::type_index() const noexcept
{
	return empty() ? std::type_index(typeid(void)) : meta_()->type_index();
}

FLUXTMP_ANY_TEMPL_ template< typename tType > inline
bool FLUXTMP_ANY_CLASS_::is() const noexcept
{
	return check_type_soft_<tType>();
}


FLUXTMP_ANY_TEMPL_ template< typename tType > inline
tType& FLUXTMP_ANY_CLASS_::get() &
{
	check_type_hard_<tType>();
	return get_unsafe<tType>();
}
FLUXTMP_ANY_TEMPL_ template< typename tType > inline
tType&& FLUXTMP_ANY_CLASS_::get() &&
{
	check_type_hard_<tType>();
	return move_unsafe<tType>();
}
FLUXTMP_ANY_TEMPL_ template< typename tType > inline
tType const& FLUXTMP_ANY_CLASS_::get() const&
{
	check_type_hard_<tType>();
	return get_unsafe<tType>();
}

FLUXTMP_ANY_TEMPL_ template< typename tType > inline
tType&& FLUXTMP_ANY_CLASS_::move()
{
	check_type_hard_<tType>();
	return move_unsafe<tType>();
}

FLUXTMP_ANY_TEMPL_ template< typename tType > inline
tType& FLUXTMP_ANY_CLASS_::get_unsafe() & noexcept
{
	return *ptr_typed_<tType>();
}
FLUXTMP_ANY_TEMPL_ template< typename tType > inline
tType&& FLUXTMP_ANY_CLASS_::get_unsafe() && noexcept
{
	return std::move(*ptr_typed_<tType>());
}
FLUXTMP_ANY_TEMPL_ template< typename tType > inline
tType const& FLUXTMP_ANY_CLASS_::get_unsafe() const& noexcept
{
	return *ptr_typed_<tType>();
}

FLUXTMP_ANY_TEMPL_ template< typename tType > inline
tType&& FLUXTMP_ANY_CLASS_::move_unsafe() noexcept
{
	return std::move(*ptr_typed_<tType>());
}

FLUXTMP_ANY_TEMPL_ template< typename tType > inline
void FLUXTMP_ANY_CLASS_::assign( tType&& aOther )
{
	using Base_ = detail::any_type_map_t<tType>;
	assign_with_aspect( std::forward<tType>(aOther), Identity<aspect::AnyTypeDefault<Base_>>{} );
}
FLUXTMP_ANY_TEMPL_ template< typename tType, class tAspect > inline
void FLUXTMP_ANY_CLASS_::assign_with_aspect( tType&& aOther, Identity<tAspect> )
{
	using Base_ = detail::any_type_map_t<tType>;

	detail::CheckAnyAspect<tAspect, tFeatures, Base_>::check(); /*TODO-implement*/

	/* The following is *probably* needed to handle stuff like
	 *
	 *  AnyT x = ...;
	 *  x = x.get<type>();
	 *
	 * Albeit, here x.get<type>() might have additional indirections (returned
	 * from a function ...)
	 */
	if( !empty() && ptr_() == &aOther )
		return;

	clear();

	detail::AnyConstruct<
		detail::AnyInPlaceEligible<Base_,tSize,kAnyDefaultAlign>::value
	>::template construct<tAspect,Base_>( mStore, std::forward<tType>(aOther) );

	mMeta.set_flag( !detail::AnyInPlaceEligible<Base_,tSize,kAnyDefaultAlign>::value );
	mMeta.set_pointer( detail::any_get_meta<Base_,tAspect>() );
}


FLUXTMP_ANY_TEMPL_ inline
void FLUXTMP_ANY_CLASS_::copy( AnyT&& aOther ) noexcept
{
	FLUX_ASSERT( this != &aOther );

	Buffer_ tmp;
	if( mMeta.flag() )
		std::memcpy( &tmp, &mStore, sizeof(void*) );
	else
		meta_()->relocate( &tmp, &mStore );

	using std::swap;
	swap( mMeta, aOther.mMeta );

	if( mMeta.flag() )
		std::memcpy( &mStore, &aOther.mStore, sizeof(void*) );
	else
		meta_()->relocate( &mStore, &aOther.mStore );
	
	if( aOther.mMeta.flag() )
		std::memcpy( &aOther.mStore, &tmp, sizeof(void*) );
	else
		aOther.mMeta.pointer()->relocate( &aOther.mStore, &tmp );
}
FLUXTMP_ANY_TEMPL_ inline
void FLUXTMP_ANY_CLASS_::copy( AnyT const& aOther )
{
	if( this == &aOther )
		return;

	clear();

	if( aOther.empty() )
		return;

	mMeta = aOther.mMeta;
	if( mMeta.flag() )
	{
		using VoidPtr_ = void*;
		new (&mStore) VoidPtr_( ::operator new( meta_()->size() ) );
	}

	meta_()->copy( ptr_(), aOther.ptr_() );
}

FLUXTMP_ANY_TEMPL_ template< class tOtherFeat, std::size_t tOtherSize > inline
void FLUXTMP_ANY_CLASS_::copy( AnyT<tOtherFeat,tOtherSize>&& aOther )
{
	clear();

	if( aOther.empty() )
		return;

	if( !meta::set_contains_set<tOtherFeat,tFeatures>::value )
	{
		constexpr auto featMask = detail::AnyFeatureSetToMask<tFeatures>::mask;
		if( featMask != (featMask & aOther.meta_()->features()) )
			detail::throw_any_type_incompat( featMask, aOther.meta_() );
	}

	mMeta = aOther.mMeta;
	if( mMeta.flag() )
	{
		std::memcpy( &mStore, &aOther.mStore, sizeof(void*) );

		aOther.mMeta.set_flag( false );
		aOther.mMeta.set_pointer( nullptr );
	}
	else
	{
		auto const size = meta_()->size();
		if( size > tSize || (kAnyDefaultAlign%meta_()->align()) != 0 )
		{
			using VoidPtr_ = void*;
			new (&mStore) VoidPtr_( ::operator new( size ) );
		}

		meta_()->move( ptr_(), aOther.ptr_() );
	}
}
FLUXTMP_ANY_TEMPL_ template< class tOtherFeat, std::size_t tOtherSize > inline
void FLUXTMP_ANY_CLASS_::copy( AnyT<tOtherFeat,tOtherSize> const& aOther )
{
	clear();

	if( aOther.empty() )
		return;

	if( !meta::set_contains_set<tOtherFeat,tFeatures>::value )
	{
		constexpr auto featMask = detail::AnyFeatureSetToMask<tFeatures>::mask;
		if( featMask != (featMask & aOther.meta_()->features()) )
			detail::throw_any_type_incompat( featMask, aOther.meta_() );
	}
	mMeta = aOther.mMeta;

	auto const size = meta_()->size();
	if( size > tSize  || (kAnyDefaultAlign%meta_()->align()) != 0 )
	{
		using VoidPtr_ = void*;
		new (&mStore) VoidPtr_( ::operator new( size ) );
		mMeta.set_flag( true );
	}
	else
	{
		mMeta.set_flag( false );
	}

	meta_()->copy( ptr_(), aOther.ptr_() );
}

FLUXTMP_ANY_TEMPL_ template< typename tType, typename... tArgs > inline
void FLUXTMP_ANY_CLASS_::emplace( tArgs&&... aArgs )
{
	clear();

	using Base_ = detail::any_type_map_t<tType>;
	using Aspect_ = aspect::AnyTypeDefault<Base_>;

	detail::AnyConstruct<
		detail::AnyInPlaceEligible<Base_,tSize,kAnyDefaultAlign>::value
	>::template construct<Aspect_,Base_>( mStore, std::forward<tArgs>(aArgs)... );

	mMeta.set_flag( !detail::AnyInPlaceEligible<Base_,tSize,kAnyDefaultAlign>::value );
	mMeta.set_pointer( detail::any_get_meta<Base_,Aspect_>() );
}

	// private functions --{{{3--
FLUXTMP_ANY_TEMPL_ inline
detail::AnyMeta const* FLUXTMP_ANY_CLASS_::meta_() const noexcept
{
	return mMeta.pointer();
}

FLUXTMP_ANY_TEMPL_ inline
void* FLUXTMP_ANY_CLASS_::ptr_() noexcept
{
	if( !mMeta.flag() )
		return &mStore;

	void* ptr;
	std::memcpy( &ptr, &mStore, sizeof(void*) );
	return ptr;
}
FLUXTMP_ANY_TEMPL_ inline
void const* FLUXTMP_ANY_CLASS_::ptr_() const noexcept
{
	if( !mMeta.flag() )
		return &mStore;

	void* ptr;
	std::memcpy( &ptr, &mStore, sizeof(void*) );
	return ptr;
}


FLUXTMP_ANY_TEMPL_ template< typename tType > inline
bool FLUXTMP_ANY_CLASS_::check_type_soft_() const noexcept
{
	if( empty() )
		return std::is_same<tType,void>::value;

	return typeid(tType) == meta_()->type_info();
}
FLUXTMP_ANY_TEMPL_ template< typename tType > inline
void FLUXTMP_ANY_CLASS_::check_type_hard_() const
{
	if( !check_type_soft_<tType>() )
	{
		detail::throw_any_type_mismatch( held_type_name_(), typeid(tType).name() );
		FLUX_INTENT_UNREACHABLE();
	}
}


FLUXTMP_ANY_TEMPL_ template< typename tType > inline
tType* FLUXTMP_ANY_CLASS_::ptr_typed_() noexcept
{
	return compat::launder( static_cast<tType*>(ptr_()) );
}
FLUXTMP_ANY_TEMPL_ template< typename tType > inline
tType const* FLUXTMP_ANY_CLASS_::ptr_typed_() const noexcept
{
	return compat::launder( static_cast<tType const*>(ptr_()) );
}


FLUXTMP_ANY_TEMPL_ inline 
char const* FLUXTMP_ANY_CLASS_::held_type_name_() const noexcept
{
	return empty() ? typeid(void).name() : meta_()->type_info().name();
}


FLUXTMP_ANY_TEMPL_ inline
ftl::Hash FLUXTMP_ANY_CLASS_::hash_() const
{
	return empty() ? ftl::Hash(0) : meta_()->hash( ptr_() );
}

#undef FLUXTMP_ANY_CLASS_
#undef FLUXTMP_ANY_TEMPL_

//--    operators                           ///{{{2///////////////////////////

	// equality AnyT
template< class tLFeat, std::size_t tLSize, class tRFeat, std::size_t tRSize >
FLUX_ENABLE_IF( detail::AnyBothHaveEqual_< tLFeat, tRFeat > )
(bool) operator== (AnyT<tLFeat,tLSize> const& aLHS, AnyT<tRFeat,tRSize> const& aRHS)
{
	return aLHS.equals( aRHS );
}
template< class tLFeat, std::size_t tLSize, class tRFeat, std::size_t tRSize >
FLUX_ENABLE_IF( detail::AnyBothHaveEqual_< tLFeat, tRFeat > )
(bool) operator!= (AnyT<tLFeat,tLSize> const& aLHS, AnyT<tRFeat,tRSize> const& aRHS)
{
	return !aLHS.equals( aRHS );
}


	// equality with tType
template< typename tType, class tFeat, std::size_t tSize > inline
FLUX_ENABLE_IF( meta::set_contains_element< tFeat, aspect::AnyFeatEqual > )
(bool) operator== (tType const& aX, AnyT<tFeat,tSize> const& aY)
{
	return aY.equals( aX );
}
template<class tFeat, std::size_t tSize, typename tType > inline
FLUX_ENABLE_IF( meta::set_contains_element< tFeat, aspect::AnyFeatEqual > )
(bool) operator== (AnyT<tFeat,tSize> const& aX, tType const& aY)
{
	return aX.equals( aY );
}

template< typename tType, class tFeat, std::size_t tSize > inline
FLUX_ENABLE_IF( meta::set_contains_element< tFeat, aspect::AnyFeatEqual > )
(bool) operator!= (tType const& aX, AnyT<tFeat,tSize> const& aY)
{
	return !aY.equals( aX );
}
template< class tFeat, std::size_t tSize, typename tType > inline
FLUX_ENABLE_IF( meta::set_contains_element< tFeat, aspect::AnyFeatEqual > )
(bool) operator!= (AnyT<tFeat,tSize> const& aX, tType const& aY)
{
	return !aX.equals( aY );
}

	// ordering with tType
template< typename tType, class tFeat, std::size_t tSize > inline
FLUX_ENABLE_IF( meta::set_contains_element< tFeat, aspect::AnyFeatLess > )
(bool) operator<= (tType const& aX, AnyT<tFeat,tSize> const& aY)
{
	return !aY.less( aX );
}
template< class tFeat, std::size_t tSize, typename tType > inline
FLUX_ENABLE_IF( meta::set_contains_element< tFeat, aspect::AnyFeatLess > )
(bool) operator< (AnyT<tFeat,tSize> const& aX, tType const& aY)
{
	return aX.less( aY );
}

//--    any_cast<>()                        ///{{{2///////////////////////////
template< typename tType, class tFeat, std::size_t tSize > inline
tType any_cast( AnyT<tFeat,tSize>& aAny )
{
	using Type_ = compat::remove_reference_t<tType>;
	return aAny.template get<Type_>();
}

template< typename tType, class tFeat, std::size_t tSize > inline 
tType any_cast( AnyT<tFeat,tSize>&& aAny )
{
	using Type_ = compat::remove_reference_t<tType>;
	return aAny.template move<Type_>();
}
template< typename tType, class tFeat, std::size_t tSize > inline
tType any_cast( AnyT<tFeat,tSize> const& aAny )
{
	using Type_ = compat::remove_reference_t<tType>;
	return aAny.template get<Type_>();
}

template< typename tType, class tFeat, std::size_t tSize > inline
tType* any_cast( AnyT<tFeat,tSize>* aAny ) noexcept
{
	if( !aAny || !aAny->template is<tType>() )
		return nullptr;

	return &aAny->template get_unsafe<tType>();
}
template< typename tType, class tFeat, std::size_t tSize > inline
tType const* any_cast( AnyT<tFeat,tSize> const* aAny ) noexcept
{
	if( !aAny || !aAny->template is<tType>() )
		return nullptr;

	return &aAny->template get_unsafe<tType>();
}

//--    *hash()                             ///{{{2///////////////////////////
template< class tFeat, std::size_t tSize >
FLUX_ENABLE_IF( meta::set_contains_element< tFeat, aspect::AnyFeatHash > )
(ftl::Hash) any_value_hash( AnyT<tFeat,tSize> const& aX )
{
	return aX.hash_();
}

template< class tFeat, std::size_t tSize >
FLUX_ENABLE_IF( meta::set_contains_element< tFeat, aspect::AnyFeatHash > )
(ftl::Hash) hash( AnyT<tFeat,tSize> const& aX )
{
	using ftl::hash_combine;
	return hash_combine( aX.type_info().hash_code(), any_value_hash(aX) );
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
