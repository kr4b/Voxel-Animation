/*-******************************************************* -- HEADER -{{{1- */
/*- Forward declarations.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_INSTR_FORWARD_HPP_1DC1262E_1875_48F0_9AF6_121A6DB2B075
#define FLUX_REPORT_INSTR_FORWARD_HPP_1DC1262E_1875_48F0_9AF6_121A6DB2B075

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/std/inttypes.hpp>

//--    >>> namespace = flux::report::instr >>> ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    forward                         ///{{{2///////////////////////////////

enum class EMode : uint8_t;
enum class EEvent : uint8_t;

struct Info;
struct Method;
struct Consumer;
struct EventBuffer;

struct MethodParams;
struct MethodInstance;

class Context;
class Instrumentor;

//--    <<< ~ flux::report::instr namespace <<< ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_INSTR_FORWARD_HPP_1DC1262E_1875_48F0_9AF6_121A6DB2B075
