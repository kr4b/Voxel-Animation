/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Quaternion stuff
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
	template< class, class, class > struct QuatSpecCompat
		: std::false_type 
	{};
	template< class tSA, class tSB >
	struct QuatSpecCompat< tSA, tSB, compat::void_t< 
			typename tSA::SrcSpace, typename tSA::DstSpace,
			typename tSB::SrcSpace, typename tSB::DstSpace
		>
	> : std::integral_constant< bool,
			   space_compatible<typename tSA::SrcSpace, typename tSB::SrcSpace>::value
			&& space_compatible<typename tSA::DstSpace, typename tSB::DstSpace>::value
		>
	{};
	
	template< typename T, class tSA >
	struct QuatSrcSpace< Quaternion<T,tSA> > {
		using type = typename tSA::SrcSpace;
	};
	template< typename T, class tSA >
	struct QuatDstSpace< Quaternion<T,tSA> > {
		using type = typename tSA::DstSpace;
	};

	template< typename T, class tSA, class tNewSrc >
	struct QuatRebindSrc< Quaternion<T,tSA>, tNewSrc > {
		using type = Quaternion< T, aspect::QuatSpec<typename tSA::DstSpace, tNewSrc> >;
	};
	template< typename T, class tSA, class tNewDst >
	struct QuatRebindDst< Quaternion<T,tSA>, tNewDst > {
		using type = Quaternion< T, aspect::QuatSpec<tNewDst, typename tSA::SrcSpace> >;
	};
	template< typename T, class tSA, class tNewDst, class tNewSrc >
	struct QuatRebind< Quaternion<T,tSA>, tNewDst, tNewSrc > {
		using type = Quaternion< T, aspect::QuatSpec<tNewDst, tNewSrc> >;
	};

	template< class tSA, class tSB >
	struct QuatSpec
	{
		using type = aspect::QuatSpec<
			space_specialized_t<typename tSA::DstSpace, typename tSB::DstSpace>,
			space_specialized_t<typename tSA::SrcSpace, typename tSB::SrcSpace>
		>;
	};


	template< bool, class, class > struct QuatMulSpec0;
	template< class tDst, class tSrc >
	struct QuatMulSpec0<true,tDst,tSrc> {
		using type = aspect::QuatSpec< tDst, tSrc >;
	};

	template< class tSA >
	struct QuatInvSpec
	{
		using type = aspect::QuatSpec< typename tSA::SrcSpace, typename tSA::DstSpace >;
	};
	template< class tSA, class tSB >
	struct QuatMulSpec
		: detail::QuatMulSpec0< 
			space_compatible<typename tSA::SrcSpace, typename tSB::DstSpace>::value,
			typename tSA::DstSpace,
			typename tSB::SrcSpace
		>
	{};
}

//--    Quaternion                          ///{{{2///////////////////////////
template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
Quaternion<T,tSA>::Quaternion( Init::Zero ) noexcept
	: elem{ T(0), T(0), T(0), T(0) }
{}
template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
Quaternion<T,tSA>::Quaternion( Init::Identity ) noexcept
	: elem{ T(0), T(0), T(0), T(1) }
{}
template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
Quaternion<T,tSA>::Quaternion( T i, T j, T k, T r ) noexcept
	: elem{ i, j, k, r }
{}

template< typename T, class tSA > inline
Quaternion<T,tSA>::Quaternion( ftl::ExplicitInitList<T> aList ) noexcept
{
	FLUX_ASSERT( aList.ilist.size() == 4 );

	auto it = aList.ilist.begin();
	elem[0] = *it++;
	elem[1] = *it++;
	elem[2] = *it++;
	elem[3] = *it++;
}

template< typename T, class tSA > template< typename Y, class tSB, class > inline FLUX_CONSTEXPR_EX
Quaternion<T,tSA>::Quaternion( Quaternion<Y,tSB> const& aR ) noexcept
	: elem{ aR[0], aR[1], aR[2], aR[3] }
{}
template< typename T, class tSA > template< typename Y, class tSB, class > inline FLUX_CONSTEXPR_EX
auto Quaternion<T,tSA>::operator=( Quaternion<Y,tSB> const& aR ) noexcept -> Quaternion&
{
	elem[0] = aR[0];
	elem[1] = aR[1];
	elem[2] = aR[2];
	elem[3] = aR[3];
	return *this;
}


template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
T& Quaternion<T,tSA>::operator[] (std::size_t aI) noexcept
{
	FLUX_ASSERT( aI < 4 );
	return elem[aI];
}
template< typename T, class tSA > inline constexpr
T const& Quaternion<T,tSA>::operator[] (std::size_t aI) const noexcept
{
	return FLUX_ASSERT( aI < 4 ), elem[aI];
}

template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
T* Quaternion<T,tSA>::data() noexcept
{
	return elem;
}
template< typename T, class tSA > inline constexpr
T const* Quaternion<T,tSA>::data() const noexcept
{
	return elem;
}

//--    operators                           ///{{{2///////////////////////////

