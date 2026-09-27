-- Game Project premake template.
-- Kismet

workspace "Kismet"
	architecture "x64"
	startproject "Kismet"

	configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}

-- NOTE SATURN_DIR environment variable always points to the root dir of Saturn
local SaturnDir = os.getenv('SATURN_DIR')
-- Now, replace the "//" with "/"
SaturnDir = SaturnDir:gsub( "\\", "/" )

-- NOTE: This acts as the Saturn Engine module!
group "Engine"
project "Saturn"
	kind "Makefile"
	language "C++"
	cppdialect "C++23"
	staticruntime "on"
	location "Build"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	debugcommand ( "" )
	buildcommands   ( "" )
	rebuildcommands ( "" )
	cleancommands   ( "" )

	files 
	{
		SaturnDir .. "/Saturn/src/**.cpp",
		SaturnDir .. "/Saturn/src/**.h",
		SaturnDir .. "/Saturn/src/**.cs",
	}

-- Kismet
group "Game"
project "Kismet"
	kind "Makefile"
	language "C++"
	cppdialect "C++23"
	staticruntime "on"
	location "Build"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{wks.location}/Source/**.h",
		"%{wks.location}/Source/**.cpp",
		"%{wks.location}/Source/**.cs"
	}

	removefiles 
	{ 
		"Generated/**.Gen.cpp", 
		"Generated/**.Gen.h" 
	}
	
	filter "system:windows"
		systemversion "latest"

		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"

			debugcommand ( SaturnDir .. "/bin/Debug-windows-x86_64/Saturn-Editor/Saturn-Editor.exe" )
			debugargs { "%{wks.location}/%{prj.name}.sproject" }
			debugdir ( SaturnDir .. "/Saturn-Editor" )

			filter { "system:windows", "configurations:Debug" }
				buildcommands
				{
					SaturnDir .. "/bin/Debug-windows-x86_64/SaturnBuildTool/RT/Run.bat /BUILD /NAME:%{prj.name} /Win64 /Debug \"/PROJECT:%{wks.location\""
				}

			filter { "system:windows", "configurations:Debug" }
				rebuildcommands 
				{
					SaturnDir .. "/bin/Debug-windows-x86_64/SaturnBuildTool/RT/Run.bat /REBUILD /NAME:%{prj.name} /Win64 /Debug /PROJECT:%{wks.location}"
				}

			filter { "system:windows", "configurations:Debug" }
				cleancommands
				{
					SaturnDir .. "/bin/Debug-windows-x86_64/SaturnBuildTool/RT/Run.bat /CLEAN /NAME:%{prj.name} /Win64 /Debug /PROJECT:%{wks.location}"
				}

		filter "configurations:Release"
			runtime "Release"
			optimize "on"

			debugcommand ( SaturnDir .. "/bin/Release-windows-x86_64/Saturn-Editor/Saturn-Editor.exe" )
			debugargs    { "%{wks.location}/%{prj.name}.sproject" }
			debugdir     ( SaturnDir .. "/Saturn-Editor" )

			filter { "system:windows", "configurations:Release" }
				buildcommands
				{
					SaturnDir .. "/bin/Release-windows-x86_64/SaturnBuildTool/RT/Run.bat /BUILD /NAME:%{prj.name} /Win64 /Release /PROJECT:%{wks.location}"
				}

			filter { "system:windows", "configurations:Release" }
				rebuildcommands 
				{
					SaturnDir .. "/bin/Release-windows-x86_64/SaturnBuildTool/RT/Run.bat /REBUILD /NAME:%{prj.name} /Win64 /Release /PROJECT:%{wks.location}"
				}

			filter { "system:windows", "configurations:Release" }
				cleancommands
				{
					SaturnDir .. "/bin/Release-windows-x86_64/SaturnBuildTool/RT/Run.bat /CLEAN /NAME:%{prj.name} /Win64 /Release /PROJECT:%{wks.location}"
				}

		filter "configurations:Dist"
			runtime "Release"
			symbols "on"

			filter { "system:windows", "configurations:Dist" }
				buildcommands
				{
					SaturnDir .. "/bin/Dist-windows-x86_64/SaturnBuildTool/RT/Run.bat /BUILD /NAME:%{prj.name} /Win64 /Dist /PROJECT:%{wks.location}"
				}

			filter { "system:windows", "configurations:Dist" }
				rebuildcommands 
				{
					SaturnDir .. "/bin/Dist-windows-x86_64/SaturnBuildTool/RT/Run.bat /REBUILD /NAME:%{prj.name} /Win64 /Dist /PROJECT:%{wks.location}"
				}

			filter { "system:windows", "configurations:Dist" }
				cleancommands
				{
					SaturnDir .. "/bin/Dist-windows-x86_64/SaturnBuildTool/RT/Run.bat /CLEAN /NAME:%{prj.name} /Win64 /Dist /PROJECT:%{wks.location}"
				}

	filter "system:macosx"
		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"

			debugcommand ( SaturnDir .. "/bin/Debug-macosx-x86_64/Saturn-Editor/Saturn-Editor" )
			debugargs { "%{wks.location}/%{prj.name}.sproject" }
			debugdir ( SaturnDir .. "/Saturn-Editor" )

			filter { "system:macosx", "configurations:Debug" }
				buildcommands
				{
					-- This is so bad, so fucking bad, but premake gives us the relative path and not the abs path
					SaturnDir .. "/bin/Debug-macosx-AnyCPU/SaturnBuildTool/RT/Run.sh /BUILD /NAME:%{prj.name} /Apple /Debug /PROJECT:" .. path.getabsolute(".")
				}

			filter { "system:macosx", "configurations:Debug" }
				rebuildcommands 
				{
					SaturnDir .. "/bin/Debug-macosx-AnyCPU/SaturnBuildTool/RT/Run.sh /REBUILD /NAME:%{prj.name} /Apple /Debug /PROJECT:" .. path.getabsolute(".")
				}

			filter { "system:macosx", "configurations:Debug" }
				cleancommands
				{
					SaturnDir .. "/bin/Debug-macosx-AnyCPU/SaturnBuildTool/RT/Run.sh /CLEAN /NAME:%{prj.name} /Apple /Debug /PROJECT:" .. path.getabsolute(".")
				}

		filter "configurations:Release"
			runtime "Release"
			optimize "on"

			debugcommand ( SaturnDir .. "/bin/Release-macosx-x86_64/Saturn-Editor/Saturn-Editor" )
			debugargs    { "%{wks.location}/%{prj.name}.sproject" }
			debugdir     ( SaturnDir .. "/Saturn-Editor" )

			filter { "system:macosx", "configurations:Release" }
				buildcommands
				{
					SaturnDir .. "/bin/Release-macosx-AnyCPU/SaturnBuildTool/RT/Run.sh /BUILD /NAME:%{prj.name} /Apple /Release /PROJECT:"  .. path.getabsolute(".")
				}

			filter { "system:macosx", "configurations:Release" }
				rebuildcommands 
				{
					SaturnDir .. "/bin/Release-macosx-AnyCPU/SaturnBuildTool/RT/Run.sh /REBUILD /NAME:%{prj.name} /Apple /Release /PROJECT:"  .. path.getabsolute(".")
				}

			filter { "system:macosx", "configurations:Release" }
				cleancommands
				{
					SaturnDir .. "/bin/Release-macosx-AnyCPU/SaturnBuildTool/RT/Run.sh /CLEAN /NAME:%{prj.name} /Apple /Release /PROJECT:"  .. path.getabsolute(".")
				}

		filter "configurations:Dist"
			runtime "Release"
			symbols "on"

			filter { "system:macosx", "configurations:Dist" }
				buildcommands
				{
					SaturnDir .. "/bin/Dist-macosx-AnyCPU/SaturnBuildTool/RT/Run.sh /BUILD /NAME:%{prj.name} /Apple /Dist /PROJECT:" .. path.getabsolute(".")
				}

			filter { "system:macosx", "configurations:Dist" }
				rebuildcommands 
				{
					SaturnDir .. "/bin/Dist-macosx-AnyCPU/SaturnBuildTool/RT/Run.sh /REBUILD /NAME:%{prj.name} /Apple /Dist /PROJECT:" .. path.getabsolute(".")
				}

			filter { "system:macosx", "configurations:Dist" }
				cleancommands
				{
					SaturnDir .. "/bin/Dist-macosx-AnyCPU/SaturnBuildTool/RT/Run.sh /CLEAN /NAME:%{prj.name} /Apple /Dist /PROJECT:" .. path.getabsolute(".")
				}
