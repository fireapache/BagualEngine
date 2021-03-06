#pragma once

#include "Common.h"

namespace bgl
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
		static BLine<BPixelPos> screenEdges[4];
		static void* surface;
		static void* pixels;

	};

}