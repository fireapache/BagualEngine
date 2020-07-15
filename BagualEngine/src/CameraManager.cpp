
#include "CameraManager.h"

#include "Camera.h"

namespace bgl
{
	BArray < std::shared_ptr<BCamera> > BCameraManager::cameras = BArray < std::shared_ptr<BCamera> >();

	std::shared_ptr<BCamera> BCameraManager::Create()
	{
		auto camera = std::make_shared<BCamera>();
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

