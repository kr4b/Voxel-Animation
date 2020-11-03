/*-******************************************************* -- HEADER -{{{1- */
/*-	Support: miniz.h error category. 
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_SUPPORT_MINIZ_CATEGORY_HPP_9A18EC37_65B9_494E_A34D_2BBADFA2D84B
#define FLUX_EXT_SUPPORT_MINIZ_CATEGORY_HPP_9A18EC37_65B9_494E_A34D_2BBADFA2D84B

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/system_error.hpp>

//--    error category              ///{{{1///////////////////////////////////
namespace flux_ext
{
	namespace support
	{
		flux::compat::error_category const& miniz_category() noexcept;
		flux::compat::error_code make_miniz_error_code( int ) noexcept;
	}
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_SUPPORT_MINIZ_CATEGORY_HPP_9A18EC37_65B9_494E_A34D_2BBADFA2D84B
