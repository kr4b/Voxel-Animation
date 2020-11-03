/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Signal, take 2
 */
/*-***************************************************************** -}}}1- */

#include <flux/ftl/signal.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    SignalLink                          ///{{{2///////////////////////////
SignalLink::SignalLink( shared_ptr<detail::SignalAnchor> aAnchor, detail::SignalId aId )
	: mId( aId )
	, mAnchor( aAnchor )
{}

void SignalLink::clear() noexcept
{
	mAnchor.reset();
}
void SignalLink::disconnect()
{
	if( auto sp = mAnchor.lock() )
		sp->disconnect( sp->signal, mId );

	clear();
}

//--    AutoSignalLink                      ///{{{2///////////////////////////
AutoSignalLink::~AutoSignalLink()
{
	mLink.disconnect();
}

AutoSignalLink::AutoSignalLink( SignalLink&& aLink ) noexcept
	: mLink( std::move(aLink) )
{}
AutoSignalLink& AutoSignalLink::operator= (SignalLink&& aLink) noexcept
{
	mLink = std::move(aLink);
	return *this;
}

AutoSignalLink::AutoSignalLink( SignalLink const& aLink ) noexcept
	: mLink( aLink )
{}
AutoSignalLink& AutoSignalLink::operator= (SignalLink const& aLink) noexcept
{
	mLink = aLink;
	return *this;
}

AutoSignalLink::AutoSignalLink( detail::SignalLinkProxy const& aLink ) noexcept
	: AutoSignalLink( aLink.operator SignalLink() )
{}
AutoSignalLink& AutoSignalLink::operator= (detail::SignalLinkProxy const& aLink) noexcept
{
	*this = aLink.operator SignalLink();
	return *this;
}


void AutoSignalLink::clear() noexcept
{
	mLink.clear();
}
void AutoSignalLink::disconnect()
{
	mLink.disconnect();
}

SignalLink AutoSignalLink::reset( SignalLink aOther ) noexcept
{
	std::swap( aOther, mLink );
	return aOther;
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
