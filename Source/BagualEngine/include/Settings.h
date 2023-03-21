#pragma once

#include "Common.h"

namespace bgl
{
	class BSettings
	{
		static uint32_t m_debugFlags;

	public:
		// Size of viewport
		static int width;
		static int height;

		static double simulationFrequency;
		static double renderingFrequency;
		
		// Current wireframe line drawing color
		static Color32Bit lineColor;
		
		static void setDebugFlags( const uint32_t flags )
		{
			m_debugFlags |= flags;
		}

		static bool isDebugFlagsSet( const uint32_t flags )
		{
			return m_debugFlags & flags;
		}
	};

} // namespace bgl