Simple Volume Demo
==================

## Outline

Source code: `src/*`
Shaders: `assets/*.{vert,frag}`
Data: `assets/`

Note: Start with `simple_vol.frag` - this is where the naive ray marching 
takes place. Look for the corresponding OpenGL code (only a few function
calls) in `main.cpp`.

## Notes on OpenGL

The demo uses slightly nonstandard OpenGL names. For, example, you will see

	GL::Uint
	GL::SRC_ALPHA
	gl->bufferData( ... )

instead of the more standard

	GLuint
	GL_SRC_ALPHA
	glBufferData( ... )

This is related to the OpenGL loader included in the "flux" toolbox. If you
migrate the code to a different loader (e.g. GLEW or GLAD), you will need to
convert to the more conventional style. (Note: flux is designed to minimally
interfere with those, so it's possible to use both flux and a standard OpenGL
loader at the same time, assuming of course that both are appropriately 
initialized.)

Note: the `gl->` part in the function calls is just convention; here `gl` is
just a `struct` that contains function pointers to the full OpenGL API (up to
some relatively recent version). Once initialized, a global instance of the
API is available via `gl::gl_api()`.

There are a few names that diverge from the standard naming, e.g. `GL::GLTRUE`
for `GL_TRUE`. This is to avoid collisions with other APIs, in this case the
Win32 API (which rudely defines TRUE as a macro).

## Building

Windows: Use the provided Visual Studio solution (SimpleVolume.sln)
Linux: Use the included make files (just run `make`).

These have been generated using Premake, see https://premake.github.io/
You can regenerate them by running premake again (if you have added additional
files to the `src` directory, it will pick those up as well).

Premake doesn't require installation, so just download the precompiled binary
from the premake website.

Commands
Visual Studio: premake5.exe vs2017
Makefiles: ./premake5 gmake2

## Usage

- Hold left mouse button to orbit camera
- Scroll wheel to move closer/further
- Escape to escape

%EOF vim:syntax=markdown:spell:
