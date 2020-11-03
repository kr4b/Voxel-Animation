/*-******************************************************* -- HEADER -{{{1- */
/*- Small, dense matrix
 *
 * Small dense matrix, also overengineered.
 *
 * TODO: maybe enable make_matrix<> with fixed sizes also for dynamic matrices.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FML_SMALL_MATRIX_HPP_74D03CF2_8E2F_4A43_A0CF_CE5C2BBD307D
#define FLUX_FML_SMALL_MATRIX_HPP_74D03CF2_8E2F_4A43_A0CF_CE5C2BBD307D

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/identity.hpp>
#include <flux/std/enable_if.hpp>

#include <flux/compat/voidt.hpp>
#include <flux/compat/constexpr.hpp>

#include <flux/ftl/dsvalue.hpp>
#include <flux/ftl/dsstore.hpp>
#include <flux/ftl/not_null.hpp>
#include <flux/ftl/explicit_init_list.hpp>

#include "forward.hpp"
#include "common.hpp"
#include "small_vector.hpp"

#include <utility>
#include <cstddef>

//--    >>> namespace = flux::fml >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    aspect                          ///{{{2///////////////////////////////
namespace aspect
{
	struct RowMajor {};
	struct ColMajor {};

	template< class tColSpace = space::AnySpace, class tRowSpace = tColSpace /*, class tLayout = XXX*/ >
	struct MatrixSpec
	{
		using ColSpace = tColSpace;
		using RowSpace = tRowSpace;
		
		//using Layout = tLayout;
	};
}

//--    $ detail                        ///{{{2///////////////////////////////
namespace detail
{
	template< class, class > struct SMatDimEBO;
	template< class, class > struct SMatElementCount;

	template< class, class, class = compat::void_t<> > struct SMatCompat;
}

//--    SmallMatrix                     ///{{{2///////////////////////////////
template< class tMSize, class tNSize, typename T = float, class tSpec = aspect::MatrixSpec<> >
struct SmallMatrix
	: detail::SMatDimEBO<tMSize,tNSize>
{
	using value_type = T;
	using ElementCount = typename detail::SMatElementCount<tMSize,tNSize>::type;

	static constexpr bool staticSize = ftl::is_static_value<ElementCount>::value;

	ftl::DSStore<ElementCount, T> elems;

	SmallMatrix() = default;

	SmallMatrix( SmallMatrix const& ) = default;
	SmallMatrix& operator= (SmallMatrix const&) = default;

	SmallMatrix( SmallMatrix&& ) = default;
	SmallMatrix& operator= (SmallMatrix&&) = default;

	FLUX_CONSTEXPR_EX SmallMatrix( tMSize, tNSize ) noexcept(staticSize);
	FLUX_CONSTEXPR_EX explicit
	SmallMatrix( Init::Zero, tMSize = tMSize(), tNSize = tNSize() ) noexcept(staticSize);
	FLUX_CONSTEXPR_EX explicit
	SmallMatrix( Init::Identity, tMSize = tMSize(), tNSize = tNSize() ) noexcept(staticSize);
	FLUX_CONSTEXPR_EX 
	SmallMatrix( Init::Fill, T, tMSize = tMSize(), tNSize = tNSize() ) noexcept(staticSize);

	SmallMatrix( ftl::ExplicitInitList<T>, tMSize = tMSize(), tNSize = tNSize() );

	template< class tOtherMat, 
		class = FLUX_ENABLE_IF(detail::SMatCompat<SmallMatrix,tOtherMat>)(int) 
	> FLUX_CONSTEXPR_EX
	SmallMatrix( tOtherMat const& ) noexcept(staticSize);
	template< class tOtherMat, 
		class = FLUX_ENABLE_IF(detail::SMatCompat<SmallMatrix,tOtherMat>)(int) 
	> FLUX_CONSTEXPR_EX
	SmallMatrix& operator=( tOtherMat const& ) noexcept(staticSize);


	FLUX_CONSTEXPR_EX T& operator[] (std::size_t) noexcept;
	constexpr T const& operator[] (std::size_t) const noexcept;

	FLUX_CONSTEXPR_EX T& operator() (std::size_t, std::size_t) noexcept;
	constexpr T const& operator() (std::size_t, std::size_t) const noexcept;

	constexpr tMSize m() const noexcept;
	constexpr tNSize n() const noexcept;
	constexpr ElementCount size() const noexcept;

	FLUX_CONSTEXPR_EX T* data() noexcept;
	constexpr T const* data() const noexcept;
};

