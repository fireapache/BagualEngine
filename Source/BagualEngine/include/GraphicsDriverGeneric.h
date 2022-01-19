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

		BGraphicsDriverGeneric();
		~BGraphicsDriverGeneric();

		static void RenderLines(BCamera* camera, const uint32 renderThreadIndex);
		static void DrawLine(BCamera* camera, BLine<BPixelPos> line);
		static void PaintPixel(class BViewport* viewportPtr, BERenderSpeed renderSpeed, uint32 i, uint32 j, uint32 rgb);

		void SwapFrames() override;
		void RenderGameFrame() override;
		void SwapUIFrame() override;
		void SwapGameFrame() override;
		void Delay(const uint32&& ms) override;
		void Delay(const uint32& ms) override;

	};
}



