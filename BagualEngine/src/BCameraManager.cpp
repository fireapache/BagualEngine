#include "BCameraManager.h"

namespace Bagual::Camera
{
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

	Bagual::Types::BArray < std::shared_ptr<BCamera> > BCameraManager::cameras = {};

	Bagual::Types::BArray<std::shared_ptr<BCamera>>& BCameraManager::GetCameras()
	{
		return cameras;
	}

}

