Third Party Software / Externals
================================

This file lists external dependencies of flux. The lists are split into
 - third party sources included in the flux source distribution
 - third party binaries distributed with the flux source distribution
 - third party libraries that are dynamically loaded
 - non-source code/executable data included in the flux source distribution
 - software that may be acquired independently of flux
 - software that is included in the flux dev-repo only (but not distributed)

Note: the flux supports partial deployments, which may not include all the
listed software. If you find a third party component listed in this file but
cannot locate its associated files, it was likely excluded from the partial
deployment. In a deployed version, the file `flux.${VERSION}.manifest` lists
all included modules and components.

## Third party source code

The following externals are included and distributed with the flux libraries.
Some contain modifications; if this is the case, these are briefly summarized
with each library, and are additionally marked in the source with a comment
containing `**flux-MODIFIED**`.

Most third party sources are included via a wrapper, typically located in
`flux_ext_misc_`. This wrapper may suppress warnings from the third party
sources, set desired compile time options. Additionally, implementations are
compiled from source code in `flux_ext_misc_` (see `src/` subdirectory). Select
third party source code is built with optimization enabled, even in debug mode
(can be disabled via `FLUXCFG_UTIL_NO_DEBUG_OPTIMIZE`).

### Vulkan Headers

- Where: `third_party/vulkan-headers`
- License: Apache v2.0
- Source: The LunarG VulkanSDK, https://www.lunarg.com/vulkan-sdk/
- What: Vulkan API headers

Only the standard Khronos Vulkan headers are included, but not the run-time
libraries, layers or similar. Auxiliary headers (e.g., Spir-V stuff) and for
creating layers etc are not included either.

### Vulkan memory allocator

- Where: flux-ext\_misc\_
- License: MIT (see LICENSE.vma)
- Source: https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
- What: Vulkan memory allocator

Modified to use the Vulkan header via `flux-dlapi_vk`.

Modified to support GCC 4.9: `VmaStlAllocator<>` requires a few more members,
including a number of typedefs (`pointer`,`const_pointer`,etc), the
`construct()` and `destroy()` methods, and the `rebind<>` template.

Modified to use `<flux_dlapi/os/win32.hpp>` instead of `<windows.h>`. This
avoids pulling in `<windows.h>` (although this could also have been achieved by
just disabling recording indiscriminately).

See `flux/vk/alloc.hpp`. This includes the Vulkan Memory Allocator and provides
a helper function to create a `VmaAllocator` using the dynamically loaded API.


### PEGTL

- Where: `third_party/pegtl`
- License: MIT
- Source: https://github.com/taocpp/PEGTL/
- What: PEGTL - The Parsing Expression Grammar Template Library

*Note:* Included via `flux_ext/pegtl.hpp` (see `flux-ext_misc_`). This sets up
custom namespace as per embedding instructions in the PEGTL documentation. This
namespace is is further exported as `flux_ext::pegtl`.

### fmtlib

- Where: `third_party/fmtlib`
- License: BSD 2-Clause
- Source: https://github.com/fmtlib/fmt
- What: C++ string formatting library

Note: Included via `flux_ext/fmtlib.hpp` (see `flux-ext_misc_`). Fmtlib is
further exposed via `<flux/std/format.hpp>`. Flux uses the `printf()`-syntax
version of fmtlib.

### zstd

- Where: `third_party/zstd`
- License: BSD
- Source: https://github.com/facebook/zstd
- What: "Fast real-time compression algorithm"

Included via `flux_ext/zstd.hpp` (see `flux-ext_misc_`) inside of flux. Custom
versions can be selected via the `FLUXCFG_EXT_ZSTD_HEADER` define. See [compile
time config](config.md) for additional information.

*Note:* The source is in the `include/zstd/lib` directory. The various
`unity-*.c` include relevant parts of the library, and are "built" via
`flux-ext_misc_` using the `zstd-{common,compress,decompress}.cpp` sources.

### miniz 

- Where: `third_party/miniz`
- License: MIT 
- Source: https://github.com/richgel999/miniz
- What: "Single C source file zlib-replacement library"

