#pragma once

#include "BCamera.h"
#include "BTypes.h"
#include <memory>

namespace bgl
{
	class BCameraManager
	{

		static bgl::BArray < std::shared_ptr<BCamera> > cameras;

	public:

		static std::shared_ptr<BCamera> Create();

		static void RemoveCamera(std::shared_ptr<BCamera> camera);

		static bgl::BArray<std::shared_ptr<BCamera>>& GetCameras();

	};
}
