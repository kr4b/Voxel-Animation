Compile-Time Config
===================

*flux* respects a number of `#``define`s that can be used to control some
implementation aspects.

<b>TODO:</b> Incomplete, in particular flux-setup!

Flags are generally enabled if defined to a non-zero value and disabled if they
evaluate to zero.

## flux configuration

### Module selection

#### `FLUXCFG_WITH_FLUXIO`

Build with/without flux-io (`flux::io`). When defined, `FLUXCFG_WITH_FLUXIO`
sets the value of `FLUX_WITH_FLUXIO`, which should be used for checks. The
default (`FLUXCFG_WITH_FLUXIO` is left undefined) is that `FLUX_WITH_FLUXIO`
is enabled.

#### `FLUXCFG_WITH_FLUXGL`

Build with/without flux-gl (`flux::gl`). When defined, `FLUXCFG_WITH_FLUXGL`
sets the value of `FLUX_WITH_FLUXGL`, which should be used for checks. The
default (`FLUXCFG_WITH_FLUXGL` is left undefined) is that `FLUX_WITH_FLUXGL`
is enabled.

Note: Disabling `FLUXCFG_WITH_FLUXGL` furthermore removes internal dependencies
on `flux-dlapi_gl`.

#### `FLUXCFG_WITH_FLUXVK`

Build with/without flux-vk (`flux::vk`). When defined, `FLUXCFG_WITH_FLUXVK`
sets the value of `FLUX_WITH_FLUXVK`, which should be used for checks. The
default (`FLUXCFG_WITH_FLUXVK` is left undefined) is that `FLUX_WITH_FLUXVK`
is enabled.

Note: Disabling `FLUXCFG_WITH_FLUXVK` furthermore removes internal dependencies
on `flux-dlapi_vk`.

### flux-vk

#### `FLUXCFG_VK_WITH_BETA_EXTENSIONS`

If `#FLUXCFG_VK_WITH_BETA_EXTENSIONS` is enabled, the Vulkan beta extension
header is included by default. Further, features from beta extensions can
be selected during device setup.  Beta extensions are enabled by default;
define `FLUXCFG_VK_WITH_BETA_EXTENSIONS=0` to disable them.

`FLUXCFG_VK_WITH_BETA_EXTENSIONS` is required for `VK_KHR_ray_tracing` support.

Note: this also affects `flux-dlapi_vk`.

#### `FLUXCFG_VK_WITH_FLUXIO`

If `#FLUXCFG_VK_WITH_FLUXIO` is enabled, additional functions for loading data
from the flux-io VFS become available. For example, when setting up a graphics
pipeline, Spir-V shader modules can be loaded directly using the VFS.

### flux-util

#### `FLUXCFG_UTIL_IGNORE_LIKELY`

If `#FLUXCFG_UTIL_IGNORE_LIKELY` is enabled, the macros `#FLUX_HINT_LIKELY`,
`#FLUX_HINT_UNLIKELY` and `#FLUX_HINT_UNPREDICTABLE` will be ignored. The flag
is disabled by default.

#### `FLUXCFG_UTIL_NO_DEBUG_OPTIMIZE`

if `#FLUXCFG_UTIL_NO_DEBUG_OPTIMIZE` is enabled, the macros
`#FLUX_DEBUG_OPTIMIZE_BEGIN()` and `#FLUX_DEBUG_OPTIMIZE_END()` will be
ignored. The macros are used to enable optimizations in third party libraries
during debug builds; if `NO_DEBUG_OPTIMIZE` is specified, all code is built
with the default debug settings.

*Note:* Some code in `flux-ext_misc_` is built with optimization flags
specified during build time. This is unaffected by this configuration setting
and must be changed in the respective build scripts.

### flux-ftl

#### `FLUXCFG_FTL_BITWISE`

`#FLUXCFG_FTL_BITWISE` controls the `is_bitwise_*<>` macros. It may take one
of three values:
 - 0 (`FLUXCFG_FTL_BITWISE_USER`): respect user-specified tags
 - 1 (`FLUXCFG_FTL_BITWISE_TRIVIAL`): use default values based of the 
   `is_trivially_*<>` traits, but ignore the user-specified tags.
 - 2 (`FLUXCFG_FTL_BITWISE_NEVER`): `is_bitwise_*<>` always evaluate to 
  `std::false_type`.

#### `FLUXCFG_FTL_REALLOC_ALLOC`

`#FLUXCFG_FTL_REALLOC_ALLOC` controls whether the `::%realloc()`-augmented
allocator is used by e.g. `ftl::Vector<>`. When enabled, `ftl::Vector<>` may
use `::%realloc()` to grow its buffer, if this is deemed possible. This in turn
depends on the `is_bitwise_*<>` traits.


### flux-report

#### `FLUXCFG_REPORT_BADNESS`

`#FLUXCFG_REPORT_BADNESS` controls whether or not badness reporting takes place
by enabling/disabling the `FLUX_REPORT_*BADNESS*` macros. Badness reporting is
enabled by default.

