
#include "GenericPlatformWindow.h"


namespace bgl
{
	BGenericPlatformWindow::BGenericPlatformWindow(const FWindowSettings& windowSettings)
	{
		settings = windowSettings;
		InitializeWindow();
	}

	void BGenericPlatformWindow::ApplyWindowSettings()
	{
		if (sdlWindow)
		{
			SDL_SetWindowTitle(sdlWindow, settings.name.c_str());
			SDL_SetWindowSize(sdlWindow, settings.width, settings.height);
		}
	}

	void BGenericPlatformWindow::InitializeWindow()
	{
		SDL_Init(SDL_INIT_VIDEO);

		sdlWindow = SDL_CreateWindow(
			"Bagual Engine",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			settings.width,
			settings.height,
			SDL_WINDOW_SHOWN);

		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	void BGenericPlatformWindow::FinalizeWindow()
	{
		if (sdlWindow)
		{
			SDL_DestroyWindow(sdlWindow);
		}
	}

	BGenericPlatformWindow::~BGenericPlatformWindow()
	{
		FinalizeWindow();
	}


}