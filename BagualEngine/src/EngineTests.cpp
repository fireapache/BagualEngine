
#include "Bagual.pch.h"

#include "EngineTests.h"

#include "CameraManager.h"
#include "Settings.h"
#include "Engine.h"
#include "PlatformBase.h"
#include "Viewport.h"
#include "GraphicsPlatform.h"

namespace bgl
{

	void BEngineTest_DrawRandomLines::Init()
	{
		auto& platform = Engine::Platform();

		BGL_ASSERT(platform != nullptr && "Not a valid platform object!");

		auto& graphicsDriver = Engine::GraphicsPlatform();

		BGL_ASSERT(graphicsDriver != nullptr && "Not a valid graphics driver object!");

		auto window = platform->CreateWindow();
		
		BGL_ASSERT(window != nullptr && "Could not create window!");

		auto& canvas = window->GetCanvas();

		const FViewportSettings viewSettings(*canvas);

		auto viewport = graphicsDriver->CreateViewport(canvas, viewSettings);

		camera = BCameraManager::Create(viewport);

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