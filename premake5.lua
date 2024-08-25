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
		-- "vendor/some_library/include"
		-- "vendor/another_library/include"
	}

	libdirs {
		-- "vendor/some_library/lib"
		-- "vendor/another_library/lib"
	}

	links {
		-- "some_library"
		-- "another_library"
	}

	-- configurations
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"