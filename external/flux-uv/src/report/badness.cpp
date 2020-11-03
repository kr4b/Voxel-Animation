/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Badness
 */
/*-***************************************************************** -}}}1- */

#include <flux/report/badness.hpp>
#include <flux/report/state.hpp>

#include <flux/std/log.hpp>
#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>


#include <flux/ftl/vector.hpp>
#include <flux_ext/phmap.hpp>

#include <mutex>
#include <sstream>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::report >>>            ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(report)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
	constexpr std::size_t kTooManyBadnessEntries_ = 16;
	
	class BadnessTracker
	{
		private:
			BadnessTracker() = default;
			~BadnessTracker();

			BadnessTracker( BadnessTracker const& ) = delete;
			BadnessTracker& operator= (BadnessTracker const&) = delete;

		public:
			void record(
				EState,
				char const* aName,
				char const* aDesc,
				char const* aSrcFile,
				char const* aSrcFunc,
				int aSrcLine,
				double aScore
			) noexcept;

			void record_declared(
				DeclaredBadness const*,
				double aScore
			) noexcept;

		public:
			static BadnessTracker& instance() noexcept;

		private:
			struct DeclaredStats_
			{
				double totalScore;
				std::size_t hitcount;
			};

			struct BadnessEntry_
			{
				std::string desc;
				std::string srcfile;
				std::string srcfunc;
				double score;
				int srcline;
				EState state;
			};
			struct Badness_
			{
				double totalScore;
				ftl::Vector<BadnessEntry_> entries;
			};

		private:
			flux_ext::flat_hash_map<DeclaredBadness const*,DeclaredStats_> mDeclaredBadness;
			flux_ext::node_hash_map<std::string,Badness_> mBadness;

			std::mutex mMut;

			double mGrandTotal = 0.0;
			std::size_t mGrandHitCount = 0;
	};

	char const* state_( EState ) noexcept;
}

//--    badness()                       ///{{{2///////////////////////////////
void badness( EState aState, char const* aName, char const* aDesc, char const* aSrcFile, char const* aSrcFunc, int aSrcLine, double aScore) noexcept
{
	// Filter on state
	auto const state = int(aState);
	auto const current = int(thread_state());
	if( EState::ignored != aState && state >= current )
		return;

	auto const mult = double(current-state);

	auto& badness = BadnessTracker::instance();
	badness.record( aState, aName, aDesc, aSrcFile, aSrcFunc, aSrcLine, mult*aScore );
}

//--    declared_badness()              ///{{{2///////////////////////////////
void declared_badness( DeclaredBadness const* aBadness, double aScore ) noexcept
{
	FLUX_ASSERT( aBadness );

	// Filter on state
	auto const state = int(aBadness->state);
	auto const current = int(thread_state());
	if( EState::ignored != aBadness->state && state >= current )
		return;

	auto const mult = double(current-state);

	auto& badness = BadnessTracker::instance();
	badness.record_declared( aBadness, mult*aScore );
}

//--    $ BadnessTracker                ///{{{2///////////////////////////////
namespace
{
	BadnessTracker::~BadnessTracker()
	{
		if( mBadness.empty() && mDeclaredBadness.empty() )
			return;

		try
		{
			std::ostringstream oss;
			print( oss, "{:.2f} badness points from {:d} sources:\n", mGrandTotal, mGrandHitCount );

			std::size_t totalEntries = 0;
			for( auto const& item : mDeclaredBadness )
			{
				//print( oss, "   [%s%s] %.2f badness from %u hits (%s)\n", item.first->name, state_(item.first->state), item.second.totalScore, item.second.hitcount, item.first->desc );
				print( oss, "  {:7.2f}/{:-3d}  {:7s}  [{}] {}\n", item.second.totalScore, item.second.hitcount, state_(item.first->state), item.first->name, item.first->desc );

				++totalEntries;
			}
			for( auto const& item : mBadness )
			{
				//print( oss, "   [{}] {:.2f} badness from {:d} hits\n", item.first, item.second.totalScore, item.second.entries.size() );
				print( oss, "  {:7.2f}/{:-3d} --{}--\n", item.second.totalScore, item.second.entries.size(), item.first );
				for( auto const& entry : item.second.entries )
				{
					//print( oss, "      - %7.2f  %7s  %s (%s:%d in %s)\n", entry.score, state_(entry.state), entry.desc, entry.srcfile, entry.srcline, entry.srcfunc );
					print( oss, "      - {:7.2f}  {:7s}  {} \n", entry.score, state_(entry.state), entry.desc );

					++totalEntries;
				}
			}

			print( oss, "(Note: some of the badness may be resolved by updating the thread/process state (flux::report::set_thread_state()).)" );

			FLUX_LOG( WARNING, "{}", oss.str() );

			if( totalEntries >= kTooManyBadnessEntries_ )
			{
				FLUX_LOG( ERROR, "Badness: many badness entries recorded ({:d}). Please resolve some of these.", totalEntries );
			}
		}
		catch( ... )
		{
			// Swallow.
		}
	}

	void BadnessTracker::record( EState aState, char const* aName, char const* aDesc, char const* aSrcFile, char const* aSrcFunc, int aSrcLine, double aScore ) noexcept try
	{
		FLUX_ASSERT( aName && aDesc );
		FLUX_ASSERT( aSrcFile && aSrcFunc );
		std::unique_lock<std::mutex> lock(mMut);

		auto& item = mBadness[aName];
		item.totalScore += aScore;
		item.entries.emplace_back( BadnessEntry_{
			aDesc,
			aSrcFile,
			aSrcFunc,
			aScore,
			aSrcLine,
			aState
		} );

		mGrandTotal += aScore;
		++mGrandHitCount;
	}
	catch( ... )
	{ 
		/* swallow */
	}

	void BadnessTracker::record_declared( DeclaredBadness const* aBadness, double aScore ) noexcept try
	{
		FLUX_ASSERT( aBadness );
		std::unique_lock<std::mutex> lock(mMut);

		auto& item = mDeclaredBadness[aBadness];

		++item.hitcount;
		item.totalScore += aScore;

		mGrandTotal += aScore;
		++mGrandHitCount;
	}
	catch( ... )
	{ 
		/* swallow */
	}

	BadnessTracker& BadnessTracker::instance() noexcept
	{
		static BadnessTracker inst;
		return inst;
	}
}

//--    $ state_()                      ///{{{2///////////////////////////////
namespace
{
	char const* state_( EState aState ) noexcept
	{
		switch( aState )
		{
			case EState::cold: return "cold";
			case EState::critical: return "crit";
			case EState::hot: return "hot";
			case EState::loading: return "loading";
			case EState::offline: return "offline";

			//case EState::inherit: return "INVALID-inherit";
			case EState::ignored: return "";
			default: break;
		}

		return "UNKNOWN";
	}
}

//--    <<< ~ flux::report namespace <<<            ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(report)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab:
