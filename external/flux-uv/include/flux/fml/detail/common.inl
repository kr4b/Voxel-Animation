/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- common implementation
 */
/*-***************************************************************** -}}}1- */

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::fml >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
}

//--    space_compatible<>                  ///{{{2///////////////////////////
template< class, class > struct space_compatible
	: std::false_type
{};

template< class tSpace > 
struct space_compatible<tSpace,tSpace>
	: std::true_type 
{};
template< class tSpace > 
struct space_compatible<tSpace,space::AnySpace> 
	: std::true_type 
{};
template< class tSpace > 
struct space_compatible<space::AnySpace,tSpace> 
	: std::true_type
{};
template<> 
struct space_compatible<space::AnySpace,space::AnySpace>
	: std::true_type
{};

//--    space_specialized<>                 ///{{{2///////////////////////////
template< class tSpace > 
struct space_specialized<tSpace,tSpace>
{
	using type = tSpace;
};
template< class tSpace > 
struct space_specialized<tSpace,space::AnySpace> 
{
	using type = tSpace;
};
template< class tSpace > 
struct space_specialized<space::AnySpace,tSpace> 
{
	using type = tSpace;
};
template<> 
struct space_specialized<space::AnySpace,space::AnySpace>
{
	using type = space::AnySpace;
};

//--    <<< ~ flux::fml namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