//--    meta functions                  ///{{{2///////////////////////////////

/** \anchor MatTraits \name Meta: Matrix traits
 * \brief Matrix traits and meta-functions
 *
 * Meta functions to identify and manipulate the matrix types.
 */
//@{

template< class, std::size_t, std::size_t > struct is_small_matrix 
	: std::false_type
{};

namespace detail
{
	template< class > struct SMatType;

	template< class > struct SMatRows;
	template< class > struct SMatCols;
	template< class > struct SMatRowSpace;
	template< class > struct SMatColSpace;

	template< class, class > struct SMatRebindCol;
	template< class, class > struct SMatRebindRow;
	template< class, class, class > struct SMatRebind;
}

template< class M > using smat_type_t = typename detail::SMatType<M>::type;

template< class M > using smat_rows_t = typename detail::SMatRows<M>::type;
template< class M > using smat_cols_t = typename detail::SMatCols<M>::type;

template< class M > using smat_row_space_t = typename detail::SMatRowSpace<M>::type;
template< class M > using smat_col_space_t = typename detail::SMatColSpace<M>::type;

template< class M, class tSpace >
using smat_rebind_col_space_t = typename detail::SMatRebindCol<M,tSpace>::type;
template< class M, class tSpace >
using smat_rebind_row_space_t = typename detail::SMatRebindRow<M,tSpace>::type;
template< class M, class tColSpace, class tRowSpace >
using smat_rebind_spaces_t = typename detail::SMatRebind<M,tColSpace,tRowSpace>::type;

namespace detail
{
	template< class, class > struct SMatResultSpec;
	template< class, class > struct SMatMulResultSpec;
	template< class, class > struct SMatSVecResultSpec;
	template< class, class > struct SVecSMatResultSpec;

	template< class, class > struct SMatCheckSize;
	template< class, class, class = compat::void_t<> > struct SMatCompatResize;
	template< class, class, class = compat::void_t<> > struct SMatCompatNarrow;

	//TODO: for square matrices... so that StaticSize<17> and DynamicSize = 17 can work
	//template< class > struct SMatCheckSquare;
}

//--    operators                       ///{{{2///////////////////////////////
#define FLUXTMP_FMLOP_TEMPL1_(op) template< class tMSize, class tNSize, typename T, class tSA, class tNA = tSA >
#define FLUXTMP_FMLOP_TEMPL2_(op) template< class tMSize, class tNSize, typename T, class tSA, class tSB, class tNA = typename detail::SMatResultSpec<tSA,tSB>::type >
#define FLUXTMP_FMLOP_CA_ SmallMatrix<tMSize,tNSize,T,tSA>
#define FLUXTMP_FMLOP_CB_ SmallMatrix<tMSize,tNSize,T,tSB>
#define FLUXTMP_FMLOP_RS_ SmallMatrix<tMSize,tNSize,T,tNA>
#define FLUXTMP_FMLOP_NOEX_ noexcept((SmallMatrix<tMSize,tNSize,T,tNA>::staticSize))

/** \name Matrix operations
 * \brief Matrix operations
 *
 * Matrix operations in operator form. With the exception of the
 * matrix-by-matrix and matrix-by-vector/vector-by-matrix multiplications,
 * these operations are element-wise.
 *
 * For element-wise operations, the spaces of the matrices must match. For the
 * multiplications, the spaces are propagated. Consider the matrices
   \code
   using MWMat = SmallMatrix< ..., aspect::MatrixSpec<WorldSpace,ModelSpace> >;
   using WCMat = SmallMatrix< ..., aspect::MatrixSpec<CameraSpace,WorldSpace> >;
  
   MWMat mw = ...;
   WCMat wc = ...;
  
   auto const mc = wc * mw;
   \endcode
 * The matrix `mc` will have be `aspect::MatrixSpec<CameraSpace,ModelSpace>`,
 * i.e. a matrix that transforms from `ModelSpace` to `CameraSpace`. Similarly,
   \code
   auto const cvec = mc * make_vector<...>( ... );
   \endcode
 * expects a `ModelSpace` vector and produces a `CameraSpace` vector `cvec`.
 *
 * \see space_compatible, space::AnySpace
 */
//@{
	// unary
