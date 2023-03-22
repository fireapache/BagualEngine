#pragma once

#include "Common.h"
#include "GraphicsDriverGeneric.h"
#include "Module.h"
#include "PlatformBase.h"

namespace bgl
{
	class BPlatformWindow;
	class BViewport;
	class BCanvas;
	class BCamera;
	class BNode;
	class BCameraComponent;
	class BMeshComponent;
} // namespace bgl

namespace bgl
{
	class BEngineTestBase : public BModule
	{
	public:
		BEngineTestBase( const char* name )
			: BModule( name ){};

		BPlatformWindow* window{ nullptr };
		BViewport* viewport{ nullptr };
		BCanvas* canvas{ nullptr };
		BNode* cameraNode{ nullptr };
		BCameraComponent* cameraComp{ nullptr };
		BCamera* camera{ nullptr };
		float defaultDepthDist{ FLT_MAX };

		GuiTickFuncType guiTickFunc;

		void CreateTestWindowAndScene();
	};

	class BEngineTest_WindowAndScene : public BEngineTestBase
	{
	public:
		BEngineTest_WindowAndScene()
			: BEngineTestBase( "Test Window and Scene" ){};

		void init() override;
		void destroy() override;

		bool autoInit() const override
		{
			return true;
		}

		bool initialized() const override
		{
			return window;
		}

		bool showOnModuleManager() const override
		{
			return false;
		}
	};

	class BEngineTest_FundamentalRendering : public BEngineTestBase
	{
		void QueueCameraLineDraw( class BCamera* camera );

	public:
		BNode* trisNode{ nullptr };

		BEngineTest_FundamentalRendering()
			: BEngineTestBase( "Fundamental Rendering" ){};

		void init() override;
		void tick() override;
		bool initialized() const override;
		void destroy() override;
	};

	class BEngineTest_CubeProjection : public BEngineTestBase
	{
	public:
		BEngineTest_CubeProjection()
			: BEngineTestBase( "Cube Projection" ){};

		BVec3f points[ 8 ];
		BNode* cubeNode{ nullptr };

		void init() override;
		void tick() override;
		bool initialized() const override;
		void destroy() override;
	};

	class BEngineTest_RoomRendering : public BEngineTestBase
	{
	public:
		BNode* roomRootNode{ nullptr };
		BMeshComponent* roomMeshComp{ nullptr };
		BMeshComponent* objectsMeshComp{ nullptr };
		BMeshComponent* charMeshComp{ nullptr };

		bool wireframes[ 3 ]{ false, false, false };

		BEngineTest_RoomRendering()
			: BEngineTestBase( "Room Rendering" ){};

		enum class BESceneSetup : uint8_t
		{
			Empty,
			WithObjects,
			ObjectsCharacter
		};

		BESceneSetup sceneSetup{ BESceneSetup::ObjectsCharacter };

		void init() override;
		void tick() override;
		bool initialized() const override;
		void destroy() override;
	};

	class BEngineTest_NodeEditor : public BEngineTestBase
	{
		void* nodeEditorContext{ nullptr };

	public:
		BEngineTest_NodeEditor()
			: BEngineTestBase( "Node Editor" ){};

		void init() override;
		bool initialized() const override;
		void destroy() override;
	};

	class BEngineTest_AABBTests : public BEngineTestBase
	{
	public:
		BEngineTest_AABBTests()
			: BEngineTestBase( "AABB Tests Module" ){};

		BMeshComponent* cubeMeshComp{ nullptr };
		BNode* cubeNode{ nullptr };

		void init() override;
		void tick() override;
		bool initialized() const override;
		void destroy() override;
	};

} // namespace bgl