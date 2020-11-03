/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Hardware clock implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/hwclock.hpp>

#include <flux/std/log.hpp>
#include <flux/std/clock.hpp>

#include <flux/setup/platform.hpp>
#include <flux/util/scope_exit.hpp>

#include <mutex>
#include <atomic>
#include <limits>

#if FLUX_PLATFORM == FLUX_PLATFORM_LINUX
#	include <cstdio>
#	include <cinttypes>
#endif

#if FLUX_ARCH == FLUX_ARCH_X86_64
/* TODO: move this to general helpers for dealing with 128 bit numbers... */
#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#		include <intrin.h>
#		pragma intrinsic(_umul128)
#		pragma intrinsic(__shiftright128)
#	else // !MSVC
#		include <cstddef>
#		if !defined(__SIZEOF_INT128__)
#			error "Currently require int128 on GCC/Clang"
#		endif
#	endif // ~ COMPILER
#endif // ~ ARCH

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
#	if FLUX_ARCH == FLUX_ARCH_X86_32 || FLUX_ARCH == FLUX_ARCH_X86_64
	struct HwClockData_
	{
		/* This is roughly based on the method used in the Linux kernel[1]:
		 *
		 * ns = ticks / frequency * nsPerSec
		 * ns = ticks / frequency * 1e9
		 * ns = ticks / frequencyInKhz * 1e6
		 *
		 * Rescaling with a power of two:
		 * ns = ticks * (1e6 * s / frequencyInKhz) / s
		 * ns = ticks * scale / s   with   scale = 1e6*s/frequencyInKhz
		 *
		 * Larger s => better results. We do 128-bit intermediate values, so
		 * we're not as constrained in our choices as the implementaiton in the
		 * linux kernel (that maybe changed after the original comments in the
		 * kernel were written?). With
		 *   frequencyInKhz = 3696014 (22 bits) and 1e6 => 19 bits
		 * we can use something like
		 *   s = 2^40
		 * and still have reasonable numbers of bits everywhere. (This might be
		 * a tiny teensy bit of overkill...)
		 *
		 * [1] https://github.com/torvalds/linux/blob/16f73eb02d7e1765ccab3d2018e0bd98eb93d973/arch/x86/kernel/tsc.c
		 */
		static constexpr uint64_t shift = 40;

		uint64_t scale;
		uint64_t freq;

		EHwClockState state;
	};

	constexpr std::size_t kTimeReadRetry_ = 8;
	constexpr std::size_t kPeriodDoublings_ = 10;
	constexpr uint64_t kInitialPeriodNs_ = 660*1000;

	struct RdtscPair_
	{
		uint64_t tsc;
		Clock::time_point time;
	};

	void rdtsc_setup_spin_sleep_( std::uint64_t ) noexcept;

	RdtscPair_ rdtsc_time_pair_() noexcept;
	uint64_t rdtsc_measure_freq_() noexcept;
	uint64_t rdtsc_measure_freq_once_( std::uint64_t ) noexcept;

	EHwClockState rdtsc_init_() noexcept;

#	else // Unknown architecture
	struct HwClockData_
	{};
#	endif // ~ ARCH

	HwClockData_ gClockData_;

	std::atomic<bool> gClockInit_;
	std::mutex gClockInitMut_;
}

//--    hwclock_init()                      ///{{{2///////////////////////////
EHwClockState hwclock_init( bool aForce ) noexcept
{
	if( gClockInit_.load() && !aForce )
		return gClockData_.state;

#	if FLUX_ARCH == FLUX_ARCH_X86_32 || FLUX_ARCH == FLUX_ARCH_X86_64
	return rdtsc_init_();
#	else // unknown architecture
	/* nothing */
	FLUX_LOG( WARNING, "hwclock_init(): no hardware clock on this system" );
	return EHwClockState::error;
#	endif // ~ architecture
}

