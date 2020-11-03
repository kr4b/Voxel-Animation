/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Path manipulation
 *
 * Dragons be here.
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/pathutil.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>
#include <flux/std/except/einfo.hpp>

#include <flux/sys/fsquery.hpp>
#include <flux/sys/fsmanip.hpp>
#include <flux/sys/errors.hpp>
#include <flux/sys/errno_category.hpp>
#include <flux/sys/win32_category.hpp>

#include <flux/util/narrow.hpp>
#include <flux/util/switch.hpp>

#include <flux/ftl/vector.hpp>

#include <sstream>

#if FLUX_PLATFORM == FLUX_PLATFORM_LINUX
#	include <cwchar>

#	include <stdio.h>
#	include <unistd.h>
#	include <libgen.h>
#	include <fcntl.h>
#	include <cstring>
#	include <sys/stat.h>
#	include <sys/types.h>
#elif FLUX_PLATFORM == FLUX_PLATFORM_WIN32
#	include <io.h>
#	include <fcntl.h>
#	include <wchar.h>
#	include <flux/sys/win32/windows.h>
#else // other platform
#	error "flux-sys [paths]: Unsupported platform / not implemented yet"
#endif  // ~ FLUX_PLATFORM

#include "pathbuf.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	//-/
	//-/ types -{{{3-
	struct SysPathRef_
	{
		SysPathChar const* beg;
		SysPathChar const* end;
		bool isBack;
	};

	using GenPathBuf_ = ftl::Vector< char, sizeof(char)*128 >;
	using SysPathStack_ = ftl::Vector<SysPathRef_, sizeof(SysPathRef_)*12>;
	
	//-/
	//-/ constants -{{{3-
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#		define FLUXTMP_SYS_CHAR_(x) x
#		define FLUXTMP_SYS_STR_(x) x
#	else // PLATFORM_WIN32
#		define FLUXTMP_SYS_CHAR_(x) L##x
#		define FLUXTMP_SYS_STR_(x) L##x
#	endif // ~ PLATFORM

	//constexpr SysPathChar kNullTerm_ = FLUXTMP_SYS_CHAR_('\0');
	constexpr SysPathChar kGenericSep_ = FLUXTMP_SYS_CHAR_('/');

#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	SysPathChar const* const kSelfDir_ = "./";
	SysPathChar const* const kPrevDir_ = "..";
#	else // PLATFORM_WIN32
	SysPathChar const* const kSelfDir_ = L".\\";
	SysPathChar const* const kPrevDir_ = L"..";
#	endif // ~ PLATFORM

	//-/
	//-/ functions -{{{3-
	std::string path_to_utf8_( SysPathView const& );
	detail::SysPathBuf path_from_utf8_( compat::string_view const& );

	bool is_self_( SysPathRef_ const& );
	bool is_prev_( SysPathRef_ const& );

	SysPathChar const* skip_root_prefix_( SysPathChar const*, SysPathChar const* );
	SysPathChar const* skip_garbage_( SysPathChar const*, SysPathChar const* );
}

//--    path_to_utf8()                      ///{{{2///////////////////////////
namespace
{
	std::string path_to_utf8_( SysPathView const& aPath )
	{
		if( aPath.empty() )
			return std::string();

#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		return std::string( aPath );

#		else // PLATFORM_WIN32
		auto const req = ::WideCharToMultiByte( CP_UTF8, 0, aPath.data(), util::narrow<int>(aPath.size()), 0, 0, 0, 0 );
		FLUX_THROW_IF( 0 == req, error::FsError )
			<< einfo::ApiError( make_win32_error_code() )
			<< einfo::ApiFunction( "::WideCharToMultiByte" )
		;

		FLUX_ASSERT( req > 0 );
		GenPathBuf_ buf( req+1, 0 );
		
		auto rr = ::WideCharToMultiByte( CP_UTF8, 0, aPath.data(), util::narrow<int>(aPath.size()), buf.data(), req+1, 0, 0 );
		FLUX_THROW_IF( 0 == rr, error::FsError )
			<< einfo::ApiError( make_win32_error_code() )
			<< einfo::ApiFunction( "::WideCharToMultiByte" )
		;
			
		return std::string( buf.data(), req );
#		endif // ~ PLATFORM
	}
}