Included via `flux_ext/miniz.hpp` (see `flux-ext_misc_`) inside of flux. Custom
versions can be selected via the `FLUXCFG_EXT_MINIZ_HEADER` define. See
[compile time config](config.md) for additional information.

### moodycamel::ConcurrentQueue

- Where: `third_party/concurrentqueue`
- License: Simplified BSD or Boost
- Source: https://github.com/cameron314/concurrentqueue
- What: Multi-producer, multi-consumer concurrent queue

Only includes the queue implementations, but not the tests. The queues are
exposed as `flux_ext::ConcurrentQueue` and `flux_ext::BlockingConcurrentQueue`,
via the `flux_ext/concurrentqueue.hpp` and
`flux_ext/blockingconcurrentqueue.hpp` headers.

### Parallel Hashmaps

- Where: `third_party/parallel_hashmap`
- License: Apache v2.0
- Source: https://github.com/greg7mdp/parallel-hashmap
- What: "A set of excellent hash map implementations"

The parallel hashmap types (`flat_hash_set`, `flat_hash_map`, `node_hash_set`,
`node_hash_map`, `parallel_flat_hash_set`, `parallel_flat_hash_map`,
`parallel_node_hash_set`, `parallel_node_hash_map`) are used via
`flux_ext/phmap.hpp` header. They may be forward declared with
`flux_ext/phmap_fwd_decl.hpp`. In either case, they are also exposed in the
`flux_ext` namespace (so, e.g., `flux_ext::flat_hash_map`).

### Duktape

- Where: `third_party/duktape`
- License: MIT
- Source: https://duktape.org/
- What: An embeddable Javascript engine

*Note:* Included via `flux_ext/duktape.hpp` (see `flux-ext_misc_`) inside of
flux. This sets up a few additional platform-specific configuration options 
for duktape using the flux configuration.

*Note:* The `duktape.c` source is in the `include/duktape` directory. It is
included by the `duktape.cpp` source in `flux-ext_misc_`.

### stb single header libraries 

- Where: `third_party/stb`
- License: Public Domain / MIT
- Source: https://github.com/nothings/stb 
- What: "stb single-file public domain libraries for C/C++"

Included via `flux_ext/stb_{NAME}.hpp` (see `flux-ext_misc_`) inside of flux.
Custom versions can be selected via the `FLUXCFG_EXT_STB_{NAME}_HEADER` /
`FLUXCFG_EXT_STB_{NAME}_NO_IMPLEMENTATION` definitions. See config.md for
additional information.

### fontstash

- Where: `third_party/fontstash`
- License: Zlib
- Source: https://github.com/memononen/fontstash
- What: "Light-weight online font texture atlas builder"

Included via `flux_ext/fontstash.hpp` (see `flux-ext_misc_`) inside of flux.
Custom versions can be selected via the `FLUXCFG_EXT_FONTSTASH_HEADER` /
`FLUXCFG_EXT_FONTSTASH_NO_IMPLEMENTATION` definitions. See config.md for
additional information.

### monocypher

- Where: `third_party/monocypher`
- License: BSD
- Source: https://monocypher.org
- What: "Boring crypto that simply works."

Included via `flux_ext/monocypher.hpp` (see `flux-ext_misc_`) inside of flux.
The include is wrapped in an `extern "C" {}` block, since the header doesn't
do this automatically.

### span-lite

- Where: flux-ext\_misc\_
- License: MIT (see LICENSE.span)
- Source: https://github.com/martinmoene/span-lite
- What: `std::span<>` implementation

Modified to always define macros before inspecting them via `#%if` and similar
(a common pattern in span-lite). Flux compiles by default with `-Wundef`, which
results in a fair amount of warning-spam. Span-lite tries to disable this via
appropriate pragmas, but various versions of GCC ignore specifically `#%pragma
GCC diagnostic ignored "-Wundef"`.

Original tests are included in `flux-ext_misc_/tests/` and have been modified 
to use Catch.

Exposed via `compat::span` in `flux-compat/`.

### string-view-lite

- Where: flux-ext\_misc\_
- License: MIT (see LICENSE.string\_view)
- Source: https://github.com/martinmoene/string-view-lite
- What: `std::string_view` implementation

