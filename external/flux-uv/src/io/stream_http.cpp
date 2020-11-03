/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	HTTP Stream implementation
 *
 *  TODO: transfer encoding chunked
 *  TODO: the strategy of trying to read until end-of-stream or until the
 *    buffer has been filled even with Mode = bestEffort is perhaps not
 *    the best. Slow connection + low timeout => timeout instead of 
 *    returning *something*. Note that this only matters if a partial
 *    stream is useful (and if people are making very large reads).
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/stream/http.hpp>
#include <flux/io/errors.hpp>

#include <flux/std/byte.hpp>
#include <flux/std/clock.hpp>
#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>

#include <flux/util/narrow.hpp>
#include <flux/util/scope_exit.hpp>

#include <flux/sys/addrinfo.hpp>
#include <flux/sys/errno_category.hpp>
#include <flux/sys/win32_category.hpp>


#if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
#	define FLUXIO_USE_WINSOCK_ 1
#	include <flux_dlapi/os/winsock.hpp>
#	include <flux_dlapi/os/dlapi_category.hpp>

#else // other platforms
#	define FLUXIO_USE_WINSOCK_ 0
#	include <errno.h>
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netinet/in.h>

#endif // ~ platform

#include <flux_ext/pegtl.hpp>
#include <tao/pegtl/contrib/uri.hpp> //XXX (?)
#include <tao/pegtl/contrib/http.hpp> //XXX (?)

#include <sstream>
#include <cstring>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	using std::chrono::duration_cast;
	using Us_ = std::chrono::microseconds;
	using Ms_ = std::chrono::milliseconds;
	using Sec_ = std::chrono::seconds;
	
#	if !FLUXIO_USE_WINSOCK_
#		define FLUXIO_WSARG_(x)

	using SocketType_ = int;
	using Timeval_ = ::timeval;

	constexpr int kSocketError_ = -1;
	constexpr SocketType_ kInvalidSock_ = -1;

	//constexpr int shut_rd_ = SHUT_RD;
	constexpr int shut_rdwr_ = SHUT_RDWR;
#	else // USE_WINSOCK_
#		define FLUXIO_WSARG_(x) , x

	namespace Ws2_ = dlapi::os::Ws2;

	using Ws2api = dlapi::os::Ws2api;
	using SocketType_ = Ws2_::SOCKET;
	using Timeval_ = Ws2_::timeval;

	constexpr int kSocketError_ = Ws2_::socket_error;
	constexpr SocketType_ kInvalidSock_ = Ws2_::invalid_socket;

	constexpr int shut_rd_ = Ws2_::sd_receive;
	constexpr int shut_rdwr_ = Ws2_::sd_both;
#	endif // ~ apis

	class StreamHTTPGet_ final : public Stream
	{
		public:	
			StreamHTTPGet_( 
				SocketType_,
				std::string aURL,
				flux::byte* aRecvBufferBase, 
				std::size_t aRecvBufferSize,
				flux::byte* aRecvBufferBeg,
				flux::byte* aRecvBufferEnd,
				bool aChunkedEncoding,
				StreamSize aContentLength,
				std::string aContentType,
				unsigned aReadTimeoutMS,
				unsigned aMaximumStreamLifetimeMS,
				int aShutdown
			);
			~StreamHTTPGet_();

		public: /* virtuals from Stream */
			StreamSize read( void*, StreamSize, EIoRequest ) override;
			StreamSize write( void const*, StreamSize, EIoRequest ) override;

			EStreamStatus status() const noexcept override;

			StreamCaps caps() const noexcept override;
			StreamSize size() const override;
			StreamSize tell() const override;
			StreamSize seek( StreamOffset, ESeekWhence ) override;
			StreamSize remaining() const override;

			void flush() override;

			std::string description() const override;
			ftl::Any native_handle() const override;

		private:
			void close_() noexcept;

		private:
			SocketType_ mSocket;

			flux::byte* const mBufferBase;
			flux::byte* const mBufferCapacity;
			flux::byte* mBufferRead;
			flux::byte* mBufferWrite;

#			if FLUXIO_USE_WINSOCK_
			Ws2api const* mWS2;
#			endif // ~ USE_WINSOCK_
			
			std::string mURL;
			std::string mContentType;

			Clock::time_point const mCreationTime;

			StreamSize mContentLength;
			StreamSize mCurrentPos;
			//TODO stuff for chunky stuff
			
			unsigned mReadTimeoutMS;
			unsigned mStreamLifetimeMS;

			int mShutdown;

			//bool mTransferChunked; // Not yet implemented.
	};

	enum class EHeaderStatus_
	{
		cont,
		done
	};
	enum class EHeaderState_
	{
		status,
		content
	};

	struct URLThings_
	{
		std::string scheme;
		std::string host;
		std::string port;
		std::string path;
		std::string query;

		bool hadUserPart = false;
	};
	struct HTTPThings_
	{
		int code = -1;
		std::string reason;
		
		std::size_t contentLength = ~std::size_t(0);
		std::string contentType;

		bool transferChunked = false;
		bool acceptRangeBytes = false;
		bool connectionKeepAlive = false;

		EHeaderState_ state = EHeaderState_::status;
	};

	URLThings_ parse_url_( char const*, char const* );
	EHeaderStatus_ parse_http_( HTTPThings_&, flux::byte*, flux::byte*&, flux::byte*& );

	std::string generate_get_( URLThings_ const& );
	//std::string generate_head_( URLThings_ const& );


	// timeouts
	enum class ESelOp_
	{
		rd = 0,
		wr = 1,
		ex = 2
	};

	bool check_lifetime_soft_( 
		unsigned aLifetime,
		Clock::time_point aBeginTime,
		unsigned* aRemaining = nullptr
	) noexcept;
	void check_lifetime_( 
		unsigned aLifetime,
		Clock::time_point aBeginTime,
		unsigned* aRemaining = nullptr
	);
	bool wait_for_socket_(
		ESelOp_,
		SocketType_,
		unsigned aRemaining
		FLUXIO_WSARG_(Ws2api const*)
	);
}

