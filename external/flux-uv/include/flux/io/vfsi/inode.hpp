/*-******************************************************* -- HEADER -{{{1- */
/*-	VFSinternal - internal node
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFSI_INODE_HPP_86CC3CC4_9690_4442_B427_85E77958997A
#define FLUX_IO_VFSI_INODE_HPP_86CC3CC4_9690_4442_B427_85E77958997A

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/optional.hpp>
#include <flux/std/shared_ptr.hpp>
#include <flux/std/unique_ptr.hpp>

#include <flux/compat/string_view.hpp>
#include <flux/compat/system_error.hpp>

#include <flux/sys/syspath.hpp>

#include "../vfs.hpp"

//--    >>> namespace = flux::io::vfsi >>>      ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    constants                       ///{{{2///////////////////////////////

extern VINode const* const kAncestorNode;

//--    VINode                          ///{{{2///////////////////////////////
class VINode
	: public std::enable_shared_from_this<VINode>
{
	public:
		VINode() noexcept;

		VINode( std::string, weak_ptr<VINode>, VfsStat const& = VfsStat{} ) noexcept;
		//VINode( VfsStat const&, std::string, shared_ptr<VINode> const& ) noexcept;

		VINode( VINode const& ) = delete;
		VINode& operator= (VINode const&) = delete;
	
		virtual ~VINode() = 0;

	public:
		VfsStat stat() const noexcept;

		std::string path( VINode const* ) const;
		compat::string_view name() const noexcept;

		weak_ptr<VINode> const& ancestor() const noexcept;

	public: /* virtuals: generic */
		virtual std::string desc() const = 0;

		virtual bool update( bool aRecursive, compat::error_code& ) noexcept;
		virtual bool remove( bool aRecursive, compat::error_code& ) noexcept;

		virtual optional<sys::SysPathStr> native_path( compat::error_code& ) const;

	public: /* virtuals: file access */
		virtual VfsSize file_size( compat::error_code& ) noexcept;

		virtual unique_ptr<Buffer> file_as_buffer( 
			EBufferMode,
			bool aMustExist,
			BufferSize aResizeTo,
			compat::error_code&
		) noexcept;
		virtual unique_ptr<Stream> file_as_stream(
			EStreamMode,
			compat::error_code&
		) noexcept;

	public: /* virtuals: directory access */
		// TODO: dir_open() => should this be able to create new entries?
		//    - extra flags?
		virtual optional<VfsStat> dir_stat( compat::string_view const&, compat::error_code& ) noexcept;
		virtual shared_ptr<VINode> dir_open( compat::string_view const&, compat::error_code& ) noexcept;

		virtual unique_ptr<Buffer> dir_open_buffer( 
			compat::string_view const&,
			EBufferMode,
			bool aMustExist, //XXX-rename to aCreateRecursive
			BufferSize aResizeTo,
			compat::error_code&
		) noexcept;
		virtual unique_ptr<Stream> dir_open_stream( 
			compat::string_view const&,
			EStreamMode,
			//bool aCreateRecursive = false //XXX-TODO-maybe?
			compat::error_code&
		) noexcept;

		virtual void dir_create( compat::string_view const&, bool = false );
		virtual bool dir_remove( compat::string_view const&, bool = false );

		//TODO: enumerate entries
	
	protected:
		// NOTE/FIXME: the following N functions allocate, but are noxcept.
		optional<optional<VfsStat>> dir_stat_defaults( 
			compat::string_view const&,
			bool aContain,
			std::string*,
			compat::error_code&
		) noexcept;
		optional<shared_ptr<VINode>> dir_open_defaults( 
			compat::string_view const&,
			bool aContain,
			std::string*,
			compat::error_code&
		) noexcept;

		optional<unique_ptr<Buffer>> dir_open_buffer_defaults(
			compat::string_view const&,
			EBufferMode,
			bool,
			BufferSize,
			bool aContain,
			std::string*,
			compat::error_code&
		) noexcept;
		optional<unique_ptr<Stream>> dir_open_stream_defaults(
			compat::string_view const&,
			EStreamMode,
			bool aContain,
			std::string*,
			compat::error_code&
		) noexcept;

		bool dir_create_defaults( 
			compat::string_view const&,
			bool,
			bool aContain = false,
			std::string* = nullptr
		);
		optional<bool> dir_remove_defaults(
			compat::string_view const&,
			bool,
			bool aContain = false,
			std::string* = nullptr
		);

	protected:
		VfsStat mStat;

		//TODO: look at this:
		//TODO: should we store the distance to parent (=> rename mAncestor), or
		//  perhaps the relative path to it?
		//
		//TODO: should mPathFromAncestor include "our" name, in which case 
		// vname() should just extract that, or just path elements, so that
		// vname() needs to be appended to the path.
		//
		//TODO: just make this mandatory? We're storing it for each node anyway
		// with the following, so we could drop the vname cap and move vname()
		// vpath to non-virtual methods (but make the constructor arguments
		// mandatory).
		weak_ptr<VINode> mAncestor;
		std::string mPathFromAncestor;
};

//--    <<< ~ flux::io::vfsi namespace <<<      ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_VFSI_INODE_HPP_86CC3CC4_9690_4442_B427_85E77958997A
