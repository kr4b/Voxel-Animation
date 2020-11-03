/*-******************************************************* -- HEADER -{{{1- */
/*-	VFS paths
 *
 * TODO
 * TODO
 * TODO
 * TODO
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VPATH_HPP_1442CF58_8120_46C5_AB19_02C401CC2687
#define FLUX_IO_VPATH_HPP_1442CF58_8120_46C5_AB19_02C401CC2687

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/string_view.hpp>

#include "forward.hpp"

#include <tuple>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    constants                       ///{{{2///////////////////////////////


//--    types                           ///{{{2///////////////////////////////

//--    functions                       ///{{{2///////////////////////////////

bool vpath_is_absolute( compat::string_view const& ) noexcept;
bool vpath_is_relative( compat::string_view const& ) noexcept;

bool vpath_escapes( compat::string_view const&, std::string* = nullptr );

std::string vpath_simplify( 
	compat::string_view const&,
	bool aRestrict = false
);

std::string vpath_join( 
	compat::string_view const&,
	compat::string_view const&,
	bool aSimplify = true,
	bool aRestrict = false
);

// Note: ignores trailing '/'
compat::string_view vpath_name( compat::string_view const& ) noexcept;
compat::string_view vpath_path( compat::string_view const& ) noexcept;

compat::string_view vpath_skip_common(
	compat::string_view const&,
	compat::string_view const&
) noexcept;


compat::string_view vpath_element_first( 
	compat::string_view const&,
	bool aSkipRootElement = true
) noexcept;

std::tuple<compat::string_view,compat::string_view> vpath_split_first(
	compat::string_view const&,
	bool aSkipRootElement = true
) noexcept;
	
#if 0
std::string vpath_parent(
	compat::string_view const&,
	int = -1,
	bool aRestrict = false
);

std::tuple<compat::string_view,compat::string_view>
vpath_split_last( compat::string_view const& ) noexcept;
std::tuple<compat::string_view,compat::string_view>
vpath_split_first( compat::string_view const& ) noexcept;
#endif

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_VPATH_HPP_1442CF58_8120_46C5_AB19_02C401CC2687
