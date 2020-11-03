/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- DEFINE_ENUM_BITFIELD() implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/compat/type_traits.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    DEFINE_ENUM_BITFIELD()              ///{{{1///////////////////////////

#undef FLUX_UTIL_DEFINE_ENUM_BITFIELD
#define FLUX_UTIL_DEFINE_ENUM_BITFIELD(se)                                  \
	inline constexpr se operator~(se aX) {                          \
		return static_cast<se>(                                             \
			~::flux::compat::underlying_type_t<se>(aX)                      \
		);                                                                  \
	}                                                                       \
	inline constexpr se operator| (se aX, se aY) {                  \
		return static_cast<se>(                                             \
			::flux::compat::underlying_type_t<se>(aX) |                     \
			::flux::compat::underlying_type_t<se>(aY)                       \
		);                                                                  \
	}                                                                       \
	inline constexpr se operator& (se aX, se aY) {                  \
		return static_cast<se>(                                             \
			::flux::compat::underlying_type_t<se>(aX) &                     \
			::flux::compat::underlying_type_t<se>(aY)                       \
		);                                                                  \
	}                                                                       \
	inline constexpr se operator^ (se aX, se aY) {                  \
		return static_cast<se>(                                             \
			::flux::compat::underlying_type_t<se>(aX) ^                     \
			::flux::compat::underlying_type_t<se>(aY)                       \
		);                                                                  \
	}                                                                       \
	inline se& operator&= (se& aX, se aY) {                                 \
		aX = aX & aY;                                                       \
		return aX;                                                          \
	}                                                                       \
	inline se& operator|= (se& aX, se aY) {                                 \
		aX = aX | aY;                                                       \
		return aX;                                                          \
	}                                                                       \
	inline se& operator^= (se& aX, se aY) {                                 \
		aX = aX ^ aY;                                                       \
		return aX;                                                          \
	}                                                                       \
	inline bool operator! (se const& aX) {                                  \
		return 0 == ::flux::compat::underlying_type_t<se>(aX);              \
	} /*ENDM*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
