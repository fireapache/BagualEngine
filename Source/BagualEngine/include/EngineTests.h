#pragma once

#include "Common.h"

#include "Module.h"
#include "Camera.h"

namespace bgl
{
	class BPlatformWindow;
	class BViewport;
	class BCanvas;
	class BNode;
	class BCameraComponent;
}

namespace bgl
{
	class BEngineTestBase : public BModuleBase
	{

	public:

		FWindowSettings windowSettings;
		
		BPlatformWindow* window = nullptr;
		BCameraComponent* cameraComp = nullptr;
		BViewport* viewport = nullptr;
		BCanvas* canvas = nullptr;
		BCamera* camera = nullptr;
		float defaultDepthDist;

		void CreateStandardWindow(const char* windowTitle = nullptr);

	};

	class BEngineTest_FundamentalRendering : public BEngineTestBase
	{

		void QueueCameraLineDraw(class BCamera* camera);

	public:

		void Init() override;
		void Tick() override;
		void Term() override;

	};

	class BEngineTest_DeProjection : public BEngineTestBase
	{

	public:

		BVec3f point1, point2;

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
		void* nodeEditorContext;

	public:

		void Init() override;
		void Term() override;

	};
}