std::string path_to_utf8( SysPathView const& aPath )
{
	if( aPath.empty() )
		return std::string();

#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	return std::string(aPath);
#	else // PLATFORM_WIN32
	return path_to_utf8_( aPath);
#	endif // ~ PLATFORM
}

//--    path_to_generic()                   ///{{{2///////////////////////////
std::string path_to_generic( SysPathView const& aPath )
{
	if( aPath.empty() )
		return std::string();

	auto beg = aPath.data();
	auto const end = aPath.data()+aPath.size();

	detail::SysPathBuf buf;
	while( beg != end )
	{
		auto next = beg;
		while( next != end && *next != kSysPathSeparator && *next != kGenericSep_ )
			++next;

		buf.insert( buf.end(), beg, next );

		if( kSysPathSeparator == *next )
		{
			buf.emplace_back( kGenericSep_ );
			++next;
		}
		else if( kGenericSep_ == *next )
		{
			buf.emplace_back( kSysPathSeparator );
			++next;
		}

		beg = next;
	}

	//buf.emplace_back( kNullTerm_ );
	return path_to_utf8_( SysPathView( buf.data(), buf.size() ) );
}

//--    path_from_utf8()                    ///{{{2///////////////////////////
namespace
{
	detail::SysPathBuf path_from_utf8_( compat::string_view const& aPath )
	{
		if( aPath.empty() )
			return detail::SysPathBuf();

#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		return detail::SysPathBuf( aPath.begin(), aPath.end() );

#	else // PLATFORM_WIN32
		auto const req = ::MultiByteToWideChar( CP_UTF8, 0, aPath.data(), util::narrow<int>(aPath.size()), 0, 0 );
		FLUX_THROW_IF( 0 == req, error::FsError )
			<< einfo::ApiError( make_win32_error_code() )
			<< einfo::ApiFunction( "::MultiByteToWideChar" )
		;

		FLUX_ASSERT( req > 0 );
		detail::SysPathBuf buf( req, 0 );
		auto rr = ::MultiByteToWideChar( CP_UTF8, 0, aPath.data(), util::narrow<int>(aPath.size()), buf.data(), req );
		FLUX_THROW_IF( 0 == rr, error::FsError )
			<< einfo::ApiError( make_win32_error_code() )
			<< einfo::ApiFunction( "::MultiByteToWideChar" )
		;

		return buf;
#	endif // ~ PLATFORM
	}
}

SysPathStr path_from_utf8( compat::string_view const& aPath )
{
	if( aPath.empty() )
		return SysPathStr();

#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	return SysPathStr( aPath );

#	else // PLATFORM_WIN32
	auto buf = path_from_utf8_( aPath );
	return SysPathStr( buf.data(), buf.size() );
#	endif // ~ PLATFORM
}
/*SysPathStr path_from_utf8( std::string const& aPath )
{
	return path_from_utf8( aPath.c_str() );
}*/

//--    path_from_generic()                 ///{{{2///////////////////////////
SysPathStr path_from_generic( compat::string_view const& aPath )
{
	if( aPath.empty() )
		return SysPathStr();

	auto const path = path_from_utf8_( aPath );

	auto beg = path.data();
	auto const end = path.data()+path.size();

	detail::SysPathBuf buf;
	while( beg != end )
	{
		auto next = beg;
		while( next != end && *next != kGenericSep_ && *next != kSysPathSeparator )
			++next;

		buf.insert( buf.end(), beg, next );

		if( next != end && kGenericSep_ == *next )
		{
			buf.emplace_back( kSysPathSeparator );
			++next;
		}
		else if( next != end && kSysPathSeparator == *next )
		{
			buf.emplace_back( kGenericSep_ );
			++next;
		}

		beg = next;
	}

	return SysPathStr( buf.begin(), buf.end() );
}
/*SysPathStr path_from_generic( std::string const& aPath )
{
	return path_from_generic( aPath.c_str() );
}*/

