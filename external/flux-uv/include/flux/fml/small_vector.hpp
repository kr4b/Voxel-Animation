/*-******************************************************* -- HEADER -{{{1- */
/*- Fixed size small vector
 * 
 * Small size, much over engineering.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FML_SMALL_VECTOR_HPP_1EB719FB_2F5D_4AC5_8CF6_BFCF4A99247A
#define FLUX_FML_SMALL_VECTOR_HPP_1EB719FB_2F5D_4AC5_8CF6_BFCF4A99247A

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

#include <cstddef>
#include <type_traits>

//--    >>> namespace = flux::fml >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    detail                          ///{{{2///////////////////////////////
namespace detail
{
	template< typename tType > struct Init2 { tType x, y; };
	template< typename tType > struct Init3 { tType x, y, z; };
	template< typename tType > struct Init4 { tType x, y, z, w; };
}

//--    aspect                          ///{{{2///////////////////////////////
namespace aspect
{
	/** Aspect: `SmallVector` models a mathematical vector
	 * 
	 * A `SmallVector` with this aspect in its `VectorSpec` models a
	 * mathematical vector. This is the default.
	 */
	struct VectorAspect;
	/** Aspect: `SmallVector` modesl a position
	 *
	 * A `SmallVector` with this aspect in its `VectorSpec` models a position.
	 * Positions only support a subset of the vector operations. For example,
	 * two positions may be subtracted (*) to yield a vector, a vector may be
	 * added/subtracted from a position to yield another position.
	 *
	 * (*) Two positions can also be added, as this maintains the symmetry 
	 * between addition and subtraction.
	 *
	 * As a special case, unary plus and minus can be understood as adding and
	 * subtracting, respectively, a position from the origin. Unary plus can
	 * thus be used as a short-hand for converting a position to the
	 * corresponding vector.
	 */
	struct PositionAspect;
	
	/** Vector specification
	 *
	 * Vector meta data. The meta data contains information about the vector's
	 * aspect (mathematical vector or position), and about the space that the
	 * vector is defined in.
	 *
	 * \see VectorAspect, PositionAspect, DefaultSpace
	 */
	template< class tAspect = VectorAspect, class tSpace = space::AnySpace >
	struct VectorSpec
	{
		using Aspect = tAspect;
		using Space = tSpace;
	};
};

//--    detail                          ///{{{2///////////////////////////////
namespace detail
{
	template< class, class, class = compat::void_t<> > struct SVecCompat;
}

//--    SmallVector                     ///{{{2///////////////////////////////

/** Mathematical Vector
 *
 * `SmallVector` is a mathematical \f$N\f$-vector or position, where \f$N\f$ is
 * given by \a tSize. Each component is represented by type \a T. \a tSpec
 * further contains meta-data about the vector object, such as whether it
 * represents a mathematical vector or a position, and what space the vector is
 * defined in.
 *
 * \a tSize may either be a compile-time constant (`ftl::StaticSize<>`), or a
 * runtime value (`ftl::DynamicSize`). `SmallVector` is further specialized for
 * sizes 2, 3, and 4 (i.e., when \a tSize is `StaticSize<2>`, `StaticSize<3>',
 * or `StaticSize<4>`). In this case, the elements of the vector can be
 * accessed using the `.x`, `.y`, `.z`, and `.w` members (where applicable).
 * The elements may always be accesses using `operator[]` or via the pointer
 * returned by `.data()`.
 *
 * A `SmallVector` with a static size is guaranteed to be a standard
 * layout/trivial type.
 *
 * \see SmallPosition
 */
template< class tSize, typename T = float, class tSpec = aspect::VectorSpec<> >
struct SmallVector final
{
	using value_type = T;
	static constexpr bool staticSize = ftl::is_static_value<tSize>::value;

	ftl::DSStore<tSize,T> elem;

	SmallVector() = default;

	SmallVector( SmallVector const& ) = default;
	SmallVector& operator=( SmallVector const& ) = default;

	SmallVector( SmallVector&& ) = default;
	SmallVector& operator=( SmallVector&& ) = default;


	FLUX_CONSTEXPR_EX SmallVector( tSize ) noexcept(staticSize);
	FLUX_CONSTEXPR_EX explicit
	SmallVector( Init::Zero, tSize = tSize() ) noexcept(staticSize);
	FLUX_CONSTEXPR_EX 
	SmallVector( Init::Fill, T, tSize = tSize() ) noexcept(staticSize);
	FLUX_CONSTEXPR_EX 
	SmallVector( Init::Identity, tSize = tSize() ) noexcept(staticSize);

	SmallVector( ftl::ExplicitInitList<T> ) noexcept(staticSize);


	template< class tOtherVec, 
		class = FLUX_ENABLE_IF(detail::SVecCompat<SmallVector,tOtherVec>)(int)
	> FLUX_CONSTEXPR_EX
	SmallVector( tOtherVec const& ) noexcept(staticSize);
	template< class tOtherVec, 
		class = FLUX_ENABLE_IF(detail::SVecCompat<SmallVector,tOtherVec>)(int)
	> FLUX_CONSTEXPR_EX
	SmallVector& operator=( tOtherVec const& ) noexcept(staticSize);


	FLUX_CONSTEXPR_EX T& operator[] (std::size_t) noexcept;
	constexpr T const& operator[] (std::size_t) const noexcept;

	constexpr tSize size() const noexcept;

	FLUX_CONSTEXPR_EX T* data() noexcept;
	constexpr T const* data() const noexcept;

};

// Specializations for 2, 3 & 4 elements
template< typename T, class tSpec >
struct SmallVector< ftl::StaticSize<2>, T, tSpec > final
{
	using value_type = T;

	union
	{
		struct { T x, y; };
		T elem[2];
	};

	SmallVector() = default;

	SmallVector( SmallVector const& ) = default;
	SmallVector& operator=( SmallVector const& ) = default;

	SmallVector( SmallVector&& ) = default;
	SmallVector& operator=( SmallVector&& ) = default;


	FLUX_CONSTEXPR_EX explicit
	SmallVector( ftl::StaticSize<2> ) noexcept;
	
	FLUX_CONSTEXPR_EX explicit
	SmallVector( Init::Zero, ftl::StaticSize<2> = {} ) noexcept;
	FLUX_CONSTEXPR_EX 
	SmallVector( Init::Fill, T, ftl::StaticSize<2> = {} ) noexcept;
	FLUX_CONSTEXPR_EX 
	SmallVector( Init::Identity, ftl::StaticSize<2> = {} ) noexcept;

	FLUX_CONSTEXPR_EX SmallVector( T, T ) noexcept;
	FLUX_CONSTEXPR_EX SmallVector( detail::Init2<T> ) noexcept;

	template< class tOtherVec, 
		class = FLUX_ENABLE_IF(detail::SVecCompat<SmallVector,tOtherVec>)(int)
	> FLUX_CONSTEXPR_EX
	SmallVector( tOtherVec const& ) noexcept;
	template< class tOtherVec, 
		class = FLUX_ENABLE_IF(detail::SVecCompat<SmallVector,tOtherVec>)(int)
	> FLUX_CONSTEXPR_EX
	SmallVector& operator=( tOtherVec const& ) noexcept;


	FLUX_CONSTEXPR_EX T& operator[] (std::size_t) noexcept;
	constexpr T const& operator[] (std::size_t) const noexcept;

	constexpr ftl::StaticSize<2> size() const noexcept;

	FLUX_CONSTEXPR_EX T* data() noexcept;
	constexpr T const* data() const noexcept;

	static constexpr bool staticSize = true;
};
template< typename T, class tSpec >
struct SmallVector< ftl::StaticSize<3>, T, tSpec > final
{
	using value_type = T;

	union
	{
		struct { T x, y, z; };
		T elem[3];
	};

	SmallVector() = default;

	SmallVector( SmallVector const& ) = default;
	SmallVector& operator=( SmallVector const& ) = default;

