#ifndef DETAIL_PATHBUF_HXX_325A9D37_36B6_4511_A392_33B8623F8AAA
#define DETAIL_PATHBUF_HXX_325A9D37_36B6_4511_A392_33B8623F8AAA

/* Helpers for creating nul-terminated paths that can be handed of to the 
 * system functions.
 */

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/system_error.hpp>

#include <flux/ftl/vector.hpp>
#include <flux/sys/syspath.hpp>
#include <flux/util/attrib.hpp>

//--    >>> namespace = flux::sys::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(sys,detail)
//--    types                               ///{{{2///////////////////////////

using SysPathBuf = ftl::Vector<SysPathChar, sizeof(SysPathChar)*128>;

//--    functions                           ///{{{2///////////////////////////

void make_zpath( 
	SysPathBuf&, 
	SysPathView const&,
	std::size_t = 0
);
FLUX_ATTR_PRECIOUS_RESULT
bool make_zpath( 
	SysPathBuf&, 
	SysPathView const&, 
	std::size_t, 
	compat::error_code& 
) noexcept;

void make_zpath( 
	SysPathBuf&, 
	SysPathView const&,
	bool,
	std::size_t
);
FLUX_ATTR_PRECIOUS_RESULT
bool make_zpath( 
	SysPathBuf&, 
	SysPathView const&,
	bool,
	std::size_t, 
	compat::error_code& 
) noexcept;

//--    <<< ~ flux::sys::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(sys,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_PATHBUF_HXX_325A9D37_36B6_4511_A392_33B8623F8AAA
