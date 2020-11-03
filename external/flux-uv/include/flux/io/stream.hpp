/*-******************************************************* -- HEADER -{{{1- */
/*-	Simple stream abstraction
 *
 * TODO:
 *   - StreamCaps::skip - can skip forward
 *   - StreamCaps::rewind - can rewind file to start
 *
 *   - Stream::skip() - skip N bytes, essentially seek() forward
 *   - Stream::rewind() - rewind Stream to start.
 *
 *   - ??? EStreamClone::{current, rewind} - passed to clone() to either clone
 *       with current state or clone a rewound stream. + related caps
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_STREAM_HPP_198EF38E_EED9_44AC_AA73_3828C87850DE
#define FLUX_IO_STREAM_HPP_198EF38E_EED9_44AC_AA73_3828C87850DE

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/io/forward.hpp>

#include <flux/compat/constexpr.hpp>

#include <flux/ftl/any.hpp>
#include <flux/std/unique_ptr.hpp>
#include <flux/util/enum_bitfield.hpp>

#include <string>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    constants                       ///{{{2///////////////////////////////

enum class EIoRequest // XXX EStreamRequest / EStreamOpRequest
{
	bestEffort,
	mustComplete
};

enum class ESeekWhence
{
	set,
	cur,
	end
};

enum class EStreamStatus : unsigned
{
	readable = (1u<<0),
	writable = (1u<<1),
	
	error = (1u<<15)
};
FLUX_UTIL_DEFINE_ENUM_BITFIELD( EStreamStatus );

//--    types                           ///{{{2///////////////////////////////

using StreamOffset = ssize_t; //TODO: make signed of StreamSize

struct StreamCaps
{
	FLUX_CONSTEXPR_EX StreamCaps() noexcept;
	
	unsigned readable : 1; //!< stream is capable of reading
	unsigned writable : 1; //!< stream is capable writing

	unsigned size : 1; //!< stream's size is known
	unsigned tell : 1; //!< stream's current position can be retrieved
	unsigned seekFwd : 1; //!< can seek forward in stream
	unsigned seekRev : 1; //!< can seek backward in stream
	unsigned remaining : 1; //!< bytes remaining in stream is known

	unsigned infinite : 1; //!< stream will produce infinite amounts of data

	// Experimental:
	unsigned clonable : 1;
#	if 0
	unsigned lockable : 1;
#	endif
};

//--    Stream                          ///{{{2///////////////////////////////

struct Stream
{
	virtual ~Stream() = 0;

	virtual StreamSize read( void*, StreamSize, EIoRequest = EIoRequest::bestEffort );
	virtual StreamSize write( void const*, StreamSize, EIoRequest = EIoRequest::bestEffort );

	/** Return stream status
	 *
	 * Returns the `Stream`'s current status. 
	 * TODO
	 *
	 * \note `status()` may return a status with `EStreamStatus::readable` set,
	 * even though there are no bytes remaining in the stream. In this case,
	 * the `read()` will succeed but return zero bytes. After this, the status
	 * will correctly indicate that the stream is no longer readable. This
	 * reflects the behaviour of the underlying APIs.
	 */
	virtual EStreamStatus status() const noexcept = 0;

	virtual StreamCaps caps() const noexcept = 0;
	virtual StreamSize size() const;
	virtual StreamSize tell() const;
	virtual StreamSize seek( StreamOffset, ESeekWhence = ESeekWhence::set );
	virtual StreamSize remaining() const;

	//virtual StreamSize skip( StreamSize ) const;
	//virtual void rewind() const;

	virtual void flush();

	virtual std::string description() const = 0;
	virtual ftl::Any native_handle() const = 0;

	// experimental
	virtual unique_ptr<Stream> clone() const;

#	if 0
	// TODO: when not lockable, should lock() / unlock() silently fail or 
	// throw?
	//
	//   Silently fail: default implementations can easily lock/unlock by default
	//   Throw: makes more sense if not implemented.
	//
	// Maybe throw if lockable is set to true
	virtual void lock( StreamSize const* = nullptr );
	virtual void unlock( StreamSize* = nullptr );
	// TODO: maybe bool try_lock(), possibly with timeout
#	endif
};

//--    functions                       ///{{{2///////////////////////////////

std::string to_string( EIoRequest );
std::string to_string( ESeekWhence );
std::string to_string( EStreamMode );
std::string to_string( EStreamStatus );

std::string to_string( StreamCaps );

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/stream.inl"
#endif // FLUX_IO_STREAM_HPP_198EF38E_EED9_44AC_AA73_3828C87850DE