//--    stream_create_httpget()             ///{{{2///////////////////////////

unique_ptr<Stream> stream_create_httpget( compat::string_view const& aURL, unsigned aStreamLifetimeMS, unsigned aConnTimeoutMS, unsigned aReadTimeoutMS, std::size_t aRecvBufferSize )
{
	auto const tbegin = Clock::now();
	auto const lifetime = std::min( aStreamLifetimeMS, aConnTimeoutMS );

	// Windows: winsock
#	if FLUXIO_USE_WINSOCK_
	auto const* ws2 = dlapi::os::ws2();
	if( !ws2 )
	{
		namespace dlos_ = dlapi::os;
		FLUX_THROW( error::StreamSysError )
			<< flux::einfo::ApiFunction( "flux::dlapi::os::ws2" )
			<< flux::einfo::ApiError( dlos_::make_dlapi_error_code( dlos_::EDLAPIError::notLoaded ) )
			<< flux::einfo::ErrorDesc( "Winsock2 API unavailable" )
		;
	}
#	endif // ~ USE_WINSOCK_

	// parse URL
	auto const url = parse_url_( aURL.data(), aURL.data()+aURL.size() );

	FLUX_THROW_IF( "http" != url.scheme, error::StreamNotAvailable )
		<< flux::einfo::ErrorDesc( "Unsupported URL: scheme not supported" )
		<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("URL = '{}'"), aURL );
	;
	FLUX_THROW_IF( url.hadUserPart, error::StreamNotAvailable )
		<< flux::einfo::ErrorDesc( "Unsupported URL: user specification not supported" )
		<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("URL = '{}'"), aURL );
	;
	
	// resolve hostname
	unsigned remaining = kHttpForever;
	check_lifetime_( lifetime, tbegin, &remaining );

	sys::AddrHints hint;
	hint.family  = sys::EAddrFamily::unspec;
	hint.type    = sys::ESockType::stream;
	hint.proto   = sys::EIpProto::tcp;
	hint.flags   = sys::EResolveFlags::v4mapped | sys::EResolveFlags::addrconfig;
	
	compat::error_code ec0;
	auto addr = sys::get_addr_info_timeout(
		url.host.c_str(),
		url.port.empty() ? url.scheme.c_str() : url.port.c_str(),
		Ms_(remaining),
		hint,
		&ec0
	);

	if( !addr )
	{
		FLUX_THROW_IF( compat::errc::timed_out == ec0, error::StreamExpired )
			<< flux::einfo::ApiFunction( "flux::sys::get_addr_info_timeout" )
			<< flux::einfo::ApiError( ec0 )
			<< flux::einfo::ErrorDesc( "Timed out while resolving address" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("host='{}', service='{}', port='{}'"), url.host, url.scheme, url.port )
		;

		FLUX_THROW( error::StreamNotAvailable )
			<< flux::einfo::ApiFunction( "flux::sys::get_addr_info_timeout" )
			<< flux::einfo::ApiError( ec0 )
			<< flux::einfo::ErrorDesc( "Couldn't resolve address" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("host='{}', service='{}', port='{}'"), url.host, url.scheme, url.port )
		;
	}
	
	// Initiate connection
	bool connected = false;

#	if !FLUXIO_USE_WINSOCK_
	auto sock = ::socket( addr->address_family(), SOCK_STREAM, IPPROTO_TCP );
	if( kInvalidSock_ == sock )
	{
		auto const errno_ = errno;
		FLUX_THROW( error::StreamSysError )
			<< flux::einfo::ApiFunction( "::socket" )
			<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
		;
	}

	FLUX_UTIL_ON_SCOPE_EXIT
	{
		if( kInvalidSock_ != sock )
		{
			if( connected )
				::shutdown( sock, SHUT_RDWR );

			::close( sock );
		}
	};
	
	auto flags = ::fcntl( sock, F_GETFL, 0 );
	if( -1 == ::fcntl( sock, F_SETFL, flags | O_NONBLOCK ) )
	{
		auto const errno_ = errno;
		FLUX_THROW( error::StreamSysError )
			<< flux::einfo::ErrorDesc( "Setting non-blocking mode" )
			<< flux::einfo::ApiFunction( "::fcntl" )
			<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
		;
	}

	auto cret = ::connect( sock, addr->address(), addr->address_length() );
	if( -1 == cret )
	{
		auto const errno_ = errno;
		if( EINPROGRESS != errno_ )
		{
			auto const errno_ = errno;
			FLUX_THROW( error::StreamSysError )
				<< flux::einfo::ApiFunction( "::connect" )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("address = {}"), sys::format_addr(*addr) )
			;
		}
	}

#	else // USE_WINSOCK_
	auto sock = ws2->socket( addr->address_family(), Ws2_::sock_stream, Ws2_::ipproto_tcp );
	if( kInvalidSock_ == sock )
	{
		auto const wsaerr = ws2->wsaGetLastError();
		FLUX_THROW( error::StreamSysError )
			<< flux::einfo::ApiFunction( "Ws2api::socket" )
			<< flux::einfo::ApiError( sys::make_win32_error_code(wsaerr) )
		;
	}

	FLUX_UTIL_ON_SCOPE_EXIT
	{
		if( kInvalidSock_ != sock )
		{	
			if( connected )
				ws2->shutdown( sock, Ws2_::sd_both );

			ws2->closesocket( sock );
		}
	};
	
	uint64_t mode = 1;
	if( Ws2_::socket_error == ws2->ioctlsocket( sock, Ws2_::fionbio, &mode ) )
	{
		auto const wsaerr = ws2->wsaGetLastError();
		FLUX_THROW( error::StreamSysError )
			<< flux::einfo::ErrorDesc( "Setting non-blocking mode" )
			<< flux::einfo::ApiFunction( "Ws2api::ioctlsocket" )
			<< flux::einfo::ApiError( sys::make_win32_error_code(wsaerr) )
		;
	}

	auto cret = ws2->connect( sock, addr->address(), util::narrow<int>(addr->address_length()) );
	if( Ws2_::socket_error == cret )
	{
		auto const werr = ws2->wsaGetLastError();
		if( Ws2_::wsaewouldblock != werr )
		{
			auto const wsaerr = ws2->wsaGetLastError();
			FLUX_THROW( error::StreamSysError )
				<< flux::einfo::ApiFunction( "Ws2api::connect" )
				<< flux::einfo::ApiError( sys::make_win32_error_code(wsaerr) )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("address = {}"), sys::format_addr(*addr) )
			;
		}
	}
#	endif // ~ apis

	// Wait for connection to become ready, and send a header
	auto const headstr = generate_get_( url );

	char const* hbeg = headstr.data();
	char const* hend = hbeg + headstr.size();
	while( hbeg != hend )
	{
		unsigned remaining = kHttpForever;
		check_lifetime_( lifetime, tbegin, &remaining );
		if( !wait_for_socket_( ESelOp_::wr, sock, remaining FLUXIO_WSARG_(ws2) ) )
		{
			FLUX_THROW( error::StreamExpired )
				<< flux::einfo::ErrorDesc( "Timed out sending header" )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("select() with {:d}ms remaining"), remaining )
			;
		}

		connected = true;

#		if !FLUXIO_USE_WINSOCK_
		auto const ret = ::send( sock, hbeg, hend-hbeg, 0 );
		if( -1 == ret )
		{
			auto const errno_ = errno;
			if( EWOULDBLOCK == errno_ )
			{
				FLUX_THROW( error::StreamSysError )
					<< flux::einfo::ApiFunction( "::send" )
					<< flux::einfo::ApiError( sys::make_errno_error_code( errno_ ) )
				;
			}
		}

		hbeg += ret;
#		else // USE_WINSOCK_
		auto const ret = ws2->send( sock, hbeg, util::narrow<int>(hend-hbeg), 0 );
		if( Ws2_::socket_error == ret )
		{
			auto const serr = ws2->wsaGetLastError();
			if( Ws2_::wsaewouldblock != serr )
			{
				FLUX_THROW( error::StreamSysError )
					<< flux::einfo::ApiFunction( "Ws2api::send" )
					<< flux::einfo::ApiError( sys::make_win32_error_code( serr ) )
				;
			}
		}

		hbeg += ret;
#		endif // ~ apis
	}

	// Allocate resulting things and recv buffer
	std::size_t alignedSize = (sizeof(StreamHTTPGet_)+16-1) & ~(16-1);
	std::size_t totalSize = alignedSize + aRecvBufferSize;

	auto* buffer = reinterpret_cast<flux::byte*>(::operator new( totalSize ));
	FLUX_UTIL_ON_SCOPE_EXIT
	{
		if( buffer ) ::operator delete( buffer );
	};

	auto* recvbuff = buffer+alignedSize;
	auto* recvstart = recvbuff;
	auto* recvend = recvbuff;

	// Receive response
	HTTPThings_ http;
	do
	{
		unsigned remaining = kHttpForever;
		check_lifetime_( lifetime, tbegin, &remaining );
		if( !wait_for_socket_( ESelOp_::rd, sock, remaining FLUXIO_WSARG_(ws2) ) )
		{
			FLUX_THROW( error::StreamExpired )
				<< flux::einfo::ErrorDesc( "Timed out receiving http response" )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("select() with {:d}ms remaining"), remaining )
			;
		}

		// Read
#		if !FLUXIO_USE_WINSOCK_
		auto* buff = reinterpret_cast<char*>(recvend);
		auto rret = ::recv( sock, buff, recvbuff+aRecvBufferSize-recvend, 0 );
		if( kSocketError_ == rret )
		{
			auto const errno_ = errno;
			FLUX_THROW( error::StreamSysError )
				<< flux::einfo::ApiFunction( "::recv" )
				<< flux::einfo::ApiError( sys::make_errno_error_code( errno_ ) )
			;
		}

		recvend += rret;
