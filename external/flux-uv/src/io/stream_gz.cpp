/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	GZip Stream implementation
 *
 * TODO: maybe implement StreamGZRead_::seek() with forward seeking?
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/stream/gz.hpp>
#include <flux/io/stream.hpp>
#include <flux/io/errors.hpp>

#include <flux/std/log.hpp>
#include <flux/std/byte.hpp>
#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>
#include <flux/std/inttypes.hpp>

#include <flux/util/attrib.hpp>
#include <flux/util/narrow.hpp>
#include <flux/util/scope_exit.hpp>

#include <flux_ext/crc32.hpp>
#include <flux_ext/miniz.hpp>

#include <chrono>
#include <algorithm>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	// Based on https://github.com/richgel999/miniz/blob/master/examples/example5.c
	constexpr mz_uint kCompNumProbes_[11] = {
		0, 1, 6, 32, 16, 32, 128, 256, 512, 768, 1500
	};
	
	class StreamGZRead_ final : public Stream
	{
		public:	
			StreamGZRead_( 
				Stream*, bool aOwn, 
				StreamSize aInputSize, mz_uint8* aInputBuff,
				StreamSize aOutputSize, mz_uint8* aOutputBuff,
				EGZFlags = EGZFlags::none, 
				StreamSize = ~StreamSize(0)
			);
			~StreamGZRead_();

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
			StreamSize mRemaining, mLimit, mUncompressed, mPosition;
			bool mOwnStream, mDecDone, mInDone, mHaveData, mError;
			EGZFlags mFlags;

			tinfl_decompressor mState;

			StreamSize mInBufferSize;
			mz_uint8* mInBuffer;
			mz_uint8* mInBufferRead;
			mz_uint8* mInBufferWrite;

			StreamSize mOutBufferSize;
			mz_uint8* mOutBuffer;
			mz_uint8* mOutBufferRead;
			mz_uint8* mOutBufferWrite;
	};
	class StreamGZWrite_ final : public Stream
	{
		public:	
			StreamGZWrite_( 
				Stream*, bool aOwn, 
				unsigned aCompLevel,
				EGZFlags = EGZFlags::none, 
				char const* aOriginalName = nullptr,
				char const* aCommentField = nullptr
			);

			~StreamGZWrite_();

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
			void init_stream_( unsigned, char const*, char const* );
			void end_stream_();

			mz_bool put_( void const*, int );
			static mz_bool tdefl_put_callback_( void const*, int, void* );

		private:
			Stream* mStream;
			StreamSize mUncompressed;
			uint32_t mCrc32;
			bool mOwnStream, mError;
			EGZFlags mFlags;

			tdefl_compressor mState;
	};
	
	FLUX_ATTR_NORETURN
	void throw_io_limited_( Stream*, StreamSize, StreamSize, StreamSize );
	
	std::string to_string_( tinfl_status );
	std::string to_string_( tdefl_status );
}

