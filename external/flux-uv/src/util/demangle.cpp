/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Demangling implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/util/demangle.hpp>
#include <flux/setup/compiler.hpp>

#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC || FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#	include <cxxabi.h>
#	include <cstdlib> // free()
#endif // ~ compiler

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::util >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(util)
//--    demangle()                          ///{{{2///////////////////////////
std::string demangle( char const* aString )
{
	if( !aString )
		return std::string();

#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC || FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
	int status = 0;
	char* res = abi::__cxa_demangle( aString, nullptr, nullptr, &status );

	if( !res )
		return aString;

	std::string ret( res );
	free( res );

	return ret;

#	elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
	// MSVC prefixes the type name with e.g., "struct", or (presumably) "class"
	// skip it
	if( 0 == strncmp( "struct ", aString, 7 ) ) aString += 7;
	if( 0 == strncmp( "class ", aString, 6 ) ) aString += 6;

	return aString;
#	else // other compilers
	return aString;
#	endif // ~ COMPILER
}

//--    <<< ~ flux::util namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(util)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
