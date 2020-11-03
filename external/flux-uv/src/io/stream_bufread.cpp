/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Buffer-backed read-only Stream implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/stream/bufread.hpp>
#include <flux/io/errors.hpp>

#include <flux/io/buffer.hpp>
#include <flux/io/stream.hpp>

#if 0
#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>

#include <flux/compat/byte.hpp>
#include <flux/util/narrow.hpp>

#include <algorithm>
#endif

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	class StreamBufread_ final : public Stream
	{
		public:	
			StreamBufread_(  );

			~StreamBufread_();

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
			
	};
}


//--    stream_create_bufread()               ///{{{2///////////////////////////
unique_ptr<Stream> stream_create_bufread( ftl::not_null<Buffer*>, BufferSize, BufferSize )
{
	//TODO
	//TODO
	//TODO
	//TODO

	return {};
}
unique_ptr<Stream> stream_create_bufread( unique_ptr<Buffer>, BufferSize, BufferSize )
{
	//TODO
	//TODO
	//TODO
	//TODO

	return {};
}

//--    $ StreamBufread_                      ///{{{2///////////////////////////
namespace
{
#	if 0
	StreamBufread_::StreamBufread_( )
	{}

	StreamBufread_::~StreamBufread_()
	{}

	StreamSize StreamBufread_::read( void* aBuffer, StreamSize aSize, EIoRequest aReq )
	{
		//TODO
	}
	StreamSize StreamBufread_::write( void const*, StreamSize, EIoRequest )
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( description() )
			<< flux::einfo::ErrorMeta( "Stream::write()" )
		;
	}

	EStreamStatus StreamBufread_::status() const noexcept
	{
		if( mBufferPos < mBufferSize )
			return EStreamStatus::readable;

		return EStreamStatus();
	}

	StreamCaps StreamBufread_::caps() const noexcept
	{
		StreamCaps caps;
		caps.readable = true;
		caps.writable = false;
		caps.size = true;
		caps.tell = true;
		caps.seekFwd = true;
		caps.seekRev = true;
		caps.remaining = true;
		caps.infinite = false;
		return caps;
	}

	StreamSize StreamBufread_::size() const
	{
		return mBufferSize;
	}

	StreamSize StreamBufread_::tell() const
	{
		return mBufferPos;
	}
	StreamSize StreamBufread_::seek( StreamOffset aOffs, ESeekWhence aWhence )
	{
		std::size_t newPos = 0;
		std::size_t absOff = util::narrow<std::size_t>(std::abs(aOffs));

		switch( aWhence )
		{
			case ESeekWhence::set: {
				FLUX_THROW_IF( aOffs < 0, error::StreamInvalidSeek )
					<< einfo::StreamMode( EStreamMode::read )
					<< einfo::StreamDesc( description() )
					<< flux::einfo::ErrorDesc( "Stream::seek(set) with negative offset" )
				;
				newPos = absOff;
				break;
			}
			case ESeekWhence::cur: {
				if( aOffs > 0 )
				{
					FLUX_THROW_IF( mBufferSize-mBufferPos < absOff, error::StreamInvalidSeek )
						<< einfo::StreamMode( EStreamMode::read )
						<< einfo::StreamDesc( description() )
						<< flux::einfo::ErrorDesc( "Stream::seek(cur) offset out of bounds" )
					;

					newPos = mBufferPos + absOff;
				}
				else
				{
					FLUX_THROW_IF( absOff > mBufferPos, error::StreamInvalidSeek )
						<< einfo::StreamMode( EStreamMode::read )
						<< einfo::StreamDesc( description() )
						<< flux::einfo::ErrorDesc( "Stream::seek(cur) offset out of bounds" )
					;

					newPos = mBufferPos - absOff;
				}
				break;
			}
			case ESeekWhence::end: {
				FLUX_THROW_IF( aOffs < 0, error::StreamInvalidSeek )
					<< einfo::StreamMode( EStreamMode::read )
					<< einfo::StreamDesc( description() )
					<< flux::einfo::ErrorDesc( "Stream::seek(end) with positive offset" )
				;
				FLUX_THROW_IF( absOff > mBufferSize, error::StreamInvalidSeek )
					<< einfo::StreamMode( EStreamMode::read )
					<< einfo::StreamDesc( description() )
					<< flux::einfo::ErrorDesc( "Stream::seek(cur) offset out of bounds" )
				;

				newPos = mBufferSize - absOff;
				break;
			}
		}

		FLUX_ASSERT( newPos <= mBufferSize );
		mBufferPos = newPos;
		return mBufferPos;
	}
	StreamSize StreamBufread_::remaining() const
	{
		return mBufferSize-mBufferPos;
	}
						
	void StreamBufread_::flush()
	{}

	std::string StreamBufread_::description() const
	{
		return format( "bufread@{:p]+{:d}", mBuffer, mBufferSize );
	}
	ftl::Any StreamBufread_::native_handle() const
	{
		return ftl::Any( mBuffer );
	}
#	endif
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
