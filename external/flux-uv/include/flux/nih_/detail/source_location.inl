/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	source_location implementation
 */
/*-***************************************************************** -}}}1- */

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    source_location                     ///{{{2///////////////////////////
constexpr
source_location::source_location() noexcept
	: mLine(0)
	, mColumn(0)
	, mFile("unknown")
	, mFunc("unknown")
{}
constexpr
source_location::source_location( std::uint_least32_t aLine, std::uint_least32_t aColumn, char const* aFile, char const* aFunc ) noexcept
	: mLine(aLine)
	, mColumn(aColumn)
	, mFile(aFile)
	, mFunc(aFunc)
{}


constexpr
std::uint_least32_t source_location::line() const noexcept
{
	return mLine;
}
constexpr
std::uint_least32_t source_location::column() const noexcept
{
	return mColumn;
}

constexpr
char const* source_location::file_name() const noexcept
{
	return mFile;
}
constexpr
char const* source_location::function_name() const noexcept
{
	return mFunc;
}

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
