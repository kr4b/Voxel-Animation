/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Dual Quaternion implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>

#include <flux/compat/constexpr.hpp>

#include <cmath>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::fml >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	template< class, class, class > struct DQuatSpecCompat
		: std::false_type 
	{};
	template< class tSA, class tSB >
	struct DQuatSpecCompat< tSA, tSB, compat::void_t< 
			typename tSA::SrcSpace, typename tSA::DstSpace,
			typename tSB::SrcSpace, typename tSB::DstSpace
		>
	> : std::integral_constant< bool,
			   space_compatible<typename tSA::SrcSpace, typename tSB::SrcSpace>::value
			&& space_compatible<typename tSA::DstSpace, typename tSB::DstSpace>::value
		>
	{};
	
	template< typename T, class tSA >
	struct DQuatSrcSpace< DualQuaternion<T,tSA> > {
		using type = typename tSA::SrcSpace;
	};
	template< typename T, class tSA >
	struct DQuatDstSpace< DualQuaternion<T,tSA> > {
		using type = typename tSA::DstSpace;
	};

	template< typename T, class tSA, class tNewSrc >
	struct DQuatRebindSrc< DualQuaternion<T,tSA>, tNewSrc > {
		using type = DualQuaternion< T, aspect::DQuatSpec<typename tSA::DstSpace, tNewSrc> >;
	};
	template< typename T, class tSA, class tNewDst >
	struct DQuatRebindDst< DualQuaternion<T,tSA>, tNewDst > {
		using type = DualQuaternion< T, aspect::DQuatSpec<tNewDst, typename tSA::SrcSpace> >;
	};
	template< typename T, class tSA, class tNewDst, class tNewSrc >
	struct DQuatRebind< DualQuaternion<T,tSA>, tNewDst, tNewSrc > {
		using type = DualQuaternion< T, aspect::DQuatSpec<tNewDst, tNewSrc> >;
	};

	template< class tSA, class tSB >
	struct DQuatSpec
	{
		using type = aspect::DQuatSpec<
			space_specialized_t<typename tSA::DstSpace, typename tSB::DstSpace>,
			space_specialized_t<typename tSA::SrcSpace, typename tSB::SrcSpace>
		>;
	};


	template< bool, class, class > struct DQuatMulSpec0;
	template< class tDst, class tSrc >
	struct DQuatMulSpec0<true,tDst,tSrc> {
		using type = aspect::DQuatSpec< tDst, tSrc >;
	};

	template< class tSA >
	struct DQuatInvSpec
	{
		using type = aspect::DQuatSpec< typename tSA::SrcSpace, typename tSA::DstSpace >;
	};
	template< class tSA, class tSB >
	struct DQuatMulSpec
		: detail::DQuatMulSpec0< 
			space_compatible<typename tSA::SrcSpace, typename tSB::DstSpace>::value,
			typename tSA::DstSpace,
			typename tSB::SrcSpace
		>
	{};
}

//--    DualQuaternion                      ///{{{2///////////////////////////
template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>::DualQuaternion( Init::Zero ) noexcept
	: i(T(0)), j(T(0)), k(T(0)), r(T(0)), ei(T(0)), ej(T(0)), ek(T(0)), er(T(0))
{}
template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>::DualQuaternion( Init::Identity ) noexcept
	: i(T(0)), j(T(0)), k(T(0)), r(T(1)), ei(T(0)), ej(T(0)), ek(T(0)), er(T(0))
{}
template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>::DualQuaternion( Real const& aQ, Dual const& aEQ ) noexcept
	: q( aQ ), eq( aEQ )
{}
template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>::DualQuaternion( T aI, T aJ, T aK, T aR, T aEi, T aEj, T aEk, T aEr ) noexcept
	: i(aI), j(aJ), k(aK), r(aR), ei(aEi), ej(aEj), ek(aEk), er(aEr)
{}

