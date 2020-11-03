/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Small vector implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>

#include <flux/compat/constexpr.hpp>

#include <flux/util/narrow.hpp>

#include <cmath>
#include <algorithm>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::fml >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	// Vector*OpAspectMap
	template< class tOp > struct VectorUOpAspectMap {
		using Aspect = aspect::VectorAspect;
	};

	template< class tOp, class tRHS > struct VectorBOpAspectMap;
	template< class tOp >
	struct VectorBOpAspectMap< tOp, aspect::VectorAspect > {
		using Aspect = aspect::VectorAspect;
	};
	template<>
	struct VectorBOpAspectMap< op::ClassAdd, aspect::PositionAspect > {
		using Aspect = aspect::PositionAspect;
	};

	// PositionOpAspectMap
	template< class tOp > struct PositionUOpAspectMap;
	template<> struct PositionUOpAspectMap< op::ClassNorm > {
		using Aspect = aspect::PositionAspect;
	};
	template<> struct PositionUOpAspectMap< op::ClassBound > {
		using Aspect = aspect::PositionAspect;
	};
	template<> struct PositionUOpAspectMap< op::ClassUnaryPM > {
		using Aspect = aspect::VectorAspect;
	};
	
	template< class tOp, class tRHS > struct PositionBOpAspectMap;
	template<>
	struct PositionBOpAspectMap< op::ClassAdd, aspect::PositionAspect > {
		using Aspect = aspect::VectorAspect;
	};
	template<>
	struct PositionBOpAspectMap< op::ClassCmp, aspect::PositionAspect > {
		using Aspect = aspect::PositionAspect;
	};
	template<>
	struct PositionBOpAspectMap< op::ClassBound, aspect::PositionAspect > {
		using Aspect = aspect::PositionAspect;
	};
	template<>
	struct PositionBOpAspectMap< op::ClassAdd, aspect::VectorAspect > {
		using Aspect = aspect::PositionAspect;
	};
	template<>
	struct PositionBOpAspectMap< op::ClassInc, aspect::VectorAspect > {
		using Aspect = aspect::PositionAspect;
	};
	template<>
	struct PositionBOpAspectMap< op::ClassProj, aspect::VectorAspect > {
		using Aspect = aspect::PositionAspect;
	};

	// helpers
	template< typename tType > inline constexpr
	tType svec_sign_( tType aX ) noexcept
	{
		return aX >= tType(0) ? tType(1) : tType(-1);
	}
}

//--    aspect::*Aspect                     ///{{{2///////////////////////////
namespace aspect
{
	struct VectorAspect 
	{
		template< class tOp >
		using UOpAspectMap = detail::VectorUOpAspectMap<tOp>;
		template< class tOp, class tRHS >
		using BOpAspectMap = detail::VectorBOpAspectMap<tOp,tRHS>;

		//TODO: static make_identity(); ???
	};
	struct PositionAspect 
	{
		template< class tOp >
		using UOpAspectMap = detail::PositionUOpAspectMap<tOp>;
		template< class tOp, class tRHS >
		using BOpAspectMap = detail::PositionBOpAspectMap<tOp,tRHS>;
		
		//TODO: static make_identity(); ???
	};
}

//--    is_small_vector                     ///{{{2///////////////////////////
template< class tSize, typename T, class tSpec >
struct is_small_vector< SmallVector<tSize, T, tSpec>, 0 > 
	: std::true_type
{};

template< std::size_t tFixedSize, typename T, class tSpec >
struct is_small_vector< SmallVector<ftl::StaticSize<tFixedSize>, T, tSpec>, tFixedSize > 
	: std::true_type
{};

