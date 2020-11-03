/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Small matrix implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>

#include <flux/compat/restrict.hpp>
#include <flux/compat/constexpr.hpp>

#include <flux/util/narrow.hpp>

#include <algorithm>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::fml >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	// SMatDimEBO
	template< class tMSize, class tNSize >
	struct SMatDimEBO
	{
		FLUX_CONSTEXPR_EX tMSize m() const noexcept { return mMSize; }
		FLUX_CONSTEXPR_EX tNSize n() const noexcept { return mNSize; }
		tMSize mMSize;
		tNSize mNSize;
	};
	template< std::size_t tMSize, class tNSize >
	struct SMatDimEBO<ftl::StaticSize<tMSize>,tNSize>
	{
		using MSize_ = ftl::StaticSize<tMSize>;
		SMatDimEBO() = default;
		FLUX_CONSTEXPR_EX SMatDimEBO( MSize_, tNSize aNSize ) : mNSize(aNSize) {}
		FLUX_CONSTEXPR_EX MSize_ m() const noexcept { return {}; }
		FLUX_CONSTEXPR_EX tNSize n() const noexcept { return mNSize; }
		tNSize mNSize;
	};
	template< class tMSize, std::size_t tNSize >
	struct SMatDimEBO<tMSize,ftl::StaticSize<tNSize>>
	{
		using NSize_ = ftl::StaticSize<tNSize>;
		SMatDimEBO() = default;
		FLUX_CONSTEXPR_EX SMatDimEBO( tMSize aMSize, NSize_ ) : mMSize(aMSize) {}
		FLUX_CONSTEXPR_EX tMSize m() const noexcept { return mMSize; }
		FLUX_CONSTEXPR_EX NSize_ n() const noexcept { return {}; }
		tMSize mMSize;
	};
	template< std::size_t tMSize, std::size_t tNSize >
	struct SMatDimEBO<ftl::StaticSize<tMSize>,ftl::StaticSize<tNSize>>
	{
		using MSize_ = ftl::StaticSize<tMSize>;
		using NSize_ = ftl::StaticSize<tNSize>;
		SMatDimEBO() = default;
		FLUX_CONSTEXPR_EX SMatDimEBO( MSize_, NSize_ ) {}
		FLUX_CONSTEXPR_EX MSize_ m() const noexcept { return {}; }
		FLUX_CONSTEXPR_EX NSize_ n() const noexcept { return {}; }
	};

	// SMatElementCount
	template< class tMSize, class tNSize >
	struct SMatElementCount
	{
		using type = decltype(std::declval<tMSize>()*std::declval<tNSize>());
	};

	// SMatCompat
	template< class, class, class > struct SMatCompat
		: std::false_type
	{};
	template< class tAM, class tAN, class tBM, class tBN, typename T, class tSA, class tSB >
	struct SMatCompat< SmallMatrix<tAM,tAN,T,tSA>, SmallMatrix<tBM,tBN,T,tSB>,
		compat::void_t<
			typename tSA::ColSpace, typename tSA::RowSpace,
			typename tSB::ColSpace, typename tSB::RowSpace
		>
	> : std::integral_constant< bool,
			   std::is_same<tAM,tBM>::value
			&& std::is_same<tAN,tBN>::value
			&& space_compatible<typename tSA::ColSpace,typename tSB::ColSpace>::value
			&& space_compatible<typename tSA::RowSpace,typename tSB::RowSpace>::value
		>
	{};
	
	// helpers
	template< typename tType > inline constexpr
	tType smat_sign_( tType aX ) noexcept
	{
		return aX >= tType(0) ? tType(1) : tType(-1);
	}

	// linear_index_()
	//TODO: different layouts.
	template< class tMSize, class tNSize > inline constexpr
	std::size_t linear_index_( std::size_t aI, std::size_t aJ, tMSize aM, tNSize /*aN*/ ) noexcept
	{
		return aJ * aM + aI;
	}

	// smat_init_id_
	template< typename tX, class tMSize, class tNSize >
	struct SMatInitId_
		: detail::SMatDimEBO<tMSize,tNSize>
	{
		FLUX_CONSTEXPR_EX SMatInitId_( tMSize aMSize, tNSize aNSize ) noexcept
			: detail::SMatDimEBO<tMSize,tNSize>{ aMSize, aNSize }
		{}

		constexpr tX operator() (std::size_t aI) const noexcept
		{
			//TODO: different layouts...
			return (aI % this->m()) == (aI / this->m());
		}
	};

	template< typename tX, class tMSize, class tNSize > FLUX_CONSTEXPR_EX
	SMatInitId_<tX,tMSize,tNSize> smat_init_id_( tMSize aMSize, tNSize aNSize ) noexcept
	{
		return SMatInitId_<tX,tMSize,tNSize>(aMSize,aNSize);
	}
}

//--    SmallMatrix                         ///{{{2///////////////////////////
#define FLUXTMP_SMAT_TEMPL_ template< class tMSize, class tNSize, typename T, class tSpec >
#define FLUXTMP_SMAT_CLASS_ SmallMatrix<tMSize,tNSize,T,tSpec>

FLUXTMP_SMAT_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SMAT_CLASS_::SmallMatrix( tMSize aMSize, tNSize aNSize ) noexcept(staticSize)
	: detail::SMatDimEBO<tMSize,tNSize>{ aMSize, aNSize }
	, elems( aMSize*aNSize )
{}

FLUXTMP_SMAT_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SMAT_CLASS_::SmallMatrix( Init::Zero, tMSize aMSize, tNSize aNSize ) noexcept(staticSize)
	: detail::SMatDimEBO<tMSize,tNSize>{ aMSize, aNSize }
	, elems( aMSize*aNSize, typename decltype(elems)::Fill{}, 0 )
{}
FLUXTMP_SMAT_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SMAT_CLASS_::SmallMatrix( Init::Identity, tMSize aMSize, tNSize aNSize ) noexcept(staticSize)
	: detail::SMatDimEBO<tMSize,tNSize>{ aMSize, aNSize }
	, elems( 
		// This is moderately terrible in the non-CONSTEXPR_EX world... :-/
		aMSize*aNSize, 
		typename decltype(elems)::Eval{}, 
		detail::smat_init_id_<T>(aMSize,aNSize)
	)
{}
FLUXTMP_SMAT_TEMPL_ inline FLUX_CONSTEXPR_EX
FLUXTMP_SMAT_CLASS_::SmallMatrix( Init::Fill, T aX, tMSize aMSize, tNSize aNSize ) noexcept(staticSize)
	: detail::SMatDimEBO<tMSize,tNSize>{ aMSize, aNSize }
	, elems( aMSize*aNSize, typename decltype(elems)::Fill{}, aX )
{}

FLUXTMP_SMAT_TEMPL_ inline
FLUXTMP_SMAT_CLASS_::SmallMatrix( ftl::ExplicitInitList<T> aList, tMSize aMSize, tNSize aNSize )
	: detail::SMatDimEBO<tMSize,tNSize>{ aMSize, aNSize }
	, elems( aMSize*aNSize, typename decltype(elems)::Fill{}, 0 )
{
	FLUX_ASSERT( size() == aList.ilist.size() );
	auto it = aList.ilist.begin();
	for( std::size_t i = 0; i < aMSize; ++i )
	{
		for( std::size_t j = 0; j < aNSize; ++j )
			this->operator()(i,j) = *it++;
	}
}

