/*-******************************************************* -- HEADER -{{{1- */
/*- Context.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_INSTR_CONTEXT_HPP_133F0D47_04DC_4755_804F_95027B32D686
#define FLUX_REPORT_INSTR_CONTEXT_HPP_133F0D47_04DC_4755_804F_95027B32D686

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/ftl/vector.hpp>
#include <flux/std/unique_ptr.hpp>

#include "forward.hpp"

//--    >>> namespace = flux::report::instr >>> ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    constants                       ///{{{2///////////////////////////////




//--    Context                         ///{{{2///////////////////////////////
class Context final
{
	public:
		Context(), ~Context();

		Context( Context const& ) = delete;
		Context& operator= (Context const&) = delete;

	public:
		void configure_frame_delay( );
		void configure_linear_delay( );

		void group_define( );
		void method_define( std::size_t aSlot, unique_ptr<Method> );

		void consumer_configure( EMode, unique_ptr<Consumer> );
		void consumers_clear();
		void consumers_clear( EMode );
	
		void flush();
		void flush( EMode );

	public:
		void attach( Instrumentor& );
		void detach( Instrumentor& );
		
		EventBuffer consume( EventBuffer );

		void request_method( Instrumentor&, std::size_t ) const;

	private:
		struct Self_;
		unique_ptr<Self_> self;
};

//--    functions                       ///{{{2///////////////////////////////

Context& instrument_context() noexcept;

//--    <<< ~ flux::report::instr namespace <<< ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_INSTRUMENT_CONTEXT_HPP_133F0D47_04DC_4755_804F_95027B32D686
