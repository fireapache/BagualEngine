
#include "Bagual.pch.h"

#include "EngineTests.h"

#include "CameraManager.h"
#include "Settings.h"
#include "BagualEngine.h"
#include "PlatformBase.h"
#include "Viewport.h"
#include "GraphicsPlatform.h"

#include <imgui.h>

namespace bgl
{

	void BEngineTest_DrawRandomLines::Init()
	{
		auto& platform = Engine::Platform();

		BGL_ASSERT(platform != nullptr && "Not a valid platform object!");

		auto& graphicsDriver = Engine::GraphicsPlatform();

		BGL_ASSERT(graphicsDriver != nullptr && "Not a valid graphics driver object!");

		//FWindowSettings windowSettings;

		windowSettings.Title = "Bagual Engine Test #1 Window #1";
		windowSettings.width = 1280;
		windowSettings.height = 720;

		auto window = platform->CreateWindow(windowSettings);

		BGL_ASSERT(window != nullptr && "Could not create window!");

		// 
		auto guiTick = []()
		{
			ImGui::ShowDemoWindow();
		};

		// Gui update procedure
		window->SetGuiTickMethod(guiTick);

		// TODO: Test multi viewport Imgui support from https://github.com/morgoth990/imgui/tree/docking
		//windowSettings.Title = "Bagual Engine Test #1 Window #2";
		//window = platform->CreateWindow(windowSettings);
		//window->SetGuiTickMethod(guiTick);

		//return;

		auto& canvas = window->GetCanvas();

		//{
		//	//auto viewport = graphicsDriver->CreateViewport(canvas);
		//	auto viewport = graphicsDriver->CreateViewport(canvas, 20, 20, 250, 200);
		//	
		//	/*BBox<BVector2<float>> normalizedSize;
		//	normalizedSize.p1.x = 0.005f;
		//	normalizedSize.p1.y = 0.01f;
		//	normalizedSize.p2.x = 0.995f;
		//	normalizedSize.p2.y = 0.495f;
		//	auto viewport = graphicsDriver->CreateViewport(canvas, normalizedSize);*/
		//	
		//	BCameraManager::Create(viewport);
		//}
		//
		//{
		//	//auto viewport = graphicsDriver->CreateViewport(canvas);
		//	//auto viewport = graphicsDriver->CreateViewport(canvas, 10, 250, 400, 100);

		//	BBox<BVector2<float>> normalizedSize;
		//	normalizedSize.p1.x = 0.005f;
		//	normalizedSize.p1.y = 0.505f;
		//	normalizedSize.p2.x = 0.995f;
		//	normalizedSize.p2.y = 0.99f;
		//	auto viewport = graphicsDriver->CreateViewport(canvas, normalizedSize);

		//	BCameraManager::Create(viewport);
		//}

		{
			//auto viewport = graphicsDriver->CreateViewport(canvas);
			//auto viewport = graphicsDriver->CreateViewport(canvas, 10, 250, 400, 100);

			BBox<BVector2<float>> normalizedSize;
			normalizedSize.p1.x = 0.005f;
			normalizedSize.p1.y = 0.005f;
			normalizedSize.p2.x = 0.995f;
			normalizedSize.p2.y = 0.995f;
			auto viewport = graphicsDriver->CreateViewport(canvas, normalizedSize);

			BCameraManager::Create(viewport);
		}
		

	}

	void BEngineTest_DrawRandomLines::Tick()
	{
		auto& cameras = BCameraManager::GetCameras();

		for (auto& camera : cameras)
		{
			DrawCameraLine(camera);
			DrawCameraLine(camera);
			DrawCameraLine(camera);
			DrawCameraLine(camera);
			DrawCameraLine(camera);
			DrawCameraLine(camera);
			DrawCameraLine(camera);
			DrawCameraLine(camera);
		}
	}

	void BEngineTest_DrawRandomLines::DrawCameraLine(std::shared_ptr<BCamera>& camera)
	{
		if (camera)
		{
			int x1 = rand() % (windowSettings.width + 500) - 250;
			int y1 = rand() % (windowSettings.height + 500) - 250;
			int x2 = rand() % (windowSettings.width + 500) - 250;
			int y2 = rand() % (windowSettings.height + 500) - 250;

			BPixelPos p1(x1, y1);
			BPixelPos p2(x2, y2);

			camera->AddLine2DBuffer(BLine<BPixelPos>(p1, p2));
		}
	}

	void BEngineTest_DrawRandomLines::Term()
	{
		
	}

}