FLUXTMP_SMAT_TEMPL_ template< class tOtherMat, class > inline FLUX_CONSTEXPR_EX
FLUXTMP_SMAT_CLASS_::SmallMatrix( tOtherMat const& aOther ) noexcept(staticSize)
	: detail::SMatDimEBO<tMSize,tNSize>{ aOther.m(), aOther.n() }
	, elems{ aOther.elems }
{}
FLUXTMP_SMAT_TEMPL_ template< class tOtherMat, class > inline FLUX_CONSTEXPR_EX
auto FLUXTMP_SMAT_CLASS_::operator=( tOtherMat const& aOther ) noexcept(staticSize) -> SmallMatrix&
{
	FLUX_ASSERT( m() == aOther.m() && n() == aOther.n() );
	for( std::size_t i = 0; i < size(); ++i )
		elems[i] = aOther[i];
	return *this;
}

FLUXTMP_SMAT_TEMPL_ inline FLUX_CONSTEXPR_EX
T& FLUXTMP_SMAT_CLASS_::operator[] (std::size_t aI) noexcept
{
	FLUX_ASSERT( aI < size() );
	return elems[aI];
}
FLUXTMP_SMAT_TEMPL_ inline constexpr
T const& FLUXTMP_SMAT_CLASS_::operator[] (std::size_t aI) const noexcept
{
	return FLUX_ASSERT( aI < size() ), elems[aI];
}

FLUXTMP_SMAT_TEMPL_ inline FLUX_CONSTEXPR_EX
T& FLUXTMP_SMAT_CLASS_::operator() (std::size_t aI, std::size_t aJ) noexcept
{
	// TODO : Spec::Layout
	return FLUX_ASSERT( aI < m() && aJ < n() ), elems[detail::linear_index_(aI,aJ,m(),n())];
}
FLUXTMP_SMAT_TEMPL_ inline constexpr
T const& FLUXTMP_SMAT_CLASS_::operator() (std::size_t aI, std::size_t aJ) const noexcept
{
	// TODO : Spec::Layout
	return FLUX_ASSERT( aI < m() && aJ < n() ), elems[detail::linear_index_(aI,aJ,m(),n())];
}

FLUXTMP_SMAT_TEMPL_ inline constexpr
tMSize FLUXTMP_SMAT_CLASS_::m() const noexcept
{
	return static_cast<detail::SMatDimEBO<tMSize,tNSize>>(*this).m();
}
FLUXTMP_SMAT_TEMPL_ inline constexpr
tNSize FLUXTMP_SMAT_CLASS_::n() const noexcept
{
	return static_cast<detail::SMatDimEBO<tMSize,tNSize>>(*this).n();
}
FLUXTMP_SMAT_TEMPL_ inline constexpr
auto FLUXTMP_SMAT_CLASS_::size() const noexcept -> ElementCount
{
	return m()*n();
}

FLUXTMP_SMAT_TEMPL_ inline FLUX_CONSTEXPR_EX
T* FLUXTMP_SMAT_CLASS_::data() noexcept
{
	return elems.data();
}
FLUXTMP_SMAT_TEMPL_ inline constexpr
T const* FLUXTMP_SMAT_CLASS_::data() const noexcept
{
	return elems.data();
}

#undef FLUXTMP_SMAT_CLASS_
#undef FLUXTMP_SMAT_TEMPL_

//--    meta functions                      ///{{{2///////////////////////////
template< class tMSize, class tNSize, typename T, class tSA >
struct is_small_matrix< SmallMatrix<tMSize,tNSize,T,tSA>, 0, 0 > 
	: std::true_type
{};

template< std::size_t tM, class tNSize, typename T, class tSA >
struct is_small_matrix< SmallMatrix<ftl::StaticSize<tM>,tNSize,T,tSA>, tM, 0 > 
	: std::true_type
{};
template< class tMSize, std::size_t tN, typename T, class tSA >
struct is_small_matrix< SmallMatrix<tMSize,ftl::StaticSize<tN>,T,tSA>, 0, tN > 
	: std::true_type
{};
template< std::size_t tM, std::size_t tN, typename T, class tSA >
struct is_small_matrix< SmallMatrix<ftl::StaticSize<tM>,ftl::StaticSize<tN>,T,tSA>, tM, tN > 
	: std::true_type
{};

namespace detail
{
	template< class tMSize, class tNSize, typename T, class tSA >
	struct SMatType< SmallMatrix<tMSize,tNSize,T,tSA> > {
		using type = T;
	};
	template< class tMSize, class tNSize, typename T, class tSA >
	struct SMatRows< SmallMatrix<tMSize,tNSize,T,tSA> > {
		using type = tMSize;
	};
	template< class tMSize, class tNSize, typename T, class tSA >
	struct SMatCols< SmallMatrix<tMSize,tNSize,T,tSA> > {
		using type = tNSize;
	};

	template< class tMSize, class tNSize, typename T, class tSA >
	struct SMatRowSpace< SmallMatrix<tMSize,tNSize,T,tSA> > {
		using type = typename tSA::RowSpace;
	};
	template< class tMSize, class tNSize, typename T, class tSA >
	struct SMatColSpace< SmallMatrix<tMSize,tNSize,T,tSA> > {
		using type = typename tSA::ColSpace;
	};


	template< class tMSize, class tNSize, typename T, class tSA, class tSpace >
	struct SMatRebindCol< SmallMatrix<tMSize,tNSize,T,tSA>, tSpace > {
		using type = SmallMatrix<tMSize,tNSize,T,aspect::MatrixSpec<tSpace, typename tSA::RowSpace>>;
	};
	template< class tMSize, class tNSize, typename T, class tSA, class tSpace >
	struct SMatRebindRow< SmallMatrix<tMSize,tNSize,T,tSA>, tSpace > {
		using type = SmallMatrix<tMSize,tNSize,T,aspect::MatrixSpec<typename tSA::ColSpace, tSpace>>;
	};
	template< class tMSize, class tNSize, typename T, class tSA, class tColS, class tRowS >
	struct SMatRebind< SmallMatrix<tMSize,tNSize,T,tSA>, tColS, tRowS > {
		using type = SmallMatrix<tMSize,tNSize,T,aspect::MatrixSpec<tColS,tRowS>>;
	};
}

//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	template< class tSA, class tSB >
	struct SMatResultSpec
	{
		using type = aspect::MatrixSpec<
			space_specialized_t< typename tSA::ColSpace, typename tSB::ColSpace >,
			space_specialized_t< typename tSA::RowSpace, typename tSB::RowSpace >
			//TODO Layout.. require same layout in this case!
		>;
	};
	
	
	template< bool, class, class > struct SMatMulResultSpec0;
	template< class tSA, class tSB >
	struct SMatMulResultSpec0< true, tSA, tSB >
	{
		using type = aspect::MatrixSpec<
			typename tSA::ColSpace,
			typename tSB::RowSpace
			//TODO Layout from tSA
		>;
	};

	template< class tSA, class tSB >
	struct SMatMulResultSpec
		: SMatMulResultSpec0<
			space_compatible<typename tSA::RowSpace,typename tSB::ColSpace>::value,
			tSA, tSB
		>
	{};

	/* TODO: should there be a op::ClassXYZ for Vector-Matrix mults? */


	template< class, class, class, class > struct SMatSVecResultSpec0;
	template< class tMA, class tVA, class tSpace >
	struct SMatSVecResultSpec0< tMA, tVA, tSpace, tSpace >
	{
		using type = aspect::VectorSpec<
			typename tVA::Aspect,
			typename tMA::ColSpace
		>;
	};
	template< class tMA, class tVA >
	struct SMatSVecResultSpec
		: SMatSVecResultSpec0< tMA, tVA, typename tMA::RowSpace, typename tVA::Space >
	{};


	template< class, class, class, class > struct SVecSMatResultSpec0;
	template< class tVA, class tMA, class tSpace >
	struct SVecSMatResultSpec0< tVA, tMA, tSpace, tSpace >
	{
		using type = aspect::VectorSpec<
			typename tVA::Aspect,
			typename tMA::RowSpace
		>;
	};
	template< class tVA, class tMA >
	struct SVecSMatResultSpec
		: SVecSMatResultSpec0< tVA, tMA, typename tVA::Space, typename tMA::ColSpace >
	{};

	template< class tMSize, class tNSize > 
	struct SMatCheckSize0 
		: std::false_type
	{};
	template< class tSize >
	struct SMatCheckSize0<tSize,tSize> : std::true_type {};
	template< class tSize >
	struct SMatCheckSize0<tSize,ftl::DynamicSize> : std::true_type {};
	template< class tSize >
	struct SMatCheckSize0<ftl::DynamicSize,tSize> : std::true_type {};
	template<>
	struct SMatCheckSize0<ftl::DynamicSize,ftl::DynamicSize> : std::true_type {};

	template< class tU, class tV >
	struct SMatCheckSize : ftl::meta::All<
		SMatCheckSize0< smat_cols_t<tU>, smat_cols_t<tV> >,
		SMatCheckSize0< smat_rows_t<tU>, smat_rows_t<tV> >
	> {};


	/*template< class tMat >
	struct SMatCheckSquare
		: SMatCheckSize0< smat_cols_t<tMat>, smat_rows_t<tMat> >
	{};*/


	// Helper: Are the two matrices are resizeable to each other
	template< class, class, class > struct SMatCompatResize
		: std::false_type
	{};
	template< class tAM, class tAN, class tBM, class tBN, typename T, class tSA, class tSB >
	struct SMatCompatResize< SmallMatrix<tAM,tAN,T,tSA>, SmallMatrix<tBM,tBN,T,tSB>, compat::void_t<
			typename tSA::ColSpace, typename tSA::RowSpace,
			typename tSB::ColSpace, typename tSB::RowSpace
		> 
	> : std::integral_constant< bool,
			   space_compatible<typename tSA::ColSpace, typename tSB::ColSpace>::value
			&& space_compatible<typename tSA::RowSpace, typename tSB::RowSpace>::value
		>
	{};
	
	// Helper: Are the two matrices are narrowable to each other
	template< class, class, class > struct SMatCompatNarrow
		: std::false_type
	{};
	template< class tAM, class tAN, class tBM, class tBN, typename T, typename Y, class tSA, class tSB >
	struct SMatCompatNarrow< SmallMatrix<tAM,tAN,T,tSA>, SmallMatrix<tBM,tBN,Y,tSB>, compat::void_t<
			typename tSA::ColSpace, typename tSA::RowSpace,
			typename tSB::ColSpace, typename tSB::RowSpace
		> 
	> : std::integral_constant< bool,
			   std::is_same<tAM,tBM>::value
			&& std::is_same<tAN,tBN>::value
			&& space_compatible<typename tSA::ColSpace, typename tSB::ColSpace>::value
			&& space_compatible<typename tSA::RowSpace, typename tSB::RowSpace>::value
		>
	{};
}

//--    d: SquareMatrix_                    ///{{{2///////////////////////////
namespace  detail
{
	template< class, typename > 
	struct SquareMatrix_
	{
		//TODO det();
		
		template< class tOut, class tIn > static FLUX_CONSTEXPR_EX
		tOut inv( tIn const& aMat ) noexcept(tOut::staticSize)
		{
			return gauss(aMat, tOut(Init::identity, aMat.n(), aMat.m()) );
		};
	};

	template< typename tType > struct SquareMatrix_<ftl::StaticSize<2>, tType> {
		template< class tIn > static FLUX_CONSTEXPR_EX
		tType det( tIn const& aM ) noexcept
		{
			return aM(0,0)*aM(1,1) - aM(0,1)*aM(1,0);
		}

		template< class tOut, class tIn > static FLUX_CONSTEXPR_EX
		tOut inv( tIn const& aM ) noexcept
		{
			return make_matrix<tOut>(
				 aM(1,1), -aM(0,1),
				-aM(1,0),  aM(0,0)
			) / det(aM);
		}
	};
	template< typename tType > struct SquareMatrix_<ftl::StaticSize<3>, tType> {
		template< class tIn > static FLUX_CONSTEXPR_EX
		tType det( tIn const& aM ) noexcept
		{
			return aM(0,0)*aM(1,1)*aM(2,2) + aM(0,1)*aM(1,2)*aM(2,0)
				+ aM(0,2)*aM(1,0)*aM(2,1) - aM(0,2)*aM(1,1)*aM(2,0)
				- aM(0,1)*aM(1,0)*aM(2,2) - aM(0,0)*aM(1,2)*aM(2,1)
			;
		}

		template< class tOut, class tIn > static FLUX_CONSTEXPR_EX
		tOut inv( tIn const& aM ) noexcept
		{
			return make_matrix<tOut>(
				aM(2,2)*aM(1,1) - aM(2,1)*aM(1,2),
				-(aM(2,2)*aM(0,1) - aM(2,1)*aM(0,2)),
				aM(1,2)*aM(0,1) - aM(1,1)*aM(0,2),

				-(aM(2,2)*aM(1,0) - aM(2,0)*aM(1,2)),
				aM(2,2)*aM(0,0) - aM(2,0)*aM(0,2),
				-(aM(1,2)*aM(0,0) - aM(1,0)*aM(0,2)),

				aM(2,1)*aM(1,0) - aM(2,0)*aM(1,1),
				-(aM(2,1)*aM(0,0) - aM(2,0)*aM(0,1)),
				aM(1,1)*aM(0,0) - aM(1,0)*aM(0,1)
			) / det(aM);
		}
	};
	template< typename tType > struct SquareMatrix_<ftl::StaticSize<4>, tType> {
		template< class tIn > static FLUX_CONSTEXPR_EX
		tType det( tIn const& aM ) noexcept
		{
			return aM(0,0)*
					( aM(1,1)*aM(2,2)*aM(3,3) + aM(1,2)*aM(2,3)*aM(3,1)
					+ aM(1,3)*aM(2,1)*aM(3,2) - aM(1,3)*aM(2,2)*aM(3,1)
					- aM(1,2)*aM(2,1)*aM(3,3) - aM(1,1)*aM(2,3)*aM(3,2) )
				- aM(0,1)*
					( aM(1,0)*aM(2,2)*aM(3,3) + aM(1,2)*aM(2,3)*aM(3,0)
					+ aM(1,3)*aM(2,0)*aM(3,2) - aM(1,3)*aM(2,2)*aM(3,0)
					- aM(1,2)*aM(2,0)*aM(3,3) - aM(1,0)*aM(2,3)*aM(3,2) )
				+ aM(0,2)*
					( aM(1,0)*aM(2,1)*aM(3,3) + aM(1,1)*aM(2,3)*aM(3,0)
					+ aM(1,3)*aM(2,0)*aM(3,1) - aM(1,3)*aM(2,1)*aM(3,0)
					- aM(1,1)*aM(2,0)*aM(3,3) - aM(1,0)*aM(2,3)*aM(3,1) )
				- aM(0,3)*
					( aM(1,0)*aM(2,1)*aM(3,2) + aM(1,1)*aM(2,2)*aM(3,0)
					+ aM(1,2)*aM(2,0)*aM(3,1) - aM(1,2)*aM(2,1)*aM(3,0)
					- aM(1,1)*aM(2,0)*aM(3,2) - aM(1,0)*aM(2,2)*aM(3,1) )
			;
		}

		template< class tOut, class tIn > static FLUX_CONSTEXPR_EX
		tOut inv( tIn const& aM ) noexcept
		{
			/* Tasty copy-pasta.
			 *
			 * Source:
			 *	http://www.euclideanspace.com/maths/algebra/matrix/
			 *		/functions/inverse/fourD/index.htm
			 */
			tOut ret(aM.n(),aM.m());
			ret(0,0) = aM(1,2)*aM(2,3)*aM(3,1) - aM(1,3)*aM(2,2)*aM(3,1) 
				+ aM(1,3)*aM(2,1)*aM(3,2) - aM(1,1)*aM(2,3)*aM(3,2) 
				- aM(1,2)*aM(2,1)*aM(3,3) + aM(1,1)*aM(2,2)*aM(3,3);
			ret(0,1) = aM(0,3)*aM(2,2)*aM(3,1) - aM(0,2)*aM(2,3)*aM(3,1) 
				- aM(0,3)*aM(2,1)*aM(3,2) + aM(0,1)*aM(2,3)*aM(3,2) 
				+ aM(0,2)*aM(2,1)*aM(3,3) - aM(0,1)*aM(2,2)*aM(3,3);
			ret(0,2) = aM(0,2)*aM(1,3)*aM(3,1) - aM(0,3)*aM(1,2)*aM(3,1) 
				+ aM(0,3)*aM(1,1)*aM(3,2) - aM(0,1)*aM(1,3)*aM(3,2) 
				- aM(0,2)*aM(1,1)*aM(3,3) + aM(0,1)*aM(1,2)*aM(3,3);
			ret(0,3) = aM(0,3)*aM(1,2)*aM(2,1) - aM(0,2)*aM(1,3)*aM(2,1) 
				- aM(0,3)*aM(1,1)*aM(2,2) + aM(0,1)*aM(1,3)*aM(2,2) 
				+ aM(0,2)*aM(1,1)*aM(2,3) - aM(0,1)*aM(1,2)*aM(2,3);
			ret(1,0) = aM(1,3)*aM(2,2)*aM(3,0) - aM(1,2)*aM(2,3)*aM(3,0) 
				- aM(1,3)*aM(2,0)*aM(3,2) + aM(1,0)*aM(2,3)*aM(3,2) 
				+ aM(1,2)*aM(2,0)*aM(3,3) - aM(1,0)*aM(2,2)*aM(3,3);
			ret(1,1) = aM(0,2)*aM(2,3)*aM(3,0) - aM(0,3)*aM(2,2)*aM(3,0) 
				+ aM(0,3)*aM(2,0)*aM(3,2) - aM(0,0)*aM(2,3)*aM(3,2) 
				- aM(0,2)*aM(2,0)*aM(3,3) + aM(0,0)*aM(2,2)*aM(3,3);
			ret(1,2) = aM(0,3)*aM(1,2)*aM(3,0) - aM(0,2)*aM(1,3)*aM(3,0) 
				- aM(0,3)*aM(1,0)*aM(3,2) + aM(0,0)*aM(1,3)*aM(3,2) 
				+ aM(0,2)*aM(1,0)*aM(3,3) - aM(0,0)*aM(1,2)*aM(3,3);
			ret(1,3) = aM(0,2)*aM(1,3)*aM(2,0) - aM(0,3)*aM(1,2)*aM(2,0) 
				+ aM(0,3)*aM(1,0)*aM(2,2) - aM(0,0)*aM(1,3)*aM(2,2) 
				- aM(0,2)*aM(1,0)*aM(2,3) + aM(0,0)*aM(1,2)*aM(2,3);
			ret(2,0) = aM(1,1)*aM(2,3)*aM(3,0) - aM(1,3)*aM(2,1)*aM(3,0) 
				+ aM(1,3)*aM(2,0)*aM(3,1) - aM(1,0)*aM(2,3)*aM(3,1) 
				- aM(1,1)*aM(2,0)*aM(3,3) + aM(1,0)*aM(2,1)*aM(3,3);
			ret(2,1) = aM(0,3)*aM(2,1)*aM(3,0) - aM(0,1)*aM(2,3)*aM(3,0) 
				- aM(0,3)*aM(2,0)*aM(3,1) + aM(0,0)*aM(2,3)*aM(3,1) 
				+ aM(0,1)*aM(2,0)*aM(3,3) - aM(0,0)*aM(2,1)*aM(3,3);
			ret(2,2) = aM(0,1)*aM(1,3)*aM(3,0) - aM(0,3)*aM(1,1)*aM(3,0) 
				+ aM(0,3)*aM(1,0)*aM(3,1) - aM(0,0)*aM(1,3)*aM(3,1) 
				- aM(0,1)*aM(1,0)*aM(3,3) + aM(0,0)*aM(1,1)*aM(3,3);
			ret(2,3) = aM(0,3)*aM(1,1)*aM(2,0) - aM(0,1)*aM(1,3)*aM(2,0) 
				- aM(0,3)*aM(1,0)*aM(2,1) + aM(0,0)*aM(1,3)*aM(2,1) 
				+ aM(0,1)*aM(1,0)*aM(2,3) - aM(0,0)*aM(1,1)*aM(2,3);
			ret(3,0) = aM(1,2)*aM(2,1)*aM(3,0) - aM(1,1)*aM(2,2)*aM(3,0) 
				- aM(1,2)*aM(2,0)*aM(3,1) + aM(1,0)*aM(2,2)*aM(3,1) 
				+ aM(1,1)*aM(2,0)*aM(3,2) - aM(1,0)*aM(2,1)*aM(3,2);
			ret(3,1) = aM(0,1)*aM(2,2)*aM(3,0) - aM(0,2)*aM(2,1)*aM(3,0) 
				+ aM(0,2)*aM(2,0)*aM(3,1) - aM(0,0)*aM(2,2)*aM(3,1) 
				- aM(0,1)*aM(2,0)*aM(3,2) + aM(0,0)*aM(2,1)*aM(3,2);
			ret(3,2) = aM(0,2)*aM(1,1)*aM(3,0) - aM(0,1)*aM(1,2)*aM(3,0) 
				- aM(0,2)*aM(1,0)*aM(3,1) + aM(0,0)*aM(1,2)*aM(3,1) 
				+ aM(0,1)*aM(1,0)*aM(3,2) - aM(0,0)*aM(1,1)*aM(3,2);
			ret(3,3) = aM(0,1)*aM(1,2)*aM(2,0) - aM(0,2)*aM(1,1)*aM(2,0) 
				+ aM(0,2)*aM(1,0)*aM(2,1) - aM(0,0)*aM(1,2)*aM(2,1) 
				- aM(0,1)*aM(1,0)*aM(2,2) + aM(0,0)*aM(1,1)*aM(2,2);

			tType const d = aM(0,0) * ret(0,0) + aM(0,1) * ret(1,0) 
				+ aM(0,2) * ret(2,0) + aM(0,3) * ret(3,0);

			return ret / d;
		}
	};
}

//--    operators                           ///{{{2///////////////////////////
#define FLUXTMP_FMLOP_TEMPL1_(op) template< class tMSize, class tNSize, typename T, class tSA, class tNA >
#define FLUXTMP_FMLOP_TEMPL2_(op) template< class tMSize, class tNSize, typename T, class tSA, class tSB, class tNA  >
#define FLUXTMP_FMLOP_CA_ SmallMatrix<tMSize,tNSize,T,tSA>
#define FLUXTMP_FMLOP_CB_ SmallMatrix<tMSize,tNSize,T,tSB>
#define FLUXTMP_FMLOP_RS_ SmallMatrix<tMSize,tNSize,T,tNA>
#define FLUXTMP_FMLOP_NOEX_ noexcept((SmallMatrix<tMSize,tNSize,T,tNA>::staticSize))


	// unary
FLUXTMP_FMLOP_TEMPL1_(op::ClassUnaryPM) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator- (FLUXTMP_FMLOP_CA_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ ret( aV.m(), aV.n() );
	for( std::size_t i = 0; i < aV.size(); ++i )
		ret[i] = -aV[i];
	return ret;
}
FLUXTMP_FMLOP_TEMPL1_(op::ClassUnaryPM) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator+ (FLUXTMP_FMLOP_CA_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ ret( aV.m(), aV.n() );
	for( std::size_t i = 0; i < aV.size(); ++i )
		ret[i] = +aV[i];
	return ret;
}

	// binary: comparisons
FLUXTMP_FMLOP_TEMPL2_(op::ClassCmp) inline FLUX_CONSTEXPR_EX
bool operator== (FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) noexcept
{
	if( aU.m() != aV.m() || aU.n() != aV.n() )
		return false;

	for( std::size_t i = 0; i < aU.size(); ++i )
	{
		if( aU[i] != aV[i] ) return false;
	}

	return true;
}
FLUXTMP_FMLOP_TEMPL2_(op::ClassCmp) inline FLUX_CONSTEXPR_EX
bool operator!= (FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) noexcept
{
	return !(aU == aV);
}

	// binary: arithmetic
FLUXTMP_FMLOP_TEMPL2_(op::ClassAdd) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator+ (FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUX_ASSERT( aU.m() == aV.m() && aU.n() == aV.n() );
	FLUXTMP_FMLOP_RS_ ret( aU.m(), aU.n() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		ret[i] = aU[i]+aV[i];
	return ret;
}
FLUXTMP_FMLOP_TEMPL2_(op::ClassAdd) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator- (FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUX_ASSERT( aU.m() == aV.m() && aU.n() == aV.n() );
	FLUXTMP_FMLOP_RS_ ret( aU.m(), aU.n() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		ret[i] = aU[i]-aV[i];
	return ret;
}

FLUXTMP_FMLOP_TEMPL1_(op::ClassMul) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator* (FLUXTMP_FMLOP_CA_ const& aV, T aX) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ ret( aV.m(), aV.n() );
	for( std::size_t i = 0; i < aV.size(); ++i )
		ret[i] = aV[i]*aX;
	return ret;
}
FLUXTMP_FMLOP_TEMPL1_(op::ClassMul) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator/ (FLUXTMP_FMLOP_CA_ const& aV, T aX) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ ret( aV.m(), aV.n() );
	for( std::size_t i = 0; i < aV.size(); ++i )
		ret[i] = aV[i]/aX;
	return ret;
}

FLUXTMP_FMLOP_TEMPL1_(op::ClassMul) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator* (T aX, FLUXTMP_FMLOP_CA_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ ret( aV.m(), aV.n() );
	for( std::size_t i = 0; i < aV.size(); ++i )
		ret[i] = aX*aV[i];
	return ret;
}

template< class tMSize, class tNSize, class tLSize, typename T, class tSA, class tSB, class tNS > inline FLUX_CONSTEXPR_EX
SmallMatrix<tMSize,tLSize,T,tNS> operator* (SmallMatrix<tMSize,tNSize,T,tSA> const& aU, SmallMatrix<tNSize,tLSize,T,tSB> const& aV) noexcept((SmallMatrix<tMSize,tLSize,T,tNS>::staticSize))
{
	FLUX_ASSERT( aU.n() == aV.m() );

	// TODO: check that with default layout = res and aU linear in innermost

	SmallMatrix<tMSize,tLSize,T,tNS> res( Init::zero, aU.m(), aV.n() );
	for( std::size_t j = 0; j < aU.n(); ++j )
	{
		for( std::size_t k = 0; k < aV.n(); ++k )
		{
			for( std::size_t i = 0; i < aU.m(); ++i )
			{
				/* The following causes MSVC to complain about 
				 *   "error C2440' initializing': cannot convert from int* to int"
				 * when attempting to do a matrix multiplication in a constexpr
				 * constext. Expanding it to the code below avoids that error. 
				 * No bloody idea as to what's going on.
				 */
				//res(i,j) += aU(i,k) * aV(k,j);
				auto x = res(i,j);
				x += aU(i,k) * aV(k,j);
				res(i,j) = x;
			}
		}
	}

	return res;
}


	// binary: assignment
FLUXTMP_FMLOP_TEMPL2_(op::ClassInc) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator+= (FLUXTMP_FMLOP_CA_& aU, FLUXTMP_FMLOP_CB_ const& aV) noexcept
{
	FLUX_ASSERT( aU.m() == aV.m() && aU.n() == aV.n() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		aU[i] += aV[i];
	return aU;
}
FLUXTMP_FMLOP_TEMPL2_(op::ClassInc) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator-= (FLUXTMP_FMLOP_CA_& aU, FLUXTMP_FMLOP_CB_ const& aV) noexcept
{
	FLUX_ASSERT( aU.m() == aV.m() && aU.n() == aV.n() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		aU[i] -= aV[i];
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

template< class tNSize, typename T, class tSA, class tSB, class > FLUX_CONSTEXPR_EX
SmallMatrix<tNSize,tNSize,T,tSA> operator*= ( SmallMatrix<tNSize,tNSize,T,tSA>& aU, SmallMatrix<tNSize,tNSize,T,tSB> const& aV ) noexcept((SmallMatrix<tNSize,tNSize,T,tSA>::staticSize))
{
	auto temp = aU * aV;
	aU = std::move(temp);
	return aU;
}

	// with vector
template< class tMSize, class tNSize, typename T, class tMA, class tVA, class tNA > FLUX_CONSTEXPR_EX
SmallVector<tMSize,T,tNA> operator* ( SmallMatrix<tMSize,tNSize,T,tMA> const& aU, SmallVector<tNSize,T,tVA> const& aV ) noexcept((SmallVector<tMSize,T,tNA>::staticSize))
{
	FLUX_ASSERT( aU.n() == aV.size() );

	SmallVector<tMSize,T,tNA> res( aU.m() );
	for( std::size_t j = 0; j < aU.n(); ++j )
	{
		for( std::size_t i = 0; i < aU.m(); ++i )
		{
			// Workaround for MSVC. See comments in operator*() taking two matrices.
			//res[i] += aU(i,j) * aV[j];
			auto x = res[i];
			x += aU(i,j) * aV[j];
			res[i] = x;
		}
	}
	return res;
}
template< class tMSize, class tNSize, typename T, class tVA, class tMA, class tNA > FLUX_CONSTEXPR_EX
SmallVector<tNSize,T,tNA> operator* ( SmallVector<tMSize,T,tVA> const& aU, SmallMatrix<tMSize,tNSize,T,tMA> const& aV ) noexcept((SmallVector<tNSize,T,tNA>::staticSize))
{
	FLUX_ASSERT( aU.size() == aV.m() );

	SmallVector<tNSize,T,tNA> res( aV.n() );
	for( std::size_t i = 0; i < aV.n(); ++i )
	{	
		for( std::size_t j = 0; j < aV.m(); ++j )
		{
			// Workaround for MSVC. See comments in operator*() taking two matrices.
			//res[i] += aU[j] * aV(j,i);
			auto x = res[i];
			x += aU[j] * aV(j,i);
			res[i] = x;
		}
	}
	return res;
}


//--    functions                           ///{{{2///////////////////////////

#if FLUX_FML_MSVC_ICE_WORKAROUND
#	define FLUXTMP_FML_IDENT_ = {}
#	define FLUXTMP_FML_ROWS_ = smat_rows_t<M>{}
#	define FLUXTMP_FML_COLS_ = smat_cols_t<M>{}
#else // !MSVC_ICE_WORKAROUND
#	define FLUXTMP_FML_IDENT_
#	define FLUXTMP_FML_ROWS_
#	define FLUXTMP_FML_COLS_
#endif // ~ MSVC_ICE_WORKAROUND

	// pseudo constructors -{{{3-
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,2,2> )
(M) make_matrix( smat_type_t<M> a00, smat_type_t<M> a01, smat_type_t<M> a10, smat_type_t<M> a11 ) noexcept
{
	M m( Init::zero, smat_rows_t<M>(2), smat_cols_t<M>(2) );
	m(0,0) = a00;  m(0,1) = a01;
	m(1,0) = a10;  m(1,1) = a11;
	return m;
}
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,3,3> )
(M) make_matrix( smat_type_t<M> a00, smat_type_t<M> a01, smat_type_t<M> a02, smat_type_t<M> a10, smat_type_t<M> a11, smat_type_t<M> a12, smat_type_t<M> a20, smat_type_t<M> a21, smat_type_t<M> a22 ) noexcept
{
	M m( Init::zero, smat_rows_t<M>(3), smat_cols_t<M>(3) );
	m(0,0) = a00;  m(0,1) = a01;  m(0,2) = a02;
	m(1,0) = a10;  m(1,1) = a11;  m(1,2) = a12;
	m(2,0) = a20;  m(2,1) = a21;  m(2,2) = a22;
	return m;
}
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,3,4> )
(M) make_matrix( smat_type_t<M> a00, smat_type_t<M> a01, smat_type_t<M> a02, smat_type_t<M> a03, smat_type_t<M> a10, smat_type_t<M> a11, smat_type_t<M> a12, smat_type_t<M> a13, smat_type_t<M> a20, smat_type_t<M> a21, smat_type_t<M> a22, smat_type_t<M> a23 ) noexcept
{
	M m( Init::zero, smat_rows_t<M>(3), smat_cols_t<M>(4) );
	m(0,0) = a00;  m(0,1) = a01;  m(0,2) = a02;  m(0,3) = a03;
	m(1,0) = a10;  m(1,1) = a11;  m(1,2) = a12;  m(1,3) = a13;
	m(2,0) = a20;  m(2,1) = a21;  m(2,2) = a22;  m(2,3) = a23;
	return m;
}
template< class M > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,4,4> )
(M) make_matrix( smat_type_t<M> a00, smat_type_t<M> a01, smat_type_t<M> a02, smat_type_t<M> a03, smat_type_t<M> a10, smat_type_t<M> a11, smat_type_t<M> a12, smat_type_t<M> a13, smat_type_t<M> a20, smat_type_t<M> a21, smat_type_t<M> a22, smat_type_t<M> a23, smat_type_t<M> a30, smat_type_t<M> a31, smat_type_t<M> a32, smat_type_t<M> a33 ) noexcept
{
	M m( Init::zero, smat_rows_t<M>(4), smat_cols_t<M>(4) );
	m(0,0) = a00;  m(0,1) = a01;  m(0,2) = a02;  m(0,3) = a03;
	m(1,0) = a10;  m(1,1) = a11;  m(1,2) = a12;  m(1,3) = a13;
	m(2,0) = a20;  m(2,1) = a21;  m(2,2) = a22;  m(2,3) = a23;
	m(3,0) = a30;  m(3,1) = a31;  m(3,2) = a32;  m(3,3) = a33;
	return m;
}

template< class M, class tSpec > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,2,0> )
(M) make_matrix_from_rows( SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const& aR0, SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const& aR1 ) noexcept(M::staticSize)
{
	FLUX_ASSERT( aR0.size() == aR1.size() );

	M m( Init::zero, smat_rows_t<M>(2), aR0.size() );
	for( std::size_t i = 0; i < aR0.size(); ++i ) m(0,i) = aR0[i];
	for( std::size_t i = 0; i < aR0.size(); ++i ) m(1,i) = aR1[i];
	return m;
}
template< class M, class tSpec > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,3,0> )
(M) make_matrix_from_rows( SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const& aR0, SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const& aR1, SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const& aR2 ) noexcept(M::staticSize)
{
	FLUX_ASSERT( aR0.size() == aR1.size() && aR0.size() == aR2.size() );

	M m( Init::zero, smat_rows_t<M>(3), aR0.size() );
	for( std::size_t i = 0; i < aR0.size(); ++i ) m(0,i) = aR0[i];
	for( std::size_t i = 0; i < aR0.size(); ++i ) m(1,i) = aR1[i];
	for( std::size_t i = 0; i < aR0.size(); ++i ) m(2,i) = aR2[i];
	return m;
}
template< class M, class tSpec > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,4,0> )
(M) make_matrix_from_rows( SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const& aR0, SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const& aR1, SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const& aR2, SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const& aR3 ) noexcept(M::staticSize)
{
	FLUX_ASSERT( aR0.size() == aR1.size() && aR0.size() == aR2.size() && aR0.size() == aR3.size() );

	M m( Init::zero, smat_rows_t<M>(4), aR0.size() );
	for( std::size_t i = 0; i < aR0.size(); ++i ) m(0,i) = aR0[i];
	for( std::size_t i = 0; i < aR0.size(); ++i ) m(1,i) = aR1[i];
	for( std::size_t i = 0; i < aR0.size(); ++i ) m(2,i) = aR2[i];
	for( std::size_t i = 0; i < aR0.size(); ++i ) m(3,i) = aR3[i];
	return m;
}

template< class M, class tSpec > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,0,2> )
(M) make_matrix_from_cols( SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const& aC0, SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const& aC1 ) noexcept(M::staticSize)
{
	FLUX_ASSERT( aC0.size() == aC1.size() );

	M m( Init::zero, aC0.size(), smat_cols_t<M>(2) );
	for( std::size_t i = 0; i < aC0.size(); ++i ) m(i,0) = aC0[i];
	for( std::size_t i = 0; i < aC0.size(); ++i ) m(i,1) = aC1[i];
	return m;
}
template< class M, class tSpec > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,0,3> )
(M) make_matrix_from_cols( SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const& aC0, SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const& aC1, SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const& aC2 ) noexcept(M::staticSize)
{
	FLUX_ASSERT( aC0.size() == aC1.size() && aC0.size() == aC2.size() );

	M m( Init::zero, aC0.size(), smat_cols_t<M>(3) );
	for( std::size_t i = 0; i < aC0.size(); ++i ) m(i,0) = aC0[i];
	for( std::size_t i = 0; i < aC0.size(); ++i ) m(i,1) = aC1[i];
	for( std::size_t i = 0; i < aC0.size(); ++i ) m(i,2) = aC2[i];
	return m;
}
template< class M, class tSpec > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,0,4> )
(M) make_matrix_from_cols( SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const& aC0, SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const& aC1, SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const& aC2, SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const& aC3 ) noexcept(M::staticSize)
{
	FLUX_ASSERT( aC0.size() == aC1.size() && aC0.size() == aC2.size() && aC0.size() == aC3.size() );

	M m( Init::zero, aC0.size(), smat_cols_t<M>(4) );
	for( std::size_t i = 0; i < aC0.size(); ++i ) m(i,0) = aC0[i];
	for( std::size_t i = 0; i < aC0.size(); ++i ) m(i,1) = aC1[i];
	for( std::size_t i = 0; i < aC0.size(); ++i ) m(i,2) = aC2[i];
	for( std::size_t i = 0; i < aC0.size(); ++i ) m(i,3) = aC3[i];
	return m;
}

