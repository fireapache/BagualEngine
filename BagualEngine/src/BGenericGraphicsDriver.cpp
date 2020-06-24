
#include "BGenericGraphicsDriver.h"
#include "BCamera.h"
#include "BCameraManager.h"
#include "BSettings.h"

#include <SDL.h>

namespace Bagual::Graphics
{
	
	BGenericGraphicsDriver::BGenericGraphicsDriver()
	{
		SDL_Window* window;
			
		SDL_Init(SDL_INIT_VIDEO);

		window = SDL_CreateWindow("Bagual Engine",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			Settings::width,
			Settings::height,
			SDL_WINDOW_SHOWN);

		Settings::window = window;

		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetRelativeMouseMode(SDL_FALSE);

		SDL_Surface* surface;
		int width = Settings::width;
		int height = Settings::height;
		BLine<BPixelPos> edges[4];

		surface = SDL_GetWindowSurface(window);

		int* pixels = (int*)surface->pixels;

		Settings::screenEdges[0] = BLine<BPixelPos>(BPixelPos(0, 0), BPixelPos(0, height - 1));
		Settings::screenEdges[1] = BLine<BPixelPos>(BPixelPos(0, height - 1), BPixelPos(width - 1, height - 1));
		Settings::screenEdges[2] = BLine<BPixelPos>(BPixelPos(width - 1, height - 1), BPixelPos(width - 1, 0));
		Settings::screenEdges[3] = BLine<BPixelPos>(BPixelPos(width - 1, 0), BPixelPos(0, 0));

		Settings::pixels = pixels;
		Settings::surface = surface;
		Settings::width = width;
		Settings::height = height;
	}

	BGenericGraphicsDriver::~BGenericGraphicsDriver()
	{
		SDL_Quit();
	}

	void BGenericGraphicsDriver::RenderFrame()
	{
		BGraphicsDriverBase::RenderFrame();

		auto surface = static_cast<SDL_Surface*>(Settings::surface);

		SDL_LockSurface(surface);
		
		auto cameras = Bagual::Camera::BCameraManager::GetCameras();

		for (auto camera : cameras)
		{

		}

		SDL_UnlockSurface(surface);

		auto window = static_cast<SDL_Window*>(Settings::window);

		SDL_UpdateWindowSurface(window);
	}

	void BGenericGraphicsDriver::Delay(unsigned int ms)
	{
		SDL_Delay(ms);
	}

	std::shared_ptr<BCanvas> BGenericGraphicsDriver::CreateCanvas(unsigned short width, unsigned short height)
	{
		return std::shared_ptr<BCanvas>();
	}

	std::weak_ptr<Bagual::Graphics::BViewport> BGenericGraphicsDriver::CreateViewport(BCanvas& canvas, FViewportSettings viewportSettings)
	{
		return std::weak_ptr<BViewport>();
	}

}

