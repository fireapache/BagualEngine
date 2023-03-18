#pragma once

#include "Common.h"

namespace bgl
{
	class BSettings
	{
	public:
		// Size of viewport
		static int width;
		static int height;

		static double simulationFrequency;
		static double renderingFrequency;
		
		// Current wireframe line drawing color
		static Color32Bit lineColor;
	};

} // namespace bgl