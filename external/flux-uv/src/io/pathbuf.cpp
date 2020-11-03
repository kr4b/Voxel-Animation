/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	SysPathBuf & helpers
 */
/*-***************************************************************** -}}}1- */

#include "pathbuf.hxx"

#include <flux/sys/syspath.hpp>
#include <flux/setup/platform.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io::detail >>>        ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,detail)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	constexpr sys::SysPathChar kTerm_ = '\0';
#	else // PLATFORM_WIN32
	constexpr sys::SysPathChar kTerm_ = L'\0';
#	endif // ~ PLATFORM

	void alloc_( SysPathBuf&, std::size_t, std::size_t );
}

//--    make_zpath()                     ///{{{2///////////////////////////
void make_zpath( SysPathBuf& aBuf, sys::SysPathView const& aPath, std::size_t aExtra )
{
	alloc_( aBuf, aPath.size(), aExtra );
	
	if( aPath.size() )
		std::memcpy( aBuf.data(), aPath.data(), sizeof(sys::SysPathChar)*aPath.size() );

	aBuf.back() = kTerm_;
}

//--    $ alloc_()                          ///{{{2///////////////////////////
namespace
{
	void alloc_( SysPathBuf& aBuf, std::size_t aSize, std::size_t aExtra )
	{
		if( aExtra )
			aBuf.reserve( aSize+1+aExtra );

		aBuf.resize( aSize+1 );
	}
}

//--    <<< ~ flux::io::detail namespace <<<        ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
