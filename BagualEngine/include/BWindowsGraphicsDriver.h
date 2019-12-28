#pragma once

#include "BIGraphicsDriver.h"
#include "BGraphicsDriverBase.h"

#include <vector>
#include <SDL.h>

#include "BTypes.h"
#include "BCameraManager.h"
#include "BMath.h"

namespace Bagual
{
	namespace GraphicsDriver
	{
		class BWindowsGraphicsDriver : public BGraphicsDriverBase
		{

		public:

			BWindowsGraphicsDriver();
			~BWindowsGraphicsDriver();

			void Render();
			virtual void Delay(unsigned int ms);

		};
	}
}



