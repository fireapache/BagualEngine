workspace "BagualEngine"
	
	configurations
	{
		"Debug32",
		"Test32",
		"Release32",
		"Debug64",
		"Test64",
		"Release64"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	startproject "BagualGame"

   filter "configurations:*32"
      architecture "x86"

   filter "configurations:*64"
      architecture "x86_64"

	
	

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

project "BagualEngine"
	
	location "BagualEngine"
	kind "StaticLib"
	language "C++"

	targetdir ("BagualEngine/lib/" .. outputdir)
	objdir ("Intermediate/%{prj.name}-" .. outputdir)

	links 
	{ 
		"SDL2.lib",
		"SDL2main.lib"
	}

	defines
	{
		"BGL_UNITY_BUILD=0"
	}

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

	filter "configurations:Test"
		defines "BGL_TEST"
		optimize "On"

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

	links 
	{ 
		"BagualEngine.lib"
	}

	includedirs
	{
		"./BagualEngine/include/"
	}

	libdirs
	{
		("BagualEngine/lib/" .. outputdir)
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

		postbuildcommands
		{
			("{COPY} %{wks.location}BagualEngine/third/SDL2/lib/%{cfg.system}/%{cfg.architecture}/SDL2.dll %{wks.location}Binaries/%{prj.name}-" .. outputdir)
		}

	filter "configurations:Debug*"
		defines "BGL_DEBUG"
		symbols "On"

	filter "configurations:Test*"
		defines "BGL_TEST"
		optimize "On"

	filter "configurations:Release*"
		defines "BGL_RELEASE"
		optimize "On"