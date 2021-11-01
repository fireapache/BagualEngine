#pragma once

#include "Common.h"

namespace bgl
{
	class BCanvas;
	class BCamera;
	class BViewport;
	struct FViewportSettings;
}

namespace bgl
{
	class BGraphicsDriverInterface
	{

	public:

		virtual void SwapFrames() = 0;
		virtual void RenderCamera(const BCamera& camera) = 0;
		virtual void Delay(const uint32&& ms) = 0;
		virtual void Delay(const uint32& ms) = 0;

	};

	class BGraphicsDriverBase : public BGraphicsDriverInterface
	{

	protected:

		bool bGameFrameReady = false;
		std::thread RenderGameFrameThread;

	public:

		void StartGameFrameRenderingThread();
		virtual void SwapFrames();
		virtual void RenderGameFrame();
		virtual void SwapUIFrame();
		virtual void SwapGameFrame();
		virtual void RenderCamera(const BCamera& camera);

	};
}