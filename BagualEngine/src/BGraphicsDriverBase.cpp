
#include "BGraphicsDriverBase.h"

#include "BCamera.h"
#include "BCameraManager.h"

namespace bgl
{

	void BGraphicsDriverBase::RenderFrame()
	{
		auto cameras = bgl::BCameraManager::GetCameras();

		for (auto& camera : cameras)
		{
			RenderCamera(camera);
		}
	}

	void BGraphicsDriverBase::RenderCamera(std::shared_ptr<bgl::BCamera> camera)
	{

	}
	

}