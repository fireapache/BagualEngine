#include "BagualCamera.h"


void Camera::DrawScreen()
{
	SDL_LockSurface(settings.GetSurface());
	//SDL_FillRect(surface, NULL, 0x00000000);

	int x1 = rand() % (settings.GetWidth() + 500) - 250;
	int y1 = rand() % (settings.GetHeight() + 500) - 250;
	int x2 = rand() % (settings.GetWidth() + 500) - 250;
	int y2 = rand() % (settings.GetHeight() + 500) - 250;

	DrawLine(settings, Pixel(x1, y1), Pixel(x2, y2));

	SDL_UnlockSurface(settings.GetSurface());
}

void Camera::SetupScreen()
{
	SDL_Surface *surface;
	int width = BagualSettings::width;
	int height = BagualSettings::height;
	Line<Pixel> edges[4];

	surface = SDL_GetWindowSurface(BagualSettings::window);

	int *pixels = (int*)surface->pixels;

	edges[0] = Line<Pixel>(Pixel(0, 0), Pixel(0, height - 1));
	edges[1] = Line<Pixel>(Pixel(0, height - 1), Pixel(width - 1, height - 1));
	edges[2] = Line<Pixel>(Pixel(width - 1, height - 1), Pixel(width - 1, 0));
	edges[3] = Line<Pixel>(Pixel(width - 1, 0), Pixel(0, 0));

	settings.SetScreenEdges(edges);
	settings.SetScreen(pixels);
	settings.SetSurface(surface);
	settings.SetWidth(width);
	settings.SetHeight(height);
}


