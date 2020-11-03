/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Bit Twiddling
 */
/*-***************************************************************** -}}}1- */

#include <flux/setup/arch.hpp>
#include <flux/setup/compiler.hpp>

#if FLUX_ARCH == FLUX_ARCH_X86_32 || FLUX_ARCH == FLUX_ARCH_X86_64
#	if FLUX_COMPILER != FLUX_COMPILER_MSVC
#		include <x86intrin.h>
#	else // MSVC
#		include <intrin.h>

#		define FLUXTMP_UTIL_BIT_HAVE_MSVCPOPCNT_ 1
#		pragma intrinsic(_BitScanReverse)
#		pragma intrinsic(_BitScanForward)

#		if FLUX_ARCH == FLUX_ARCH_X86_64
#			define FLUXTMP_UTIL_BIT_HAVE_MSVCPOPCNT64_ 1
#			pragma intrinsic(_BitScanReverse64)
#			pragma intrinsic(_BitScanForward64)
#		endif // ~ 64-bit only
#	endif // ~ compiler


#	if defined(__POPCNT__)
#		define FLUXTMP_UTIL_BIT_HAVE_POPCNT_ 1

#		if FLUX_ARCH == FLUX_ARCH_X86_64
#			define FLUXTMP_UTIL_BIT_HAVE_POPCNT_ 1
#		endif // ~ 64-bit only
#	endif

#	if defined(__LZCNT__)
#		define FLUXTMP_UTIL_BIT_HAVE_LZCNT_ 1
#	endif

#	if defined(__BMI__)
#		define FLUXTMP_UTIL_BIT_HAVE_TZCNT_ 1
#	endif

#	if FLUX_COMPILER == FLUX_COMPILER_GCC
	/* GCC <= 4.8 bug: use of __lzcnt() / __tzcnt() ICEs the compiler in 
	 * release mode, i.e., either due to optimizations or due to LTO. 
	 *
	 * Debug mode (no optimizations and/or LTO) works, but in order to keep
	 * the debug and release code somewhat similar, we'll disable it there
	 * as well.
	 */
#		if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ <= 8)
#			undef FLUXTMP_UTIL_BIT_HAVE_LZCNT_
#			undef FLUXTMP_UTIL_BIT_HAVE_TZCNT_

#			define FLUXTMP_UTIL_BIT_HAVE_LZCNT_ 0
#			define FLUXTMP_UTIL_BIT_HAVE_TZCNT_ 0
#		endif // ~ GCC before or equal to 4.8
#	endif // ~ COMPILER_GCC
#endif

#if !defined(FLUXTMP_UTIL_BIT_HAVE_POPCNT_)
#	define FLUXTMP_UTIL_BIT_HAVE_POPCNT_ 0
#endif
#if !defined(FLUXTMP_UTIL_BIT_HAVE_POPCNT64_)
#	define FLUXTMP_UTIL_BIT_HAVE_POPCNT64_ 0
#endif

#if !defined(FLUXTMP_UTIL_BIT_HAVE_LZCNT_)
#	define FLUXTMP_UTIL_BIT_HAVE_LZCNT_ 0
#endif

#if !defined(FLUXTMP_UTIL_BIT_HAVE_TZCNT_)
#	define FLUXTMP_UTIL_BIT_HAVE_TZCNT_ 0
#endif

#if !defined(FLUXTMP_UTIL_BIT_HAVE_MSVCPOPCNT_)
#	define FLUXTMP_UTIL_BIT_HAVE_MSVCPOPCNT_ 0
#endif
#if !defined(FLUXTMP_UTIL_BIT_HAVE_MSVCPOPCNT64_)
#	define FLUXTMP_UTIL_BIT_HAVE_MSVCPOPCNT64_ 0
#endif

#include <climits> // CHAR_BIT

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::util >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(util)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	template< typename tType > inline
	tType popcount_( tType aX )
	{
		// See https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
		// Valid up to 128-bit integers.
		aX = aX - ((aX >> 1) & (tType)~(tType)0/3);
		aX = (aX & (tType)~(tType)0/15*3) + ((aX >> 2) & (tType)~(tType)0/15*3);
		aX = (aX + (aX >> 4)) & (tType)~(tType)0/255*15;
		return (tType)(aX * ((tType)~(tType)0/255)) >> (sizeof(tType) - 1) * CHAR_BIT;
	}
}