//--    path_to_wchar()                     ///{{{2///////////////////////////
std::basic_string<wchar_t> path_to_wchar( SysPathStr const& aPath )
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	std::mbstate_t mbs = std::mbstate_t{};
	char const* str = aPath.c_str();
	auto req = std::mbsrtowcs( nullptr, &str, 0, &mbs ) + 1;

	ftl::Vector<wchar_t> buff( req, L'\0' );
	mbs = std::mbstate_t{};
	str = aPath.c_str();

	std::mbsrtowcs( buff.data(), &str, buff.size(), &mbs );
	return std::basic_string<wchar_t>( buff.begin(), buff.end() );
	
#	else // PLATFORM_WCHAR
	return aPath;
#	endif // ~ PLATFORM_WIN32
}

//--    path_from_wchar()                   ///{{{2///////////////////////////
SysPathStr path_from_wchar( wchar_t const* aStr )
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	std::mbstate_t mbs = std::mbstate_t{};
	wchar_t const* str = aStr;
	auto req = std::wcsrtombs( nullptr, &str, 0, &mbs ) + 1;

	ftl::Vector<char> buff( req, '\0' );
	mbs = std::mbstate_t{};
	str = aStr;

	std::wcsrtombs( buff.data(), &str, buff.size(), &mbs );
	return SysPathStr( buff.begin(), buff.end() );
	
#	else // PLATFORM_WCHAR
	return SysPathStr(aStr);
#	endif // ~ PLATFORM_WIN32
}
SysPathStr path_from_wchar( std::basic_string<wchar_t> const& aStr )
{
	return path_from_wchar( aStr.c_str() );
}

//--    path_lexical_join()                 ///{{{2///////////////////////////
SysPathStr path_lexical_join( SysPathView const& aLeft, SysPathView const& aRight )
{
	if( aLeft.empty() )
		return SysPathStr(aRight);
	else if( aRight.empty() )
		return SysPathStr(aLeft);
	
	detail::SysPathBuf buf;

	auto const* lbeg = aLeft.data();
	auto const* lend = aLeft.data()+aLeft.size();

	FLUX_ASSERT( lbeg != lend );
	//res.write( lbeg, lend-lbeg );
	buf.insert( buf.end(), lbeg, lend );

	if( kSysPathSeparator != *(lend-1) )
		buf.emplace_back( kSysPathSeparator );

	auto const* rbeg = aRight.data();
	auto const* rend = aRight.data()+aRight.size();
	if( rbeg != rend && kSysPathSeparator == *rbeg )
		++rbeg;
	
	if( rbeg != rend )
		buf.insert( buf.end(), rbeg, rend );

	return SysPathStr( buf.begin(), buf.end() );
}

//--    path_lexical_parent()               ///{{{2///////////////////////////
SysPathStr path_lexical_parent( SysPathView const& aPath, int aSteps, bool aRestrict )
{
	/* XXX-maybe: merge with lexical_normalize()? */

	FLUX_THROW_IF( aSteps > 0, error::PathInvalidRequest )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("aSteps = {:d} must be < 0"), aSteps )
	;
	
	auto const* beg = aPath.data();
	auto const* end = aPath.data()+aPath.size();
	
	SysPathRef_ prefix{ nullptr, nullptr, false };
	auto rootPrefix = skip_root_prefix_( beg, end );
	if( rootPrefix != beg )
	{
		prefix.beg = beg;
		prefix.end = rootPrefix;
		beg = skip_garbage_( rootPrefix, end );
	}

	SysPathStack_ segments;
	while( beg != end )
	{
		auto nbeg = skip_garbage_( beg, end );
		auto nend = std::find( nbeg, end, kSysPathSeparator );

		if( nbeg != nend )
		{
			segments.emplace_back( SysPathRef_{ nbeg, nend, false } );
		}

		beg = nend;
	}


	while( !segments.empty() && aSteps < 0 )
	{
		auto const& last = segments.back();
		if( is_prev_( last ) )
			break;

		if( !is_self_( last ) )
			++aSteps;

		segments.pop_back();
	}


	if( prefix.beg == prefix.end || !aRestrict )
	{
		for( ; aSteps < 0; ++aSteps )
		{
			segments.emplace_back( SysPathRef_{ kPrevDir_, kPrevDir_+2, true } );
		}
	}

	detail::SysPathBuf buf;
	if( prefix.beg != prefix.end )
	{
		buf.insert( buf.end(), prefix.beg, prefix.end );

		if( prefix.end[-1] != kSysPathSeparator && !segments.empty() )
			buf.push_back( kSysPathSeparator );
	}

	if( segments.empty() && prefix.beg == prefix.end )
		buf.emplace_back( kPrevDir_[0] );
	
	for( auto const& seg : segments )
	{
		buf.insert( buf.end(), seg.beg, seg.end );
		buf.emplace_back( kSysPathSeparator );
	}

	if( !segments.empty() ) buf.pop_back();

	return SysPathStr( buf.begin(), buf.end() );
}

