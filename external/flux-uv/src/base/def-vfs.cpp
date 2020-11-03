/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Default VFS key
 */
/*-***************************************************************** -}}}1- */

#include <flux/setup.pkg>

#if FLUX_WITH_FLUXIO
#	include <flux/base/def/vfs.hpp>

#	include <flux/ftl/catalog.hpp>
#	include <flux/base/default_catalogs.hpp>
#	include <flux/io/vfs/default_entry_record.hpp>
#endif // ~ WITH_FLUXIO

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::base::def::detail >>> ///{{{1///////////////////
FLUX_NAMESPACE_ENTER3(base,def,detail)
//--    d: gDefaultVfsAutoconf              ///{{{2///////////////////////////
#if FLUX_WITH_FLUXIO
namespace
{
	using FPtr_ = void (*)(void*, ftl::Catalog&);
}

ftl::CatalogKeyAutoconf const gProcessVfsAutoconf = {
	base::kProcessCatalog,
	sizeof(io::VfsRecord),
	static_cast<FPtr_>([] (void* p, ftl::Catalog&) { 
		new (p) io::VfsRecord( io::vfs::default_entry_record() );
	}),
	static_cast<FPtr_>([] (void* p, ftl::Catalog&) { 
		reinterpret_cast<io::VfsRecord*>(p)->~VfsRecord();
	})
};
#endif // ~ WITH_FLUXIO

//--    <<< ~ flux::base::def::detail namespace <<< ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE3(base,def,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