//--    stream_create_gz_read()             ///{{{2///////////////////////////
unique_ptr<Stream> stream_create_gz_read( ftl::not_null<unique_ptr<Stream>> aStream, EGZFlags aFlags, StreamSize aInputSize, StreamSize aOutputSize )
{
	FLUX_THROW_IF( !aStream->caps().readable, error::StreamOpNotSupported )
		<< einfo::StreamDesc( aStream->description() )
		<< flux::einfo::ErrorDesc( "stream_create_gz_read(): requires readable input stream" )
	;

	std::size_t alignedSize = (sizeof(StreamGZRead_)+16-1) & ~(16-1);
	std::size_t totalSize = alignedSize + aOutputSize + aInputSize;

	auto* buffer = reinterpret_cast<mz_uint8*>(::operator new( totalSize ));
	FLUX_UTIL_ON_SCOPE_EXIT
	{
		if( buffer ) ::operator delete( buffer );
	};

	auto* ofbuff = buffer+alignedSize;
	auto* inbuff = buffer+alignedSize+aOutputSize;

	unique_ptr<Stream> underlying = std::move(aStream);
	auto* stream = new (buffer) StreamGZRead_( 
		underlying.release(), true, 
		aInputSize, inbuff,
		aOutputSize, ofbuff,
		aFlags
	);

	buffer = nullptr;
	return unique_ptr<Stream>(stream);
}
unique_ptr<Stream> stream_create_gz_read( ftl::not_null<Stream*> aStream, StreamSize aMaxRead, EGZFlags aFlags, StreamSize aInputSize, StreamSize aOutputSize )
{
	FLUX_THROW_IF( !aStream->caps().readable, error::StreamOpNotSupported )
		<< einfo::StreamDesc( aStream->description() )
		<< flux::einfo::ErrorDesc( "stream_create_gz_read(): requires readable input stream" )
	;

	std::size_t alignedSize = (sizeof(StreamGZRead_)+16-1) & ~(16-1);
	std::size_t totalSize = alignedSize + aOutputSize + aInputSize;

	auto* buffer = reinterpret_cast<mz_uint8*>(::operator new( totalSize ));
	FLUX_UTIL_ON_SCOPE_EXIT
	{
		if( buffer ) ::operator delete( buffer );
	};

	auto* ofbuff = buffer+alignedSize;
	auto* inbuff = buffer+alignedSize+aOutputSize;

	auto* stream = new (buffer) StreamGZRead_( 
		aStream, false, 
		aInputSize, inbuff,
		aOutputSize, ofbuff,
		aFlags,
		aMaxRead
	);

	buffer = nullptr;
	return unique_ptr<Stream>(stream);
}

//--    stream_create_gz_write()            ///{{{2///////////////////////////
unique_ptr<Stream> stream_create_gz_write( ftl::not_null<unique_ptr<Stream>> aStream, unsigned aCompLevel, EGZFlags aFlags, char const* aOriginalName, char const* aCommentField )
{
	FLUX_THROW_IF( !aStream->caps().writable, error::StreamOpNotSupported )
		<< einfo::StreamDesc( aStream->description() )
		<< flux::einfo::ErrorDesc( "stream_create_gz_write(): requires writable output stream" )
	;

	unique_ptr<Stream> underlying = std::move(aStream);
	return make_unique<StreamGZWrite_>( 
		underlying.release(), true,
		aCompLevel,
		aFlags,
		aOriginalName, aCommentField
	);
}
unique_ptr<Stream> stream_create_gz_write( ftl::not_null<Stream*> aStream, unsigned aCompLevel, EGZFlags aFlags, char const* aOriginalName, char const* aCommentField )
{
	FLUX_THROW_IF( !aStream->caps().writable, error::StreamOpNotSupported )
		<< einfo::StreamDesc( aStream->description() )
		<< flux::einfo::ErrorDesc( "stream_create_gz_write(): requires writable output stream" )
	;

	return make_unique<StreamGZWrite_>( 
		aStream, false,
		aCompLevel,
		aFlags,
		aOriginalName, aCommentField
	);
}

//--    $ StreamGZRead_                     ///{{{2///////////////////////////
namespace
{
	StreamGZRead_::StreamGZRead_( Stream* aStream, bool aOwn, StreamSize aInSize, mz_uint8* aInBuff, StreamSize aOfSize, mz_uint8* aOfBuff, EGZFlags aFlags, StreamSize aMaxRead )
		: mStream( aStream )
		, mRemaining( aMaxRead )
		, mLimit( aMaxRead )
		, mUncompressed( ~StreamSize(0) )
		, mOwnStream( aOwn )
		, mError( false )
		, mFlags( aFlags )
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

	StreamGZRead_::~StreamGZRead_()
	{
		if( mOwnStream )
			delete mStream;
	}

	StreamSize StreamGZRead_::read( void* aBuffer, StreamSize aSize, EIoRequest aReq )
	{
		FLUX_ASSERT( mStream );
		FLUX_ASSERT( aBuffer );

		auto needed = util::narrow<std::size_t>(aSize);
		auto* buffer = reinterpret_cast<flux::byte*>(aBuffer);

		try
		{
			while( needed && !(mDecDone && mOutBufferRead == mOutBufferWrite) )
			{
				// If there is no output available, try to fill the output buffer.
				if( !mDecDone && mOutBufferWrite < mOutBuffer+mOutBufferSize )
				{
					// Get more data if needed (and possible)
					StreamSize inSpace = mInBuffer+mInBufferSize - mInBufferWrite;

					mInDone = !(mRemaining && mStream->status() == EStreamStatus::readable);
					if( inSpace && !mInDone )
					{
						auto const req = std::min( inSpace, mRemaining );
						auto const got = mStream->read( mInBufferWrite, req, EIoRequest::bestEffort );

						mInBufferWrite += got;

						if( ~StreamSize(0) != mRemaining )
						{
							FLUX_ASSERT( got <= mRemaining );
							mRemaining -= got;
						}
					}
					
					// Decompress a bit
					auto inBytes = util::narrow<std::size_t>(mInBufferWrite-mInBufferRead);
					auto outBytes = util::narrow<std::size_t>(mOutBuffer+mOutBufferSize-mOutBufferWrite);

					auto res = tinfl_decompress(
						&mState,
						reinterpret_cast<mz_uint8 const*>(mInBufferRead), &inBytes,
						mOutBuffer, mOutBufferWrite, &outBytes,
						(mInDone ? 0 : TINFL_FLAG_HAS_MORE_INPUT) /*| TINFL_FLAG_PARSE_ZLIB_HEADER */ //XXX
					);

					mInBufferRead += inBytes;
					if( mInBufferRead == mInBufferWrite )
						mInBufferRead = mInBufferWrite = mInBuffer;

					mOutBufferWrite += outBytes;
		
					FLUX_THROW_IF( res < TINFL_STATUS_DONE, error::StreamBadFormat )
						<< einfo::StreamMode( EStreamMode::read )
						<< einfo::StreamDesc( description() )
						<< flux::einfo::ApiFunction( "tinf_decompress" )
						<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("tinfl_status({:d}): {}"), res, to_string_(res) )
					;

					mDecDone = (TINFL_STATUS_DONE == res);

					/* Note sure about the following. The behaviour matches
					 * (roughly) the one found in the miniz-examples. Somewhat
					 * strangely, error status are considered to provide data.
					 * During testing, I had to add HAS_MORE_OUTPUT as
					 * decompression would otherwise occasionally require a
					 * output buffer of the size of the total output (which is
					 * a bit impractical). However, NEEDS_MORE_INPUT *must* not
					 * be added (it seems), as this will occasionally result in
					 * incomplete output (visible in the test cases when
					 * setting the input buffer to a very small value, e.g. 16
					 * bytes).
					 */
					mHaveData = (res <= TINFL_STATUS_DONE) || (res == TINFL_STATUS_HAS_MORE_OUTPUT); //XXX
				}

				// If we already have output decompressed, try to empty the output
				// buffer first.
				if( mHaveData && mOutBufferRead < mOutBufferWrite )
				{
					auto const bytes = std::min( util::narrow<std::size_t>(needed), std::size_t(mOutBufferWrite-mOutBufferRead) );
					std::memcpy( buffer, mOutBufferRead, bytes );

					//mOutBufferAvail -= bytes;
					mOutBufferRead += bytes;

					needed -= bytes;
					buffer += bytes;

					if( mOutBufferRead == mOutBufferWrite )
					{
						mHaveData = false;
						mOutBufferRead = mOutBufferWrite = mOutBuffer;
					}
				}
			}
		}
		catch( ... )
		{
			mError = true;
			throw;
		}

		auto const total = aSize-util::narrow<StreamSize>(needed);
		if( needed && (EIoRequest::mustComplete == aReq) )
		{
			auto remaining = mRemaining;
			if( mStream->caps().remaining )
				remaining = std::min( remaining, mStream->remaining() );
			
			FLUX_THROW( error::StreamIoIncomplete )
				<< einfo::StreamMode( EStreamMode::read )
				<< einfo::StreamDesc( description() )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("requested: {:d}, have: {:d} (underlying stream: {:d})\n"), aSize, total, remaining )
			;
		}

		mPosition += total;
		return total;
	}
	StreamSize StreamGZRead_::write( void const*, StreamSize, EIoRequest )
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::write()" )
		;
	}

	EStreamStatus StreamGZRead_::status() const noexcept
	{
		if( mError )
			return EStreamStatus::error;

		if( !mDecDone || mOutBufferRead != mOutBufferWrite )
			return EStreamStatus::readable;
		
		return EStreamStatus(0);
	}

	StreamCaps StreamGZRead_::caps() const noexcept
	{
		StreamCaps caps;
		caps.readable = true;
		caps.writable = false;
		caps.size = (mUncompressed == ~StreamSize(0)) ? false : true;
		caps.tell = true;
		caps.seekFwd = false; //XXX XXX XXX
		caps.seekRev = false;
		caps.remaining = (mUncompressed == ~StreamSize(0)) ? false : true;
		caps.infinite = false;
		caps.clonable = false;
		return caps;
	}

	StreamSize StreamGZRead_::size() const
	{
		FLUX_ASSERT( mStream );
		FLUX_THROW_IF( mUncompressed == ~StreamSize(0), io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::size()" )
		;

		return mUncompressed;
	}

	StreamSize StreamGZRead_::tell() const
	{
		return mPosition;
	}
	StreamSize StreamGZRead_::seek( StreamOffset, ESeekWhence )
	{
		FLUX_ASSERT( mStream );
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::seek()" )
		;
	}
	StreamSize StreamGZRead_::remaining() const
	{
		FLUX_THROW_IF( mUncompressed == ~StreamSize(0), io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::remaining()" )
		;

		//FLUX_ASSERT( mPosition < mUncompressed ); // mUncompressed might be wrong.
		return mUncompressed - mPosition;
	}

	void StreamGZRead_::flush()
	{
		FLUX_ASSERT( mStream );
		mStream->flush();
	}

	std::string StreamGZRead_::description() const
	{
		FLUX_ASSERT( mStream );
		return format( "gz://{}", mStream->description() );
	}
	ftl::Any StreamGZRead_::native_handle() const
	{
		return ftl::Any( mStream );
	}


	void StreamGZRead_::init_stream_()
	{
		// Initialize state
		mPosition = 0;

		mDecDone = false;
		mHaveData = false;
		mError = false;
		mInDone = false;
		
		tinfl_init( &mState );

		mInBufferRead = mInBufferWrite = mInBuffer;
		mOutBufferRead = mOutBufferWrite = mOutBuffer;

		auto const& caps = mStream->caps();

		// Sanity check
		if( !caps.tell || !caps.seekFwd || !caps.seekRev )
			mFlags |= EGZFlags::noFooter;

		if( !!(EGZFlags::noFooter & mFlags) )
			mFlags |= EGZFlags::ignoreUncompressedSize;


		// Read and parse header
		if( !(EGZFlags::noHeader & mFlags) )
		{
			/* The .gz header is 10 bytes. */
			uint8_t header[10];
			try
			{
				if( mRemaining < 10 )
					throw_io_limited_( mStream, mLimit, mRemaining, 10 );

				mStream->read( header, 10, EIoRequest::mustComplete );

				if( ~std::size_t(0) != mRemaining ) mRemaining -= 10;
			}
			catch( error::StreamIoIncomplete const& )
			{
				FLUX_THROW_WITH_NESTED( io::error::StreamBadFormat )
					<< einfo::StreamMode( EStreamMode::read )
					<< einfo::StreamDesc( description() )
					<< flux::einfo::ErrorDesc( "Unable to read .gz header (mandatory)" )
				;
			}

			// header[0]...header[1] => file magic
			if( 0x1f != header[0] || 0x8b != header[1] )
			{
				FLUX_THROW( io::error::StreamBadFormat )
					<< einfo::StreamMode( EStreamMode::read )
					<< einfo::StreamDesc( description() )
					<< flux::einfo::ErrorDesc( "Invalid .gz header" )
				;
			}

			//header[2] => compression method
			if( 8 != header[2] )
			{
				FLUX_THROW( io::error::StreamBadFormat )
					<< einfo::StreamMode( EStreamMode::read )
					<< einfo::StreamDesc( description() )
					<< flux::einfo::ErrorDesc( "Unsupported .gz. compression method" )
				;
			}

			//bool const textmode = !!(header[3] & 1);
			bool const checksum = !!(header[3] & 2);
			bool const extras = !!(header[3] & 4);
			bool const namestr = !!(header[3] & 8);
			bool const comment = !!(header[3] & 16);

			// header[4]...header[7] => 32 bit timestamp
			// header[8] => compression/extra flags
			// header[9] => operating system that created the .gz.

			// extra header
			if( extras )
			{
				uint16_t length;
				try
				{
					if( mRemaining < 2 )
						throw_io_limited_( mStream, mLimit, mRemaining, 2 );

					mStream->read( &length, 2, EIoRequest::mustComplete );

					if( ~std::size_t(0) != mRemaining ) mRemaining -= 2;
				}
				catch( error::StreamIoIncomplete const& )
				{
					FLUX_THROW_WITH_NESTED( io::error::StreamBadFormat )
						<< einfo::StreamMode( EStreamMode::read )
						<< einfo::StreamDesc( description() )
						<< flux::einfo::ErrorDesc( "Unable to read .gz header (extras length)" )
					;
				}

				try
				{
					if( mRemaining < util::narrow<StreamSize>(length) )
						throw_io_limited_( mStream, mLimit, mRemaining, length );
				}
				catch( error::StreamIoIncomplete const& )
				{
					FLUX_THROW_WITH_NESTED( io::error::StreamBadFormat )
						<< einfo::StreamMode( EStreamMode::read )
						<< einfo::StreamDesc( description() )
						<< flux::einfo::ErrorDesc( "Unable to read .gz header (extras length)" )
					;
				}

				if( caps.seekFwd )
				{
					mStream->seek( util::narrow<StreamOffset>(length), ESeekWhence::cur );
				}
				else
				{
					auto rem = util::narrow<StreamSize>(length);
					while( rem && !!(EStreamStatus::readable & mStream->status()) )
					{
						auto req = std::min( rem, mInBufferSize );
						auto got = mStream->read( mInBuffer, req, EIoRequest::bestEffort );
						rem -= got;
					}

					FLUX_THROW( io::error::StreamBadFormat )
						<< einfo::StreamMode( EStreamMode::read )
						<< einfo::StreamDesc( description() )
						<< flux::einfo::ErrorDesc( "Unable to read .gz header (extras)" )
					;
				}

				if( ~std::size_t(0) != mRemaining ) mRemaining -= util::narrow<StreamSize>(length);
			}

			// Original file name.
			if( namestr )
			{
				bool more = true;
				while( more && !!(EStreamStatus::readable & mStream->status()) && mRemaining )
				{
					if( mInBufferRead == mInBufferWrite )
					{
						auto req = std::min( mRemaining, mInBufferSize );
						auto got = mStream->read( mInBuffer, req, EIoRequest::bestEffort );

						mInBufferRead = mInBuffer;
						mInBufferWrite = mInBuffer + got;

						if( ~std::size_t(0) != mRemaining ) mRemaining -= got;
					}
					
					for( ; mInBufferRead != mInBufferWrite; ++mInBufferRead )
					{
						if( '\0' == *mInBufferRead ) 
						{
							++mInBufferRead;
							more = false;
							break;
						}
					}
				}

				FLUX_THROW_IF( more, io::error::StreamBadFormat )
					<< einfo::StreamMode( EStreamMode::read )
					<< einfo::StreamDesc( description() )
					<< flux::einfo::ErrorDesc( "Unable to read .gz header (original name)" )
				;
			}
			
			// Comment.
			if( comment )
			{
				bool more = true;
				while( more && !!(EStreamStatus::readable & mStream->status()) && mRemaining )
				{
					if( mInBufferRead == mInBufferWrite )
					{
						auto req = std::min( mRemaining, mInBufferSize );
						auto got = mStream->read( mInBuffer, req, EIoRequest::bestEffort );

						mInBufferRead = mInBuffer;
						mInBufferWrite = mInBuffer + got;

						if( ~std::size_t(0) != mRemaining ) mRemaining -= got;
					}
					
					for( ; mInBufferRead != mInBufferWrite; ++mInBufferRead )
					{
						if( '\0' == *mInBufferRead ) 
						{
							++mInBufferRead;
							more = false;
							break;
						}
					}
				}

				FLUX_THROW_IF( more, io::error::StreamBadFormat )
					<< einfo::StreamMode( EStreamMode::read )
					<< einfo::StreamDesc( description() )
					<< flux::einfo::ErrorDesc( "Unable to read .gz header (comments)" )
				;
			}

			// Checksum
			if( checksum )
			{
				uint16_t crc16;
				try
				{
					if( mRemaining < 2 )
						throw_io_limited_( mStream, mLimit, mRemaining, 2 );

					mStream->read( &crc16, 2, EIoRequest::mustComplete );

					if( ~std::size_t(0) != mRemaining ) mRemaining -= 2;
				}
				catch( error::StreamIoIncomplete const& )
				{
					FLUX_THROW_WITH_NESTED( io::error::StreamBadFormat )
						<< einfo::StreamMode( EStreamMode::read )
						<< einfo::StreamDesc( description() )
						<< flux::einfo::ErrorDesc( "Unable to read .gz header (checksum)" )
					;
				}
			}
		}

		// See if we can get the stream size...
		if( !(EGZFlags::noFooter & mFlags) )
		{
			FLUX_ASSERT( caps.tell && caps.seekFwd && caps.seekRev );

			auto current = mStream->tell();

			mStream->seek( -8, ESeekWhence::end );

			uint32_t footer[2]; // footer[0] = CRC32, footer[1] == size in bytes
			try
			{
				mStream->read( footer, 8, EIoRequest::mustComplete );

				if( !(EGZFlags::ignoreUncompressedSize & mFlags) )
					mUncompressed = footer[1];
			}
			catch( error::StreamError const& )
			{
				mFlags |= EGZFlags::noFooter;
				mFlags |= EGZFlags::ignoreUncompressedSize;
			}

			mStream->seek( current, ESeekWhence::set );
		}
	}
}

//--    $ StreamGZWrite_                     ///{{{2///////////////////////////
namespace
{
	StreamGZWrite_::StreamGZWrite_( Stream* aStream, bool aOwn, unsigned aCompLevel, EGZFlags aFlags, char const* aOriginalName, char const* aCommentField )
		: mStream( aStream )
		, mOwnStream( aOwn )
		, mFlags( aFlags )
	{
		try
		{
			init_stream_( aCompLevel, aOriginalName, aCommentField );
		}
		catch( ... )
		{
			if( mOwnStream )
				delete mStream;

			throw;
		}
	}

	StreamGZWrite_::~StreamGZWrite_()
	{
		try
		{
			end_stream_();
		}
		catch( std::exception const& eErr )
		{
			FLUX_LOG( ERROR, "StreamGZRead_: could not finalize GZ stream due to error:\n{}", eErr.what() );
		}
		
		if( mOwnStream )
			delete mStream;
	}

	StreamSize StreamGZWrite_::read( void*, StreamSize, EIoRequest )
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::write )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::read()" )
		;
	}
	StreamSize StreamGZWrite_::write( void const* aData, StreamSize aBytes, EIoRequest )
	{
		auto const req = util::narrow<std::size_t>(aBytes);
		auto const res = tdefl_compress_buffer( &mState, aData, req, TDEFL_NO_FLUSH );

		mError = (res < TDEFL_STATUS_OKAY);
		FLUX_THROW_IF( mError, error::StreamIoIncomplete )
			<< einfo::StreamMode( EStreamMode::write )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorDesc( "Writing (TDEFL_NO_FLUSH) .gz stream failed" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("tdefl_status({:d}) = {}"), int(res), to_string_(res) )
			<< flux::einfo::ApiFunction( "tdefl_compress_buffer" )
		;

		// XXX: if the data buffer is large, this is kinda bad, since we need
		// to load the memory twice. It would be better to iterate over it in
		// cachline-sized bites and perform the compression and crc32() at the
		// same time.
		if( !(EGZFlags::noFooter & mFlags) )
			mCrc32 = flux_ext::crc32_fast( aData, req, mCrc32 );

		mUncompressed += req;
		return req;
	}

	EStreamStatus StreamGZWrite_::status() const noexcept
	{
		FLUX_ASSERT( mStream );
		auto ustat = mStream->status();

		if( mError || !!(EStreamStatus::error & ustat) )
			return EStreamStatus::error;

		return EStreamStatus::writable & ustat;
	}

	StreamCaps StreamGZWrite_::caps() const noexcept
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

	StreamSize StreamGZWrite_::size() const
	{
		return mUncompressed;
	}

	StreamSize StreamGZWrite_::tell() const
	{
		return mUncompressed;
	}
	StreamSize StreamGZWrite_::seek( StreamOffset, ESeekWhence )
	{
		FLUX_ASSERT( mStream );
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::write )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::seek()" )
		;
	}
	StreamSize StreamGZWrite_::remaining() const
	{
		FLUX_THROW(io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::write )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::remaining()" )
		;
	}

	void StreamGZWrite_::flush()
	{
		FLUX_ASSERT( mStream );
		auto res = tdefl_compress_buffer( &mState, nullptr, 0, TDEFL_FULL_FLUSH );
		FLUX_THROW_IF( res < TDEFL_STATUS_OKAY, error::StreamIoIncomplete )
			<< einfo::StreamMode( EStreamMode::write )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorDesc( "Flushing (TDEFL_FULL_FLUSH) .gz stream failed" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("tdefl_status({:d}) = {}"), int(res), to_string_(res) )
			<< flux::einfo::ApiFunction( "tdefl_compress_buffer" )
		;

		mStream->flush();
	}

	std::string StreamGZWrite_::description() const
	{
		FLUX_ASSERT( mStream );
		return format( "gz://{}", mStream->description() );
	}
	ftl::Any StreamGZWrite_::native_handle() const
	{
		return ftl::Any( mStream );
	}


	void StreamGZWrite_::init_stream_( unsigned aCompLevel, char const* aOriginalName, char const* aCommentField )
	{
		// Initialize state
		mError = false;
		mCrc32 = 0;
		mUncompressed = 0;
		
		// Based on https://github.com/richgel999/miniz/blob/master/examples/example5.c
		aCompLevel = std::min( aCompLevel, util::narrow<unsigned>(sizeof(kCompNumProbes_)/sizeof(kCompNumProbes_[0])) );
		int flags = kCompNumProbes_[aCompLevel] | (aCompLevel <= 3 ? TDEFL_GREEDY_PARSING_FLAG : 0);
		if( !aCompLevel )
			flags |= TDEFL_FORCE_ALL_RAW_BLOCKS;
		
		tdefl_init( &mState, &StreamGZWrite_::tdefl_put_callback_, this, flags );

		// Write header
		if( !(EGZFlags::noHeader & mFlags) )
		{
			uint8_t const flags = 0
				// unset: bit 0: textmode
				// unset: bit 1: checksum
				// unset: bit 2: extras
				| (aOriginalName ? 8 : 0)
				| (aCommentField ? 16 : 0)
				// reserved bits... leave zero
			;

			using Clock_ = std::chrono::system_clock;
			uint32_t const timestamp = !!(EGZFlags::headerOmitTime & mFlags)
				? 0
				: uint32_t(Clock_::to_time_t( Clock_::now() ))
			;
			uint8_t timebytes[sizeof(uint32_t)];
			std::memcpy( timebytes, &timestamp, sizeof(uint32_t) );

			uint8_t const compflags = 0
				| ((0 == aCompLevel) ? 2 : 0)
				| ((10 <= aCompLevel) ? 4 : 0)
			;

			uint8_t const os = !!(EGZFlags::headerOmitOS & mFlags)
				? 255
				: (
#					if FLUX_PLATFORM == FLUX_PLATFORM_LINUX
					3 // "Unix"
#					elif FLUX_PLATFORM == FLUX_PLATFORM_WIN32
					11 // "NTFS filesystem", rather than 0 for "FAT filesystem"
#					elif FLUX_PLATFORM == FLUX_PLATFORM_MACOS
					7 // "Macintosh"
#					else /* Unknown platform */
					255
#					endif // ~ PLATFORM
				)
			;

			/* The .gz header is 10 bytes. */
			uint8_t const header[10] = {
				0x1f, 0x8b,   				// magic
				8,            				// compression method
				flags,                      // flags
				timebytes[0], timebytes[1], timebytes[2], timebytes[3],
				compflags,
				os
			};

			mStream->write( header, sizeof(header), EIoRequest::mustComplete );

			// Original file name.
			if( aOriginalName )
			{
				FLUX_ASSERT( !!(header[3] & 8) );

				auto const length = std::strlen( aOriginalName );
				mStream->write( aOriginalName, length+1 /*nul*/, EIoRequest::mustComplete );
			}
			if( aCommentField )
			{
				FLUX_ASSERT( !!(header[3] & 16) );

				auto const length = std::strlen( aCommentField );
				mStream->write( aCommentField, length+1 /*nul*/, EIoRequest::mustComplete );

			}
		}
	}
	void StreamGZWrite_::end_stream_()
	{
		auto res = tdefl_compress_buffer( &mState, nullptr, 0, TDEFL_FINISH );
		FLUX_THROW_IF( TDEFL_STATUS_DONE != res, error::StreamIoIncomplete )
			<< einfo::StreamMode( EStreamMode::write )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorDesc( "Finishing (TDEFL_FINISH) .gz stream failed" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("tdefl_status({:d}) = {}"), int(res), to_string_(res) )
			<< flux::einfo::ApiFunction( "tdefl_compress_buffer" )
		;
		
		if( !(EGZFlags::noFooter & mFlags) )
		{
			uint32_t const footer[2] = {
				mCrc32,                    // CRC32
				uint32_t(mUncompressed),   // uncompressed file size MOD 2^32
			};

			mStream->write( footer, sizeof(footer), EIoRequest::mustComplete );
		}
	}

	mz_bool StreamGZWrite_::put_( void const* aData, int aBytes )
	{
		FLUX_ASSERT( mStream );

		auto const bytes = util::narrow<StreamSize>(aBytes);
		try
		{
			mStream->write( aData, bytes, EIoRequest::mustComplete );
		}
		catch( error::StreamIoIncomplete const& )
		{
			return false;
		}

		return true;
	}
	mz_bool StreamGZWrite_::tdefl_put_callback_( void const* aData, int aBytes, void* aThis )
	{
		return reinterpret_cast<StreamGZWrite_*>(aThis)->put_( aData, aBytes );
	}
}

//--    $ throw_io_limited_()               ///{{{2///////////////////////////
namespace
{
	FLUX_ATTR_NORETURN
	void throw_io_limited_( Stream* aStream, StreamSize aLimit, StreamSize aRem, StreamSize aReq )
	{
		FLUX_ASSERT( aStream );
		FLUX_THROW( error::StreamIoIncomplete )
			<< einfo::StreamDesc( aStream->description() )
			<< flux::einfo::ErrorDesc( "Input stream size was limited" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("Limit = {:d}, remaining = {:d}, requested = {:d}"), aLimit, aRem, aReq )
		;
	}
}

//--    $ to_string_()                      ///{{{2///////////////////////////
namespace
{
	std::string to_string_( tinfl_status aStatus )
	{
		switch( aStatus )
		{
#			define FLUXTMP_MATCH_(x) case x: return #x
			FLUXTMP_MATCH_(TINFL_STATUS_FAILED_CANNOT_MAKE_PROGRESS);
			FLUXTMP_MATCH_(TINFL_STATUS_BAD_PARAM);
			FLUXTMP_MATCH_(TINFL_STATUS_ADLER32_MISMATCH);
			FLUXTMP_MATCH_(TINFL_STATUS_FAILED);
			FLUXTMP_MATCH_(TINFL_STATUS_DONE);
			FLUXTMP_MATCH_(TINFL_STATUS_NEEDS_MORE_INPUT);
			FLUXTMP_MATCH_(TINFL_STATUS_HAS_MORE_OUTPUT);
#			undef FLUXTMP_MATCH_
		}

		return format( "tinfl_status({:d})", int(aStatus) );
	}
}

//--    $ to_string_()                      ///{{{2///////////////////////////
namespace
{
	std::string to_string_( tdefl_status aStatus )
	{
		switch( aStatus )
		{
#			define FLUXTMP_MATCH_(x) case x: return #x
			FLUXTMP_MATCH_(TDEFL_STATUS_BAD_PARAM);
			FLUXTMP_MATCH_(TDEFL_STATUS_PUT_BUF_FAILED);
			FLUXTMP_MATCH_(TDEFL_STATUS_OKAY);
			FLUXTMP_MATCH_(TDEFL_STATUS_DONE);
#			undef FLUXTMP_MATCH_
		}

		return format( "tdefl_status({:d})", int(aStatus) );
	}
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
