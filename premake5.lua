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
		"vendor/GLFW/include", -- TODO: problem with some funy /M /MT /Md /MTd
		"vendor/GLAD/include",
		"vendor/GLM/include",
		"vendor/entt/include"
	}

	-- Libraries
	libdirs {
		"vendor/GLFW/lib-vc2022",
	}

	links {
		"glfw3",
		"opengl32"
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

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"