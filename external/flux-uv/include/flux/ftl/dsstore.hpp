/*-******************************************************* -- HEADER -{{{1- */
/*-	Dynamic/Static size Storage
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_DSSTORE_HPP_C949D2B7_6D5B_4FBA_8CFA_ECD63FF090A3
#define FLUX_FTL_DSSTORE_HPP_C949D2B7_6D5B_4FBA_8CFA_ECD63FF090A3

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/constexpr.hpp>
#include <flux/compat/integer_sequence.hpp>

#include <flux/ftl/dsvalue.hpp>
#include <flux/ftl/initialized.hpp>
#include <flux/ftl/explicit_init_list.hpp>

#include <cstddef>
#include <type_traits>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	template< typename tX > inline FLUX_CONSTEXPR_EX
	tX init_ident_( tX const& aX, std::size_t ) noexcept(std::is_nothrow_copy_constructible<tX>::value)
	{
		return aX;
	}
}

//--    DSStore                             ///{{{2///////////////////////////
template< class tValue, typename tStoreType >
class DSStore;

template< typename tSType, tSType tSVal, typename tStoreType >
class DSStore< StaticValue<tSType,tSVal>, tStoreType > final
{
	public:
		struct Default {};
		struct Fill {};
		struct Eval {};
	
	public:
		DSStore() = default;

		FLUX_CONSTEXPR_EX explicit
		DSStore( StaticValue<tSType,tSVal> ) noexcept
			: mStore{}
		{}

		DSStore( ftl::ExplicitInitList<tStoreType> aInit ) noexcept(std::is_nothrow_copy_assignable<tStoreType>::value)
		{
			FLUX_ASSERT( aInit.ilist.size() <= tSVal );
			ftl::initialized_copy( aInit.ilist.begin(), aInit.ilist.end(), mStore );
		}

		FLUX_CONSTEXPR_EX
		DSStore( StaticValue<tSType,tSVal>, Default ) noexcept(std::is_nothrow_default_constructible<tStoreType>::value)
			: mStore{}
		{}
		FLUX_CONSTEXPR_EX
		DSStore( StaticValue<tSType,tSVal>, Fill, tStoreType const& aTemplate ) noexcept(std::is_nothrow_copy_constructible<tStoreType>::value)
			: DSStore( Fill{}, aTemplate, compat::make_index_sequence<tSVal>{} )
		{}
		template< class tFn > FLUX_CONSTEXPR_EX
		DSStore( StaticValue<tSType,tSVal>, Eval, tFn&& aFn ) noexcept(std::is_nothrow_copy_constructible<tStoreType>::value)
			: DSStore( Eval{}, std::forward<tFn>(aFn), compat::make_index_sequence<tSVal>{} )
		{}


	private:
#		if FLUX_COMPILER != FLUX_COMPILER_MSVC || _MSC_VER > 1800 // TODO maybe VS2015.update1 ??
		template< std::size_t... tIs > FLUX_CONSTEXPR_EX
		DSStore( Fill, tStoreType const& aTemplate, compat::index_sequence<tIs...> ) noexcept(std::is_nothrow_copy_constructible<tStoreType>::value)
			: mStore{ detail::init_ident_(aTemplate,tIs)... }
		{}
		template< class tFn, std::size_t... tIs > FLUX_CONSTEXPR_EX
		DSStore( Eval, tFn&& aFn, compat::index_sequence<tIs...> ) noexcept(std::is_nothrow_copy_constructible<tStoreType>::value)
			: mStore{ aFn(tIs)... }
		{}
#		else // Visual Studio 2013
		template< std::size_t... tIs > FLUX_CONSTEXPR_EX
		DSStore( Fill, tStoreType const& aTemplate, compat::index_sequence<tIs...> ) noexcept(std::is_nothrow_copy_constructible<tStoreType>::value)
		{
			for( std::size_t i = 0; i < sizeof...(tIs); ++i )
				mStore[i] = aTemplate;
		}
		template< class tFn, std::size_t... tIs > FLUX_CONSTEXPR_EX
		DSStore( Eval, tFn&& aFn, compat::index_sequence<tIs...> ) noexcept(std::is_nothrow_copy_constructible<tStoreType>::value)
		{
			for( std::size_t i = 0; i < sizeof...(tIs); ++i )
				mStore[i] = aFN(i);
		}
#		endif // ~ VisualStudio 2013
	
	public:
		FLUX_CONSTEXPR_EX
		tStoreType& operator[] (tSType aIdx) noexcept {
			return mStore[aIdx];
		}
		FLUX_CONSTEXPR_EX 
		tStoreType const& operator[] (tSType aIdx) const noexcept {
			return mStore[aIdx];
		}

	public:
		FLUX_CONSTEXPR_EX
		StaticValue<tSType,tSVal> size() const noexcept {
			return {};
		}
	
		FLUX_CONSTEXPR_EX
		tStoreType* data() noexcept {
			return mStore;
		}
		FLUX_CONSTEXPR_EX
		tStoreType const* data() const noexcept {
			return mStore;
		}

	private:
		tStoreType mStore[tSVal];
};

template< typename tDType, typename tStoreType >
class DSStore< DynamicValue<tDType>, tStoreType > final
{
	public:
		struct Default {};
		struct Fill {};
		struct Eval {};
	
	public:
		FLUX_CONSTEXPR_EX DSStore() noexcept
			: mSize(0)
			, mStore(nullptr)
		{}
		~DSStore()
		{
			delete [] mStore;
		}

		FLUX_CONSTEXPR_EX explicit
		DSStore( DynamicValue<tDType> aSize )
			: mSize(aSize)
		{
			mStore = new tStoreType[aSize];
		}

		FLUX_CONSTEXPR_EX DSStore( DSStore&& aOther ) noexcept
			: mSize(aOther.mSize)
			, mStore(aOther.mStore)
		{
			aOther.mSize = 0;
			aOther.mStore = nullptr;
		}
		FLUX_CONSTEXPR_EX DSStore& operator= (DSStore&& aOther) noexcept
		{
			std::swap( aOther.mSize, mSize );
			std::swap( aOther.mStore, mStore );
			return *this;
		}

		DSStore( DSStore const& aOther )
			: mSize( aOther.mSize )
		{
			mStore = new tStoreType[mSize];
			ftl::initialized_copy_n( aOther.mStore, mSize, mStore );
		}
		DSStore& operator= (DSStore const& aOther)
		{
			if( mStore ) delete [] mStore;

			mSize = aOther.mSize;
			mStore = new tStoreType[mSize];
			ftl::initialized_copy_n( aOther.mStore, mSize, mStore );
			return *this;
		}

		DSStore( ftl::ExplicitInitList<tStoreType> aInit ) noexcept(std::is_nothrow_copy_assignable<tStoreType>::value)
			: DSStore( DynamicValue<tDType>(aInit.ilist.size()) )
		{
			ftl::initialized_copy( aInit.ilist.begin(), aInit.ilist.end(), mStore );
		}

		DSStore( DynamicValue<tDType> aSize, Default )
			: mSize( aSize )
			, mStore( new tStoreType[aSize]() )
		{}
		DSStore( DynamicValue<tDType> aSize, Fill, tStoreType const& aTemplate )
			: mSize( aSize )
		{
			mStore = new tStoreType[mSize];
			ftl::initialized_fill_n( mStore, mSize, aTemplate );
		}
		template< class tFn >
		DSStore( DynamicValue<tDType> aSize, Eval, tFn&& aFn )
			: mSize( aSize )
		{
			mStore = new tStoreType[mSize];
			for( tDType i = 0; i < aSize; ++i )
				mStore[i] = aFn(i);
		}

	public:
		FLUX_CONSTEXPR_EX
		tStoreType& operator[] (tDType aIdx) noexcept {
			return mStore[aIdx];
		}
		FLUX_CONSTEXPR_EX 
		tStoreType const& operator[] (tDType aIdx) const noexcept {
			return mStore[aIdx];
		}

	public:
		FLUX_CONSTEXPR_EX
		DynamicValue<tDType> size() const noexcept {
			return mSize;
		}

		FLUX_CONSTEXPR_EX
		tStoreType* data() noexcept {
			return mStore;
		}
		FLUX_CONSTEXPR_EX
		tStoreType* data() const noexcept {
			return mStore;
		}

	private:
		DynamicValue<tDType> mSize;
		tStoreType* mStore;
};

//--    bitwise tags                        ///{{{2///////////////////////////

template< class tSize, typename tType >
auto tag_is_bitwise_conjurable( DSStore<tSize,tType> const& )
	-> std::true_type { return {}; }
template< class tSize, typename tType >
auto tag_is_bitwise_materializable( DSStore<tSize,tType> const& )
	-> ftl::is_static_value<tSize> { return {}; }
template< class tSize, typename tType >
auto tag_is_bitwise_duplicable( DSStore<tSize,tType> const& )
	-> ftl::is_static_value<tSize> { return {}; }
template< class tSize, typename tType >
auto tag_is_bitwise_relocatable( DSStore<tSize,tType> const& )
	-> std::true_type { return {}; }
template< class tSize, typename tType >
auto tag_is_bitwise_dismissible( DSStore<tSize,tType> const& )
	-> ftl::is_static_value<tSize> { return {}; }

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_FTL_DSSTORE_HPP_C949D2B7_6D5B_4FBA_8CFA_ECD63FF090A3
