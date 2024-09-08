project "RuntimeDependencies"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	architecture "x64"

	targetdir "%{wks.location}/Bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/Bin-Int/%{cfg.buildcfg}"

	-- Source
	files {
		"./premake5.lua",
		"./Include/**.h",
		"./Include/**.hpp",
		"./Source/**.c",
		"./Source/**.cpp",
	}

	includedirs {
		"./Include"
	}

	vpaths {
		[""] = { "./premake5.lua" },
		["Include/*"] = { 
			"./Include/**.h",
			"./Include/**.hpp",
		},
		["Source/*"] = { 
			"./Source/**.c", 
			"./Source/**.cpp", 
		},
	}

	disablewarnings { "4996" } -- GLFW uses annoying "unsafe" functions
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"

	filter "system:windows"
        defines { "_GLFW_WIN32" }
        links { 
			"gdi32", 
			"opengl32" 
		}

    filter "system:macosx"
        defines { "_GLFW_COCOA" }
        links { 
			"Cocoa.framework", 
			"IOKit.framework", 
			"CoreVideo.framework",
			"OpenGL.framework" 
		}

    filter "system:linux"
        defines { "_GLFW_X11" }
        links { 
			"X11", 
			"pthread", 
			"GL" 
		}