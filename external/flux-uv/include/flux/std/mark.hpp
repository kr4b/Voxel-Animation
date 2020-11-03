/*-******************************************************* -- HEADER -{{{1- */
/*-	Mark<>
 *
 * Used to implement the "attorney-client" or "badge" pattern. Uses the term
 * "Mark" because reasons (shadowrun).
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_MARK_HPP_111F4ACF_3001_4415_AA2C_5F1950F43704
#define FLUX_STD_MARK_HPP_111F4ACF_3001_4415_AA2C_5F1950F43704

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>


//--    >>> namespace = flux >>>                    ///{{{1///////////////////
FLUX_NAMESPACE_ENTER()
//--    Mark                                ///{{{2///////////////////////////

template< class tAccessor >
class Mark
{
	friend tAccessor;

	constexpr Mark() noexcept {}
	constexpr Mark( Mark const& ) noexcept {}
	constexpr Mark& operator= (Mark const&) const noexcept { return *this; }
};

//TODO: multimark

//--    <<< ~ flux: namespace <<<                   ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE()
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_MARK_HPP_111F4ACF_3001_4415_AA2C_5F1950F43704
