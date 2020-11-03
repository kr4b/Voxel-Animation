/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	ZStandard Read Stream implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/stream/zstd.hpp>
#include <flux/io/stream.hpp>
#include <flux/io/errors.hpp>

#include <flux/std/log.hpp>
#include <flux/std/byte.hpp>
#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>

#include <flux/util/narrow.hpp>
#include <flux/util/scope_exit.hpp>

#include <flux/compat/exchange.hpp>

#include <flux_ext/zstd.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	class StreamZStdRead_ final : public Stream
	{
		public:	
			StreamZStdRead_( 
				Stream*, bool aOwn, 
				StreamSize aInputSize, void* aInputBuff,
				StreamSize aOutputSize, void* aOutputBuff,
				StreamSize = ~StreamSize(0)
			);
			~StreamZStdRead_();

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
			void init_stream_();

		private:
			Stream* mStream;
			StreamSize mRemaining, mPosition, mUncompressed;
			std::size_t mOutStart;
			bool mOwnStream, mError, mInDone;

			ZSTD_DCtx* mCtx;
			ZSTD_inBuffer mInput;
			ZSTD_outBuffer mOutput;

			StreamSize mInBufferSize;
			void* mInBuffer;

			StreamSize mOutBufferSize;
			void* mOutBuffer;
	};
}

//--    stream_create_zstd_read()           ///{{{2///////////////////////////
unique_ptr<Stream> stream_create_zstd_read( ftl::not_null<unique_ptr<Stream>> aStream, StreamSize aInputBuffSize, StreamSize aOutputBuffSize )
{
	FLUX_THROW_IF( !aStream->caps().readable, error::StreamOpNotSupported )
		<< einfo::StreamDesc( aStream->description() )
		<< flux::einfo::ErrorDesc( "stream_create_zstd_read(): requires readable input stream" )
	;

	auto const inputBuffSize = aInputBuffSize == ~StreamSize(0)
		? ZSTD_DStreamInSize()
		: aInputBuffSize
	;
	auto const outputBuffSize = aOutputBuffSize == ~StreamSize(0)
		? ZSTD_DStreamOutSize()
		: aOutputBuffSize
	;

	auto const alignedInputSize = (inputBuffSize+16-1) & ~(16-1);

	std::size_t alignedSize = (sizeof(StreamZStdRead_)+16-1) & ~(16-1);
	std::size_t totalSize = alignedSize + alignedInputSize + outputBuffSize;

	auto* buffer = reinterpret_cast<flux::byte*>(::operator new( totalSize ));
	FLUX_UTIL_ON_SCOPE_EXIT
	{
		if( buffer ) ::operator delete( buffer );
	};

	auto* inbuff = buffer+alignedSize;
	auto* ofbuff = buffer+alignedSize+alignedInputSize;

	unique_ptr<Stream> underlying = std::move(aStream);
	auto* stream = new (buffer) StreamZStdRead_( 
		underlying.release(), true, 
		inputBuffSize, inbuff,
		outputBuffSize, ofbuff
	);

	buffer = nullptr;
	return unique_ptr<Stream>(stream);
}
unique_ptr<Stream> stream_create_zstd_read( ftl::not_null<Stream*> aStream, StreamSize aMaxRead, StreamSize aInputBuffSize, StreamSize aOutputBuffSize )
{
	FLUX_THROW_IF( !aStream->caps().readable, error::StreamOpNotSupported )
		<< einfo::StreamDesc( aStream->description() )
		<< flux::einfo::ErrorDesc( "stream_create_zstd_read(): requires readable input stream" )
	;
	auto const inputBuffSize = aInputBuffSize == ~StreamSize(0)
		? ZSTD_DStreamInSize()
		: aInputBuffSize
	;
	auto const outputBuffSize = aOutputBuffSize == ~StreamSize(0)
		? ZSTD_DStreamOutSize()
		: aOutputBuffSize
	;

	std::size_t alignedSize = (sizeof(StreamZStdRead_)+16-1) & ~(16-1);
	std::size_t totalSize = alignedSize + inputBuffSize + outputBuffSize;

	auto* buffer = reinterpret_cast<flux::byte*>(::operator new( totalSize ));
	FLUX_UTIL_ON_SCOPE_EXIT
	{
		if( buffer ) ::operator delete( buffer );
	};

	auto* inbuff = buffer+alignedSize;
	auto* ofbuff = buffer+alignedSize+inputBuffSize;

	auto* stream = new (buffer) StreamZStdRead_( 
		aStream, false, 
		inputBuffSize, inbuff,
		outputBuffSize, ofbuff,
		aMaxRead
	);

	buffer = nullptr;
	return unique_ptr<Stream>(stream);
}