//--    path_lexical_normalize()            ///{{{2///////////////////////////
SysPathStr path_lexical_normalize( SysPathView const& aPath, bool aRestrict )
{
	if( aPath.empty() )
		return SysPathStr();
	
	auto const* beg = aPath.data();
	auto const* end = aPath.data()+aPath.size();

	SysPathRef_ prefix{ nullptr, nullptr, false };
	auto rootPrefix = skip_root_prefix_( beg, end );
	if( rootPrefix != beg )
	{
		prefix.beg = beg;
		prefix.end = rootPrefix;
		beg = skip_garbage_( rootPrefix, end );
	}

	bool const restr = (prefix.beg != prefix.end) && aRestrict;
	bool const trailingSep = (beg != end && kSysPathSeparator == *(end-1));

	if( trailingSep )
		--end;

	SysPathStack_ segments;
	while( beg != end )
	{
		auto nbeg = skip_garbage_( beg, end );
		auto nend = std::find( nbeg, end, kSysPathSeparator );

		beg = nend;

		if( nbeg != nend )
		{
			SysPathRef_ seg{ nbeg, nend, false };

			if( is_self_( seg ) )
				continue;

			bool const prev = is_prev_( seg );
			if( prev && !segments.empty() && !is_prev_( segments.back() ) )
			{
				segments.pop_back();
			}
			else
			{
				if( !restr || !prev )
					segments.emplace_back( std::move(seg) );
			}
		}
	}

	detail::SysPathBuf buf;

	std::size_t prefixLen = 0;
	if( prefix.beg != prefix.end )
	{
		buf.insert( buf.end(), prefix.beg, prefix.end );
		prefixLen = prefix.end - prefix.beg;

		if( prefix.end[-1] != kSysPathSeparator )
		{
			buf.emplace_back( kSysPathSeparator );
			++prefixLen;
		}
	}
	else if( segments.empty() )
	{
		buf.emplace_back( kSelfDir_[0] );
	}

	for( auto const& seg : segments )
	{
		buf.insert( buf.end(), seg.beg, seg.end );
		buf.emplace_back( kSysPathSeparator );
	}

	FLUX_ASSERT( !buf.empty() );
	bool const endsOnSep = (kSysPathSeparator == buf.back());

	if( !endsOnSep && trailingSep ) buf.emplace_back( kSysPathSeparator );
	else if( endsOnSep && !trailingSep && buf.size() > prefixLen ) buf.pop_back();

	return SysPathStr( buf.begin(), buf.end() );
}

//--    path_lexical_is_absolute()          ///{{{2///////////////////////////
bool path_lexical_is_absolute( SysPathView const& aPath )
{
	return skip_root_prefix_( aPath.data(), aPath.data()+aPath.size() ) != aPath.data();
}

//--    path_make_absolute()                ///{{{2///////////////////////////
SysPathStr path_make_absolute( SysPathStr const& aPath )
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	char* res = ::realpath( aPath.c_str(), nullptr );
	FLUX_THROW_IF( !res, error::FsError )
		<< einfo::ApiError( make_errno_error_code() )
		<< einfo::ApiFunction( "::realpath" )
	;

	SysPathStr ret( res );
	::free( res );
	return ret;

#	else // PLATFORM_WIN32
	/* Unfortunately, setting the last argument to zero will always fail? */
	wchar_t* res = _wfullpath( nullptr, aPath.c_str(), 32767 );
	FLUX_THROW_IF( !res, error::FsError )
		<< einfo::ApiError( make_errno_error_code() )
		<< einfo::ApiFunction( "::_wfullpath" )
	;
	
	SysPathStr ret( res );
	::free( res );
	return ret;
#	endif // ~ PLATFORM_WIN32
}

//--    l: is_*_()                          ///{{{2///////////////////////////
namespace
{
	//TODO: FIXME.
	bool is_self_( SysPathRef_ const& aSeg )
	{
		return (aSeg.end-aSeg.beg == 1) && kSelfDir_[0] == *aSeg.beg;
	}
	bool is_prev_( SysPathRef_ const& aSeg )
	{
		return (aSeg.end-aSeg.beg == 2)
			&& kPrevDir_[0] == aSeg.beg[0] 
			&& kPrevDir_[1] == aSeg.beg[1]
		;
	}
}

//--    l: skip_root_prefix_()              ///{{{2///////////////////////////
namespace
{
	SysPathChar const* skip_root_prefix_( SysPathChar const* aBeg, SysPathChar const* aEnd )
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		if( aBeg != aEnd && '/' == *aBeg )
			++aBeg;

		return aBeg;

#		else // PLATFORM_WIN32
		/* We need to consider the following prefixes:
		 *
		 *  - \ (*)
		 *  - X:\, for x in [a-zA-Z]
		 *  - \\host, \\host\share for host and share as [a-zA-Z]
		 *  - \\?\X:\
		 *  - \\?\UNC\host, \\?\UNC\host\share
		 *  - \\.\
		 *
		 * (*) Win32 refers to this as an absolute path, even though this is relative
		 * to the current drive.
		 */
		auto const checkDrive = [aEnd] (SysPathChar const* aCur) -> SysPathChar const* {
			if( aEnd - aCur < 3 )
				return nullptr;

			auto const d = aCur[0];
			if( ((d >= L'a' && d <= L'z') || (d >= L'A' && d <= L'Z')) && L':' == aCur[1] && L'\\' == aCur[2] )
				return aCur + 3;

			return nullptr;
		};
		auto const checkShare = [aEnd] (SysPathChar const* aCur) -> SysPathChar const* {
			auto ptr = aCur;

			while( ptr != aEnd && L'\\' != *ptr )
				++ptr;

			auto qtr = ptr;
			if( qtr != aEnd && L'\\' == *qtr )
				++qtr;

			while( qtr != aEnd && L'\\' != *qtr )
				++qtr;

			if( ptr+1 == qtr )
				--qtr;

			return qtr;
		};

		if( auto drive = checkDrive( aBeg ) )
			return drive;

		auto ptr = aBeg;
		if( ptr != aEnd && L'\\' == *ptr )
		{
			++ptr;

			if( L'\\' == ptr[0] && L'?' == ptr[1] && L'\\' == ptr[2] )
			{
				if( auto drive = checkDrive( ptr+3 ) )
					return drive;

				if( L'U' == ptr[3] && L'N' == ptr[4] && L'C' == ptr[5] && L'\\' == ptr[6] )
					return checkShare( ptr + 7 );
			}
			else if( L'\\' == ptr[0] )
			{
				return checkShare( ptr+1 );
			}
		}

		return ptr;
#		endif
	}
}

//--    l: skip_garbage_()                  ///{{{2///////////////////////////
namespace
{
	SysPathChar const* skip_garbage_( SysPathChar const* aBeg, SysPathChar const* aEnd )
	{
		while( aBeg != aEnd && kSysPathSeparator == *aBeg )
			++aBeg;

		return aBeg;
	}
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
