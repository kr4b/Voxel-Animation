/*-******************************************************* -- HEADER -{{{1- */
/*-	VFS
 *
 * TODO
 *   - Need rethink.
 *   - VfsRoot that carries a context (separate? or implicit?) which allows
 *     specification of e.g., what is a directory, so that we can treat .zip
 *     files as directories automagically.
 *
 *     User callback or similar.
 *
 *   - Would be nice to avoid those stinkin' shared_ptr<>s everywhere. Maybe
 *     a light-weight wrapper? Although at that point we might just be re-
 *     inventing a worse shared_ptr<>...
 *
 *   - Should have convenience functions to avoid fucking around too much.
 *     E.g.:
 *        -  vfs_{dir_}get_entry() <- generic
 *        -  vfs_{dir_}get_dir() <- directory only
 *        -  vfs_{dir_}get_file() <- file only
 *        -  vfs_{dir_}create_stream() <- Stream*
 *        -  vfs_{dir_}create_buffer() <- Buffer*
 *     (maybe skip the "dir_" part in the middle, because annoying.)
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFS_HPP_1F8363CD_81F5_4BDC_B215_5CB5F7238D07
#define FLUX_IO_VFS_HPP_1F8363CD_81F5_4BDC_B215_5CB5F7238D07

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/constexpr.hpp>
#include <flux/compat/string_view.hpp>
#include <flux/compat/system_error.hpp>

#include <flux/std/optional.hpp>
#include <flux/std/shared_ptr.hpp>
#include <flux/std/unique_ptr.hpp>

#include <flux/sys/syspath.hpp> // TODO: move this into a separate header?

#include "forward.hpp"

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    constants                       ///{{{2///////////////////////////////

#if 0
namespace vfs
{
	enum class EFindFilter
	{
		defaults = 0
	};
}
#endif

//--    types                           ///{{{2///////////////////////////////

struct VfsStat
{
	FLUX_CONSTEXPR_EX VfsStat() noexcept;

	unsigned removable : 1;
	
	unsigned file : 1;
	unsigned readable : 1;
	unsigned writable : 1;
	unsigned mappable : 1;
	unsigned sized : 1;

	unsigned directory : 1;
	unsigned accessible : 1;
	unsigned listable : 1;
	unsigned changeable : 1;
	unsigned flat : 1; // flat = can't have nested directories.

	unsigned special : 1;
	// readable
	// writable
	
	unsigned hasNativeMap : 1;
	unsigned hasNativePath : 1;

	//unsigned vfsName : 1;
	//unsigned vfsPath : 1; //XXX- XX- XXX 
};


//--    VfsRecord                       ///{{{2///////////////////////////////
namespace detail
{
	struct VfsRecordFactory;
}

class VfsRecord final
{
	public:
		VfsRecord(), ~VfsRecord(); //XXX

	public:
		shared_ptr<vfsi::VINode> const& self() const;
		shared_ptr<vfsi::VINode> const& root() const;

		//void reset_root( shared_ptr<vfsi::VINode> const& = {} );
	
	private:
		VfsRecord(
			shared_ptr<vfsi::VINode>,
			shared_ptr<vfsi::VINode>
		) noexcept;

		friend struct detail::VfsRecordFactory;

	private:
		shared_ptr<vfsi::VINode> mSelf;
		shared_ptr<vfsi::VINode> mRoot;
		//TODO: some sort of Context record.
};

//--    functions                       ///{{{2///////////////////////////////

//TODO: docs. Important: when does vfs_stat() fill in the error_code and
//   when doesn't it do so? NotFound = no error code, but what about 
//   missing intermediate directories? When is there an error_code?
VfsStat vfs_stat( VfsRecord const& ) noexcept;
optional<VfsStat> vfs_stat( 
	VfsRecord const&, 
	compat::string_view const&
) noexcept;
optional<VfsStat> vfs_stat( 
	VfsRecord const&, 
	compat::string_view const&,
	compat::error_code& 
) noexcept;

bool vfs_is_file( VfsRecord const& ) noexcept;
bool vfs_is_dir( VfsRecord const& ) noexcept;
bool vfs_is_special( VfsRecord const& ) noexcept;



VfsRecord vfs_open(
	VfsRecord const&,
	compat::string_view const&
);
optional<VfsRecord> vfs_open(
	VfsRecord const&,
	compat::string_view const&,
	compat::error_code&
) noexcept;


unique_ptr<Buffer> vfs_open_buffer( 
	VfsRecord const&,
	EBufferMode = EBufferMode::read,
	bool aMustExist = true,
	BufferSize aResizeTo = ~BufferSize(0)
);
unique_ptr<Stream> vfs_open_stream(
	VfsRecord const&,
	EStreamMode = EStreamMode::read
);
//TODO: noexcept overloads of the above?

unique_ptr<Buffer> vfs_open_buffer( 
	VfsRecord const&,
	compat::string_view const&,
	EBufferMode = EBufferMode::read,
	bool aMustExist = true,
	BufferSize aResizeTo = ~BufferSize(0)
);
unique_ptr<Stream> vfs_open_stream(
	VfsRecord const&,
	compat::string_view const&,
	EStreamMode = EStreamMode::read
);
//TODO: noexcept overloads of the above?


void vfs_mkdir(
	VfsRecord const&,
	compat::string_view const&,
	bool aRecursive = true
);

void vfs_remove( 
	VfsRecord const&,
	compat::string_view const&,
	bool aRecursive = false
);
/* Small cautionary note: This used to be called `vfs_remove()`, i.e.  it was
 * an overload of the above method. However, it turns out that `char const*`
 * will convert more readily to a `bool` (that is true) than to a
 * `string_view`. 
 */
void vfs_delete_record(
	VfsRecord const&, 
	bool aRecursive = false 
);

//TODO: noexcept overloads of the above?


std::string vfs_abspath( VfsRecord const& );
std::string vfs_description( VfsRecord const& );

#if 0
VfsRecord vfs_open( VfsRecord const&, compat::string_view const& );
VfsRecord vfs_open_dir( VfsRecord const&, compat::string_view const& );
VfsRecord vfs_open_file( VfsRecord const&, compat::string_view const& );
#endif

//TODO:
//
//  - something: get file size of VfsRecord if it's a file
//  - [maybe] function to get native path (if any) of VfsRecord
//
//  - enumerating items in directory :-(

#if 0
std::size_t vfs_file_size( VfsEntry const& );

VfsEntry vfs_dir_find( VfsEntry const&, vfs::Path const&, vfs::EFindFilter = vfs::EFindFilter::defaults );
vfs::DirIterator vfs_dir_begin( VfsEntry const& ); // TODO: flags? recursive?


bool vfs_dir_has_root( VfsEntry const& );

//VfsEntry const& vfs_default_root(); // TODO: Catalog? Here or in flux-base?
#endif

std::string to_string( VfsStat const& );

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/vfs.inl"
#endif // FLUX_IO_VFS_HPP_1F8363CD_81F5_4BDC_B215_5CB5F7238D07
