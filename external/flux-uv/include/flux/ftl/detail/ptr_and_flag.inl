/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- PtrAndFlag<> implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/setup/stdfeat.hpp>

#include <flux/std/assert.hpp>

#include <flux/compat/constexpr.hpp>

#include <cstdint>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    $ local                             ///{{{2///////////////////////////
namespace detail
{
	template< typename tSrc, typename tDst >
	struct Ptr_Copy_CV_ { 
		using type = typename std::remove_cv<tDst>::type;
	};

	template< typename tSrc, typename tDst >
	struct Ptr_Copy_CV_< tSrc const, tDst > {
		using type = typename std::remove_cv<tDst>::type const; 
	};
	template< typename tSrc, typename tDst >
	struct Ptr_Copy_CV_< tSrc volatile, tDst > {
		using type = typename std::remove_cv<tDst>::type volatile; 
	};
	template< typename tSrc, typename tDst >
	struct Ptr_Copy_CV_< tSrc const volatile, tDst > {
		using type = typename std::remove_cv<tDst>::type const volatile; 
	};
};

//--    d: Embed_Flag_In_Ptr<>              ///{{{2///////////////////////////
namespace detail
{
	template< typename tPtr, std::size_t tAlign >
	struct Embed_Flag_In_Ptr : std::false_type
	{};

	template< typename tType, std::size_t tAlign >
	struct Embed_Flag_In_Ptr<tType*,tAlign> : std::true_type
	{};

	template< typename tType >
	struct Embed_Flag_In_Ptr<tType*,1> : std::false_type
	{};
}

//--    d: Ptr_Type_Alignment<>             ///{{{2///////////////////////////
namespace detail
{
	template< typename tPtr >
	struct Ptr_Type_Alignment : std::alignment_of<
		compat::remove_pointer_t<tPtr>
	> {};

	template<> struct Ptr_Type_Alignment<void*> 
		: std::integral_constant<size_t,1> {};
	template<> struct Ptr_Type_Alignment<void const*>
		: std::integral_constant<size_t,1> {};
	template<> struct Ptr_Type_Alignment<void volatile*>
		: std::integral_constant<size_t,1> {};
	template<> struct Ptr_Type_Alignment<void  const volatile*>
		: std::integral_constant<size_t,1> {};
}

//--    d: PtrAndFlagImpl<>                 ///{{{2///////////////////////////
namespace detail
{
	template< typename tPtr, bool tEmbed >
	class PtrAndFlagImpl
	{
		public:
			PtrAndFlagImpl() = default;
			FLUX_CONSTEXPR_EX
			PtrAndFlagImpl( tPtr aPtr, bool aFlag ) noexcept
				: mPtr(aPtr)
				, mFlag(aFlag)
			{}
			FLUX_CONSTEXPR_EX
			PtrAndFlagImpl( std::nullptr_t, bool aFlag ) noexcept
				: mPtr(nullptr)
				, mFlag(aFlag)
			{}

		public:
			FLUX_CONSTEXPR_EX bool flag() const noexcept { return mFlag; }
			FLUX_CONSTEXPR_EX tPtr pointer() const noexcept { return mPtr; }

			FLUX_CONSTEXPR_EX 
			void set_flag( bool aFlag ) noexcept { mFlag = aFlag; }
			FLUX_CONSTEXPR_EX 
			void set_pointer( tPtr aPtr ) noexcept { mPtr = aPtr; }
			FLUX_CONSTEXPR_EX 
			void set_pointer( std::nullptr_t ) noexcept { mPtr = nullptr; }

			FLUX_CONSTEXPR_EX
			void swap( PtrAndFlagImpl& aOther ) noexcept
			{
				using std::swap;
				swap( aOther.mPtr, mPtr );
				swap( aOther.mFlag, mFlag );
			}
		
		private:
			tPtr mPtr;
			bool mFlag;
	};

	template< typename tPtr >
	class PtrAndFlagImpl<tPtr,true>
	{
		using UIntPtr_ = std::uintptr_t;
		using UIntPtrCV_ = typename Ptr_Copy_CV_<tPtr,UIntPtr_>::type;