template< typename T, class tSA > inline
DualQuaternion<T,tSA>::DualQuaternion( ftl::ExplicitInitList<T> aList ) noexcept
{
	FLUX_ASSERT( aList.ilist.size() == 8 );

	auto it = aList.ilist.begin();
	i = *it++;
	j = *it++;
	k = *it++;
	r = *it++;

	ei = *it++;
	ej = *it++;
	ek = *it++;
	er = *it++;
}

template< typename T, class tSA > template< typename Y, class tSB, class > inline FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>::DualQuaternion( DualQuaternion<Y,tSB> const& aR ) noexcept
	: i(aR.i), j(aR.j), k(aR.k), r(aR.r), ei(aR.ei), ej(aR.ej), ek(aR.ek), er(aR.er)
{}
template< typename T, class tSA > template< typename Y, class tSB, class > inline FLUX_CONSTEXPR_EX
auto DualQuaternion<T,tSA>::operator=( DualQuaternion<Y,tSB> const& aR ) noexcept -> DualQuaternion&
{
	i = aR.i;
	j = aR.j;
	k = aR.k;
	r = aR.r;

	ei = aR.ei;
	ej = aR.ej;
	ek = aR.ek;
	er = aR.er;
	return *this;
}


template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
T& DualQuaternion<T,tSA>::operator[] (std::size_t aI) noexcept
{
	FLUX_ASSERT( aI < 8 );
	return elems[aI];
}
template< typename T, class tSA > inline constexpr
T const& DualQuaternion<T,tSA>::operator[] (std::size_t aI) const noexcept
{
	return FLUX_ASSERT( aI < 8 ), elems[aI];
}

template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
auto DualQuaternion<T,tSA>::real() noexcept -> Real&
{
	return q;
}
template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
auto DualQuaternion<T,tSA>::real() const noexcept -> Real const&
{
	return q;
}
template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
auto DualQuaternion<T,tSA>::dual() noexcept -> Dual&
{
	return eq;
}
template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
auto DualQuaternion<T,tSA>::dual() const noexcept -> Dual const&
{
	return eq;
}


template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
T* DualQuaternion<T,tSA>::data() noexcept
{
	return elems;
}
template< typename T, class tSA > inline constexpr
T const* DualQuaternion<T,tSA>::data() const noexcept
{
	return elems;
}

//--    operators                           ///{{{2///////////////////////////

template< typename T, class tSA > inline constexpr
DualQuaternion<T,tSA> operator- (DualQuaternion<T,tSA> const& aQ) noexcept
{
	return make_dquat<DualQuaternion<T,tSA>>( 
		-aQ[0], -aQ[1], -aQ[2], -aQ[3], -aQ[4], -aQ[5], -aQ[6], -aQ[7]
	);
}
template< typename T, class tSA > inline constexpr
DualQuaternion<T,tSA> operator+ (DualQuaternion<T,tSA> const& aQ) noexcept
{
	return make_dquat<DualQuaternion<T,tSA>>( 
		+aQ[0], +aQ[1], +aQ[2], +aQ[3], +aQ[4], +aQ[5], +aQ[6], +aQ[7]
	);
}


template< typename T, class tSA, class tSB, class > inline constexpr
bool operator== (DualQuaternion<T,tSA> const& aQ, DualQuaternion<T,tSB> const& aR ) noexcept
{
	return aQ[0] == aR[0] && aQ[1] == aR[1] && aQ[2] == aR[2] && aQ[3] == aR[3]
		&& aQ[4] == aR[4] && aQ[5] == aR[5] && aQ[6] == aR[6] && aQ[7] == aR[7]
	;
}
template< typename T, class tSA, class tSB, class > inline constexpr
bool operator!= (DualQuaternion<T,tSA> const& aQ, DualQuaternion<T,tSB> const& aR ) noexcept
{
	return !(aQ == aR);
}


