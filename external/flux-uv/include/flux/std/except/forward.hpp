/*-******************************************************* -- HEADER -{{{1- */
/*-	Forward definitions
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_EXCEPT_FORWARD_HPP_4F9BF7A6_1F71_4159_ACD8_345797BDC221
#define FLUX_STD_EXCEPT_FORWARD_HPP_4F9BF7A6_1F71_4159_ACD8_345797BDC221

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

//--    >>> namespace = flux::error >>>             ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(error)
//--    forward                             ///{{{2///////////////////////////

class Exception;

namespace detail
{
	class ExceptData;
	ExceptData& exception_data( Exception const& );
}

//--    <<< ~ flux::error namespace <<<             ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(error)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_EXCEPT_FORWARD_HPP_4F9BF7A6_1F71_4159_ACD8_345797BDC221
