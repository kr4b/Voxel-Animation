/*-******************************************************* -- HEADER -{{{1- */
/*-	source_location fallback
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_SOURCE_LOCATION_HPP_C70D2AEA_58FD_4BC7_B817_3C2F537C66DA
#define FLUX_NIH_SOURCE_LOCATION_HPP_C70D2AEA_58FD_4BC7_B817_3C2F537C66DA

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
//#include <flux/setup/compiler.hpp>


#include <cstdint>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    config                      ///{{{1///////////////////////////////////

#define FLUX_NIH_SOURCE_LOCATION_IS_STUB 1

//--    source_location             ///{{{1///////////////////////////////////

/** Fallback for `std::experimental::source_location`
 *
 * Non-conforming fallback for `std::experimental::source_location`. Only
 * provides sensible values on GCC 4.8 and later at the moment -- other and
 * earlier compiles do not provide the neccessary builtins. 
 *
 * Furthermore, `current()` doesn't conform to the spec 100% either (it 
 * must take the values as default arguments). 
 *
 * There might be other quirks.
 *
 * Also see https://gcc.gnu.org/ml/gcc-patches/2015-11/msg01687.html.
 */
struct source_location
{
	constexpr source_location() noexcept;
	constexpr source_location( std::uint_least32_t, std::uint_least32_t, char const*, char const* ) noexcept;

	constexpr std::uint_least32_t line() const noexcept;
	constexpr std::uint_least32_t column() const noexcept;

	constexpr char const* file_name() const noexcept;
	constexpr char const* function_name() const noexcept;

#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8))
#		undef FLUX_NIH_SOURCE_LOCATION_IS_STUB
#		define FLUX_NIH_SOURCE_LOCATION_IS_STUB 0

	static constexpr source_location current( std::uint_least32_t aLine =
	__builtin_LINE(), std::uint_least32_t aColumn = 0, char const* aFile =
	__builtin_FILE(), char const* aFunc = __builtin_FUNCTION() ) noexcept
	{
		return source_location{ aLine, aColumn, aFile, aFunc };
	}
#	else
	static constexpr source_location current() noexcept
	{
		return source_location{};
	}
#	endif // ~ COMPILER

	private:
		std::uint_least32_t mLine;
		std::uint_least32_t mColumn;
		char const* mFile;
		char const* mFunc;
};

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/source_location.inl"
#endif // FLUX_NIH_SOURCE_LOCATION_HPP_C70D2AEA_58FD_4BC7_B817_3C2F537C66DA
