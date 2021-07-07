project "GLEW"
	kind "StaticLib"
	language "C"
	architecture "x86_64"

	targetdir "../bin/%{cfg.buildcfg}"
	objdir "../_build_/%{cfg.buildcfg}-%{cfg.platform}-%{cfg.toolset}"
	
	includedirs { "glew-cmake/include/" }

	files
    {
        "glew-cmake/src/glew.c"
    }
    
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		defines
		{
			"_GLEW_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines 
		{ 
			"_GLEW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"