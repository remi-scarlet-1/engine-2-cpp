project "Runtime"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	architecture "x64"

	targetdir "%{wks.location}/Bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/Bin-Int/%{cfg.buildcfg}"

	-- Source
	files { 
		"./premake5.lua",
		"./Source/**.h",
		"./Source/**.cpp"
	}

	includedirs {
		"./Source",
		"%{wks.location}/../RuntimeDependencies/Include"
	}

	defines {
		-- IMPORTANT! only choose one graphics backend (currently there is only OpenGL)
		"RT_BACKEND_GL",
	}

	-- Libraries
	libdirs { "%{wks.location}/Bin/%{cfg.buildcfg}" }
	links { "RuntimeDependencies" }

	-- Organize virtual folders
	vpaths {
		[""] = { "./premake5.lua" },
		["Source/*"] = { 
			"./Source/**.c", 
			"./Source/**.cpp", 
			"./Source/**.h", 
			"./Source/**.hpp" 
		}
	}

	-- Configurations
	filter "configurations:Release"
		defines { "RT_NDEBUG" }
		optimize "On"
		symbols "Off"

	filter "configurations:Debug"
		defines { "RT_DEBUG" }
		optimize "Off"
		symbols "On"

	-- Platforms
	filter "system:windows"
		defines { "RT_PLATFORM_WINDOWS" }

	filter "system:linux"
		defines { "RT_PLATFORM_LINUX" }

	filter "system:macosx"
		defines { "RT_PLATFORM_MACOS" }

	-- Reset filters to apply to all platforms
	filter {}