/*-******************************************************* -- HEADER -{{{1- */
/*-	VFSinternal - special user directory
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFSI_USERDIR_HPP_5C2A27E9_D60A_44F5_9065_AE8AC5AE0525
#define FLUX_IO_VFSI_USERDIR_HPP_5C2A27E9_D60A_44F5_9065_AE8AC5AE0525

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/sys/defpath.hpp>

#include "inode.hpp"

//--    >>> namespace = flux::io::vfsi >>>      ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    constants                       ///{{{2///////////////////////////////

//--    UserDir                         ///{{{2///////////////////////////////
class UserDir final : public VINode
{
	public:
		UserDir( sys::EUserPath, std::string, weak_ptr<VINode> );

	public: /* virtuals: generic */
		std::string desc() const override;

		bool update( bool, compat::error_code& ) noexcept override;
		bool remove( bool, compat::error_code& ) noexcept override;

		optional<sys::SysPathStr> native_path( compat::error_code& ) const override;

	public: /* virtuals: directory access */
		optional<VfsStat> dir_stat( compat::string_view const&, compat::error_code& ) noexcept override;
		shared_ptr<VINode> dir_open( compat::string_view const&, compat::error_code& ) noexcept override;

		unique_ptr<Buffer> dir_open_buffer( 
			compat::string_view const&,
			EBufferMode,
			bool,
			BufferSize,
			compat::error_code&
		) noexcept override;
		unique_ptr<Stream> dir_open_stream( 
			compat::string_view const&,
			EStreamMode,
			compat::error_code&
		) noexcept override;

		void dir_create( compat::string_view const&, bool ) override;
		bool dir_remove( compat::string_view const&, bool ) override;

	private:
		std::string dirname_() const;
		sys::SysPathStr make_path_( compat::string_view const& );
		
	private:
		sys::EUserPath mUserPath;
};

//--    <<< ~ flux::io::vfsi namespace <<<      ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_VFSI_USERDIR_HPP_5C2A27E9_D60A_44F5_9065_AE8AC5AE0525
