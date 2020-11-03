/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	make_unique implementation
 */
/*-***************************************************************** -}}}1- */

#include <utility>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    make_unique()                       ///{{{2///////////////////////////
template< typename tType, typename... tArgs > inline
FLUX_DISABLE_IF( std::is_array<tType> )
(std::unique_ptr<tType>) make_unique( tArgs&&... aArgs )
{
	return std::unique_ptr<tType>( new tType( std::forward<tArgs>(aArgs)... ) );
}

template< typename tType > inline
FLUX_ENABLE_IF_C( std::is_array<tType>::value && (0 == std::extent<tType>::value) )
(std::unique_ptr<tType>) make_unique( std::size_t aCount )
{
	using BaseType_ = typename std::remove_extent<tType>::type;
	return std::unique_ptr<tType>( new BaseType_[aCount]() );
}

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
