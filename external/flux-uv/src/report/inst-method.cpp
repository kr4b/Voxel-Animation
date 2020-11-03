/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Instrumentation Method
 */
/*-***************************************************************** -}}}1- */

#include <flux/report/instr/method.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::report::instr >>>     ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    MethodInstance                  ///{{{2///////////////////////////////

MethodInstance::~MethodInstance() = default;

void MethodInstance::resolve( std::size_t, Value*, uint32_t const* ) noexcept
{}
void MethodInstance::discard( std::size_t, Value* ) noexcept
{}

void MethodInstance::set_params( ftl::not_null<MethodParams const*> )
{}

//--    Method                          ///{{{2///////////////////////////////

Method::~Method() = default;

//--    <<< ~ flux::report::instr namespace <<<     ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
