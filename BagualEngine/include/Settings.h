#pragma once

#include "Types.h"

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
		static bgl::BLine<bgl::BPixelPos> screenEdges[4];
		static void* surface;
		static void* pixels;

	};

}