	SmallVector( SmallVector&& ) = default;
	SmallVector& operator=( SmallVector&& ) = default;


	FLUX_CONSTEXPR_EX explicit
	SmallVector( ftl::StaticSize<3> ) noexcept;
	
	FLUX_CONSTEXPR_EX explicit
	SmallVector( Init::Zero, ftl::StaticSize<3> = {} ) noexcept;
	FLUX_CONSTEXPR_EX 
	SmallVector( Init::Fill, T, ftl::StaticSize<3> = {} ) noexcept;
	FLUX_CONSTEXPR_EX 
	SmallVector( Init::Identity, ftl::StaticSize<3> = {} ) noexcept;

	FLUX_CONSTEXPR_EX SmallVector( T, T, T ) noexcept;
	FLUX_CONSTEXPR_EX SmallVector( detail::Init3<T> ) noexcept;

	template< class tOtherVec, 
		class = FLUX_ENABLE_IF(detail::SVecCompat<SmallVector,tOtherVec>)(int)
	> FLUX_CONSTEXPR_EX
	SmallVector( tOtherVec const& ) noexcept;
	template< class tOtherVec, 
		class = FLUX_ENABLE_IF(detail::SVecCompat<SmallVector,tOtherVec>)(int)
	> FLUX_CONSTEXPR_EX
	SmallVector& operator=( tOtherVec const& ) noexcept;


	FLUX_CONSTEXPR_EX T& operator[] (std::size_t) noexcept;
	constexpr T const& operator[] (std::size_t) const noexcept;

	constexpr ftl::StaticSize<3> size() const noexcept;

	FLUX_CONSTEXPR_EX T* data() noexcept;
	constexpr T const* data() const noexcept;

	static constexpr bool staticSize = true;
};
template< typename T, class tSpec >
struct SmallVector< ftl::StaticSize<4>, T, tSpec > final
{
	using value_type = T;

	union
	{
		struct { T x, y, z, w; };
		T elem[4];
	};

	SmallVector() = default;

	SmallVector( SmallVector const& ) = default;
	SmallVector& operator=( SmallVector const& ) = default;

	SmallVector( SmallVector&& ) = default;
	SmallVector& operator=( SmallVector&& ) = default;


	FLUX_CONSTEXPR_EX explicit
	SmallVector( ftl::StaticSize<4> ) noexcept;
	
	FLUX_CONSTEXPR_EX explicit
	SmallVector( Init::Zero, ftl::StaticSize<4> = {} ) noexcept;
	FLUX_CONSTEXPR_EX 
	SmallVector( Init::Fill, T, ftl::StaticSize<4> = {} ) noexcept;
	FLUX_CONSTEXPR_EX 
	SmallVector( Init::Identity, ftl::StaticSize<4> = {} ) noexcept;

	FLUX_CONSTEXPR_EX SmallVector( T, T, T, T ) noexcept;
	FLUX_CONSTEXPR_EX SmallVector( detail::Init4<T> ) noexcept;

	template< class tOtherVec, 
		class = FLUX_ENABLE_IF(detail::SVecCompat<SmallVector,tOtherVec>)(int)
	> FLUX_CONSTEXPR_EX
	SmallVector( tOtherVec const& ) noexcept;
	template< class tOtherVec, 
		class = FLUX_ENABLE_IF(detail::SVecCompat<SmallVector,tOtherVec>)(int)
	> FLUX_CONSTEXPR_EX
	SmallVector& operator=( tOtherVec const& ) noexcept;


	FLUX_CONSTEXPR_EX T& operator[] (std::size_t) noexcept;
	constexpr T const& operator[] (std::size_t) const noexcept;

	constexpr ftl::StaticSize<4> size() const noexcept;

	FLUX_CONSTEXPR_EX T* data() noexcept;
	constexpr T const* data() const noexcept;

	static constexpr bool staticSize = true;
};

//--    Aliases                         ///{{{2///////////////////////////////

