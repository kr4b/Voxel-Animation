/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Vfs paths implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vpath.hpp>
#include <flux/ftl/vector.hpp>


//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	using VPathBuf_ = ftl::Vector< char, sizeof(char)*64 >;

	struct VPathRef_
	{
		char const* beg;
		char const* end;
		bool isBack;
	};

	char const* skip_seps_( char const*, char const* ) noexcept;

	bool seg_is_parent_( VPathRef_ const& ) noexcept;
	bool seg_is_selfdir_( VPathRef_ const& ) noexcept;
}

//--    vpath_is_*()                        ///{{{2///////////////////////////
bool vpath_is_absolute( compat::string_view const& aPath ) noexcept
{
	return !aPath.empty() && '/' == aPath[0];
}
bool vpath_is_relative( compat::string_view const& aPath ) noexcept
{
	return !vpath_is_absolute( aPath );
}

//--    vpath_escapes()                     ///{{{2///////////////////////////
bool vpath_escapes( compat::string_view const& aPath, std::string* aSimplified )
{
	auto sp = vpath_simplify( aPath, false );
	
	auto ptr = sp.data();
	auto len = sp.size();
	if( len && '/' == *ptr ) ++ptr, --len;

	bool const ret = len >= 2
		&& '.' == ptr[0]
		&& '.' == ptr[1]
		&& (len == 2 || '/' == ptr[2])
	;

	if( aSimplified ) *aSimplified = std::move(sp);
	return ret;
}

//--    vpath_simplify()                    ///{{{2///////////////////////////
std::string vpath_simplify( compat::string_view const& aPath, bool aRestrict )
{
	if( aPath.empty() )
		return std::string();

	auto const* beg = aPath.data();
	auto const* end = aPath.data()+aPath.size();

	bool const isAbs = ('/' == *beg);
	if( isAbs ) ++beg;

	bool const trailingSep = (beg != end && '/' == *(end-1));
	if( trailingSep ) --end;

	ftl::Vector< VPathRef_, sizeof(VPathRef_)*8 > segments;
	while( beg != end )
	{
		auto nbeg = skip_seps_( beg, end );
		auto nend = std::find( nbeg, end, '/' );

		beg = nend;

		if( nbeg != nend )
		{
			VPathRef_ seg{ nbeg, nend, false };

			if( seg_is_selfdir_( seg ) )
				continue;

			bool append = false;
			if( (seg.isBack = seg_is_parent_( seg )) )
			{
				if( !segments.empty() && !segments.back().isBack )
					segments.pop_back();
				else if( !aRestrict )
					append = true;
			}
			else
			{
				append = true;
			}

			if( append )
				segments.emplace_back( std::move(seg) );
		}
	}

	VPathBuf_ buf;
	std::size_t prefixLength = 0;

	if( isAbs )
	{
		buf.push_back( '/' );
		prefixLength = 1;
	}
	else if( segments.empty() )
	{
		buf.push_back( '.' );
	}

	for( auto const& seg : segments )
	{
		buf.insert( buf.end(), seg.beg, seg.end );
		buf.emplace_back( '/' );
	}

	FLUX_ASSERT( !buf.empty() );
	bool const endsOnSep = ('/' == buf.back());

	if( !endsOnSep && trailingSep ) buf.emplace_back( '/' );
	else if( endsOnSep && !trailingSep && !(isAbs && prefixLength == buf.size()) ) buf.pop_back();

	return std::string( buf.begin(), buf.end() );
}

//--    vpath_join()                        ///{{{2///////////////////////////
std::string vpath_join( compat::string_view const& aLeft, compat::string_view const& aRight, bool aSimplify, bool aRestrict )
{
	if( aLeft.empty() )
		return aSimplify ? vpath_simplify( aRight, aRestrict ) : std::string(aRight);
	else if( aRight.empty() )
		return aSimplify ? vpath_simplify( aLeft, aRestrict ) : std::string(aLeft);
	
	if( !aSimplify )
	{
		VPathBuf_ buf;
		auto const* lbeg = aLeft.data();
		auto const* lend = aLeft.data()+aLeft.size();

		FLUX_ASSERT( lbeg != lend );
		buf.insert( buf.end(), lbeg, lend );

		if( '/' != *(lend-1) )
			buf.emplace_back( '/' );

		auto const* rbeg = aRight.data();
		auto const* rend = aRight.data()+aRight.size();
		if( rbeg != rend && '/' == *rbeg )
			++rbeg;
		
		if( rbeg != rend )
			buf.insert( buf.end(), rbeg, rend );

		return std::string( buf.begin(), buf.end() );
	}

	auto const* lbeg = aLeft.data();
	auto const* lend = aLeft.data()+aLeft.size();

	bool const isAbs = ('/' == *lbeg);
	if( isAbs ) ++lbeg;

	auto const* rbeg = aRight.data();
	auto const* rend = aRight.data()+aRight.size();

	bool const trailingSep = ('/' == *(rend-1));
	if( trailingSep ) --rend;

	ftl::Vector< VPathRef_, sizeof(VPathRef_)*8 > segments;
	while( lbeg != lend )
	{
		auto nbeg = skip_seps_( lbeg, lend );
		auto nend = std::find( nbeg, lend, '/' );

		lbeg = nend;

		if( nbeg != nend )
		{
			VPathRef_ seg{ nbeg, nend, false };

			if( seg_is_selfdir_( seg ) )
				continue;

			bool append = false;
			if( (seg.isBack = seg_is_parent_( seg )) )
			{
				if( !segments.empty() && !segments.back().isBack )
					segments.pop_back();
				else if( !aRestrict )
					append = true;
			}
			else
			{
				append = true;
			}

			if( append )
				segments.emplace_back( std::move(seg) );
		}
	}
	while( rbeg != rend )
	{
		auto nbeg = skip_seps_( rbeg, rend );
		auto nend = std::find( nbeg, rend, '/' );

		rbeg = nend;

		if( nbeg != nend )
		{
			VPathRef_ seg{ nbeg, nend, false };

			if( seg_is_selfdir_( seg ) )
				continue;

			bool append = false;
			if( (seg.isBack = seg_is_parent_( seg )) )
			{
				if( !segments.empty() && !segments.back().isBack )
					segments.pop_back();
				else if( !aRestrict )
					append = true;
			}
			else
			{
				append = true;
			}

			if( append )
				segments.emplace_back( std::move(seg) );
		}
	}

	VPathBuf_ buf;
	std::size_t prefixLength = 0;

	if( isAbs )
	{
		buf.push_back( '/' );
		prefixLength = 1;
	}
	else if( segments.empty() )
	{
		buf.push_back( '.' );
	}

	for( auto const& seg : segments )
	{
		buf.insert( buf.end(), seg.beg, seg.end );
		buf.emplace_back( '/' );
	}

	FLUX_ASSERT( !buf.empty() );
	bool const endsOnSep = ('/' == buf.back());

	if( !endsOnSep && trailingSep ) buf.emplace_back( '/' );
	else if( endsOnSep && !trailingSep && !(isAbs && prefixLength == buf.size()) ) buf.pop_back();

	return std::string( buf.begin(), buf.end() );
}

