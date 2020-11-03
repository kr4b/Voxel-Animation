/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	ExceptData
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_35D5BF20_50C8_4228_AD9E_AA8CEFCC776A
#define DETAIL_35D5BF20_50C8_4228_AD9E_AA8CEFCC776A

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/std/identity.hpp>
#include <flux/std/optional.hpp>
#include <flux/std/unique_ptr.hpp>

#include "../except/forward.hpp"
#include "../except/einfo.hpp"

#include <iosfwd>
#include <typeindex>

//--    >>> namespace = flux::error::detail >>>     ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(error,detail)
//--    ExceptDataItemBase                  ///{{{2///////////////////////////
struct ExceptDataItemBase
{
	virtual ~ExceptDataItemBase() = 0;

	virtual void const* item() const noexcept = 0;
	virtual std::type_index type() const noexcept = 0;

	virtual char const* name() const noexcept = 0;
	virtual std::string item_as_string() const = 0;
};

//--    ExceptData                          ///{{{2///////////////////////////
class ExceptData final
{
	public:
		ExceptData() noexcept, ~ExceptData();

	public:
		template< class tInfo >
		void set( typename tInfo::type&& );
		template< class tInfo >
		void set( typename tInfo::type const& );

		template< class tInfo >
		optional<typename tInfo::type> get() const;

		char const* what_string( std::string );
		void diagnostic_output( std::ostream&, int = 0 ) const;

	private:
		void set_( Identity<einfo::SourceLine>, int ) noexcept;
		void set_( Identity<einfo::SourceFile>, char const* ) noexcept;
		void set_( Identity<einfo::SourceFunc>, char const* ) noexcept;

		void set_( Identity<einfo::ApiError>, compat::error_code ) noexcept;
		void set_( Identity<einfo::ApiFunction>, char const* ) noexcept;
		void set_( Identity<einfo::ApiResource>, std::uintptr_t ) noexcept;

		void set_( Identity<einfo::ErrorExpr>, std::string );
		void set_( Identity<einfo::ErrorDesc>, std::string );
		void set_( Identity<einfo::ErrorMeta>, std::string );

		template< class tInfo > 
		void set_( Identity<tInfo>, typename tInfo::type&& );
		template< class tInfo > 
		void set_( Identity<tInfo>, typename tInfo::type const& );

		optional<int> get_( Identity<einfo::SourceLine> ) const noexcept;
		optional<char const*> get_( Identity<einfo::SourceFile> ) const noexcept;
		optional<char const*> get_( Identity<einfo::SourceFunc> ) const noexcept;

		optional<compat::error_code> get_( Identity<einfo::ApiError> ) const noexcept;
		optional<char const*> get_( Identity<einfo::ApiFunction> ) const noexcept;
		optional<std::uintptr_t> get_( Identity<einfo::ApiResource> ) const noexcept;

		optional<std::string> get_( Identity<einfo::ErrorExpr> ) const;
		optional<std::string> get_( Identity<einfo::ErrorDesc> ) const;
		optional<std::string> get_( Identity<einfo::ErrorMeta> ) const;

		template< class tInfo >
		optional<typename tInfo::type> get_( Identity<tInfo> ) const;

		void set_erased_( unique_ptr<ExceptDataItemBase> );
		ExceptDataItemBase const* get_erased_( std::type_index ) const;

	private:
		int mSourceLine;
		char const* mSourceFile;
		char const* mSourceFunc;

		compat::error_code mApiError;
		char const* mApiFunc;
		optional<std::uintptr_t> mApiResource;

		std::string mErrorExpr;
		std::string mErrorDesc;
		std::string mErrorMeta;
	
		struct ErasedData_;
		unique_ptr<ErasedData_> mData;
};

//--    ExceptDataItem                      ///{{{2///////////////////////////
template< class tInfo >
struct ExceptDataItem final : ExceptDataItemBase
{
	using Item = typename tInfo::type;

	ExceptDataItem( Item&& aItem )
		: item_(std::move(aItem))
	{}
	ExceptDataItem( Item const& aItem )
		: item_(aItem)
	{}
	
	void const* item() const noexcept override
	{
		return &item_;
	}
	std::type_index type() const noexcept override
	{
		return std::type_index(typeid(tInfo));
	}

	char const* name() const noexcept override
	{
		return tInfo::name();
	}
	std::string item_as_string() const override
	{
		return tInfo::to_string( item_ );
	}

	Item item_;
};

//--    functions                           ///{{{2///////////////////////////

ExceptData& exception_data( Exception const& aException );

//--    <<< ~ flux::error::detail namespace <<<     ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(error,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "except_data.inl"
#endif // DETAIL_35D5BF20_50C8_4228_AD9E_AA8CEFCC776A
