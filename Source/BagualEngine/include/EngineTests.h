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

	protected:

		FWindowSettings windowSettings;
		
		BPlatformWindow* window;
		BViewport* viewport;
		BCanvas* canvas;
		float defaultDepthDist;

	public:

		void CreateStandardWindow(const char* windowTitle = nullptr);

	};

	class BEngineTest_FundamentalRendering : public BEngineTestBase
	{

		void DrawCameraLine(class BCamera* camera);

	public:

		void Init() override;
		void Tick() override;
		void Term() override;

	};

	class BEngineTest_RoomRendering : public BEngineTestBase
	{
		
	public:

		BCameraComponent* cameraComp;

		void Init() override;
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