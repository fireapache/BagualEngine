#pragma once

#include "Common.h"

namespace bgl
{
	class BViewport;
}

namespace bgl
{
	class BCamera;
}

namespace bgl
{
	class BCameraManager
	{

		static BArray < std::shared_ptr<BCamera> > cameras;

	public:

		static std::shared_ptr<BCamera> Create(const std::shared_ptr<BViewport>& viewport);

		static void RemoveCamera(std::shared_ptr<BCamera> camera);

		static BArray<std::shared_ptr<BCamera>>& GetCameras();

	};
}
