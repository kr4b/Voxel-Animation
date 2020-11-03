/*-******************************************************* -- HEADER -{{{1- */
/*-	Application info
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_APPINFO_HPP_915C3C46_B5F3_41EC_9EF7_121887B0B286
#define FLUX_STD_APPINFO_HPP_915C3C46_B5F3_41EC_9EF7_121887B0B286

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>

//--    >>> namespace = flux >>>                    ///{{{1///////////////////
FLUX_NAMESPACE_ENTER()
//--    functions                           ///{{{2///////////////////////////

/** Get human-readable application name
 *
 * Gets a human-readable application name, previously set by `set_appinfo()`.
 * If not set, it defaults to "Unnamed flux-${VERSION_NAME} Project".
 *
 * \see appinfo_ident(), set_appinfo()
 */
char const* appinfo_name() noexcept;
/** Get application identifier
 *
 * Gets application identifier, previously set by `set_appinfo()`.  If not set,
 * it defaults to "flux-${VERSION_NAME}-default".
 *
 * \see appinfo_ident(), set_appinfo()
 */
char const* appinfo_ident() noexcept;

/** Set application info
 *
 * Set the application information. 
 *   - \a aName is a human-readable name, and is only ever used in this
 *     context (which, thus far, means "never").
 *   - \a aIdent is an identification string. It should be usable (at the
 *     minimum) as a file or directory name.
 *
 * The following limitations apply:
 *   - \a aName: length <= 127 characters
 *   - \a aName: length <= 63 characters
 *
 * \see appinfo_name(), appinfo_ident()
 */
void set_appinfo( char const* aName, char const* aIdent );

//--    <<< ~ flux namespace <<<                    ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE()
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_APPINFO_HPP_915C3C46_B5F3_41EC_9EF7_121887B0B286
