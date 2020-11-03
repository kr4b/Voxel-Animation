#ifndef DETAIL_VFS_RECORD_FACTORY_HXX_A05F2662_9575_4FC6_B318_CBC8B71306B1
#define DETAIL_VFS_RECORD_FACTORY_HXX_A05F2662_9575_4FC6_B318_CBC8B71306B1

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/io/vfs.hpp>

//--    >>> namespace = flux::io::detail >>>        ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,detail)
//--    VfsRecordFactory                    ///{{{2///////////////////////////

struct VfsRecordFactory
{
	static
	VfsRecord make( shared_ptr<vfsi::VINode> aSelf, shared_ptr<vfsi::VINode> aRoot ) noexcept
	{
		FLUX_ASSERT( aSelf && aRoot );
		return VfsRecord( aSelf, aRoot );
	};
};

//--    <<< ~ flux::io::detail namespace <<<        ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_VFS_RECORD_FACTORY_HXX_A05F2662_9575_4FC6_B318_CBC8B71306B1