//--    vpath_name()                        ///{{{2///////////////////////////
compat::string_view vpath_name( compat::string_view const& aPath ) noexcept
{
	if( aPath.empty() )
		return {};

	if( 1 == aPath.size() && '/' == aPath[0] )
		return aPath;
	
	auto end = aPath.size()-1;
	while( end && '/' == aPath[end] )
		--end;
	
	auto const rr = aPath.find_last_of( '/', end );
	if( compat::string_view::npos == rr )
		return aPath;

	return compat::string_view( aPath.data()+rr+1, end-rr );
}

//--    vpath_path()                        ///{{{2///////////////////////////
compat::string_view vpath_path( compat::string_view const& aPath ) noexcept
{
	if( aPath.empty() || (1 == aPath.size() && '/' == aPath[0]) )
		return {};

	auto end = aPath.size()-1;
	while( end && '/' == aPath[end] )
		--end;
	
	auto const rr = aPath.find_last_of( '/', end );
	if( compat::string_view::npos == rr )
		return {};

	return compat::string_view( aPath.data(), rr==0 ? 1 : rr );

}

//--    vpath_skip_common()                 ///{{{2///////////////////////////
compat::string_view vpath_skip_common( compat::string_view const& aPath, compat::string_view const& aBase ) noexcept
{
	auto const* pbeg = aPath.data();
	auto const* pend = aPath.data()+aPath.size();
	auto const* bbeg = aBase.data();
	auto const* bend = aBase.data()+aBase.size();

	if( '/' == *pbeg && '/' != *bbeg )
		return aPath;

	while( pbeg != pend )
	{
		auto next = std::find( pbeg+1, pend, '/' );
		auto dist = next-pbeg;

		if( dist > (bend-bbeg) || (bbeg[dist] != '/' && bbeg+dist != bend) || !std::equal( pbeg, next, bbeg ) )
		{
			if( '/' == *pbeg )
				++pbeg;

			return compat::string_view( pbeg, pend-pbeg );
		}

		bbeg += dist;
		pbeg = next;
	}

	return {};
}

//--    vpath_element_first()               ///{{{2///////////////////////////
compat::string_view vpath_element_first( compat::string_view const& aPath, bool aSkipRootElement ) noexcept
{
	if( aPath.empty() )
		return {};

	auto const* beg = aPath.data();
	auto const* end = aPath.data()+aPath.size();

	if( '/' == *beg )
	{
		if( aSkipRootElement ) ++beg;
		else return compat::string_view( beg, 1 );
	}

	auto const* next = std::find( beg, end, '/' );
	return compat::string_view( beg, next-beg );
}

//--    vpath_split_first()                 ///{{{2///////////////////////////
std::tuple<compat::string_view,compat::string_view> vpath_split_first( compat::string_view const& aPath, bool aSkipRootElement ) noexcept
{
	if( aPath.empty() )
		return std::make_tuple( compat::string_view(), compat::string_view() );

	auto const* beg = aPath.data();
	auto const* end = aPath.data()+aPath.size();

	if( '/' == *beg )
	{
		if( aSkipRootElement ) ++beg;
		else return std::make_tuple( compat::string_view( beg, 1 ), compat::string_view( beg+1, end-beg-1 ) );
	}

	auto const* next = std::find( beg, end, '/' );
	auto const found = (next != end);
	return std::make_tuple( compat::string_view( beg, next-beg ), compat::string_view( found ? next+1 : end, found ? end-next-1 : 0 ) );
}


//--    $ skip_seps_()                      ///{{{2///////////////////////////
namespace
{
	char const* skip_seps_( char const* aBeg, char const* aEnd ) noexcept
	{
		while( aBeg != aEnd && '/' == *aBeg )
			++aBeg;

		return aBeg;
	}
}

//--    $ seg_is_*()                        ///{{{2///////////////////////////
namespace
{
	bool seg_is_parent_( VPathRef_ const& aSeg ) noexcept
	{
		return 2 == (aSeg.end - aSeg.beg)
			&& '.' == aSeg.beg[0]
			&& '.' == aSeg.beg[1]
		;
	}
	bool seg_is_selfdir_( VPathRef_ const& aSeg ) noexcept
	{
		return 1 == (aSeg.end - aSeg.beg)
			&& '.' == aSeg.beg[0]
		;
	}
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