#### `FLUXCFG_REPORT_INSTRUMENT`

`#FLUXCFG_REPORT_INSTRUMENT` controls whether or not instrumentation takes
place by enabling/disabling the `FLUX_*INSTRUMENT*` macros. Instrumentation is
enabled by default.

### flux-dlapi_os

#### `FLUXCFG_DLAPI_LOAD_VFSSYSTEM`

Controls whether the extended searches for dynamically loaded APIs is allowed
to consider paths in `/@flux/opt/system`. If enabled (the default), the paths
may be considered, if selected via per-item search flags. If disabled, the
paths will never be considered.

#### `FLUXCFG_DLAPI_LOAD_AUTOFETCH`

Controls whether the extended search for dynamically loaded APIs is allowed to
perform autofetching. If enabled (the default), autofetching may be performed,
if selected via the per-item search flags. If disabled, autofetching is never
performed.

#### `FLUXCFG_DLAPI_AUTOFETCH_TIMEOUT`

Specifies a timeout in milliseconds for the maximum duration of an autofetching
operation. If autofetching does not complete successfully in the given time
frame, the attempt is aborted. The default value is 3000 milliseconds.


## Externals

For information on the various external/third party libraries and components,
see [list of third party software and items](third_party.md).

Note: some externals can be excluded via `FLUXCFG_EXT_WITH_${FOO}` toggles.
This may require other configuration changes, and is in some cases only useful
for partial deployments -- toggling these without additional changes may result
in build failures.

### Vulkan Headers

#### `FLUXCFG_VULKAN_HEADER`

If defined, `flux-dlapi_vk` will include `FLUXCFG_VULKAN_HEADER` instead of
`<vulkan/vulkan.h>`, which should default to the bundled Vulkan headers in
`third_party/vulkan-headers`.

#### `FLUXCFG_VULKAN_BETA_HEADER`

If defined, `flux-dlapi_vk` will include `FLUXCFG_VULKAN_BETA_HEADER` instead
of `<vulkan/vulkan_beta.h>`, which should default to the bundled Vulkan headers
in `third_party/vulkan-headers`.

#### `FLUXCFG_VULKAN_PLATFORM_HEADER_PREFIX`

If defined, `flux-dlapi_vk` will include platform headers (e.g.`vulkan_xlib.h`)
as `<FLUXCFG_VULKAN_PLATFORM_HEADER_PREFIX${platform}.h>`.

### Vulkan Memory Allocator

#### `FLUXCFG_EXT_WITH_VKMEMALLOC`

Enables/disables inclusion of Vulkan memory allocator. Enabled if Vulkan 
support (`FLUXCFG_WITH_VULKAN`) is enabled.

#### `FLUXCFG_EXT_VKMEMALLOC_HEADER`

If defined, include `FLUXCFG_EXT_VKMEMALLOC_HEADER` instead of the bundled
`vk_mem_alloc.h`. Using a custom header very likely requires enabling
`FLUXCFG_EXT_VKMEMALLOC_NO_IMPLEMENTATION` and manual building of and linking
with the corresponding Vulkan memory allocator implementation.

#### `FLUXCFG_EXT_VKMEMALLOC_NO_IMPLEMENTATION`

If defined and enabled, do not build the bundled Vulkan memory allocator
implementation, and instead rely on an implementation provided and linked-in by
the end-user.

### Miniz library

#### `FLUXCFG_EXT_WITH_MINIZ`

Enables/disables use of miniz for zlib compression, .zip archive reading and
a few other things. Enabled by default.

#### `FLUXCFG_EXT_MINIZ_HEADER`

If defined, include `FLUXCFG_EXT_MINIZ_HEADER` instead of the bundled `miniz.h`
from `third_party`. Using a custom header very likely requires enabling
`FLUXCFG_EXT_MINIZ_NO_IMPLEMENTATION` and manual building of and linking with
the corresponding miniz implementation.

#### `FLUXCFG_EXT_MINIZ_NO_IMPLEMENTATION`

If defined and enabled, do not build the bundled miniz implementation, and
instead rely on an implementation provided and linked-in by the end-user.

### ZStd Compression

#### `FLUXCFG_EXT_WITH_ZSTD`

Enables/disables use of ZStd compression. Enabled by default.

#### `FLUXCFG_EXT_ZSTD_HEADER`

If defined, include `FLUXCFG_EXT_ZSTD_HEADER` instead of the bundled `zstd.h`
from `third_party`. Using a custom header very likely requires enabling
`FLUXCFG_EXT_ZSTD_NO_IMPLEMENTATION` and manual building of and linking with
the corresponding Zstd implementation.

#### `FLUXCFG_EXT_ZSTD_NO_IMPLEMENTATION`

If defined and enabled, do not build the bundled Zstd implementation, and
instead rely on an implementation provided and linked-in by the end-user.

### Monocypher

#### `FLUXCFG_EXT_WITH_MONOCYPHER`

