/*-******************************************************* -- HEADER -{{{1- */
/*-	DEFINE_ENUM_BITFIELD()
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_UTIL_ENUM_BITFIELD_HPP_275DD127_8632_4E5D_8EAA_80CF9BC0EB89
#define FLUX_UTIL_ENUM_BITFIELD_HPP_275DD127_8632_4E5D_8EAA_80CF9BC0EB89

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>

//--    >>> namespace = flux::util >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(util)
//--    DEFINE_ENUM_BITFIELD()              ///{{{2///////////////////////////

/** Define bitwise operators for `enum class`
 *
 * Define bitwise operators (`operator~`, `operator|`, `operator&`,
 * `operator^`, `operator|=`, `operator&=` and `operator^=`) for the `enum
 * class` \a enumClass. With this, \a enumClass becomes a
 * [BitmaskType](http://en.cppreference.com/w/cpp/concept/BitmaskType).
 *
 * Additionally defines `operator!` which makes the code using the bitmasks
 * a slight bit less messy.
 *
 * Example:
   \code
   enum class EEnum
   {
	   a = (1<<0),
	   b = (1<<1)
   };
  
   FLUX_UTIL_DEFINE_ENUM_BITFIELD( EEnum );
  
   //...
   EEnum flag = EEnum::a|EEnum::b;
   \endcode
 */
#define FLUX_UTIL_DEFINE_ENUM_BITFIELD(enumClass) /*IMPLEMENTED_ELSEWHERE*/

//--    <<< ~ flux::util namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(util)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/enum_bitfield.inl"
#endif // FLUX_UTIL_ENUM_BITFIELD_HPP_275DD127_8632_4E5D_8EAA_80CF9BC0EB89
