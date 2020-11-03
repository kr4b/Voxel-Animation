/*-******************************************************* -- HEADER -{{{1- */
/*-	Styling for stdio
 *
 * Helpers for default styling in standard output.
 *
 * TODO: make EStdioStyleScheme that is customizable
 * TODO: load customizable from .config by default??
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_STDIO_STYLE_HPP_EA252B95_1169_49B9_B3FE_87834BC2F498
#define FLUX_SYS_STDIO_STYLE_HPP_EA252B95_1169_49B9_B3FE_87834BC2F498

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/sys/stdio.hpp>

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    constants                   ///{{{1///////////////////////////////////

enum class EStdioFmt
{
	plain,
	bold,
	bright = bold,
	light,
	italic,
	underline,
	max
};

enum class EStdioSemanticFmt
{
	plain,
	heading,
	paragraph,
	emph,
	note,
	max
};

// TODO-docs : might not correspond to actual colors if RGB is unsupported
// and if the user has remapped the default color maps. Will use RGB or 8bit
// if available.
enum class EStdioColor
{
	plain,
	black,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan,
	white,
	max
};
enum class EStdioSemanticColor
{
	plain,
	
	// Increasing importance
	attn,
	warn,
	error,

	// Decreasing importance
	hint,
	note,
	aside,

	// Yes, No
	ack,
	nak,

	// Neutral categories
	// XXX??
	cat0,
	cat1,
	cat2,
	cat3,

	max
};

enum class EStdioStyleScheme
{
	muted,
	full,
	max
};

//--    types                       ///{{{1///////////////////////////////////

struct StdioStyleReset {};

//--    functions                   ///{{{1///////////////////////////////////

char const* stdio_style( StdioStyleReset, StdioCaps const* = nullptr ) noexcept;

char const* stdio_style( EStdioFmt, StdioCaps const* = nullptr ) noexcept;
char const* stdio_style( EStdioColor, StdioCaps const* = nullptr ) noexcept;

char const* stdio_style( EStdioSemanticFmt, StdioCaps const* = nullptr ) noexcept;
char const* stdio_style( EStdioSemanticColor, StdioCaps const* = nullptr ) noexcept;

EStdioStyleScheme stdio_style_config( EStdioStyleScheme ) noexcept;

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 

#endif // FLUX_SYS_STDIO_STYLE_HPP_EA252B95_1169_49B9_B3FE_87834BC2F498