FLUXTMP_FMLOP_TEMPL1_(op::ClassUnaryPM) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator- (FLUXTMP_FMLOP_CA_ const&) FLUXTMP_FMLOP_NOEX_;
FLUXTMP_FMLOP_TEMPL1_(op::ClassUnaryPM) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator+ (FLUXTMP_FMLOP_CA_ const&) FLUXTMP_FMLOP_NOEX_;

	// binary: comparisons
FLUXTMP_FMLOP_TEMPL2_(op::ClassCmp) FLUX_CONSTEXPR_EX
bool operator== (FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) noexcept;
FLUXTMP_FMLOP_TEMPL2_(op::ClassCmp) FLUX_CONSTEXPR_EX
bool operator!= (FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) noexcept;

	// binary: arithmetic
FLUXTMP_FMLOP_TEMPL2_(op::ClassAdd) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator+ (FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) FLUXTMP_FMLOP_NOEX_;
FLUXTMP_FMLOP_TEMPL2_(op::ClassAdd) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator- (FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) FLUXTMP_FMLOP_NOEX_;

FLUXTMP_FMLOP_TEMPL1_(op::ClassMul) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator* (FLUXTMP_FMLOP_CA_ const&, T) FLUXTMP_FMLOP_NOEX_;
FLUXTMP_FMLOP_TEMPL1_(op::ClassMul) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator/ (FLUXTMP_FMLOP_CA_ const&, T) FLUXTMP_FMLOP_NOEX_;

FLUXTMP_FMLOP_TEMPL1_(op::ClassMul) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator* (T, FLUXTMP_FMLOP_CA_ const&) FLUXTMP_FMLOP_NOEX_;

template< class tMSize, class tNSize, class tLSize, typename T, class tSA, class tSB, class tNS = typename detail::SMatMulResultSpec<tSA,tSB>::type > FLUX_CONSTEXPR_EX
SmallMatrix<tMSize,tLSize,T,tNS> operator* (
	SmallMatrix<tMSize,tNSize,T,tSA> const&,
	SmallMatrix<tNSize,tLSize,T,tSB> const&
) noexcept((SmallMatrix<tMSize,tLSize,T,tNS>::staticSize));

	// binary: assignment
FLUXTMP_FMLOP_TEMPL2_(op::ClassInc) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator+= (FLUXTMP_FMLOP_CA_&, FLUXTMP_FMLOP_CB_ const&) noexcept;
FLUXTMP_FMLOP_TEMPL2_(op::ClassInc) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator-= (FLUXTMP_FMLOP_CA_&, FLUXTMP_FMLOP_CB_ const&) noexcept;

FLUXTMP_FMLOP_TEMPL1_(op::ClassScale) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator*= (FLUXTMP_FMLOP_CA_&, T) noexcept;
FLUXTMP_FMLOP_TEMPL1_(op::ClassScale) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator/= (FLUXTMP_FMLOP_CA_&, T) noexcept;

template< class tNSize, typename T, class tSA, class tSB, class = typename detail::SMatCompat<tSA,tSB>::type > FLUX_CONSTEXPR_EX
SmallMatrix<tNSize,tNSize,T,tSA> operator*= (
	SmallMatrix<tNSize,tNSize,T,tSA>&,
	SmallMatrix<tNSize,tNSize,T,tSB> const& 
) noexcept((SmallMatrix<tNSize,tNSize,T,tSA>::staticSize));

	// with vector
template< class tMSize, class tNSize, typename T, class tMA, class tVA, class tNA = typename detail::SMatSVecResultSpec<tMA,tVA>::type > FLUX_CONSTEXPR_EX
SmallVector<tMSize,T,tNA> operator* (
	SmallMatrix<tMSize,tNSize,T,tMA> const&, 
	SmallVector<tNSize,T,tVA> const& 
) noexcept((SmallVector<tMSize,T,tNA>::staticSize));
template< class tMSize, class tNSize, typename T, class tVA, class tMA, class tNA = typename detail::SVecSMatResultSpec<tVA,tMA>::type > FLUX_CONSTEXPR_EX
SmallVector<tNSize,T,tNA> operator* (
	SmallVector<tMSize,T,tVA> const&,
	SmallMatrix<tMSize,tNSize,T,tMA> const&
) noexcept((SmallVector<tNSize,T,tNA>::staticSize));
//@}

//--    functions                       ///{{{2///////////////////////////////

	// pseudo constructors
/** \name Matrix pseudo-constructors
 * \brief Matrix pseudo-constructors
 *
 * Create `SmallMatrix` of type \a M as follows:
 *
 *  - `make_matrix`: from the provided values(*)
 *  - `make_zero`: `SmallMatrix` with all elements equal to zero
 *  - `make_one`: `SmallMatrix` with all elements equal to one
 *  - `make_identity`: Identity `SmallMatrix`
 *
 * Examples:
   \code
   // For statically sized matrices.
   auto const v = make_matrix<mat33f>( 
   	1.f, 2.f, 3.f, 
	4.f, 5.f, 6.f, 
	7.f, 8.f, 9.f 
   );
   auto const z = make_zero<mat33f>(); // alt: make_zero( flux::Identity<mat33f>{} );
   auto const I = make_identity<mat33f>(); // idenentity matrix
   
   // Dynamically sized:
   auto const d = make_zero( Identity<DynamicMatrix>{}, 123, 321 ); // 123x321 matrix
   \endcode
 *
 * (*) `make_matrix` with four arguments is valid only for a `SmallMatrix` with
 * a static size of two by two. Similarly, the nine argument version can only
 * be used to create matrices of size \f$3\times 3\f$; the sixteen argument
 * version can only be used to create matrices of size \f$4\times 4\f$. A
 * generic `make_matrix` takes the sizes and a pointer -- this version can be
 * used on all matrices (but it requires that the provided size matches with
 * the size of static matrix).
 *
 * The additional size-arguments in e.g. `make_zero()` are required for
 * dynamically sized matrices. It may be omitted for statically sized ones.
 */
//@{
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,2,2> )
(M) make_matrix( 
	smat_type_t<M>, smat_type_t<M>,
	smat_type_t<M>, smat_type_t<M>
) noexcept;
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,3,3> )
(M) make_matrix( 
	smat_type_t<M>, smat_type_t<M>, smat_type_t<M>,
	smat_type_t<M>, smat_type_t<M>, smat_type_t<M>,
	smat_type_t<M>, smat_type_t<M>, smat_type_t<M>
) noexcept;
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,3,4> )
(M) make_matrix( 
	smat_type_t<M>, smat_type_t<M>, smat_type_t<M>, smat_type_t<M>,
	smat_type_t<M>, smat_type_t<M>, smat_type_t<M>, smat_type_t<M>,
	smat_type_t<M>, smat_type_t<M>, smat_type_t<M>, smat_type_t<M>
) noexcept;
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,4,4> )
(M) make_matrix( 
	smat_type_t<M>, smat_type_t<M>, smat_type_t<M>, smat_type_t<M>,
	smat_type_t<M>, smat_type_t<M>, smat_type_t<M>, smat_type_t<M>,
	smat_type_t<M>, smat_type_t<M>, smat_type_t<M>, smat_type_t<M>,
	smat_type_t<M>, smat_type_t<M>, smat_type_t<M>, smat_type_t<M>
) noexcept;

//TODO: check spaces.
template< class M, class tSpec > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,2,0> )
(M) make_matrix_from_rows( 
	SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const&,
	SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const&
) noexcept(M::staticSize);
template< class M, class tSpec > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,3,0> )
(M) make_matrix_from_rows( 
	SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const&,
	SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const&,
	SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const&
) noexcept(M::staticSize);
template< class M, class tSpec > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,4,0> )
(M) make_matrix_from_rows( 
	SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const&,
	SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const&,
	SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const&,
	SmallVector< smat_cols_t<M>, smat_type_t<M>, tSpec > const&
) noexcept(M::staticSize);

//TODO: check spaces.
template< class M, class tSpec > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,0,2> )
(M) make_matrix_from_cols( 
	SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const&,
	SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const&
) noexcept(M::staticSize);
template< class M, class tSpec > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,0,3> )
(M) make_matrix_from_cols( 
	SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const&,
	SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const&,
	SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const&
) noexcept(M::staticSize);
template< class M, class tSpec > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M,0,4> )
(M) make_matrix_from_cols( 
	SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const&,
	SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const&,
	SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const&,
	SmallVector< smat_rows_t<M>, smat_type_t<M>, tSpec > const&
) noexcept(M::staticSize);