//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	// Helpers: accessors
	template< class tSize, typename T, class tSpec >
	struct SVecType< SmallVector<tSize, T, tSpec> >
	{
		using type = T;
	};

	template< class tSize, typename T, class tSpec >
	struct SVecSize< SmallVector<tSize, T, tSpec> >
	{
		using type = tSize;
	};

	template< class tSize, typename T, class tSpec >
	struct SVecSpec< SmallVector<tSize, T, tSpec> >
	{
		using type = tSpec;
	};

	template< class tSize, typename T, class tSpec >
	struct SVecSpace< SmallVector<tSize, T, tSpec> >
	{
		using type = typename tSpec::Space;
	};

	template< class tSize, typename T, class tSpec >
	struct SVecAspect< SmallVector<tSize, T, tSpec> >
	{
		using type = typename tSpec::Aspect;
	};

	template< class tVec, class tAspect >
	struct svec_is_aspect
		: std::is_same< svec_aspect_t<tVec>, tAspect >
	{};

	template< class tSize, typename T, class tSpec, class tSpace >
	struct SVecRebind<SmallVector<tSize,T,tSpec>,tSpace>
	{
		using spec = aspect::VectorSpec<typename tSpec::Aspect,tSpace>;
		using type = SmallVector<tSize,T,spec>;
	};

	// Helpers: checks
	template< class tOp, class tSA >
	struct SVecUOpSpec< tOp, tSA, compat::void_t<
		typename tSA::Aspect, typename tSA::Space,
		typename tSA::Aspect::template UOpAspectMap<tOp>::Aspect
	> > {
		using type = aspect::VectorSpec<
			typename tSA::Aspect::template UOpAspectMap<tOp>::Aspect,
			typename tSA::Space
		>;
	};
	template< class tOp, class tSA, class tSB >
	struct SVecBOpSpec< tOp, tSA, tSB, compat::void_t<
		typename tSA::Aspect, typename tSA::Space,
		typename tSB::Aspect, typename tSB::Space,
		FLUX_ENABLE_IF(space_compatible<typename tSA::Space,typename tSB::Space>)(int),
		typename tSA::Aspect::template BOpAspectMap<tOp,typename tSB::Aspect>::Aspect
	> > {
		using type = aspect::VectorSpec<
			typename tSA::Aspect::template BOpAspectMap<tOp,typename tSB::Aspect>::Aspect,
			space_specialized_t<typename tSA::Space,typename tSB::Space>
		>;
	};


	// Check if two SmallVectors are compatible.
	template< class, class, class > struct SVecCompat
		: std::false_type
	{};
	template< class tASize, class tBSize, typename T, class tSA, class tSB >
	struct SVecCompat< SmallVector<tASize,T,tSA>, SmallVector<tBSize,T,tSB>, compat::void_t<
			typename tSA::Aspect, typename tSA::Space,
			typename tSB::Aspect, typename tSB::Space
		> 
	> : std::integral_constant< bool,
			//   SVecSizeCompat<tASize,tBSize>::value
			   std::is_same<tASize,tBSize>::value /* Require explicit casts here */
			&& std::is_same<typename tSA::Aspect, typename tSB::Aspect>::value
			&& space_compatible<typename tSA::Space, typename tSB::Space>::value
		>
	{};

	// Helper: Are sizes compatible?
	template< class, class > struct SVecSizeCompat : std::false_type {};
	template< class tSize > struct SVecSizeCompat<tSize,tSize>
		: std::true_type
	{};
	template< class tSize > struct SVecSizeCompat<tSize,ftl::DynamicSize>
		: std::true_type
	{};
	template< class tSize > struct SVecSizeCompat<ftl::DynamicSize,tSize>
		: std::true_type
	{};
	template<> struct SVecSizeCompat<ftl::DynamicSize,ftl::DynamicSize>
		: std::true_type
	{};

	// Helper: Are the two vectors resizeable to each other
	template< class, class, class > struct SVecCompatResize
		: std::false_type
	{};
	template< class tASize, class tBSize, typename T, class tSA, class tSB >
	struct SVecCompatResize< SmallVector<tASize,T,tSA>, SmallVector<tBSize,T,tSB>, compat::void_t<
			typename tSA::Aspect, typename tSA::Space,
			typename tSB::Aspect, typename tSB::Space
		> 
	> : std::integral_constant< bool,
			   std::is_same<typename tSA::Aspect, typename tSB::Aspect>::value
			&& space_compatible<typename tSA::Space, typename tSB::Space>::value
		>
	{};

	// Helper: Are the two vectors narrowable to each other
	template< class, class, class > struct SVecCompatNarrow
		: std::false_type
	{};
	template< class tASize, class tBSize, typename T, typename Y, class tSA, class tSB >
	struct SVecCompatNarrow< SmallVector<tASize,T,tSA>, SmallVector<tBSize,Y,tSB>, compat::void_t<
			typename tSA::Aspect, typename tSA::Space,
			typename tSB::Aspect, typename tSB::Space
		> 
	> : std::integral_constant< bool,
			   std::is_same<tASize,tBSize>::value /* Require explicit casts here */
			&& std::is_same<typename tSA::Aspect, typename tSB::Aspect>::value
			&& space_compatible<typename tSA::Space, typename tSB::Space>::value
		>
	{};
}

//--    SmallVector - generic               ///{{{2///////////////////////////

#define FLUXTMP_SVEC_TEMPL_ template< class tSize, typename T, class tSpec > 
#define FLUXTMP_SVEC_CLASS_ SmallVector<tSize,T,tSpec>

FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( tSize aSize ) noexcept(staticSize)
	: elem( aSize )
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( Init::Zero, tSize aSize ) noexcept(staticSize)
	: elem( aSize, typename ftl::DSStore<tSize,T>::Default{} )
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( Init::Fill, T aTemplate, tSize aSize ) noexcept(staticSize)
	: elem( aSize, typename ftl::DSStore<tSize,T>::Fill{}, aTemplate )
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( Init::Identity, tSize aSize ) noexcept(staticSize)
	: elem( aSize, typename ftl::DSStore<tSize,T>::Fill{}, T(0) /*TODO:dynamic size=4*/ )
{}
FLUXTMP_SVEC_TEMPL_ inline
FLUXTMP_SVEC_CLASS_::SmallVector( ftl::ExplicitInitList<T> aList ) noexcept(staticSize)
	: elem( aList )
{}

FLUXTMP_SVEC_TEMPL_ template< class tOtherVec, class > inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( tOtherVec const& aOther ) noexcept(staticSize)
	: elem{}
{
	FLUX_ASSERT( size() == aOther.size() );
	for( std::size_t i = 0; i < size(); ++i )
		elem[i] = aOther[i];
}
FLUXTMP_SVEC_TEMPL_ template< class tOtherVec, class > inline FLUX_CONSTEXPR_EX
auto FLUXTMP_SVEC_CLASS_::operator=( tOtherVec const& aOther ) noexcept(staticSize) -> SmallVector&
{
	FLUX_ASSERT( size() == aOther.size() );
	for( std::size_t i = 0; i < size(); ++i )
		elem[i] = aOther[i];
	return *this;
}


FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
T& FLUXTMP_SVEC_CLASS_::operator[]( std::size_t aIdx ) noexcept
{
	FLUX_ASSERT( aIdx < elem.size() );
	return elem[aIdx];
}
FLUXTMP_SVEC_TEMPL_ inline constexpr
T const& FLUXTMP_SVEC_CLASS_::operator[]( std::size_t aIdx ) const noexcept
{
	return FLUX_ASSERT( aIdx < elem.size() ), elem[aIdx];
}

FLUXTMP_SVEC_TEMPL_ inline constexpr
tSize FLUXTMP_SVEC_CLASS_::size() const noexcept
{
	return elem.size();
}

FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
T* FLUXTMP_SVEC_CLASS_::data() noexcept
{
	return elem.data();
}
FLUXTMP_SVEC_TEMPL_ inline constexpr
T const* FLUXTMP_SVEC_CLASS_::data() const noexcept
{
	return elem.data();
}

#undef FLUXTMP_SVEC_CLASS_
#undef FLUXTMP_SVEC_TEMPL_

//--    SmallVector - specs                 ///{{{2///////////////////////////

	// SmallVector<2>
#define FLUXTMP_SVEC_TEMPL_ template< typename T, class tSpec > 
#define FLUXTMP_SVEC_CLASS_ SmallVector<ftl::StaticSize<2>,T,tSpec>

FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( ftl::StaticSize<2> ) noexcept
	: elem{}
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( Init::Zero, ftl::StaticSize<2> ) noexcept
	: elem{}
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( Init::Fill, T aTemplate, ftl::StaticSize<2> ) noexcept
	: elem{ aTemplate, aTemplate }
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( Init::Identity, ftl::StaticSize<2> ) noexcept
	: elem{ T(0), T(0) }
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( T aX, T aY ) noexcept
	: elem{ aX, aY }
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( detail::Init2<T> aInit ) noexcept
	: elem{ aInit.x, aInit.y }
{}

FLUXTMP_SVEC_TEMPL_ template< class tOtherVec, class > inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( tOtherVec const& aOther ) noexcept
	: elem{ aOther[0], aOther[1] }
{
	FLUX_ASSERT( size() == aOther.size() );
}
FLUXTMP_SVEC_TEMPL_ template< class tOtherVec, class > inline FLUX_CONSTEXPR_EX
auto FLUXTMP_SVEC_CLASS_::operator=( tOtherVec const& aOther ) noexcept -> SmallVector&
{
	FLUX_ASSERT( size() == aOther.size() );
	elem[0] = aOther[0];
	elem[1] = aOther[1];
	return *this;
}


FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
T& FLUXTMP_SVEC_CLASS_::operator[]( std::size_t aIdx ) noexcept
{
	FLUX_ASSERT( aIdx < 2 );
	return elem[aIdx];
}
FLUXTMP_SVEC_TEMPL_ inline constexpr
T const& FLUXTMP_SVEC_CLASS_::operator[]( std::size_t aIdx ) const noexcept
{
	return FLUX_ASSERT( aIdx < 2 ), elem[aIdx];
}

FLUXTMP_SVEC_TEMPL_ inline constexpr
ftl::StaticSize<2> FLUXTMP_SVEC_CLASS_::size() const noexcept
{
	return {};
}

FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
T* FLUXTMP_SVEC_CLASS_::data() noexcept
{
	return elem;
}
FLUXTMP_SVEC_TEMPL_ inline constexpr
T const* FLUXTMP_SVEC_CLASS_::data() const noexcept
{
	return elem;
}

