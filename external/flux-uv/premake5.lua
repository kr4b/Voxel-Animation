-- premake5: Define flux project
-- premake5: 
-- premake5: Note: this is not a standalone file. This file should be included
-- premake5: from another premake5 file, outside of any 'project()'s.

-- Paths
fluxBasePath = path.getabsolute( "." );

includedirs( "include" );

includedirs( "third_party/concurrentqueue/include" );
includedirs( "third_party/fmtlib/include" );
includedirs( "third_party/miniz/include" );
includedirs( "third_party/monocypher/include" );
includedirs( "third_party/pegtl/include" );
includedirs( "third_party/parallel-hashmap/include" );
includedirs( "third_party/zstd/include" );


-- Generate flux project
project( "flux-uv-preview" )
	local sources = {
		  "src/**.c"
		, "src/**.cpp"
		, "src/**.cxx"
		, "src/**.h*"
		, "src/**.inl"
		, "src/**.inc"
		
	};
	local headers = {
		  "include/flux*/**.h*"
		, "include/flux*/**.inl"
		, "include/flux*/**.inc"
	};
	
	kind "StaticLib"
	location "."

	files( sources );
	files( headers );

	
	filter "files:src/ext_misc_/**.cpp"
		optimize "on"
		flags "NoRuntimeChecks"
		flags "OmitDefaultLibrary"
	filter "*"

	--postbuildcommands {
	--	"{COPY} \"%{prj.location}/assets/flux-default-assets.zip\" \"%{cfg.targetdir}\""
	--}

--EOF
