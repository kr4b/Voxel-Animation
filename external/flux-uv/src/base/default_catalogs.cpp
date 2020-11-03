/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Default catalogs
 */
/*-***************************************************************** -}}}1- */

#include <flux/base/default_catalogs.hpp>

#include <flux/ftl/catalog.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::base >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(base)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	using ThreadCatalog_ = ftl::NestedCatalog<ftl::Catalog>;
}

//--    thread_catalog()                    ///{{{2///////////////////////////
ThreadCatalog_& thread_catalog() noexcept
{
	thread_local ThreadCatalog_ tlCatalog( &process_catalog(), kThreadCatalog );
	return tlCatalog;
}

//--    process_catalog()                   ///{{{2///////////////////////////
ftl::Catalog& process_catalog() noexcept
{
	static ftl::Catalog sProcessCatalog( kProcessCatalog );
	return sProcessCatalog;
}

//--    <<< ~ flux::base namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(base)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