#undef FLUXTMP_SVEC_CLASS_
#undef FLUXTMP_SVEC_TEMPL_

	// SmallVector<3>
#define FLUXTMP_SVEC_TEMPL_ template< typename T, class tSpec >
#define FLUXTMP_SVEC_CLASS_ SmallVector<ftl::StaticSize<3>,T,tSpec>

FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( ftl::StaticSize<3> ) noexcept
	: elem{}
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( Init::Zero, ftl::StaticSize<3> ) noexcept
	: elem{}
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( Init::Fill, T aTemplate, ftl::StaticSize<3> ) noexcept
	: elem{ aTemplate, aTemplate, aTemplate }
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( Init::Identity, ftl::StaticSize<3> ) noexcept
	: elem{ T(0), T(0), T(0) }
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( T aX, T aY, T aZ ) noexcept
	: elem{ aX, aY, aZ }
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( detail::Init3<T> aInit ) noexcept
	: elem{ aInit.x, aInit.y, aInit.z }
{}

FLUXTMP_SVEC_TEMPL_ template< class tOtherVec, class > inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( tOtherVec const& aOther ) noexcept
	: elem{ aOther[0], aOther[1], aOther[2] }
{
	FLUX_ASSERT( size() == aOther.size() );
}
FLUXTMP_SVEC_TEMPL_ template< class tOtherVec, class > inline FLUX_CONSTEXPR_EX
auto FLUXTMP_SVEC_CLASS_::operator=( tOtherVec const& aOther ) noexcept -> SmallVector&
{
	FLUX_ASSERT( size() == aOther.size() );
	elem[0] = aOther[0];
	elem[1] = aOther[1];
	elem[2] = aOther[2];
	return *this;
}

FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
T& FLUXTMP_SVEC_CLASS_::operator[]( std::size_t aIdx ) noexcept
{
	FLUX_ASSERT( aIdx < 3 );
	return elem[aIdx];
}
FLUXTMP_SVEC_TEMPL_ inline constexpr
T const& FLUXTMP_SVEC_CLASS_::operator[]( std::size_t aIdx ) const noexcept
{
	return FLUX_ASSERT( aIdx < 3 ), elem[aIdx];
}

FLUXTMP_SVEC_TEMPL_ inline constexpr
ftl::StaticSize<3> FLUXTMP_SVEC_CLASS_::size() const noexcept
{
	return {};
}

FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
T* FLUXTMP_SVEC_CLASS_::data() noexcept
{
	return elem;
}
FLUXTMP_SVEC_TEMPL_ inline constexpr
T const* FLUXTMP_SVEC_CLASS_::data() const noexcept
{
	return elem;
}

#undef FLUXTMP_SVEC_CLASS_
#undef FLUXTMP_SVEC_TEMPL_

		// SmallVector<4>
#define FLUXTMP_SVEC_TEMPL_ template< typename T, class tSpec >
#define FLUXTMP_SVEC_CLASS_ SmallVector<ftl::StaticSize<4>,T,tSpec>

FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( ftl::StaticSize<4> ) noexcept
	: elem{}
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( Init::Zero, ftl::StaticSize<4> ) noexcept
	: elem{}
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( Init::Fill, T aTemplate, ftl::StaticSize<4> ) noexcept
	: elem{ aTemplate, aTemplate, aTemplate, aTemplate }
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( Init::Identity, ftl::StaticSize<4> ) noexcept
	: elem{ T(0), T(0), T(0), T(1) }
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( T aX, T aY, T aZ, T aW ) noexcept
	: elem{ aX, aY, aZ, aW }
{}
FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( detail::Init4<T> aInit ) noexcept
	: elem{ aInit.x, aInit.y, aInit.z, aInit.w }
{}

FLUXTMP_SVEC_TEMPL_ template< class tOtherVec, class > inline FLUX_CONSTEXPR_EX
FLUXTMP_SVEC_CLASS_::SmallVector( tOtherVec const& aOther ) noexcept
	: elem{ aOther[0], aOther[1], aOther[2], aOther[3] }
{
	FLUX_ASSERT( size() == aOther.size() );
}
FLUXTMP_SVEC_TEMPL_ template< class tOtherVec, class > inline FLUX_CONSTEXPR_EX
auto FLUXTMP_SVEC_CLASS_::operator=( tOtherVec const& aOther ) noexcept -> SmallVector&
{
	FLUX_ASSERT( size() == aOther.size() );
	elem[0] = aOther[0];
	elem[1] = aOther[1];
	elem[2] = aOther[2];
	elem[3] = aOther[3];
	return *this;
}

FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
T& FLUXTMP_SVEC_CLASS_::operator[]( std::size_t aIdx ) noexcept
{
	FLUX_ASSERT( aIdx < 4 );
	return elem[aIdx];
}
FLUXTMP_SVEC_TEMPL_ inline constexpr
T const& FLUXTMP_SVEC_CLASS_::operator[]( std::size_t aIdx ) const noexcept
{
	return FLUX_ASSERT( aIdx < 4 ), elem[aIdx];
}

FLUXTMP_SVEC_TEMPL_ inline constexpr
ftl::StaticSize<4> FLUXTMP_SVEC_CLASS_::size() const noexcept
{
	return {};
}

FLUXTMP_SVEC_TEMPL_ inline FLUX_CONSTEXPR_EX
T* FLUXTMP_SVEC_CLASS_::data() noexcept
{
	return elem;
}
FLUXTMP_SVEC_TEMPL_ inline constexpr
T const* FLUXTMP_SVEC_CLASS_::data() const noexcept
{
	return elem;
}

#undef FLUXTMP_SVEC_CLASS_
#undef FLUXTMP_SVEC_TEMPL_


//--    operators                           ///{{{2///////////////////////////

#define FLUXTMP_FMLOP_TEMPL1_(op) template< class tSize, typename T, class tSA, class tNA >
#define FLUXTMP_FMLOP_TEMPL2_(op) template< class tSize, typename T, class tSA, class tSB, class tNA >
#define FLUXTMP_FMLOP_CA_ SmallVector<tSize,T,tSA>
#define FLUXTMP_FMLOP_CB_ SmallVector<tSize,T,tSB>
#define FLUXTMP_FMLOP_RS_ SmallVector<tSize,T,tNA>
#define FLUXTMP_FMLOP_NOEX_ noexcept((SmallVector<tSize,T,tNA>::staticSize))


	// unary
FLUXTMP_FMLOP_TEMPL1_(op::ClassAdd) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator- (FLUXTMP_FMLOP_CA_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ ret( aV.size() );
	for( std::size_t i = 0; i < aV.size(); ++i )
		ret[i] = -aV[i];
	return ret;
}

FLUXTMP_FMLOP_TEMPL1_(op::ClassAdd) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator+ (FLUXTMP_FMLOP_CA_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ ret( aV.size() );
	for( std::size_t i = 0; i < aV.size(); ++i )
		ret[i] = +aV[i];
	return ret;
}

	// binary: compare
FLUXTMP_FMLOP_TEMPL2_(op::ClassCmp) inline FLUX_CONSTEXPR_EX
bool operator== (FLUXTMP_FMLOP_CA_ const& aV, FLUXTMP_FMLOP_CB_ const& aU) noexcept
{
	for( std::size_t i = 0; i < aV.size(); ++i )
	{
		if( aV[i] != aU[i] ) return false;
	}

	return true;
}
FLUXTMP_FMLOP_TEMPL2_(op::ClassCmp) inline FLUX_CONSTEXPR_EX
bool operator!= (FLUXTMP_FMLOP_CA_ const& aV, FLUXTMP_FMLOP_CB_ const& aU) noexcept
{
	return !(aV == aU);
}


	// binary: arithmetic
FLUXTMP_FMLOP_TEMPL2_(op::ClassAdd) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator+ (FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ ret( aV.size() );
	for( std::size_t i = 0; i < aV.size(); ++i )
		ret[i] = aU[i]+aV[i];
	return ret;
}
FLUXTMP_FMLOP_TEMPL2_(op::ClassAdd) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator- (FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ ret( aV.size() );
	for( std::size_t i = 0; i < aV.size(); ++i )
		ret[i] = aU[i]-aV[i];
	return ret;
}

FLUXTMP_FMLOP_TEMPL2_(op::ClassMul) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator* (FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ ret( aV.size() );
	for( std::size_t i = 0; i < aV.size(); ++i )
		ret[i] = aU[i]*aV[i];
	return ret;
}
FLUXTMP_FMLOP_TEMPL2_(op::ClassMul) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator/ (FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ ret( aV.size() );
	for( std::size_t i = 0; i < aV.size(); ++i )
		ret[i] = aU[i]/aV[i];
	return ret;
}

FLUXTMP_FMLOP_TEMPL1_(op::ClassMul) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator* (FLUXTMP_FMLOP_CA_ const& aU, T aX) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ ret( aU.size() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		ret[i] = aU[i]*aX;
	return ret;
}
FLUXTMP_FMLOP_TEMPL1_(op::ClassMul) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator/ (FLUXTMP_FMLOP_CA_ const& aU, T aX) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ ret( aU.size() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		ret[i] = aU[i]/aX;
	return ret;
}

