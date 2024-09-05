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

	-- Copy assets into bin (testing)
	postbuildcommands {
        "{COPYDIR} %{wks.location}../Resources %{cfg.targetdir}/Resources"
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