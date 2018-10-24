#pragma once

#include <SDL2\SDL.h>

#include "BTypes.h"
#include "BSettings.h"
#include "BCameraSettings.h"
#include "BDraw.h"

namespace Bagual
{
	namespace Camera
	{
		class BCamera
		{
			BCameraSettings settings;

		public:

			void DrawScreen();

			void SetupScreen();

		};

	}
}