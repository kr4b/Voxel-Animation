/*-******************************************************* -- HEADER -{{{1- */
/*-	VFSinternal - system directory
 *
 * TODO: additional access restrictions
 *   - E.g., being contained
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFSI_SYSDIR_HPP_567311C3_E2D0_447B_A9D0_2733CEEE3C36
#define FLUX_IO_VFSI_SYSDIR_HPP_567311C3_E2D0_447B_A9D0_2733CEEE3C36

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/sys/syspath.hpp>

#include <flux/util/enum_bitfield.hpp>

#include "inode.hpp"

//--    >>> namespace = flux::io::vfsi >>>      ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    constants                       ///{{{2///////////////////////////////

enum class ESysDirConstraints
{
	none,

	/** Contain paths
	 *
	 * Requires that paths that reference a parent directory (via `..`) go
	 * via the `SysDir`'s ancestor. If no ancestor is set, then access is
	 * denied via a `error::VfsDenied` exception.
	 */
	contain = (1<<0),
};
FLUX_UTIL_DEFINE_ENUM_BITFIELD(ESysDirConstraints);

//--    SysDir                          ///{{{2///////////////////////////////
class SysDir final : public VINode
{
	public:
		SysDir( sys::SysPathStr const&, ESysDirConstraints );
		SysDir( sys::SysPathStr const&, ESysDirConstraints, std::string, weak_ptr<VINode> );

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
		sys::SysPathStr make_path_( compat::string_view const& );
		
	private:
		sys::SysPathStr mSelfPath;
		ESysDirConstraints mConstraints;
};

//--    <<< ~ flux::io::vfsi namespace <<<      ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_VFSI_SYSDIR_HPP_567311C3_E2D0_447B_A9D0_2733CEEE3C36
