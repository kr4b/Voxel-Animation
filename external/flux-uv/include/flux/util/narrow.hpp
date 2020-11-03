/*-******************************************************* -- HEADER -{{{1- */
/*-	Safely narrow stuff
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_UTIL_NARROW_HPP_C10FC475_2494_4387_8ED1_B520F5867783
#define FLUX_UTIL_NARROW_HPP_C10FC475_2494_4387_8ED1_B520F5867783

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>

#include <flux/std/optional.hpp>
#include <flux/std/stdexcept.hpp>


//--    >>> namespace = flux::util >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(util)
//--    narrow_cast()                       ///{{{2///////////////////////////

template< typename tTarget, typename tSource >
tTarget narrow_cast( tSource&& ) noexcept;

//--    narrow()                            ///{{{2///////////////////////////

template< typename tTarget, typename tSource >
tTarget narrow( tSource );

template< typename tTarget, typename tSource >
optional<tTarget> narrow( tSource, compat::error_code& ) noexcept;

//--    errors                              ///{{{2///////////////////////////
namespace error
{
	struct NarrowError : flux::error::LogicError
	{};
};

//--    <<< ~ flux::util namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(util)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/narrow.inl"
#endif // FLUX_UTIL_NARROW_HPP_C10FC475_2494_4387_8ED1_B520F5867783
