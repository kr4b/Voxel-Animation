/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Identification Stuff
 *
 * XXX-FIXME: THIS NEEDS TO BE REFACTORED! SOME STUFF MAYBE SHOULD GO TO 
 * FLUX-STD SO THAT IT CAN BE SHARED WITH FLUX_LOG() ET AL..
 */
/*-***************************************************************** -}}}1- */

#include <flux/report/identity.hpp>

#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>
#include <flux/std/unique_ptr.hpp>

#include <flux/util/hint.hpp>
#include <flux/util/at_thread_exit.hpp>

#include <flux/sys/info.hpp>
#include <flux/ftl/id_pool.hpp>

#include <flux_ext/phmap.hpp>

//#include <map>
#include <mutex>
#include <atomic>
#include <string>
#include <sstream>

#include <cstddef>
#include <cstring>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::report >>>            ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(report)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
	struct ProcessIdent_
	{
		ProcessIdent_() = default;

		ProcessIdent_( ProcessIdent_ const& ) = delete;
		ProcessIdent_& operator= (ProcessIdent_ const&) = delete;
		
		std::atomic<std::size_t> generation{ 0 };

		std::mutex mut;

		std::size_t uniqueThreadCounter = 0;

		std::string hostName;
		std::string processName;

		ftl::IdPool<std::size_t>* tids;
		flux_ext::flat_hash_map<std::string,std::string> config;

		std::string cachedConfigString;

		bool initialized = false;

		static ProcessIdent_* immortal_instance() noexcept;
	} *gProcessIdent_ = ProcessIdent_::immortal_instance();

	void process_init_( ProcessIdent_*, std::unique_lock<std::mutex> const& );
	void process_clean_( ProcessIdent_* );
	void process_rebuild_cache_( ProcessIdent_*, std::unique_lock<std::mutex> const& );

	struct ThreadIdent_
	{
		std::size_t generation;
		std::size_t unique, active;

		std::string threadName;

		std::string cachedIdentity;
		std::string cachedHostName;
		std::string cachedProcessName;
		std::string cachedConfigString;
	};

	thread_local ThreadIdent_* tlThreadIdent_ = nullptr;

	ThreadIdent_* thread_ident_() noexcept;
	void thread_rebuild_cache_( ThreadIdent_* );
}

//--    kConfigFlag                     ///{{{2///////////////////////////////

char const* const kConfigFlag = "7549463F_B4BA_4803_897D_1CDF4F2D3EE1";

//--    identity()                      ///{{{2///////////////////////////////
char const* identity() noexcept
{
	if( auto const* ti = thread_ident_() )
		return ti->cachedIdentity.c_str();

	return "unknown-identity";
}

//--    *_name()                        ///{{{2///////////////////////////////
char const* process_name() noexcept
{
	if( auto const* ti = thread_ident_() )
		return ti->cachedProcessName.c_str();

	return "unknown-process";
}
char const* thread_name() noexcept
{
	if( auto const* ti = thread_ident_() )
		return ti->threadName.c_str();

	return "unknown-thread";
}

//--    config_string()                 ///{{{2///////////////////////////////
char const* config_string() noexcept
{
	if( auto const* ti = thread_ident_() )
		return ti->cachedConfigString.c_str();

	return "unknown-config";
}

//--    set_*_name()                    ///{{{2///////////////////////////////
void set_process_name( char const* aName ) noexcept
{
	if( !gProcessIdent_ ) return;
	if( !aName ) aName = "";

	try
	{
		std::unique_lock<std::mutex> lock( gProcessIdent_->mut );
		gProcessIdent_->processName = aName;
		++gProcessIdent_->generation;
	}
	catch( ... )
	{} // swallow
}
void set_thread_name( char const* aName ) noexcept
{
	if( auto* ti = thread_ident_() )
	{
		try
		{
			if( aName ) ti->threadName = aName;
			else ti->threadName = format( "t{:d}", ti->unique );

			thread_rebuild_cache_( ti );
		}
		catch( ... )
		{} // swallow
	}
}

