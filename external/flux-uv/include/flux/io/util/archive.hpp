/*-******************************************************* -- HEADER -{{{1- */
/*-	Utility: write archives
 *
 * This is mainly intended for offline use (e.g., to bake final data).
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_UTIL_ARCHIVE_HPP_F5C69019_A6B9_40D5_9F37_C0758A2BC75C
#define FLUX_IO_UTIL_ARCHIVE_HPP_F5C69019_A6B9_40D5_9F37_C0758A2BC75C

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/io/forward.hpp>

#include <flux/ftl/not_null.hpp>
#include <flux/std/unique_ptr.hpp>

#include <flux/util/enum_bitfield.hpp>

//--    >>> namespace = flux::io::util >>>      ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,util)
//--    constants                       ///{{{2///////////////////////////////

enum class EArchiveEntryFlags
{
	none = 0,
	hintUncompressed = (1<<0),
	hintCompressFast = (1<<1),
	hintCompressGood = (1<<2),
	hintCompressBest = (1<<3),
};

FLUX_UTIL_DEFINE_ENUM_BITFIELD( EArchiveEntryFlags );

//--    ArchiveWriter                   ///{{{2///////////////////////////////
struct ArchiveWriter
{
	virtual ~ArchiveWriter() = 0;

	virtual void add_buffer( 
		char const*, 
		ftl::not_null<Buffer*>, 
		EArchiveEntryFlags = EArchiveEntryFlags::none,
		BufferSize = 0, 
		BufferSize = ~BufferSize(0)
	) = 0;
	virtual void add_stream( 
		char const*, 
		ftl::not_null<Stream*>,
		EArchiveEntryFlags = EArchiveEntryFlags::none,
		StreamSize = ~StreamSize(0)
	) = 0;
	virtual void add_memory(
		char const*,
		void const*,
		std::size_t,
		EArchiveEntryFlags = EArchiveEntryFlags::none
	) = 0;
};


//--    functions                       ///{{{2///////////////////////////////

/** Zip Archive Writer
 *
 * The \a aDefaultLevel argument specifies the default compression level. It
 * accepts a value \f$\in \left[0, 10\right]\f$, Levels zero to nine are the
 * default zlib levels, ranging from no compression to best compression. Level
 * 10 is a new level for `miniz.h` with better compression but possibly being
 * very slow (unfortunately, `miniz.h` does not go all the way to 11).
 *
 * Compression level can be overridden via the `EArchiveEntryFlags` argument
 * for each individual entry. The hints are translated as follows:
 *   - `EArchiveEntryFlags::hintUncompressed`: `MZ_NO_COMPRESSION` = level 0
 *   - `EArchiveEntryFlags::hintCompressFast`: `MZ_BEST_SPEED` = level 1
 *   - `EArchiveEntryFlags::hintCompressGood`: `MZ_BEST_COMPRESSION` = level 9
 *   - `EArchiveEntryFlags::hintCompressBest`: `MZ_UBER_COMPRESSION` = level 10
 * 
 * \note The `miniz.h` library only supports writing from memory, meaning that
 * the whole input data has to be loaded into memory. As such, prefer the
 * `add_buffer()` / `add_memory()` methods over `add_stream()`. The latter will
 * attempt to buffer the whole stream (`buffer_create_stream()`) and is
 * restricted to `Stream`s where this is possible.
 */
unique_ptr<ArchiveWriter> archive_writer_create_zip( 
	VfsRecord const&, 
	char const*,
	int aDefaultLevel = 6
);

//TODO one with just a stream input.

//--    <<< ~ flux::io::util namespace <<<      ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,util)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_UTIL_ARCHIVE_HPP_F5C69019_A6B9_40D5_9F37_C0758A2BC75C
