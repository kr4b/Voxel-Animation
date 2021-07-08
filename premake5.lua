workspace "HPBVoxel"
    platforms { "x64" }
    configurations { "debug", "release" }

    startproject "simplevol"

    language "C++"
    cppdialect "C++17"

    flags "NoPCH"
    flags "MultiProcessorCompile"

    debugdir "%{wks.location}"
    objdir "_build_/%{cfg.buildcfg}-%{cfg.platform}-%{cfg.toolset}"
    targetsuffix "-%{cfg.buildcfg}-%{cfg.platform}-%{cfg.toolset}"
    
    -- Default toolset options
    filter "toolset:gcc or toolset:clang"
        linkoptions { "-pthread" }
        buildoptions { "-march=native", "-Wall", "-pthread" }

    filter "toolset:msc"
        defines { "_CRT_SECURE_NO_WARNINGS=1" }
        defines { "_SCL_SECURE_NO_WARNINGS=1" }
        buildoptions { "/utf-8" }
    
    filter "*"

    -- default libraries
    filter "system:linux"
        links "dl"
    
    filter "system:windows"

    filter "*"

    -- default outputs
    filter "kind:StaticLib"
        targetdir "lib/"
				libdirs "lib/%{cfg.buildcfg}"

    filter "kind:ConsoleApp"
        targetdir "bin/"
				libdirs "bin/%{cfg.buildcfg}"
        targetextension ".exe"
    
    filter "*"

    --configurations
    configuration "debug"
        symbols "On"
        defines { "_DEBUG=1" }

    configuration "release"
        optimize "On"
        defines { "NDEBUG=1" }

    configuration "*"

-- External projects
include "external/glew-cmake"
include "external/glfw"
include "external/glm"
include "external/imgui"

-- Projects
project "hpb-voxel"
    local sources = { 
        "src/**.hpp",
        "src/**.cpp",
        "src/miniz.c"
    }

    kind "ConsoleApp"
    location "src"

    files( sources )

    includedirs { "external/glew-cmake/include/", "external/glfw/include/", "external/glm/", "external/imgui" }

    links { "GLEW", "GLFW", "GLM", "ImGui" }

-- EOF