#		else // USE_WINSOCK_
		auto* buff = reinterpret_cast<char*>(recvend);
		auto rret = ws2->recv( sock, buff, util::narrow<int>(recvbuff+aRecvBufferSize-recvend), 0 );
		if( Ws2_::socket_error == rret )
		{
			FLUX_THROW( error::StreamSysError )
				<< flux::einfo::ApiFunction( "Ws2api::recv" )
				<< flux::einfo::ApiError( sys::make_win32_error_code( ws2->wsaGetLastError() ) )
			;
		}

		recvend += rret;
#		endif // ~ apis

		// Handle chunklet.
		auto ret = parse_http_( http, recvbuff, recvstart, recvend );
		if( EHeaderStatus_::done == ret )
			break;

		if( recvend == recvbuff + aRecvBufferSize )
		{
			FLUX_THROW( error::StreamBadOptions )
				<< flux::einfo::ErrorDesc( "Cannot make progress parsing HTTP respose" )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("aRecvBufferSize({:d}) is too small"), aRecvBufferSize )
			;
		}
	} while( true );

	// All done.
	if( 200 != http.code )
	{
		FLUX_THROW_IF( 404 == http.code, error::StreamNotAvailable )
			<< flux::einfo::ErrorDesc( FLUX_FMT_STRING("Not found (http 404 {})"), http.reason )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("host = '{}', path = '{}{}{}"), url.host, url.path, url.query.empty() ? "":"?", url.query )
		;

		char const* eclass = "Unhandled response";
		if( http.code >= 400 && http.code < 500 )
			eclass = "Client error";
		else if( http.code >= 500 && http.code < 600 )
			eclass = "Server error";

		FLUX_THROW( error::StreamError )
			<< flux::einfo::ErrorDesc( FLUX_FMT_STRING("{} (http {:d} {})"), eclass, http.code, http.reason )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("host = '{}', path = '{}{}{}"), url.host, url.path, url.query.empty() ? "":"?", url.query )
		;
	}

	//TODO
	//TODO  Currently unhandled:
	if( http.transferChunked )
	{
		FLUX_THROW( error::StreamNotAvailable )
			<< flux::einfo::ErrorDesc( "Chunked transfer enconding not yet supported" )
		;
	}

	// Create stream and return
	if( kHttpForever != aStreamLifetimeMS )
	{
		auto const tnow = Clock::now();
		auto const elapsed = util::narrow<unsigned>(duration_cast<Ms_>(tnow-tbegin).count());

		FLUX_THROW_IF( elapsed >= lifetime, error::StreamExpired )
			<< flux::einfo::ErrorDesc( "Timeout" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("lifetime = {:d}ms, elapsed = {:d}ms"), lifetime, elapsed )
		;	
		
		aStreamLifetimeMS -= elapsed;
	}

	auto* stream = new (buffer) StreamHTTPGet_( 
		sock, std::string(aURL),
		recvbuff, aRecvBufferSize,
		recvstart, recvend,
		http.transferChunked,
		util::narrow<StreamSize>(http.contentLength),
		std::move(http.contentType),
		aReadTimeoutMS,
		aStreamLifetimeMS,
		shut_rdwr_ //shut_rd_ // <<- Apparently shutting down one side early messes things up
	);

	buffer = nullptr;
	sock = kInvalidSock_;

	return unique_ptr<Stream>(stream);
}

//--    $ StreamHTTPGet_                    ///{{{2///////////////////////////
namespace
{
	StreamHTTPGet_::StreamHTTPGet_( SocketType_ aSocket, std::string aURL, flux::byte* aRecvBufferBase, std::size_t aRecvBufferSize, flux::byte* aRecvBufferBeg, flux::byte* aRecvBufferEnd, bool /*aChunkedEncoding*/, StreamSize aContentLength, std::string aContentType, unsigned aReadTimeoutMS, unsigned aMaximumStreamLifetimeMS, int aShutdown )
		: mSocket( aSocket )
		, mBufferBase( aRecvBufferBase )
		, mBufferCapacity( aRecvBufferBase + aRecvBufferSize )
		, mBufferRead( aRecvBufferBeg )
		, mBufferWrite( aRecvBufferEnd )
		, mURL( std::move(aURL) )
		, mContentType( std::move(aContentType) )
		, mCreationTime( Clock::now() )
		, mContentLength( aContentLength )
		, mCurrentPos( 0 )
		, mReadTimeoutMS( aReadTimeoutMS )
		, mStreamLifetimeMS( aMaximumStreamLifetimeMS )
		, mShutdown( aShutdown )
		//, mTransferChunked( aChunkedEncoding )
	{
#		if FLUXIO_USE_WINSOCK_
		mWS2 = dlapi::os::ws2();
		FLUX_ASSERT( mWS2 );
#		endif // ~ USE_WINSOCK_
	}

	StreamHTTPGet_::~StreamHTTPGet_()
	{
		close_();
	}

	StreamSize StreamHTTPGet_::read( void* aBuffer, StreamSize aSize, EIoRequest aReq )
	{
		FLUX_ASSERT( aBuffer );

		//TODO: split into read_plain_()
		//TODO: split into read_chunked_()

		// Has the stream expired already?
		unsigned readtime = kHttpForever;
		check_lifetime_( mStreamLifetimeMS, mCreationTime, &readtime );

		readtime = std::min( readtime, mReadTimeoutMS );
		auto const readBegin = Clock::now();
		
		// Read
		char const* reason = "";

		auto* optr = reinterpret_cast<flux::byte*>(aBuffer);
		auto needed = aSize;
		while( needed && (kInvalidSock_ != mSocket || mBufferRead != mBufferWrite) )
		{
			if( mBufferRead != mBufferWrite )
			{
				auto const got = util::narrow<StreamSize>(mBufferWrite - mBufferRead);
				auto const ret = std::min( needed, got );

				std::memcpy( optr, mBufferRead, ret );

				optr += ret;
				needed -= ret;
				mBufferRead += ret;
				mCurrentPos += ret;

				if( mBufferRead == mBufferWrite )
					mBufferRead = mBufferWrite = mBufferBase;

				if( !needed || (mCurrentPos == mContentLength) )
					break;
			}

			FLUX_ASSERT( mBufferWrite != mBufferCapacity );

			if( kInvalidSock_ != mSocket )
			{
				// Wait for data to become available
				unsigned remaining = kHttpForever;
				if( !check_lifetime_soft_( readtime, readBegin, &remaining ) )
				{
					reason = " (timeout while receiving)";
					break;
				}
				if( !wait_for_socket_( ESelOp_::rd, mSocket, remaining FLUXIO_WSARG_(mWS2) ) )
				{
					reason = " (timeout while waiting for data)";
					break;
				}

				// Receive
#				if !FLUXIO_USE_WINSOCK_
				auto space = mBufferCapacity - mBufferWrite;
				auto rret = ::recv( mSocket, mBufferWrite, space, 0 );
				if( kSocketError_ == rret )
				{

					//TODO: set error status?

					auto const errno_ = errno;
					FLUX_THROW( error::StreamSysError )
						<< flux::einfo::ApiFunction( "::recv" )
						<< flux::einfo::ApiError( sys::make_errno_error_code( errno_ ) )
						<< einfo::StreamMode( EStreamMode::read )
						<< einfo::StreamDesc( description() )
					;
				}
				else if( 0 == rret )
				{
					close_();
					break;
				}

				mBufferWrite += util::narrow<std::size_t>(rret);
#				else // USE_WINSOCK_
				auto space = mBufferCapacity - mBufferWrite;
				auto rret = mWS2->recv( mSocket, reinterpret_cast<char*>(mBufferWrite), util::narrow<int>(space), 0 );
				if( kSocketError_ == rret )
				{
					//TODO: set error status?

					FLUX_THROW( error::StreamSysError )
						<< flux::einfo::ApiFunction( "Ws2api::recv" )
						<< flux::einfo::ApiError( sys::make_win32_error_code( mWS2->wsaGetLastError() ) )
						<< einfo::StreamMode( EStreamMode::read )
						<< einfo::StreamDesc( description() )
					;
				}
				else if( 0 == rret )
				{
					close_();
					break;
				}

				mBufferWrite += util::narrow<std::size_t>(rret);
#				endif // ~ apis
			}
		}

		auto const total = aSize-needed;
		if( needed && (EIoRequest::mustComplete == aReq) )
		{
			FLUX_THROW( error::StreamIoIncomplete )
				<< einfo::StreamMode( EStreamMode::read )
				<< einfo::StreamDesc( description() )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("requested: {:d}, got: {:d}{}"), aSize, total, reason )
			;
		}

