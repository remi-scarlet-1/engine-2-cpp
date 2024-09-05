workspace "Engine"
	configurations { "Release", "Debug" }
    platforms { "x64" }
	location "Generated"

dofile("./RuntimeDependencies/premake5.lua") -- Import the Runtime project's premake script
dofile("./Runtime/premake5.lua") -- Import the RuntimeDependencies project's premake script