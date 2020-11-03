/*-******************************************************* -- HEADER -{{{1- */
/*- Methods.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_INSTR_METHOD_HPP_86B6DABB_15EE_4EAA_93B6_B91FC7B40CF5
#define FLUX_REPORT_INSTR_METHOD_HPP_86B6DABB_15EE_4EAA_93B6_B91FC7B40CF5

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/inttypes.hpp>
#include <flux/std/unique_ptr.hpp>


#include <flux/ftl/not_null.hpp>

//--    >>> namespace = flux::report::instr >>> ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    constants                       ///{{{2///////////////////////////////

constexpr std::size_t kMethodCPU = 0;
constexpr std::size_t kMethodGL = 1;
constexpr std::size_t kMethodVK = 2;
constexpr std::size_t kMethodCU = 3;
constexpr std::size_t kMethodUserMin = 4;
constexpr std::size_t kMethodUserMax = 64;

//--    types                           ///{{{2///////////////////////////////

using Value = double; //TODO: variant<>??

struct MethodParams 
{
	void const* methodId;
};

//--    MethodInstance                  ///{{{2///////////////////////////////
struct MethodInstance
{
	virtual ~MethodInstance() = 0;

	//TODO: additional identifier, such as the thread ID, CUDA GPU or similar

	virtual void begin( Value&, std::size_t ) noexcept = 0;
	virtual void end( Value&, std::size_t, Value&, std::size_t ) noexcept = 0;

	virtual void resolve( std::size_t, Value*, uint32_t const* ) noexcept;
	virtual void discard( std::size_t, Value* ) noexcept;

	virtual void set_params( ftl::not_null<MethodParams const*> );
};

//--    Method                          ///{{{2///////////////////////////////
struct Method
{
	virtual ~Method() = 0;

	virtual char const* name() noexcept = 0;
	virtual char const* unit() noexcept = 0;

	virtual unique_ptr<MethodInstance> create_instance() = 0;
};

//--    functions                       ///{{{2///////////////////////////////


//--    <<< ~ flux::report::instr namespace <<< ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_INSTR_METHOD_HPP_86B6DABB_15EE_4EAA_93B6_B91FC7B40CF5
