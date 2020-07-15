
#include "GraphicsDriverBase.h"

#include "CameraManager.h"

namespace bgl
{
	void BGraphicsDriverBase::RenderFrame()
	{
		auto cameras = BCameraManager::GetCameras();

		for (auto& camera : cameras)
		{
			RenderCamera(camera);
		}
	}

	void BGraphicsDriverBase::RenderCamera(std::shared_ptr<BCamera> camera)
	{

	}
}