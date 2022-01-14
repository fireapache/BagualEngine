
#include "Bagual.pch.h"

#include "EngineTests.h"

#include "CameraManager.h"
#include "Settings.h"
#include "BagualEngine.h"
#include "PlatformBase.h"
#include "Viewport.h"
#include "GraphicsPlatform.h"
#include "Scene.h"

#include <imgui.h>

namespace bgl
{

	void BEngineTest_BaseRendering::Init()
	{
		// Creating application window
		windowSettings.Title = "Bagual Engine Test #1 Window #1";
		windowSettings.width = 1280;
		windowSettings.height = 720;

		auto window = BEngine::Platform().CreateWindow(windowSettings);

		BGL_ASSERT(window != nullptr && "Could not create window!");

		// Setting up ImGui
		auto guiTick = []()
		{
			ImGui::ShowDemoWindow();
		};

		window->SetGuiTickMethod(guiTick);

		// Setting viewport and camera
		auto& canvas = window->GetCanvas();
		auto viewport = BEngine::GraphicsPlatform().CreateViewport(canvas);
		BCameraManager::Create(viewport);

		// Creating scene nodes
		auto roomNode = BEngine::Scene().CreateNode("Room");
		auto objectsNode = BEngine::Scene().CreateNode("Objects");
		auto charNode = BEngine::Scene().CreateNode("Character");

		// Creating mesh components and loading geometry from disk
		roomNode->CreateComponent<BMeshComponent>("RoomMesh", "./assets/basemap/basemap.obj");
		objectsNode->CreateComponent<BMeshComponent>("ObjectsMesh", "./assets/basemap/basemap_objects.obj");
		charNode->CreateComponent<BMeshComponent>("ObjectsMesh", "./assets/basemesh/basemesh.obj");

		// Ways to access scene nodes
		auto rootNode = BEngine::Scene().GetRootNode();
		auto sceneNodes = BEngine::Scene().GetNodes();

	}

	void BEngineTest_BaseRendering::Tick()
	{
		//auto& cameras = BCameraManager::GetCameras();

		//for (auto& camera : cameras)
		//{
		//	DrawCameraLine(camera);
		//	DrawCameraLine(camera);
		//	DrawCameraLine(camera);
		//	DrawCameraLine(camera);
		//	DrawCameraLine(camera);
		//	DrawCameraLine(camera);
		//	DrawCameraLine(camera);
		//	DrawCameraLine(camera);
		//}
	}

	void BEngineTest_BaseRendering::DrawCameraLine(std::shared_ptr<BCamera>& camera)
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

	void BEngineTest_BaseRendering::Term()
	{
		
	}

}