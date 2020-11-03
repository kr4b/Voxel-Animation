/*-******************************************************* -- HEADER -{{{1- */
/*- std::byte
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_BYTE_HPP_6C753094_0B17_425D_A5E4_F0B9EE49732E
#define FLUX_COMPAT_BYTE_HPP_6C753094_0B17_425D_A5E4_F0B9EE49732E

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>

//--    byte<                       ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX17_BYTE
#	include <cstddef>

FLUX_NAMESPACE_ENTER1(compat)
	using std::byte;
	using std::to_integer;
FLUX_NAMESPACE_LEAVE1(compat)

#else // !CXX17_BYTE
#	include <type_traits>

#	include <flux/std/inttypes.hpp>
#	include <flux/std/enable_if.hpp>

#	include <flux/compat/constexpr.hpp>

#	include <flux/util/attrib.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	struct FLUX_ATTR_MAY_ALIAS byte
	{
		uint8_t value;

		byte() = default;

		template< typename tType,
			typename = FLUX_ENABLE_IF(std::is_integral<tType>)(void)
		> constexpr inline
		explicit byte( tType aValue )
			: value(uint8_t(aValue))
		{}

		template< typename tType,
			typename = FLUX_ENABLE_IF(std::is_integral<tType>)(void)
		> constexpr inline
		explicit operator tType() const noexcept { return value; }

		constexpr inline
		bool operator== (byte const& aOther) const noexcept {
			return value == aOther.value;
		}
		constexpr inline
		bool operator!= (byte const& aOther) const noexcept {
			return value != aOther.value;
		}

		constexpr inline
		bool operator < (byte const& aOther) const noexcept {
			return value < aOther.value;
		}
		constexpr inline
		bool operator > (byte const& aOther) const noexcept {
			return value > aOther.value;
		}
		constexpr inline
		bool operator <= (byte const& aOther) const noexcept {
			return value < aOther.value;
		}
		constexpr inline
		bool operator >= (byte const& aOther) const noexcept {
			return value > aOther.value;
		}
	};

	template< typename tInt,
		typename = FLUX_ENABLE_IF(std::is_integral<tInt>)(void)
	> constexpr inline
	tInt to_integer( byte const& aByte ) noexcept {
		return tInt(aByte);
	}

	template< typename tInt,
		typename = FLUX_ENABLE_IF(std::is_integral<tInt>)(void)
	> constexpr inline
	byte operator<< (byte aX, tInt aShift) noexcept {
		return byte( static_cast<unsigned>(aX) << aShift);
	}
	template< typename tInt,
		typename = FLUX_ENABLE_IF(std::is_integral<tInt>)(void)
	> constexpr inline
	byte operator>> (byte aX, tInt aShift) noexcept {
		return byte( static_cast<unsigned>(aX) >> aShift);
	}

	template< typename tInt,
		typename = FLUX_ENABLE_IF(std::is_integral<tInt>)(void)
	> FLUX_CONSTEXPR_EX inline
	byte& operator<<= (byte& aX, tInt aShift) noexcept {
		return (aX = (aX << aShift));
	}
	template< typename tInt,
		typename = FLUX_ENABLE_IF(std::is_integral<tInt>)(void)
	> FLUX_CONSTEXPR_EX inline
	byte& operator>>= (byte& aX, tInt aShift) noexcept {
		return (aX = (aX >> aShift));
	}

	constexpr inline
	byte operator| (byte aX, byte aY) noexcept {
		return byte(static_cast<unsigned>(aX) | static_cast<unsigned>(aY));
	}
	constexpr inline
	byte operator& (byte aX, byte aY) noexcept {
		return byte(static_cast<unsigned>(aX) & static_cast<unsigned>(aY));
	}
	constexpr inline
	byte operator^ (byte aX, byte aY) noexcept {
		return byte(static_cast<unsigned>(aX) ^ static_cast<unsigned>(aY));
	}

	FLUX_CONSTEXPR_EX inline
	byte& operator|= (byte& aX, byte aY) noexcept {
		return aX = (aX | aY);
	}
	FLUX_CONSTEXPR_EX inline
	byte& operator&= (byte& aX, byte aY) noexcept {
		return aX = (aX & aY);
	}
	FLUX_CONSTEXPR_EX inline
	byte& operator^= (byte& aX, byte aY) noexcept {
		return aX = (aX ^ aY);
	}

	constexpr inline
	byte operator~ (byte aX) noexcept {
		return byte(~static_cast<unsigned>(aX));
	}

FLUX_NAMESPACE_LEAVE1(compat)
#endif // ~ CXX17_BYTE

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab:
#endif // FLUX_COMPAT_BYTE_HPP_6C753094_0B17_425D_A5E4_F0B9EE49732E