/** Alias: `SmallVector<>` modeling a position
 *
 * `SmallPosition<>` is a `SmallVector` with the `aspect::PositionAspect`.  As
 * such, it models a position rather than a vector. Positions do not support
 * the full range of vector operations, and may only be combined with positions
 * and vectors in specific ways (which introduces another layer of type safety,
 * or another layer of programmer annoyance, a bit depending on the end user's
 * opinion on hand-holding.)
 *
 * \see aspect::PositionAspect
 */
template< 
	class tSize, 
	typename T, 
	class tSpec = aspect::VectorSpec<aspect::PositionAspect> 
>
using SmallPosition = SmallVector<tSize,T,tSpec>;

//--    meta functions                  ///{{{2///////////////////////////////

/** \anchor VecTraits \name Meta: Vector traits
 * \brief Vector traits and meta-functions
 *
 * Meta functions to identify and manipulate the vector types.
 */
//@{

/** Meta function: Check if type is a `SmallVector`
 *
 * Check if the provided type is a `SmallVector` of the specified size, or any
 * `SmallVector` if the size is set to zero (the default).
 *
 * \warning Dynamically sized vectors will never be considered to be a
 * `SmallVector` of a specific size. So `is_small_vector<SVecWithDynamicSize>`
 * will be true, but `is_small_vector<SVecProbablySameSize,N>` will be false
 * for any non-zero `N`.
 */
template< class, std::size_t > struct is_small_vector : std::false_type {};


namespace detail
{
	template< class > struct SVecType;
	template< class > struct SVecSize;
	template< class > struct SVecSpec;
	template< class > struct SVecSpace;
	template< class > struct SVecAspect;

	template< class, class > struct SVecRebind;
}

template< class V > using svec_type_t = typename detail::SVecType<V>::type;
template< class V > using svec_size_t = typename detail::SVecSize<V>::type;
template< class V > using svec_spec_t = typename detail::SVecSpec<V>::type;
template< class V > using svec_space_t = typename detail::SVecSpace<V>::type;
template< class V > using svec_aspect_t = typename detail::SVecAspect<V>::type;

template< class, class > struct svec_is_aspect;

template< class V, class tSpace >
using svec_rebind_space_t = typename detail::SVecRebind<V,tSpace>::type;

namespace detail
{
	template< class, class, class = compat::void_t<> > struct SVecUOpSpec;
	template< class, class, class, class = compat::void_t<> > struct SVecBOpSpec;

	template< class, class > struct SVecSizeCompat;

	template< class, class, class = compat::void_t<> > struct SVecCompatResize;
	template< class, class, class = compat::void_t<> > struct SVecCompatNarrow;
}
//@}

//--    operators                       ///{{{2///////////////////////////////

#define FLUXTMP_FMLOP_TEMPL1_(op) template< class tSize, typename T, class tSA, typename tNA = typename detail::SVecUOpSpec<op,tSA>::type  >
#define FLUXTMP_FMLOP_TEMPL2_(op) template< class tSize, typename T, class tSA, class tSB, typename tNA = typename detail::SVecBOpSpec<op,tSA,tSB>::type  >
#define FLUXTMP_FMLOP_CA_ SmallVector<tSize,T,tSA>
#define FLUXTMP_FMLOP_CB_ SmallVector<tSize,T,tSB>
#define FLUXTMP_FMLOP_RS_ SmallVector<tSize,T,tNA>
#define FLUXTMP_FMLOP_NOEX_ noexcept((SmallVector<tSize,T,tNA>::staticSize))

	// unary
/** \name Unary vector operators
 *  \brief Unary vector operators
 *
 * Unary plus and minus are defined for mathematical vectors
 * (`aspect::VectorAspect`) and positions (`aspect::PositionAspect`).
 */
//@{
FLUXTMP_FMLOP_TEMPL1_(op::ClassUnaryPM) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator- (FLUXTMP_FMLOP_CA_ const&) FLUXTMP_FMLOP_NOEX_;

