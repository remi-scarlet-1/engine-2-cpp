project "Runtime"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	architecture "x64"

	targetdir "%{wks.location}/Bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/Bin-Int/%{cfg.buildcfg}"

	-- Source
	files { 
		"./Source/**.h",
		"./Source/**.cpp"
	}

	includedirs {
		"./Source",
		"%{wks.location}/../RuntimeDependencies/Include"
	}

	-- Libraries
	libdirs { "%{wks.location}/Bin/%{cfg.buildcfg}" }
	links { "RuntimeDependencies" }

	-- Organize virtual folders
	vpaths {
		["Source/*"] = { 
			"./Source/**.c", 
			"./Source/**.cpp", 
			"./Source/**.h", 
			"./Source/**.hpp" 
		}
	}

	-- Configurations
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		symbols "Off"

	filter "configurations:Debug"
		defines { "DEBUG" }
		optimize "Off"
		symbols "On"

	-- Platforms
	filter "system:windows"
		defines { "PLATFORM_WINDOWS" }

	filter "system:linux"
		defines { "PLATFORM_LINUX" }

	filter "system:macosx"
		defines { "PLATFORM_MACOS" }

	-- Reset filters to apply to all platforms
	filter {}