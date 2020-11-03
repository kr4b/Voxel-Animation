/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	uninitialized__* implementations
 */
/*-***************************************************************** -}}}1- */

#include <memory>
#include <iterator>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    uninitialized_move*()               ///{{{2///////////////////////////

template< typename tInput, typename tForward > inline
tForward uninitialized_move( tInput aBeg, tInput aEnd, tForward aOut )
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;

	tForward cur = aOut;
	try
	{
		for( ; aBeg != aEnd; ++aBeg, ++cur )
			::new (std::addressof(*cur)) Ty_(std::move(*aBeg));
	}
	catch( ... )
	{
		nih_::destroy( aOut, cur );
		throw;
	}

	return cur;
}
template< typename tInput, typename tForward > inline
std::pair<tInput,tForward> uninitialized_move_n( tInput aBeg, std::size_t aCount, tForward aOut )
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;

	tForward cur = aOut;
	try
	{
		for( std::size_t i = 0; i < aCount; ++i, ++aBeg, ++cur )
			::new (std::addressof(*cur)) Ty_(std::move(*aBeg));
	}
	catch( ... )
	{
		nih_::destroy( aOut, cur );
		throw;
	}

	return std::make_pair(aBeg,cur);
}

//--    uninitialized_value_construct*()    ///{{{2///////////////////////////

template< typename tForward > inline
void uninitialized_value_construct( tForward aBeg, tForward aEnd )
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;

	tForward cur = aBeg;
	try
	{
		for( ; cur != aEnd; ++cur )
			::new (std::addressof(*cur)) Ty_();
	}
	catch( ... )
	{
		nih_::destroy( aBeg, cur );
		throw;
	}
}
template< typename tForward > inline
tForward uninitialized_value_construct_n( tForward aBeg, std::size_t aCount )
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;

	tForward cur = aBeg;
	try
	{
		for( std::size_t i = 0; i < aCount; ++i, ++cur )
			::new (std::addressof(*cur)) Ty_();
	}
	catch( ... )
	{
		nih_::destroy( aBeg, cur );
		throw;
	}

	return cur;
}

//--    uninitialized_default_construct*()  ///{{{2///////////////////////////

template< typename tForward > inline
void uninitialized_default_construct( tForward aBeg, tForward aEnd )
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;

	tForward cur = aBeg;
	try
	{
		for( ; cur != aEnd; ++cur )
			::new (std::addressof(*cur)) Ty_;
	}
	catch( ... )
	{
		nih_::destroy( aBeg, cur );
		throw;
	}
}
template< typename tForward > inline
tForward uninitialized_default_construct_n( tForward aBeg, std::size_t aCount )
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;

	tForward cur = aBeg;
	try
	{
		for( std::size_t i = 0; i < aCount; ++i, ++cur )
			::new (std::addressof(*cur)) Ty_;
	}
	catch( ... )
	{
		nih_::destroy( aBeg, cur );
		throw;
	}

	return cur;
}

//--    destroy()                           ///{{{2///////////////////////////

template< typename tForward > inline
void destroy( tForward aBeg, tForward aEnd )
{
	for( ; aBeg != aEnd; ++aBeg )
		nih_::destroy_at( std::addressof(*aBeg) );
}
template< typename tForward > inline
tForward destroy_n( tForward aBeg, std::size_t aCount )
{
	for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
		nih_::destroy_at( std::addressof(*aBeg) );

	return aBeg;
}
template< typename tType > inline
void destroy_at( tType* aPtr )
{
	aPtr->~tType();
}

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
