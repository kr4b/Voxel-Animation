/*-******************************************************* -- HEADER -{{{1- */
/*- Instrumentor.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_INSTR_INSTRUMENTOR_HPP_FE68469B_621B_47CE_B253_39E01016FE62
#define FLUX_REPORT_INSTR_INSTRUMENTOR_HPP_FE68469B_621B_47CE_B253_39E01016FE62

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/unique_ptr.hpp>


#include "forward.hpp"

//--    >>> namespace = flux::report::instr >>> ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    Instrumentor                    ///{{{2///////////////////////////////
class Instrumentor final
{
	public:
		explicit Instrumentor( Context& );
		~Instrumentor();

		Instrumentor( Instrumentor const& ) = delete;
		Instrumentor& operator= (Instrumentor const&) = delete;

	public:
		void method_enable( std::size_t );
		void method_disable( std::size_t );
	
		void mode_transition( );

		void frame_begin( );
		void frame_end( );
		void frame_advance( );
	
		void scope_enter( ) noexcept;
		void scope_leave( ) noexcept;
		void scope_insert( ); //TODO: insert fake scope, e.g. from fast cpu timers

		void counter_set( );
		void counter_inc( );
		void counter_add( );
		void counter_sub( );

		void rate_feed( );

		void flush();
		
		void set_params( std::size_t, MethodParams const* );
		void set_frame_delay( std::size_t ) noexcept;

	public:
		void configure_method( std::size_t, unique_ptr<MethodInstance> );

	private:
		void process_();

	private:
		struct Self_;
		unique_ptr<Self_> self;
};

//--    functions                       ///{{{2///////////////////////////////

Instrumentor& instrument_thread_instrumentor() noexcept;

//--    <<< ~ flux::report::instr namespace <<< ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_INSTR_INSTRUMENTOR_HPP_FE68469B_621B_47CE_B253_39E01016FE62
