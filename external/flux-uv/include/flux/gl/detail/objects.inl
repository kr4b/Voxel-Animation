/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- UniqueObj<> implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/compat/exchange.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    UniqueObj<>                         ///{{{2///////////////////////////

template< void tDeleter(GL::UInt) noexcept > inline
UniqueObj<tDeleter>::~UniqueObj()
{
	if( 0 != id )
		tDeleter( id );
}

template< void tDeleter(GL::UInt) noexcept > FLUX_CONSTEXPR_EX inline
UniqueObj<tDeleter>::UniqueObj( GL::UInt aProgram ) noexcept
	: id( aProgram )
{}

template< void tDeleter(GL::UInt) noexcept > FLUX_CONSTEXPR_EX inline
UniqueObj<tDeleter>::UniqueObj( UniqueObj&& aOther ) noexcept
	: id( compat::exchange( aOther.id, 0 ) )
{}
template< void tDeleter(GL::UInt) noexcept > FLUX_CONSTEXPR_EX inline
auto UniqueObj<tDeleter>::operator= (UniqueObj&& aOther ) noexcept -> UniqueObj&
{
	std::swap( aOther.id, id );
	return* this;
}

template< void tDeleter(GL::UInt) noexcept > FLUX_CONSTEXPR_EX inline
UniqueObj<tDeleter>::operator GL::UInt&() noexcept
{
	return id;
}
template< void tDeleter(GL::UInt) noexcept > FLUX_CONSTEXPR_EX inline
UniqueObj<tDeleter>::operator GL::UInt const&() const noexcept
{
	return id;
}

template< void tDeleter(GL::UInt) noexcept > FLUX_CONSTEXPR_EX inline
GL::UInt* UniqueObj<tDeleter>::operator&() noexcept
{
	return &id;
}

template< void tDeleter(GL::UInt) noexcept > FLUX_CONSTEXPR_EX inline
GL::UInt const* UniqueObj<tDeleter>::operator&() const noexcept
{
	return &id;
}

template< void tDeleter(GL::UInt) noexcept > inline
void UniqueObj<tDeleter>::reset()
{
	if( 0 != id )
	{
		tDeleter( id );
		id = 0;
	}
}

template< void tDeleter(GL::UInt) noexcept > FLUX_CONSTEXPR_EX inline
bool UniqueObj<tDeleter>::valid() const noexcept
{
	return 0 != id;
}
template< void tDeleter(GL::UInt) noexcept > FLUX_CONSTEXPR_EX inline
GL::UInt UniqueObj<tDeleter>::yield() noexcept
{
	return compat::exchange( id, 0 );
}

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
