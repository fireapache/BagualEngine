
#include "Bagual.pch.h"

#include "GraphicsDriverBase.h"

#include "CameraManager.h"
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
	
	void BGraphicsDriverBase::SetEnabled(const bool bValue)
	{
		if (bValue == m_bEnabled) return;

		if (bValue)
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
		m_renderGameFrameThread = std::thread([this]()
			{
				m_bEnabled = true;
				while (!m_bStopRenderThread)
				{
					RenderGameFrame();
					m_bGameFrameReady = true;
				}
				m_bEnabled = false;
			});
	}

	void BGraphicsDriverBase::StopGameFrameRenderingThread()
	{
		m_bStopRenderThread = true;
	}

	void BGraphicsDriverBase::SwapFrames()
	{
		if (true)
		{
			SwapGameFrame();
			m_bGameFrameReady = false;
		}

		SwapUIFrame();
	}

	void BGraphicsDriverBase::RenderGameFrame()
	{
		auto cameras = BCameraManager::GetCameras();

		for (auto& camera : cameras)
		{
			RenderCamera(*camera.get());
		}

	}

	void BGraphicsDriverBase::SwapUIFrame()
	{

	}

	void BGraphicsDriverBase::SwapGameFrame()
	{
		
	}

	void BGraphicsDriverBase::RenderCamera(const BCamera& camera)
	{

	}
}