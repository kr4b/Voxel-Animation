/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Dynamic Library loading implementation
 */
/*-***************************************************************** -}}}1- */

#include <type_traits>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    dylib_get_sym()                     ///{{{2///////////////////////////
template< typename tType > inline
tType dylib_get_sym( DylibHandle aHandle, char const* aName )
{
	using Type_ = typename std::remove_pointer<tType>::type;
	constexpr bool fn = std::is_member_function_pointer<tType>::value
		|| (std::is_pointer<tType>::value && std::is_function<Type_>::value)
	;

	if( fn )
		return reinterpret_cast<tType>( dylib_get_proc( aHandle, aName ) ); 

	return reinterpret_cast<tType>( dylib_get_var( aHandle, aName ) );
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
