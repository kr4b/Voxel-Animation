*flux* is library to help with the development of stand-alone cross-platform
programs, mainly utilizing OpenGL or Vulkan. It aims at being easy to deploy,
small enough to include in source distributions, and to be self-contained.

## Summary

*flux* aims to provide
 - a "normalized" environment across compilers/operating system
 - minimal infrastructure (e.g., API loaders for OpenGL, Vulkan)
 - minimal abstractions for IO, locating and loading resources

(*flux* additionally includes some other things that have crept in over time.)

*flux* is C++11 compatible, but optionally utilizes select features from newer
standards. It has been tested with GCC 4.9.x and later, Visual Studio 2015† and
newer, and Clang 6 and newer.

† Visual Studio 2013 may be supported with certain configurations.

*flux* includes (optionally) all of its third party dependencies. See [list of
third party software](third_party.md) for a list of (potentially) included
third party components. Necessary third party components can be linked in
externally, see [compile time configuration options](config.md).

Note: this readme is used for deployments of *flux*. *flux* supports partial
deployments, i.e., this copy of *flux* may not include all of *flux*'s
functionality (however, such functionality may be mentioned in documentation).

## Usage

The recommended usage is to build *flux* as a static library component from
source with each project. Deployments of *flux* include a
[premake5](https://premake.github.io/) that can be included from client
projects. (See the included `example-project.lua` for an example client premake
project file.)

Building a project based on the example is a two step process:
 
 1. Run premake (e.g. `premake5.exe vs2017` or `premake5 gmake2`)
 2. Use native tools (open in Visual Studio, hit build; or `make ...`)

## License

*flux* is licensed under the MIT license. See the included LICENSE.txt file.

Included third party components have their own licenses. See [list of third
party software](third_party.md) for additional information. 

%EOF vim:syntax=markdown:spell:
