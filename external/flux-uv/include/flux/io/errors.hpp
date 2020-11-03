/*-******************************************************* -- HEADER -{{{1- */
/*-	IO Errors
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_ERRORS_HPP_B9716339_BE12_45FF_89F8_971F79AFDA5F
#define FLUX_IO_ERRORS_HPP_B9716339_BE12_45FF_89F8_971F79AFDA5F

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/io/vfs.hpp>
#include <flux/io/forward.hpp>

#include <flux/std/stdexcept.hpp>
#include <flux/std/except/einfo.hpp>

//--    >>> namespace = flux::io::error >>>     ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,error)
//--    errors                          ///{{{2///////////////////////////////

struct IoError
	: flux::error::RuntimeError
{};

	struct StreamError
		: IoError
	{};

		struct StreamNotAvailable
			: StreamError
		{};

		struct StreamIoIncomplete
			: StreamError
		{};
		struct StreamOpNotSupported
			: StreamError
		{};
		struct StreamInvalidSeek
			: StreamError
		{};

		struct StreamBadFormat
			: StreamError
		{};
		struct StreamBadOptions
			: StreamError
		{};
		struct StreamExpired
			: StreamError
		{};

		struct StreamSysError
			: StreamError
		{};

	struct BufferError
		: IoError
	{};

		struct BufferNotAvailable
			: BufferError
		{};
		struct BufferOpNotSupported
			: BufferError
		{};
		struct BufferOutOfRange
			: BufferError
		{};

		struct BufferTooLarge
			: BufferError
		{};

	struct VfsError
		: IoError
	{};

		struct VfsDenied
			: VfsError
		{};
		struct VfsNotFound
			: VfsError
		{};
		struct VfsNotADir
			: VfsError
		{};
		struct VfsOpNotSupported
			: VfsError
		{};

		struct VfsInvalidRecord
			: VfsError
		{};

//--    <<< ~ flux::io::errors namespace <<<    ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,error)
//--    >>> namespace = flux::io::einfo >>>     ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,einfo)
//--    error info                      ///{{{2///////////////////////////////

using VfsDesc = flux::einfo::ErrorInfo< struct VfsDesc_, std::string >;
using StreamDesc = flux::einfo::ErrorInfo< struct StreamDesc_, std::string >;
using BufferDesc = flux::einfo::ErrorInfo< struct BufferDesc_, std::string >;

struct StreamMode : flux::einfo::ErrorInfo< struct StreamMode_, EStreamMode >
{
	using EInfo_ = flux::einfo::ErrorInfo< struct StreamMode_, EStreamMode >;
	
	public:
		explicit StreamMode( EStreamMode aMode ) noexcept
			: EInfo_( aMode )
		{}

	public:
		using type = /*typename*/ EInfo_::type;
	
	public:
		static char const* name() noexcept
		{
			return "io::EStreamMode";
		}
		static std::string to_string( EStreamMode aMode )
		{
			return io::to_string(aMode);
		}
};
struct BufferMode : flux::einfo::ErrorInfo< struct BufferMode_, EBufferMode >
{
	using EInfo_ = flux::einfo::ErrorInfo< struct BufferMode_, EBufferMode >;
	
	public:
		explicit BufferMode( EBufferMode const& aMode ) noexcept
			: EInfo_( aMode )
		{}

	public:
		using type = /*typename*/ EInfo_::type;
	
	public:
		static char const* name() noexcept
		{
			return "io::EBufferMode";
		}
		static std::string to_string( EBufferMode const& aMode )
		{
			return io::to_string(aMode);
		}
};

struct VfsRecordStat : flux::einfo::ErrorInfo< struct VfsStat_, VfsStat>
{
	using EInfo_ = flux::einfo::ErrorInfo< struct VfsStat_, VfsStat >;
	
	public:
		explicit VfsRecordStat( VfsStat const& aStat ) noexcept
			: EInfo_( aStat )
		{}

	public:
		using type = /*typename*/ EInfo_::type;
	
	public:
		static char const* name() noexcept
		{
			return "io::VfsStat";
		}
		static std::string to_string( VfsStat const& aStat )
		{
			return io::to_string(aStat);
		}
};

//--    <<< ~ flux::io::einfo namespace <<<     ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,einfo)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_ERRORS_HPP_B9716339_BE12_45FF_89F8_971F79AFDA5F
