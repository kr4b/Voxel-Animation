/*-******************************************************* -- HEADER -{{{1- */
/*-	VFSinternal - lazily created directory
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFSI_LAZYDIR_HPP_614D8C59_73D1_4D26_B798_239F02F77955
#define FLUX_IO_VFSI_LAZYDIR_HPP_614D8C59_73D1_4D26_B798_239F02F77955

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/sys/defpath.hpp>

#include "inode.hpp"

//--    >>> namespace = flux::io::vfsi >>>      ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    constants                       ///{{{2///////////////////////////////

//--    LazyDir                         ///{{{2///////////////////////////////
class LazyDir final : public VINode
{
	public:
		LazyDir( std::string, shared_ptr<VINode> );

	public: /* virtuals: generic */
		std::string desc() const override;

		bool update( bool, compat::error_code& ) noexcept override;
		bool remove( bool, compat::error_code& ) noexcept override;

		//TODO: might have a native path, depending on parent. Currently no
		//easy way of getting it from the parent; would have to reconstruct it
		//manually (which would limit it to certain special parents).
		//optional<sys::SysPathStr> native_path( compat::error_code& ) const override;

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
		std::string make_path_( compat::string_view const& );
		bool try_make_dir_( compat::error_code& ) noexcept;
		
	private:
		shared_ptr<VINode> mParent;
};

//--    <<< ~ flux::io::vfsi namespace <<<      ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_VFSI_LAZYDIR_HPP_614D8C59_73D1_4D26_B798_239F02F77955
