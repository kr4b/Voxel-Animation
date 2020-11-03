/*-******************************************************* -- HEADER -{{{1- */
/*- Angle types
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FML_ANGLE_HPP_37307338_6D8D_4097_B3B6_C1172315E583
#define FLUX_FML_ANGLE_HPP_37307338_6D8D_4097_B3B6_C1172315E583

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/enable_if.hpp>
#include <flux/ftl/meta_bool.hpp>
#include <flux/compat/constexpr.hpp>

//--    >>> namespace = flux::fml >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    aspects                         ///{{{2///////////////////////////////
namespace aspect
{
	struct Radians
	{
		template< class, typename T > static constexpr
		T from_unit( T );
	};

	struct Degrees
	{
		template< class, typename T > static constexpr
		T from_unit( T );
	};
};

//--    Angle                           ///{{{2///////////////////////////////
/* Note: Angle<> doesn't wrap it's argument into the range of angles. */
template< typename T = float, class tUnit = aspect::Radians >
struct Angle
{
	constexpr explicit Angle( T = T(0) );

	constexpr Angle( Angle const& );
	FLUX_CONSTEXPR_EX Angle& operator= (Angle const&);

	template< class tOtherUnit > constexpr
	Angle( Angle<T, tOtherUnit> const& );
	template< class tOtherUnit > FLUX_CONSTEXPR_EX
	Angle& operator= (Angle<T, tOtherUnit> const&);

	constexpr T value() const;

	private: T mValue;
	private: template< typename, class > friend struct Angle;
};

/** \name Angle: pre-defined types
 * \brief Pre-defined angles
 */
//@{
typedef Angle<float, aspect::Radians> Radiansf;
typedef Angle<float, aspect::Degrees> Degreesf;
//@}

//--    meta functions                  ///{{{2///////////////////////////////
template< typename >
struct is_angle : std::false_type
{};

template< typename tType, class tAspect >
struct is_angle< Angle<tType,tAspect> > : std::true_type
{};

//--    operators                       ///{{{2///////////////////////////////

inline namespace literals
{
	constexpr Angle< float, aspect::Radians > operator "" _radf(long double);
	constexpr Angle< float, aspect::Degrees > operator "" _degf(long double);

	constexpr Angle< double, aspect::Radians > operator "" _rad(long double);
	constexpr Angle< double, aspect::Degrees > operator "" _deg(long double);
}

	// arithmetic
template< typename T, class tUnit > constexpr
Angle<T,tUnit> operator- (Angle<T,tUnit> const&);
template< typename T, class tUnit > constexpr
Angle<T,tUnit> operator+ (Angle<T,tUnit> const&);

template< typename T, class tUnit, class tOther > constexpr
Angle<T,tUnit> operator+ (Angle<T,tUnit> const&, Angle<T,tOther> const&);
template< typename T, class tUnit, class tOther > constexpr
Angle<T,tUnit> operator- (Angle<T,tUnit> const&, Angle<T,tOther> const&);

template< typename T, class tUnit > constexpr
Angle<T,tUnit> operator* (Angle<T,tUnit> const&, T);
template< typename T, class tUnit > constexpr
Angle<T,tUnit> operator/ (Angle<T,tUnit> const&, T);
template< typename T, class tUnit, class tOther > inline constexpr
T operator/ (Angle<T,tUnit> const&, Angle<T,tOther> const&);

template< typename T, class tUnit > constexpr
Angle<T,tUnit> operator* (T, Angle<T,tUnit> const&);

template< typename T, class tUnit, class tOther > FLUX_CONSTEXPR_EX
Angle<T,tUnit>& operator+= (Angle<T,tUnit>&, Angle<T,tOther> const&);
template< typename T, class tUnit, class tOther > FLUX_CONSTEXPR_EX
Angle<T,tUnit>& operator-= (Angle<T,tUnit>&, Angle<T,tOther> const&);

template< typename T, class tUnit > FLUX_CONSTEXPR_EX
Angle<T,tUnit>& operator*= (Angle<T,tUnit>&, T);
template< typename T, class tUnit > FLUX_CONSTEXPR_EX
Angle<T,tUnit>& operator/= (Angle<T,tUnit>&, T);


	// comparison
template< typename T, class tUnit > constexpr
bool operator== (Angle<T,tUnit> const&, Angle<T,tUnit> const&);
template< typename T, class tUnit > constexpr
bool operator!= (Angle<T,tUnit> const&, Angle<T,tUnit> const&);

template< typename T, class tUnit, class tOther > constexpr
bool operator== (Angle<T,tUnit> const&, Angle<T,tOther> const&);
template< typename T, class tUnit, class tOther > constexpr
bool operator!= (Angle<T,tUnit> const&, Angle<T,tOther> const&);


//--    functions                       ///{{{2///////////////////////////////

constexpr Angle< float, aspect::Radians > radiansf(float);
constexpr Angle< float, aspect::Degrees > degreesf(float);

constexpr Angle< double, aspect::Radians > radians(double);
constexpr Angle< double, aspect::Degrees > degrees(double);

template< typename tTo, typename tFrom, class tUnits > constexpr
Angle<tTo,tUnits> angle_cast( Angle<tFrom,tUnits> const& );

//--    <<< ~ flux::fml namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/angle.inl"
#endif // FLUX_FML_ANGLE_HPP_37307338_6D8D_4097_B3B6_C1172315E583
