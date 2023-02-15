
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
#include <Draw.h>

namespace bgl
{

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

		// Creating 8 simple triangles
		BArray<BTriangle<float>> tris;

		tris.Add(BTriangle<float>(
			BVec3f(-1.f, -0.25f, -5.f),
			BVec3f(1.f, -0.25f, -5.f),
			BVec3f(0.f, -2.25f, -5.f)));
		tris.Add(BTriangle<float>(
			BVec3f(-3.f, 1.f, -5.f),
			BVec3f(-1.f, 1.f, -5.f),
			BVec3f(-2.f, -1.f, -5.f)));
		tris.Add(BTriangle<float>(
			BVec3f(1.f, 1.f, -5.f),
			BVec3f(3.f, 1.f, -5.f),
			BVec3f(2.f, -1.f, -5.f)));
		tris.Add(BTriangle<float>(
			BVec3f(-1.f, 2.25f, -5.f),
			BVec3f(1.f, 2.25f, -5.f),
			BVec3f(0.f, 0.25f, -5.f)));

		tris.Add(BTriangle<float>(
			BVec3f(-1.f, -0.25f, -5.f),
			BVec3f(1.f, -0.25f, -5.f),
			BVec3f(0.f, -2.25f, -5.f)));
		tris.Add(BTriangle<float>(
			BVec3f(-3.f, 1.f, -5.f),
			BVec3f(-1.f, 1.f, -5.f),
			BVec3f(-2.f, -1.f, -5.f)));
		tris.Add(BTriangle<float>(
			BVec3f(1.f, 1.f, -5.f),
			BVec3f(3.f, 1.f, -5.f),
			BVec3f(2.f, -1.f, -5.f)));
		tris.Add(BTriangle<float>(
			BVec3f(-1.f, 2.25f, -5.f),
			BVec3f(1.f, 2.25f, -5.f),
			BVec3f(0.f, 0.25f, -5.f)));

		auto trisNode = BEngine::Scene().CreateNode("SimpleTriangles");
		auto meshComp = trisNode->CreateComponent<BMeshComponent>("Triangles");
		meshComp->AddTriangles(tris);

		auto cameraNode = BEngine::Scene().CreateNode("Camera");
		auto cameraComp = cameraNode->CreateComponent<BCameraComponent>("CameraComp", viewport);
		auto camera = cameraComp->GetCamera();
		camera->SetRenderOutputType(BERenderOutputType::UvColor);
		camera->SetRenderSpeed(BERenderSpeed::Normal);
		camera->SetIntrinsicsMode(BEIntrinsicsMode::AVX);
		//camera->SetRenderThreadMode(BERenderThreadMode::SingleThread);

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
				QueueCameraLineDraw(camera);
				QueueCameraLineDraw(camera);
				QueueCameraLineDraw(camera);
				QueueCameraLineDraw(camera);
				QueueCameraLineDraw(camera);
				QueueCameraLineDraw(camera);
				QueueCameraLineDraw(camera);
				QueueCameraLineDraw(camera);
			}
		}
	}

	void BEngineTest_FundamentalRendering::QueueCameraLineDraw(BCamera* camera)
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
		roomMeshComp = roomNode->CreateComponent<BMeshComponent>("RoomMesh", "./assets/basemap/basemap.obj");
		objectsMeshComp = objectsNode->CreateComponent<BMeshComponent>("ObjectsMesh", "./assets/basemap/basemap_objects.obj");
		charMeshComp = charNode->CreateComponent<BMeshComponent>("CharMesh", "./assets/basemesh/basemesh.obj");

		auto cameraNode = BEngine::Scene().CreateNode("Camera");
		cameraComp = cameraNode->CreateComponent<BCameraComponent>("CameraComp", viewport);
		camera = cameraComp->GetCamera();
		camera->SetLocation(BVec3f(-1.33f, 2.f, -4.f));
		camera->SetRotation(BRotf(8.19f, 20.f, 0.f));
		camera->SetDepthDistance(800.f);
		camera->SetFOV(30.f);
		camera->SetRenderSpeed(BERenderSpeed::Normal);
		camera->SetRenderOutputType(BERenderOutputType::UvColor);
		camera->SetIntrinsicsMode(BEIntrinsicsMode::AVX);

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

			ImGui::Checkbox("Show Room Wireframe", &(roomMeshComp->getShowWireframe_Mutable()));
			ImGui::SameLine();
			ImGui::Checkbox("Show Object Wireframe", &(objectsMeshComp->getShowWireframe_Mutable()));
			ImGui::SameLine();
			ImGui::Checkbox("Show Character Wireframe", &(charMeshComp->getShowWireframe_Mutable()));

			auto camera = cameraComp->GetCamera();

			auto& renderOutputMode = camera->GetRenderOutputType_Mutable();
			const char* renderOutputOptions[] = { "Pixel Depth", "UV Color" };
			ImGui::Combo("Render Output", reinterpret_cast<int*>(&renderOutputMode), renderOutputOptions, IM_ARRAYSIZE(renderOutputOptions));

			const float positionRange = 10.f;
			BVec3f& camPos = cameraComp->GetTransform_Mutable().translation;
			ImGui::SliderFloat3("Camera Position", reinterpret_cast<float*>(&camPos), -positionRange, positionRange);

			const float rotRange = 20.f;
			BRotf& camRot = cameraComp->GetTransform_Mutable().rotation;
			ImGui::SliderFloat3("Camera Rotation", reinterpret_cast<float*>(&camRot), -rotRange, rotRange);

			auto& depthDist = camera->GetDepthDistance_Mutable();
			const float depthDistRange = 500.f;
			ImGui::SliderFloat("Scene Depth", &depthDist, defaultDepthDist - depthDistRange, defaultDepthDist + depthDistRange);

			auto& sensorSize = camera->GetSensorSize_Mutable();
			ImGui::InputFloat2("Sensor Size", reinterpret_cast<float*>(&sensorSize));

			auto& renderThreadMode = camera->GetRenderThreadMode_Mutable();
			const char* renderThreadOptions[] = { "Single Thread", "Multi Thread", "Hyper Thread"};
			ImGui::Combo("Render Thread Mode", reinterpret_cast<int*>(&renderThreadMode), renderThreadOptions, IM_ARRAYSIZE(renderThreadOptions));

			auto& renderMode = camera->GetIntrinsicsMode_Mutable();
			const char* renderModeOptions[] = { "Off", "AVX" };
			ImGui::Combo("Intrinsics", reinterpret_cast<int*>(&renderMode), renderModeOptions, IM_ARRAYSIZE(renderModeOptions));

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

	void BEngineTest_RoomRendering::Tick()
	{
		
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

#pragma region Cube Projection

	void BEngineTest_CubeProjection::Init()
	{
		CreateStandardWindow("Bagual Engine Test #4 (Cube Projection)");

		// Creating scene nodes
		auto cubeNode = BEngine::Scene().CreateNode("Cube");

		// Creating mesh components and loading geometry from disk
		cubeNode->CreateComponent<BMeshComponent>("CubeMesh", "./assets/basemap/basemap_cube.obj");

		auto cameraNode = BEngine::Scene().CreateNode("Camera");
		cameraComp = cameraNode->CreateComponent<BCameraComponent>("CameraComp", viewport);
		camera = cameraComp->GetCamera();
		camera->SetLocation(BVec3f(0.f, 0.f, 0.f));
		camera->SetRotation(BRotf(0.f, 0.f, 0.f));
		camera->SetDepthDistance(800.f);
		camera->SetFOV(60.f);
		camera->SetRenderSpeed(BERenderSpeed::Normal);
		camera->SetRenderOutputType(BERenderOutputType::Depth);
		camera->SetIntrinsicsMode(BEIntrinsicsMode::AVX);

		points[0] = BVec3f(1.f, 1.f, 5.f);
		points[1] = BVec3f(-1.f, 1.f, 5.f);

		points[2] = BVec3f(1.f, -1.f, 5.f);
		points[3] = BVec3f(-1.f, -1.f, 5.f);

		points[4] = BVec3f(1.f, 1.f, 7.f);
		points[5] = BVec3f(-1.f, 1.f, 7.f);

		points[6] = BVec3f(1.f, -1.f, 7.f);
		points[7] = BVec3f(-1.f, -1.f, 7.f);

		auto guiTick = [this]()
		{
			IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

			ImGuiViewport* main_viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkPos().x + 650, main_viewport->GetWorkPos().y + 20), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

			ImGuiWindowFlags window_flags = 0;
			if (!ImGui::Begin("Bagual Engine Test #4 Settings", nullptr, window_flags))
			{
				ImGui::End();
				return;
			}

			ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

			const float positionRange = 10.f;
			BVec3f& camPos = cameraComp->GetTransform_Mutable().translation;
			ImGui::SliderFloat3("Camera Position", reinterpret_cast<float*>(&camPos), -positionRange, positionRange);

			const float rotRange = 20.f;
			BRotf& camRot = cameraComp->GetTransform_Mutable().rotation;
			ImGui::SliderFloat3("Camera Rotation", reinterpret_cast<float*>(&camRot), -rotRange, rotRange);

			ImGui::End();

		};

		// Gui update procedure
		window->SetGuiTickFunc(guiTick);

	}

	void BEngineTest_CubeProjection::Tick()
	{
		//if (camera)
		//{
		//	BPixelPos pp[8];
		//	bool bPp[8];

		//	bPp[0] = BDraw::ProjectPoint(viewport, points[0], pp[0]);
		//	bPp[1] = BDraw::ProjectPoint(viewport, points[1], pp[1]);
		//	bPp[2] = BDraw::ProjectPoint(viewport, points[2], pp[2]);
		//	bPp[3] = BDraw::ProjectPoint(viewport, points[3], pp[3]);

		//	bPp[4] = BDraw::ProjectPoint(viewport, points[4], pp[4]);
		//	bPp[5] = BDraw::ProjectPoint(viewport, points[5], pp[5]);
		//	bPp[6] = BDraw::ProjectPoint(viewport, points[6], pp[6]);
		//	bPp[7] = BDraw::ProjectPoint(viewport, points[7], pp[7]);

		//	if (bPp[0] && bPp[1]) BDraw::DrawLine(viewport, pp[0], pp[1]);
		//	if (bPp[2] && bPp[3]) BDraw::DrawLine(viewport, pp[2], pp[3]);
		//	if (bPp[0] && bPp[2]) BDraw::DrawLine(viewport, pp[0], pp[2]);
		//	if (bPp[1] && bPp[3]) BDraw::DrawLine(viewport, pp[1], pp[3]);

		//	if (bPp[4] && bPp[5]) BDraw::DrawLine(viewport, pp[4], pp[5]);
		//	if (bPp[6] && bPp[7]) BDraw::DrawLine(viewport, pp[6], pp[7]);
		//	if (bPp[4] && bPp[6]) BDraw::DrawLine(viewport, pp[4], pp[6]);
		//	if (bPp[5] && bPp[7]) BDraw::DrawLine(viewport, pp[5], pp[7]);

		//	if (bPp[0] && bPp[4]) BDraw::DrawLine(viewport, pp[0], pp[4]);
		//	if (bPp[1] && bPp[5]) BDraw::DrawLine(viewport, pp[1], pp[5]);
		//	if (bPp[2] && bPp[6]) BDraw::DrawLine(viewport, pp[2], pp[6]);
		//	if (bPp[3] && bPp[7]) BDraw::DrawLine(viewport, pp[3], pp[7]);
		//}
	}

	void BEngineTest_CubeProjection::Term()
	{

	}

#pragma endregion	

}