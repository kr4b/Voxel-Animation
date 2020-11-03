/*-******************************************************* -- HEADER -{{{1- */
/*-	VFSinternal - system file
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFSI_SYSFILE_HPP_998097BB_8F0B_4E8D_B815_E515462F3A9A
#define FLUX_IO_VFSI_SYSFILE_HPP_998097BB_8F0B_4E8D_B815_E515462F3A9A

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/sys/syspath.hpp>

#include "inode.hpp"

//--    >>> namespace = flux::io::vfsi >>>      ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    SysFile                         ///{{{2///////////////////////////////
class SysFile final : public VINode
{
	public:
		SysFile( sys::SysPathStr const& /*TODO: extra access masks*/ );
		SysFile( sys::SysPathStr const& /*TODO: extra access masks*/, std::string, weak_ptr<VINode> );

	public: /* virtuals: generic */
		std::string desc() const override;

		bool update( bool, compat::error_code& ) noexcept override;
		bool remove( bool, compat::error_code& ) noexcept override;

		optional<sys::SysPathStr> native_path( compat::error_code& ) const override;

	public: /* virtuals: file access */
		VfsSize file_size( compat::error_code& ) noexcept override;
	
		unique_ptr<Buffer> file_as_buffer( EBufferMode, bool, BufferSize, compat::error_code& ) noexcept override;
		unique_ptr<Stream> file_as_stream( EStreamMode, compat::error_code& ) noexcept override;

	private:
		sys::SysPathStr mSelfPath;
};

//--    <<< ~ flux::io::vfsi namespace <<<      ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_VFSI_SYSFILE_HPP_998097BB_8F0B_4E8D_B815_E515462F3A9A