Enables/disables use of monocypher crypto library. Enabled by default.

#### `FLUXCFG_EXT_MONOCYPHER_HEADER`

If defined, include `FLUXCFG_EXT_MONOCYPHER_HEADER` instead of the bundled
`monocypher.h` from `third_party`. Using a custom header very likely requires
enabling `FLUXCFG_EXT_MONOCYPHER_NO_IMPLEMENTATION` and manual building of and
linking with the corresponding monocypher implementation.

#### `FLUXCFG_EXT_MONOCYPHER_NO_IMPLEMENTATION`

If defined and enabled, do not build the bundled monocypher implementation, and
instead rely on an implementation provided and linked-in by the end-user.

### PEGTL

#### `FLUXCFG_EXT_WITH_PEGTL`

Enables/disables use of PEGTL. Enabled by default. Note: since flux defined a
custom PEGTL namespace (by defining `TAO_PEGTL_NAMESPACE`, as per PEGTL
documentation), multiple instances of PEGTL (possibly with different versions)
should not clash.

### Duktape JS VM

#### `FLUXCFG_EXT_DUKTAPE_HEADER`

If defined, include `FLUXCFG_EXT_DUKTAPE_HEADER` instead of the bundled
`duktape.h` from `third_party`. Should likely be matched with a
`FLUXCFG_EXT_DUKTAPE_SOURCE`.

#### `FLUXCFG_EXT_DUKTAPE_SOURCE`

If defined, compile `FLUXCFG_EXT_DUKTAPE_SOURCE` instead of the bundled
`duktape.c` from `third_party`. If defined to `0`, no `duktape.c` source will
be built, and instead relies on an implementation linked in by the end-user.

### STB libraries

#### `FLUXCFG_EXT_STB_IMAGE_HEADER`

If defined, include `FLUXCFG_EXT_STB_IMAGE_HEADER` instead of the bundled
`stb_image.h` from `third_party`. May be combined with
`FLUXCFG_EXT_STB_IMAGE_NO_IMPLEMENTATION`.

#### `FLUXCFG_EXT_STB_IMAGE_NO_IMPLEMENTATION`

If defined and enabled, do not build the `stb_image.h` implementation (by
defining `STB_IMAGE_IMPLEMENTATION`), and instead rely on the end-user linking
against an appropriate implementation. If not defined, `flux-ext_misc_` will
include the `stb_image.h` implementation from the selected header (see
`FLUX_EXT_STB_IMAGE_HEADER`).

#### `FLUXCFG_EXT_STB_IMAGE_WRITE_HEADER`

If defined, include `FLUXCFG_EXT_STB_IMAGE_WRITE_HEADER` instead of the bundled
`stb_image_write.h` from `third_party`. May be combined with
`FLUXCFG_EXT_STB_IMAGE_WRITE_NO_IMPLEMENTATION`.

#### `FLUXCFG_EXT_STB_IMAGE_WRITE_NO_IMPLEMENTATION`

If defined and enabled, do not build the `stb_image_write.h` implementation (by
defining `STB_IMAGE_WRITE_IMPLEMENTATION`), and instead rely on the end-user
linking against an appropriate implementation. If not defined, `flux-ext_misc_`
will include the `stb_image_write.h` implementation from the selected header
(see `FLUX_EXT_STB_IMAGE_WRITE_HEADER`).

#### `FLUXCFG_EXT_STB_TRUETYPE_HEADER`

If defined, include `FLUXCFG_EXT_STB_TRUETYPE_HEADER` instead of the bundled
`stb_truetype.h` from `third_party`. May be combined with
`FLUXCFG_EXT_STB_TRUETYPE_NO_IMPLEMENTATION`.

#### `FLUXCFG_EXT_STB_TRUETYPE_NO_IMPLEMENTATION`

If defined and enabled, do not build the `stb_truetype.h` implementation (by
defining `STB_TRUETYPE_IMPLEMENTATION`), and instead rely on the end-user
linking against an appropriate implementation. If not defined, `flux-ext_misc_`
will include the `stb_truetype.h` implementation from the selected header (see
`FLUX_EXT_STB_TURETYPE_HEADER`).

### Fontstash

#### `FLUXCFG_EXT_FONTSTASH_HEADER`

If defined, include `FLUXCFG_EXT_FONTSTASH_HEADER` instead of the bundled
`fontstash.h` from `third_party`. May be combined with
`FLUXCFG_EXT_FONTSTASH_NO_IMPLEMENTATION`.

#### `FLUXCFG_EXT_FONTSTASH_NO_IMPLEMENTATION`

If defined and enabled, do not build the `fontstash.h` implementation (by
defining `FONTSTASH_IMPLEMENTATION`), and instead rely on the end-user linking
against an appropriate implementation. If not defined, `flux-ext_misc_` will
include the `fontstash.h` implementation from the selected header (see
`FLUX_EXT_FONTSTASH_HEADER`).

#%EOF vim:syntax=markdown:
