
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "GraphicsDriverBase.h"

#include "CameraManager.h"
#include "Settings.h"

#include <thread>

namespace bgl
{
	BGraphicsDriverBase::BGraphicsDriverBase()
	{
	}

	BGraphicsDriverBase::~BGraphicsDriverBase()
	{
		StopGameFrameRenderingThread();
	}

	void BGraphicsDriverBase::SetEnabled( const bool bValue )
	{
		if( bValue == m_bEnabled )
			return;

		if( bValue )
		{
			StartGameFrameRenderingThread();
		}
		else
		{
			StopGameFrameRenderingThread();
		}
	}

	bool BGraphicsDriverBase::IsEnabled() const
	{
		return m_bEnabled;
	}

	void BGraphicsDriverBase::StartGameFrameRenderingThread()
	{
		m_bStopRenderThread = false;
		m_renderGameFrameThread = std::thread(
			[ this ]()
			{
				m_bEnabled = true;
				while( !m_bStopRenderThread )
				{
					const auto renderStartTime = std::chrono::high_resolution_clock::now();
					const auto renderDuration = std::chrono::duration< double, std::milli >( BSettings::renderingFrequency * 1000.0 );
					const auto frameEndTime = renderStartTime + renderDuration;

					RenderGameFrame();
					m_bGameFrameReady = true;

					// limiting frame frequency
					if( BSettings::isDebugFlagsSet( DebugFlags::DBF_CapFPS )
						&& std::chrono::high_resolution_clock::now() < frameEndTime )
					{
						std::this_thread::sleep_until( frameEndTime );
					}

					const auto renderTime = std::chrono::high_resolution_clock::now() - renderStartTime;
					averageRenderTime.add( renderTime );
				}
				m_bEnabled = false;
			} );
	}

	void BGraphicsDriverBase::StopGameFrameRenderingThread()
	{
		m_bStopRenderThread = true;
	}

	void BGraphicsDriverBase::SwapFrames()
	{
		SwapGameFrame();
		SwapUIFrame();
	}

	void BGraphicsDriverBase::RenderGameFrame()
	{
		auto cameras = BCameraManager::GetCameras();

		for( auto camera : cameras )
		{
			RenderCamera( *camera );
		}
	}

	void BGraphicsDriverBase::SwapUIFrame()
	{
	}

	void BGraphicsDriverBase::SwapGameFrame()
	{
	}

	void BGraphicsDriverBase::RenderCamera( const BCamera& camera )
	{
	}

	ECameraRotationMethod& BGraphicsDriverBase::GetCameraRotationMethod_Mutator()
	{
		return cameraRotationMethod;
	}
} // namespace bgl