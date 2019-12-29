#pragma once

#include <SDL.h>

#include "BMath.h"

namespace Bagual
{
	namespace Camera
	{
		class BCameraSettings
		{
			int width;
			int height;

			BLine<BPixelPos> screenEdges[4];

			SDL_Surface *surface = nullptr;

			int *screen = nullptr;

		public:

			BCameraSettings() : width(320), height(240) {}

			int GetWidth() const { return width; }
			int GetHeight() const { return height; }
			int* GetScreen() const { return screen; }

			const BLine<BPixelPos>& GetScreenEdge(int i) const { return screenEdges[i]; }

			SDL_Surface* GetSurface() const { return surface; }

			void SetWidth(int width) { this->width = width; }
			void SetHeight(int height) { this->height = height; }
			void SetScreenEdges(BLine<BPixelPos> *edges)
			{
				screenEdges[0] = edges[0];
				screenEdges[1] = edges[1];
				screenEdges[2] = edges[2];
				screenEdges[3] = edges[3];
			}
			void SetSurface(SDL_Surface *surface) { this->surface = surface; }
			void SetScreen(int *screen) { this->screen = screen; }

		};
	}
}