//--    hwclock_to_ns()                     ///{{{2///////////////////////////
uint64_t hwclock_to_ns( HwClockTicks aTicks ) noexcept
{
	if( !gClockInit_.load() )
	{
		hwclock_init();

		//TODO
		//TODO  badness - should be initialized already. add msg if unreliable
		//TODO
	}

#	if FLUX_ARCH == FLUX_ARCH_X86_64
#		if FLUX_COMPILER_FAMILY != FLUX_COMPILER_MSVC
	using UInt128_ = unsigned __int128;
	auto res = static_cast<UInt128_>(aTicks) * static_cast<UInt128_>(gClockData_.scale);
	return static_cast<uint64_t>(res >> gClockData_.shift);
#		else // COMPILER_MSVC
	uint64_t hi;
	uint64_t lo = _umul128( aTicks, gClockData_.scale, &hi );
	return __shiftright128( lo, hi, char(gClockData_.shift) );
#		endif // ~ COMPILER
#	else // unknown architecture
	// See https://stackoverflow.com/a/46924301
	auto const ticksHi = aTicks >> 32;
	auto const ticksLo = aTicks & 0xffffffffu;
	auto const scaleHi = gClockData_.scale >> 32;
	auto const scaleLo = gClockData_.scale & 0xffffffffu;

	auto const hilo = ticksHi * scaleLo;
	auto const lolo = ticksLo * scaleLo;
	auto const lohi = ticksLo * scaleHi;
	auto const hihi = ticksHi * scaleHi;

	auto const sum = hilo + lohi;
	auto const scarry = uint64_t(!!(sum < hilo));

	auto const plo = lolo + (sum << 32);
	auto const pcarry = uint64_t(!!(plo < lolo));

	auto const phi = hihi + (sum >> 32) + (scarry << 32) + pcarry;

	//if( gClockData_.shift < 64 )
		return (plo >> gClockData_.shift) | (phi << (64-gClockData_.shift));
	//else
	//	return (phi >> (gClockData_.shift-64));
#	endif // ~ architecture
}

//--    hwclock_frequency_in_khz()          ///{{{2///////////////////////////
uint64_t hwclock_frequency_in_khz() noexcept
{
	if( !gClockInit_.load() )
	{
		hwclock_init();

		//TODO
		//TODO  badness - should be initialized already. add msg if unreliable
		//TODO
	}

#	if FLUX_ARCH == FLUX_ARCH_X86_32 || FLUX_ARCH == FLUX_ARCH_X86_64
	return gClockData_.freq;
#	else // unknown architecture
	return 0;
#	endif // ~ architecture
}

//--    $ x86/x86-64 TSC                    ///{{{2///////////////////////////
namespace
{
	/* On x86/x86-64, we use the rdtscp instruction. This returns some value
	 * that is sometimes related to the CPU frequency and sometimes not.
	 *
	 * We follow the routine from the Abseil library here:
	 *   - Check if the magic non-standard `tsc_freq_khz` is exposed by the
	 *     kernel (Linux only).
	 *   - Otherwise, perform a calibration routine that relies on the standard
	 *     clocks. This may take a few milliseconds.
	 */
#	if FLUX_ARCH == FLUX_ARCH_X86_32 || FLUX_ARCH == FLUX_ARCH_X86_64
	void rdtsc_setup_spin_sleep_( std::uint64_t aSleepNs ) noexcept
	{
		/* Spin for approx aSleepNs. This is a workaround for missing nanosleep
		 * on Windows (and to avoid this_thread::sleep_for(), which is
		 * increadibly unreliable in some quick tests).
		 *
		 * Note: I use it on linux as well -- it seems to work well, and that
		 * keeps various differences down.
		 */
		auto const until = Clock::now() + std::chrono::nanoseconds(aSleepNs);
		while( Clock::now() < until )
			;
	}

