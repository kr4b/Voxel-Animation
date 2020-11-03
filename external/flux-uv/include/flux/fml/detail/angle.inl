/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Angle stuff
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>
#include <flux/std/static_assert.hpp>

#include <flux/compat/constexpr.hpp>

#include <flux/fml/constants.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::fml >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	template< class tTo, class tFrom >
	struct ConvertAngle_;

	// trivial case: to/from is same unit
	template< class tUnit > struct ConvertAngle_<tUnit,tUnit> {
		template< typename tType > static inline constexpr
		tType conv( tType aValue ) {
			return aValue;
		}
	};

	// Radians and Degrees
	template<> struct ConvertAngle_<aspect::Radians,aspect::Degrees> {
		template< typename tType > static inline constexpr
		tType conv( tType aDeg ) {
			return aDeg * tType(FLUX_FML_PI / 180);
		}
	};
	template<> struct ConvertAngle_<aspect::Degrees,aspect::Radians> {
		template< typename tType > static inline constexpr
		tType conv( tType aRad ) {
			return aRad * tType(180 * FLUX_FML_1_PI);
		}
	};
}

//--    aspects                             ///{{{2///////////////////////////
namespace aspect
{
	template< class tOther, typename T > inline constexpr
	T Radians::from_unit( T aValue )
	{
		return detail::ConvertAngle_<Radians,tOther>::conv( aValue );
	}
	template< class tOther, typename T > inline constexpr
	T Degrees::from_unit( T aValue )
	{
		return detail::ConvertAngle_<Degrees,tOther>::conv( aValue );
	}
}

//--    Angle                               ///{{{2///////////////////////////
template< typename T, class tUnit > inline constexpr
Angle<T,tUnit>::Angle( T aValue )
	: mValue( aValue )
{}

template< typename T, class tUnit > inline constexpr
Angle<T,tUnit>::Angle( Angle const& aAngle )
	: mValue( aAngle.mValue )
{}
template< typename T, class tUnit > inline FLUX_CONSTEXPR_EX
Angle<T,tUnit>& Angle<T,tUnit>::operator= (Angle const& aAngle)
{
	mValue = aAngle.mValue;
	return *this;
}

template< typename T, class tUnit > template< class tOtherUnit > inline constexpr
Angle<T,tUnit>::Angle( Angle<T,tOtherUnit> const& aAngle )
	: mValue( tUnit::template from_unit<tOtherUnit>(aAngle.mValue) )
{}
template< typename T, class tUnit > template< class tOtherUnit > inline FLUX_CONSTEXPR_EX
Angle<T,tUnit>& Angle<T,tUnit>::operator= (Angle<T,tOtherUnit> const& aAngle)
{
	mValue = tUnit::template from_unit<tOtherUnit>(aAngle.mValue);
	return *this;
}


template< typename T, class tUnit > inline constexpr
T Angle<T,tUnit>::value() const
{
	return mValue;
}

//--    operators                           ///{{{2///////////////////////////
inline namespace literals
{
	inline constexpr
	Angle< float, aspect::Radians > operator "" _radf(long double aLit)
	{
		return Angle<float, aspect::Radians>(float(aLit));
	}
	inline constexpr
	Angle< float, aspect::Degrees > operator "" _degf(long double aLit)
	{
		return Angle<float, aspect::Degrees>(float(aLit));
	}

	inline constexpr
	Angle< double, aspect::Radians > operator "" _rad(long double aLit)
	{
		return Angle<double, aspect::Degrees>(double(aLit));
	}
	inline constexpr
	Angle< double, aspect::Degrees > operator "" _deg(long double aLit)
	{
		return Angle<double, aspect::Degrees>(double(aLit));
	}
}

	// arithmetic -{{{3-
template< typename T, class tUnit > inline constexpr
Angle<T,tUnit> operator- (Angle<T,tUnit> const& aX)
{
	return Angle<T,tUnit>(-aX.value());
}
template< typename T, class tUnit > inline constexpr
Angle<T,tUnit> operator+ (Angle<T,tUnit> const& aX)
{
	return Angle<T,tUnit>(+aX.value());
}

template< typename T, class tUnit, class tOther > inline constexpr
Angle<T,tUnit> operator+ (Angle<T,tUnit> const& aX, Angle<T,tOther> const& aY)
{
	return Angle<T,tUnit>(aX.value()+tUnit::template from_unit<tOther>(aY.value()));
}
template< typename T, class tUnit, class tOther > inline constexpr
Angle<T,tUnit> operator- (Angle<T,tUnit> const& aX, Angle<T,tOther> const& aY)
{
	return Angle<T,tUnit>(aX.value()-tUnit::template from_unit<tOther>(aY.value()));
}

template< typename T, class tUnit > inline constexpr
Angle<T,tUnit> operator* (Angle<T,tUnit> const& aX, T aY)
{
	return Angle<T,tUnit>(aX.value()*aY);
}
template< typename T, class tUnit > inline constexpr
Angle<T,tUnit> operator/ (Angle<T,tUnit> const& aX, T aY)
{
	return Angle<T,tUnit>(aX.value()/aY);
}
template< typename T, class tUnit, class tOther > inline constexpr
T operator/ (Angle<T,tUnit> const& aX, Angle<T,tOther> const& aY)
{
	return aX.value() / tUnit::template from_unit<tOther>(aY.value());
}


template< typename T, class tUnit > inline constexpr
Angle<T,tUnit> operator* (T aX, Angle<T,tUnit> const& aY)
{
	return Angle<T,tUnit>(aX*aY.value());
}

template< typename T, class tUnit, class tOther > inline FLUX_CONSTEXPR_EX
Angle<T,tUnit>& operator+= (Angle<T,tUnit>& aX, Angle<T,tOther> const& aY)
{
	aX = aX + aY;
	return aX;
}
template< typename T, class tUnit, class tOther > inline FLUX_CONSTEXPR_EX
Angle<T,tUnit>& operator-= (Angle<T,tUnit>& aX, Angle<T,tOther> const& aY)
{
	aX = aX - aY;
	return aX;
}

template< typename T, class tUnit > inline FLUX_CONSTEXPR_EX
Angle<T,tUnit>& operator*= (Angle<T,tUnit>& aX, T aY)
{
	aX = aX * aY;
	return aX;
}
template< typename T, class tUnit > inline FLUX_CONSTEXPR_EX
Angle<T,tUnit>& operator/= (Angle<T,tUnit>& aX, T aY)
{
	aX = aX / aY;
	return aX;
}

	// comparison -{{{3-
template< typename T, class tUnit > inline constexpr
bool operator== (Angle<T,tUnit> const& aX, Angle<T,tUnit> const& aY)
{
	return aX.value() == aY.value();
}
template< typename T, class tUnit > inline constexpr
bool operator!= (Angle<T,tUnit> const& aX, Angle<T,tUnit> const& aY)
{
	return aX.value() != aY.value();
}

template< typename T, class tUnit, class tOther > inline constexpr
bool operator== (Angle<T,tUnit> const& aX, Angle<T,tOther> const& aY)
{
	return aX.value() == Angle<T,tUnit>(aY).value();
}
template< typename T, class tUnit, class tOther > inline constexpr
bool operator!= (Angle<T,tUnit> const& aX, Angle<T,tOther> const& aY)
{
	return aX.value() != Angle<T,tUnit>(aY).value();
}

//--    functions                           ///{{{2///////////////////////////
inline constexpr 
Angle< float, aspect::Radians > radiansf(float aX)
{
	return Angle<float, aspect::Radians>(aX);
}
inline constexpr
Angle< float, aspect::Degrees > degreesf(float aX)
{
	return Angle<float, aspect::Degrees>(aX);
}

inline constexpr
Angle< double, aspect::Radians > radians(double aX)
{
	return Angle<double, aspect::Radians>(aX);
}
inline constexpr
Angle< double, aspect::Degrees > degrees(double aX)
{
	return Angle<double, aspect::Degrees>(aX);
}



template< typename tTo, typename tFrom, class tUnits >
inline constexpr
Angle<tTo,tUnits> angle_cast( Angle<tFrom,tUnits> const& aAngle )
{
	return Angle<tTo,tUnits>( tTo(aAngle.value()) );
}

//--    <<< ~ flux::fml namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
