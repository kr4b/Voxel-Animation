/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- EBufferMap implementation
 */
/*-***************************************************************** -}}}1- */

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    EBufferMap                          ///{{{2///////////////////////////
inline constexpr
EBufferMap::EBufferMap( EBufferMapRO aRO ) noexcept
	: mValue( static_cast<unsigned>(aRO) )
{}

inline constexpr
EBufferMap::EBufferMap( EBufferMapRW aRW ) noexcept
	: mValue( static_cast<unsigned>(aRW) )
{}


inline constexpr
EBufferMap::operator EBufferMapRO() const noexcept
{
	return static_cast<EBufferMapRO>(mValue);
}
inline constexpr
EBufferMap::operator EBufferMapRW() const noexcept
{
	return static_cast<EBufferMapRW>(mValue);
}


inline constexpr
unsigned EBufferMap::value() const noexcept
{
	return mValue;
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
