/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- error::detail::ExceptData implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/detail/except_data.hxx>

#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>

#include <flux/util/demangle.hpp>

#include <ostream>
#include <typeindex>
#include <unordered_map>

#include <cstring>

//--//////////////////////////////////////////////////////////////////////////
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	/* Hack: shorten flux-internal paths a bit ... */
	char const* skip_path_( char const* aFilePath )
	{
		FLUX_ASSERT( aFilePath );

		char const* prev = aFilePath;
		while( char const* next = std::strstr( prev+1, "/flux" ) )
			prev = next;
		while( char const* next = std::strstr( prev+1, "\\flux" ) )
			prev = next;

		FLUX_ASSERT( prev );
		return prev;
	}
}

//--    >>> namespace = flux::error::detail >>>     ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(error,detail)
//--    ExceptDataItemBase                  ///{{{2///////////////////////////
ExceptDataItemBase::~ExceptDataItemBase()
{}

//--    ExceptData::ErasedData_             ///{{{2///////////////////////////
struct ExceptData::ErasedData_
{
	std::unordered_map<std::type_index,unique_ptr<ExceptDataItemBase>> items;
	std::string whatString;
};

//--    ExceptData                          ///{{{2///////////////////////////

	// [cd]tor -{{{3-
ExceptData::ExceptData() noexcept
	: mSourceLine(-1)
	, mSourceFile(nullptr)
	, mSourceFunc(nullptr)
	, mApiFunc(nullptr)
{}

ExceptData::~ExceptData() = default;

	// public -{{{3-
void ExceptData::diagnostic_output( std::ostream& aOut, int aIndent ) const
{
	int const in = aIndent;

	if( !mErrorExpr.empty() ) print( aOut, FLUX_FMT_STRING("{1:>{0}}  - check: '{2}'\n"), in, "", mErrorExpr );
	if( !mErrorMeta.empty() ) print( aOut, FLUX_FMT_STRING("{1:>{0}}  - {2}\n"), in, "", mErrorMeta );
	if( !mErrorDesc.empty() ) print( aOut, FLUX_FMT_STRING("{1:>{0}}  - {2}\n"), in, "", mErrorDesc );
	
	if( mSourceFile ) 
	{
		print( aOut, FLUX_FMT_STRING("{1:>{0}}  - in {2}"), in, "", skip_path_(mSourceFile) );
		if( mSourceLine >= 0 ) print( aOut, FLUX_FMT_STRING(" line {:d}"), mSourceLine );
		aOut << "\n";
	}
	if( mSourceFunc ) print( aOut, FLUX_FMT_STRING("{1:>{0}}  - from function '{2}'\n"), in, "", mSourceFunc );

	if( mApiError ) print( aOut, FLUX_FMT_STRING("{1:>{0}}  - code {2:d} in category {3}: {4}\n"), in, "", mApiError.value(), mApiError.category().name(), mApiError.message() );
	if( mApiFunc ) print( aOut, FLUX_FMT_STRING("{1:>{0}}  - by function '{2}'\n"), in, "", mApiFunc );
	if( mApiResource ) print( aOut, FLUX_FMT_STRING("{1:>{0}}  - for handle {2:#x}\n"), in, "", mApiResource.value() );

	if( mData )
	{
		for( auto const& it : mData->items )
		{
			FLUX_ASSERT( it.second );
			print( aOut, FLUX_FMT_STRING("{1:>{0}}  - {2} = {3}\n"), in, "", it.second->name(), it.second->item_as_string() );
		}
	}
};

char const* ExceptData::what_string( std::string aWhatString )
{
	if( !mData )
		mData = make_unique<ErasedData_>();

	mData->whatString = std::move(aWhatString);
	return mData->whatString.c_str();
}
	
	// private -{{{3-
void ExceptData::set_erased_( unique_ptr<ExceptDataItemBase> aItem )
{
	if( !mData )
		mData = make_unique<ErasedData_>();
	
	mData->items[aItem->type()] = std::move(aItem);
}

ExceptDataItemBase const* ExceptData::get_erased_( std::type_index aIndex ) const
{
	return mData ? mData->items[aIndex].get() : nullptr;
}

//--    <<< ~ flux::error::detail namespace <<<     ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(error,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
