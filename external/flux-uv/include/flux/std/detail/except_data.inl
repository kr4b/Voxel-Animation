/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	ExceptData implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::error::detail >>>     ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(error,detail)
//--    ExceptData                           ///{{{2///////////////////////////

	// public -{{{3-
template< class tInfo > inline
void ExceptData::set( typename tInfo::type&& aData )
{
	set_( Identity<tInfo>{}, std::move(aData) );
}
template< class tInfo > inline
void ExceptData::set( typename tInfo::type const& aData )
{
	set_( Identity<tInfo>{}, aData );
}

template< class tInfo > inline
optional<typename tInfo::type> ExceptData::get() const
{
	return get_( Identity<tInfo>{} );
}

	// private -{{{3-
inline void ExceptData::set_( Identity<einfo::SourceLine>, int aLine ) noexcept
{
	mSourceLine = aLine;
}
inline void ExceptData::set_( Identity<einfo::SourceFile>, char const* aFile ) noexcept
{
	mSourceFile = aFile;
}
inline void ExceptData::set_( Identity<einfo::SourceFunc>, char const* aFunc ) noexcept
{
	mSourceFunc = aFunc;
}

inline void ExceptData::set_( Identity<einfo::ApiError>, compat::error_code aCode ) noexcept
{
	mApiError = aCode;
}
inline void ExceptData::set_( Identity<einfo::ApiFunction>, char const* aApiFunc ) noexcept
{
	mApiFunc = aApiFunc;
}
inline void ExceptData::set_( Identity<einfo::ApiResource>, std::uintptr_t aApiResource ) noexcept
{
	mApiResource = aApiResource;
}

inline void ExceptData::set_( Identity<einfo::ErrorExpr>, std::string aErrorExpr )
{
	mErrorExpr = std::move(aErrorExpr);
}
inline void ExceptData::set_( Identity<einfo::ErrorDesc>, std::string aErrorDesc )
{
	mErrorDesc = std::move(aErrorDesc);
}
inline void ExceptData::set_( Identity<einfo::ErrorMeta>, std::string aErrorMeta )
{
	mErrorMeta = std::move(aErrorMeta);
}

template< class tInfo > inline
void ExceptData::set_( Identity<tInfo>, typename tInfo::type&& aData )
{
	set_erased_( make_unique<ExceptDataItem<tInfo>>( std::move(aData) ) );
}
template< class tInfo > inline
void ExceptData::set_( Identity<tInfo>, typename tInfo::type const& aData )
{
	set_erased_( make_unique<ExceptDataItem<tInfo>>( aData ) );
}


inline optional<int> ExceptData::get_( Identity<einfo::SourceLine> ) const noexcept
{
	return mSourceLine >= 0 
		? optional<int>(mSourceLine)
		: nullopt
	;
}
inline optional<char const*> ExceptData::get_( Identity<einfo::SourceFile> ) const noexcept
{
	return !!mSourceFile 
		? optional<char const*>(mSourceFile)
		: nullopt
	;
}
inline optional<char const*> ExceptData::get_( Identity<einfo::SourceFunc> ) const noexcept
{
	return !!mSourceFunc 
		? optional<char const*>(mSourceFunc) 
		: nullopt
	;
}

inline optional<compat::error_code> ExceptData::get_( Identity<einfo::ApiError> ) const noexcept
{
	return !!mApiError 
		? optional<compat::error_code>(mApiError)
		: nullopt
	;
}
inline optional<char const*> ExceptData::get_( Identity<einfo::ApiFunction> ) const noexcept
{
	return !!mApiFunc
		? optional<char const*>(mApiFunc)
		: nullopt
	;
}
inline optional<std::uintptr_t> ExceptData::get_( Identity<einfo::ApiResource> ) const noexcept
{
	return mApiResource;
}

inline optional<std::string> ExceptData::get_( Identity<einfo::ErrorExpr> ) const
{
	return !mErrorExpr.empty() ? make_optional(mErrorExpr) : nullopt;
}
inline optional<std::string> ExceptData::get_( Identity<einfo::ErrorDesc> ) const
{
	return !mErrorDesc.empty() ? make_optional(mErrorDesc) : nullopt;
}
inline optional<std::string> ExceptData::get_( Identity<einfo::ErrorMeta> ) const
{
	return !mErrorMeta.empty() ? make_optional(mErrorMeta) : nullopt;
}

template< class tInfo > inline
optional<typename tInfo::type> ExceptData::get_( Identity<tInfo> ) const
{
	if( auto ptr = get_erased_( std::type_index(typeid(tInfo)) ) )
	{
		auto it = static_cast<typename tInfo::type const*>(ptr->item());
		FLUX_ASSERT( it );

		return make_optional( *it );
	}

	return nullopt;
}

//--    exception_data()                    ///{{{2///////////////////////////
inline ExceptData& exception_data( Exception const& aException )
{
	aException.ensure_edata_();
	return *aException.mData;
}

//--    <<< ~ flux::error::detail namespace <<<     ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(error,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
