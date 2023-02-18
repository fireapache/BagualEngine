#pragma once

#include "Common.h"

#include <thread>

namespace bgl
{
	class BCanvas;
	class BCamera;
	class BViewport;
	struct FViewportSettings;
} // namespace bgl

namespace bgl
{
	class BGraphicsDriverInterface
	{
	public:
		virtual void SetEnabled( const bool bValue ) = 0;
		virtual bool IsEnabled() const = 0;
		virtual void SwapFrames() = 0;
		virtual void RenderCamera( const BCamera& camera ) = 0;
		virtual void Delay( const uint32&& ms ) = 0;
		virtual void Delay( const uint32& ms ) = 0;
	};

	enum class BERenderOutputType
	{
		Depth,
		UvColor
	};

	class BGraphicsDriverBase : public BGraphicsDriverInterface
	{
	protected:
		bool m_bGameFrameReady = false;
		std::thread m_renderGameFrameThread;
		bool m_bEnabled = false;
		bool m_bStopRenderThread = false;

	public:
		BGraphicsDriverBase();
		~BGraphicsDriverBase();

		virtual void SetEnabled( const bool bValue );
		virtual bool IsEnabled() const;
		void StartGameFrameRenderingThread();
		void StopGameFrameRenderingThread();
		virtual void SwapFrames();
		virtual void RenderGameFrame();
		virtual void SwapUIFrame();
		virtual void SwapGameFrame();
		virtual void RenderCamera( const BCamera& camera );
	};
} // namespace bgl