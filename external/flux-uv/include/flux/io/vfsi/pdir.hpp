/*-******************************************************* -- HEADER -{{{1- */
/*-	VFSinternal - virtual portal directory
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_PDIR_HPP_D782F86F_90F2_4147_8E8F_6A9B66F1A755
#define FLUX_IO_PDIR_HPP_D782F86F_90F2_4147_8E8F_6A9B66F1A755

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

//#include <unordered_map>
#include <flux_ext/phmap.hpp>

#include "inode.hpp"

//--    >>> namespace = flux::io::vfsi >>>      ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    PDir                            ///{{{2///////////////////////////////
class PDir final : public VINode
{
	public:
		explicit PDir( std::string, weak_ptr<VINode> );

	public:
		void add( compat::string_view const&, shared_ptr<VINode> );
		void remove( compat::string_view const& );

	public: /* virtuals: generic */
		std::string desc() const override;

		bool update( bool, compat::error_code& ) noexcept override;
		bool remove( bool, compat::error_code& ) noexcept override;

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
		//std::unordered_map<std::string,shared_ptr<VINode>> mDir;
		flux_ext::flat_hash_map<std::string,shared_ptr<VINode>> mDir;
};

//--    <<< ~ flux::io::vfsi namespace <<<      ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_PDIR_HPP_D782F86F_90F2_4147_8E8F_6A9B66F1A755
