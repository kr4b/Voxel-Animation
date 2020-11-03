/*-******************************************************* -- HEADER -{{{1- */
/*-	Forward declarations
 *
 * TODO
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_FORWARD_HPP_95FCFF47_88B5_472B_B621_2E82FF4CB47D
#define FLUX_IO_FORWARD_HPP_95FCFF47_88B5_472B_B621_2E82FF4CB47D

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/util/enum_bitfield.hpp>

#include <string>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    types                           ///{{{2///////////////////////////////

struct Buffer;
struct Stream;

using VfsSize = std::size_t;
using BufferSize = std::size_t;
using StreamSize = std::size_t;

struct VfsStat;
class VfsRecord;

#if 0
namespace vfs
{
	class Path;
	class Node;
}
#endif

namespace vfsi
{
	class VINode;
};

//--    constants                       ///{{{2///////////////////////////////

constexpr VfsSize kInvalidVfsSize = ~VfsSize(0);

enum class EBufferMode : unsigned
{
	read  = (1<<0),
	write = (1<<1)
};

FLUX_UTIL_DEFINE_ENUM_BITFIELD( EBufferMode );


enum class EStreamMode : unsigned
{
	read = (1u<<0),
	write = (1u<<1),

	text = (1u<<15),
	append = (write | (1u<<16)),
};

FLUX_UTIL_DEFINE_ENUM_BITFIELD( EStreamMode );


//--    functions                       ///{{{2///////////////////////////////

std::string to_string( EBufferMode );
std::string to_string( EStreamMode );

std::string to_string( VfsStat const& );

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_FORWARD_HPP_95FCFF47_88B5_472B_B621_2E82FF4CB47D
