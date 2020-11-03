/*-******************************************************* -- HEADER -{{{1- */
/*-	Platform identification and basic capabilities
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SETUP_ENDIAN_HPP_BC368AD8_CED5_4D6A_A2C6_763AABC2E578
#define FLUX_SETUP_ENDIAN_HPP_BC368AD8_CED5_4D6A_A2C6_763AABC2E578

//--//////////////////////////////////////////////////////////////////////////
//--    Include                     ///{{{1///////////////////////////////////

#include "defaults.hpp"

#include "arch.hpp"
#include "platform.hpp"

//--    Endian                      ///{{{1///////////////////////////////////

#define FLUX_ENDIAN_UNKNOWN       0  //!< unknown/undetectable endian 
#define FLUX_ENDIAN_LITTLE        1  //!< little endian (x86 & co)
#define FLUX_ENDIAN_BIG           2  //!< big endian
#define FLUX_ENDIAN_DS9K          3  //!< wonky endian
#define FLUX_ENDIAN_PDP           FLUX_ENDIAN_DS9K

/** Platform endian
 *
 * Reflects the target platform's endian. The endian is detected automatically,
 * if the underlying standard libraries allow detection via pre-processor. If
 * not, the endian is set to `FLUX_ENDIAN_UNKNOWN`.
 *
 * The target platform's endian can be overridden via `FLUXCFG_ENDIAN`.
 *
 * \see FLUXCFG_ENDIAN
 */
#define FLUX_ENDIAN        FLUX_ENDIAN_UNKNOWN /*OVERRIDDEN_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/endian.hxx"
#endif // FLUX_SETUP_ENDIAN_HPP_BC368AD8_CED5_4D6A_A2C6_763AABC2E578
