#pragma once

#include "GraphicsDriverBase.h"

namespace bgl
{
	class BGraphicsDriverGeneric : public BGraphicsDriverBase
	{
		class BGenericPlatformWindow* CachedPlatformWindowPtr = nullptr;

	public:

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