#if !FLUX_FML_MSVC_ICE_WORKAROUND
template< class M >
FLUX_ENABLE_IF( is_small_matrix<M> )
(M) make_matrix( 
	ftl::not_null<smat_type_t<M> const*>,
	smat_rows_t<M> = smat_rows_t<M>{},
	smat_cols_t<M> = smat_cols_t<M>{}
) noexcept(M::staticSize);

template< class M > constexpr
FLUX_ENABLE_IF( is_small_matrix<M> )
(M) make_zero( Identity<M> = {}, smat_rows_t<M> = smat_rows_t<M>{}, smat_cols_t<M> = smat_cols_t<M>{} ) noexcept(M::staticSize);
template< class M > constexpr
FLUX_ENABLE_IF( is_small_matrix<M> )
(M) make_one( Identity<M> = {}, smat_rows_t<M> = smat_rows_t<M>{}, smat_cols_t<M> = smat_cols_t<M>{} ) noexcept(M::staticSize);
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M> )
(M) make_identity( Identity<M> = {}, smat_rows_t<M> = smat_rows_t<M>{}, smat_cols_t<M> = smat_cols_t<M>{} ) noexcept(M::staticSize);
template< class M > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_matrix<M> )
(M) make_splat( smat_type_t<M>, Identity<M> = {}, smat_rows_t<M> = smat_rows_t<M>{}, smat_cols_t<M> = smat_cols_t<M>{} ) noexcept(M::staticSize);
#endif // ~ MSVC_ICE_WORKAROUND
//@}

	// conversions & accessors
/** \name Matrix conversion and accessors
 * \brief Matrix conversion and accessors
 *
 * Functions to convert between different matrices, and to access subsets of
 * matrices.
 *
 * A `SmallMatrix` may be converted into a `SmallMatrix` of a different size
 * with `expand()` and `shrink()`. Other properties of the `SmallMatrix` must
 * remain the same, i.e., `expand()` cannot be used to convert between types,
 * or from one aspect/space to another.
 *
 * A `SmallMatrix` may be converted to a `SmallMatrix§` with a different element
 * type with `narrow()`. This method will ensure that the conversion is
 * performed without losses.
 *
 * `matrix_cast<>` can be used to convert between `SmallMatrix`s with different
 * element types, with different aspects and from different spaces.
 * `matrix_cast<>` may not be used to change the size of a `SmallMatrix`.
 * `matrix_cast<>` performs no additional checks otherwise.
 *
 * `data_ptr` retrieves a pointer to the `SmallMatrix`'s data. It is equivalent
 * to calling the `SmallMatrix`'s `.data()` method.
 *
 * //TODO : get_col/get_row, and later set_col,set_row
 */
//@{
template< class M, class tMSize, class tNSize, typename T, class tSA > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && detail::SMatCompatResize<M, SmallMatrix<tMSize,tNSize,T,tSA>>::value )
(M) expand( 
	SmallMatrix<tMSize,tNSize,T,tSA> const&, 
	M const& = M(Init::identity) 
) noexcept(M::staticSize);

#if !FLUX_FML_MSVC_ICE_WORKAROUND
template< class M, class tMSize, class tNSize, typename T, class tSA > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && detail::SMatCompatResize<M, SmallMatrix<tMSize,tNSize,T,tSA>>::value )
(M) shrink( 
	SmallMatrix<tMSize,tNSize,T,tSA> const&, 
	smat_rows_t<M> = smat_rows_t<M>(), 
	smat_cols_t<M> = smat_cols_t<M>() 
) noexcept(M::staticSize);
#endif // ~ MSVC_ICE_WORKAROUND

template< class M, class tMSize, class tNSize, typename T, class tSA > /*FLUX_CONSTEXPR_EX*/
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && detail::SMatCompatNarrow<M,SmallMatrix<tMSize,tNSize,T,tSA>>::value )
(M) narrow( SmallMatrix<tMSize,tNSize,T,tSA> const& );

template< class M, class tMSize, class tNSize, typename T, class tSA > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_matrix<M>::value && detail::SMatCheckSize<M,SmallMatrix<tMSize,tNSize,T,tSA>>::value )
(M) matrix_cast( SmallMatrix<tMSize,tNSize,T,tSA> const& ) noexcept(M::staticSize);


