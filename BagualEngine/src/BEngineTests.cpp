
#include "BEngineTests.h"

#include "BCameraManager.h"

namespace Bagual::Tests
{

	void BEngineTest_DrawRandomLines::Init()
	{
		camera = Bagual::Camera::BCameraManager::Create();

		if (camera)
		{
			camera->SetupScreen();
		}
	}

	void BEngineTest_DrawRandomLines::Tick()
	{
		if (camera)
		{
			camera->DrawScreen();
		}
	}

	void BEngineTest_DrawRandomLines::Term()
	{
		Bagual::Camera::BCameraManager::RemoveCamera(camera);
		camera = nullptr;
	}

}