FLUXTMP_FMLOP_TEMPL1_(op::ClassUnaryPM) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator+ (FLUXTMP_FMLOP_CA_ const&) FLUXTMP_FMLOP_NOEX_;
//@}

	// binary: comparison
/** \name Vector comparison operators
 *  \brief Vector comparison operators
 *
 * Mathematical vectors (`aspect::VectorAspect`) and positions
 * (`aspect::PositionAspect`) can both be compared for equality and inequality.
 */
//@{
FLUXTMP_FMLOP_TEMPL2_(op::ClassCmp) FLUX_CONSTEXPR_EX
bool operator== (FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) noexcept;
FLUXTMP_FMLOP_TEMPL2_(op::ClassCmp) FLUX_CONSTEXPR_EX
bool operator!= (FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) noexcept;
//@}

	// binary: arithmetic
/** \name Vector arithmetic operators
 *  \brief Vector arithmetic operators
 *
 * Addition and subtraction are defined for both mathematical vectors
 * (`aspect::VectorAspect`) and positions (`aspect::PositionAspect`).
 * Multiplication and division are defined for mathematical vectors only.
 *
 * Addition/subtractions of two positions yields a vector. A vector and a
 * position may be added/subtracted to yield a position.
 *
 * Multiplication/division of a vector with a vector performs element-wise
 * multiplication and division, respectively.
 */
//@{
FLUXTMP_FMLOP_TEMPL2_(op::ClassAdd) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator+ (FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) FLUXTMP_FMLOP_NOEX_;
FLUXTMP_FMLOP_TEMPL2_(op::ClassAdd) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator- (FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) FLUXTMP_FMLOP_NOEX_;

FLUXTMP_FMLOP_TEMPL2_(op::ClassMul) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator* (FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) FLUXTMP_FMLOP_NOEX_;
FLUXTMP_FMLOP_TEMPL2_(op::ClassMul) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator/ (FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) FLUXTMP_FMLOP_NOEX_;

FLUXTMP_FMLOP_TEMPL1_(op::ClassMul) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator* (FLUXTMP_FMLOP_CA_ const&, T) FLUXTMP_FMLOP_NOEX_;
FLUXTMP_FMLOP_TEMPL1_(op::ClassMul) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator/ (FLUXTMP_FMLOP_CA_ const&, T) FLUXTMP_FMLOP_NOEX_;

FLUXTMP_FMLOP_TEMPL1_(op::ClassMul) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ operator* (T, FLUXTMP_FMLOP_CA_ const&) FLUXTMP_FMLOP_NOEX_;
//@}

	// binary: assignment
/** \name Vector self-assigning arithmetic
 *  \brief Vector self-assigning arithmetic
 *
 * Incrementing/decrementing (`+=`,`-=`) is defined between mathematical
 * vectors (`aspect::VectorAspect`), and for positions
 * (`aspect::PositionAspect`) with a mathematical vector.
 *
 * Scaling (`*=`,`/=`) is defined only for mathematical vectors. Scaling a
 * vector by a vector performs the operation element-wise.
 */
//@{
FLUXTMP_FMLOP_TEMPL2_(op::ClassInc) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator+= (FLUXTMP_FMLOP_CA_&, FLUXTMP_FMLOP_CB_ const&) noexcept;
FLUXTMP_FMLOP_TEMPL2_(op::ClassInc) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator-= (FLUXTMP_FMLOP_CA_&, FLUXTMP_FMLOP_CB_ const&) noexcept;

FLUXTMP_FMLOP_TEMPL2_(op::ClassScale) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator*= (FLUXTMP_FMLOP_CA_&, FLUXTMP_FMLOP_CB_ const&) noexcept;
FLUXTMP_FMLOP_TEMPL2_(op::ClassScale) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator/= (FLUXTMP_FMLOP_CA_&, FLUXTMP_FMLOP_CB_ const&) noexcept;