Modified to always define macros before inspecting them via `#%if` and similar
(see comments for span-lite).

Original tests are included in `flux-ext_misc_/tests/` and have been modified 
to use Catch.

Exposed via `compat::string_view` in flux-compat/.

### invoke-lite

- Where: flux-ext\_misc\_
- License: MIT (see LICENSE.invoke)
- Source: https://github.com/martinmoene/invoke-lite
- What: `std::invoke` implementation

Modified to use `FLUX_STDFEAT_*` for consistency with feature detection in
flux.. Also exports `invoke_result` (C++17 and C++11).

Exposed via `compat::invoke` & `compat::invoke_result` in flux-compat/.

### variant-lite

- Where: flux-ext\_misc\_
- License: Boost (see LICENSE.variant)
- Source: https://github.com/martinmoene/variant-lite
- What: `std::variant` implementation

Modified to always define macros before inspecting them via `#%if` and similar
(see comments for span-lite).

Original tests are included in `flux-ext_misc_/tests/` and have been modified
to use Catch.

Exposed via `compat::variant` et al. in flux-compat/.

### `shared_mutex`

- Where: flux-ext\_misc\_
- License: Boost (see `shared_mutex` header)
- Source: http://howardhinnant.github.io/shared_mutex{,.cpp}
- What: `std::shared_mutex` implementation

C++11 compatible `shared_mutex` implementation by Howard Hinnant. Modified to
comment the parts that it tries to place in the `std::` namespace (but only for
libc++?). This probably makes the implementation incomplete w.r.t. upgrading
shared locks.

