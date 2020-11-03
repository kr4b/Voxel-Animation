/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	AnyT<> utilities & helpers
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_2B2DB7B5_F567_4FFB_A223_4FA427F7FB51
#define DETAIL_2B2DB7B5_F567_4FFB_A223_4FA427F7FB51

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/std/assert.hpp>

#include <flux/ftl/hash.hpp>
#include <flux/ftl/meta_set.hpp>
#include <flux/ftl/uninitialized.hpp>

#include <flux/compat/launder.hpp>
#include <flux/compat/constexpr.hpp>

#include <flux/util/returns.hpp>

#include <typeinfo>

//TODO
//TODO
//TODO   make Meta::move() NOEXCEPT or a hard error when called.
//TODO
//TODO

//--    >>> namespace = flux::ftl::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,detail)
//--    Any features                        ///{{{2///////////////////////////

struct AnyFeatEqual_ { static constexpr unsigned mask = (1u<<0); };
struct AnyFeatLess_ { static constexpr unsigned mask = (1u<<1); };
struct AnyFeatHash_ { static constexpr unsigned mask = (1u<<2); };

//--    AnyMeta                             ///{{{2///////////////////////////
class AnyMeta 
{
	public:
		virtual ~AnyMeta() = 0;

	public:
		virtual std::size_t size() const noexcept = 0;
		virtual std::size_t align() const noexcept = 0;

		virtual std::type_index type_index() const noexcept = 0;
		virtual std::type_info const& type_info() const noexcept = 0;

		virtual unsigned features() const noexcept = 0;
	
		virtual void destroy( void* ) const noexcept = 0;

		virtual void move( void*, void* ) const noexcept = 0;
		virtual void relocate( void*, void* ) const noexcept = 0;
		
		virtual void copy( void*, void const* ) const = 0;

		virtual bool equal( void const*, void const* ) const = 0;
		virtual bool less( void const*, void const* ) const = 0;
		virtual ftl::Hash hash( void const* ) const = 0;
};

	// implementation -{{{3-
inline AnyMeta::~AnyMeta() = default;

//--    any_meta_maybe_*_()                 ///{{{2///////////////////////////
template< typename tType > inline
void any_meta_maybe_move_( tType* aDest, tType* aSrc, std::true_type ) noexcept
{
	using Mut_ = compat::remove_cv_t<tType>;
	ftl::uninitialized_move( aSrc, aSrc+1, const_cast<Mut_*>(aDest) );
}
template< typename tType > inline
void any_meta_maybe_move_( tType*, tType*, std::false_type ) noexcept
{
	FLUX_ASSERT( false && "Any: move() on non-noexcept movable type." );
}

template< typename tType > inline
void any_meta_maybe_relocate_( tType* aDest, tType* aSrc, std::true_type ) noexcept
{
	using Mut_ = compat::remove_cv_t<tType>;
	ftl::uninitialized_relocate( aSrc, aSrc+1, const_cast<Mut_*>(aDest) );
}
template< typename tType > inline
void any_meta_maybe_relocate_( tType*, tType*, std::false_type ) noexcept
{
	FLUX_ASSERT( false && "Any: relocate() on non-noexcept movable type." );
}


template< class tAspect, typename tType > inline FLUX_CONSTEXPR_EX
auto any_meta_maybe_equal_( tType const& aLHS, tType const& aRHS, std::true_type )
FLUX_RETURNS( tAspect::equal( aLHS, aRHS ) )

template< class tAspect, typename tType > inline constexpr
bool any_meta_maybe_equal_( tType const&, tType const&, std::false_type ) noexcept
{
	return false;
}

template< class tAspect, typename tType > inline FLUX_CONSTEXPR_EX
auto any_meta_maybe_less_( tType const& aLHS, tType const& aRHS, std::true_type )
FLUX_RETURNS( tAspect::less( aLHS, aRHS ) )

template< class tAspect, typename tType > inline constexpr
bool any_meta_maybe_less_( tType const&, tType const&, std::false_type ) noexcept
{
	return false;
}

template< class tAspect, typename tType > inline FLUX_CONSTEXPR_EX
auto any_meta_maybe_hash_( tType const& aVal, std::true_type )
FLUX_RETURNS( tAspect::hash( aVal ) )

template< class tAspect, typename tType > inline constexpr
ftl::Hash any_meta_maybe_hash_( tType const&, std::false_type ) noexcept
{
	return ftl::Hash();
}

//--    AnyFeatureSetToMask                 ///{{{2///////////////////////////
template< class tSet >
struct AnyFeatureSetToMask;

template< typename tHead, typename... tTail >
struct AnyFeatureSetToMask< meta::Set<tHead,tTail...> >
{
	static constexpr unsigned mask =
		tHead::mask | AnyFeatureSetToMask< meta::Set<tTail...> >::mask
	;
};
template<>
struct AnyFeatureSetToMask< meta::Set<> >
{
	static constexpr unsigned mask = 0u;
};

//--    AnyMetaT                            ///{{{2///////////////////////////
template< typename tType, class tAspect >
struct AnyMetaT final : AnyMeta
{
	//virtual ~AnyMetaT();
	
	std::size_t size() const noexcept override;
	std::size_t align() const noexcept override;

	std::type_index type_index() const noexcept override;
	std::type_info const& type_info() const noexcept override;

	unsigned features() const noexcept override;

	void destroy( void* ) const noexcept override;

	void move( void*, void* ) const noexcept override;
	void relocate( void*, void* ) const noexcept override;

	void copy( void*, void const* ) const override;

	bool equal( void const*, void const* ) const override;
	bool less( void const*, void const* ) const override;
	ftl::Hash hash( void const* ) const override;
};

	// implementation -{{{3-
/*template< typename tType, class tAspect > inline
AnyMetaT<tType,tAspect>::~AnyMetaT()
{}*/

template< typename tType, class tAspect > inline
std::size_t AnyMetaT<tType,tAspect>::size() const noexcept
{
	return sizeof(tType);
}
template< typename tType, class tAspect > inline
std::size_t AnyMetaT<tType,tAspect>::align() const noexcept
{
	return alignof(tType);
}

template< typename tType, class tAspect > inline
std::type_index AnyMetaT<tType,tAspect>::type_index() const noexcept
{
	return std::type_index(typeid(tType));
}
template< typename tType, class tAspect > inline
std::type_info const& AnyMetaT<tType,tAspect>::type_info() const noexcept
{
	return typeid(tType);
}

template< typename tType, class tAspect > inline
unsigned AnyMetaT<tType,tAspect>::features() const noexcept
{
	return AnyFeatureSetToMask<typename tAspect::Features>::mask;
}

template< typename tType, class tAspect > inline
void AnyMetaT<tType,tAspect>::destroy( void* aPtr ) const noexcept
{
	auto ptr = static_cast<tType*>(aPtr);

	tAspect::destroy( compat::launder(ptr) );
	//tAspect::destroy( ptr );
}

template< typename tType, class tAspect > inline
void AnyMetaT<tType,tAspect>::move( void* aDst, void* aSrc ) const noexcept
{
	auto dst = static_cast<tType*>(aDst);
	auto src = static_cast<tType*>(aSrc);
	any_meta_maybe_move_( compat::launder(dst), compat::launder(src), std::integral_constant<bool,std::is_nothrow_move_constructible<tType>::value>{} /*XXX-check if this is the correct option*/ );
}
template< typename tType, class tAspect > inline
void AnyMetaT<tType,tAspect>::relocate( void* aDst, void* aSrc ) const noexcept
{
	auto dst = static_cast<tType*>(aDst);
	auto src = static_cast<tType*>(aSrc);
	any_meta_maybe_relocate_( compat::launder(dst), compat::launder(src), std::integral_constant<bool,std::is_nothrow_move_constructible<tType>::value>{} /* TODO: is_nothrow_relocatable<> or whatever */ );
}


template< typename tType, class tAspect > inline
void AnyMetaT<tType,tAspect>::copy( void* aDst, void const* aSrc ) const
{
	auto src = static_cast<tType const*>(aSrc);

	FLUX_ASSERT( src );
	tAspect::construct( aDst, *compat::launder(src) );
}

template< typename tType, class tAspect > inline
bool AnyMetaT<tType,tAspect>::equal( void const* aLHS, void const* aRHS ) const
{
	auto lhs = static_cast<tType const*>(aLHS);
	auto rhs = static_cast<tType const*>(aRHS);

	FLUX_ASSERT( lhs && rhs );
	/* Workaround: the below line ICEs MSVC 19.14.26433 under some conditions. */
#	if FLUX_COMPILER != FLUX_COMPILER_MSVC || _MSC_FULL_VER != 191426433
	return any_meta_maybe_equal_<tAspect>( *compat::launder(lhs), *compat::launder(rhs), typename meta::set_contains_element<typename tAspect::Features, AnyFeatEqual_>::type{} );
#	else
	return any_meta_maybe_equal_<tAspect>( *lhs, *rhs, typename meta::set_contains_element<typename tAspect::Features, AnyFeatEqual_>::type{} );
#	endif
}
template< typename tType, class tAspect > inline
bool AnyMetaT<tType,tAspect>::less( void const* aLHS, void const* aRHS ) const
{
	auto lhs = static_cast<tType const*>(aLHS);
	auto rhs = static_cast<tType const*>(aRHS);

	FLUX_ASSERT( lhs && rhs );
	/* Workaround: the below line ICEs MSVC 19.14.26433 under some (other) conditions. */
#	if FLUX_COMPILER != FLUX_COMPILER_MSVC || _MSC_FULL_VER != 191426433
	return any_meta_maybe_less_<tAspect>( *compat::launder(lhs), *compat::launder(rhs), typename meta::set_contains_element<typename tAspect::Features, AnyFeatLess_>::type{} );
#	else
	return any_meta_maybe_less_<tAspect>( *lhs, *rhs, typename meta::set_contains_element<typename tAspect::Features, AnyFeatLess_>::type{} );
#	endif
}

template< typename tType, class tAspect > inline
ftl::Hash AnyMetaT<tType,tAspect>::hash( void const* aPtr ) const
{
	auto ptr = static_cast<tType const*>(aPtr);

	FLUX_ASSERT( ptr );
	return any_meta_maybe_hash_<tAspect>( *compat::launder(ptr), typename meta::set_contains_element<typename tAspect::Features, AnyFeatHash_>::type{} );
}

//--    AnyMetaImmortal                     ///{{{2///////////////////////////
template< class tMeta >
struct AnyMetaImmortal
{
	/* This object ensures that the tMeta instance remains alive forever; this
	 * ensures that the Any<> may be used even during program shutdown.
	 *
	 * I'm not entirely sure how conformant this is, since technically the
	 * lifetime of the "mStore" ends when the AnyMetaImmortal destructor runs.
	 * On the other hand, it should be a trivial object.
	 */
	
	AnyMetaImmortal() noexcept
	{
		new (&mStore) tMeta;
	}
	~AnyMetaImmortal()
	{
		// don't destroy tMeta!
	}

	FLUX_CONSTEXPR_EX
	tMeta const* obj() const noexcept
	{
		auto ptr = reinterpret_cast<tMeta const*>(&mStore);
		return compat::launder(ptr);
	}

	private: typename std::aligned_storage<
		sizeof(tMeta),
		alignof(tMeta)
	>::type mStore;
};

//--    any_get_meta()                      ///{{{2///////////////////////////
template< typename tType, class tAspect > inline
AnyMeta const* any_get_meta() noexcept
{
	static AnyMetaImmortal< AnyMetaT<tType,tAspect> > const sInstance_;
	return sInstance_.obj();
}

//--    AnyInjectNull                       ///{{{2///////////////////////////
template< class tFeature >
struct AnyInjectNull_
{};

//--    AnyInjectExtra                      ///{{{2///////////////////////////
template< class tFeature, class, template<class,std::size_t> class >
struct AnyInjectExtra;

//--    AnyInjectExtra - EQUAL              ///{{{2///////////////////////////
template< class tAny, template<class,std::size_t> class tAnyT >
struct AnyInjectExtra< detail::AnyFeatEqual_, tAny, tAnyT >
{
	template< typename tType >
	bool equals( tType const& ) const;
	template< class tFeat, std::size_t tSize >
	bool equals( tAnyT<tFeat,tSize> const& ) const;
};

template< class tAny, template<class,std::size_t> class tAnyT > template< typename tType > inline
bool AnyInjectExtra<detail::AnyFeatEqual_,tAny,tAnyT>::equals( tType const& aValue ) const
{
	tAny const* self = static_cast<tAny const*>(this);
	return self->template is<tType>() 
		&& self->meta_()->equal( self->ptr_(), &aValue )
	;
}
template< class tAny, template<class,std::size_t> class tAnyT > template< class tFeat, std::size_t tSize > inline
bool AnyInjectExtra<detail::AnyFeatEqual_,tAny,tAnyT>::equals( tAnyT<tFeat,tSize> const& aAny ) const
{
	tAny const* self = static_cast<tAny const*>(this);
	return (self->empty() && aAny.empty()) || (self->type_info() == aAny.type_info()
		&& self->meta_()->equal( self->ptr_(), aAny.ptr_() ))
	;
}

//--    AnyInjectExtra - LESS               ///{{{2///////////////////////////
template< class tAny, template<class,std::size_t> class tAnyT >
struct AnyInjectExtra< detail::AnyFeatLess_, tAny, tAnyT >
{
	template< typename tType >
	bool less( tType const& ) const;
	template< class tFeat, std::size_t tSize >
	bool less( tAnyT<tFeat,tSize> const& ) const;
};

template< class tAny, template<class,std::size_t> class tAnyT > template< typename tType > inline
bool AnyInjectExtra<detail::AnyFeatLess_,tAny,tAnyT>::less( tType const& aValue ) const
{
	tAny const* self = static_cast<tAny const*>(this);
	if( self->empty() )
		return false;
	
	auto const& lti = self->type_info();
	auto const& rti = typeid(tType);

	return (lti != rti)
		? lti.before(rti)
		: self->meta_()->less( self->ptr_(), &aValue )
	;
}
template< class tAny, template<class,std::size_t> class tAnyT > template< class tFeat, std::size_t tSize > inline
bool AnyInjectExtra<detail::AnyFeatLess_,tAny,tAnyT>::less( tAnyT<tFeat,tSize> const& aAny ) const
{
	tAny const* self = static_cast<tAny const*>(this);

	bool const sempty = self->empty();
	bool const aempty = aAny.empty();
	if( sempty || aempty )
		return !sempty && aempty;

	auto const& lti = self->type_info();
	auto const& rti = aAny.type_info();
	return (lti != rti)
		? lti.before(rti)
		: self->meta_()->less( self->ptr_(), aAny.ptr_() )
	;
}

//--    AnyExtras                           ///{{{2///////////////////////////
template< template<class,std::size_t> class tAnyT, class tFeat, std::size_t tSize >
struct AnyExtras
	: std::conditional< meta::set_contains_element<tFeat,detail::AnyFeatEqual_>::value,
			AnyInjectExtra< detail::AnyFeatEqual_, tAnyT<tFeat,tSize>, tAnyT >,
			AnyInjectNull_< detail::AnyFeatEqual_ >
		>::type
	, std::conditional< meta::set_contains_element<tFeat,detail::AnyFeatLess_>::value,
			AnyInjectExtra< detail::AnyFeatLess_, tAnyT<tFeat,tSize>, tAnyT >,
			AnyInjectNull_< detail::AnyFeatLess_ >
		>::type
{};

//--    <<< ~ flux::ftl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_2B2DB7B5_F567_4FFB_A223_4FA427F7FB51
