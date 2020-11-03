#ifndef DETAIL_PATHBUF_HXX_37A1B5F0_633E_46A1_87AF_4B0A50C2E9CE
#define DETAIL_PATHBUF_HXX_37A1B5F0_633E_46A1_87AF_4B0A50C2E9CE

/* Helpers for creating nul-terminated paths that can be handed of to the 
 * system functions.
 *
 * For now, somewhat copy-pasta from flux-sys.
 */

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/ftl/vector.hpp>
#include <flux/sys/syspath.hpp>

//--    >>> namespace = flux::io::detail >>>        ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,detail)
//--    types                               ///{{{2///////////////////////////

using SysPathBuf = ftl::Vector<sys::SysPathChar, sizeof(sys::SysPathChar)*128>;

//--    functions                           ///{{{2///////////////////////////

void make_zpath( 
	SysPathBuf&, 
	sys::SysPathView const&,
	std::size_t = 0 
);

//--    <<< ~ flux::io::detail namespace <<<        ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(it,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_PATHBUF_HXX_37A1B5F0_633E_46A1_87AF_4B0A50C2E9CE
