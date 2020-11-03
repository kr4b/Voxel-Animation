/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	TimeSource implementations
 */
/*-***************************************************************** -}}}1- */

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    TimeSource [[ QUERYPC ]]           ///{{{2///////////////////////////
#if FLUX_SYS_TIMER == FLUX_SYS_TIMER_QUERYPC
inline TimeSource::TimeSource() noexcept
{
	read_freq( mFreqData );
	read_time( mStartTime );
}

inline bool TimeSource::valid() const noexcept
{
	LARGE_INTEGER freq, now;
	return !!QueryPerformanceFrequency( &freq ) && !!QueryPerformanceCounter( &now );
}
inline bool TimeSource::steady() const noexcept
{
	// See https://msdn.microsoft.com/en-us/library/dn553408(v=vs.85).aspx
	return true;
}

inline bool TimeSource::read_time( TimePoint& aNow ) const noexcept
{
	return !!QueryPerformanceCounter( &aNow );
}
inline bool TimeSource::read_freq( TimerFreqData& aFreq ) const noexcept
{
	return !!QueryPerformanceFrequency( &aFreq );
}

template< class tDuration > inline
tDuration TimeSource::elapsed() const noexcept
{
	TimePoint now;
	read_time( now );
	return delta<tDuration>( mStartTime, now );
}
template< class tDuration > inline
tDuration TimeSource::delta( TimePoint const& aA, TimePoint const& aB ) const noexcept
{
	LARGE_INTEGER delta;
	delta.QuadPart = aB.QuadPart - aA.QuadPart;

	return std::chrono::duration_cast<tDuration>( std::chrono::nanoseconds( 
		delta.QuadPart*detail::kNSecPerSec/mFreqData.QuadPart
	) );
}

template< class tDuration > inline
tDuration TimeSource::resolution() const noexcept
{
	return std::chrono::duration_cast<tDuration>( std::chrono::nanoseconds( 
		detail::kNSecPerSec/mFreqData.QuadPart
	) );
}
#endif // ~ FLUX_SYS_TIMER_QUERYPC

//--    TimeSource [[ CLOCKGETTIME ]]       ///{{{2///////////////////////////
#if FLUX_SYS_TIMER == FLUX_SYS_TIMER_CLOCKGETTIME
inline TimeSource::TimeSource() noexcept
	: mClockId( CLOCK_REALTIME )
{
	// Got monotonic clock?
	timespec now;
	if( 0 == clock_gettime( CLOCK_MONOTONIC, &now ) )
	{
		mClockId = CLOCK_MONOTONIC;
	}

	// read starting time
	read_time( mStartTime );
}

inline bool TimeSource::valid() const noexcept
{
	timespec now, res;
	return 0 == clock_gettime( mClockId, &now ) && 0 == clock_getres( mClockId, &res );
}
inline bool TimeSource::steady() const noexcept
{
	return mClockId == CLOCK_MONOTONIC;
}
inline bool TimeSource::read_time( TimePoint& aNow ) const noexcept
{
	return 0 == clock_gettime( mClockId, &aNow );
}
inline bool TimeSource::read_freq( TimerFreqData& aFreq ) const noexcept
{
	return 0 == clock_getres( mClockId, &aFreq );
}

template< class tDuration > inline
tDuration TimeSource::elapsed() const noexcept
{
	TimePoint now;
	read_time( now );
	return delta<tDuration>( mStartTime, now );
}
template< class tDuration > inline
tDuration TimeSource::delta( TimePoint const& aA, TimePoint const& aB ) const  noexcept
{
	return std::chrono::duration_cast<tDuration>( std::chrono::nanoseconds( 
		(aB.tv_sec-aA.tv_sec)*detail::kNSecPerSec + aB.tv_nsec - aA.tv_nsec
	) );
}

template< class tDuration > inline
tDuration TimeSource::resolution() const noexcept
{
	TimerFreqData res;
	read_freq( res );

	return std::chrono::duration_cast<tDuration>( std::chrono::nanoseconds( 
		res.tv_sec*detail::kNSecPerSec + res.tv_nsec
	) );
}
#endif // ~ FLUX_SYS_TIMER_CLOCKGETTIME

//--    TimeSource [[ MACHTIME ]]           ///{{{2///////////////////////////
#if FLUX_SYS_TIMER == FLUX_SYS_TIMER_MACHTIME
/* XXX-FIXME-XXX WARNING . This is totally and hilariously untested. */
#error "WARNING - untested code!"

inline TimeSource::TimeSource() noexcept
{
	read_time( mStartTime );
	read_freq( mFreqData );
}

inline bool TimeSource::valid() const noexcept
{
	//XXX-this might be terribly wrong
	mach_timebase_info_data_t data;
	return !!mach_timebase_info( &data );
}
inline bool TimeSource::steady() const noexcept
{
	// maybe? it's barely documented, except in about 1000 useless blog posts
	// that pollute the search results. :-(
	return false; // TODO-FIXME
}

inline bool TimeSource::read_time( TimePoint& aNow ) const noexcept
{
	mach_absolute_time( &aNow );
	return true;
}
inline bool TimeSource::read_freq( TimerFreqData& aFreq ) const noexcept
{
	return !!mach_timebase_info( &aFreq );
}

template< class tDuration > inline
tDuration TimeSource::elapsed() const noexcept
{
	TimePoint now;
	read_time( now );
	return delta<tDuration>( mStartTime, now );
}
template< class tDuration > inline
tDuration TimeSource::delta( TimePoint const& aA, TimePoint const& aB ) const noexcept
{
	return std::chrono::duration_cast<tDuration>( std::chrono::nanoseconds( 
		(aB-aA) * kNSecPerSec * mFreqData.denom / mFreqData.numer
	) );
}

template< class tDuration > inline
tDuration TimeSource::resolution() const noexcept
{
	return std::chrono::duration_cast<tDuration>( 
		std::chrono::nanoseconds( kNSecPerSec * mFreqData.denom / mFreqData.numer )
	);
}
#endif // ~ FLUX_SYS_TIMER_MACHTIME

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
