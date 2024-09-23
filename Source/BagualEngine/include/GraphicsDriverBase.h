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
		virtual void Delay( const uint32_t&& ms ) = 0;
		virtual void Delay( const uint32_t& ms ) = 0;
		virtual ECameraRotationMethod& GetCameraRotationMethod_Mutator() = 0;
		virtual std::chrono::steady_clock::duration FrameTime() = 0;
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

		uint64_t frameCount{ 0 };

		Average< std::chrono::steady_clock::duration, 50 > averageRenderTime;

		ECameraRotationMethod cameraRotationMethod{ DefaultCameraRotationMethod };

		void SetEnabled( const bool bValue ) override;
		bool IsEnabled() const override;
		void StartGameFrameRenderingThread();
		void StopGameFrameRenderingThread();
		void SwapFrames() override;
		virtual void RenderGameFrame();
		virtual void SwapUIFrame();
		virtual void SwapGameFrame();
		void RenderCamera( const BCamera& camera ) override;
		ECameraRotationMethod& GetCameraRotationMethod_Mutator() override;
		virtual std::chrono::steady_clock::duration FrameTime() override
		{
			return averageRenderTime.get();
		}
	};
} // namespace bgl