template< class tMSize, class tNSize, typename T, class tSA, class tVec = svec_rebind_space_t<SmallVector<tNSize,T>,typename tSA::RowSpace> > FLUX_CONSTEXPR_EX
tVec get_row( SmallMatrix<tMSize,tNSize,T,tSA> const&, std::size_t ) noexcept(ftl::is_static_value<tNSize>::value);
template< class tMSize, class tNSize, typename T, class tSA, class tVec = svec_rebind_space_t<SmallVector<tMSize,T>,typename tSA::ColSpace> > FLUX_CONSTEXPR_EX
tVec get_col( SmallMatrix<tMSize,tNSize,T,tSA> const&, std::size_t ) noexcept(ftl::is_static_value<tMSize>::value);

//TODO: set_row()
//TODO: set_col()

	// helpers
template< class tMSize, class tNSize, typename T, class tSA > FLUX_CONSTEXPR_EX
T* data_ptr( SmallMatrix<tMSize,tNSize,T,tSA>& ) noexcept;
template< class tMSize, class tNSize, typename T, class tSA > FLUX_CONSTEXPR_EX
T const* data_ptr( SmallMatrix<tMSize,tNSize,T,tSA> const& ) noexcept;
//@}

	// simple transformations
/** \name Matrix functions
 * \brief Matrix functions
 *
 * General mathematical matrix functions, including matrix transpose,
 * determinant, trace and inverse.
 *
 * \note Some of these methods have been specialized for common matrix sizes,
 * such as \f$2\times 2\f$, \f$3\times 3\f$ and \f$4\times 4\f$.
 *
 * \see gauss()
 */
//@{
template< class tMSize, class tNSize, typename T, class tSA, class tNA = aspect::MatrixSpec<typename tSA::RowSpace,typename tSA::ColSpace> > FLUX_CONSTEXPR_EX
SmallMatrix<tNSize,tMSize,T,tNA> transpose( SmallMatrix<tMSize,tNSize,T,tSA> const& ) noexcept(ftl::is_static_value<tMSize>::value && ftl::is_static_value<tNSize>::value);

template< class tNSize, typename T, class tSA > FLUX_CONSTEXPR_EX
T det( SmallMatrix<tNSize,tNSize,T,tSA> const& ) noexcept;

template< class tNSize, typename T, class tSA > FLUX_CONSTEXPR_EX
T trace( SmallMatrix<tNSize,tNSize,T,tSA> const& ) noexcept;

template< class tNSize, typename T, class tSA, class tNA = aspect::MatrixSpec<typename tSA::RowSpace, typename tSA::ColSpace> > FLUX_CONSTEXPR_EX
SmallMatrix<tNSize,tNSize,T,tNA> invert( SmallMatrix<tNSize,tNSize,T,tSA> const& ) noexcept(ftl::is_static_value<tNSize>::value);
//@}

/** \name Linear equation system solvers
 * \brief Solve linear equation systems
 *
 * Solve the linear equation system \f$\mbox{aLHS}\,\mathbf{x} = \mbox{aRHS}\f$,
 * where \f$\mbox{aLHS}\f$ is a square matrix, and \f$\mbox{aRHS}\f$ is either
 * a single (column) vector, or a matrix containing multiple column vectors.
 * The methods return \f$\mathbf{x}\f$, which is either a vector or matrix,
 * depending on the right-hand side type.
 *
 * Currently, only gaussian elimination is implemented. For serious use other
 * methods should be considered. The matrix-overload of `gauss()` can be used
 * to invert matrices, but in general, the `invert()` method should be
 * preferred, as it provides special implementations for common matrix sizes,
 * including \f$2\times 2\f$, \f$3\times 3\f$ and \f$4\times 4\f$.
 *
 * TODO- LU decomposition as a alternative (better) method
 */
//@{
/** \brief Solve linear equation system
 *
 * Solve the linear equation system \f$\mbox{aLHS}\,\mathbf{x} =
 * \mbox{aRHS}\f$, where \a aLHS is a square matrix, and \a aRHS is a vector
 * with matching length. Returns the vector \f$\mathbf{x}\f$.
 */
template< class tNSize, typename T, class tSA, typename Y, class tSB, class tNA = aspect::VectorSpec<typename tSB::Aspect, typename tSA::RowSpace> > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( space_compatible<typename tSA::ColSpace,typename tSB::Space> )
(SmallVector<tNSize,Y,tNA>) gauss( SmallMatrix<tNSize,tNSize,T,tSA> aLHS, SmallVector<tNSize,Y,tSB> const& aRHS ) noexcept(ftl::is_static_value<tNSize>::value);
/** \brief Solve linear equation systems
 *
 * Solve the linear equation system \f$\mbox{aLHS}\,\mathbf{x}_i = \mathbf{b}_i
 * \f$, where \a aLHS is a square matrix, and \a aRHS is a matrix composed of
 * the vectors \f$\mathbf{b}_i\f$ in its columns. Returns the matrix
 * \f$\mathbf{x}_i\f$.
 */
