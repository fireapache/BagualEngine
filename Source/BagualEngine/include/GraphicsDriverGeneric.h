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

	enum class BERenderThreadMode
	{
		SingleThread,
		MultiThread,
		HyperThread
	};

	class BGraphicsDriverGeneric : public BGraphicsDriverBase
	{
		class BGenericPlatformWindow* m_cachedPlatformWindowPtr = nullptr;

	public:

		static double maxZ, minZ;
		static BVector3<float> camOrig;
		static BVector3<float> camRot;
		static BViewport* cachedViewport;
		static BVector2<float> sensorSize;
		static BCamera* cachedCamera;
		static BERenderSpeed renderSpeed;
		static BESceneSetup sceneSetup;
		static BERenderThreadMode renderThreadMode;

		BGraphicsDriverGeneric();
		~BGraphicsDriverGeneric();

		static void RenderLines(BCamera* camera, const uint32 renderThreadIndex);
		static void PaintPixel(class BViewport* viewportPtr, uint32 i, uint32 j, uint32 rgb);

		void SwapFrames() override;
		void RenderGameFrame() override;
		void SwapUIFrame() override;
		void SwapGameFrame() override;
		void Delay(const uint32&& ms) override;
		void Delay(const uint32& ms) override;

	};
}



