/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	SysPathBuf & helpers
 */
/*-***************************************************************** -}}}1- */

#include "pathbuf.hxx"

#include <flux/sys/syspath.hpp>
#include <flux/setup/platform.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(sys,detail)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	constexpr SysPathChar kTerm_ = '\0';
#	else // PLATFORM_WIN32
	constexpr SysPathChar kTerm_ = L'\0';
#	endif // ~ PLATFORM

	void alloc_( SysPathBuf&, std::size_t, std::size_t );
	bool alloc_( SysPathBuf&, std::size_t, std::size_t, compat::error_code& ) noexcept;

	std::size_t zpath_req_len_(
		SysPathView const&,
		bool aWin32Fixups
	) noexcept;
	void zpath_encode_(
		SysPathChar*,
		std::size_t aMaxLength,
		SysPathView const&,
		bool aWin32Fixups
	) noexcept;
}

//--    make_zpath()                     ///{{{2///////////////////////////
void make_zpath( SysPathBuf& aBuf, SysPathView const& aPath, std::size_t aExtra )
{
	alloc_( aBuf, aPath.size(), aExtra );
	
	if( aPath.size() )
		std::memcpy( aBuf.data(), aPath.data(), sizeof(SysPathChar)*aPath.size() );

	aBuf.back() = kTerm_;
}

bool make_zpath( SysPathBuf& aBuf, SysPathView const& aPath, std::size_t aExtra, compat::error_code& aErrorOut ) noexcept
{
	if( !alloc_( aBuf, aPath.size(), aExtra, aErrorOut ) )
		return false;
	
	if( aPath.size() )
		std::memcpy( aBuf.data(), aPath.data(), sizeof(SysPathChar)*aPath.size() );

	aBuf.back() = kTerm_;
	return true;
}

void make_zpath( SysPathBuf& aBuf, SysPathView const& aPath, bool aWin32Fixups, std::size_t aExtra )
{
	auto const req = zpath_req_len_( aPath, aWin32Fixups );
	alloc_( aBuf, req, aExtra );
	
	zpath_encode_( aBuf.data(), req+1, aPath, aWin32Fixups );
	aBuf.back() = kTerm_;
}

bool make_zpath( SysPathBuf& aBuf, SysPathView const& aPath, bool aWin32Fixups, std::size_t aExtra, compat::error_code& aErrorOut ) noexcept
{
	auto const req = zpath_req_len_( aPath, aWin32Fixups );
	if( !alloc_( aBuf, req, aExtra, aErrorOut ) )
		return false;

	zpath_encode_( aBuf.data(), req+1, aPath, aWin32Fixups );
	aBuf.back() = kTerm_;
	return true;
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
	bool alloc_( SysPathBuf& aBuf, std::size_t aSize, std::size_t aExtra, compat::error_code& aErrorOut ) noexcept try
	{
		if( aExtra )
			aBuf.reserve( aSize+1+aExtra );

		aBuf.resize( aSize+1 );
		return true;
	}
	catch( std::bad_alloc const& )
	{
		aErrorOut = compat::make_error_code( compat::errc::not_enough_memory );
		return false;
	}
}

//--    $ zpath_*()                         ///{{{2///////////////////////////
namespace
{
	std::size_t zpath_req_len_( SysPathView const& aPath, bool aWin32Fixups ) noexcept
	{
		auto required = aPath.size();

#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		(void)aWin32Fixups;
#		else // PLATFORM_WIN32
		if( aWin32Fixups )
		{
			// If we just as for a drive, _wstat() will return an error. Fix
			// this here, for simplicity elsewhere. The different flavours to
			// handle are:
			//   - X:
			//   - \\?\X:
			// (XXX-others?)
			bool appendedBS = false;
			std::size_t extras = 0;
			if( 2 == aPath.size() && L':' == aPath[1] )
			{
				extras += 1; // extra backslash
				appendedBS = true;
			}
			else if( 6 == aPath.size() && L'\\' == aPath[0] && L'\\' == aPath[1] && L'?' == aPath[2] && L'\\' == aPath[3] && L':' == aPath[5] )
			{
				extras += 1; // extra backslash
				appendedBS = true;
			}
	
			// If the last character is a '\', _wstat() will be confused UNLESS
			// we're looking at a drive root directory in the new fancy "\\?\"
			// style, in which case it'll get confused if there is a ".". 
			bool appendedDot = false;
			if( !aPath.empty() && (L'\\' == aPath.back() || appendedBS) )
			{
				appendedDot = true;
				extras += 1; // extra dot
			}

			if( 8 == (aPath.size()+extras) && L'\\' == aPath[0] && L'\\' == aPath[1] && L'?' == aPath[2] && L'\\' == aPath[3] && L':' == aPath[5] && (appendedBS || L'\\' == aPath[6]) && (appendedDot || L'.' == aPath[7]) )
			{
				extras -= 1;
			}

			required += extras;
		}
#		endif // ~ PLATFORM_WIN32

		return required;
	}
	void zpath_encode_( SysPathChar* aDst, std::size_t aMaxLen, SysPathView const& aPath, bool aWin32Fixups ) noexcept
	{
		FLUX_ASSERT( aPath.size() <= aMaxLen );
		
		if( aPath.size() )
			std::memcpy( aDst, aPath.data(), sizeof(SysPathChar)*aPath.size() );

		std::size_t len = aPath.size();

#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		(void)aWin32Fixups;

		FLUX_ASSERT( len < aMaxLen );
		aDst[len] = '\0';
#		else // ~ PLATFORM_WIN32
		if( aWin32Fixups )
		{
			// See above (zpath_required_len_())
			if( 2 == len && L':' == aDst[1] )
			{
				FLUX_ASSERT( len < aMaxLen );
				aDst[len++] = L'\\';
			}
			else if( 6 == len && L'\\' == aDst[0] && L'\\' == aDst[1] && L'?' == aDst[2] && L'\\' == aDst[3] && L':' == aDst[5] )
			{
				FLUX_ASSERT( len < aMaxLen );
				aDst[len++] = L'\\';
			}

			if( 0 != len && L'\\' == aDst[len] )
			{
				FLUX_ASSERT( len < aMaxLen );
				aDst[len++] = L'.';
			}

			if( 8 == len && L'\\' == aDst[0] && L'\\' == aDst[1] && L'?' == aDst[2] && L'\\' == aDst[3] && L':' == aDst[5] && L'\\' == aDst[6] && L'.' == aDst[7] )
			{
				--len;
			}
		}

		FLUX_ASSERT( len < aMaxLen );
		aDst[len] = L'\0';
#		endif // ~ PLATFORM_WIN32
	}
}

//--    <<< ~ flux::sys::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(sys,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
