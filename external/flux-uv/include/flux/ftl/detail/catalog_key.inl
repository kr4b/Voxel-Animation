/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- CatalogKey implementation
 */
/*-***************************************************************** -}}}1- */

#include <type_traits>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    is_catalog_key<>                    ///{{{2///////////////////////////
template< class > 
struct is_catalog_key 
	: std::false_type 
{};

template< typename tType, typename tHandle, class tTag, ECatalogKeyKind tKind, CatalogKeyAutoconf const& tAutoConf  >
struct is_catalog_key< CatalogKey<tType,tHandle,tTag,tKind,tAutoConf> >
	: std::true_type
{};

//--    catalog_key_is_*<>                  ///{{{2///////////////////////////

template< class tKey >
struct catalog_key_is_value
	: std::integral_constant< bool, ECatalogKeyKind::value == tKey::kind >
{};

template< class tKey >
struct catalog_key_is_factory
	: std::integral_constant< bool, ECatalogKeyKind::factory == tKey::kind >
{};

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