		public:
			PtrAndFlagImpl() = default;
			FLUX_CONSTEXPR_EX PtrAndFlagImpl( tPtr aPtr, bool aFlag ) noexcept
				: mBits( reinterpret_cast<UIntPtr_>(aPtr) )
			{
				if( aFlag ) mBits |= 1;
			}
			FLUX_CONSTEXPR_EX PtrAndFlagImpl( std::nullptr_t, bool aFlag ) noexcept
				: mBits( aFlag ? 1 : 0 )
			{}

		public:
			constexpr bool flag() const noexcept { return !!(mBits & 1); }
			constexpr tPtr pointer() const noexcept { 
				return reinterpret_cast<tPtr>(mBits & ~UIntPtr_(1));
			}

			FLUX_CONSTEXPR_EX
			void set_flag( bool aFlag ) noexcept {
				mBits = (mBits & ~UIntPtr_(1)) | (aFlag ? 1 : 0);
			}
			FLUX_CONSTEXPR_EX
			void set_pointer( tPtr aPtr ) noexcept {
				const_cast<UIntPtrCV_&>(mBits) = reinterpret_cast<UIntPtr_>(aPtr) | (mBits & 1);
			}
			FLUX_CONSTEXPR_EX
			void set_pointer( std::nullptr_t ) noexcept {
				const_cast<UIntPtrCV_&>(mBits) &= 1;
			}

			FLUX_CONSTEXPR_EX
			void swap( PtrAndFlagImpl& aOther ) noexcept {
				std::swap( aOther.mBits, mBits );
			}

		private:
			UIntPtr_ mBits;
	};
}

//--    PtrAndFlag<>                        ///{{{2///////////////////////////
template< typename tType, std::size_t tAlign > inline
PtrAndFlag<tType*,tAlign>::PtrAndFlag( Pointer aPtr, bool aFlag ) noexcept
	: detail::PtrAndFlagImpl<tType*, 
		detail::Embed_Flag_In_Ptr<tType*,tAlign>::value
	>( aPtr, aFlag )
{}
template< typename tType, std::size_t tAlign > inline
PtrAndFlag<tType*,tAlign>::PtrAndFlag( std::nullptr_t, bool aFlag ) noexcept
	: detail::PtrAndFlagImpl<tType*, 
		detail::Embed_Flag_In_Ptr<tType*,tAlign>::value
	>( nullptr, aFlag )
{}

//--    PtrAndFlag<const>                   ///{{{2///////////////////////////
template< typename tType, std::size_t tAlign > inline
PtrAndFlag<tType* const,tAlign>::PtrAndFlag( Pointer aPtr, bool aFlag ) noexcept
	: detail::PtrAndFlagImpl<tType* const, 
		detail::Embed_Flag_In_Ptr<tType*,tAlign>::value
	>( aPtr, aFlag )
{}
template< typename tType, std::size_t tAlign > inline
PtrAndFlag<tType* const,tAlign>::PtrAndFlag( std::nullptr_t, bool aFlag ) noexcept
	: detail::PtrAndFlagImpl<tType* const,
		detail::Embed_Flag_In_Ptr<tType*,tAlign>::value
	>( nullptr, aFlag )
{}


//--    get<>()                             ///{{{2///////////////////////////
template< typename tType, typename tPtr, std::size_t tAlign > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( std::is_same<compat::remove_cv_t<tType>,bool> )
(bool) get( PtrAndFlag<tPtr,tAlign> const& aPtr ) noexcept
{
	return aPtr.flag();
}
template< typename tType, typename tPtr, std::size_t tAlign > FLUX_CONSTEXPR_EX
FLUX_ENABLE_IF( meta::All< std::is_convertible<tPtr,tType>, meta::Not<std::is_same<compat::remove_cv_t<tType>,bool>> > )
(tType) get( PtrAndFlag<tPtr,tAlign> const& aPtr ) noexcept
{
	return aPtr.pointer();
}

//--    swap()                              ///{{{2///////////////////////////
template< typename tPtr, std::size_t tAlign > inline FLUX_CONSTEXPR_EX
void swap( PtrAndFlag<tPtr,tAlign>& aX, PtrAndFlag<tPtr,tAlign>& aY ) noexcept
{
	aX.swap( aY );
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