//--    $ StreamZStdRead_                     ///{{{2///////////////////////////
namespace
{
	StreamZStdRead_::StreamZStdRead_( Stream* aStream, bool aOwn, StreamSize aInSize, void* aInBuff, StreamSize aOfSize, void* aOfBuff, StreamSize aMaxRead )
		: mStream( aStream )
		, mRemaining( aMaxRead )
		, mOwnStream( aOwn )
		, mCtx( nullptr )
		, mInBufferSize( aInSize )
		, mInBuffer( aInBuff )
		, mOutBufferSize( aOfSize )
		, mOutBuffer( aOfBuff )
	{
		try
		{
			init_stream_();
		}
		catch( ... )
		{
			if( mOwnStream )
				delete mStream;

			throw;
		}
	}

	StreamZStdRead_::~StreamZStdRead_()
	{
		if( mCtx )
			ZSTD_freeDCtx( mCtx );
		
		if( mOwnStream )
			delete mStream;
	}

	StreamSize StreamZStdRead_::read( void* aBuffer, StreamSize aSize, EIoRequest aReq )
	{
		FLUX_ASSERT( mStream );
		FLUX_ASSERT( aBuffer );
	
		auto* optr = reinterpret_cast<flux::byte*>(aBuffer);
		auto needed = aSize;
		while( needed && (!mInDone || mOutStart != mOutput.pos) )
		{
			if( mOutStart != mOutput.pos )
			{
				auto const got = mOutput.pos - mOutStart;
				auto const ret = std::min( needed, util::narrow<StreamSize>(got) );

				auto const* bptr = reinterpret_cast<flux::byte const*>(mOutput.dst);
				std::memcpy( optr, bptr+mOutStart, ret );

				optr += ret;
				needed -= ret;
				mOutStart += ret;
				
				if( mOutStart == mOutput.pos )
				{
					mOutput = ZSTD_outBuffer{ mOutBuffer, mOutBufferSize, 0 };
					mOutStart = 0;
				}

				if( !needed ) break;
			}

			// From the stream example:
			// > Given a valid frame, zstd won't consume the last byte of the
			// > frame until it has flushed all of the decompressed data of the
			// > frame.  Therefore, instead of checking if the return code is
			// > 0, we can decompress just check if input.pos < input.size.
			if( mInput.pos == mInput.size )
			{
				if( mRemaining && !!(EStreamStatus::readable & mStream->status()) )
				{
					auto const req = std::min( mInBufferSize, mRemaining );
					auto const got = mStream->read( mInBuffer, req, EIoRequest::bestEffort );

					mInput.src = mInBuffer;
					mInput.pos = 0;
					mInput.size = got;

					if( ~StreamSize(0) != mRemaining ) mRemaining -= got;
				}
				else
				{

					mInDone = true;
					mInput = ZSTD_inBuffer{ mInBuffer, 0, 0 };
				}
			}

			auto const res = ZSTD_decompressStream(
				mCtx,
				&mOutput, 
				&mInput
			);

			mError = !!ZSTD_isError( res );
			FLUX_THROW_IF( mError, error::StreamBadFormat )
				<< einfo::StreamMode( EStreamMode::read )
				<< einfo::StreamDesc( description() )
				<< flux::einfo::ApiFunction( "ZSTD_decompressStream" )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("{} ({:d})"), ZSTD_getErrorName(res), res )
			;
		}
		
