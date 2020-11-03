/*-******************************************************* -- HEADER -{{{1- */
/*-	Mapping helper
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_BUFFER_${NAME}_${UUID}
#define FLUX_IO_BUFFER_${NAME}_${UUID}

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/io/buffer.hpp>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    constants                       ///{{{2///////////////////////////////


//--    types                           ///{{{2///////////////////////////////

//--    Mapping                         ///{{{2///////////////////////////////

template< typename tDataType >
class Mapping final
{
	//TODO: maybe move this to buffer/mapping.hpp
	//TODO: use compat::span.
	//TODO: RO mapping if tDataType is const, RW mapping if it isn't

	public:
		explicit Mapping( Buffer*, BufferSize = 0, BufferSize = ~BufferSize(0) );
		explicit Mapping( unique_ptr<Buffer> const&, BufferSize = 0, BufferSize = ~BufferSize(0) );

		~Mapping();

	public:
		//TODO: operators. Probably operator[]

	public:
		//TODO: public interface
		//  - data()
		//  - size()
		//  - span()

	private:
		Buffer* mBuffer;

		tDataType* mPointer;
		BufferSize mMapSize;
};

//--    functions                       ///{{{2///////////////////////////////

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_BUFFER_${NAME}_${UUID}

