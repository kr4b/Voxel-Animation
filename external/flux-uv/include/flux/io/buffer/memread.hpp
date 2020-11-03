/*-******************************************************* -- HEADER -{{{1- */
/*-	Read-only memory-backed buffer
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_BUFFER_MEMREAD_HPP_AF0F4EE4_6C73_46E6_A174_C902C004EA4F
#define FLUX_IO_BUFFER_MEMREAD_HPP_AF0F4EE4_6C73_46E6_A174_C902C004EA4F

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/io/buffer.hpp>

#include <flux/std/function.hpp>
#include <flux/std/unique_ptr.hpp>

#include <flux/ftl/not_null.hpp>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    functions                       ///{{{2///////////////////////////////

/** Create memory-backed read-only `Buffer`
 *
 * Create a read-only `Buffer` backed by the provided memory area. The `Buffer`
 * does not take ownership of the provided memory, i.e., the caller must
 * perform necessary cleanup eventually.
 *
 * If \a aMakeCopy is false, it is the callers responsibility to ensure that
 * the memory area remains accessible for the life-time of the returned 
 * `Buffer`. If \a aMakeCopy is true, the `Buffer` will maintain an internal
 * copy of the buffer, and the provided memory area can be reused immediately.
 */
unique_ptr<Buffer> buffer_create_memread(
	ftl::not_null<void const*>, 
	BufferSize,
	bool aMakeCopy = false
);
/** Create memory-backed read-only `Buffer`
 *
 * Create a read-only `Buffer` backed by the provided memory area. The `Buffer`
 * takes ownership of the provided memory, and will call the provided clean-up
 * function when the `Buffer` is being destroyed.
 */
unique_ptr<Buffer> buffer_create_memread_dealloc(
	ftl::not_null<void const*>,
	BufferSize,
	unique_function<void(void const*)> 
);

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_BUFFER_MEMREAD_HPP_AF0F4EE4_6C73_46E6_A174_C902C004EA4F