FLUXTMP_FMLOP_TEMPL1_(op::ClassMul) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator* (T aX, FLUXTMP_FMLOP_CA_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ ret( aV.size() );
	for( std::size_t i = 0; i < aV.size(); ++i )
		ret[i] = aX*aV[i];
	return ret;
}

	// binary: assignment
FLUXTMP_FMLOP_TEMPL2_(op::ClassInc) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator+= (FLUXTMP_FMLOP_CA_& aU, FLUXTMP_FMLOP_CB_ const& aV) noexcept
{
	for( std::size_t i = 0; i < aU.size(); ++i )
		aU[i] += aV[i];
	return aU;
}
FLUXTMP_FMLOP_TEMPL2_(op::ClassInc) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator-= (FLUXTMP_FMLOP_CA_& aU, FLUXTMP_FMLOP_CB_ const& aV) noexcept
{
	for( std::size_t i = 0; i < aU.size(); ++i )
		aU[i] -= aV[i];
	return aU;
}

FLUXTMP_FMLOP_TEMPL2_(op::ClassScale) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator*= (FLUXTMP_FMLOP_CA_& aU, FLUXTMP_FMLOP_CB_ const& aV) noexcept
{
	for( std::size_t i = 0; i < aU.size(); ++i )
		aU[i] *= aV[i];
	return aU;
}
FLUXTMP_FMLOP_TEMPL2_(op::ClassScale) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator/= (FLUXTMP_FMLOP_CA_& aU, FLUXTMP_FMLOP_CB_ const& aV) noexcept
{
	for( std::size_t i = 0; i < aU.size(); ++i )
		aU[i] /= aV[i];
	return aU;
}

FLUXTMP_FMLOP_TEMPL1_(op::ClassScale) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator*= (FLUXTMP_FMLOP_CA_& aU, T aX) noexcept
{
	for( std::size_t i = 0; i < aU.size(); ++i )
		aU[i] *= aX;
	return aU;
}
FLUXTMP_FMLOP_TEMPL1_(op::ClassScale) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator/= (FLUXTMP_FMLOP_CA_& aU, T aX) noexcept
{
	for( std::size_t i = 0; i < aU.size(); ++i )
		aU[i] /= aX;
	return aU;
}

//--    functions                           ///{{{2///////////////////////////

#if FLUX_FML_MSVC_ICE_WORKAROUND
#	define FLUXTMP_FML_IDENT_ = {}
#	define FLUXTMP_FML_SIZE_ = svec_size_t<V>{}
#else // !MSVC_ICE_WORKAROUND
#	define FLUXTMP_FML_IDENT_
#	define FLUXTMP_FML_SIZE_
#endif // ~ MSVC_ICE_WORKAROUND

	// pseudo constructors
template< class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V,2> )
(V) make_vector( svec_type_t<V> aX, svec_type_t<V> aY ) noexcept(V::staticSize)
{
	return V{{ aX, aY }};
}

template< class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V,3> )
(V) make_vector( svec_type_t<V> aX, svec_type_t<V> aY, svec_type_t<V> aZ  ) noexcept(V::staticSize)
{
	return V{{ aX, aY, aZ }};
}

template< class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V,4> )
(V) make_vector( svec_type_t<V> aX, svec_type_t<V> aY, svec_type_t<V> aZ, svec_type_t<V> aW ) noexcept(V::staticSize)
{
	return V{{ aX, aY, aZ, aW }};
}

template< class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V> )
(V) make_vector( ftl::not_null<svec_type_t<V> const*> aPtr, svec_size_t<V> aSize ) noexcept(V::staticSize)
{
	V ret( aSize );
	for( std::size_t i = 0; i < ret.size(); ++i )
		ret[i] = aPtr[i];
	return ret;
}

template< class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V> )
(V) make_zero( Identity<V> FLUXTMP_FML_IDENT_, svec_size_t<V> aSize FLUXTMP_FML_SIZE_ ) noexcept(V::staticSize)
{
	return V( Init::zero, aSize );
}
template< class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V> )
(V) make_one( Identity<V> FLUXTMP_FML_IDENT_, svec_size_t<V> aSize FLUXTMP_FML_SIZE_ ) noexcept(V::staticSize)
{
	return V( Init::fill, typename V::value_type(1), aSize );
}
template< class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V> )
(V) make_iota( Identity<V> FLUXTMP_FML_IDENT_, svec_size_t<V> aSize FLUXTMP_FML_SIZE_ ) noexcept(V::staticSize)
{
	typedef typename V::value_type Type_;
	V ret( Init::zero, aSize );
	for( std::size_t i = 0; i < ret.size(); ++i )
		ret[i] = Type_(i);
	return ret;
}
template< class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V> )
(V) make_identity( Identity<V> FLUXTMP_FML_IDENT_, svec_size_t<V> aSize FLUXTMP_FML_SIZE_ ) noexcept(V::staticSize)
{
	return V( Init::identity, aSize );
}
template< class V > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V> )
(V) make_splat( typename V::value_type aX, Identity<V> FLUXTMP_FML_IDENT_, svec_size_t<V> aSize FLUXTMP_FML_SIZE_ ) noexcept(V::staticSize)
{
	return V( Init::fill, aX, aSize );
}

	// conversions
