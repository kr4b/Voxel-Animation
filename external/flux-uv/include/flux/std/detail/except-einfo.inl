/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	except/einfo.hpp implementation
 */
/*-***************************************************************** -}}}1- */

#include "except_data.hxx"

#include <utility>
#include <typeinfo>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::einfo >>>             ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(einfo)
//--    ErrorInfo                           ///{{{2///////////////////////////
template< class tTag, typename tType > template< typename... tArgs > inline
ErrorInfo<tTag,tType>::ErrorInfo( tArgs&&... aArgs ) noexcept(std::is_nothrow_constructible<tType,tArgs...>::value)
	: mValue( tType(std::forward<tArgs>(aArgs)...) )
{}

template< class tTag, typename tType > inline
tType&& ErrorInfo<tTag,tType>::get() && noexcept
{
	return std::move(mValue);
}
template< class tTag, typename tType > inline
tType const& ErrorInfo<tTag,tType>::get() const& noexcept
{
	return mValue;
}



template< class tTag, typename tType > inline
char const* ErrorInfo<tTag,tType>::name() noexcept
{
	return typeid(ErrorInfo).name();
}
template< class tTag, typename tType > inline
std::string ErrorInfo<tTag,tType>::to_string( tType const& aValue )
{
	return format( "{}", aValue );
}

//--    ErrorInfo<X,std::string>            ///{{{2///////////////////////////
template< class tTag > inline
ErrorInfo<tTag,std::string>::ErrorInfo( char const* aStr )
	: mString( aStr )
{}
template< class tTag > inline
ErrorInfo<tTag,std::string>::ErrorInfo( std::string&& aString ) noexcept
	: mString( std::move(aString) )
{}
template< class tTag > inline
ErrorInfo<tTag,std::string>::ErrorInfo( std::string const& aString )
	: mString( aString )
{}

template< class tTag > template< typename tS, typename... tArgs > inline
ErrorInfo<tTag,std::string>::ErrorInfo( tS const& aFmt, tArgs&&... aArgs )
	: mString( format( aFmt, std::forward<tArgs>(aArgs)... ) )
{}

/* FLUX_DEPRECATED_EX() is repeated. Visual Studio seems to forget about 
 * deprecation otherwise.
 */
template< class tTag > template< typename... tArgs > 
FLUX_DEPRECATED_EX( "Use FLUX_FMT_STRING()" )
inline
ErrorInfo<tTag,std::string>::ErrorInfo( char const* aFmt, tArgs&&... aArgs )
	: mString( format( aFmt, std::forward<tArgs>(aArgs)... ) )
{}

#if 0
template< class tTag > inline
std::string&& ErrorInfo<tTag,std::string>::move() noexcept
{
	return std::move(mString);
}
template< class tTag > inline
std::string const& ErrorInfo<tTag,std::string>::value() const noexcept
{
	return mString;
}
#endif

template< class tTag > inline
std::string&& ErrorInfo<tTag,std::string>::get() && noexcept
{
	return std::move(mString);
}
template< class tTag > inline
std::string const& ErrorInfo<tTag,std::string>::get() const& noexcept
{
	return mString;
}

template< class tTag > inline
char const* ErrorInfo<tTag,std::string>::name() noexcept
{
	return typeid(ErrorInfo).name();
}
template< class tTag> inline
std::string const& ErrorInfo<tTag,std::string>::to_string( std::string const& aValue )
{
	return aValue;
}


//--    ApiError                            ///{{{2///////////////////////////
inline ApiError::ApiError( compat::error_code const& aCode ) noexcept
	: Parent_( aCode )
{}

inline ApiError::ApiError( int aCode, compat::error_category const& aCategory ) noexcept
	: Parent_( compat::error_code( aCode, aCategory ) )
{}

inline ApiError::ApiError( compat::errc aErrCode ) noexcept
	: Parent_( make_error_code( aErrCode ) )
{}

template< typename tErrorEnum, typename > inline
ApiError::ApiError( tErrorEnum aErrorEnum ) noexcept
	: Parent_ ( make_error_code( aErrorEnum ) )
{}

inline char const* ApiError::name() noexcept
{
	return "ApiError";
}

//--    operator<<                          ///{{{2///////////////////////////

template< class tExcept, class tInfo >
FLUX_ENABLE_IF_C( std::is_base_of<error::Exception, tExcept>::value && std::is_base_of<einfo::detail::ErrorInfoBase,compat::remove_reference_t<tInfo>>::value )
(tExcept const&) operator<< (tExcept const& aExcept, tInfo&& aInfo )
{
	auto& edata = error::detail::exception_data(aExcept);
	edata.template set<tInfo>( std::forward<tInfo>(aInfo).get() );
	return aExcept;
}

//--    <<< ~ flux::einfo namespace <<<             ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(einfo)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