Exposed via `compat::shared_mutex` et al. in flux-compat`.

### `type_safe`

- Where: flux-ext\_misc\_
- License: MIT (see LICENSE.type\_safe)
- Source: https://github.com/foonathan/type_safe
- What: "Zero overhead utilities for preventing bugs at compile time"

Currently, flux only includes the `strong_typedef.hpp` and related tests
(`type_safe_-_strong_typedef.cpp`). These have further been modified to use the
flux libraries (such as flux-setup for feature detection).

### `cxx_function`

- Where: flux-ext\_misc\_
- License: MIT (see LICENSE.cxx\_function)
- Source: https://github.com/potswa/cxx_function
- What: Replacement for `std::function`

Replacement for `std::function` with additional capabilities, notably including
being `noexcept` movable, `unique_function` and a few others.

Modified to eliminate warnings about undefined preprocessor on GCC and to
otherwise use flux.setup where necessary. Renamed internal macros to avoid
collision with e.g. `windows.h`.

Also exposed as `flux::function` and `flux::unique_function`.

### loguru

- Where: flux-ext\_misc\_
- License: Public domain (see license statement in README.loguru.md)
- Source: https://github.com/emilk/loguru
- What: Logging helpers and facilities

Exposed via flux.std (see `<flux/std/log.hpp>`) and via a custom set of macros.
Compiled into flux-ext\_misc\_ (see the readme for details).

Slightly modified to reduce the message width when logging to stderr. (In
short, discard everything in the default preamble up to the thread name. The
date and runtime is not very interesting in the intended use case ... and it
will still be available in the log files.)

Configured to use fmtlib (see below), but is modified to use the printf-like
API of fmtlib. Also uses the new version of fmtlib.

### crc32 

- Where: flux-ext\_misc\_
- License: ZLib
- Source: https://github.com/stbrumme/crc32
- What: CRC32 computations

Major modifications: Only exposes a modified "slicing-by-4" implementation and
the associated tables. The modified version ensures that the data is properly
aligned for 32-bit words before switching to this (x86-based CPUs can deal with
the misalignment, but it's still against the C++ standard).


## Third party binaries

### shaderc / glslc

- Where: `third_party/shaderc`
- License: Apache v2.0
- Source: https://github.com/google/shaderc
- What: (prebuilt) GLSL compiler

This includes the `glslc` GLSL compiler binary for a number of platforms. The
binaries have been compressed with UPX to reduce their size and make
distribution a bit less painful (the compression introduces an overhead to each
invocation of the compiler, so projects with very many shaders should opt for
uncompressed binaries).


## Dynamically loaded third party libraries

Flux dynamically loads several APIs, including OpenGL, Vulkan and platform
specific APIs (e.g. a small very subset of the Win32 API, parts of X11, GLX and
similar). These are assumed to be provided by the host system.

Flux can additionally use the following third party libraries via dynamic
loading of prebuilt binary DLLs or SOs:

### GLFW

- License: ZLib/Libpng
- Source: https://www.glfw.org/
- What: "Library for OpenGL, OpenGL ES and Vulkan development on the desktop"
- Dynamic loader in `flux-dlapi_os`

The dynamic loader attempts to load the GLFW dll/so and retrieves functions to
a subset of the GLFW API. See `flux/dlapi/os/glew.hpp`.


## Non-source code included with Flux

### Droid Sans

- Where: flux-toolkit/assets-source/droid-sans
- License: Apache v2.0
- Source: https://www.fontsquirrel.com/fonts/droid-sans
- What: "Droid Sans" TTF font files

Some of these files are also included in the `flux_default_assets.zip`.

### Droid Sans Mono (Dotted)

- Where: flux-toolkit/assets-source/droid-sans-mono
- License: Apache v2.0
- Source: http://www.cosmix.org/software/
- What: "Droid Sans Mono (w/ dotted zero)" TTF font file

This is a slightly modified version of the Droid Sans Mono font included in the
Android project, where the zero is dotted (to make it easier to distinguish
from an upper-case 'o').

Some of these files are also included in the `flux_default_assets.zip`.

### Droid Serif

- Where: flux-toolkit/assets-source/droid-serif
- License: Apache v2.0
- Source: https://www.fontsquirrel.com/fonts/droid-serif
- What: "Droid Serif" TTF font files

Note: A subset of Droid Sans is created and included in data of the
`minimal-fonts.inc` source file.

Some of these files are also included in the `flux_default_assets.zip`.

### Noto Emoji

- Where: flux-toolkit/assets-source/noto-emoji
- License: SIL Open Font License 1.1
- Source: https://www.google.com/get/noto/
- What: "Noto Emoji" TTF font file

Note: A subset of Noto Emoji is created and included in data of the
`minimal-fonts.inc` source file.

Some of these files are also included in the `flux_default_assets.zip`.


## Not included with Flux

(nothing at the moment)

## In the Flux Dev Repository

### Vulkan XML Specification, `vk.xml`

- Where: `third_party/vulkan-spec`
- License: Apache v2.0
- Source: https://github.com/KhronosGroup/Vulkan-Docs/
- What: Vulkan API XML Specification

Only the `vk.xml` is included. Used by `flux-dlapi_vk` to generate the Vulkan
API loader. The `notes.md` in `flux-dlapi_vk` includes some additional notes.

### glLoadGen

- Where: flux-dlapi\_gl
- License: MIT
- Source: https://bitbucket.org/alfonse/glloadgen/wiki/Home
- What: GL Loader Generator

**Heavily modified** Only uses the files `glspecs/*.lua` from the original code
base; the actual generator is completely custom (and not as pretty as the
original).

See documents in flux-dlapi\_gl for additional information. In particular, the
Notes.md explains what's going on.

### Catch2

- Where: `third_party/catch`
- License: Boost Software License (v1.0)
- Source: https://github.com/catchorg/Catch2
- What: C++ unit testing framework.

Note: Only includes the single-header version.

### TinyXML

- Where: `flux_dlapi_vk/utility/genAPI/tinyxml2*`
- License: zlib
- Source: https://github.com/leethomason/tinyxml2
- What: TinyXML2 -- a tiny stand-alone XML parser

Only used to generate the Vulkan API loader code from the Vulkan XML
specification.

### Valgrind Client Headers

- Where: `third_party/valgrind`
- License: BSD
- Source: http://valgrind.org
- What: Client interaction with host valgrind process

Note: This **only** includes the `valgrind.h` client header (which is released
under a BSD license), and not the valgrind main application (which is released
under the GPL).

The `valgrind.h` client header enables a client process to detect if it's
running under valgrind (among other things). This is currently used to disable
some tests that would otherwise take an unreasonably long time to execute (and
since they rely on timing, they would be useless anyway).

#%EOF vim:syntax=markdown:
