
#include "BGraphicsDriverBase.h"

#include "BCamera.h"
#include "BCameraManager.h"

namespace Bagual::Graphics
{

	void BGraphicsDriverBase::RenderFrame()
	{
		auto cameras = Bagual::Camera::BCameraManager::GetCameras();

		for (auto& camera : cameras)
		{
			RenderCamera(camera);
		}
	}

	void BGraphicsDriverBase::RenderCamera(std::shared_ptr<Bagual::Camera::BCamera> camera)
	{

	}
	

}