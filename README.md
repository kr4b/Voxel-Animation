HPB Voxel Spline Animation Demo
==================

## Outline

Source code: `src/`

Shaders: `assets/*.{vert,frag}`

Data (not supplied): `assets/`

2d version: `2d_debug/`

External libraries: `external/`

The entire rendering algorithm is contained within `assets/spline_map.frag`, where the uniforms are supplied from `src/scene/spline_map_scene.hpp`.
Debug code for most of the rendering algorithm is available in `src`.

## Building

To build the project, first run Premake, see https://premake.github.io/.
Premake doesn't require installation, so just download the precompiled binary
from the premake website.

Commands:
- Visual Studio 2017: premake5.exe vs2017
- Makefiles: ./premake5 gmake2

Once the build files have been generated you can, you build the project.

- Windows: Use the provided Visual Studio solution (SimpleVolume.sln)
- Linux: Use the included make files (just run `make`).

## Usage

- Hold left mouse button to orbit camera
- Scroll wheel to move closer/further
- Escape to escape
- TAB to enter debug mode
- Space to emit debug rays 
- Backspace to return to the camera transformation from where the debug rays were emitted
