
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "EngineTests.h"

#include "BagualEngine.h"
#include "CameraManager.h"
#include "GraphicsPlatform.h"
#include "PlatformBase.h"
#include "Scene.h"
#include "Viewport.h"
#include <Draw.h>
#include <imgui.h>
#include <imgui_node_editor.h>
#include <imgui_node_editor_internal.h>

namespace bgl
{
	void BEngineTestBase::CreateTestWindowAndScene()
	{
		const auto nWindows = BEngine::Platform().getWindows().size();

		if( nWindows > 0 )
		{
			const auto foundWindow = BEngine::Platform().getWindows().first();
			window = foundWindow ? foundWindow->get() : nullptr;
			canvas = window ? window->getCanvas() : nullptr;
			const auto foundViewport = canvas ? canvas->getViewports().first() : nullptr;
			viewport = foundViewport ? *foundViewport : nullptr;

			auto& scene = BEngine::Scene();
			cameraNode = scene.getNode( "CameraNode" );
			cameraComp = cameraNode ? dynamic_cast< BCameraComponent* >( cameraNode->getComponent( "CameraComp" ) ) : nullptr;
			camera = cameraComp ? cameraComp->getCamera() : nullptr;

			return;
		}

		FWindowSettings windowSettings{ BGL_WINDOW_CENTRALIZED, BGL_WINDOW_CENTRALIZED, 1280, 720, "Bagual Engine Test" };

		window = BEngine::Platform().createWindow( windowSettings );

		BGL_ASSERT( window != nullptr && "Could not create window!" );

		canvas = window->getCanvas();
		viewport = BEngine::GraphicsPlatform().CreateViewport( canvas );

		auto& scene = BEngine::Scene();
		cameraNode = scene.addNode( "CameraNode" );
		cameraComp = scene.addComponent< BCameraComponent >( cameraNode, "CameraComp", viewport );
		camera = cameraComp->getCamera();
	}

#pragma region Window& Scene

	void BEngineTest_WindowAndScene::init()
	{
		CreateTestWindowAndScene();
	}

	void BEngineTest_WindowAndScene::destroy()
	{
	}

#pragma endregion

#pragma region Fundamental Rendering

	void BEngineTest_FundamentalRendering::init()
	{
		CreateTestWindowAndScene();

		// Creating 8 simple triangles
		BArray< BTriangle< float > > tris;

		constexpr float z = 5.f;

		tris.add( BTriangle< float >( BVec3f( -1.f, -0.25f, z ), BVec3f( 1.f, -0.25f, z ), BVec3f( 0.f, -2.25f, z ) ) );
		tris.add( BTriangle< float >( BVec3f( -3.f, 1.f, z ), BVec3f( -1.f, 1.f, z ), BVec3f( -2.f, -1.f, z ) ) );
		tris.add( BTriangle< float >( BVec3f( 1.f, 1.f, z ), BVec3f( 3.f, 1.f, z ), BVec3f( 2.f, -1.f, z ) ) );
		tris.add( BTriangle< float >( BVec3f( -1.f, 2.25f, z ), BVec3f( 1.f, 2.25f, z ), BVec3f( 0.f, 0.25f, z ) ) );

		tris.add( BTriangle< float >( BVec3f( -1.f, -0.25f, z ), BVec3f( 1.f, -0.25f, z ), BVec3f( 0.f, -2.25f, z ) ) );
		tris.add( BTriangle< float >( BVec3f( -3.f, 1.f, z ), BVec3f( -1.f, 1.f, z ), BVec3f( -2.f, -1.f, z ) ) );
		tris.add( BTriangle< float >( BVec3f( 1.f, 1.f, z ), BVec3f( 3.f, 1.f, z ), BVec3f( 2.f, -1.f, z ) ) );
		tris.add( BTriangle< float >( BVec3f( -1.f, 2.25f, z ), BVec3f( 1.f, 2.25f, z ), BVec3f( 0.f, 0.25f, z ) ) );

		auto& scene = BEngine::Scene();

		trisNode = scene.addNode( "SimpleTriangles" );
		auto meshComp = scene.addComponent< BMeshComponent >( trisNode, "Triangles" );
		meshComp->addTriangles( tris );
		meshComp->setShowWireframe( true );

		if( cameraNode )
		{
			cameraNode->setLocation( { 0.f, 0.f, 0.f } );
			cameraNode->setRotation( { 0.f, 0.f, 0.f } );
		}

		if( camera )
		{
			camera->SetRenderOutputType( BERenderOutputType::UvColor );
			camera->SetRenderSpeed( BERenderSpeed::Normal );
			camera->SetIntrinsicsMode( BEIntrinsicsMode::AVX );
			//camera->SetRenderThreadMode(BERenderThreadMode::SingleThread);
		}
	}

	void BEngineTest_FundamentalRendering::destroy()
	{
	}

	void BEngineTest_FundamentalRendering::tick()
	{
		auto cameras = BCameraManager::GetCameras();

		for( auto camera : cameras )
		{
			if( camera )
			{
				QueueCameraLineDraw( camera );
				QueueCameraLineDraw( camera );
				QueueCameraLineDraw( camera );
				QueueCameraLineDraw( camera );
				QueueCameraLineDraw( camera );
				QueueCameraLineDraw( camera );
				QueueCameraLineDraw( camera );
				QueueCameraLineDraw( camera );
			}
		}
	}

	bool BEngineTest_FundamentalRendering::initialized() const
	{
		return trisNode;
	}

	void BEngineTest_FundamentalRendering::QueueCameraLineDraw( BCamera* camera )
	{
		if( camera && canvas )
		{
			const int32_t width = canvas->getWidth();
			const int32_t height = canvas->getHeight();
			const int x1 = rand() % ( width + 500 ) - 250;
			const int y1 = rand() % ( height + 500 ) - 250;
			const int x2 = rand() % ( width + 500 ) - 250;
			const int y2 = rand() % ( height + 500 ) - 250;

			const BPixelPos p1( x1, y1 );
			const BPixelPos p2( x2, y2 );

			camera->AddLine2DBuffer( BLine< BPixelPos >( p1, p2 ) );
		}
	}

#pragma endregion

#pragma region Room Rendering

