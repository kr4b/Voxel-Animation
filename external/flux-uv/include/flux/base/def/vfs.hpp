/*-******************************************************* -- HEADER -{{{1- */
/*-	Default Catalog item: VFS
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_BASE_DEF_VFS_HPP_EC187BD0_AE9D_4F0D_8587_47149A93A096
#define FLUX_BASE_DEF_VFS_HPP_EC187BD0_AE9D_4F0D_8587_47149A93A096

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/io/vfs.hpp>
#include <flux/ftl/catalog_key.hpp>

//--    checks                      ///{{{1///////////////////////////////////

#if !FLUX_WITH_FLUXIO
#	error "flux/base/def/vfs.hpp: requires flux-io (FLUXCFG_WITH_FLUXIO)"
#endif // ~ WITH_FLUXIO

//--    >>> namespace = flux::base::def >>>         ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(base,def)
//--    keys                                ///{{{2///////////////////////////
namespace detail
{
	extern ftl::CatalogKeyAutoconf const gProcessVfsAutoconf;
}

using ProcessVfs = ftl::CatalogKey<
	io::VfsRecord,
	io::VfsRecord const&,
	struct ProcessVfsTag_,
	ftl::ECatalogKeyKind::value,
	detail::gProcessVfsAutoconf
>;

//--    <<< ~ flux::base::def namespace <<<         ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(base,def)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_BASE_DEF_VFS_HPP_EC187BD0_AE9D_4F0D_8587_47149A93A096
