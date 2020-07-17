
#include "Bagual.pch.h"

#include "PlatformGeneric.h"

namespace bgl
{
	std::shared_ptr<BPlatformWindow> BPlatformGeneric::CreateWindow(const FWindowSettings& settings)
	{
		auto window = std::make_shared<BPlatformWindow>(settings);
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
		if (sdlWindow)
		{
			SDL_SetWindowTitle(sdlWindow, settings.name.c_str());
			SDL_SetWindowSize(sdlWindow, settings.width, settings.height);
		}
	}

	void BGenericPlatformWindow::Create()
	{
		BGL_ASSERT(sdlWindow == nullptr && "This window was already created!");
		
		sdlWindow = SDL_CreateWindow(
			"Bagual Engine",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			settings.width,
			settings.height,
			SDL_WINDOW_SHOWN);

		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetRelativeMouseMode(SDL_FALSE);

		sdlSurface = SDL_GetWindowSurface(sdlWindow);

		canvas = std::make_shared<BCanvas>(this, sdlSurface->pixels, settings.width, settings.height);

	}

	void BGenericPlatformWindow::Destroy()
	{
		if (sdlWindow)
		{
			SDL_DestroyWindow(sdlWindow);
		}
	}

	BGenericPlatformWindow::~BGenericPlatformWindow()
	{
		Destroy();
	}

	SDL_Window* BGenericPlatformWindow::GetSDL_Window()
	{
		return sdlWindow;
	}

	SDL_Surface* BGenericPlatformWindow::GetSDL_Surface()
	{
		return sdlSurface;
	}

}