
#include "Bagual.pch.h"

#include "CameraManager.h"

#include "Camera.h"
#include "Viewport.h"

namespace bgl
{
	BArray < std::shared_ptr<BCamera> > BCameraManager::cameras = BArray < std::shared_ptr<BCamera> >();

	std::shared_ptr<BCamera> BCameraManager::Create(const std::shared_ptr<BViewport>& viewport)
	{
		auto camera = std::make_shared<BCamera>(viewport);
		cameras.Add(camera);
		return camera;
	}

	void BCameraManager::RemoveCamera(std::shared_ptr<BCamera> camera)
	{
		cameras.Remove(camera);
	}

	BArray < std::shared_ptr<BCamera> > BCameraManagers = {};

	BArray<std::shared_ptr<BCamera>>& BCameraManager::GetCameras()
	{
		return cameras;
	}

}