		auto const total = aSize-needed;
		if( needed && (EIoRequest::mustComplete == aReq) )
		{
			auto remaining = mRemaining;
			if( mStream->caps().remaining )
				remaining = std::min( remaining, mStream->remaining() );
			
			FLUX_THROW( error::StreamIoIncomplete )
				<< einfo::StreamMode( EStreamMode::read )
				<< einfo::StreamDesc( description() )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("requested: {:d}, have: {:d} (underlying stream: {:d})"), aSize, total, remaining )
			;
		}

		mPosition += total;
		return total;
	}
	StreamSize StreamZStdRead_::write( void const*, StreamSize, EIoRequest )
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::write()" )
		;
	}

	EStreamStatus StreamZStdRead_::status() const noexcept
	{
		if( mError )
			return EStreamStatus::error;

		FLUX_ASSERT( mStream );
		if( mOutStart != mOutput.pos || mInput.pos != mInput.size || (mRemaining && !!(EStreamStatus::readable & mStream->status())) )
			return EStreamStatus::readable;
		
		return EStreamStatus(0);
	}

	StreamCaps StreamZStdRead_::caps() const noexcept
	{
		StreamCaps caps;
		caps.readable = true;
		caps.writable = false;
		caps.size = !!(~StreamSize(0) != mUncompressed);
		caps.tell = true;
		caps.seekFwd = false; //XXX XXX XXX
		caps.seekRev = false;
		caps.remaining = !!(~StreamSize(0) != mUncompressed);
		caps.infinite = false;
		caps.clonable = false;
		return caps;
	}

	StreamSize StreamZStdRead_::size() const
	{
		FLUX_THROW_IF( ~StreamSize(0) == mUncompressed, io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::size()" )
		;

		return mUncompressed;
	}

	StreamSize StreamZStdRead_::tell() const
	{
		return mPosition;
	}
	StreamSize StreamZStdRead_::seek( StreamOffset, ESeekWhence )
	{
		FLUX_ASSERT( mStream );
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::seek()" )
		;
	}
	StreamSize StreamZStdRead_::remaining() const
	{
		FLUX_THROW_IF( ~StreamSize(0) == mUncompressed, io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::remaining()" )
		;

		return mUncompressed - mPosition;
	}

	void StreamZStdRead_::flush()
	{
		FLUX_ASSERT( mStream );
		mStream->flush();
	}

	std::string StreamZStdRead_::description() const
	{
		FLUX_ASSERT( mStream );
		return format( "zstd://{}", mStream->description() );
	}
	ftl::Any StreamZStdRead_::native_handle() const
	{
		return ftl::Any( mStream );
	}


	void StreamZStdRead_::init_stream_()
	{
		// Reset state
		mError = false;
		mInDone = false;

		auto ctx = ZSTD_createDCtx();
		FLUX_THROW_IF( !ctx, error::StreamSysError )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ApiFunction( "ZSTD_createDCtx" )
			<< flux::einfo::ErrorDesc( "Unable to create ZSTD DCtx" )
		;

		FLUX_UTIL_ON_SCOPE_EXIT
		{
			if( ctx ) ZSTD_freeDCtx( ctx );
		};

		mInput = ZSTD_inBuffer{ 0, 0, 0 };
		mOutput = ZSTD_outBuffer{ mOutBuffer, mOutBufferSize, 0 };
		mOutStart = 0;
		mPosition = 0;

		// Try to read the header, and see if we can get a content size
		if( mRemaining && !!(EStreamStatus::readable & mStream->status()) )
		{
			auto const req = std::min( mInBufferSize, mRemaining );
			auto const got = mStream->read( mInBuffer, req, EIoRequest::bestEffort );

			mInput.src = mInBuffer;
			mInput.pos = 0;
			mInput.size = got;

			if( ~StreamSize(0) != mRemaining ) mRemaining -= got;

			auto const res = ZSTD_getFrameContentSize( mInput.src, mInput.size );
			FLUX_THROW_IF( ZSTD_CONTENTSIZE_ERROR == res, error::StreamBadFormat )
				<< einfo::StreamMode( EStreamMode::read )
				<< einfo::StreamDesc( description() )
				<< flux::einfo::ApiFunction( "ZSTD_getFrameContentSize" )
			;

			if( ZSTD_CONTENTSIZE_UNKNOWN != res )
				mUncompressed = util::narrow<StreamSize>(res);
			else
				mUncompressed = ~StreamSize(0);
		}

		mCtx = compat::exchange( ctx, nullptr );
	}
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
