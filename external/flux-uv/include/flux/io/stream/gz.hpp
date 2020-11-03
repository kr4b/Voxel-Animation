/*-******************************************************* -- HEADER -{{{1- */
/*-	GZip Filter Stream
 *
 * TODO: make it possible to return the original file name and file comments
 * when reading .gz files. Perhaps via ftl::VectorBase<char>* pointers that
 * would be filled when the respective fields are present.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_STREAM_GZ_HPP_CCD9BFA3_DE28_422B_B777_658EB6A0F97F
#define FLUX_IO_STREAM_GZ_HPP_CCD9BFA3_DE28_422B_B777_658EB6A0F97F

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/io/forward.hpp>
#include <flux/ftl/not_null.hpp>
#include <flux/std/unique_ptr.hpp>
#include <flux/util/enum_bitfield.hpp>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    constants                       ///{{{2///////////////////////////////

/* \brief Default compression level
 *
 * The default compression level of \f$6\f$ provides a reasonable trade-off
 * between speed and compression ratio. The compression ratio is (for a single
 * quick test case) roughly equal to that of the `gzip` command (about 45%)
 * and performs similarly (about 25MB/s).
 */
constexpr unsigned kGZDefCompLevel = 6;

constexpr StreamSize kGZDefDecompInBuffer = 128*1024;
constexpr StreamSize kGZDefDecompOutBuffer = 256*1024;

enum class EGZFlags
{
	none = 0,

	noHeader = (1<<0),           //!< Assume that there is no .gz file header
	noFooter = (1<<1),           //!< Assume that there is no .gz file footer (implies `ignoreUncompressedSize` when reading)

	/*! \brief Ignore uncompressed size
	 *
	 * Ignore the uncompressed size that might be reported in the file footer.
	 * This size is stored as a 32-bit unsigned integer and wraps at 4G. It is
	 * thus incorrect for large (>= 4G) files.
	 *
	 * By default, the size is nevertheless read and report, as this is useful
	 * when attempting to fully buffer a input stream.
	 *
	 * If this flag is set, the uncompressed size is unknown and the returned
	 * `Stream` does not report the  `size` and `remaining` capabilities
	 * (`StreamCaps::size` and `StreamCaps::remaining`), meaning that it is not
	 * permissible to call `Stream::size()` or `Stream::remaining()` on the
	 * returned object.
	 *
	 * This flag only affects reading only.
	 */
	ignoreUncompressedSize = (1<<2),

	headerOmitOS = (1<<3),       //!< Always write "Unknown OS" (255) in the header
	headerOmitTime = (1<<4),     //!< Always write a timestamp of zero in the header

	//zlibHeaders = (1<<5),
};

FLUX_UTIL_DEFINE_ENUM_BITFIELD( EGZFlags );

//--    functions                       ///{{{2///////////////////////////////

/** \brief Filter: .gz compressed stream reader
 *
 * Create a `Stream` that reads and decompresses data from an underlying .gz
 * compressed `Stream` on the fly. The underlying \a Stream must be readable.
 * 
 * Two versions of this function exist. The first one takes ownership of the
 * underlying stream and closes/destroys it when the returned `Stream` object
 * is destroyed. The second version does not take ownership (and the caller is
 * is required to eventually dispose of both stream objects). The second
 * version has an additional parameter, \a StreamSize, which limits the amount
 * of data that will be read from the underlying stream.
 *
 * By default, the `stream_create_gz_read()` method will try to read and parse
 * the .gz header. If the header cannot be read, or is invalid, the method will
 * throw `error::StreamBadFormat`. Reading and parsing of the header can be
 * disabled by passing `EGZFlags::noHeader` to the \a EGZFlags argument.
 *
 * Decompression relies on two buffers that are allocated on creation. The
 * input buffer is filled by reading from the underlying stream. Its size
 * determines the approximate size of the reads made to the underlying stream,
 * but can otherwise be choosen freely (\f$\ge 1\f$). The output buffer serves
 * both as a buffer for some run-time state of the decompressor as well as a
 * staging area for the decompressed data. Its size must be a power of two and
 * it must be large enough to hold dictionary data during decompression. The
 * maximum size of the dictionary is 32k, meaning that 64k is likely the
 * smallest safe choice. The default values yield good performance when the
 * underlying stream is reading from a system file.
 *
 * \note The buffer and stream objects are placed in a single allocation of
 * size \f$\mbox{sizeof(Stream Implementation)} + \mbox{padding for alignment}
 * + \mbox{output buffer size} + \mbox{input buffer size}\f$. The sub-objects
 * are laid out in this order in the allocation.
 * 
 * The returned stream will throw `error::StreamBadFormat` from `read()` or
 * `seek()` if the underlying stream cannot be decompressed successfully.
 *
 * \warning The created streams do <b>not</b> verify the CRC checksums that 
 * .gz files include. This includes both the (optional) CRC16 of the header,
 * and the CRC32 of the decompressed data included in the footer.
 */
unique_ptr<Stream> stream_create_gz_read(
	ftl::not_null<unique_ptr<Stream>>, 
	EGZFlags = EGZFlags::none, 
	StreamSize aInputBufferSize = kGZDefDecompInBuffer, 
	StreamSize aOutputBufferSize = kGZDefDecompOutBuffer
);
/** \copydoc stream_create_gz_read() */
unique_ptr<Stream> stream_create_gz_read( 
	ftl::not_null<Stream*>, 
	StreamSize = ~StreamSize(0), 
	EGZFlags = EGZFlags::none,
	StreamSize aInputBufferSize = kGZDefDecompInBuffer, 
	StreamSize aOutputBufferSize = kGZDefDecompOutBuffer
);

/** \brief Filter: .gz compressed stream writer
 *
 * Create a stream that produces a .gz-compatible compressed output that is
 * forwarded to the underlying stream (which must be writable). The stream
 * includes the necessary headers and footers (unless disabled via \a
 * EGZFlags).
 *
 * \warning The .gz file footer is only written when the stream is destroyed.
 * Care should be taken to destroy the .gz stream before resuming other write
 * operations to the underlying stream object. (As a limitation to the current
 * API, the error cannot be reported, as it would involve throwing from the
 * destructor. The current implementation will write an error to the log.)
 *
 * \note When outputting the footer, the CRC32 checksum is computed. This has
 * some overhead. Omitting the footer will avoid this, at the cost of producing
 * .gz-incompatible output.
 */
unique_ptr<Stream> stream_create_gz_write( 
	ftl::not_null<unique_ptr<Stream>>,
	unsigned aCompLevel = kGZDefCompLevel,
	EGZFlags = EGZFlags::none,
	char const* aOriginalName = nullptr,
	char const* aComment = nullptr
);
/** \copydoc stream_create_gz_write() */
unique_ptr<Stream> stream_create_gz_write( 
	ftl::not_null<Stream*>,
	unsigned aCompLevel = kGZDefCompLevel,
	EGZFlags = EGZFlags::none,
	char const* aOriginalName = nullptr,
	char const* aComment = nullptr
);

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_STREAM_GZ_HPP_CCD9BFA3_DE28_422B_B777_658EB6A0F97F
