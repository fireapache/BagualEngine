
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
		if (glfWindow)
		{
			//SDL_SetWindowTitle(sdlWindow, settings.name.c_str());
			//SDL_SetWindowSize(sdlWindow, settings.width, settings.height);
		}
	}

	void BGenericPlatformWindow::Create()
	{
		BGL_ASSERT(glfWindow == nullptr && "This window was already created!");
		
		glfWindow = glfwCreateWindow(
			settings.width, 
			settings.height, 
			"Bagual Engine", 
			NULL, NULL);
		
		BGL_ASSERT(glfWindow != nullptr && "This window was already created!");

		//canvas = std::make_shared<BCanvas>(this, sdlSurface, 640, 480);

	}

	void BGenericPlatformWindow::Destroy()
	{
		if (glfWindow)
		{
			glfwDestroyWindow(glfWindow);
		}
	}

	BGenericPlatformWindow::~BGenericPlatformWindow()
	{
		Destroy();
	}

	bool BGenericPlatformWindow::Tick()
	{
		if (glfWindow == nullptr)
		{
			return false;
		}

		if (glfwWindowShouldClose(glfWindow))
		{
			Destroy();
			return false;
		}

		return true;
	}

	GLFWwindow* BGenericPlatformWindow::GetGLFW_Window()
	{
		return glfWindow;
	}


}