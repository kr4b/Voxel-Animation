/*-******************************************************* -- HEADER -{{{1- */
/*-	VFSinternal - overlay directory
 *
 * NOTE: Technically, this could be composed via the newer UnionDir+VDir
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFSI_OVERLAY_HPP_C2FF4B03_C93A_46E8_8C40_7265CA4CE62F
#define FLUX_IO_VFSI_OVERLAY_HPP_C2FF4B03_C93A_46E8_8C40_7265CA4CE62F

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

//#include <unordered_map>
#include <flux_ext/phmap.hpp>

#include "inode.hpp"

//--    >>> namespace = flux::io::vfsi >>>      ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    OverlayDir                      ///{{{2///////////////////////////////
class OverlayDir final : public VINode
{
	public:
		/** Create overlay over \a VINode
		 *
		 * Creates an overlay over existing directory node \a VINode (the
		 * "base" directory).
		 *
		 * Normally, the overlay may only be modified via the special `add` and
		 * `remove` methods of `OverlayDir`. If `aPermitOverlayChanges` is set
		 * to true, the overlay (<b>and base</b>) may be deleted via the
		 * standard `remove()` method, and overlay entries can be deleted via
		 * the standard `dir_remove()` method.
		 */
		explicit OverlayDir( shared_ptr<VINode>, bool aPermitOverlayChanges = false );

	public:
		void add( shared_ptr<VINode> );

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
		shared_ptr<VINode> mBase;
		//std::unordered_map<compat::string_view,shared_ptr<VINode>> mOverlay;
		flux_ext::flat_hash_map<compat::string_view,shared_ptr<VINode>> mOverlay;
		bool mPermitOverlayChanges;
};

//--    <<< ~ flux::io::vfsi namespace <<<      ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_VFSI_SYSDIR_HPP_567311C3_E2D0_447B_A9D0_2733CEEE3C36
