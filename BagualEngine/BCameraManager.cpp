#include "BCameraManager.h"

namespace Bagual
{
	namespace Camera
	{
		void BCameraManager::AddCamera(BCamera *camera)
		{
			if (find(cameras.begin(), cameras.end(), camera) == cameras.end())
			{
				cameras.push_back(camera);
				camera->SetupScreen();
			}
		}

		void BCameraManager::RemoveCamera(BCamera *camera)
		{
			std::vector<BCamera*>::iterator it;
			it = std::find(cameras.begin(), cameras.end(), camera);
			if (it != cameras.end()) cameras.erase(it);
		}

		std::vector<BCamera*> BCameraManager::cameras = {};

	}
}

