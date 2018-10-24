#pragma once

#include <vector>
#include <SDL2\SDL.h>

#include "BTypes.h"
#include "BCameraManager.h"
#include "BMath.h"

namespace Bagual
{
	namespace Renderer
	{
		class BRenderer
		{
			//static Renderer *instance;

			//Renderer() {}

		public:

			/*Renderer(const Renderer&) {}
			void operator=(Renderer&) {}

			static Renderer* Instance()
			{
				if (!instance) instance = new Renderer();
				return instance;
			}*/

			static void Render();

		};
	}
}



