#pragma once

#include "Camera.h"
#include "Common.h"
#include "Module.h"

namespace bgl
{
	class BPlatformWindow;
	class BViewport;
	class BCanvas;
	class BNode;
	class BCameraComponent;
	class BMeshComponent;
} // namespace bgl

namespace bgl
{
	class BEngineTestBase : public BModuleBase
	{
	public:
		FWindowSettings windowSettings;

		BPlatformWindow* window = nullptr;
		BMeshComponent* roomMeshComp = nullptr;
		BMeshComponent* objectsMeshComp = nullptr;
		BMeshComponent* charMeshComp = nullptr;
		BCameraComponent* cameraComp = nullptr;
		BViewport* viewport = nullptr;
		BCanvas* canvas = nullptr;
		BCamera* camera = nullptr;
		float defaultDepthDist;

		void CreateStandardWindow( const char* windowTitle = nullptr );
	};

	class BEngineTest_FundamentalRendering : public BEngineTestBase
	{
		void QueueCameraLineDraw( class BCamera* camera );

	public:
		void Init() override;
		void Tick() override;
		void Term() override;
	};

	class BEngineTest_CubeProjection : public BEngineTestBase
	{
	public:
		BVec3f points[ 8 ];

		void Init() override;
		void Tick() override;
		void Term() override;
	};

	class BEngineTest_RoomRendering : public BEngineTestBase
	{
	public:
		BESceneSetup sceneSetup = BESceneSetup::ObjectsCharacter;

		void Init() override;
		void Tick() override;
		void Term() override;
	};

	class BEngineTest_NodeEditor : public BEngineTestBase
	{
		void* nodeEditorContext = nullptr;

	public:
		void Init() override;
		void Term() override;
	};

	class BEngineTest_AABBTests : public BEngineTestBase
	{
	public:
		BMeshComponent* cubeMeshComp = nullptr;
		
		void Init() override;
		void Tick() override;
		void Term() override;
	};

} // namespace bgl