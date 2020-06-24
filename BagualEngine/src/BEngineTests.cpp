
#include "BEngineTests.h"

#include "BCameraManager.h"

namespace Bagual::Tests
{

	void BEngineTest_DrawRandomLines::Init()
	{
		camera = Bagual::Camera::BCameraManager::Create();

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

			Bagual::Types::BPixelPos p1(x1, y1);
			Bagual::Types::BPixelPos p2(x2, y2);

			camera->AddLine2DBuffer(Bagual::Types::BLine<Bagual::Types::BPixelPos>(p1, p2));
		}
	}

	void BEngineTest_DrawRandomLines::Term()
	{
		Bagual::Camera::BCameraManager::RemoveCamera(camera);
		camera = nullptr;
	}

}