	RdtscPair_ rdtsc_time_pair_() noexcept
	{
		/* Similar to Abseil (see base/internal/sysinfo.cc), perform a few
		 * measurements and pick the one where reading took the shortest time.
		 * This minimizes the probability that the thread was preempted between
		 * the reads.
		 *
		 * TODO-maybe: Technically, we could actually check for preemtions.
		 */
		using Duration_ = decltype((Clock::time_point{} - Clock::time_point{}).count());
		
		RdtscPair_ best;
		auto duration = std::numeric_limits<Duration_>::max();

		for( std::size_t i = 0; i < kTimeReadRetry_; ++i )
		{
			auto const a = Clock::now();
			auto const t = hwclock_now();
			auto const b = Clock::now();

			auto const dt = (b-a).count();
			if( dt < duration )
			{
				best.time = a; // /*hmm...*/ + (b-a)/2;
				best.tsc = t;
				duration = dt;
			}
		}

		return best;
	}

	uint64_t rdtsc_measure_freq_once_( std::uint64_t aSleepNs ) noexcept
	{
		/* Similar to Abseil (see base/internal/sysinfo.cc) take two measurements
		 * approximately aSleepNs nanoseconds apart. Derive frequency from that.
		 */
		auto const beg = rdtsc_time_pair_();
		rdtsc_setup_spin_sleep_( aSleepNs );
		auto const end = rdtsc_time_pair_();

		auto const ticks = end.tsc - beg.tsc;
		auto const dt = end.time - beg.time;

		// TODO-maybe: do this in 128bit math? I don't think that this should overflow
		// with the current settings, but thinking is hard ...

		return uint64_t(1000000) * ticks / std::chrono::duration_cast<std::chrono::nanoseconds>(dt).count();
	}

	uint64_t rdtsc_measure_freq_() noexcept
	{
		/* Similar to Abseil (see base/internal/sysinfo.cc) take multiple
		 * measurements with increasing sleep periods. Abseil accepts an
		 * measurement when it repeats once with an error of less than 1%.
		 * Note sure about that, but let's try it for now.
		 *
		 * Use 0.78% as the cutoff; this makes it a bit easier to stick with
		 * integers.
		 */
		auto period = kInitialPeriodNs_;
		uint64_t lastFreqKhz = ~uint64_t(0);
		for( std::size_t i = 0; i < kPeriodDoublings_; ++i )
		{
			auto const freq = rdtsc_measure_freq_once_( period );
			auto const err = freq >> 7;
			
			if( freq - err < lastFreqKhz && freq + err > lastFreqKhz )
				return freq;

			lastFreqKhz = freq;
			period *= 2;
		}

		return lastFreqKhz;
	}

#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
	void rdtsc_cpuid_( int* aData, int aCmd ) noexcept
	{
		__cpuid( aData, aCmd );
	}
#	else // !MSVC
	void rdtsc_cpuid_( int* aData, int aCmd ) noexcept
	{
		__asm__(
			"cpuid"
			: "=a"(aData[0]), "=b"(aData[1]), "=c"(aData[2]), "=d"(aData[3])
			: "a"(aCmd)
		);
	}
#	endif // ~ COMPILER_FAMILY

