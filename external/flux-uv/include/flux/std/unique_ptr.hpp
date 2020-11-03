/*-******************************************************* -- HEADER -{{{1- */
/*-	unique_ptr<>
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_UNIQUE_PTR_HPP_42E08F25_2CE0_4A97_A6AA_136A688D6FA5
#define FLUX_STD_UNIQUE_PTR_HPP_42E08F25_2CE0_4A97_A6AA_136A688D6FA5

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>
#include <flux/compat/make_unique.hpp>

#include <memory>

//--    unique_ptr<>                ///{{{1///////////////////////////////////

FLUX_NAMESPACE_ENTER()
	using std::unique_ptr;
	using flux::compat::make_unique;
FLUX_NAMESPACE_LEAVE()

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_UNIQUE_PTR_HPP_42E08F25_2CE0_4A97_A6AA_136A688D6FA5
