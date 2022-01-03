#pragma once

#include "GraphicsDriverBase.h"

namespace bgl
{
	class BGraphicsDriverGeneric : public BGraphicsDriverBase
	{
		class BGenericPlatformWindow* CachedPlatformWindowPtr = nullptr;

	public:

		static double maxZ, minZ;
		static BVector3<float> camOrig;
		static BVector3<float> camRot;
		static uint32 i, j;
		static BViewport* cachedViewport;
		static BVector2<float> sensorSize;
		static BCamera* cachedCamera;

		BGraphicsDriverGeneric();
		~BGraphicsDriverGeneric();

		void SwapFrames() override;
		void RenderGameFrame() override;
		void SwapUIFrame() override;
		void SwapGameFrame() override;
		void Delay(const uint32&& ms) override;
		void Delay(const uint32& ms) override;

	};
}



