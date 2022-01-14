#pragma once

#include "GraphicsDriverBase.h"

namespace bgl
{
	enum class BERenderSpeed
	{
		Normal,
		Fast,
		VeryFast
	};

	enum class BESceneSetup
	{
		Empty,
		WithObjects,
		ObjectsCharacter
	};

	enum class BERenderMode
	{
		SingleThread,
		MultiThread,
		HyperThread
	};

	class BGraphicsDriverGeneric : public BGraphicsDriverBase
	{
		class BGenericPlatformWindow* CachedPlatformWindowPtr = nullptr;

		void loadTris(const char* filePath, BArray<BTriangle<float>>& triBuffer, uint32 meshID = 0);

	public:

		static double maxZ, minZ;
		static BVector3<float> camOrig;
		static BVector3<float> camRot;
		static BViewport* cachedViewport;
		static BVector2<float> sensorSize;
		static BCamera* cachedCamera;
		static BERenderSpeed renderSpeed;
		static BESceneSetup sceneSetup;
		static BERenderMode renderMode;

		BGraphicsDriverGeneric();
		~BGraphicsDriverGeneric();

		static void RenderLines(BCamera* camera, const uint32 renderThreadIndex);

		void SwapFrames() override;
		void RenderGameFrame() override;
		void SwapUIFrame() override;
		void SwapGameFrame() override;
		void Delay(const uint32&& ms) override;
		void Delay(const uint32& ms) override;

	};
}