	EHwClockState rdtsc_init_() noexcept
	{
		// Check first if rdtscp is available
		int cid[4];

		rdtsc_cpuid_( cid, 0x80000001 );
		if( !(cid[3] & (1<<27)) )
		{
			std::unique_lock<std::mutex> lock(gClockInitMut_);
			gClockData_.state = EHwClockState::error;
			gClockInit_.store( true );

			return EHwClockState::error;
		}

		// Check if the TSC is constant/invariant
		auto state = EHwClockState::success;

		rdtsc_cpuid_( cid, 0x80000007 ); // invariant a.k.a. "constant_tsc"
		if( !(cid[3] & (1<<8)) )
			state = EHwClockState::unreliable;

		// nonstop_tsc flag - do we care? Probably not, don't expect this to be
		// used across sleeps/suspensions.

#		if FLUX_PLATFORM == FLUX_PLATFORM_LINUX
		/* Assume that there is a cpu0 in the system... */
		if( auto fl = std::fopen( "/sys/devices/system/cpu/cpu0/tsc_freq_khz", "r" ) )
		{
			FLUX_UTIL_ON_SCOPE_EXIT
			{
				std::fclose( fl );
			};

			uint64_t freqKhz = 0; char dummy[2];
			if( 1 == std::fscanf( fl, "%" SCNu64 "%1[^\n\r]", &freqKhz, dummy ) )
			{
				std::unique_lock<std::mutex> lock(gClockInitMut_);
				gClockData_.scale = (1000000ull << gClockData_.shift) / freqKhz;
				gClockData_.freq = freqKhz;
				gClockData_.state = state;
				gClockInit_.store( true );
				return state;
			}
		}
#		endif // ~ PLATFORM_LINUX

		// Check if cpuid exposes the TSC frequency
		// In theory: 
		//    - cpuid(0).eax  => highest CPUID leaf. Check that it's larger 
		//    	than 21 == 0x15
		//    - cpuid(21) => ecx, ebx, eax
		//       - ecx = nominal core crystal clock frequency
		//       - ebx/eax = TSC/"core crystal clock" ratio
		//      => TSC frequency == uint64_t(ecx) * ebx / eax 
		// However:
		//    - ebx = 0 => TSC/"core crystal clock" not enumerated
		//    - ecx = 0 => nominal core crystal clock frequency not enumerated
		//
		// If ecx == 0, instead usa a value derived from the family and model
		// numbers, as per instructions from some crusty Intel manual^W^W^W^W
		// the internet.
		rdtsc_cpuid_( cid, 0 );
		if( cid[0] >= 21 )
		{
			rdtsc_cpuid_( cid, 21 );
			auto const eax = cid[0];
			auto const ebx = cid[1];
			auto const ecx = cid[2];
			

			auto const cfreq = [&] {
				// Don't divide by eax=zero; better to just fail this part and
				// use the measurements.
				if( 0 == eax )
					return uint64_t(0);

				// If ecx is non-zero, that tells us the base frequency...
				if( 0 != ecx )
					return uint64_t(ecx) * ebx / eax;

				// ... otherwise, use a "well known" base frequency based on
				// the family / model.
				rdtsc_cpuid_( cid, 1 );

				auto family = (cid[0] >> 8) & 0xf;
				auto mod = (cid[0] >> 4) & 0xf;
				if( 15 == family || 6 == family )
					mod += ((cid[0] >> 16) & 0xf) << 4;
				if( 15 == family )
					family += (cid[0] >> 20) & 0xff;

				if( 6 == family )
				{
					if( 0x4e == mod || 0x5e == mod || 0x8e == mod || 0x9e == mod )
						return uint64_t(24)*1000*1000*ebx/eax;
				}

				// base frequency onknown.
				return uint64_t(0);
			}();

			// Zero: family/model wasn't known at the time of writing, or ebx
			// was zero, i.e. the "core crystal clock" ratio wasn't enumerated.
			// Or CPUID returned somethat that was zero.
			if( cfreq )
			{
				std::unique_lock<std::mutex> lock(gClockInitMut_);
				gClockData_.scale = (1000000ull << gClockData_.shift) / (cfreq/1000);
				gClockData_.freq = (cfreq/1000);
				gClockData_.state = state;
				gClockInit_.store( true );
				return state;
			}
		}

		// "Measure" the frequency
		auto const freq = rdtsc_measure_freq_();
		{
			std::unique_lock<std::mutex> lock(gClockInitMut_);
			gClockData_.scale = (1000000ull << gClockData_.shift) / freq;
			gClockData_.freq = freq;
			gClockData_.state = state;
			gClockInit_.store( true );
		}

		return state;
	}
#	endif // ~ x86-like
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
