/*-******************************************************* -- HEADER -{{{1- */
/*-	Helpers for guarding code against bugs
 *
 * - Strong typedefs
 * - not_null<>
 * - ...
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_GUARDED_HPP_13ADC0E4_0BF7_44D8_89AD_5318BCB3BA04
#define FLUX_STD_GUARDED_HPP_13ADC0E4_0BF7_44D8_89AD_5318BCB3BA04

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/ftl/not_null.hpp>
#include <flux_ext/strong_typedef.hpp>

//--    guards                      ///{{{1///////////////////////////////////

FLUX_NAMESPACE_ENTER()
	// not_null
	using ftl::not_null;
	using ftl::not_null_unsafe;

	using ftl::make_not_null;
	using ftl::make_not_null_unsafe;

	// strong_typedef
	using flux_ext::type_safe::strong_typedef;
	namespace strong_typedef_op = flux_ext::type_safe::strong_typedef_op;

	template< typename tStrongTypedef>
	using strong_typedef_hash = flux_ext::type_safe::hashable<tStrongTypedef>;

	//// out<>: annotate output parameters.
	//template< typename tType >
	//using out = tType;
FLUX_NAMESPACE_LEAVE()

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_GUARDED_HPP_13ADC0E4_0BF7_44D8_89AD_5318BCB3BA04
