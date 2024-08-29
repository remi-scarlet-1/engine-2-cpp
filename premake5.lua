workspace "Engine"
	configurations { "Release", "Debug" }
    platforms { "x64" }
	location "generated"

project "Runtime"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	architecture "x64"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	-- Source
	files { 
		"./assets/**.*",
		"./src/**.h",
		"./src/**.cpp",
		"./vendor/GLAD/src/glad.c"
	}

	includedirs {
		"src",
		"vendor/SFML-2.6.1/include",
		"vendor/GLAD/include",
		"vendor/GLM/include",
		"vendor/TinyGLTF/include"
	}

	-- Libraries
	defines { 
		"SFML_STATIC",
		"TINYGLTF_USE_CPP14"
	}

	libdirs {
		"vendor/SFML-2.6.1/lib",
	}

	links {
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
	}

	-- Organize virtual folders
	vpaths {
		["Assets/*"] 		= { "./assets/**.*" },
		["Source/*"] 		= { "./src/**.c", "./src/**.cpp", "./src/**.h", "./src/**.hpp" },
		["Vendor/*"] 		= { "./vendor/**.c", "./vendor/**.cpp" },
	}

	-- Copy assets into bin
	postbuildcommands {
        "{COPYDIR} %{wks.location}../assets %{cfg.targetdir}/assets"
    }

	-- Configurations
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

		links {
			"sfml-main",
			"sfml-system-s",
			"sfml-window-s",
			"sfml-audio-s",
			"sfml-network-s",
		}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

		links {
			"sfml-main-d",
			"sfml-system-s-d",
			"sfml-window-s-d",
			"sfml-audio-s-d",
			"sfml-network-s-d",
		}