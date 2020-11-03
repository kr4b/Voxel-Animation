Environment Variables
=====================

Flux looks at some environment variables to determine some of its run-time 
configuration. The variables are listed as follows.

**NOTE:** The settings are case *sensitive*!


## flux-specific

### flux.std : Process Random Value

`FLUXENV_PROCESS_RANDOM` can be used to override the per-process random
value that is normally generated on process startup. See `#flux::kProcessRandom`
for additional information (`flux-std/process_random.hpp`).

`FLUXENV_LOG_CONSUMERS` can be used to select the consumers (outputs) for the
default logger. The environment value takes a semicolon seprated list of the
following names:
  - `native` : default native logger, that outputs to `stderr`


### flux.sys : User Home and Desktop Directory

`FLUXENV_SYS_USER_HOME` can be used to override the path returned by
`flux::sys::path_get_userdir( EUserPath::home )`. The path specified via the
environment variable will be returned instead. `FLUXENV_SYS_USER_HOME` *does
not* affect other derived paths (e.g., `EUserPath::config`), but is merely a
way to protect against (future) developers who may consider a user's home
directory to be an acceptable place for automatically generated files. (It is
not.)

`FLUXENV_SYS_USER_DESKTOP` can similarly be used to override the path returned
by `flux::sys::path_get_userdir( EUserPath::desktop )`.

<b>Note:</b> currently, if set `get_path_userdir()` will return the contents of
these environment variables unconditionally, i.e., regardless of setting of the
`EPathOptions` argument. This may change in future iterations.

### flux.io : Assets Directory

If set, the directory specified by `FLUXENV_ASSET_DIR` is added to the list of
assets directories (i.e., the directories accessed via the `/@flux/opt/assets/`
VFS path). This directory has the highest priority of all default directories.
If the specified path is not an accessible directory, it will be ignored.

### flux.io : System Directory

If set, the directory specified by `FLUXENV_SYSTEM_DIR` is added to the list of
assets directories (i.e., the directories accessed via the `/@flux/opt/system/`
VFS path). This directory has the highest priority of all default directories.
If the specified path is not an accessible directory, it will be ignored.

### flux_dlapi.vk : Vulkan SO / DLL

`FLUXENV_VULKAN_SO` can be used to override the default name of the Vulkan
shared object / DLL.


## General

### `${HOME}`

`flux-sys` respects the `${HOME}` environment variable when returning user
paths (see `#flux::sys::path_get_userdir()`).

### `${TMPDIR}`, `${TMP}`, `${TEMP}`, `${TEMPDIR}`

`flux-sys` checks the `${TMPDIR}`, `${TMP}`, `${TEMP}`, and `${TEMPDIR}`
environment variables in the listed order when determining the system's
temporary directory on non-WIN32 platforms (WIN32 uses `GetTempPathW()`, which
internally respects one or more environemtn variables -- see MSDN). See
`#flux::sys::path_get_tempdir()`.

### flux.sys : Paths

On non-WIN32 platforms, flux.sys respects the following environment variables
when determning user-directories (`#flux::sys::path_get_userdir()`):
 
 - `XDG_DATA_HOME`:  `#flux::sys::EUserPath::data` and `#flux::sys::EUserPath::saves`
 - `XDG_CACHE_HOME`:  `#flux::sys::EUserPath::cache`
 - `XDG_CONFIG_HOME`:  `#flux::sys::EUserPath::config`

If undefined, default paths will be constructed based on `${HOME}`.

#%EOF vim:syntax=markdown:
