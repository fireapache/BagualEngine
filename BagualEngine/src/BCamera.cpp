#include "BCamera.h"
#include "iostream"

using namespace Bagual::Draw;

namespace Bagual
{
	namespace Camera
	{
		void BCamera::DrawScreen()
		{
			SDL_LockSurface(settings.GetSurface());
			//SDL_FillRect(surface, NULL, 0x00000000);

			int x1 = rand() % (settings.GetWidth() + 500) - 250;
			int y1 = rand() % (settings.GetHeight() + 500) - 250;
			int x2 = rand() % (settings.GetWidth() + 500) - 250;
			int y2 = rand() % (settings.GetHeight() + 500) - 250;

			DrawLine(settings, BPixel(x1, y1), BPixel(x2, y2));

			SDL_UnlockSurface(settings.GetSurface());
		}

		void BCamera::SetupScreen()
		{
			SDL_Surface *surface;
			int width = BagualSettings::width;
			int height = BagualSettings::height;
			BLine<BPixel> edges[4];

			surface = SDL_GetWindowSurface(BagualSettings::window);

			int *pixels = (int*)surface->pixels;

			edges[0] = BLine<BPixel>(BPixel(0, 0), BPixel(0, height - 1));
			edges[1] = BLine<BPixel>(BPixel(0, height - 1), BPixel(width - 1, height - 1));
			edges[2] = BLine<BPixel>(BPixel(width - 1, height - 1), BPixel(width - 1, 0));
			edges[3] = BLine<BPixel>(BPixel(width - 1, 0), BPixel(0, 0));

			settings.SetScreenEdges(edges);
			settings.SetScreen(pixels);
			settings.SetSurface(surface);
			settings.SetWidth(width);
			settings.SetHeight(height);
		}

	}
}