//--    set_config()                    ///{{{2///////////////////////////////
void set_config( char const* aName, char const* aValue ) noexcept
{
	FLUX_ASSERT( aName );

	if( !gProcessIdent_ ) return;
	if( 0 == std::strcmp( "DEBUG", aName ) ) return;

	try 
	{
		std::unique_lock<std::mutex> lock( gProcessIdent_->mut );

		if( aValue )
		{
			if( kConfigFlag != aValue )
				gProcessIdent_->config[aName] = aValue;
			else
				gProcessIdent_->config[aName] = std::string();
		}
		else
		{
			auto it = gProcessIdent_->config.find(aName);
			if( gProcessIdent_->config.end() != it )
				gProcessIdent_->config.erase( it );
		}

		process_rebuild_cache_( gProcessIdent_, lock );
	}
	catch( ... )
	{} // swallow
}

//--    thread_*_index()                ///{{{2///////////////////////////////
std::size_t thread_unique_index() noexcept
{
	if( auto const* ti = thread_ident_() )
		return ti->unique;

	return ~std::size_t(0);
}
std::size_t thread_active_index() noexcept
{
	if( auto const* ti = thread_ident_() )
		return ti->active;

	return ~std::size_t(0);
}

//--    $ ProcessIdent_                 ///{{{2///////////////////////////////
namespace
{
#	if 0
	ProcessIdent_::ProcessIdent_()
		: generation(0)
	{
		//XXX-ERROR-FIXME
		//XXX-ERROR-FIXME  THIS IS RUN AS PART OF THE STARTUP CODE;
		//XXX-ERROR-FIXME

		// Host: host name
		hostName = sys::get_host_name(); // XXX-ERROR-FIXME: this tries to dlopen win32 api, which causes all sorts of things to occur (whuch is not good as part of STARTUP CODE)

		// Process: initially empty
		processName = "";

		// Config: set DEBUG if FLUX_DEBUG
#		if FLUX_DEBUG
		config.insert( std::make_pair( "DEBUG", std::string() ) );
#		endif // ~ DEBUG

		{
			std::unique_lock<std::mutex> lock( mut );
			process_rebuild_cache_( this, lock );
		}
	}
#	endif

	ProcessIdent_* ProcessIdent_::immortal_instance() noexcept
	{
		static std::aligned_storage<
			sizeof(ProcessIdent_),
			alignof(ProcessIdent_)
		>::type storage;

		try
		{
			std::atexit( static_cast<void(*)()>([] {
				if( gProcessIdent_ )
				{
					// Clean up memory allocations; this helps with valgrind,
					// but leave the object otherwise "intact".
					process_clean_( gProcessIdent_ );

					// For debugging purposes, make sure new threads get 
					// unrealistically large uniqe IDs. These appear in the
					// default name in various locations.
					gProcessIdent_->uniqueThreadCounter = ~std::size_t(0)-~std::size_t(0)/16;
				}
			}) );

			return new (&storage) ProcessIdent_();
		}
		catch( ... )
		{} 

		return nullptr;
	}
}

//--    $ thread_ident_()               ///{{{2///////////////////////////////
namespace
{
	ThreadIdent_* thread_ident_() noexcept
	{
		auto* ti = tlThreadIdent_;
		if( FLUX_HINT_LIKELY(ti) )
		{
			if( FLUX_HINT_UNLIKELY(gProcessIdent_ && gProcessIdent_->generation != ti->generation) )
			{
				thread_rebuild_cache_( ti );
			}

			return ti;
		}

		try
		{
			auto tl = make_unique<ThreadIdent_>();
	
			if( gProcessIdent_ )
			{
				std::unique_lock<std::mutex> lock(gProcessIdent_->mut);
				if( !gProcessIdent_->initialized )
					process_init_( gProcessIdent_, lock );

				if( gProcessIdent_->tids )
					tl->active = gProcessIdent_->tids->alloc();
				else
					tl->active = ~std::size_t(0);
				tl->unique = gProcessIdent_->uniqueThreadCounter++;
			}
			else
			{
				tl->active = tl->unique = ~std::size_t(0);
			}

			tl->generation = 0;
			tl->threadName = format( "t{:d}", tl->unique );

			thread_rebuild_cache_( tl.get() );

			util::at_thread_exit( [] { 
				if( gProcessIdent_ )
				{
					std::unique_lock<std::mutex> lock(gProcessIdent_->mut);
					if( gProcessIdent_->tids )
						gProcessIdent_->tids->release( tlThreadIdent_->active );
				}
				
				delete tlThreadIdent_; 
			} );
			return (tlThreadIdent_ = tl.release());
		}
		catch( ... )
		{}

		return nullptr;
	}
}

