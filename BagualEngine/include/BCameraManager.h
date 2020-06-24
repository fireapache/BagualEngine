#pragma once

#include "BCamera.h"
#include "BTypes.h"
#include <memory>

namespace Bagual
{
	namespace Camera
	{
		class BCameraManager
		{

			static Bagual::Types::BArray < std::shared_ptr<BCamera> > cameras;

		public:

			static std::shared_ptr<BCamera> Create();

			static void RemoveCamera(std::shared_ptr<BCamera> camera);

			static Bagual::Types::BArray<std::shared_ptr<BCamera>>& GetCameras();

		};
	}
}