template< typename T, class tSA > inline constexpr
Quaternion<T,tSA> operator- (Quaternion<T,tSA> const& aQ) noexcept
{
	return make_quat<Quaternion<T,tSA>>( -aQ[0], -aQ[1], -aQ[2], -aQ[3] );
}
template< typename T, class tSA > inline constexpr
Quaternion<T,tSA> operator+ (Quaternion<T,tSA> const& aQ) noexcept
{
	return make_quat<Quaternion<T,tSA>>( +aQ[0], +aQ[1], +aQ[2], +aQ[3] );
}


template< typename T, class tSA, class tSB, class > inline constexpr
bool operator== (Quaternion<T,tSA> const& aQ, Quaternion<T,tSB> const& aR ) noexcept
{
	return aQ[0] == aR[0] && aQ[1] == aR[1] && aQ[2] == aR[2] && aQ[3] == aR[3];
}
template< typename T, class tSA, class tSB, class > inline constexpr
bool operator!= (Quaternion<T,tSA> const& aQ, Quaternion<T,tSB> const& aR ) noexcept
{
	return !(aQ == aR);
}


template< typename T, class tSA, class tSB, class tNA > inline constexpr
Quaternion<T,tNA> operator+ (Quaternion<T,tSA> const& aQ, Quaternion<T,tSB> const& aR ) noexcept
{
	return make_quat<Quaternion<T,tSA>>( aQ[0]+aR[0], aQ[1]+aR[1], aQ[2]+aR[2], aQ[3]+aR[3] );
}
template< typename T, class tSA, class tSB, class tNA > inline constexpr
Quaternion<T,tNA> operator- (Quaternion<T,tSA> const& aQ, Quaternion<T,tSB> const& aR ) noexcept
{
	return make_quat<Quaternion<T,tSA>>( aQ[0]-aR[0], aQ[1]-aR[1], aQ[2]-aR[2], aQ[3]-aR[3] );
}
template< typename T, class tSA, class tSB, class tNS > inline constexpr
Quaternion<T,tNS> operator* (Quaternion<T,tSA> const& aQ, Quaternion<T,tSB> const& aR ) noexcept
{
	return make_quat<Quaternion<T,tNS>>(
		+aQ[0]*aR[3] +aQ[1]*aR[2] -aQ[2]*aR[1] +aQ[3]*aR[0],
		-aQ[0]*aR[2] +aQ[1]*aR[3] +aQ[2]*aR[0] +aQ[3]*aR[1],
		+aQ[0]*aR[1] -aQ[1]*aR[0] +aQ[2]*aR[3] +aQ[3]*aR[2],
		-aQ[0]*aR[0] -aQ[1]*aR[1] -aQ[2]*aR[2] +aQ[3]*aR[3]
	);
}

template< typename T, class tSA > inline constexpr
Quaternion<T,tSA> operator* (Quaternion<T,tSA> const& aQ, T aX ) noexcept
{
	return make_quat<Quaternion<T,tSA>>( aQ[0]*aX, aQ[1]*aX, aQ[2]*aX, aQ[3]*aX );
}
template< typename T, class tSA > inline constexpr
Quaternion<T,tSA> operator/ (Quaternion<T,tSA> const& aQ, T aX ) noexcept
{
	return make_quat<Quaternion<T,tSA>>( aQ[0]/aX, aQ[1]/aX, aQ[2]/aX, aQ[3]/aX );
}
template< typename T, class tSA > inline constexpr
Quaternion<T,tSA> operator* (T aX, Quaternion<T,tSA> const& aQ ) noexcept
{
	return make_quat<Quaternion<T,tSA>>( aX*aQ[0], aX*aQ[1], aX*aQ[2], aX*aQ[3] );
}


template< typename T, class tSA, class tSB, class > inline FLUX_CONSTEXPR_EX
Quaternion<T,tSA>& operator+= (Quaternion<T,tSA>& aQ, Quaternion<T,tSB> const& aR) noexcept
{
	aQ[0] += aR[0];
	aQ[1] += aR[1];
	aQ[2] += aR[2];
	aQ[3] += aR[3];
	return aQ;
}
template< typename T, class tSA, class tSB, class > inline FLUX_CONSTEXPR_EX
Quaternion<T,tSA>& operator-= (Quaternion<T,tSA>& aQ, Quaternion<T,tSB> const& aR) noexcept
{
	aQ[0] -= aR[0];
	aQ[1] -= aR[1];
	aQ[2] -= aR[2];
	aQ[3] -= aR[3];
	return aQ;
}
template< typename T, class tSA, class tSB, class > inline FLUX_CONSTEXPR_EX
Quaternion<T,tSA>& operator*= (Quaternion<T,tSA>& aQ, Quaternion<T,tSB> const& aR) noexcept
{
	T const qx = aQ[0]; T const rx = aR[0];
	T const qy = aQ[1]; T const ry = aR[1];
	T const qz = aQ[2]; T const rz = aR[2];
	T const qw = aQ[3]; T const rw = aR[3];

	aQ[0] = +qx*rw +qy*rz -qz*ry +qw*rx;
	aQ[1] = -qx*rz +qy*rw +qz*rx +qw*ry;
	aQ[2] = +qx*ry -qy*rx +qz*rw +qw*rz;
	aQ[3] = -qx*rx -qy*ry -qz*rz +qw*rw;

	return aQ;
}

template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
Quaternion<T,tSA>& operator*= (Quaternion<T,tSA>& aQ, T aX) noexcept
{
	aQ[0] *= aX;
	aQ[1] *= aX;
	aQ[2] *= aX;
	aQ[3] *= aX;
	return aQ;
}
template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
Quaternion<T,tSA>& operator/= (Quaternion<T,tSA>& aQ, T aX) noexcept
{
	aQ[0] /= aX;
	aQ[1] /= aX;
	aQ[2] /= aX;
	aQ[3] /= aX;
	return aQ;
}

//--    functions                           ///{{{2///////////////////////////

	// pseudo constructors -{{{3-
template< typename Q > inline constexpr
FLUX_ENABLE_IF( is_quaternion<Q> )
(Q) make_quat( quat_type_t<Q> i, quat_type_t<Q> j, quat_type_t<Q> k, quat_type_t<Q> r ) noexcept
{
	return Q{ i, j, k, r };
}

template< typename Q > inline 
FLUX_ENABLE_IF( is_quaternion<Q> )
(Q) make_quat( quat_type_t<Q> const* aPtr ) noexcept
{
	FLUX_ASSERT( aPtr );
	return Q{ aPtr[0], aPtr[1], aPtr[2], aPtr[3] };
}

template< typename Q > inline constexpr
FLUX_ENABLE_IF( is_quaternion<Q> )
(Q) make_zero( Identity<Q> ) noexcept
{
	return Q( Init::zero );
}
template< typename Q > inline constexpr
FLUX_ENABLE_IF( is_quaternion<Q> )
(Q) make_identity( Identity<Q> ) noexcept
{
	return Q( Init::identity );
}

	// conversion
template< class Q, class R > inline constexpr
FLUX_ENABLE_IF_C( is_quaternion<Q>::value && is_quaternion<R>::value )
(Q) quaternion_cast( R const& aR ) noexcept
{
	return Q{
		quat_type_t<Q>(aR[0]),
		quat_type_t<Q>(aR[1]),
		quat_type_t<Q>(aR[2]),
		quat_type_t<Q>(aR[3])
	};
}

template< typename T, class tSA > inline FLUX_CONSTEXPR_EX
T* data_ptr( Quaternion<T,tSA>& aQ ) noexcept
{
	return aQ.data();
}
template< typename T, class tSA > inline constexpr
T const* data_ptr( Quaternion<T,tSA> const& aQ ) noexcept
{
	return aQ.data();
}

	// math. functions
template< typename T, class tSA, class tNA > inline constexpr
Quaternion<T,tNA> conj( Quaternion<T,tSA> const& aQ ) noexcept
{
	return make_quat<Quaternion<T,tNA>>( -aQ[0], -aQ[1], -aQ[2], aQ[3] );
}
template< typename T, class tSA, class tNA > inline
Quaternion<T,tNA> invert( Quaternion<T,tSA> const& aQ ) noexcept
{
	return conj(aQ) / norm_2(aQ);
}

template< typename T, class tSA > inline
T length( Quaternion<T,tSA> const& aQ ) noexcept
{
	return norm_2(aQ);
}
template< typename T, class tSA > inline constexpr
T length_squared( Quaternion<T,tSA> const& aQ ) noexcept
{
	return aQ[0]*aQ[0] + aQ[1]*aQ[1] + aQ[2]*aQ[2] + aQ[3]*aQ[3];
}
template< typename T, class tSA > inline
T norm_2( Quaternion<T,tSA> const& aQ ) noexcept
{
	return T(std::sqrt(aQ[0]*aQ[0]+aQ[1]*aQ[1]+aQ[2]*aQ[2]+aQ[3]*aQ[3]));
}

template< typename T, class tSA > inline
Quaternion<T,tSA> normalize( Quaternion<T,tSA> const& aQ ) noexcept
{
	return aQ / norm_2(aQ);
}
template< typename T, class tSA > inline constexpr
Quaternion<T,tSA> normalize_eps( Quaternion<T,tSA> const& aQ ) noexcept
{
	// See http://stackoverflow.com/a/12934750 for a somewhat interesting 
	// discussion on this. Essentially this uses a first order Padé approx
	// to account for small deviations from the normalized length.
	return aQ * (T(2) / (T(1) + length_squared(aQ)));
}

template< typename T, class tSA, class tSB, class tNA > inline constexpr
Quaternion<T,tNA> mix( T aX, Quaternion<T,tSA> const& aQ, Quaternion<T,tSB> const& aR ) noexcept
{
	return lerp( aX, aQ, aR );
}
template< typename T, class tSA, class tSB, class tNA > inline constexpr
Quaternion<T,tNA> lerp( T aX, Quaternion<T,tSA> const& aQ, Quaternion<T,tSB> const& aR ) noexcept
{
	return aQ + (aR-aQ) * aX;
}
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

//--    <<< ~ flux::fml namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