		//mCurrentPos += total;
		return total;
	}
	StreamSize StreamHTTPGet_::write( void const*, StreamSize, EIoRequest )
	{
		FLUX_THROW( error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
		;
	}

	EStreamStatus StreamHTTPGet_::status() const noexcept
	{
		auto const elapsed = util::narrow<unsigned>(duration_cast<Ms_>(Clock::now()-mCreationTime).count());
		if( elapsed >= mStreamLifetimeMS )
			return EStreamStatus::error;
		
		if( mSocket == kInvalidSock_ && mBufferRead == mBufferWrite )
			return EStreamStatus(0);

		if( mContentLength == mCurrentPos )
			return EStreamStatus(0);

		return EStreamStatus::readable;
	}

	StreamCaps StreamHTTPGet_::caps() const noexcept
	{
		StreamCaps caps;
		caps.readable = true;
		caps.writable = false;
		caps.size = !!(~StreamSize(0) != mContentLength);
		caps.tell = true;
		caps.seekFwd = false; //XXX XXX XXX
		caps.seekRev = false;
		caps.remaining = !!(~StreamSize(0) != mContentLength);
		caps.infinite = false;
		caps.clonable = false;
		return caps;
	}

	StreamSize StreamHTTPGet_::size() const
	{
		FLUX_THROW_IF( ~StreamSize(0) == mContentLength, io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::size()" )
		;

		return mContentLength;
	}

	StreamSize StreamHTTPGet_::tell() const
	{
		return mCurrentPos;
	}
	StreamSize StreamHTTPGet_::seek( StreamOffset, ESeekWhence )
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::seek()" )
		;
	}
	StreamSize StreamHTTPGet_::remaining() const
	{
		FLUX_THROW_IF( ~StreamSize(0) == mContentLength, io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::remaining()" )
		;

		return mContentLength - mCurrentPos;
	}

	void StreamHTTPGet_::flush()
	{}

	std::string StreamHTTPGet_::description() const
	{
		return format( "{}[{}]", mURL, mContentType );
	}
	ftl::Any StreamHTTPGet_::native_handle() const
	{
		return ftl::Any(mSocket);
	}

	void StreamHTTPGet_::close_() noexcept
	{
		if( kInvalidSock_ != mSocket )
		{
#			if FLUXIO_USE_WINSOCK_
			namespace Ws2_ = dlapi::os::Ws2;
			mWS2->shutdown( mSocket, mShutdown );
			mWS2->closesocket( mSocket );
#			else
			::shutdown( mSocket, mShutdown );
			::close( mSocket );
#			endif

			mSocket = kInvalidSock_;
		}
	}
}

//--    $ parse_url_()                      ///{{{2///////////////////////////
namespace
{
	std::string generate_get_( URLThings_ const& aURL )
	{
		std::ostringstream oss;
		flux::print( oss, "GET {}{}{} HTTP/1.1\r\n", 
			aURL.path, 
			aURL.query.empty() ? "" : "?",
			aURL.query
		);
		flux::print( oss, "Host: {}\r\n", aURL.host );
		flux::print( oss, "Connection: close\r\n" );
		flux::print( oss, "\r\n" );
		return oss.str();
	}

#	if 0
	std::string generate_head_( URLThings_ const& aURL )
	{
		std::ostringstream oss;
		flux::print( oss, "HEAD {}{}{} HTTP/1.1\r\n", 
			aURL.path, 
			aURL.query.empty() ? "" : "?",
			aURL.query
		);
		flux::print( oss, "Host: {}\r\n", aURL.host );
		flux::print( oss, "Connection: keep-alive\r\n" );
		flux::print( oss, "\r\n" );
		return oss.str();
	}
#	endif
}

//--    $ parse_url_()                      ///{{{2///////////////////////////
namespace
{
	namespace urlparser_
	{
		namespace pegtl = flux_ext::pegtl;
		namespace uri = flux_ext::pegtl::uri;

		template< std::string URLThings_::*tField >
		struct BindStr
		{
			template< class tInput >
			static void apply( tInput const& aInput, URLThings_& aURL )
			{
				aURL.*tField = aInput.string();
			}
		};

		using Grammar = pegtl::must<
			uri::absolute_URI,
			pegtl::eof
		>;

		template< typename tRule >
		struct Action
			: pegtl::nothing< tRule >
		{};

		template<> struct Action< uri::scheme > : BindStr< &URLThings_::scheme > {};
		template<> struct Action< uri::host > : BindStr< &URLThings_::host > {};
		template<> struct Action< uri::port > : BindStr< &URLThings_::port > {};
		template<> struct Action< uri::path_noscheme > : BindStr< &URLThings_::path > {};
		template<> struct Action< uri::path_rootless > : BindStr< &URLThings_::path > {};
		template<> struct Action< uri::path_absolute > : BindStr< &URLThings_::path > {};
		template<> struct Action< uri::path_abempty > : BindStr< &URLThings_::path > {};
		template<> struct Action< uri::query > : BindStr< &URLThings_::query > {};

		template<> struct Action< uri::opt_userinfo > {
			template< class tInput >
			static void apply( tInput const& aInput, URLThings_& aURL )
			{
				aURL.hadUserPart = !aInput.empty();
			}
		};
	}

	URLThings_ parse_url_( char const* aBegin, char const* aEnd )
	{
		FLUX_ASSERT( aBegin && aEnd );

		flux_ext::pegtl::memory_input<> in( aBegin, aEnd, "url" );

		URLThings_ ret;

		try
		{
			flux_ext::pegtl::parse<
				urlparser_::Grammar,
				urlparser_::Action
			>( in, ret );
		}
		catch( flux_ext::pegtl::parse_error const& eErr )
		{
			auto const& pos = eErr.positions.front();
			auto const& line = in.line_at( pos );

			auto const* head = line.data();
			auto const* errc = head + pos.byte_in_line;
			auto const* tail = errc+1;
			auto const* end = line.data()+line.size();

			FLUX_THROW( error::StreamNotAvailable )
				<< flux::einfo::ErrorDesc( "Cannot parse URL" )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("URL = {}[{:c}]{}"), compat::string_view(head,errc-head), *errc, compat::string_view(tail,end-tail) )
			;
		}

		return ret;
	}
}

//--    $ parse_http_()                     ///{{{2///////////////////////////
namespace
{
	namespace httpparser_
	{
		namespace pegtl = flux_ext::pegtl;
		namespace http = flux_ext::pegtl::http;
		namespace abnf = flux_ext::pegtl::abnf;

		// helpers
		template< bool HTTPThings_::*tField, bool tValue = true >
		struct BindBool
		{
			static void apply0( HTTPThings_& aHttp ) noexcept
			{
				aHttp.*tField = tValue;
			}
		};

		// Status line: 
		// http::status_line requires a reason to be given after the status
		// code, but at minimum some nginx versions seem to omit that for 404
		// errors.
		using GrammarStatus = pegtl::seq<
			http::HTTP_version,
			abnf::SP,
			http::status_code,
			pegtl::star<
				abnf::SP,
				http::reason_phrase
			>,
			abnf::CRLF
		>;
	
		template< typename tRule >
		struct ActionStatus
			: pegtl::nothing< tRule >
		{};

		template<> struct ActionStatus< http::status_code > {
			template< class tInput >
			static void apply( tInput const& aInput, HTTPThings_& aHttp )
			{
				aHttp.code = std::stoi( aInput.string() );
			}
		};
		template<> struct ActionStatus< http::reason_phrase > {
			template< class tInput >
			static void apply( tInput const& aInput, HTTPThings_& aHttp )
			{
				aHttp.reason = aInput.string();
			}
		};


		// Other lines: need to cobble together our own.
		struct ContentTypeValue : http::field_value {};
		using ContentType = pegtl::if_must< 
			TAO_PEGTL_ISTRING( "content-type" ),
			pegtl::one< ':' >,
			http::OWS,
			ContentTypeValue,
			http::OWS,
			abnf::CRLF 
		>;

		struct ContentLengthValue : http::field_value {};
		using ContentLength = pegtl::if_must< 
			TAO_PEGTL_ISTRING( "content-length" ),
			pegtl::one< ':' >,
			http::OWS,
			ContentLengthValue,
			http::OWS,
			abnf::CRLF 
		>;

		struct TransferEncodingChunked : TAO_PEGTL_ISTRING( "chunked" ) {};
		using TransferEncodingOther = http::token;
		using TransferEncodingValue = pegtl::sor<
			TransferEncodingChunked, 
			TransferEncodingOther
		>;
		using TransferEncoding = pegtl::if_must<
			TAO_PEGTL_ISTRING( "transfer-encoding" ),
			pegtl::one< ':' >,
			http::OWS,
			TransferEncodingValue,
			pegtl::star<
				http::OWS,
				pegtl::one< ',' >,
				http::OWS,
				TransferEncodingValue
			>,
			http::OWS,
			abnf::CRLF 
		>;

		struct AcceptRangesBytes : TAO_PEGTL_ISTRING( "bytes" ) {};
		struct AcceptRangesNone : TAO_PEGTL_ISTRING( "none" ) {};
		using AcceptRangesValue = pegtl::sor<
			AcceptRangesBytes, 
			AcceptRangesNone
		>;
		using AcceptRanges = pegtl::if_must<
			TAO_PEGTL_ISTRING( "accept-ranges" ),
			pegtl::one< ':' >,
			http::OWS,
			AcceptRangesValue,
			http::OWS,
			abnf::CRLF 
		>;

		struct ConnectionKeepAlive : TAO_PEGTL_ISTRING( "keep-alive" ) {};
		using ConnectionOther = http::token;
		using ConnectionValue = pegtl::sor<
			ConnectionKeepAlive, 
			ConnectionOther
		>;
		using Connection = pegtl::if_must<
			TAO_PEGTL_ISTRING( "connection" ),
			pegtl::one< ':' >,
			http::OWS,
			ConnectionValue,
			pegtl::star<
				http::OWS,
				pegtl::one< ',' >,
				http::OWS,
				ConnectionValue
			>,
			http::OWS,
			abnf::CRLF 
		>;

		using GrammarHeader = pegtl::sor<
			ContentType,
			ContentLength,
			TransferEncoding,
			AcceptRanges,
			Connection,
			http::header_field
		>;


		template< typename tRule >
		struct ActionContent
			: pegtl::nothing< tRule >
		{};

		template<> struct ActionContent< ContentTypeValue > {
			template< class tInput >
			static void apply( tInput const& aInput, HTTPThings_& aHttp )
			{
				aHttp.contentType = aInput.string();
			}
		};
		template<> struct ActionContent< ContentLengthValue > {
			template< class tInput >
			static void apply( tInput const& aInput, HTTPThings_& aHttp )
			{
				aHttp.contentLength = util::narrow<std::size_t>(std::stoull( aInput.string() ));
			}
		};

		template<> struct ActionContent< TransferEncodingChunked > 
			: BindBool< &HTTPThings_::transferChunked, true >
		{};

		template<> struct ActionContent< AcceptRangesBytes >
			: BindBool< &HTTPThings_::acceptRangeBytes, true >
		{};
		template<> struct ActionContent< AcceptRangesNone >
			: BindBool< &HTTPThings_::acceptRangeBytes, false >
		{};

		template<> struct ActionContent< ConnectionKeepAlive >
			: BindBool< &HTTPThings_::connectionKeepAlive, true >
		{};
	}

