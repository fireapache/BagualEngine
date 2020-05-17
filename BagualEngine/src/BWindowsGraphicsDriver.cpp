
#include "BWindowsGraphicsDriver.h"
#include "BCamera.h"
#include "BCameraManager.h"

#include <SDL.h>

namespace Bagual::Graphics
{
	
	BWindowsGraphicsDriver::BWindowsGraphicsDriver()
	{
		SDL_Window* window;
			
		SDL_Init(SDL_INIT_VIDEO);

		window = SDL_CreateWindow("Bagual Engine",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			BagualSettings::width,
			BagualSettings::height,
			SDL_WINDOW_SHOWN);

		BagualSettings::window = window;

		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	BWindowsGraphicsDriver::~BWindowsGraphicsDriver()
	{
		SDL_Quit();
	}

	void BWindowsGraphicsDriver::Render()
	{
		BGraphicsDriverBase::Render();
		SDL_UpdateWindowSurface(BagualSettings::window);
	}

	void BWindowsGraphicsDriver::Delay(unsigned int ms)
	{
		SDL_Delay(ms);
	}

	std::weak_ptr<BCanvas> BWindowsGraphicsDriver::CreateCanvas(unsigned short width, unsigned short height)
	{
		return std::weak_ptr<BCanvas>();
	}


}

