/*-******************************************************* -- HEADER -{{{1- */
/*-	Memory-backed read-only io::Streams
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_STREAM_MEMREAD_HPP_E1117836_39E3_4851_9C68_B367CDF2F5DD
#define FLUX_IO_STREAM_MEMREAD_HPP_E1117836_39E3_4851_9C68_B367CDF2F5DD

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/io/stream.hpp>

#include <flux/ftl/not_null.hpp>
#include <flux/std/function.hpp>
#include <flux/std/unique_ptr.hpp>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    functions                       ///{{{2///////////////////////////////

/** Create memory-backed read-only `Stream`
 *
 * Create a read-only `Stream` backed by the provided memory area. The `Stream`
 * does not take ownership of the provided memory, i.e., the caller must
 * perform necessary cleanup eventually.
 *
 * If \a aMakeCopy is false, it is the callers responsibility to ensure that
 * the memory area remains accessible for the life-time of the returned 
 * `Stream`. If \a aMakeCopy is true, the `Stream` will maintain an internal
 * copy of the buffer, and the provided memory area can be reused immediately.
 */
unique_ptr<Stream> stream_create_memread(
	ftl::not_null<void const*>, 
	StreamSize,
	bool aMakeCopy = false
);
/** Create memory-backed read-only `Stream`
 *
 * Create a read-only `Stream` backed by the provided memory area. The `Stream`
 * takes ownership of the provided memory, and will call the provided clean-up
 * function when the `Stream` is being destroyed.
 */
unique_ptr<Stream> stream_create_memread_dealloc(
	ftl::not_null<void const*>,
	StreamSize,
	function<void(void const*)> 
);

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_STREAM_MEMREAD_HPP_E1117836_39E3_4851_9C68_B367CDF2F5DD