template< class M > inline
FLUX_ENABLE_IF( is_small_matrix<M> )
(M) make_matrix( ftl::not_null<smat_type_t<M> const*> aData, smat_rows_t<M> aM FLUXTMP_FML_ROWS_, smat_cols_t<M> aN FLUXTMP_FML_COLS_ ) noexcept(M::staticSize)
{
	smat_type_t<M> const* ptr = aData;

	M m( Init::zero, aM, aN );
	for( std::size_t i = 0; i < aM; ++i )
	{
		for( std::size_t j = 0; j < aN; ++j )
			m(i,j) = *ptr++;
	}
	return m;
}


template< class M > inline constexpr
FLUX_ENABLE_IF( is_small_matrix<M> )
(M) make_zero( Identity<M> FLUXTMP_FML_IDENT_, smat_rows_t<M> aM FLUXTMP_FML_ROWS_, smat_cols_t<M> aN FLUXTMP_FML_COLS_ ) noexcept(M::staticSize)
{
	return M( Init::zero, aM, aN );
}
template< class M > constexpr
FLUX_ENABLE_IF( is_small_matrix<M> )
(M) make_one( Identity<M> FLUXTMP_FML_IDENT_, smat_rows_t<M> aM FLUXTMP_FML_ROWS_, smat_cols_t<M> aN FLUXTMP_FML_COLS_ ) noexcept(M::staticSize)
{
	return M( Init::fill, smat_type_t<M>(1), aM, aN );
}
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M> )
(M) make_identity( Identity<M> FLUXTMP_FML_IDENT_, smat_rows_t<M> aM FLUXTMP_FML_ROWS_, smat_cols_t<M> aN FLUXTMP_FML_COLS_ ) noexcept(M::staticSize)
{
	return M( Init::identity, aM, aN );
}
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M> )
(M) make_splat( smat_type_t<M> aX, Identity<M> FLUXTMP_FML_IDENT_, smat_rows_t<M> aM FLUXTMP_FML_ROWS_, smat_cols_t<M> aN FLUXTMP_FML_COLS_ ) noexcept(M::staticSize)
{
	return M( Init::fill, aX, aM, aN );
}

	// conversions -{{{3-
template< class M, class tMSize, class tNSize, typename T, class tSA > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && detail::SMatCompatResize<M, SmallMatrix<tMSize,tNSize,T,tSA>>::value )
(M) expand( SmallMatrix<tMSize,tNSize,T,tSA> const& aU, M const& aV ) noexcept(M::staticSize)
{
	FLUX_ASSERT( aU.n() <= aV.n() && aU.m() <= aV.m() );

	M ret( aV.m(), aV.n() );
	std::size_t j = 0;
	for( ; j < aU.n(); ++j )
	{
		std::size_t i = 0;
		for( ; i < aU.m(); ++i ) ret(i,j) = aU(i,j);
		for( ; i < aV.m(); ++i ) ret(i,j) = aV(i,j);
	}
	for( ; j < aV.n(); ++j )
	{
		for( std::size_t i = 0; i < aV.m(); ++i )
			ret(i,j) = aV(i,j);
	}
	return ret;

}

template< class M, class tMSize, class tNSize, typename T, class tSA > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && detail::SMatCompatResize<M, SmallMatrix<tMSize,tNSize,T,tSA>>::value )
(M) shrink( SmallMatrix<tMSize,tNSize,T,tSA> const& aU, smat_rows_t<M> aMSize FLUXTMP_FML_ROWS_, smat_cols_t<M> aNSize FLUXTMP_FML_COLS_ ) noexcept(M::staticSize)
{
	FLUX_ASSERT( aU.n() >= aNSize && aU.m() >= aMSize );

	M ret( aMSize, aNSize );
	for( std::size_t j = 0; j < aNSize; ++j )
	{
		for( std::size_t i = 0; i < aMSize; ++i ) ret(i,j) = aU(i,j);
	}
	return ret;
}

