
--[[

	Premake5 script for Bagual Engine and its dependicies.

	Projects:
		BagualEngine
		BagualGame

	Dependencies:
		GLAD
		GLFW
		IMGUI
		IMGUI-NE
		BVH

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

newoption {
	trigger = "unity",
	description = "Enabled unity build configuration."
}

group "Dependencies"

project "GLAD"
	
	location "Source/GLAD/"
	kind "StaticLib"
	language "C"
	targetdir ("Intermediate/%{prj.name}-" .. outputdir)
	objdir ("Intermediate/%{prj.name}-" .. outputdir)
	
	defines {  }

	libdirs {  }

	includedirs { "%{prj.location}/include/" }

	files
	{
		"%{prj.location}/include/**.h",
		"%{prj.location}/src/glad.c"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines {  }

		files
		{

		}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

project "GLFW"
	
	location "Source/GLFW/"
	kind "StaticLib"
	language "C"
	targetdir ("Intermediate/%{prj.name}-" .. outputdir)
	objdir ("Intermediate/%{prj.name}-" .. outputdir)
	
	defines {  }

	libdirs {  }

	files
	{
		"%{prj.location}/src/internal.h",
		"%{prj.location}/src/mappings.h",
        "%{prj.location}/src/glfw_config.h",
        "%{prj.location}/include/GLFW/glfw3.h",
        "%{prj.location}/include/GLFW/glfw3native.h",
		"%{prj.location}/src/context.c",
		"%{prj.location}/src/init.c",
        "%{prj.location}/src/input.c",
		"%{prj.location}/src/monitor.c",
		"%{prj.location}/src/vulkan.c",
        "%{prj.location}/src/window.c"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		defines { "_GLFW_WIN32", "_CRT_SECURE_NO_WARNINGS" }

		files
		{
			"%{prj.location}/src/win32_platform.h",
			"%{prj.location}/src/win32_joystick.h",
			"%{prj.location}/src/wgl_context.h",
			"%{prj.location}/src/egl_context.h",
			"%{prj.location}/src/osmesa_context.h",
			"%{prj.location}/src/win32_init.c",
			"%{prj.location}/src/win32_joystick.c",
			"%{prj.location}/src/win32_monitor.c",
			"%{prj.location}/src/win32_time.c",
			"%{prj.location}/src/win32_thread.c",
			"%{prj.location}/src/win32_window.c",
			"%{prj.location}/src/wgl_context.c",
			"%{prj.location}/src/egl_context.c",
			"%{prj.location}/src/osmesa_context.c"
		}

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		optimize "On"
		runtime "Release"

project "IMGUI"
	
	location "Source/IMGUI/"
	kind "StaticLib"
	language "C"
	targetdir ("Intermediate/%{prj.name}-" .. outputdir)
	objdir ("Intermediate/%{prj.name}-" .. outputdir)
	
	defines {  }

	libdirs
	{
		("Intermediate/GLAD-" .. outputdir),
		("Intermediate/GLFW-" .. outputdir)
	}

	dependson { "GLAD", "GLFW" }

	includedirs
	{
		"%{prj.location}/",
		"%{prj.location}/examples",
		"Source/GLAD/include/",
		"Source/GLFW/include/"
	}

	files
	{
		"%{prj.location}/imgui.h",
		"%{prj.location}/imgui.cpp",
		"%{prj.location}/imgui_draw.cpp",
		"%{prj.location}/imgui_internal.h",
		"%{prj.location}/imgui_widgets.cpp",
		"%{prj.location}/imgui_demo.cpp",
		"%{prj.location}/examples/imgui_impl_glfw.h",
		"%{prj.location}/examples/imgui_impl_glfw.cpp",
		"%{prj.location}/examples/imgui_impl_opengl3.h",
		"%{prj.location}/examples/imgui_impl_opengl3.cpp"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		defines {  }

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		optimize "On"
		runtime "Release"

project "IMGUI-NE"
	
	location "Source/IMGUI-NE/"
	kind "StaticLib"
	language "C"
	targetdir ("Intermediate/%{prj.name}-" .. outputdir)
	objdir ("Intermediate/%{prj.name}-" .. outputdir)
	
	defines {  }

	libdirs
	{
		("Intermediate/GLAD-" .. outputdir),
		("Intermediate/GLFW-" .. outputdir),
		("Intermediate/IMGUI-" .. outputdir)
	}

	dependson { "GLAD", "GLFW", "IMGUI" }

	includedirs
	{
		"%{prj.location}/",
		"%{prj.location}/examples",
		"Source/GLAD/include/",
		"Source/GLFW/include/",
		"Source/IMGUI/"
	}

	files
	{
		"%{prj.location}/crude_json.h",
		"%{prj.location}/crude_json.cpp",
		"%{prj.location}/imgui_bezier_math.h",
		"%{prj.location}/imgui_bezier_math.inl",
		"%{prj.location}/imgui_canvas.cpp",
		"%{prj.location}/imgui_canvas.h",
		"%{prj.location}/imgui_extra_math.h",
		"%{prj.location}/imgui_extra_math.inl",
		"%{prj.location}/imgui_node_editor.h",
		"%{prj.location}/imgui_node_editor.cpp",
		"%{prj.location}/imgui_node_editor_api.cpp",
		"%{prj.location}/imgui_node_editor_internal.h",
		"%{prj.location}/imgui_node_editor_internal.inl"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		defines {  }

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		optimize "On"
		runtime "Release"

group ""

project "BagualEngine"
	
	location "Source/BagualEngine/"
	kind "StaticLib"
	language "C++"
	targetdir ("Intermediate/%{prj.name}-" .. outputdir)
	objdir ("Intermediate/%{prj.name}-" .. outputdir)
	
	-- define OBJL_CONSOLE_OUTPUT for loading geometry information
	
	pchheader "Bagual.pch.h"
	pchsource "Source/BagualEngine/src/Bagual.pch.cpp"

	includedirs
	{
		"%{prj.location}/include/",
		"Source/GLAD/include/",
		"Source/GLFW/include/",
		"Source/IMGUI/",
		"Source/IMGUI/examples/",
		"Source/IMGUI-NE/",
		"Source/OBJ-Loader/",
		"Source/BVH/src/",
		"Source/Embree/include/",
		"Source/glm/",
	}

	dependson { "GLAD", "GLFW", "IMGUI", "IMGUI-NE" }
	
	links
	{
		"GLAD.lib",
		"GLFW.lib",
		"IMGUI.lib",
		"IMGUI-NE.lib",
		"opengl32.lib",
		"embree4.lib",
		"tbb12.lib"
	}

	libdirs
	{
		("Intermediate/GLAD-" .. outputdir),
		("Intermediate/GLFW-" .. outputdir),
		("Intermediate/IMGUI-" .. outputdir),
		("Intermediate/IMGUI-NE-" .. outputdir),
		"Source/Embree/lib/"
	}

	files
	{
		"%{prj.location}/include/**.h",
		"%{prj.location}/src/**.cpp"
	}
	
	removefiles "%{prj.location}/src/Bagual.unity.cpp"
	
	filter "options:unity"
		removefiles "%{prj.location}/src/**.cpp"
		files { "%{prj.location}/src/Bagual.unity.cpp", "%{prj.location}/src/Bagual.pch.cpp" }
		defines { "BGL_UNITY_BUILD" }
	
	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		defines "BGL_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "BGL_RELEASE"
		optimize "On"
		runtime "Release"

project "BagualGame"

	location "Source/BagualGame/"
	kind "ConsoleApp"
	language "C++"
	targetdir ("Binaries/%{prj.name}-" .. outputdir)
	objdir ("Intermediate/%{prj.name}-" .. outputdir)
	
	dependson { "BagualEngine" }
	includedirs { "Source/BagualEngine/include/" }
	links { "BagualEngine.lib" }
	libdirs { ("Intermediate/BagualEngine-" .. outputdir) }

	files
	{
		"%{prj.location}/include/**.h",
		"%{prj.location}/src/**.cpp"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		postbuildcommands { "xcopy /y /d $(ProjectDir)..\\Embree\\bin\\*.dll $(ProjectDir)$(OutDir)" }

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		optimize "On"
		runtime "Release"
