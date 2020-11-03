/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	C FILE* Stream implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/stream/cfile.hpp>
#include <flux/io/errors.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>

#include <flux/compat/byte.hpp>
#include <flux/compat/system_error.hpp>

#include <flux/util/narrow.hpp>
#include <flux/sys/errno_category.hpp>

#include <algorithm>
#include <cstdio>
#include <errno.h>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	class StreamSpecialFILE_ final : public Stream
	{
		public:	
			StreamSpecialFILE_( FILE*, EStreamMode, std::string );

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
			void check_mode_( EStreamMode ) const;

		private:
			FILE* mFile;

			EStreamMode mMode;
			std::string mDesc;
	};

	class StreamSpecialNull_ final : public Stream
	{
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

			unique_ptr<Stream> clone() const override;
	};
	class StreamSpecialZero_ final : public Stream
	{
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

			unique_ptr<Stream> clone() const override;
	};

}

//--    stream_create_null()                ///{{{2///////////////////////////
unique_ptr<Stream> stream_create_null()
{
	return make_unique<StreamSpecialNull_>();
}

//--    stream_create_zero()                ///{{{2///////////////////////////
unique_ptr<Stream> stream_create_zero()
{
	return make_unique<StreamSpecialZero_>();
}

//--    stream_create_std*()                ///{{{2///////////////////////////
unique_ptr<Stream> stream_create_stdin()
{
	return make_unique<StreamSpecialFILE_>( stdin, EStreamMode::read, "<stdin>" );
}
unique_ptr<Stream> stream_create_stdout()
{
	return make_unique<StreamSpecialFILE_>( stdout, EStreamMode::append, "<stdout>" );
}
unique_ptr<Stream> stream_create_stderr()
{
	return make_unique<StreamSpecialFILE_>( stderr, EStreamMode::append, "<stderr>" );
}

//--    $ StreamSpecialNull_                ///{{{2///////////////////////////
namespace
{
	StreamSize StreamSpecialNull_::read( void*, StreamSize, EIoRequest )
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::append )
			<< einfo::StreamDesc( "<special:null>" )
			<< flux::einfo::ErrorMeta( "Stream::read()" )
		;
	}
	StreamSize StreamSpecialNull_::write( void const* aBuffer, StreamSize aSize, EIoRequest )
	{
		FLUX_ASSERT( aBuffer );
		(void)aBuffer;
	
		return aSize;
	}

	EStreamStatus StreamSpecialNull_::status() const noexcept
	{
		return EStreamStatus::writable;
	}

	StreamCaps StreamSpecialNull_::caps() const noexcept
	{
		StreamCaps caps;
		caps.readable = false;
		caps.writable = true;
		caps.size = false;
		caps.tell = false;
		caps.seekFwd = true;
		caps.seekRev = true;
		caps.remaining = false;
		caps.infinite = false;
		caps.clonable = true;
		return caps;
	}

	StreamSize StreamSpecialNull_::size() const
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::append )
			<< einfo::StreamDesc( "<special:null>" )
			<< flux::einfo::ErrorMeta( "Stream::size()" )
		;
	}

	StreamSize StreamSpecialNull_::tell() const
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::append )
			<< einfo::StreamDesc( "<special:null>" )
			<< flux::einfo::ErrorMeta( "Stream::tell()" )
		;
	}
	StreamSize StreamSpecialNull_::seek( StreamOffset, ESeekWhence )
	{
		return 0;
	}
	StreamSize StreamSpecialNull_::remaining() const
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::append )
			<< einfo::StreamDesc( "<special:null>" )
			<< flux::einfo::ErrorMeta( "Stream::remaining()" )
		;
	}

	void StreamSpecialNull_::flush()
	{}

	std::string StreamSpecialNull_::description() const
	{
		return "<special:null>";
	}
	ftl::Any StreamSpecialNull_::native_handle() const
	{
		return ftl::Any();
	}

	unique_ptr<Stream> StreamSpecialNull_::clone() const
	{
		return make_unique<StreamSpecialNull_>();
	}
}

//--    $ StreamSpecialZero_                ///{{{2///////////////////////////
namespace
{
	StreamSize StreamSpecialZero_::read( void* aBuffer, StreamSize aSize, EIoRequest )
	{
		std::memset( aBuffer, 0, aSize );
		return aSize;
	}
	StreamSize StreamSpecialZero_::write( void const*, StreamSize, EIoRequest )
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( "<special:zero>" )
			<< flux::einfo::ErrorMeta( "Stream::write()" )
		;
	}

	EStreamStatus StreamSpecialZero_::status() const noexcept
	{
		return EStreamStatus::readable;
	}

	StreamCaps StreamSpecialZero_::caps() const noexcept
	{
		StreamCaps caps;
		caps.readable = true;
		caps.writable = false;
		caps.size = false;
		caps.tell = false;
		caps.seekFwd = true;
		caps.seekRev = true;
		caps.remaining = false;
		caps.infinite = true;
		caps.clonable = true;
		return caps;
	}

	StreamSize StreamSpecialZero_::size() const
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( "<special:zero>" )
			<< flux::einfo::ErrorMeta( "Stream::size()" )
		;
	}

	StreamSize StreamSpecialZero_::tell() const
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( "<special:zero>" )
			<< flux::einfo::ErrorMeta( "Stream::tell()" )
		;
	}
	StreamSize StreamSpecialZero_::seek( StreamOffset, ESeekWhence )
	{
		return 0;
	}
	StreamSize StreamSpecialZero_::remaining() const
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( EStreamMode::read )
			<< einfo::StreamDesc( "<special:zero>" )
			<< flux::einfo::ErrorMeta( "Stream::remaining()" )
		;
	}

	void StreamSpecialZero_::flush()
	{}

	std::string StreamSpecialZero_::description() const
	{
		return "<special:zero>";
	}
	ftl::Any StreamSpecialZero_::native_handle() const
	{
		return ftl::Any();
	}

	unique_ptr<Stream> StreamSpecialZero_::clone() const
	{
		return make_unique<StreamSpecialZero_>();
	}

}

//--    $ StreamSpecialFILE_               ///{{{2///////////////////////////
namespace
{
	StreamSpecialFILE_::StreamSpecialFILE_( FILE* aFile, EStreamMode aMode, std::string aDesc )
		: mFile( aFile )
		, mMode( aMode )
		, mDesc( aDesc )
	{}

	StreamSize StreamSpecialFILE_::read( void* aBuffer, StreamSize aSize, EIoRequest aReq )
	{
		FLUX_ASSERT( mFile && aBuffer );
		check_mode_( EStreamMode::read );

		auto req = util::narrow<std::size_t>(aSize);
		auto res = std::size_t(0);
		auto buf = static_cast<compat::byte*>(aBuffer);

		do
		{
			auto const rr = std::fread( buf+res, 1, req-res, mFile );
			if( 0 == rr )
				break;

			res += rr;
		} while( EIoRequest::mustComplete == aReq && res != req );

		if( res != req )
		{
			auto const errno_ = errno;
			FLUX_THROW_IF( std::ferror( mFile ), io::error::StreamSysError )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ApiFunction( "fread" )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			;
			
			FLUX_THROW_IF( EIoRequest::mustComplete == aReq, io::error::StreamIoIncomplete )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("read(mustComplete): requested {:d} bytes, got {:d}"), req, res )
			;
		}

		return util::narrow<StreamSize>(res);
	}
	StreamSize StreamSpecialFILE_::write( void const* aBuffer, StreamSize aSize, EIoRequest aReq )
	{
		FLUX_ASSERT( mFile && aBuffer );
		check_mode_( EStreamMode::write );

		auto req = util::narrow<std::size_t>(aSize);
		auto res = std::size_t(0);
		auto buf = static_cast<compat::byte const*>(aBuffer);

		do
		{
			auto const rr = std::fwrite( buf+res, 1, req-res, mFile );
			if( 0 == rr )
				break;

			res += rr;
		} while( EIoRequest::mustComplete == aReq && res != req );

		if( res != req )
		{
			auto const errno_ = errno;
			FLUX_THROW_IF( std::ferror( mFile ), io::error::StreamSysError )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ApiFunction( "fwrite" )
				<< flux::einfo::ApiError( sys::make_errno_error_code(errno_) )
			;
			
			FLUX_THROW_IF( EIoRequest::mustComplete == aReq, io::error::StreamIoIncomplete )
				<< einfo::StreamDesc( mDesc )
				<< einfo::StreamMode( mMode )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("write(mustComplete): specified {:d} bytes, wrote {:d}"), req, res )
			;
		}

		return util::narrow<StreamSize>(res);
	}

	EStreamStatus StreamSpecialFILE_::status() const noexcept
	{
		if( std::ferror( mFile ) )
			return EStreamStatus::error;

		EStreamStatus status = EStreamStatus(0);
		
		if( !!(EStreamMode::write & mMode) )
			status |= EStreamStatus::writable;

		if( !!(EStreamMode::read & mMode) && !std::feof(mFile) )
			status |= EStreamStatus::readable;
	
		return status;
	}

	StreamCaps StreamSpecialFILE_::caps() const noexcept
	{
		StreamCaps caps;
		caps.readable = !!(EStreamMode::read & mMode);
		caps.writable = !!(EStreamMode::write & mMode);
		caps.size = false;
		caps.tell = false;
		caps.seekFwd = false;
		caps.seekRev = false;
		caps.remaining = false;
		caps.infinite = false;
		return caps;
	}

	StreamSize StreamSpecialFILE_::size() const
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( mMode )
			<< einfo::StreamDesc( mDesc )
			<< flux::einfo::ErrorMeta( "Stream::size()" )
		;
	}

	StreamSize StreamSpecialFILE_::tell() const
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( mMode )
			<< einfo::StreamDesc( mDesc )
			<< flux::einfo::ErrorMeta( "Stream::tell()" )
		;
	}
	StreamSize StreamSpecialFILE_::seek( StreamOffset, ESeekWhence )
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( mMode )
			<< einfo::StreamDesc( mDesc )
			<< flux::einfo::ErrorMeta( "Stream::seek()" )
		;
	}
	StreamSize StreamSpecialFILE_::remaining() const
	{
		FLUX_THROW( io::error::StreamOpNotSupported )
			<< einfo::StreamMode( mMode )
			<< einfo::StreamDesc( mDesc )
			<< flux::einfo::ErrorMeta( "Stream::remaining()" )
		;
	}

	void StreamSpecialFILE_::flush()
	{
		FLUX_ASSERT( mFile );
		std::fflush( mFile );
	}

	std::string StreamSpecialFILE_::description() const
	{
		return mDesc;
	}
	ftl::Any StreamSpecialFILE_::native_handle() const
	{
		return ftl::Any( mFile );
	}


	void StreamSpecialFILE_::check_mode_( EStreamMode aRequired ) const
	{
		FLUX_THROW_IF( aRequired != (aRequired & mMode), error::StreamOpNotSupported )
			<< einfo::StreamMode( mMode )
			<< einfo::StreamDesc( mDesc )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("required mode: {}\n"), to_string(aRequired) )
		;
	}
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
