#include "BagualCameraManager.h"


void CameraManager::AddCamera(Camera *camera)
{
	bool found = false;

	for (const auto& cam : cameras)
	{
		if (cam == camera)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		cameras.push_back(camera);
		camera->SetupScreen();
	}
}

void CameraManager::RemoveCamera(Camera *camera)
{
	auto itr = cameras.begin();

	while (itr != cameras.end())
	{
		if (*itr == camera)
		{
			cameras.erase(itr);
		}

		itr++;
	}
}

vector<Camera*> CameraManager::cameras = {};
