/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	at_thread_exit() implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/util/at_thread_exit.hpp>

#include <flux/setup/platform.hpp>
#include <flux/setup/langfeat.hpp>

#include <stack>
#include <functional>

//--//////////////////////////////////////////////////////////////////////////
//--    at_thread_exit() - C++11            ///{{{2///////////////////////////
namespace
{
	class AtThreadExitDispatcher_
	{
		public:
			AtThreadExitDispatcher_() = default;
			~AtThreadExitDispatcher_();

			AtThreadExitDispatcher_( AtThreadExitDispatcher_ const& ) = delete;
			AtThreadExitDispatcher_& operator= (AtThreadExitDispatcher_ const&) = delete;

		public:
			void add( flux::util::AtThreadExitCallback const& );

		private:
			std::stack< flux::util::AtThreadExitCallback > mCallStack;
	};

	AtThreadExitDispatcher_::~AtThreadExitDispatcher_()
	{
		for( ; !mCallStack.empty(); mCallStack.pop() )
		{
			mCallStack.top()();
		}
	}

	void AtThreadExitDispatcher_::add( flux::util::AtThreadExitCallback const& aCallback )
	{
		mCallStack.push( aCallback );
	};
}

FLUX_NAMESPACE_ENTER1(util)
void at_thread_exit( AtThreadExitCallback const& aCallback )
{
	static thread_local AtThreadExitDispatcher_ tlThreadExitDispatch;
	tlThreadExitDispatch.add( aCallback );
}
FLUX_NAMESPACE_LEAVE1(util)

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