FLUXTMP_FMLOP_TEMPL1_(op::ClassScale) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator*= (FLUXTMP_FMLOP_CA_&, T) noexcept;
FLUXTMP_FMLOP_TEMPL1_(op::ClassScale) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_CA_& operator/= (FLUXTMP_FMLOP_CA_&, T) noexcept;
//@}

//--    functions                       ///{{{2///////////////////////////////

	// pseudo constructors
/** \name Vector pseudo-constructors
 *  \brief Vector pseudo-constructors
 *
 * Create `SmallVector`s of type \a V as follows:
 *
 *  - `make_vector`: from the provided values(*)
 *  - `make_zero`: `SmallVector` with all elements equal to zero
 *  - `make_one`: `SmallVector` with all elements equal to one
 *  - `make_iota`: `SmallVector` with all elements equal to their index
 *  - `make_identity`: `SmallVector` with all elements equal to zero, except
 *     for 4-wide vectors, where the fourth component will be equal to one.
 *     (TODO- deprecate).
 *  - `make_splat`: `SmallVector` with all elements equal to the provided
 *     value.
 *
 * Examples:
   \code
   // For statically sized vectos.
   auto const v = make_vector<vec3f>( 1.f, 2.f, 3.f );
   auto const z = make_zero<vec3f>(); // alt: make_zero( flux::Identity<vec3f>{} );
   auto const s = make_splat<vec3f>( 3.f ); // == {3.f, 3.f, 3.f}
   
   // Dynamically sized:
   auto const d = make_zero( Identity<DynamicVector>{}, 123 ); // 123-element vector
   \endcode
 *
 * (*) `make_vector` with two arguments is valid only for a `SmallVector` with
 * a static size of two. Similarly, the three argument version can only be used
 * to create vectors of size three; the four argument version can only be used
 * to create vectors of size four. A generic `make_vector` takes a size and a
 * pointer -- this version can be used on all vectors (but it requires that the
 * provided size matches with the size of static vectors).
 *
 * The additional size-argument in e.g. `make_zero()` is required for
 * dynamically sized vectors. It may be omitted for statically sized ones.
 */
//@{
template< class V > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V,2> )
(V) make_vector( 
	svec_type_t<V>,
	svec_type_t<V>
) noexcept(V::staticSize);

template< class V > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V,3> )
(V) make_vector( 
	svec_type_t<V>, 
	svec_type_t<V>, 
	svec_type_t<V> 
) noexcept(V::staticSize);

template< class V > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V,4> )
(V) make_vector( 
	svec_type_t<V>, 
	svec_type_t<V>,
	svec_type_t<V>, 
	svec_type_t<V> 
) noexcept(V::staticSize);

template< class V > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V> )
(V) make_vector( 
	ftl::not_null<svec_type_t<V> const*>, 
	svec_size_t<V> = svec_size_t<V>{} 
) noexcept(V::staticSize);

#if !FLUX_FML_MSVC_ICE_WORKAROUND
template< class V > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V> )
(V) make_zero( Identity<V> = {}, svec_size_t<V> = svec_size_t<V>{} ) noexcept(V::staticSize);
template< class V > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V> )
(V) make_one( Identity<V> = {}, svec_size_t<V> = svec_size_t<V>{} ) noexcept(V::staticSize);
template< class V > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V> )
(V) make_iota( Identity<V> = {}, svec_size_t<V> = svec_size_t<V>{} ) noexcept(V::staticSize);
template< class V > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V> )
(V) make_identity( Identity<V> = {}, svec_size_t<V> = svec_size_t<V>{} ) noexcept(V::staticSize);
template< class V > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( is_small_vector<V> )
(V) make_splat( typename V::value_type, Identity<V> = {}, svec_size_t<V> = svec_size_t<V>{} ) noexcept(V::staticSize);
#endif // ~ MSVC_ICE_WORKAROUND
//@}

	// conversions
/** \name Vector conversion functions
 *  \brief Vector conversion functions
 *
 * A `SmallVector` may be converted into a `SmallVector` of a different size
 * with `expand()` and `shrink()`. Other properties of the `SmallVector` must
 * remain the same, i.e., `expand()` cannot be used to convert between types,
 * or from one aspect/space to another.
 *
 * A `SmallVector` may be converted to a `SmallVector` with a different element
 * type with `narrow()`. This method will ensure that the conversion is
 * performed without losses.
 *
 * `vector_cast<>` can be used to convert between `SmallVector`s with different
 * element types, with different aspects and from different spaces.
 * `vector_cast<>` may not be used to change the size of a `SmallVector`.
 * `vector_cast<>` performs no additional checks otherwise.
 *
 * `data_ptr` retrieves a pointer to the `SmallVector`'s data. It is equivalent
 * to calling the `SmallVector`'s `.data()` method.
 */
//@{
template< class V, class tSize, typename T, class tSA > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_vector<V>::value && detail::SVecCompatResize<V,SmallVector<tSize,T,tSA>>::value )
(V) expand( 
	SmallVector<tSize,T,tSA> const&, 
	V const& = V( Init::identity, svec_size_t<V>{} ) 
) noexcept(V::staticSize);

#if !FLUX_FML_MSVC_ICE_WORKAROUND
template< class V, class tSize, typename T, class tSA > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_vector<V>::value && detail::SVecCompatResize<V,SmallVector<tSize,T,tSA>>::value )
(V) shrink( 
	SmallVector<tSize,T,tSA> const&, 
	svec_size_t<V> = svec_size_t<V>() 
) noexcept(V::staticSize);
#endif // ~ MSVC_ICE_WORKAROUND

template< class V, class tSize, typename T, class tSA > /*FLUX_CONSTEXPR_EX*/
FLUX_ENABLE_IF_C( is_small_vector<V>::value && detail::SVecCompatNarrow<V,SmallVector<tSize,T,tSA>>::value )
(V) narrow( SmallVector<tSize,T,tSA> const& ) noexcept(V::staticSize);


template< class V, class tSize, typename T, class tSA > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF_C( is_small_vector<V>::value && detail::SVecSizeCompat<svec_size_t<V>,tSize>::value )
(V) vector_cast( SmallVector<tSize,T,tSA> const& ) noexcept(V::staticSize);

template< class tSize, typename T, class tSA > FLUX_CONSTEXPR_EX
T* data_ptr( SmallVector<tSize,T,tSA>& ) noexcept;
template< class tSize, typename T, class tSA > FLUX_CONSTEXPR_EX
T const* data_ptr( SmallVector<tSize,T,tSA> const& ) noexcept;
//@}

	// math: general
FLUXTMP_FMLOP_TEMPL2_(op::ClassDot) FLUX_CONSTEXPR_EX
T dot(FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) noexcept;

FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm)
T length(FLUXTMP_FMLOP_CA_ const&) noexcept;
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) FLUX_CONSTEXPR_EX
T length_squared(FLUXTMP_FMLOP_CA_ const&) noexcept;

FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) FLUX_CONSTEXPR_EX
T max(FLUXTMP_FMLOP_CA_ const&) noexcept;
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) FLUX_CONSTEXPR_EX
T min(FLUXTMP_FMLOP_CA_ const&) noexcept;

FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm) 
T norm_1(FLUXTMP_FMLOP_CA_ const&) noexcept;
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm)
T norm_2(FLUXTMP_FMLOP_CA_ const&) noexcept;
FLUXTMP_FMLOP_TEMPL1_(op::ClassNorm)
T norm_inf(FLUXTMP_FMLOP_CA_ const&) noexcept;

	// math: vectors
FLUXTMP_FMLOP_TEMPL2_(op::ClassBound) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ max(FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) FLUXTMP_FMLOP_NOEX_;
FLUXTMP_FMLOP_TEMPL2_(op::ClassBound) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ min(FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) FLUXTMP_FMLOP_NOEX_;

FLUXTMP_FMLOP_TEMPL1_(op::ClassBound) 
FLUXTMP_FMLOP_RS_ abs(FLUXTMP_FMLOP_CA_ const&) FLUXTMP_FMLOP_NOEX_;
FLUXTMP_FMLOP_TEMPL1_(op::ClassBound) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ sign(FLUXTMP_FMLOP_CA_ const&) FLUXTMP_FMLOP_NOEX_;

