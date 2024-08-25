workspace "Engine"
	configurations { "Debug", "Release" }
	location "generated"

project "Runtime"
	kind "ConsoleApp"
	language "C++"
	architecture "x64"

	objdir "bin-int/%{cfg.buildcfg}"
	targetdir "bin/%{cfg.buildcfg}"

	-- source
	files { 
		"./include/**.h", 
		"./src/**.cpp"
	}

	-- vendors
	includedirs {
		"include",
		"vendor/SFML-2.6.1/include"
		-- "vendor/some_library/include"
		-- "vendor/another_library/include"
	}
	
	libdirs {
		"vendor/SFML-2.6.1/lib"
		-- "vendor/some_library/lib"
		-- "vendor/another_library/lib"
	}
	
	links {
		"sfml-main",
		"sfml-system-s",
		"sfml-window-s",
		"sfml-audio-s",
		"sfml-network-s",
		"opengl32",
		"winmm",
		"gdi32",
		"freetype",
		"vorbis",
		"vorbisenc",
		"vorbisfile",
		"openal32",
		"ogg",
		"flac",
		-- "some_library"
		-- "another_library"
	}

	defines { 
		"SFML_STATIC" 
	}
	
	-- configurations
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"