//--    bit_count_set()                     ///{{{2///////////////////////////
inline uint16_t bit_count_set( uint16_t aX ) noexcept
{
	// TODO: MSVC __popcnt16() ?!

#	if FLUXTMP_UTIL_BIT_HAVE_POPCNT_
	return _mm_popcnt_u32( aX );

#	elif FLUX_COMPILER == FLUX_COMPILER_GCC || FLUX_COMPILER == FLUX_COMPILER_CLANG
	return __builtin_popcount( aX );

#	elif FLUXTMP_UTIL_BIT_HAVE_MSVCPOPCNT_
	return __popcnt16(aX);

#	else // fallback
	return detail::popcount_( aX );
#	endif // ~ implementation
}
inline uint32_t bit_count_set( uint32_t aX ) noexcept
{
#	if FLUXTMP_UTIL_BIT_HAVE_POPCNT_
	return _mm_popcnt_u32( aX );

#	elif FLUX_COMPILER == FLUX_COMPILER_GCC || FLUX_COMPILER == FLUX_COMPILER_CLANG
	return __builtin_popcount( aX );

#	elif FLUXTMP_UTIL_BIT_HAVE_MSVCPOPCNT_
	return __popcnt(aX);

#	else // fallback
	return detail::popcount_( aX );
#	endif // ~ implementation
}
inline uint64_t bit_count_set( uint64_t aX ) noexcept
{
#	if FLUXTMP_UTIL_BIT_HAVE_POPCNT64_
	return _mm_popcnt_u64( aX );

#	elif FLUX_COMPILER == FLUX_COMPILER_GCC || FLUX_COMPILER == FLUX_COMPILER_CLANG
	return __builtin_popcountll( aX );
	
#	elif FLUXTMP_UTIL_BIT_HAVE_MSVCPOPCNT64_
	return __popcnt64(aX);

#	else // fallback
	return detail::popcount_( aX );
#	endif // ~ implementation
}

//--    bit_count_leading_zeros()           ///{{{2///////////////////////////
inline uint16_t bit_count_leading_zeros( uint16_t aX ) noexcept
{
#	if FLUXTMP_UTIL_BIT_HAVE_LZCNT_
	return __lzcnt16( aX );

#	elif FLUX_COMPILER == FLUX_COMPILER_GCC || FLUX_COMPILER == FLUX_COMPILER_CLANG
	return aX ? __builtin_clz( uint32_t(aX) ) - 16 : 16;

#	elif FLUX_COMPILER == FLUX_COMPILER_MSVC
	unsigned long res;
	if( _BitScanReverse( &res, aX ) )
		return 15-uint16_t(res);

	return 16;

#	else // fallback
	if( !aX ) return 16;

	uint16_t res = 0;

	if( !(aX & 0xff00) ) (res +=  8), (aX <<=  8);
	if( !(aX & 0xf000) ) (res +=  4), (aX <<=  4);
	if( !(aX & 0xc000) ) (res +=  2), (aX <<=  2);
	if( !(aX & 0x8000) ) (res +=  1);

	return res;

#	endif // ~ implementation
}
inline uint32_t bit_count_leading_zeros( uint32_t aX ) noexcept
{
#	if FLUXTMP_UTIL_BIT_HAVE_LZCNT_
	return __lzcnt32( aX );

#	elif FLUX_COMPILER == FLUX_COMPILER_GCC || FLUX_COMPILER == FLUX_COMPILER_CLANG
	return aX ? __builtin_clz( aX ) : 32;

#	elif FLUX_COMPILER == FLUX_COMPILER_MSVC
	unsigned long res;
	if( _BitScanReverse( &res, aX ) )
		return 31-res;

	return 32;

#	else // fallback
	if( !aX ) return 32;

	uint32_t res = 0;

	if( !(aX & 0xffff0000) ) (res += 16), (aX <<= 16);
	if( !(aX & 0xff000000) ) (res +=  8), (aX <<=  8);
	if( !(aX & 0xf0000000) ) (res +=  4), (aX <<=  4);
	if( !(aX & 0xc0000000) ) (res +=  2), (aX <<=  2);
	if( !(aX & 0x80000000) ) (res +=  1);

	return res;
#	endif // ~ implementation
}
inline uint64_t bit_count_leading_zeros( uint64_t aX ) noexcept
{
#	if FLUXTMP_UTIL_BIT_HAVE_LZCNT_ && defined(__x86_64__)
	return __lzcnt64( aX );

#	elif FLUX_COMPILER == FLUX_COMPILER_GCC || FLUX_COMPILER == FLUX_COMPILER_CLANG
	return aX ? __builtin_clzll( aX ) : 64;

#	elif FLUX_COMPILER == FLUX_COMPILER_MSVC && FLUX_ARCH == FLUX_ARCH_X86_64
	unsigned long res;
	if( _BitScanReverse64( &res, aX ) )
		return 63-res;

	return 64;

#	else // fallback
	if( !aX ) return 64;

	uint64_t res = 0;

	if( !(aX & 0xffffffff00000000) ) (res += 32), (aX <<= 32);
	if( !(aX & 0xffff000000000000) ) (res += 16), (aX <<= 16);
	if( !(aX & 0xff00000000000000) ) (res +=  8), (aX <<=  8);
	if( !(aX & 0xf000000000000000) ) (res +=  4), (aX <<=  4);
	if( !(aX & 0xc000000000000000) ) (res +=  2), (aX <<=  2);
	if( !(aX & 0x8000000000000000) ) (res +=  1);

	return res;
#	endif // ~ implementation
}

