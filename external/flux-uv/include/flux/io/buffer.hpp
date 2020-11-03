/*-******************************************************* -- HEADER -{{{1- */
/*-	Simple host-mappable buffer abstraction
 *
 * TODO
 *   - nix: mmap() et al.
 *   - win: OpenFileMapping(), MapViewOfFile(), UnmapViewOfFile(), CloseHandle()
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_BUFFER_HPP_2A3BE85A_A832_4791_94B4_78B481B48E9F
#define FLUX_IO_BUFFER_HPP_2A3BE85A_A832_4791_94B4_78B481B48E9F

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/io/forward.hpp>

#include <flux/compat/byte.hpp>
#include <flux/compat/span.hpp>

#include <flux/ftl/any.hpp>
#include <flux/util/enum_bitfield.hpp>

#include <string>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    constants                       ///{{{2///////////////////////////////

enum class EBufferMapRO : unsigned { ro = 0 };
enum class EBufferMapRW : unsigned { wo = 1, rw };

struct EBufferMap
{
	static constexpr EBufferMapRO ro = EBufferMapRO::ro;
	static constexpr EBufferMapRW wo = EBufferMapRW::wo;
	static constexpr EBufferMapRW rw = EBufferMapRW::rw;

	EBufferMap() = default;
	constexpr EBufferMap( EBufferMapRO ) noexcept;
	constexpr EBufferMap( EBufferMapRW ) noexcept;

	constexpr operator EBufferMapRO() const noexcept;
	constexpr operator EBufferMapRW() const noexcept;

	constexpr unsigned value() const noexcept;

	private: unsigned mValue;
};

//--    types                           ///{{{2///////////////////////////////

struct BufferCaps
{
	BufferSize minOffsetAlignment;

	unsigned readable : 1;
	unsigned writable : 1;

	unsigned partialMap : 1;
	unsigned partialUnmap : 1;
	unsigned multiMap : 1;

	unsigned resizable : 1;
};

//--    Buffer                          ///{{{2///////////////////////////////

struct Buffer
{
	virtual ~Buffer() = 0;

	virtual compat::span<compat::byte> map( EBufferMapRW, BufferSize = 0, BufferSize = ~BufferSize(0) ) = 0;
	virtual compat::span<compat::byte const> map( EBufferMapRO, BufferSize = 0, BufferSize = ~BufferSize(0) ) = 0;

	virtual void unmap( compat::span<compat::byte const> const& ) = 0;

	virtual BufferCaps caps() const noexcept = 0;
	virtual BufferSize size() const = 0;

	virtual void flush( compat::span<compat::byte const> const& ) = 0;
	virtual void resize( BufferSize, bool aKeepContents = true ) = 0;

	virtual std::string description() const = 0;
	virtual ftl::Any native_handle() const = 0;
};

//--    functions                       ///{{{2///////////////////////////////

std::string to_string( EBufferMap );
std::string to_string( EBufferMapRO );
std::string to_string( EBufferMapRW );
std::string to_string( EBufferMode );

std::string to_string( BufferCaps );

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/buffer.inl"
#endif // FLUX_IO_BUFFER_HPP_2A3BE85A_A832_4791_94B4_78B481B48E9F

