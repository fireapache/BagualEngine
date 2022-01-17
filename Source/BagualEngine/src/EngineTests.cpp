
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
#include <imgui_node_editor.h>
#include <imgui_node_editor_internal.h>

namespace bgl
{

	void BEngineTestBase::CreateStandardWindow(const char* windowTitle)
	{
		// Creating application window
		windowSettings.Title = windowTitle ? windowTitle : "Bagual Engine Test";
		windowSettings.width = 1280;
		windowSettings.height = 720;

		window = BEngine::Platform().CreateWindow(windowSettings);

		BGL_ASSERT(window != nullptr && "Could not create window!");

		// Setting viewport and camera
		auto& canvas = window->GetCanvas();
		auto viewport = BEngine::GraphicsPlatform().CreateViewport(canvas);
		BCameraManager::Create(viewport);
	}

#pragma region Fundamental Rendering

	void BEngineTest_FundamentalRendering::Init()
	{
		CreateStandardWindow("Bagual Engine Test #1 (Fundamental Rendering)");

		// Creating 2 simple triangles
		BArray<BTriangle<float>> tris;

		tris.Add(BTriangle<float>(
			BVec3f(-1.f, -1.f, -5.f),
			BVec3f(1.f, -1.f, -5.f),
			BVec3f(0.f, 1.f, -5.f)));
		tris.Add(BTriangle<float>(
			BVec3f(-4.f, -1.f, -6.f),
			BVec3f(-2.f, -1.f, -5.f),
			BVec3f(-3.f, 1.f, -5.f)));

		auto trisNode = BEngine::Scene().CreateNode("SimpleTriangles");
		auto meshComp = trisNode->CreateComponent<BMeshComponent>("Triangles");
		meshComp->AddTriangles(tris);

		BEngine::GraphicsPlatform().SetRenderOutputType(BERenderOutputType::UvColor);

	}

	void BEngineTest_FundamentalRendering::Term()
	{

	}

	void BEngineTest_FundamentalRendering::Tick()
	{
		auto& cameras = BCameraManager::GetCameras();

		for (auto& camera : cameras)
		{
			auto cameraPtr = camera.get();

			if (cameraPtr)
			{
				DrawCameraLine(cameraPtr);
				DrawCameraLine(cameraPtr);
				DrawCameraLine(cameraPtr);
				DrawCameraLine(cameraPtr);
				DrawCameraLine(cameraPtr);
				DrawCameraLine(cameraPtr);
				DrawCameraLine(cameraPtr);
				DrawCameraLine(cameraPtr);
			}
		}
	}

	void BEngineTest_FundamentalRendering::DrawCameraLine(BCamera* camera)
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

#pragma endregion

#pragma region Room Rendering

	void BEngineTest_RoomRendering::Init()
	{
		CreateStandardWindow("Bagual Engine Test #2 (Room Rendering)");

		// Creating scene nodes
		auto roomNode = BEngine::Scene().CreateNode("Room");
		auto objectsNode = BEngine::Scene().CreateNode("Objects");
		auto charNode = BEngine::Scene().CreateNode("Character");

		// Creating mesh components and loading geometry from disk
		roomNode->CreateComponent<BMeshComponent>("RoomMesh", "./assets/basemap/basemap.obj");
		objectsNode->CreateComponent<BMeshComponent>("ObjectsMesh", "./assets/basemap/basemap_objects.obj");
		charNode->CreateComponent<BMeshComponent>("CharMesh", "./assets/basemesh/basemesh.obj");

		// Ways to access scene nodes
		auto rootNode = BEngine::Scene().GetRootNode();
		auto sceneNodes = BEngine::Scene().GetNodes();

	}

	void BEngineTest_RoomRendering::Term()
	{
		
	}

#pragma endregion

#pragma region Node Editor

	void BEngineTest_NodeEditor::Init()
	{
		CreateStandardWindow("Bagual Engine Test #3 (Node Editor)");

		// Setting up ImGui node editor context
		nodeEditorContext = ImGuiNE::CreateEditor();

		// Setting up ImGui
		auto guiTick = [this]()
		{
			ImGuiNE::SetCurrentEditor(reinterpret_cast<ImGuiNE::EditorContext*>(nodeEditorContext));

			ImGuiNE::Begin("My Editor");

			int uniqueId = 1;

			// Start drawing nodes.
			ImGuiNE::BeginNode(uniqueId++);
			ImGui::Text("Node A");
			ImGuiNE::BeginPin(uniqueId++, ImGuiNE::PinKind::Input);
			ImGui::Text("-> In");
			ImGuiNE::EndPin();
			ImGui::SameLine();
			ImGuiNE::BeginPin(uniqueId++, ImGuiNE::PinKind::Output);
			ImGui::Text("Out ->");
			ImGuiNE::EndPin();
			ImGuiNE::EndNode();

			ImGuiNE::End();
		};

		window->SetGuiTickFunc(guiTick);

	}

	void BEngineTest_NodeEditor::Term()
	{
		ImGuiNE::DestroyEditor(reinterpret_cast<ImGuiNE::EditorContext*>(nodeEditorContext));
	}

#pragma endregion

}