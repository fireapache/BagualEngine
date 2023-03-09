
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "PlatformGeneric.h"

#include <imgui.h>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_node_editor.h>
// clang-format on

namespace bgl
{

	BPlatformGeneric::BPlatformGeneric()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		imguiConfig = &ImGui::GetIO();
		( void )imguiConfig;
		imguiConfig->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		imguiConfig->ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
		imguiConfig->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so m_platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if( imguiConfig->ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
		{
			style.WindowRounding = 0.0f;
			style.Colors[ ImGuiCol_WindowBg ].w = 1.0f;
		}
	}

	BPlatformGeneric::~BPlatformGeneric()
	{
	}

	BPlatformWindow* BPlatformGeneric::createWindow( const FWindowSettings& settings )
	{
		windows.Add( std::make_shared< BGenericPlatformWindow >( settings ) );
		return windows.back().get();
	}

	BPlatformWindow* BPlatformGeneric::createWindow()
	{
		windows.Add( std::make_shared< BGenericPlatformWindow >( FWindowSettings() ) );
		return windows.back().get();
	}

	ImGuiIO* BPlatformGeneric::GetImguiConfig()
	{
		return imguiConfig;
	}

	BGenericPlatformWindow::BGenericPlatformWindow( const FWindowSettings& windowSettings )
	{
		settings = windowSettings;
		create();
	}

	void BGenericPlatformWindow::applySettings()
	{
		if( glfwWindow )
		{
			glfwSetWindowTitle( glfwWindow, settings.Title.c_str() );
			glfwSetWindowSize( glfwWindow, settings.width, settings.height );
		}
	}

	void BGenericPlatformWindow::create()
	{
		BGL_ASSERT( glfwWindow == nullptr && "This window was already created!" );

		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );

		glfwWindow = glfwCreateWindow(
			static_cast< int >( settings.width ),
			static_cast< int >( settings.height ),
			settings.Title.c_str(),
			nullptr,
			nullptr );

		BGL_ASSERT( glfwWindow != nullptr && "This window was already created!" );

		// Setting window starting position on the first monitor
		if( settings.x == BGL_WINDOW_CENTRALIZED && settings.y == BGL_WINDOW_CENTRALIZED )
		{
			int32_t monitorCount;
			auto monitors = glfwGetMonitors( &monitorCount );
			GLFWmonitor* monitor = monitors[ 0 ];

			const GLFWvidmode* mode = glfwGetVideoMode( monitor );

			BVec2i monitorPos;
			glfwGetMonitorPos( monitor, &monitorPos.x, &monitorPos.y );

			const float windowHalfSizeX( static_cast< float >( mode->width - settings.width ) / 2.f );
			const float windowHalfSizeY( static_cast< float >( mode->height - settings.height ) / 2.f * 0.25f );

			glfwSetWindowPos(
				glfwWindow,
				monitorPos.x + static_cast< int >( windowHalfSizeX ),
				monitorPos.y + static_cast< int >( windowHalfSizeY ) );
		}
		else
		{
			glfwSetWindowPos( glfwWindow, static_cast< int >( settings.x ), static_cast< int >( settings.y ) );
		}

		// Creating canvas
		m_canvas = std::make_unique< BCanvas >( this, settings.width, settings.height );

		// Need context to load GLAD
		glfwMakeContextCurrent( glfwWindow );
		// TODO: Load GLAD only once
		BGL_ASSERT( gladLoadGL() && "Could not load GLAD!" );

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL( glfwWindow, true );
		ImGui_ImplOpenGL3_Init( glsl_version );
	}

	void BGenericPlatformWindow::destroy()
	{
		if( glfwWindow )
		{
			glfwDestroyWindow( glfwWindow );
			glfwWindow = nullptr;
		}
	}

	BGenericPlatformWindow::~BGenericPlatformWindow()
	{
		destroy();
	}

	bool BGenericPlatformWindow::Tick()
	{
		if( glfwWindow == nullptr )
		{
			return false;
		}

		if( glfwWindowShouldClose( glfwWindow ) )
		{
			destroy();
			return false;
		}

		glfwPollEvents();

		// Checking if need to resize canvas based on the window size
		{
			int width, height;
			glfwGetWindowSize( glfwWindow, &width, &height );

			if( width != settings.width || height != settings.height )
			{
				BGL_LOG( "Resizing window" );
				settings.width = width;
				settings.height = height;
				m_canvas->setSize( width, height );
			}
		}

		return true;
	}

	GLFWwindow* BGenericPlatformWindow::GetGLFW_Window()
	{
		return glfwWindow;
	}

	uint32_t& BGenericPlatformWindow::GetglTex()
	{
		return glTex;
	}

	void BGenericPlatformWindow::SetglTex( uint32_t index )
	{
		glTex = index;
	}

} // namespace bgl