template< typename T, class tSA, class tSB, class tNA > inline constexpr
DualQuaternion<T,tNA> operator+ (DualQuaternion<T,tSA> const& aQ, DualQuaternion<T,tSB> const& aR ) noexcept
{
	return make_dquat<DualQuaternion<T,tSA>>( 
		aQ[0]+aR[0], aQ[1]+aR[1], aQ[2]+aR[2], aQ[3]+aR[3],
		aQ[4]+aR[4], aQ[5]+aR[5], aQ[6]+aR[6], aQ[7]+aR[7] 
	);
}
template< typename T, class tSA, class tSB, class tNA > inline constexpr
DualQuaternion<T,tNA> operator- (DualQuaternion<T,tSA> const& aQ, DualQuaternion<T,tSB> const& aR ) noexcept
{
	return make_dquat<DualQuaternion<T,tSA>>( 
		aQ[0]-aR[0], aQ[1]-aR[1], aQ[2]-aR[2], aQ[3]-aR[3],
		aQ[4]-aR[4], aQ[5]-aR[5], aQ[6]-aR[6], aQ[7]-aR[7] 
	);
}
template< typename T, class tSA, class tSB, class tNS > inline constexpr
DualQuaternion<T,tNS> operator* (DualQuaternion<T,tSA> const& aQ, DualQuaternion<T,tSB> const& aR ) noexcept
{
	return DualQuaternion<T,tNS>(
		aQ.real() * aR.real(),
		aQ.real() * aR.dual()  +  aQ.dual() * aR.real()
	);
}

template< typename T, class tSA > inline constexpr
DualQuaternion<T,tSA> operator* (DualQuaternion<T,tSA> const& aQ, T aX ) noexcept
{
	return make_dquat<DualQuaternion<T,tSA>>( 
		aQ[0]*aX, aQ[1]*aX, aQ[2]*aX, aQ[3]*aX,
		aQ[4]*aX, aQ[5]*aX, aQ[6]*aX, aQ[7]*aX
	);
}
template< typename T, class tSA > inline constexpr
DualQuaternion<T,tSA> operator/ (DualQuaternion<T,tSA> const& aQ, T aX ) noexcept
{
	return make_dquat<DualQuaternion<T,tSA>>( 
		aQ[0]/aX, aQ[1]/aX, aQ[2]/aX, aQ[3]/aX,
		aQ[4]/aX, aQ[5]/aX, aQ[6]/aX, aQ[7]/aX 
	);
}
template< typename T, class tSA > inline constexpr
DualQuaternion<T,tSA> operator* (T aX, DualQuaternion<T,tSA> const& aQ ) noexcept
{
	return make_dquat<DualQuaternion<T,tSA>>( 
		aX*aQ[0], aX*aQ[1], aX*aQ[2], aX*aQ[3],
		aX*aQ[4], aX*aQ[5], aX*aQ[6], aX*aQ[7]
	);
}

template< typename T, class tSA, class tSB, class > inline FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>& operator+= (DualQuaternion<T,tSA>& aQ, DualQuaternion<T,tSB> const& aR) noexcept
{
	aQ[0] += aR[0];
	aQ[1] += aR[1];
	aQ[2] += aR[2];
	aQ[3] += aR[3];
	aQ[4] += aR[4];
	aQ[5] += aR[5];
	aQ[6] += aR[6];
	aQ[7] += aR[7];
	return aQ;
}
template< typename T, class tSA, class tSB, class > inline FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>& operator-= (DualQuaternion<T,tSA>& aQ, DualQuaternion<T,tSB> const& aR) noexcept
{
	aQ[0] -= aR[0];
	aQ[1] -= aR[1];
	aQ[2] -= aR[2];
	aQ[3] -= aR[3];
	aQ[4] -= aR[4];
	aQ[5] -= aR[5];
	aQ[6] -= aR[6];
	aQ[7] -= aR[7];
	return aQ;
}
template< typename T, class tSA, class tSB, class > inline FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>& operator*= (DualQuaternion<T,tSA>& aQ, DualQuaternion<T,tSB> const& aR) noexcept
{
	auto const x = aQ * aR;
	return (aQ = x);
}