template< class tNSize, typename T, class tSA, class tMSize = tNSize, typename Y = T, class tSB = aspect::MatrixSpec<typename tSA::RowSpace, typename tSA::ColSpace> > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( space_compatible<typename tSA::ColSpace, typename tSB::RowSpace> /*TODO- the other spaces should probably also match...*/ )
(SmallMatrix<tNSize,tMSize,Y,tSB>) gauss( SmallMatrix<tNSize,tNSize,T,tSA> aLHS, SmallMatrix<tNSize,tMSize,Y,tSB> = SmallMatrix<tNSize,tMSize,Y,tSB>(Init::identity) ) noexcept;
//@}

	// Element wise
/** \name Element-wise matrix functions
 * \brief Element-wise matrix functions
 */
//@{
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) FLUX_CONSTEXPR_EX
T max( FLUXTMP_FMLOP_CA_ const& ) noexcept;
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) FLUX_CONSTEXPR_EX
T min( FLUXTMP_FMLOP_CA_ const& ) noexcept;

FLUXTMP_FMLOP_TEMPL2_(op::ClassNorm) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ max( FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const& ) FLUXTMP_FMLOP_NOEX_;
FLUXTMP_FMLOP_TEMPL2_(op::ClassNorm) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ min( FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const& ) FLUXTMP_FMLOP_NOEX_;

FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ abs( FLUXTMP_FMLOP_CA_ const& ) FLUXTMP_FMLOP_NOEX_;
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ sign( FLUXTMP_FMLOP_CA_ const& ) FLUXTMP_FMLOP_NOEX_;

FLUXTMP_FMLOP_TEMPL2_(op::ClassNorm) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ mix( T, FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const& ) FLUXTMP_FMLOP_NOEX_;
FLUXTMP_FMLOP_TEMPL2_(op::ClassNorm) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ lerp( T, FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const& ) FLUXTMP_FMLOP_NOEX_;
//@}

#undef FLUXTMP_FMLOP_NOEX_
#undef FLUXTMP_FMLOP_RS_
#undef FLUXTMP_FMLOP_CB_
#undef FLUXTMP_FMLOP_CA_
#undef FLUXTMP_FMLOP_TEMPL2_
#undef FLUXTMP_FMLOP_TEMPL1_

//--    meta: bitwise traits            ///{{{2///////////////////////////////

/** \addtogroup FTLBitwiseTags
 * @{
 */
template< class tMSize, class tNSize, typename tType, class tSpec >
auto tag_is_bitwise_conjurable( SmallMatrix<tMSize,tNSize,tType,tSpec> const& )
	-> std::true_type { return {}; }
template< class tMSize, class tNSize, typename tType, class tSpec >
auto tag_is_bitwise_materializable( SmallMatrix<tMSize,tNSize,tType,tSpec> const& )
	-> ftl::is_static_value<detail::SMatElementCount<tMSize,tNSize>> { return {}; }
template< class tMSize, class tNSize, typename tType, class tSpec >
auto tag_is_bitwise_duplicable( SmallMatrix<tMSize,tNSize,tType,tSpec> const& )
	-> ftl::is_static_value<detail::SMatElementCount<tMSize,tNSize>> { return {}; }
template< class tMSize, class tNSize, typename tType, class tSpec >
auto tag_is_bitwise_relocatable( SmallMatrix<tMSize,tNSize,tType,tSpec> const& )
	-> std::true_type { return {}; }
template< class tMSize, class tNSize, typename tType, class tSpec >
auto tag_is_bitwise_dismissible( SmallMatrix<tMSize,tNSize,tType,tSpec> const& )
	-> ftl::is_static_value<detail::SMatElementCount<tMSize,tNSize>> { return {}; }
/** @} */

//--    <<< ~ flux::fml namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/small_matrix.inl"
#endif // FLUX_FML_SMALL_MATRIX_HPP_74D03CF2_8E2F_4A43_A0CF_CE5C2BBD307D
