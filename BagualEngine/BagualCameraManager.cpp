#include "BagualCameraManager.h"


void CameraManager::AddCamera(Camera *camera)
{
	if (find(cameras.begin(), cameras.end(), camera) == cameras.end())
	{
		cameras.push_back(camera);
		camera->SetupScreen();
	}
}

void CameraManager::RemoveCamera(Camera *camera)
{
	vector<Camera*>::iterator it;
	it = find(cameras.begin(), cameras.end(), camera);
	if (it != cameras.end()) cameras.erase(it);
}

vector<Camera*> CameraManager::cameras = {};
