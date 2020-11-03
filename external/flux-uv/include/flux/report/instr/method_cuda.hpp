/*-******************************************************* -- HEADER -{{{1- */
/*- Methods: CUDA timer
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_INSTR_METHOD_CUDA_HPP_5089DFB7_5A80_4019_B4C6_0C3DA0A81AB6
#define FLUX_REPORT_INSTR_METHOD_CUDA_HPP_5089DFB7_5A80_4019_B4C6_0C3DA0A81AB6

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/std/unique_ptr.hpp>

#include <flux_dlapi/cuda/cuda.hpp>

#include "method.hpp"

//--    >>> namespace = flux::report::instr >>> ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    constants                       ///{{{2///////////////////////////////

extern void const* const kMethodParamsCuda;

//--    MethodParamsCuda                ///{{{2///////////////////////////////
struct MethodParamsCuda : MethodParams
{
	dlapi::cuda::CU::Context context;
};

//--    functions                       ///{{{2///////////////////////////////

unique_ptr<Method> method_cuda_create( dlapi::cuda::CUapi const* = nullptr );

//--    <<< ~ flux::report::instr namespace <<< ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_INSTR_METHOD_CUDA_HPP_5089DFB7_5A80_4019_B4C6_0C3DA0A81AB6
