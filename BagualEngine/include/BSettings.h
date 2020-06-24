#pragma once

#include "BTypes.h"

namespace Bagual
{

	class Settings
	{

	public:

		// Size of viewport
		static int width;
		static int height;

		// Pointer to the main engine window object
		static void *window;

		// Screen edges information 
		static Bagual::Types::BLine<Bagual::Types::BPixelPos> screenEdges[4];
		static void* surface;
		static void* pixels;

	};

}