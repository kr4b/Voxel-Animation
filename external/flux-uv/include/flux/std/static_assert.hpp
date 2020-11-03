/*-******************************************************* -- HEADER -{{{1- */
/*-	FLUX_STATIC_ASSERT
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_STATIC_ASSERT_HPP_7683AC67_71B3_46C5_BAE7_2E534D011225
#define FLUX_STD_STATIC_ASSERT_HPP_7683AC67_71B3_46C5_BAE7_2E534D011225

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/langfeat.hpp>

//--    FLUX_STATIC_ASSERT          ///{{{1///////////////////////////////////

#if FLUX_LANGFEAT_SHORT_STATIC_ASSERT
#	define FLUX_STATIC_ASSERT(...) static_assert(__VA_ARGS__)
#else
#	define FLUX_STATIC_ASSERT(...) static_assert(__VA_ARGS__,#__VA_ARGS__)
#endif // ~ FLUX_LANGFEAT_STATIC_ASSERT

//--    FLUX_STATIC_ASSERTM         ///{{{1///////////////////////////////////

#define FLUX_STATIC_ASSERTM(expr,msg) static_assert( expr, msg )

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_STATIC_ASSERT_HPP_7683AC67_71B3_46C5_BAE7_2E534D011225
