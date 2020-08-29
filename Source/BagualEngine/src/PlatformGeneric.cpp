
#include "Bagual.pch.h"

#include "PlatformGeneric.h"

namespace bgl
{
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
		canvas = std::make_shared<BCanvas>(settings.width, settings.height);

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


}