	void BEngineTest_RoomRendering::init()
	{
		CreateTestWindowAndScene();

		auto& scene = BEngine::Scene();

		roomRootNode = scene.addNode( "Room Rendering Root" );

		// Creating scene nodes
		auto roomNode = scene.addNode( *roomRootNode, "Room" );
		auto objectsNode = scene.addNode( *roomRootNode, "Objects" );
		auto charNode = scene.addNode( *roomRootNode, "Character" );

		// Creating mesh components and loading geometry from disk
		roomMeshComp = scene.addComponent< BMeshComponent >( roomNode, "RoomMesh", "./assets/basemap/basemap.obj" );
		objectsMeshComp
			= scene.addComponent< BMeshComponent >( objectsNode, "ObjectsMesh", "./assets/basemap/basemap_objects.obj" );
		charMeshComp = scene.addComponent< BMeshComponent >( charNode, "CharMesh", "./assets/basemesh/basemesh.obj" );

		camera->SetLocation( BVec3f( -1.33f, 2.f, -4.f ) );
		camera->SetRotation( BRotf( 8.19f, 20.f, 0.f ) );
		camera->SetDepthDistance( 800.f );
		camera->SetFOV( 30.f );
		camera->SetRenderSpeed( BERenderSpeed::Normal );
		camera->SetRenderOutputType( BERenderOutputType::UvColor );
		camera->SetIntrinsicsMode( BEIntrinsicsMode::AVX );

		defaultDepthDist = cameraComp->getCamera()->GetDepthDistance();

		// Ways to access scene nodes
		auto rootNode = BEngine::Scene().getRootNode();
		auto& sceneNodes = BEngine::Scene().getNodes();

		// Setting a ImGui window
		guiTickFunc = [ this ]()
		{
			IM_ASSERT( ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!" );

			ImGuiViewport* main_viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(
				ImVec2( main_viewport->GetWorkPos().x + 650, main_viewport->GetWorkPos().y + 20 ),
				ImGuiCond_FirstUseEver );
			ImGui::SetNextWindowSize( ImVec2( 550, 680 ), ImGuiCond_FirstUseEver );

			ImGuiWindowFlags window_flags = 0;
			if( !ImGui::Begin( "Room Rendering", nullptr, window_flags ) )
			{
				ImGui::End();
				return;
			}

			ImGui::PushItemWidth( ImGui::GetFontSize() * -12 );

			if( ImGui::Button( "Restart Rendering" ) )
			{
				if( viewport )
				{
					viewport->ResetPixelDepth();
					BCanvas* canvas = viewport->GetCanvas();

					if( canvas )
					{
						canvas->getColorBuffer().SetBufferValue( 0 );
					}
				}
			}

			if( roomMeshComp )
			{
				if( ImGui::Checkbox( "Show Room Wireframe", &( wireframes[ 0 ] ) ) )
				{
					roomMeshComp->setShowWireframe( wireframes[ 0 ] );
				}
			}

			if( objectsMeshComp )
			{
				ImGui::SameLine();
				if( ImGui::Checkbox( "Show Object Wireframe", &( wireframes[ 1 ] ) ) )
				{
					objectsMeshComp->setShowWireframe( wireframes[ 1 ] );
				}
			}

			if( charMeshComp )
			{
				ImGui::SameLine();
				if( ImGui::Checkbox( "Show Character Wireframe", &( wireframes[ 2 ] ) ) )
				{
					charMeshComp->setShowWireframe( wireframes[ 2 ] );
				}
			}

			if( cameraComp && cameraComp->getCamera() )
			{
				BCamera* camera = cameraComp->getCamera();

				auto& renderOutputMode = camera->GetRenderOutputType_Mutable();
				const char* renderOutputOptions[] = { "Pixel Depth", "UV Color" };
				ImGui::Combo(
					"Render Output",
					reinterpret_cast< int* >( &renderOutputMode ),
					renderOutputOptions,
					IM_ARRAYSIZE( renderOutputOptions ) );

				const float positionRange = 10.f;
				BVec3f& camPos = cameraComp->getTransform_mutable().translation;
				ImGui::SliderFloat3( "Camera Position", reinterpret_cast< float* >( &camPos ), -positionRange, positionRange );

				const float rotRange = 20.f;
				BRotf& camRot = cameraComp->getTransform_mutable().rotation;
				ImGui::SliderFloat3( "Camera Rotation", reinterpret_cast< float* >( &camRot ), -rotRange, rotRange );

				auto& depthDist = camera->GetDepthDistance_Mutable();
				const float depthDistRange = 500.f;
				ImGui::SliderFloat(
					"Scene Depth",
					&depthDist,
					defaultDepthDist - depthDistRange,
					defaultDepthDist + depthDistRange );

				auto& sensorSize = camera->GetSensorSize_Mutable();
				ImGui::InputFloat2( "Sensor Size", reinterpret_cast< float* >( &sensorSize ) );

				auto& renderThreadMode = camera->GetRenderThreadMode_Mutable();
				const char* renderThreadOptions[] = { "Single Thread", "Multi Thread", "Hyper Thread" };
				ImGui::Combo(
					"Render Thread Mode",
					reinterpret_cast< int* >( &renderThreadMode ),
					renderThreadOptions,
					IM_ARRAYSIZE( renderThreadOptions ) );

				auto& renderMode = camera->GetIntrinsicsMode_Mutable();
				const char* renderModeOptions[] = { "Off", "AVX" };
				ImGui::Combo(
					"Intrinsics",
					reinterpret_cast< int* >( &renderMode ),
					renderModeOptions,
					IM_ARRAYSIZE( renderModeOptions ) );

				auto& renderSpeed = camera->GetRenderSpeed_Mutable();
				const char* renderSpeedOptions[] = { "Normal", "Fast", "Very Fast" };
				ImGui::Combo(
					"Render Speed",
					reinterpret_cast< int* >( &renderSpeed ),
					renderSpeedOptions,
					IM_ARRAYSIZE( renderSpeedOptions ) );
			}

			const char* sceneSetupOptions[] = { "Empty", "With Objects", "Objects and Character" };
			if( ImGui::Combo(
					"Scene Setup",
					reinterpret_cast< int* >( &sceneSetup ),
					sceneSetupOptions,
					IM_ARRAYSIZE( sceneSetupOptions ) ) )
			{
				auto& sceneNodes = BEngine::Scene().getNodes();

				for( auto& node : sceneNodes )
				{
					if( node->getName() == "Objects" )
					{
						node->setHidden( sceneSetup < BESceneSetup::WithObjects );
					}
					else if( node->getName() == "Character" )
					{
						node->setHidden( sceneSetup < BESceneSetup::ObjectsCharacter );
					}
				}
			}

			constexpr float fovRange = 60.f;
			constexpr float fovCenter = 90.f;
			constexpr float leftRange = fovCenter - fovRange;
			constexpr float rightRange = fovCenter + fovRange;
			ImGui::SliderFloat( "Camera FOV", &camera->GetFOV_Mutable(), leftRange, rightRange );

			ImGui::End();
		};

		// Gui update procedure
		BEngine::Instance().registerGuiTickFunc( &guiTickFunc );
	}

	void BEngineTest_RoomRendering::tick()
	{
	}

	bool BEngineTest_RoomRendering::initialized() const
	{
		return roomMeshComp;
	}

	void BEngineTest_RoomRendering::destroy()
	{
		BEngine::Instance().unregisterGuiTickFunc( &guiTickFunc );
		auto& scene = BEngine::Scene();
		scene.deleteNode( roomRootNode );
		roomRootNode = nullptr;
		roomMeshComp = nullptr;
		objectsMeshComp = nullptr;
		charMeshComp = nullptr;
	}

#pragma endregion

#pragma region Node Editor

	void BEngineTest_NodeEditor::init()
	{
		CreateTestWindowAndScene();

		// Setting up ImGui node editor context
		nodeEditorContext = ImGuiNE::CreateEditor();

		// Setting up ImGui
		guiTickFunc = [ this ]()
		{
			ImGuiNE::SetCurrentEditor( static_cast< ImGuiNE::EditorContext* >( nodeEditorContext ) );

			ImGuiNE::Begin( "My Editor" );

			int uniqueId = 1;

			// Start drawing nodes.
			ImGuiNE::BeginNode( uniqueId++ );
			ImGui::Text( "Node A" );
			ImGuiNE::BeginPin( uniqueId++, ImGuiNE::PinKind::Input );
			ImGui::Text( "-> In" );
			ImGuiNE::EndPin();
			ImGui::SameLine();
			ImGuiNE::BeginPin( uniqueId++, ImGuiNE::PinKind::Output );
			ImGui::Text( "Out ->" );
			ImGuiNE::EndPin();
			ImGuiNE::EndNode();

			ImGuiNE::End();
		};

		BEngine::Instance().registerGuiTickFunc( &guiTickFunc );
	}

	bool BEngineTest_NodeEditor::initialized() const
	{
		return nodeEditorContext;
	}

	void BEngineTest_NodeEditor::destroy()
	{
		ImGuiNE::DestroyEditor( static_cast< ImGuiNE::EditorContext* >( nodeEditorContext ) );
	}

#pragma endregion

#pragma region Cube Projection

	void BEngineTest_CubeProjection::init()
	{
		CreateTestWindowAndScene();

		auto& scene = BEngine::Scene();

		// Creating scene nodes
		cubeNode = BEngine::Scene().addNode( "Cube" );

		// Creating mesh components and loading geometry from disk
		scene.addComponent< BMeshComponent >( cubeNode, "CubeMesh", "./assets/basemap/basemap_cube.obj" );

		camera->SetLocation( BVec3f( 0.f, 0.f, 0.f ) );
		camera->SetRotation( BRotf( 0.f, 0.f, 0.f ) );
		camera->SetDepthDistance( 800.f );
		camera->SetFOV( 60.f );
		camera->SetRenderSpeed( BERenderSpeed::Normal );
		camera->SetRenderOutputType( BERenderOutputType::Depth );
		camera->SetIntrinsicsMode( BEIntrinsicsMode::AVX );

		points[ 0 ] = BVec3f( 1.f, 1.f, 5.f );
		points[ 1 ] = BVec3f( -1.f, 1.f, 5.f );

		points[ 2 ] = BVec3f( 1.f, -1.f, 5.f );
		points[ 3 ] = BVec3f( -1.f, -1.f, 5.f );

		points[ 4 ] = BVec3f( 1.f, 1.f, 7.f );
		points[ 5 ] = BVec3f( -1.f, 1.f, 7.f );

		points[ 6 ] = BVec3f( 1.f, -1.f, 7.f );
		points[ 7 ] = BVec3f( -1.f, -1.f, 7.f );

		guiTickFunc = [ this ]()
		{
			IM_ASSERT( ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!" );

			ImGuiViewport* main_viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(
				ImVec2( main_viewport->GetWorkPos().x + 650, main_viewport->GetWorkPos().y + 20 ),
				ImGuiCond_FirstUseEver );
			ImGui::SetNextWindowSize( ImVec2( 550, 680 ), ImGuiCond_FirstUseEver );

			ImGuiWindowFlags window_flags = 0;
			if( !ImGui::Begin( "Bagual Engine Test #4 Settings", nullptr, window_flags ) )
			{
				ImGui::End();
				return;
			}

			ImGui::PushItemWidth( ImGui::GetFontSize() * -12 );

			const float positionRange = 10.f;
			BVec3f& camPos = cameraComp->getTransform_mutable().translation;
			ImGui::SliderFloat3( "Camera Position", reinterpret_cast< float* >( &camPos ), -positionRange, positionRange );

			const float rotRange = 20.f;
			BRotf& camRot = cameraComp->getTransform_mutable().rotation;
			ImGui::SliderFloat3( "Camera Rotation", reinterpret_cast< float* >( &camRot ), -rotRange, rotRange );

			ImGui::End();
		};

		// Gui update procedure
		BEngine::Instance().registerGuiTickFunc( &guiTickFunc );
	}

	void BEngineTest_CubeProjection::tick()
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

	bool BEngineTest_CubeProjection::initialized() const
	{
		return cubeNode;
	}

	void BEngineTest_CubeProjection::destroy()
	{
	}

#pragma endregion

#pragma region AABB Tests

	void BEngineTest_AABBTests::init()
	{
		CreateTestWindowAndScene();

		auto& scene = BEngine::Scene();

		// Creating scene nodes
		cubeNode = BEngine::Scene().addNode( "Cube" );

		// Creating mesh components and loading geometry from disk
		cubeMeshComp = scene.addComponent< BMeshComponent >( cubeNode, "CubeMesh", "./assets/basemap/basemap_cube.obj" );

		camera->SetLocation( BVec3f( 0.f, 0.f, 0.f ) );
		camera->SetRotation( BRotf( 0.f, 0.f, 0.f ) );
		camera->SetDepthDistance( 800.f );
		camera->SetFOV( 60.f );
		camera->SetRenderSpeed( BERenderSpeed::Normal );
		camera->SetRenderOutputType( BERenderOutputType::Depth );
		camera->SetIntrinsicsMode( BEIntrinsicsMode::AVX );

		BCubef cube;

		guiTickFunc = [ this ]()
		{
			IM_ASSERT( ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!" );

			ImGuiViewport* main_viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(
				ImVec2( main_viewport->GetWorkPos().x + 650, main_viewport->GetWorkPos().y + 20 ),
				ImGuiCond_FirstUseEver );
			ImGui::SetNextWindowSize( ImVec2( 550, 680 ), ImGuiCond_FirstUseEver );

			ImGuiWindowFlags window_flags = 0;
			if( !ImGui::Begin( "Bagual Engine Test #5 Settings", nullptr, window_flags ) )
			{
				ImGui::End();
				return;
			}

			ImGui::PushItemWidth( ImGui::GetFontSize() * -12 );

			ImGui::Checkbox( "Show Wireframe", &( cubeMeshComp->getShowWireframe_Mutable() ) );

			ImGui::ColorEdit3( "Wireframe Color", cubeMeshComp->getColorMutable().value );

			const float positionRange = 10.f;
			BVec3f& camPos = cameraComp->getTransform_mutable().translation;
			ImGui::SliderFloat3( "Camera Position", reinterpret_cast< float* >( &camPos ), -positionRange, positionRange );

			const float rotRange = 20.f;
			BRotf& camRot = cameraComp->getTransform_mutable().rotation;
			ImGui::SliderFloat3( "Camera Rotation", reinterpret_cast< float* >( &camRot ), -rotRange, rotRange );

			ImGui::End();
		};

		// Gui update procedure
		BEngine::Instance().registerGuiTickFunc( &guiTickFunc );
	}

	void BEngineTest_AABBTests::tick()
	{
	}

	bool BEngineTest_AABBTests::initialized() const
	{
		return cubeNode;
	}

	void BEngineTest_AABBTests::destroy()
	{
	}

#pragma endregion

} // namespace bgl