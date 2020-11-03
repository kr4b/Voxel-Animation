/*-******************************************************* -- HEADER -{{{1- */
/*- Forward declarations
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FML_FORWARD_HPP_F54EF87A_FCDF_4DB2_96CB_76D6E22212A2
#define FLUX_FML_FORWARD_HPP_F54EF87A_FCDF_4DB2_96CB_76D6E22212A2

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <cstddef>

//--    >>> namespace = flux::fml >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(fml)
//--    forward                         ///{{{2///////////////////////////////

template< typename, class > struct Quaternion;
template< typename, class > struct DualQuaternion;

template< class, typename, class > struct SmallVector;
template< class, class, typename, class > struct SmallMatrix;


template< class > struct is_quaternion;
template< class > struct is_dual_quaternion;

template< class, std::size_t = 0 > struct is_small_vector;
template< class, std::size_t = 0, std::size_t = 0 > struct is_small_matrix;

//--    <<< ~ flux::fml namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(fml)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_FML_FORWARD_HPP_F54EF87A_FCDF_4DB2_96CB_76D6E22212A2
