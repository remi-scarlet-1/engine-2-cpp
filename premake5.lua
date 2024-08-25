workspace "Engine"
	configurations { "Debug", "Release" }
	location "generated"

project "Runtime"
	kind "ConsoleApp"
	language "C++"
	architecture "x64"

	objdir "bin-int/%{cfg.buildcfg}"
	targetdir "bin/%{cfg.buildcfg}"

	-- Source
	files { 
		"./include/**.h", 
		"./src/**.cpp",
		"./vendor/GLAD/src/glad.c"
	}

	includedirs {
		"include",
		"vendor/SFML-2.6.1/include",
		"vendor/GLAD/include",
	}

	-- Libraries
	defines { 
		"SFML_STATIC",
	}

	libdirs {
		"vendor/SFML-2.6.1/lib",
	}

	links {
		"opengl32",
		"winmm",
		"gdi32",
		"freetype",
		"sfml-main",
		"sfml-system-s",
		"sfml-window-s",
		"sfml-audio-s",
		"sfml-network-s",
		"vorbis",
		"vorbisenc",
		"vorbisfile",
		"openal32",
		"ogg",
		"flac",
	}

	-- Organize vendor files under a virtual folder
	vpaths {
		["Source/*"] = { "./include/**.h", "./include/**.hpp", "./src/**.c", "./src/**.cpp" },
		["Vendor/*"] = { "./vendor/**.c", "./vendor/**.cpp" },
	}

	-- configurations
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"