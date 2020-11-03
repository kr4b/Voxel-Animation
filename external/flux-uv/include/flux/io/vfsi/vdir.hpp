/*-******************************************************* -- HEADER -{{{1- */
/*-	VFSinternal - virtual directory
 *
 * TODO: can this be merged with the OverlayDir? It's very very similar.
 *   - Maybe OverlayDir = Stack{ VDir, Base }??
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFSI_VDIR_HPP_A91D4428_C0E4_4681_91C5_C13BAF73BA78
#define FLUX_IO_VFSI_VDIR_HPP_A91D4428_C0E4_4681_91C5_C13BAF73BA78

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

//#include <unordered_map>
#include <flux_ext/phmap.hpp>

#include "inode.hpp"

//--    >>> namespace = flux::io::vfsi >>>      ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    VDir                            ///{{{2///////////////////////////////
class VDir final : public VINode
{
	public:
		explicit VDir( std::string, weak_ptr<VINode>, bool aReadOnly = true );

	public:
		void add( shared_ptr<VINode> );
		void add( shared_ptr<VINode>, compat::string_view const& );

		void remove( shared_ptr<VINode> );
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
		//std::unordered_map<compat::string_view,shared_ptr<VINode>> mDir;
		flux_ext::flat_hash_map<compat::string_view, shared_ptr<VINode>> mDir;
		bool mReadOnly;
};

//--    <<< ~ flux::io::vfsi namespace <<<      ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_VFSI_VDIR_HPP_A91D4428_C0E4_4681_91C5_C13BAF73BA78
