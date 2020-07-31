
--[[

	Premake5 script for Bagual Engine and its dependicies.

	Projects:
		BagualEngine
		BagualGame

	Windows:
		Debug, Release
		Win32, x64

	Linux: TODO!

--]]

workspace "BagualEngine"
	
	configurations { "Debug", "Release" }
	platforms { "Win32", "x64" }

	startproject "BagualGame"
	defaultplatform "x64"

	flags { "MultiProcessorCompile" }

	filter { "platforms:Win32" }
		system "Windows"
		architecture "x86"

	filter { "platforms:x64" }
		system "Windows"
		architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

project "BagualEngine"
	
	location "BagualEngine"
	kind "StaticLib"
	language "C++"
	targetdir ("BagualEngine/lib/" .. outputdir)
	objdir ("Intermediate/%{prj.name}-" .. outputdir)
	links { "SDL2.lib", "SDL2main.lib" }
	defines { "BGL_UNITY_BUILD=0" }

	includedirs
	{
		"./BagualEngine/include/",
		"./BagualEngine/third/SDL2/include/"
	}

	libdirs
	{
		"./BagualEngine/third/SDL2/lib/%{cfg.system}/%{cfg.architecture}/"
	}

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.cpp"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		defines "BGL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "BGL_RELEASE"
		optimize "On"

project "BagualGame"

	location "BagualGame"
	kind "ConsoleApp"
	language "C++"
	targetdir ("Binaries/%{prj.name}-" .. outputdir)
	objdir ("Intermediate/%{prj.name}-" .. outputdir)
	dependson { "BagualEngine" }
	links { "BagualEngine.lib" }
	includedirs { "./BagualEngine/include/" }
	libdirs { ("BagualEngine/lib/" .. outputdir) }

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.cpp"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		postbuildcommands
		{
			("{COPY} %{wks.location}BagualEngine/third/SDL2/lib/%{cfg.system}/%{cfg.architecture}/SDL2.dll %{wks.location}Binaries/%{prj.name}-" .. outputdir)
		}

	filter "configurations:Debug"
		defines "BGL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "BGL_RELEASE"
		optimize "On"