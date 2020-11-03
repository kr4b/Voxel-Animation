/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	ZStandard Write Stream implementation
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
	class StreamZStdWrite_ final : public Stream
	{
		public:	
			StreamZStdWrite_( 
				Stream*, bool aOwn, 
				unsigned aCompLevel,
				StreamSize aPledgedSize,
				StreamSize aOutputSize, void* aOutputBuff
			);

			~StreamZStdWrite_();

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
			void init_stream_( unsigned, StreamSize );
			void end_stream_();

		private:
			Stream* mStream;
			StreamSize mUncompressed, mPledgedSize;
			bool mOwnStream, mError;

			ZSTD_CCtx* mCtx;
			ZSTD_outBuffer mOutput;

			StreamSize mOutBufferSize;
			void* mOutBuffer;
			std::size_t mOutBufferStart;
	};
}

//--    stream_create_zstd_write()            ///{{{2///////////////////////////
unique_ptr<Stream> stream_create_zstd_write( ftl::not_null<unique_ptr<Stream>> aStream, unsigned aCompLevel, StreamSize aPledgedSize, StreamSize aOutputBuffSize )
{
	FLUX_THROW_IF( !aStream->caps().writable, error::StreamOpNotSupported )
		<< einfo::StreamDesc( aStream->description() )
		<< flux::einfo::ErrorDesc( "stream_create_zstd_write(): requires writable output stream" )
	;

	auto const outputBuffSize = aOutputBuffSize == ~StreamSize(0)
		? ZSTD_CStreamOutSize()
		: aOutputBuffSize
	;

	std::size_t alignedSize = (sizeof(StreamZStdWrite_)+16-1) & ~(16-1);
	std::size_t totalSize = alignedSize + outputBuffSize;

	auto* buffer = reinterpret_cast<flux::byte*>(::operator new( totalSize ));
	FLUX_UTIL_ON_SCOPE_EXIT
	{
		if( buffer ) ::operator delete( buffer );
	};

	auto* ofbuff = buffer+alignedSize;

	unique_ptr<Stream> underlying = std::move(aStream);
	auto* stream = new (buffer) StreamZStdWrite_( 
		underlying.release(), true,
		aCompLevel,
		aPledgedSize,
		outputBuffSize, ofbuff
	);

	buffer = nullptr;
	return unique_ptr<Stream>(stream);
}
unique_ptr<Stream> stream_create_zstd_write( ftl::not_null<Stream*> aStream, unsigned aCompLevel, StreamSize aPledgedSize, StreamSize aOutputBuffSize )
{
	FLUX_THROW_IF( !aStream->caps().writable, error::StreamOpNotSupported )
		<< einfo::StreamDesc( aStream->description() )
		<< flux::einfo::ErrorDesc( "stream_create_zstd_write(): requires writable output stream" )
	;

	auto const outputBuffSize = aOutputBuffSize == ~StreamSize(0)
		? ZSTD_CStreamOutSize()
		: aOutputBuffSize
	;

	std::size_t alignedSize = (sizeof(StreamZStdWrite_)+16-1) & ~(16-1);
	std::size_t totalSize = alignedSize + outputBuffSize;

	auto* buffer = reinterpret_cast<flux::byte*>(::operator new( totalSize ));
	FLUX_UTIL_ON_SCOPE_EXIT
	{
		if( buffer ) ::operator delete( buffer );
	};

	auto* ofbuff = buffer+alignedSize;

	auto* stream = new (buffer) StreamZStdWrite_( 
		aStream, false,
		aCompLevel,
		aPledgedSize,
		outputBuffSize, ofbuff
	);

	buffer = nullptr;
	return unique_ptr<Stream>(stream);
}

//--    $ StreamZStdWrite_                  ///{{{2///////////////////////////
namespace
{
	StreamZStdWrite_::StreamZStdWrite_( Stream* aStream, bool aOwn, unsigned aCompLevel, StreamSize aPledgedSize, StreamSize aOutputBuffSize, void* aOutputBuff )
		: mStream( aStream )
		, mOwnStream( aOwn )
		, mCtx( nullptr )
		, mOutBufferSize( aOutputBuffSize )
		, mOutBuffer( aOutputBuff )
	{
		try
		{
			init_stream_( aCompLevel, aPledgedSize );
		}
		catch( ... )
		{
			if( mOwnStream )
				delete mStream;

			throw;
		}
	}

