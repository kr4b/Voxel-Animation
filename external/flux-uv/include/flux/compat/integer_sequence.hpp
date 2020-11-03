/*-******************************************************* -- HEADER -{{{1- */
/*-	std::integer_sequence<> et al.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_INTEGER_SEQUENCE_HPP_DFFF4921_FFFC_4348_A6F9_CE6AACB4A548
#define FLUX_COMPAT_INTEGER_SEQUENCE_HPP_DFFF4921_FFFC_4348_A6F9_CE6AACB4A548

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>

//--    integer_sequence<>          ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX14_INTEGER_SEQ
#	include <utility>

FLUX_NAMESPACE_ENTER1(compat)
	using std::index_sequence;
	using std::integer_sequence;
	
	using std::make_index_sequence;
	using std::make_integer_sequence;
FLUX_NAMESPACE_LEAVE1(compat)

#else // fallback
#	include <flux/nih_/integer_sequence.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::index_sequence;
	using nih_::integer_sequence;
	
	using nih_::make_index_sequence;
	using nih_::make_integer_sequence;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_INTEGER_SEQUENCE_HPP_DFFF4921_FFFC_4348_A6F9_CE6AACB4A548