template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>& operator*= (DualQuaternion<T,tSA>& aQ, T aX) noexcept
{
	aQ[0] *= aX;
	aQ[1] *= aX;
	aQ[2] *= aX;
	aQ[3] *= aX;
	aQ[4] *= aX;
	aQ[5] *= aX;
	aQ[6] *= aX;
	aQ[7] *= aX;
	return aQ;
}
template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA>& operator/= (DualQuaternion<T,tSA>& aQ, T aX) noexcept
{
	aQ[0] /= aX;
	aQ[1] /= aX;
	aQ[2] /= aX;
	aQ[3] /= aX;
	aQ[4] /= aX;
	aQ[5] /= aX;
	aQ[6] /= aX;
	aQ[7] /= aX;
	return aQ;
}

//--    functions                           ///{{{2///////////////////////////

	// pseudo constructors -{{{3-
template< typename Q > inline constexpr
FLUX_ENABLE_IF( is_dual_quaternion<Q> )
(Q) make_dquat( dquat_type_t<Q> i, dquat_type_t<Q> j, dquat_type_t<Q> k, dquat_type_t<Q> r, dquat_type_t<Q> ei, dquat_type_t<Q> ej, dquat_type_t<Q> ek, dquat_type_t<Q> er ) noexcept
{
	return Q{ i, j, k, r, ei, ej, ek, er };
}

template< typename Q > inline 
FLUX_ENABLE_IF( is_dual_quaternion<Q> )
(Q) make_dquat( dquat_type_t<Q> const* aPtr ) noexcept
{
	FLUX_ASSERT( aPtr );
	return Q{ aPtr[0], aPtr[1], aPtr[2], aPtr[3], aPtr[4], aPtr[5], aPtr[6], aPtr[7] };
}

template< typename Q > inline constexpr
FLUX_ENABLE_IF( is_dual_quaternion<Q> )
(Q) make_zero( Identity<Q> ) noexcept
{
	return Q( Init::zero );
}
template< typename Q > inline constexpr
FLUX_ENABLE_IF( is_dual_quaternion<Q> )
(Q) make_identity( Identity<Q> ) noexcept
{
	return Q( Init::identity );
}

	// conversion
template< class Q, class R > inline constexpr
FLUX_ENABLE_IF_C( is_dual_quaternion<Q>::value && is_dual_quaternion<R>::value )
(Q) dual_quaternion_cast( R const& aR ) noexcept
{
	return Q{
		dquat_type_t<Q>(aR[0]),
		dquat_type_t<Q>(aR[1]),
		dquat_type_t<Q>(aR[2]),
		dquat_type_t<Q>(aR[3]),
		dquat_type_t<Q>(aR[4]),
		dquat_type_t<Q>(aR[5]),
		dquat_type_t<Q>(aR[6]),
		dquat_type_t<Q>(aR[7])
	};
}

template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
T* data_ptr( DualQuaternion<T,tSA>& aQ ) noexcept
{
	return aQ.data();
}
template< typename T, class tSA > inline constexpr
T const* data_ptr( DualQuaternion<T,tSA> const& aQ ) noexcept
{
	return aQ.data();
}

	// math. functions
template< typename T, class tSA, class tNA > inline constexpr
DualQuaternion<T,tNA> conj( DualQuaternion<T,tSA> const& aQ ) noexcept
{
	return make_dquat<DualQuaternion<T,tNA>>( 
		-aQ[0], -aQ[1], -aQ[2], aQ[3],
		-aQ[4], -aQ[5], -aQ[6], aQ[7] 
	);
}
template< typename T, class tSA, class tNA > inline
DualQuaternion<T,tNA> invert( DualQuaternion<T,tSA> const& aQ ) noexcept
{
	auto const nr = norm_2_real(aQ);
	auto const nd = norm_2_dual(aQ);

	return DualQuaternion<T,tNA>(
		nr * conj(aQ.real()),
		(nr-nd) * conj(aQ.dual())
	);
}
template< typename T, class tSA, class tNA > inline constexpr
DualQuaternion<T,tNA> invert_normalized( DualQuaternion<T,tSA> const& aQ ) noexcept
{
	return conj(aQ);
}


