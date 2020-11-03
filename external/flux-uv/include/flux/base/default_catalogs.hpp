/*-******************************************************* -- HEADER -{{{1- */
/*-	Default `ftl::Catalog` instances
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_BASE_DEFAULT_CATALOGS_HPP_FAFDD5A9_F1E0_4C3E_A9F1_7550A5C611C0
#define FLUX_BASE_DEFAULT_CATALOGS_HPP_FAFDD5A9_F1E0_4C3E_A9F1_7550A5C611C0

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/ftl/catalog.hpp>

//--    >>> namespace = flux::base >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(base)
//--    constants                           ///{{{2///////////////////////////

constexpr ftl::ECatalogIdent kThreadCatalog = ftl::ECatalogIdent(0xffffff02u);
constexpr ftl::ECatalogIdent kProcessCatalog = ftl::ECatalogIdent(0xffffff01u);

//--    functions                           ///{{{2///////////////////////////

/** Default process-wide catalog
 *
 * `process_catalog()` returns a reference to the default process-wide 
 * `ftl::Catalog`.
 * 
 * \see thread_catalog()
 */
ftl::Catalog& process_catalog() noexcept;

/** Default thread-local catalog
 *
 * `thread_catalog()` returns a thread-local catalog; each thread owns an
 * unique instance of this catalog. The catalog inherits items from the
 * `process_catalog()`.
 *
 * Items in the `thread_catalog()` are destroyed when the thread exits.
 */
ftl::NestedCatalog<ftl::Catalog>& thread_catalog() noexcept;

//--    <<< ~ flux::base namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(base)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_BASE_DEFAULT_CATALOGS_HPP_FAFDD5A9_F1E0_4C3E_A9F1_7550A5C611C0
