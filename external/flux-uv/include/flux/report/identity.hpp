/*-******************************************************* -- HEADER -{{{1- */
/*- Identities & identification
 *
 * Identify the current execution environment. Generate names for the current
 * build (via build options), but also enumerate and assign identifiers to 
 * threads and such.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_IDENTITY_HPP_4C86D2C4_7166_4976_91E4_2623314F369A
#define FLUX_REPORT_IDENTITY_HPP_4C86D2C4_7166_4976_91E4_2623314F369A

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <cstddef>

//--    >>> namespace = flux::report >>>        ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(report)
//--    constants                       ///{{{2///////////////////////////////

extern char const* const kConfigFlag;

//--    functions                       ///{{{2///////////////////////////////

/** Get full identification string
 *
 * Get the full identification string. This contains the following information:
 *   
 *   - host name
 *   - process name (if set)
 *   - thread name 
 *   - configuration string (if non-empty)
 *
 * The identification string is automatically included in most outputs from
 * Flux.Report, either in the actual output, or in the file name (if the output
 * format doesn't support free-form data).
 */
char const* identity() noexcept;

/** Get process name
 *
 * Get process name, previously set with `set_process_name()`.
 *
 * \see set_process_name()
 */
char const* process_name() noexcept;
/** Get thread name
 *
 * Get thread name, previously set with `set_thread_name()`. If not set, the
 * thread name defaults to `tNNN`, where `NNN` is replaced by the thread's 
 * unique id.
 *
 * \see set_thread_name()
 */
char const* thread_name() noexcept;

/** Get config string/identifier
 *
 * Gets a string that concatenates all configuration values set through 
 * `set_config()`.
 *
 * \see set_config()
 */
char const* config_string() noexcept;

/** Specify process name
 *
 * Set the process name. Default is empty. Setting a process name is primarily
 * useful in situations with multiple concurrent processes (e.g. MPI).
 *
 * Setting the process name to NULL is treated internally as if it were set to
 * an empty string (`""`).
 */
void set_process_name( char const* ) noexcept;
/** Specify thread name
 *
 * Set thread name. Default is `tNNN`, where `NNN` is replaced by the thread's
 * unique id.
 *
 * Setting the thread name to NULL will reset the name to the default value.
 */
void set_thread_name( char const* ) noexcept;


/** Set configuration value
 *
 * Set configuration value \a aName to \a aValue. All key-value pairs are
 * concatenated into a single string to generate the configuration identifier
 * returned by `config_string()`.
 *
 * To remove a previously set configuration value, call `set_config()` with
 * \a aValue set to NULL for the \a aName that should be removed.
 *
 * The \a aValue argument can be set to the special value `kConfigFlag`, in
 * which case only the name \a aName will show up in the generated identifier.
 *
 * Debug builds of flux automatically inlude a `DEBUG` key-value pair that is
 * set to `kConfigFlag`. It is not possible change the value of the `DEBUG`
 * config, nor is it possible to remove it.
 */
void set_config( char const* aName, char const* aValue ) noexcept;


/** Get thread unique index
 *
 * Gets a per-thread index. Each thread for which profiling functions are used
 * receives an unique index that is increment for new such thread. The index
 * starts at zero for the first thread where Flux.Report functions are used.
 *
 * Unique indices are *not* reused, even after threads exit.
 *
 * \note Threads created during process shutdown will receive indices close to
 * the maximum value `std::size_t`.
 *
 * \see thread_active_index()
 */
std::size_t thread_unique_index() noexcept;
/** Get thread active index
 *
 * Gets a per-thread index. Each thread for which profiling functions are used
 * receives an active index. The active index may be reused after a thread
 * exits, so the larget active index corresponds to the largest number of
 * concurrently executing threads (from which Flux.Report functions were used).
 *
 * \note Threads created during process shutdown will receive `~std::size_t(0)`
 * as their active index.
 *
 * \see thread_unique_index()
 */
std::size_t thread_active_index() noexcept;

//--    <<< ~ flux::report namespace <<<        ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(report)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_IDENTITY_HPP_4C86D2C4_7166_4976_91E4_2623314F369A