//--    bit_count_trailing_zeros()          ///{{{2///////////////////////////
inline uint16_t bit_count_trailing_zeros( uint16_t aX ) noexcept
{
#	if FLUXTMP_UTIL_BIT_HAVE_TZCNT_
	return __tzcnt_u16( aX );

#	elif FLUX_COMPILER == FLUX_COMPILER_GCC || FLUX_COMPILER == FLUX_COMPILER_CLANG
	return aX ? __builtin_ctz( aX ) : 16;

#	elif FLUX_COMPILER == FLUX_COMPILER_MSVC
	unsigned long res;
	if( _BitScanForward( &res, aX ) )
		return uint16_t(res);

	return 16;

#	else // fallback
	if( !aX ) return 16;

	uint16_t res = 0;

	if( !(aX & 0x00ff) ) (res +=  8), (aX >>=  8);
	if( !(aX & 0x000f) ) (res +=  4), (aX >>=  4);
	if( !(aX & 0x0003) ) (res +=  2), (aX >>=  2);
	if( !(aX & 0x0001) ) (res +=  1);

	return res;

#	endif // ~ implementation
}
inline uint32_t bit_count_trailing_zeros( uint32_t aX ) noexcept
{
#	if FLUXTMP_UTIL_BIT_HAVE_TZCNT_
	return __tzcnt_u32( aX );

#	elif FLUX_COMPILER == FLUX_COMPILER_GCC || FLUX_COMPILER == FLUX_COMPILER_CLANG
	return aX ? __builtin_ctz( aX ) : 32;

#	elif FLUX_COMPILER == FLUX_COMPILER_MSVC
	unsigned long res;
	if( _BitScanForward( &res, aX ) )
		return res;

	return 32;

#	else // fallback
	if( !aX ) return 32;

	uint32_t res = 0;

	if( !(aX & 0x0000ffff) ) (res +=  16), (aX >>=  16);
	if( !(aX & 0x000000ff) ) (res +=  8), (aX >>=  8);
	if( !(aX & 0x0000000f) ) (res +=  4), (aX >>=  4);
	if( !(aX & 0x00000003) ) (res +=  2), (aX >>=  2);
	if( !(aX & 0x00000001) ) (res +=  1);

	return res;

#	endif // ~ implementation
}
inline uint64_t bit_count_trailing_zeros( uint64_t aX ) noexcept
{
#	if FLUXTMP_UTIL_BIT_HAVE_TZCNT_ && defined(__x86_64__)
	return __tzcnt_u64( aX );

#	elif FLUX_COMPILER == FLUX_COMPILER_GCC || FLUX_COMPILER == FLUX_COMPILER_CLANG
	return aX ? __builtin_ctzll( aX ) : 64;

#	elif FLUX_COMPILER == FLUX_COMPILER_MSVC && FLUX_ARCH == FLUX_ARCH_X86_64
	unsigned long res;
	if( _BitScanForward64( &res, aX ) )
		return res;

	return 64;

#	else // fallback
	if( !aX ) return 64;

	uint64_t res = 0;

	if( !(aX & 0x00000000ffffffff) ) (res +=  32), (aX >>=  32);
	if( !(aX & 0x000000000000ffff) ) (res +=  16), (aX >>=  16);
	if( !(aX & 0x00000000000000ff) ) (res +=  8), (aX >>=  8);
	if( !(aX & 0x000000000000000f) ) (res +=  4), (aX >>=  4);
	if( !(aX & 0x0000000000000003) ) (res +=  2), (aX >>=  2);
	if( !(aX & 0x0000000000000001) ) (res +=  1);

	return res;

#	endif // ~ implementation
}

//--    $ cleanup                           ///{{{2///////////////////////////

#undef FLUXTMP_UTIL_BIT_HAVE_LZCNT_
#undef FLUXTMP_UTIL_BIT_HAVE_TZCNT_
#undef FLUXTMP_UTIL_BIT_HAVE_POPCNT_
#undef FLUXTMP_UTIL_BIT_HAVE_MSVCPOPCNT_
#undef FLUXTMP_UTIL_BIT_HAVE_MSVCPOPCNT64_

//--    <<< ~ flux::util namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(util)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
