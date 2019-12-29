
#include "BEngine.h"
#include "BCamera.h"
#include "BLogger.h"

namespace Bagual::Camera
{

	BCamera::BCamera()
	{
		auto graphics = Bagual::BagualEngine::GraphicsPlatform();

		if (graphics)
		{
			canvas = graphics->CreateCanvas(settings.GetWidth(), settings.GetHeight());
		}
		else
		{
			Bagual::Utils::BLogger::Log("Could not create canvas for camera object!");
		}
	}

	void BCamera::DrawScreen()
	{
		SDL_LockSurface(settings.GetSurface());
		//SDL_FillRect(surface, NULL, 0x00000000);

		int x1 = rand() % (settings.GetWidth() + 500) - 250;
		int y1 = rand() % (settings.GetHeight() + 500) - 250;
		int x2 = rand() % (settings.GetWidth() + 500) - 250;
		int y2 = rand() % (settings.GetHeight() + 500) - 250;

		Bagual::Draw::DrawLine(settings, BPixelPos(x1, y1), BPixelPos(x2, y2));

		SDL_UnlockSurface(settings.GetSurface());
	}

	void BCamera::SetupScreen()
	{
		SDL_Surface *surface;
		int width = BagualSettings::width;
		int height = BagualSettings::height;
		BLine<BPixelPos> edges[4];

		surface = SDL_GetWindowSurface(BagualSettings::window);

		int *pixels = (int*)surface->pixels;

		edges[0] = BLine<BPixelPos>(BPixelPos(0, 0), BPixelPos(0, height - 1));
		edges[1] = BLine<BPixelPos>(BPixelPos(0, height - 1), BPixelPos(width - 1, height - 1));
		edges[2] = BLine<BPixelPos>(BPixelPos(width - 1, height - 1), BPixelPos(width - 1, 0));
		edges[3] = BLine<BPixelPos>(BPixelPos(width - 1, 0), BPixelPos(0, 0));

		settings.SetScreenEdges(edges);
		settings.SetScreen(pixels);
		settings.SetSurface(surface);
		settings.SetWidth(width);
		settings.SetHeight(height);
	}

	
}