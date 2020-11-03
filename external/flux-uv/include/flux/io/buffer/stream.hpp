/*-******************************************************* -- HEADER -{{{1- */
/*-	Stream buffer
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_BUFFER_STREAM_HPP_471777F1_4803_43AB_B2DF_293872FEFB6E
#define FLUX_IO_BUFFER_STREAM_HPP_471777F1_4803_43AB_B2DF_293872FEFB6E

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/io/buffer.hpp>
#include <flux/io/forward.hpp>

#include <flux/ftl/not_null.hpp>
#include <flux/std/unique_ptr.hpp>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    functions                       ///{{{2///////////////////////////////

/** Create buffer from stream
 *
 * Creates a `Buffer` representing the data of the provided `Stream`. The
 * `buffer_create_stream()` method chooses automatically between lazy and eager
 * buffering (see `buffer_create_stream_eager()` and
 * `buffer_create_stream_lazy()`, respectively), and thus only requires that
 * the `Stream` be readable and eventually terminate.
 *
 * \warning This does not use memory mapping, but reads the stream into a
 * buffer. For memory maps, see e.g. `buffer_create_sysfile()`. This function
 * is useful for `Stream`s that do not stem from a memory-mappable resource.
 *
 * The `Stream` must remain available and accessible for the life-time of 
 * the returned `Buffer`. 
 *
 * \see buffer_create_stream_eager(), buffer_create_stream_lazy()
 */
//@{
unique_ptr<Buffer> buffer_create_stream( 
	ftl::not_null<Stream*>, 
	StreamSize = ~StreamSize(0) 
); 
unique_ptr<Buffer> buffer_create_stream( 
	unique_ptr<Stream>, 
	StreamSize = ~StreamSize(0) 
);
//@}

/** Create buffer from stream (eagerly)
 *
 * Eagerly reads \a StreamSize bytes (or until the end of the stream if \a
 * StreamSize is set to `~StreamSize(0)`) from the provided stream and creates
 * a `Buffer` containing the read data.
 *
 * The `Buffer` is created w.r.t. the current position of the `Stream`. I.e.,
 * requesting a mapping at offset zero will return the data that would have
 * been returned by the next `Stream::read()`.
 *
 * \warning This does not use memory mapping, but reads the stream into a
 * buffer. For memory maps, see e.g. `buffer_create_sysfile()`. This function
 * is useful for `Stream`s that do not stem from a memory-mappable resource.
 *
 * The `Stream*` is no longer needed once the call completes, and may be
 * disposed of. (The overload taking a `unique_ptr<Stream>` does so
 * automatically.)
 *
 * Eager buffering requires only that the provided `Stream` is readable and
 * that the `Stream` eventually terminates. If \a aStreamSize is set to a fixed
 * length, the `Stream` must be able to provide at least that many bytes.
 *
 * \note Eager `Buffer`s are limited to at most
 * `buffer_stream_get_max_size_eager()` bytes. Attempting to `Buffer` a larger
 * `Stream` will result in a `error::BufferTooLarge`. The limit can be changed
 * using `buffer_stream_set_max_size_eager()`. The default size is 512Mbytes.
 */
//@{
unique_ptr<Buffer> buffer_create_stream_eager(
	ftl::not_null<Stream*>, 
	StreamSize = ~StreamSize(0) 
);
unique_ptr<Buffer> buffer_create_stream_eager(
	unique_ptr<Stream>,
	StreamSize = ~StreamSize(0)
);
//@}

/** Create buffer from stream (lazy)
 *
 * Creates a `Buffer` representing the contents of the provided `Stream`. Data
 * from the `Stream` is whenever a `map()` request is made.
 *
 * The `Buffer` is created w.r.t. the current position of the `Stream`. I.e.,
 * requesting a mapping at offset zero will return the data that would have
 * been returned by the next `Stream::read()`.
 *
 * The `Stream` must remain available and accessible for the life-time of 
 * the returned `Buffer`. 
 *
 * \warning This does not use memory mapping, but reads the stream into a
 * buffer. For memory maps, see e.g. `buffer_create_sysfile()`. This function
 * is useful for `Stream`s that do not stem from a memory-mappable resource.
 *
 * Lazy buffering requires that the provided `Stream` is readable, seekable
 * (in both directions), has a known current position and remaining size.
 * 
 * \note Mappings of a lazy `Buffer`s are limited to at most
 * `buffer_stream_get_max_size_lazy()` bytes. Attempting to `map()` a larger
 * range will result in a `error::BufferTooLarge`. The limit can be changed
 * using `buffer_stream_set_max_size_lazy()`. The default size is 128Mbytes.
 */
//@{
unique_ptr<Buffer> buffer_create_stream_lazy(
	ftl::not_null<Stream*>, 
	StreamSize = ~StreamSize(0) 
);
unique_ptr<Buffer> buffer_create_stream_lazy(
	unique_ptr<Stream>, 
	StreamSize = ~StreamSize(0) 
);
//@}


void buffer_stream_set_max_size_lazy( BufferSize ) noexcept;
void buffer_stream_set_max_size_eager( BufferSize ) noexcept;

BufferSize buffer_stream_get_max_size_lazy() noexcept;
BufferSize buffer_stream_get_max_size_eager() noexcept;

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_BUFFER_STREAM_HPP_471777F1_4803_43AB_B2DF_293872FEFB6E