template< typename T, class tSA > inline
T norm_2_real( DualQuaternion<T,tSA> const& aQ ) noexcept
{
	return T(std::sqrt(norm_2_real_squared(aQ)));
}
template< typename T, class tSA > inline
T norm_2_dual( DualQuaternion<T,tSA> const& aQ ) noexcept
{
	// Urk. norm_2_dual_squared() isn't guaranteed to be positive; and in fact
	// numerical errors can cause it to become slightly negative when it should
	// be zero. Good times.
	static constexpr T eps = T(1e-6);
	
	auto const nds = norm_2_dual_squared(aQ);
	if( std::abs(nds) < eps )
		return T(0);
	
	return T(std::sqrt(nds));
}
template< typename T, class tSA > inline
T norm_2_real_squared( DualQuaternion<T,tSA> const& aQ ) noexcept
{
	return aQ[0]*aQ[0] + aQ[1]*aQ[1] + aQ[2]*aQ[2] + aQ[3]*aQ[3];
}
template< typename T, class tSA > inline
T norm_2_dual_squared( DualQuaternion<T,tSA> const& aQ ) noexcept
{
	return T(2) * (aQ[0]*aQ[4] + aQ[1]*aQ[5] + aQ[2]*aQ[6] + aQ[3]*aQ[7]);
}


template< typename T, class tSA > inline
DualQuaternion<T,tSA> normalize( DualQuaternion<T,tSA> const& aQ ) noexcept
{
	auto const dual = aQ.dual() * conj(aQ.real());
	auto const real = normalize(aQ.real());
	
	return DualQuaternion<T,tSA>( 
		real,
		dual * real
	);
}
template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
DualQuaternion<T,tSA> normalize_eps( DualQuaternion<T,tSA> const& aQ ) noexcept
{
	auto const dual = aQ.dual() * conj(aQ.real());
	auto const real = normalize_eps(aQ.real());
	
	return DualQuaternion<T,tSA>( 
		real,
		dual * real
	);
}

template< typename T, class tSA, class tSB, class tNA > inline constexpr
DualQuaternion<T,tNA> mix( T aX, DualQuaternion<T,tSA> const& aQ, DualQuaternion<T,tSB> const& aR ) noexcept
{
	return lerp( aX, aQ, aR );
}
template< typename T, class tSA, class tSB, class tNA > inline constexpr
DualQuaternion<T,tNA> lerp( T aX, DualQuaternion<T,tSA> const& aQ, DualQuaternion<T,tSB> const& aR ) noexcept
{
	return aQ + (aR-aQ) * aX;
}
#if 0
template< typename T, class tSA, class tSB, class tNA > inline 
Quaternion<T,tNA> slerp( T aX, Quaternion<T,tSA> const& aQ, Quaternion<T,tSB> const& aR ) noexcept
{
	using Quat_ = Quaternion<T,tSA>;

	Quat_ r = aR;
	T cosHalfTheta = aQ[0]*r[0] + aQ[1]*r[1] + aQ[2]*r[2] + aQ[3]*r[3];

	// -q and q represent the same orientation, but behave differently under
	// interpolation. check sign of q*r and change sign when necessary.
	if( cosHalfTheta < T(0) )
	{
		r = -r;
		cosHalfTheta = aQ[0]*r[0] + aQ[1]*r[1] + aQ[2]*r[2] + aQ[3]*r[3];
	}

	// close to each other? just lerp
	if( std::abs(cosHalfTheta) >= T(0.99) )
		return normalize_eps(lerp( aX, aQ, r ));

	// ...
	T halfTheta = std::acos(cosHalfTheta);
	T sinHalfTheta = std::sqrt( T(1) - cosHalfTheta*cosHalfTheta );

	T const a = std::sin( (T(1)-aX) * halfTheta ) / sinHalfTheta;
	T const b = std::sin( aX * halfTheta ) / sinHalfTheta;

	return a * aQ + b * r;
}
#endif

//--    <<< ~ flux::fml namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