template< class V, class tSize, typename T, class tSA > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_vector<V>::value && detail::SVecCompatResize<V,SmallVector<tSize,T,tSA>>::value )
(V) expand( SmallVector<tSize,T,tSA> const& aU, V const& aV ) noexcept(V::staticSize)
{
	FLUX_ASSERT( aU.size() < aV.size() );

	V ret( aV.size() );
	std::size_t i = 0;
	for( ; i < aU.size(); ++i ) ret[i] = aU[i];
	for( ; i < aV.size(); ++i ) ret[i] = aV[i];
	return ret;
}


template< class V, class tSize, typename T, class tSA > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_vector<V>::value && detail::SVecCompatResize<V,SmallVector<tSize,T,tSA>>::value )
(V) shrink( SmallVector<tSize,T,tSA> const& aU, svec_size_t<V> aSize FLUXTMP_FML_SIZE_ ) noexcept(V::staticSize)
{
	V ret( Init::zero, aSize );
	FLUX_ASSERT( aU.size() > ret.size() );
	for( std::size_t i = 0; i < ret.size(); ++i ) ret[i] = aU[i];
	return ret;
}

template< class V, class tSize, typename T, class tSA > inline /*FLUX_CONSTEXPR_EX*/
FLUX_ENABLE_IF_C( is_small_vector<V>::value && detail::SVecCompatNarrow<V,SmallVector<tSize,T,tSA>>::value )
(V) narrow( SmallVector<tSize,T,tSA> const& aU ) noexcept(V::staticSize)
{
	typedef typename V::value_type Type_;

	V ret( aU.size() );
	for( std::size_t i = 0; i < ret.size(); ++i ) ret[i] = util::narrow<Type_>(aU[i]);
	return ret;
}

template< class V, class tSize, typename T, class tSA > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_vector<V>::value && detail::SVecSizeCompat<svec_size_t<V>,tSize>::value )
(V) vector_cast( SmallVector<tSize,T,tSA> const& aU ) noexcept(V::staticSize)
{
	typedef typename V::value_type Type_;

	V ret( aU.size() );
	for( std::size_t i = 0; i < ret.size(); ++i ) ret[i] = Type_(aU[i]);
	return ret;
}

	// helpers
template< class tSize, typename T, class tSA > inline FLUX_CONSTEXPR_EX
T* data_ptr( SmallVector<tSize,T,tSA>& aV ) noexcept
{
	return aV.data();
}
template< class tSize, typename T, class tSA > inline FLUX_CONSTEXPR_EX
T const* data_ptr( SmallVector<tSize,T,tSA> const& aV ) noexcept
{
	return aV.data();
}

	// general math stuff
FLUXTMP_FMLOP_TEMPL2_(op::ClassDot) inline FLUX_CONSTEXPR_EX
T dot(FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) noexcept
{
	T res = 0;
	for( std::size_t i = 0; i < aU.size(); ++i )
		res += aU[i]*aV[i];
	return res;
}
	
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) inline
T length(FLUXTMP_FMLOP_CA_ const& aU) noexcept
{
	return norm_2(aU);
}
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) inline FLUX_CONSTEXPR_EX
T length_squared(FLUXTMP_FMLOP_CA_ const& aU) noexcept
{
	T res = 0;
	for( std::size_t i = 0; i < aU.size(); ++i )
		res += aU[i]*aU[i];
	return res;
}

FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) inline FLUX_CONSTEXPR_EX
T max(FLUXTMP_FMLOP_CA_ const& aU) noexcept
{
	T res = aU[0];
	for( std::size_t i = 1; i < aU.size(); ++i )
		res = std::max( res, aU[i] );
	return res;
}
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) inline FLUX_CONSTEXPR_EX
T min(FLUXTMP_FMLOP_CA_ const& aU) noexcept
{
	T res = aU[0];
	for( std::size_t i = 1; i < aU.size(); ++i )
		res = std::min( res, aU[i] );
	return res;
}

FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) inline
T norm_1(FLUXTMP_FMLOP_CA_ const& aU) noexcept
{
	T res = 0;
	for( std::size_t i = 0; i < aU.size(); ++i )
		res += std::abs(aU[i]);
	return res;
}
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) inline
T norm_2(FLUXTMP_FMLOP_CA_ const& aU) noexcept
{
	T res = 0;
	for( std::size_t i = 0; i < aU.size(); ++i )
		res += aU[i]*aU[i];
	return T(std::sqrt(res));
}
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) inline
T norm_inf(FLUXTMP_FMLOP_CA_ const& aU) noexcept
{
	T res = std::abs(aU[0]);
	for( std::size_t i = 1; i < aU.size(); ++i )
		res = std::max( res, std::abs(aU[i]) );
	return res;
}


	// vector stuff
