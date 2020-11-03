/*-******************************************************* -- HEADER -{{{1- */
/*-	Very basic HTTP GET io::Stream
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_HTTP_HPP_898FCBF6_A989_48E7_BF27_11837E20D0C2
#define FLUX_IO_HTTP_HPP_898FCBF6_A989_48E7_BF27_11837E20D0C2

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/io/stream.hpp>
#include <flux/std/unique_ptr.hpp>

#include <flux/compat/string_view.hpp>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    constants                       ///{{{2///////////////////////////////

/** \brief HTTP: never timeout
 *
 * \see stream_create_httpget
 */
constexpr unsigned kHttpForever        =  ~0u;
/* \brief HTTP: receive buffer
 *
 * \see stream_create_httpget
 */
constexpr std::size_t kHttpBufferSize  = 16*1024;

//--    functions                       ///{{{2///////////////////////////////

/** \brief Simple HTTP stream
 *
 * Return a `Stream` that reads from a resource via plain HTTP. Data is
 * received on demand (in calls to `read()`) - it is thus not necessary to
 * fully buffer the to-be-received resource.
 *
 * Since the duration of network operations can be somewhat unpredictable,
 * several timeouts can be set. Any timeout set to `kHttpForever` causes the
 * timeout to be ignored.
 *
 *   - \a aStreamLifetimeMS controls the total lifetime of the stream,
 *     including establishing the initial connection, performing the HTTP
 *     request and receiving the resulting document/data. In essence, the time
 *     of the call to `stream_create_httpget()` is recorded, and if more than
 *     \a aStreamLifetimeMS milliseconds have elapsed since, the stream will
 *     expire (`status()` returns `EStreamStatus::error` and `read()` will
 *     throw a `error::StreamExpired` exception). The stream may expire before
 *     the connection is fully established, in which case
 *     `stream_create_httpget()` will throw `error::StreamExpired`.
 *   - \a aConnTimeoutMS controls the maximal duration (in milliseconds) that
 *     establishing the connection may take. This includes time for address
 *     resolution, setting up a TCP connection, sending the HTTP request and
 *     receiving the HTTP response. If the connection cannot be established in
 *     the specified time, `stream_create_httpget()` will throw
 *     `error::StreamExpired`.
 *   - \a aReadTimeoutMS controls the maximal duration (in milliseconds) of a
 *     single `read()` operation. If the mode is `EIoRequest::bestEffort`, and
 *     the time limit is reached, the call will return the data received so far
 *     (which <b>can be zero bytes</b>). If the mode is
 *     `EIoRequest::mustComplete`, the call throws `error::StreamIoIncomplete`
 *     (as the request could not be fully satisfied in the given time).
 *
 * \a aRecvBufferSize defines the size of the internal buffer used to receive
 * data. This buffer must be large enough to hold individual lines in the HTTP
 * response, as parsing of this response will fail otherwise. (Both the stream
 * and the buffer are part of a single allocation, so the buffer will only be
 * freed once the returned stream is destroyed.)
 *
 * <b>Unsupported</b> features by `stream_create_httpget()` include (but are
 * not limited to):
 *   - Chunked transfer encoding (TODO)
 *   - Compressed transfers
 *   - Following redirects (TODO-maybe, control via option, and limit to a
 *     fixed number!)
 *   - SSL/HTTPS, or HTTP 2.0
 */
unique_ptr<Stream> stream_create_httpget( 
	compat::string_view const& aURL,
	unsigned aStreamLifetimeMS = kHttpForever,
	unsigned aConnTimeoutMS = kHttpForever,
	unsigned aReadTimeoutMS = kHttpForever,
	std::size_t aRecvBufferSize = kHttpBufferSize
	//TODO: additional headers? Range request would be useful.
);

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_HTTP_HPP_898FCBF6_A989_48E7_BF27_11837E20D0C2
