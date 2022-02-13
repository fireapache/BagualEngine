
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

	BPlatformWindow* BEngineTestBase::window = nullptr;
	BViewport* BEngineTestBase::viewport = nullptr;
	BCanvas* BEngineTestBase::canvas = nullptr;

	void BEngineTestBase::CreateStandardWindow(const char* windowTitle)
	{
		// Keeping the same window if already created by another standard test module
		if (window) return;

		// Creating application window
		windowSettings.Title = windowTitle ? windowTitle : "Bagual Engine Test";
		windowSettings.width = 1280;
		windowSettings.height = 720;

		window = BEngine::Platform().CreateWindow(windowSettings);

		BGL_ASSERT(window != nullptr && "Could not create window!");

		// Setting viewport and camera
		canvas = window->GetCanvas();
		viewport = BEngine::GraphicsPlatform().CreateViewport(canvas);
	}

#pragma region Fundamental Rendering

	void BEngineTest_FundamentalRendering::Init()
	{
		CreateStandardWindow("Bagual Engine Test #1 (Fundamental Rendering)");

		// Creating 2 simple triangles
		BArray<BTriangle<float>> tris;

		tris.Add(BTriangle<float>(
			BVec3f(-1.f, 1.f, -5.f),
			BVec3f(1.f, 1.f, -5.f),
			BVec3f(0.f, -1.f, -5.f)));
		tris.Add(BTriangle<float>(
			BVec3f(4.f, 1.f, -6.f),
			BVec3f(2.f, 1.f, -5.f),
			BVec3f(3.f, -1.f, -5.f)));

		auto trisNode = BEngine::Scene().CreateNode("SimpleTriangles");
		auto meshComp = trisNode->CreateComponent<BMeshComponent>("Triangles");
		meshComp->AddTriangles(tris);

		auto cameraNode = BEngine::Scene().CreateNode("Camera");
		auto cameraComp = cameraNode->CreateComponent<BCameraComponent>("CameraComp", viewport);
		auto camera = cameraComp->GetCamera();
		camera->SetRenderOutputType(BERenderOutputType::UvColor);
		camera->SetRenderSpeed(BERenderSpeed::Normal);

	}

	void BEngineTest_FundamentalRendering::Term()
	{

	}

	void BEngineTest_FundamentalRendering::Tick()
	{
		auto cameras = BCameraManager::GetCameras();

		for (auto camera : cameras)
		{
			if (camera)
			{
				camera->ClearLine2DBuffer();
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

		auto cameraNode = BEngine::Scene().CreateNode("Camera");
		cameraComp = cameraNode->CreateComponent<BCameraComponent>("CameraComp", viewport);
		auto camera = cameraComp->GetCamera();
		camera->SetLocation(BVec3f(-1.33f, 2.f, -4.f));
		camera->SetRotation(BVec3f(8.19f, 20.f, 0.f));
		camera->SetDepthDistance(800.f);
		camera->SetFOV(30.f);
		camera->SetRenderSpeed(BERenderSpeed::Fast);
		camera->SetRenderOutputType(BERenderOutputType::UvColor);

		defaultDepthDist = cameraComp->GetCamera()->GetDepthDistance();

		// Ways to access scene nodes
		auto rootNode = BEngine::Scene().GetRootNode();
		auto sceneNodes = BEngine::Scene().GetNodes();

		// Setting a ImGui window
		auto guiTick = [this]()
		{
			IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

			ImGuiViewport* main_viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkPos().x + 650, main_viewport->GetWorkPos().y + 20), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

			ImGuiWindowFlags window_flags = 0;
			if (!ImGui::Begin("Bagual Engine Test #2 Settings", nullptr, window_flags))
			{
				ImGui::End();
				return;
			}

			ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

			if (ImGui::Button("Restart Rendering"))
			{
				if (viewport)
				{
					viewport->ResetPixelDepth();
					BCanvas* canvas = viewport->GetCanvas();

					if (canvas)
					{
						canvas->GetColorBuffer().SetBufferValue(0);
					}
				}
			}

			auto camera = cameraComp->GetCamera();

			auto& renderOutputMode = camera->GetRenderOutputType_Mutable();
			const char* renderOutputOptions[] = { "Pixel Depth", "UV Color" };
			ImGui::Combo("Render Output", reinterpret_cast<int*>(&renderOutputMode), renderOutputOptions, IM_ARRAYSIZE(renderOutputOptions));

			const float positionRange = 10.f;
			BVec3f& camPos = cameraComp->GetTransform_Mutable().translation;
			ImGui::SliderFloat3("Camera Position", reinterpret_cast<float*>(&camPos), -positionRange, positionRange);

			const float rotRange = 20.f;
			BVec3f& camRot = cameraComp->GetTransform_Mutable().rotation;
			ImGui::SliderFloat3("Camera Rotation", reinterpret_cast<float*>(&camRot), -rotRange, rotRange);

			auto& depthDist = camera->GetDepthDistance_Mutable();
			const float depthDistRange = 500.f;
			ImGui::SliderFloat("Scene Depth", &depthDist, defaultDepthDist - depthDistRange, defaultDepthDist + depthDistRange);

			auto& sensorSize = camera->GetSensorSize_Mutable();
			ImGui::InputFloat2("Sensor Size", reinterpret_cast<float*>(&sensorSize));

			auto& renderMode = camera->GetIntrinsicsMode_Mutable();
			const char* renderModeOptions[] = { "Sequential", "AVX" };
			ImGui::Combo("Render Mode", reinterpret_cast<int*>(&renderMode), renderModeOptions, IM_ARRAYSIZE(renderModeOptions));

			auto& renderSpeed = camera->GetRenderSpeed_Mutable();
			const char* renderSpeedOptions[] = { "Normal", "Fast", "Very Fast" };
			ImGui::Combo("Render Speed", reinterpret_cast<int*>(&renderSpeed), renderSpeedOptions, IM_ARRAYSIZE(renderSpeedOptions));

			const char* sceneSetupOptions[] = { "Empty", "With Objects", "Objects and Character" };
			if (ImGui::Combo("Scene Setup", reinterpret_cast<int*>(&sceneSetup), sceneSetupOptions, IM_ARRAYSIZE(sceneSetupOptions)))
			{
				auto& sceneNodes = BEngine::Scene().GetNodes();

				for (auto& node : sceneNodes)
				{
					if (node->GetName() == "Objects")
					{
						node->SetHidden(sceneSetup < BESceneSetup::WithObjects);
					}
					else if (node->GetName() == "Character")
					{
						node->SetHidden(sceneSetup < BESceneSetup::ObjectsCharacter);
					}
				}

			}

			const float fovRange = 60.f;
			const float fovCenter = 90.f;
			ImGui::SliderFloat("Camera FOV", &camera->GetFOV_Mutable(), fovCenter - fovRange, fovCenter + fovRange);

			ImGui::End();
		};

		// Gui update procedure
		window->SetGuiTickFunc(guiTick);

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