template< class M, class tMSize, class tNSize, typename T, class tSA > inline /*FLUX_CONSTEXPR_EX*/
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && detail::SMatCompatNarrow<M,SmallMatrix<tMSize,tNSize,T,tSA>>::value )
(M) narrow( SmallMatrix<tMSize,tNSize,T,tSA> const& aU )
{
	M ret( aU.m(), aU.n() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		ret[i] = util::narrow<smat_type_t<M>>(aU[i]);
	return ret;
}

template< class M, class tMSize, class tNSize, typename T, class tSA > inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && detail::SMatCheckSize<M,SmallMatrix<tMSize,tNSize,T,tSA>>::value )
(M) matrix_cast( SmallMatrix<tMSize,tNSize,T,tSA> const& aU ) noexcept(M::staticSize)
{
	M ret( aU.m(), aU.n() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		ret[i] = smat_type_t<M>(aU[i]);
	return ret;
}
template< class tMSize, class tNSize, typename T, class tSA, class tVec > inline FLUX_CONSTEXPR_EX
tVec get_row( SmallMatrix<tMSize,tNSize,T,tSA> const& aV, std::size_t aI ) noexcept(ftl::is_static_value<tNSize>::value)
{
	tVec ret( aV.n() );
	for( std::size_t i = 0; i < aV.n(); ++i ) ret[i] = aV(aI,i);
	return ret;
}
template< class tMSize, class tNSize, typename T, class tSA, class tVec > inline FLUX_CONSTEXPR_EX
tVec get_col( SmallMatrix<tMSize,tNSize,T,tSA> const& aV, std::size_t aI ) noexcept(ftl::is_static_value<tMSize>::value)
{
	tVec ret( aV.m() );
	for( std::size_t i = 0; i < aV.m(); ++i ) ret[i] = aV(i,aI);
	return ret;
}

	// helpers
template< class tMSize, class tNSize, typename T, class tSA > inline FLUX_CONSTEXPR_EX
T* data_ptr( SmallMatrix<tMSize,tNSize,T,tSA>& aU ) noexcept
{
	return aU.data();
}
template< class tMSize, class tNSize, typename T, class tSA > inline FLUX_CONSTEXPR_EX
T const* data_ptr( SmallMatrix<tMSize,tNSize,T,tSA> const& aU ) noexcept
{
	return aU.data();
}

	// simple transformations -{{{3-
template< class tMSize, class tNSize, typename T, class tSA, class tNA > inline FLUX_CONSTEXPR_EX
SmallMatrix<tNSize,tMSize,T,tNA> transpose( SmallMatrix<tMSize,tNSize,T,tSA> const& aV ) noexcept(ftl::is_static_value<tMSize>::value && ftl::is_static_value<tNSize>::value)
{
	SmallMatrix<tNSize,tMSize,T,tNA> ret( aV.n(), aV.m() );
	for( std::size_t i = 0; i < aV.m(); ++i )
	{
		for( std::size_t j = 0; j < aV.n(); ++j )
			ret(j,i) = aV(i,j);
	}
	return ret;
}

	// square matrices -{{{3-

template< class tNSize, typename T, class tSA > FLUX_CONSTEXPR_EX
T det( SmallMatrix<tNSize,tNSize,T,tSA> const& aU ) noexcept
{
	FLUX_ASSERT( aU.m() == aU.n() );
	return detail::SquareMatrix_<tNSize,T>::det( aU );
}

template< class tNSize, typename T, class tSA > FLUX_CONSTEXPR_EX
T trace( SmallMatrix<tNSize,tNSize,T,tSA> const& aU ) noexcept
{
	FLUX_ASSERT( aU.m() == aU.n() );

	T res = 0;
	for( std::size_t i = 0; i < aU.m(); ++i )
		res += aU(i,i);
	return res;
}

template< class tNSize, typename T, class tSA, class tNA > inline FLUX_CONSTEXPR_EX
SmallMatrix<tNSize,tNSize,T,tNA> invert( SmallMatrix<tNSize,tNSize,T,tSA> const& aU ) noexcept(ftl::is_static_value<tNSize>::value)
{
	FLUX_ASSERT( aU.m() == aU.n() );
	return detail::SquareMatrix_<tNSize,T>::template inv<SmallMatrix<tNSize,tNSize,T,tNA>>( aU );
}

namespace detail
{
	/* Note: gauss_() doesn't qualify to be a constexpr function in reality.
	 * Maybe something to look at in the future.
	 */
	template< class tMat, typename tTyV > inline FLUX_CONSTEXPR_EX
	void gauss_( tMat& FLUX_RESTRICT aLHS, tTyV* FLUX_RESTRICT aRHS, std::size_t aRM, std::size_t aRN )
	{
		FLUX_ASSERT( aLHS.n() == aLHS.m() ); // left hand side must be square
		FLUX_ASSERT( aLHS.n() == aRM ); // right hand size must have same number of rows

		auto const N = aLHS.n();

		// reduce to upper triangular
		for( std::size_t i = 0; i < N; ++i )
		{
			auto maxIdx = i;
			auto maxVal = aLHS(i,i);

			for( std::size_t j = i+1; j < N; ++j )
			{
				if( aLHS(j,i) > maxVal )
				{
					maxIdx = j;
					maxVal = aLHS(j,i);
				}
			}

			for( std::size_t k = i; k < N; ++k )
				std::swap( aLHS(i,k), aLHS(maxIdx,k) );

			for( std::size_t k = 0; k < aRN; ++k )
				std::swap( aRHS[linear_index_(i,k,aRM,aRN)], aRHS[linear_index_(maxIdx,k,aRM,aRN)] );

			for( std::size_t j = i+1; j < N; ++j )
			{
				auto const x = aLHS(j,i) / aLHS(i,i);

				aLHS(j,i) = smat_type_t<tMat>(0);
				for( std::size_t k = i+1; k < N; ++k )
					aLHS(j,k) -= x * aLHS(i,k);

				for( std::size_t k = 0; k < aRN; ++k )
					aRHS[linear_index_(j,k,aRM,aRN)] -= x * aRHS[linear_index_(i,k,aRM,aRN)];
			}
		}

		// -> upper triangular. solve now
		for( std::size_t ii = 0; ii < N; ++ii )
		{
			auto const i = N-ii-1;
			auto const r = smat_type_t<tMat>(1) / aLHS(i,i);

			aLHS(i,i) *= r;
			for( std::size_t k = 0; k < aRN; ++k )
				aRHS[linear_index_(i,k,aRM,aRN)] *= r;

			for( std::size_t j = 0; j < i; ++j )
			{
				auto const r = aLHS(j,i) / aLHS(i,i);
				for( std::size_t k = 0; k <= i; ++k )
					aLHS(j,k) -= r * aLHS(i,k);

				for( std::size_t k = 0; k < aRN; ++k )
					aRHS[linear_index_(j,k,aRM,aRN)] -= r * aRHS[linear_index_(i,k,aRM,aRN)];
			}
		}
	}
}

template< class tNSize, typename T, class tSA, typename Y, class tSB, class tNA >  inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( space_compatible<typename tSA::ColSpace,typename tSB::Space> )
(SmallVector<tNSize,Y,tNA>) gauss( SmallMatrix<tNSize,tNSize,T,tSA> aLHS, SmallVector<tNSize,Y,tSB> const& aRHS ) noexcept(ftl::is_static_value<tNSize>::value)
{
	FLUX_ASSERT( aLHS.n() == aLHS.m() );
	FLUX_ASSERT( aLHS.m() == aRHS.size() );

	SmallVector<tNSize,Y,tNA> ret( aRHS.size() );
	for( std::size_t i = 0; i < ret.size(); ++i )
		ret[i] = aRHS[i];

	detail::gauss_( aLHS, ret.data(), aRHS.size(), 1 );
	return ret;
}
template< class tNSize, typename T, class tSA, class tMSize, typename Y, class tSB >  inline FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( space_compatible<typename tSA::ColSpace, typename tSB::RowSpace> )
(SmallMatrix<tNSize,tMSize,Y,tSB>) gauss( SmallMatrix<tNSize,tNSize,T,tSA> aLHS, SmallMatrix<tNSize,tMSize,Y,tSB> aRHS ) noexcept
{
	FLUX_ASSERT( aLHS.n() == aLHS.m() );
	FLUX_ASSERT( aLHS.m() == aRHS.m() );

	SmallMatrix<tNSize,tMSize,Y,tSB> ret( std::move(aRHS) );
	detail::gauss_( aLHS, ret.data(), aRHS.m(), aRHS.n() ); // TODO-layout changes strides.
	return ret;
}

	// Element wise -{{{3-
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) inline FLUX_CONSTEXPR_EX
T max( FLUXTMP_FMLOP_CA_ const& aV) noexcept
{
	T res = aV[0];
	for( std::size_t i = 1; i < aV.size(); ++i )
		res = std::max( res, aV[i] );
	return res;
}
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) inline FLUX_CONSTEXPR_EX
T min( FLUXTMP_FMLOP_CA_ const& aV) noexcept
{
	T res = aV[0];
	for( std::size_t i = 1; i < aV.size(); ++i )
		res = std::min( res, aV[i] );
	return res;
}

FLUXTMP_FMLOP_TEMPL2_(op::ClassNorm) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ max( FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUX_ASSERT( aU.m() == aV.m() && aU.n() == aV.n() );

	FLUXTMP_FMLOP_RS_ res( aU.m(), aU.n() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		res[i] = std::max( aU[i], aV[i] );
	return res;
}
FLUXTMP_FMLOP_TEMPL2_(op::ClassNorm) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ min( FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	FLUX_ASSERT( aU.m() == aV.m() && aU.n() == aV.n() );

	FLUXTMP_FMLOP_RS_ res( aU.m(), aU.n() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		res[i] = std::min( aU[i], aV[i] );
	return res;
}

FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ abs( FLUXTMP_FMLOP_CA_ const& aU ) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ res( aU.m(), aU.n() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		res[i] = std::abs( aU[i] );
	return res;
}
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ sign( FLUXTMP_FMLOP_CA_ const& aU ) FLUXTMP_FMLOP_NOEX_
{
	FLUXTMP_FMLOP_RS_ res( aU.m(), aU.n() );
	for( std::size_t i = 0; i < aU.size(); ++i )
		res[i] = detail::smat_sign_(aU[i]);
	return res;
}

FLUXTMP_FMLOP_TEMPL2_(op::ClassNorm) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ mix( T aX, FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV ) FLUXTMP_FMLOP_NOEX_
{
	return lerp( aX, aU, aV );
}
FLUXTMP_FMLOP_TEMPL2_(op::ClassNorm) inline FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ lerp( T aX, FLUXTMP_FMLOP_CA_ const& aU, FLUXTMP_FMLOP_CB_ const& aV) FLUXTMP_FMLOP_NOEX_
{
	return aU + (aV-aU)*aX;
}

#undef FLUXTMP_FMLOP_NOEX_
#undef FLUXTMP_FMLOP_RS_
#undef FLUXTMP_FMLOP_CB_
#undef FLUXTMP_FMLOP_CA_
#undef FLUXTMP_FMLOP_TEMPL2_
#undef FLUXTMP_FMLOP_TEMPL1_

//--    <<< ~ flux::fml namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
