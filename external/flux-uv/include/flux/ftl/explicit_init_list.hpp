/*-******************************************************* -- HEADER -{{{1- */
/*-	Explicit Initializer List
 *
 * Based of the discussion relating to the C++11 std::initializer_list, and
 * the brace-elision problems. See
 *
 * https://groups.google.com/a/isocpp.org/forum/#!msg/sg14/Z_FVbk_bs58/WkZqO1pwAwAJ
 *
 * Using a wrapper to make the initializer list more explicit is suggested
 * in that discussion.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_EXPLICIT_INIT_LIST_HPP_4629B518_B3A0_4ED4_880C_5220746FFFB0
#define FLUX_FTL_EXPLICIT_INIT_LIST_HPP_4629B518_B3A0_4ED4_880C_5220746FFFB0

//--//////////////////////////////////////////////////////////////////////////
//--    includes            ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <initializer_list>

//--    >>> namespace = flux::ftl >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    ExplicitInitList                ///{{{2///////////////////////////////
/** Explicit initializer list
 *
 * `ExplicitInitList` wraps `std::initializer_list`, which requires the user
 * to type a second (explicit) set of braces for types that accept such a
 * list on construction. For example,
   \code
   SmallVector<float> list{ {1.0f, 2.0f, 3.0f, 4.0f} };
   \endcode
 * 
 * This avoids problems where a `std::initializer_list` constructor would be
 * preferred over other constructors when it is present and brace-construction
 * is used.
 */
template< typename tType >
struct ExplicitInitList
{
	constexpr inline
	ExplicitInitList( std::initializer_list<tType> aList )
		: ilist( aList )
	{}

	std::initializer_list<tType> ilist;
};

//--    <<< ~ flux::ftl namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_FTL_EXPLICIT_INIT_LIST_HPP_4629B518_B3A0_4ED4_880C_5220746FFFB0