//--    $ *_rebuild_cache_()            ///{{{2///////////////////////////////
namespace
{
	void thread_rebuild_cache_( ThreadIdent_* aThreadData )
	{
		FLUX_ASSERT( aThreadData );

		if( !gProcessIdent_ )
		{
			aThreadData->cachedHostName = "unknown-host";
			aThreadData->cachedProcessName.clear();
			aThreadData->cachedConfigString.clear();
		}
		else if( aThreadData->generation != gProcessIdent_->generation )
		{
			std::unique_lock<std::mutex> lock(gProcessIdent_->mut);
			if( !gProcessIdent_->initialized )
				process_init_( gProcessIdent_, lock );

			aThreadData->cachedHostName = gProcessIdent_->hostName;
			aThreadData->cachedProcessName = gProcessIdent_->processName;
			aThreadData->cachedConfigString = gProcessIdent_->cachedConfigString;

			aThreadData->generation = gProcessIdent_->generation;
		}

		std::ostringstream oss;
		oss << aThreadData->cachedHostName;
		if( !aThreadData->cachedProcessName.empty() )
			oss << "_" << aThreadData->cachedProcessName;
		if( !aThreadData->threadName.empty() )
			oss << "_" << aThreadData->threadName;

		if( !aThreadData->cachedConfigString.empty() )
			oss << "_-_" << aThreadData->cachedConfigString;

		aThreadData->cachedIdentity = oss.str();
	}

	void process_init_( ProcessIdent_* aProcessData, std::unique_lock<std::mutex> const& aLock )
	{
		FLUX_ASSERT( aProcessData );
		FLUX_ASSERT( !aProcessData->initialized );

		// Host: host name
		// WARNING: On Win32, this performs a Win32 API call, which loads the
		// Win32api in flux-dlapi_os(). This causes problems if it occurs too
		// early!
		aProcessData->hostName = sys::get_host_name();

		// Process: initially empty
		aProcessData->processName = "";

		// Config: set DEBUG if FLUX_DEBUG
#		if FLUX_DEBUG
		aProcessData->config.emplace( std::make_pair( "DEBUG", std::string() ) );
#		endif // ~ DEBUG

		// Allocate tids
		aProcessData->tids = new ftl::IdPool<std::size_t>();

		// Set initialized and then attempt to rebuild cache
		// NOTE: process_init_() may have been called process_rebuild_cache_(),
		// so it's important to set the initialized flag first!
		aProcessData->initialized = true;
		process_rebuild_cache_( aProcessData, aLock );
	}
	void process_clean_( ProcessIdent_* aProcessData )
	{
		FLUX_ASSERT( aProcessData );

		// Make sure all memory is freed
		std::string hn, pn, ccs;
		std::swap( aProcessData->hostName, hn );
		std::swap( aProcessData->processName, pn );
		std::swap( aProcessData->cachedConfigString, ccs );

		flux_ext::flat_hash_map<std::string,std::string> cf;
		std::swap( aProcessData->config, cf );

		delete aProcessData->tids;
	}

	void process_rebuild_cache_( ProcessIdent_* aProcessData, std::unique_lock<std::mutex> const& aLock )
	{
		FLUX_ASSERT( aProcessData );
		FLUX_ASSERT( aLock.owns_lock() );

		if( !aProcessData->initialized )
			process_init_( aProcessData, aLock );

		bool sep = false;;
		std::ostringstream oss;
		for( auto const& cfg : aProcessData->config )
		{
			if( sep ) oss << "_";

			oss << cfg.first;
			if( !cfg.second.empty() )
				oss << cfg.second;

			sep = true;
		}

		aProcessData->cachedConfigString = oss.str();
		++aProcessData->generation;
	}
}

//--    <<< ~ flux::report namespace <<<            ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(report)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