FLUXTMP_FMLOP_TEMPL1_(op::ClassDot) /* note: not ClassNorm! */
FLUXTMP_FMLOP_RS_ normalize(FLUXTMP_FMLOP_CA_ const&) FLUXTMP_FMLOP_NOEX_;

FLUXTMP_FMLOP_TEMPL2_(op::ClassLerp) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ mix(T, FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) FLUXTMP_FMLOP_NOEX_;
FLUXTMP_FMLOP_TEMPL2_(op::ClassLerp) FLUX_CONSTEXPR_EX
FLUXTMP_FMLOP_RS_ lerp(T, FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) FLUXTMP_FMLOP_NOEX_;

//TODO: project can be in different spaces?
FLUXTMP_FMLOP_TEMPL2_(op::ClassProj)
FLUXTMP_FMLOP_RS_ project(FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) FLUXTMP_FMLOP_NOEX_;
FLUXTMP_FMLOP_TEMPL2_(op::ClassProj)
FLUXTMP_FMLOP_RS_ reflect(FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) FLUXTMP_FMLOP_NOEX_;

	// math: special
FLUXTMP_FMLOP_TEMPL1_(op::ClassDot) FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( std::is_same<tSize,ftl::StaticSize<2>> )
(FLUXTMP_FMLOP_RS_) orthogonal(FLUXTMP_FMLOP_CA_ const&) FLUXTMP_FMLOP_NOEX_;

FLUXTMP_FMLOP_TEMPL1_(op::ClassDot) FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( std::is_same<tSize,ftl::StaticSize<3>> )
(FLUXTMP_FMLOP_RS_) orthogonal(FLUXTMP_FMLOP_CA_ const&) FLUXTMP_FMLOP_NOEX_;

FLUXTMP_FMLOP_TEMPL2_(op::ClassDot) FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( std::is_same<tSize,ftl::StaticSize<3>> )
(FLUXTMP_FMLOP_RS_) cross(FLUXTMP_FMLOP_CA_ const&, FLUXTMP_FMLOP_CB_ const&) FLUXTMP_FMLOP_NOEX_;

#undef FLUXTMP_FMLOP_NOEX_
#undef FLUXTMP_FMLOP_RS_
#undef FLUXTMP_FMLOP_CA_
#undef FLUXTMP_FMLOP_CB_
#undef FLUXTMP_FMLOP_TEMPL1_
#undef FLUXTMP_FMLOP_TEMPL2_

//--    meta: bitwise traits            ///{{{2///////////////////////////////

/** \addtogroup FTLBitwiseTags
 * @{
 */
template< class tSize, typename tType, class tSpec >
auto tag_is_bitwise_conjurable( SmallVector<tSize,tType,tSpec> const& )
	-> std::true_type { return {}; }
template< class tSize, typename tType, class tSpec >
auto tag_is_bitwise_materializable( SmallVector<tSize,tType,tSpec> const& )
	-> ftl::is_static_value<tSize> { return {}; }
template< class tSize, typename tType, class tSpec >
auto tag_is_bitwise_duplicable( SmallVector<tSize,tType,tSpec> const& )
	-> ftl::is_static_value<tSize> { return {}; }
template< class tSize, typename tType, class tSpec >
auto tag_is_bitwise_relocatable( SmallVector<tSize,tType,tSpec> const& )
	-> std::true_type { return {}; }
template< class tSize, typename tType, class tSpec >
auto tag_is_bitwise_dismissible( SmallVector<tSize,tType,tSpec> const& )
	-> ftl::is_static_value<tSize> { return {}; }
/** @} */

//--    <<< ~ flux::fml namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/small_vector.inl"
#endif // FLUX_FML_SMALL_VECTOR_HPP_1EB719FB_2F5D_4AC5_8CF6_BFCF4A99247A
