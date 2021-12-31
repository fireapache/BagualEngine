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
		static uint32 i, j;
		static BViewport* cachedViewport;
		static float zValue;

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