	StreamZStdWrite_::~StreamZStdWrite_()
	{
		try
		{
			end_stream_();
		}
		catch( std::exception const& eErr )
		{
			FLUX_LOG( ERROR, "StreamZStdRead_: could not finalize zstd stream due to error:\n{}", eErr.what() );
		}

		if( mCtx )
			ZSTD_freeCCtx( mCtx );
		
		if( mOwnStream )
			delete mStream;
	}

	StreamSize StreamZStdWrite_::read( void*, StreamSize, EIoRequest )
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::write )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::read()" )
		;
	}
	StreamSize StreamZStdWrite_::write( void const* aData, StreamSize aBytes, EIoRequest aReq )
	{
		FLUX_ASSERT( mStream );
		auto const error = compat::exchange( mError, true );

		ZSTD_inBuffer in = ZSTD_inBuffer{ aData, aBytes, 0 };

		do
		{
			auto const rem = ZSTD_compressStream2( mCtx, &mOutput, &in, ZSTD_e_continue );
			FLUX_THROW_IF( ZSTD_isError(rem), error::StreamSysError )
				<< einfo::StreamMode( EStreamMode::write )
				<< einfo::StreamDesc( description() )
				<< flux::einfo::ApiFunction( "ZSTD_compressStream2(ZSTD_e_continue)" )
				<< flux::einfo::ErrorDesc( FLUX_FMT_STRING("{} ({:d})"), ZSTD_getErrorName(rem), rem )
			;

			auto ret = mStream->write( reinterpret_cast<flux::byte*>(mOutBuffer)+mOutBufferStart, mOutput.pos, aReq );

			mOutBufferStart += ret;
			if( mOutBufferStart == mOutput.pos )
			{
				mOutBufferStart = 0;
				mOutput = ZSTD_outBuffer{ mOutBuffer, mOutBufferSize, 0 };
			}
		} while( EIoRequest::mustComplete == aReq && in.pos != in.size );

		auto const written = util::narrow<StreamSize>(in.pos);
		mUncompressed += written;

		mError = error;
		return written;
	}

	EStreamStatus StreamZStdWrite_::status() const noexcept
	{
		FLUX_ASSERT( mStream );
		auto ustat = mStream->status();

		if( mError || !!(EStreamStatus::error & ustat) )
			return EStreamStatus::error;

		return EStreamStatus::writable & ustat;
	}

	StreamCaps StreamZStdWrite_::caps() const noexcept
	{
		StreamCaps caps;
		caps.readable = false;
		caps.writable = true;
		caps.size = true;
		caps.tell = true;
		caps.seekFwd = false; //XXX write zeroes? might be useful
		caps.seekRev = false;
		caps.remaining = false;
		caps.infinite = false;
		caps.clonable = false;
		return caps;
	}

	StreamSize StreamZStdWrite_::size() const
	{
		return mUncompressed;
	}

	StreamSize StreamZStdWrite_::tell() const
	{
		return mUncompressed;
	}
	StreamSize StreamZStdWrite_::seek( StreamOffset, ESeekWhence )
	{
		FLUX_ASSERT( mStream );
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::write )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::seek()" )
		;
	}
	StreamSize StreamZStdWrite_::remaining() const
	{
		FLUX_THROW(io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::write )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::remaining()" )
		;
	}

	void StreamZStdWrite_::flush()
	{
		auto const error = compat::exchange( mError, true );
		ZSTD_inBuffer in = ZSTD_inBuffer{ nullptr, 0, 0 };

		bool finished = false;
		do
		{
			auto const rem = ZSTD_compressStream2( mCtx, &mOutput, &in, ZSTD_e_flush );
			FLUX_THROW_IF( ZSTD_isError(rem), error::StreamSysError )
				<< einfo::StreamMode( EStreamMode::write )
				<< einfo::StreamDesc( description() )
				<< flux::einfo::ApiFunction( "ZSTD_compressStream2(ZSTD_e_flush)" )
				<< flux::einfo::ErrorDesc( FLUX_FMT_STRING("{} ({:d})"), ZSTD_getErrorName(rem), rem )
			;

			mStream->write( reinterpret_cast<flux::byte*>(mOutBuffer)+mOutBufferStart, mOutput.pos, EIoRequest::mustComplete );

			mOutBufferStart = 0;
			mOutput = ZSTD_outBuffer{ mOutBuffer, mOutBufferSize, 0 };

			finished = (rem == 0);
		} while( !finished );

		mStream->flush();
		mError = error;
	}

	std::string StreamZStdWrite_::description() const
	{
		FLUX_ASSERT( mStream );
		return format( "zstd://{}", mStream->description() );
	}
	ftl::Any StreamZStdWrite_::native_handle() const
	{
		return ftl::Any( mStream );
	}


	void StreamZStdWrite_::init_stream_( unsigned aCompLevel, StreamSize aPledgedSize )
	{
		// Initialize state
		mError = true;
		mUncompressed = 0;
		mOutBufferStart = 0;
		mOutput = ZSTD_outBuffer{ mOutBuffer, mOutBufferSize, 0 };
		
		// Create and configure ZSTD context
		auto ctx = ZSTD_createCCtx();
		FLUX_THROW_IF( !ctx, error::StreamSysError )
			<< einfo::StreamMode( EStreamMode::write )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ApiFunction( "ZSTD_createCCtx" )
			<< flux::einfo::ErrorDesc( "Unable to create ZSTD CCtx" )
		;

		FLUX_UTIL_ON_SCOPE_EXIT
		{
			if( ctx ) ZSTD_freeCCtx( ctx );
		};

		auto resa = ZSTD_CCtx_setParameter( ctx, ZSTD_c_compressionLevel, aCompLevel );
		FLUX_THROW_IF( ZSTD_isError(resa), error::StreamSysError )
			<< einfo::StreamMode( EStreamMode::write )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ApiFunction( "ZSTD_CCtx_setParameter(ZSTD_c_compressionLevel)" )
			<< flux::einfo::ErrorDesc( FLUX_FMT_STRING("{} ({:d})"), ZSTD_getErrorName(resa), resa )
		;

		auto resb = ZSTD_CCtx_setParameter( ctx, ZSTD_c_checksumFlag, 1 );
		FLUX_THROW_IF( ZSTD_isError(resb), error::StreamSysError )
			<< einfo::StreamMode( EStreamMode::write )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ApiFunction( "ZSTD_CCtx_setParameter(ZSTD_c_checksumFlag)" )
			<< flux::einfo::ErrorDesc( FLUX_FMT_STRING("{} ({:d})"), ZSTD_getErrorName(resb), resb )
		;

		if( ~StreamSize(0) != aPledgedSize )
		{
			auto resc = ZSTD_CCtx_setPledgedSrcSize( ctx, util::narrow<unsigned long long>(aPledgedSize) );
			FLUX_THROW_IF( ZSTD_isError(resc), error::StreamSysError )
				<< einfo::StreamMode( EStreamMode::write )
				<< einfo::StreamDesc( description() )
				<< flux::einfo::ApiFunction( "ZSTD_CCtx_setPledgedSrcSize" )
				<< flux::einfo::ErrorDesc( FLUX_FMT_STRING("{} ({:d})"), ZSTD_getErrorName(resc), resc )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("pledged size = {:d}"), aPledgedSize )
			;

		}

		mPledgedSize = aPledgedSize;
		mError = false;
		mCtx = compat::exchange( ctx, nullptr );
	}
	void StreamZStdWrite_::end_stream_()
	{
		if( mError )
			return;
		
		ZSTD_inBuffer in = ZSTD_inBuffer{ nullptr, 0, 0 };

		bool finished = false;
		do
		{
			auto const rem = ZSTD_compressStream2( mCtx, &mOutput, &in, ZSTD_e_end );
			FLUX_THROW_IF( ZSTD_isError(rem), error::StreamSysError )
				<< einfo::StreamMode( EStreamMode::write )
				<< einfo::StreamDesc( description() )
				<< flux::einfo::ApiFunction( "ZSTD_compressStream2(ZSTD_e_end)" )
				<< flux::einfo::ErrorDesc( FLUX_FMT_STRING("{} ({:d})"), ZSTD_getErrorName(rem), rem )
			;

			mStream->write( reinterpret_cast<flux::byte*>(mOutBuffer)+mOutBufferStart, mOutput.pos, EIoRequest::mustComplete );

			mOutBufferStart = 0;
			mOutput = ZSTD_outBuffer{ mOutBuffer, mOutBufferSize, 0 };

			finished = (rem == 0);
		} while( !finished );

		// Technically not needed: ZSTD complains loudly already.
		/*if( ~StreamSize(0) != mPledgedSize && mUncompressed != mPledgedSize )
		{
			FLUX_LOG( WARNING, "StreamZStdWrite_: pledged size ({:d}) mismatches the actual size ({:d}) of the written data!", mPledgedSize, mUncompressed );
		}*/
	}
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
