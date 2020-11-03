/*-******************************************************* -- HEADER -{{{1- */
/*-	Sys errors
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_ERRORS_HPP_87B174CC_9D0E_4389_B9FD_0CD54782A445
#define FLUX_SYS_ERRORS_HPP_87B174CC_9D0E_4389_B9FD_0CD54782A445

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/std/stdexcept.hpp>

//--    >>> namespace = flux::sys::error >>>        ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(sys,error)
//--    errors - generic                    ///{{{2///////////////////////////
struct SysError : flux::error::RuntimeError
{};

//--    errors - paths & files              ///{{{2///////////////////////////
/** Error: Generic file system related error
 */
struct FsError : SysError
{};

	struct FsNotSupported 
		: FsError
	{};

	/** Error: path type mismatch
	 *
	 * Error indicating that the specified file type mismatches the expected file
	 * type. For instance, attempting to recursive `dir_remove()` a file will 
	 * throw `PathTypeMismatch`.
	 */
	struct FsTypeMismatch 
		: FsError
	{};
	/** Error: unexpected segment in path
	 *
	 * Function encountered an unexpected segment in the current path. This is
	 * currently thrown by `dir_remove()` under Win32 if directory traversal ever
	 * encounters a reparse point (=symlink) that is also advertised as a
	 * directory. Under current windows versions this should never occur (as stated
	 * by MSDN), but the check is neverthless included as following malicious
	 * symlinks in recursive directory traversal can have fairly far reaching
	 * consequences.
	 */
	struct FsPathUnexpected 
		: FsError
	{};

	/** Error: permission denied
	 *
	 * Thrown when an operation cannot be completed because access to the file/path
	 * was denied due to lack of permissions.
	 */
	struct FsPermissionDenied
		: FsError
		//, virtual flux::error::SystemError< compat::errc, compat::errc::permission_denied > // TODO: make these just be lightweight tags?
	{};

/** Error: invalid request
 *
 * The code requested an invalid operation. Thrown by, for example,
 * `path_lexical_parent()` if the `aSteps` parameter is not a negative value.
 */
struct PathInvalidRequest : flux::error::LogicError
{};

//--    errors - file mapping               ///{{{2///////////////////////////
struct MapError
	: SysError
{};

	struct InvalidFileMap
		: MapError
	{};

	struct MapPrepareFailed
		: MapError
	{};
	struct MapResizeFailed
		: MapError
	{};

	struct MapOutOfRange
		: MapError
	{};
	struct MapBadAlignment
		: MapError
	{};

//--    <<< ~ flux::sys::error namespace <<<        ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(sys,error)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_ERRORS_HPP_87B174CC_9D0E_4389_B9FD_0CD54782A445
