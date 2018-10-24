#pragma once

#include <vector>

#include "BCamera.h"

namespace Bagual
{
	namespace Camera
	{
		class BCameraManager
		{

			static std::vector<BCamera*> cameras;

		public:

			static void AddCamera(BCamera *camera);

			static void RemoveCamera(BCamera *camera);

			static std::vector<BCamera*>& GetCameras() { return cameras; }

		};
	}
}
