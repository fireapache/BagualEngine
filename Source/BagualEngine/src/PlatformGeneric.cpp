
#include "Bagual.pch.h"

#include "PlatformGeneric.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace bgl
{

	BPlatformGeneric::BPlatformGeneric()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		imguiConfig = &ImGui::GetIO(); (void)imguiConfig;
		imguiConfig->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		imguiConfig->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		imguiConfig->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (imguiConfig->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

	}

	std::shared_ptr<BPlatformWindow> BPlatformGeneric::CreateWindow(const FWindowSettings& settings)
	{
		auto window = std::make_shared<BGenericPlatformWindow>(settings);
		windows.Add(window);
		return window;
	}

	std::shared_ptr<bgl::BPlatformWindow> BPlatformGeneric::CreateWindow()
	{
		auto window = std::make_shared<BGenericPlatformWindow>(FWindowSettings());
		windows.Add(window);
		return window;
	}

	ImGuiIO* BPlatformGeneric::GetImguiConfig()
	{
		return imguiConfig;
	}

	BGenericPlatformWindow::BGenericPlatformWindow(const FWindowSettings& windowSettings)
	{
		settings = windowSettings;
		Create();
	}

	void BGenericPlatformWindow::ApplySettings()
	{
		if (glfwWindow)
		{
			glfwSetWindowTitle(glfwWindow, settings.Title.c_str());
			glfwSetWindowSize(glfwWindow, settings.width, settings.height);
		}
	}

	void BGenericPlatformWindow::Create()
	{
		BGL_ASSERT(glfwWindow == nullptr && "This window was already created!");
		
		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		glfwWindow = glfwCreateWindow(
			settings.width, 
			settings.height, 
			settings.Title.c_str(), 
			nullptr, nullptr);
		
		BGL_ASSERT(glfwWindow != nullptr && "This window was already created!");

		// Setting window starting position on the first monitor
		if (settings.x == BGL_WINDOW_CENTRALIZED && settings.y == BGL_WINDOW_CENTRALIZED)
		{
			int32 monitorCount;
			auto monitors = glfwGetMonitors(&monitorCount);
			GLFWmonitor* monitor = monitors[0];

			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			int monitorX, monitorY;
			glfwGetMonitorPos(monitor, &monitorX, &monitorY);

			glfwSetWindowPos(glfwWindow,
				monitorX + (mode->width - settings.width) / 2,
				monitorY + (mode->height - settings.height) / 2);
		}
		else
		{
			glfwSetWindowPos(glfwWindow, settings.x, settings.y);
		}

		// Creating canvas
		canvas = std::make_shared<BCanvas>(this, settings.width, settings.height);

		// Need context to load GLAD
		glfwMakeContextCurrent(glfwWindow);
		// TODO: Load GLAD only once
		BGL_ASSERT(gladLoadGL() && "Could not load GLAD!");

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

	}

	void BGenericPlatformWindow::Destroy()
	{
		if (glfwWindow)
		{
			glfwDestroyWindow(glfwWindow);
			glfwWindow = nullptr;
		}
	}

	BGenericPlatformWindow::~BGenericPlatformWindow()
	{
		Destroy();
	}

	bool BGenericPlatformWindow::Tick()
	{
		if (glfwWindow == nullptr)
		{
			return false;
		}

		if (glfwWindowShouldClose(glfwWindow))
		{
			Destroy();
			return false;
		}

		glfwPollEvents();

		// Checking if need to resize canvas based on the window size
		{
			int width, height;
			glfwGetWindowSize(glfwWindow, &width, &height);

			if (width != settings.width || height != settings.height)
			{
				BGL_LOG("Resizing window");
				settings.width = width;
				settings.height = height;
				canvas->SetSize(width, height);
			}
		}

		return true;
	}

	GLFWwindow* BGenericPlatformWindow::GetGLFW_Window()
	{
		return glfwWindow;
	}

	uint32& BGenericPlatformWindow::GetglTex()
	{
		return glTex;
	}

	void BGenericPlatformWindow::SetglTex(uint32 index)
	{
		glTex = index;
	}

}