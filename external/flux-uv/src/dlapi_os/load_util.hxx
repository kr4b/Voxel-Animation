#ifndef DETAIL_LOAD_UTIL_HXX_FF2A0FD8_1A24_40E5_836A_0D5C5BB3B850
#define DETAIL_LOAD_UTIL_HXX_FF2A0FD8_1A24_40E5_836A_0D5C5BB3B850

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>


#include <mutex>
#include <atomic>

//--    >>> namespace = flux::dalpi::os::detail >>> ///{{{1///////////////////
FLUX_NAMESPACE_ENTER3(dlapi,os,detail)
//--    ApiRecord                           ///{{{2///////////////////////////
template< class tApi >
class ApiRecord
{
	public:
		ApiRecord() = default;
		~ApiRecord();

		ApiRecord( ApiRecord const& ) = delete;
		ApiRecord& operator=( ApiRecord const&) = delete;

	public:
		tApi const* api() const noexcept;

		bool acquire( char const* aDlibName = nullptr );
		bool acquire( void* (*)(char const*, void*), void* );

		void release();

		tApi const* try_autoload() noexcept;

	private:
		std::mutex mMutex;
		std::atomic<tApi const*> mApi;
		std::atomic<bool> mAutoloadAttempted;
};

//--    FnPtrProxy                          ///{{{2///////////////////////////

/* This is moderately evil, but we're going to be casting things either way. */
struct FnPtrProxy
{
	FnPtrProxy( void* aPtr ) noexcept;

	template< typename tType > 
	operator tType*() const noexcept;

	void* mPtr;
};

//--    <<< ~ flux::dlapi::os::detail namespace <<< ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE3(dlapi,os,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "load_util.inl"
#endif // DETAIL_LOAD_UTIL_HXX_FF2A0FD8_1A24_40E5_836A_0D5C5BB3B850