FLUXTMP_FMLOP_TEMPL2_(op::ClassBound) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ max(FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ res( Init::zero, aU.size() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		res[i] = std::max(aU[i], aV[i]);
	return res;
}
FLUXTMP_FMLOP_TEMPL2_(op::ClassBound) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ min(FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ res( Init::zero, aU.size() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		res[i] = std::min(aU[i], aV[i]);
	return res;
}

FLUXTMP_FMLOP_TEMPL1_(op::ClassBound) inline
FLUXTMP_FMLOP_RS_ abs(FLUXTMP_FMLOP_CA_ const& aU) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ res( Init::zero, aU.size() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		res[i] = std::abs(aU[i]);
	return res;
}
FLUXTMP_FMLOP_TEMPL1_(op::ClassBound) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ sign(FLUXTMP_FMLOP_CA_ const& aU) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ res( Init::zero, aU.size() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		res[i] = detail::svec_sign_(aU[i]);
	return res;
}


FLUXTMP_FMLOP_TEMPL1_(op::ClassDot) inline
FLUXTMP_FMLOP_RS_ normalize(FLUXTMP_FMLOP_CA_ const& aU) FLUXTMP_FMLOP_NOEX_
{
	return aU / norm_2(aU);
}

FLUXTMP_FMLOP_TEMPL2_(op::ClassLerp) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ mix(T aX, FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	return lerp( aX, aU, aV );
}
FLUXTMP_FMLOP_TEMPL2_(op::ClassLerp) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ lerp(T aX, FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	return aU + (aV-aU)*aX;
	//return (T(1)-aX) * aU + aX * aV;
}

FLUXTMP_FMLOP_TEMPL2_(op::ClassProj) inline
FLUXTMP_FMLOP_RS_ project(FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	using V_ = FLUXTMP_FMLOP_CB_;
	using P_ = FLUXTMP_FMLOP_RS_;
	
	auto const vv = normalize(aV);
	return vector_cast<P_>( dot(vector_cast<V_>(aU),vv) * vv );
}
FLUXTMP_FMLOP_TEMPL2_(op::ClassProj) inline
FLUXTMP_FMLOP_RS_ reflect(FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	using V_ = FLUXTMP_FMLOP_CB_;

	auto const vv = normalize(aV);
	return aU - T(2) * dot(vv,vector_cast<V_>(aU)) * vv;
}

	// special
FLUXTMP_FMLOP_TEMPL1_(op::ClassDot) inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( std::is_same<tSize,ftl::StaticSize<2>> )
(FLUXTMP_FMLOP_RS_) orthogonal(FLUXTMP_FMLOP_CA_ const& aU ) FLUXTMP_FMLOP_NOEX_
{
	return make_vector<FLUXTMP_FMLOP_RS_>( aU[1], -aU[0] );
}

FLUXTMP_FMLOP_TEMPL1_(op::ClassDot) FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( std::is_same<tSize,ftl::StaticSize<3>> )
(FLUXTMP_FMLOP_RS_) orthogonal(FLUXTMP_FMLOP_CA_ const& aU ) FLUXTMP_FMLOP_NOEX_
{
	if( std::abs(aU[0]) > std::abs(aU[1]) )
		return make_vector<FLUXTMP_FMLOP_RS_>( -aU[2], T(0), aU[0] );

	return make_vector<FLUXTMP_FMLOP_RS_>( T(0), -aU[2], aU[1] );
}

FLUXTMP_FMLOP_TEMPL2_(op::ClassDot) FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( std::is_same<tSize,ftl::StaticSize<3>> )
(FLUXTMP_FMLOP_RS_) cross(FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	return make_vector< FLUXTMP_FMLOP_RS_>(
		+aU[1] * aV[2] -aU[2] * aV[1],
		-aU[0] * aV[2] +aU[2] * aV[0],
		+aU[0] * aV[1] -aU[1] * aV[0]
	);
}

#undef FLUXTMP_FML_SIZE_
#undef FLUXTMP_FML_IDENT_

#undef FLUXTMP_FMLOP_NOEX_
#undef FLUXTMP_FMLOP_RS_
#undef FLUXTMP_FMLOP_CA_
#undef FLUXTMP_FMLOP_CB_
#undef FLUXTMP_FMLOP_TEMPL1_
#undef FLUXTMP_FMLOP_TEMPL2_

//--    <<< ~ flux::fml namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
