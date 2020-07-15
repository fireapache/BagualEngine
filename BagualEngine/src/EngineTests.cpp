
#include "EngineTests.h"

#include "CameraManager.h"
#include "Settings.h"

namespace bgl
{

	void BEngineTest_DrawRandomLines::Init()
	{
		camera = BCameraManager::Create();

		if (camera)
		{
			
		}
	}

	void BEngineTest_DrawRandomLines::Tick()
	{
		if (camera)
		{
			int x1 = rand() % (Settings::width + 500) - 250;
			int y1 = rand() % (Settings::height + 500) - 250;
			int x2 = rand() % (Settings::width + 500) - 250;
			int y2 = rand() % (Settings::height + 500) - 250;

			BPixelPos p1(x1, y1);
			BPixelPos p2(x2, y2);

			camera->AddLine2DBuffer(BLine<BPixelPos>(p1, p2));
		}
	}

	void BEngineTest_DrawRandomLines::Term()
	{
		BCameraManager::RemoveCamera(camera);
		camera = nullptr;
	}

}