	EHeaderStatus_ parse_http_( HTTPThings_& aHttp, flux::byte* aBase, flux::byte*& aBeg, flux::byte*& aEnd )
	{
		auto const* beg = reinterpret_cast<char const*>(aBeg);
		auto const* end = reinterpret_cast<char const*>(aEnd);

		while( beg != end )
		{
			// Look for a \r\n
			auto const* cr = static_cast<char const*>(std::memchr( beg, '\r', end-beg ));
			if( !cr )
				break;
				//return EHeaderStatus_::cont;

			auto const* lf = cr+1;
			if( lf == end || '\n' != *lf )
				break;
				//return EHeaderStatus_::cont;

			auto const* next = lf+1; // next <= end 

			// Handle line
			flux_ext::pegtl::memory_input<> in( beg, next, "http-header" );

			try
			{
				switch( aHttp.state )
				{
					case EHeaderState_::status: {
						flux_ext::pegtl::parse<
							flux_ext::pegtl::must<httpparser_::GrammarStatus>,
							httpparser_::ActionStatus
						>( in, aHttp );

						aHttp.state = EHeaderState_::content;
					} break;
					case EHeaderState_::content: {
						// End of header?
						if( cr == beg && lf == beg+1 )
						{
							aBeg += next-reinterpret_cast<char const*>(aBeg);
							FLUX_ASSERT( aBeg <= aEnd );
							return EHeaderStatus_::done;
						}

						flux_ext::pegtl::parse<
							flux_ext::pegtl::must<httpparser_::GrammarHeader>,
							httpparser_::ActionContent
						>( in, aHttp );
					} break;
				}
			}
			catch( flux_ext::pegtl::parse_error const& eErr )
			{
				auto const& pos = eErr.positions.front();
				auto const& line = in.line_at( pos );

				auto const* head = line.data();
				auto const* errc = head + pos.byte_in_line;
				auto const* tail = errc+1;
				auto const* end = line.data()+line.size();

				FLUX_THROW( error::StreamNotAvailable )
					<< flux::einfo::ErrorDesc( "Unparsable mayhem in HTTP response" )
					<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("field = '{}[{:c}]{}'"), compat::string_view(head,errc-head), *errc, compat::string_view(tail,end-tail) )
				;
			}

			// Advance
			beg = next;
		}

		// Fix up buffer pointers.
		if( beg != end )
		{
			if( beg != reinterpret_cast<char const*>(aBase) )
			{
				std::memmove( aBase, beg, end-beg );
				aBeg = aBase;
				aEnd = aBase + (end-beg);
			}
		}
		else
		{
			aBeg = aEnd = aBase;
		}
		
		return EHeaderStatus_::cont;
	}
}

//--    $ check_lifetime*_()                ///{{{2///////////////////////////
namespace
{
	bool check_lifetime_soft_( unsigned aLifetime, Clock::time_point aBeginTime, unsigned* aRemaining ) noexcept
	{
		if( kHttpForever == aLifetime )
		{
			if( aRemaining ) *aRemaining = kHttpForever;
			return true;
		}

		unsigned const elapsed = util::narrow<unsigned>(duration_cast<Ms_>(Clock::now() - aBeginTime).count());
		if( elapsed >= aLifetime )
		{
			if( aRemaining ) *aRemaining = 0;
			return false;
		}

		if( aRemaining )
			*aRemaining = aLifetime - elapsed;

		return true;
	}

	void check_lifetime_( unsigned aLifetime, Clock::time_point aBeginTime, unsigned* aRemaining )
	{
		bool const timedOut = !check_lifetime_soft_( aLifetime, aBeginTime, aRemaining );
		FLUX_THROW_IF( timedOut, error::StreamExpired )
			<< flux::einfo::ErrorDesc( "Timeout" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("lifetime = {:d}ms, elapsed = {:d}ms"), aLifetime, duration_cast<Ms_>(Clock::now()-aBeginTime).count() )
		;
	}
}

//--    $ check_lifetime_()                 ///{{{2///////////////////////////
namespace
{
	bool wait_for_socket_( ESelOp_ aSelOp, SocketType_ aSock, unsigned aRemaining FLUXIO_WSARG_(Ws2api const* aWs2) )
	{
		Timeval_ tv{}, *ptv = nullptr;
		if( kHttpForever != aRemaining )
		{
			// Different types in Timeval_ for windows and linux. :-(
			using SecTy_ = decltype(Timeval_::tv_sec);
			using USecTy_ = decltype(Timeval_::tv_usec);

			tv.tv_sec = util::narrow<SecTy_>(aRemaining / 1000);
			tv.tv_usec = util::narrow<USecTy_>((aRemaining % 1000) * 1000);

			ptv = &tv;
		}

#		if !FLUXIO_USE_WINSOCK_
		::fd_set set;
		FD_ZERO( &set );
		FD_SET( aSock, &set );

		::fd_set* sets[3] = { nullptr, nullptr, nullptr };
		sets[std::size_t(aSelOp)] = &set;

		auto sret = ::select( aSock+1, sets[0], sets[1], sets[2], ptv );
#		else // USE_WINSOCK_
		Ws2_::fd_set set;
		aWs2->fd_zero( &set );
		aWs2->fd_set( aSock, &set );

		Ws2_::fd_set* sets[3] = { nullptr, nullptr, nullptr };
		sets[std::size_t(aSelOp)] = &set;

		auto sret = aWs2->select( util::narrow<int>(aSock+1), sets[0], sets[1], sets[2], ptv );
#		endif // ~ apis

		if( kSocketError_ == sret )
		{
			auto const ec =
#				if !FLUXIO_USE_WINSOCK_
				sys::make_errno_error_code()
#				else // USE_WINSOCK_
				sys::make_win32_error_code( aWs2->wsaGetLastError() )
#				endif // ~ apis
			;

			FLUX_THROW( error::StreamSysError )
				<< flux::einfo::ApiError( ec )
				<< flux::einfo::ApiFunction( "select" )
			;
		}

		return 0 != sret;
	}
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
