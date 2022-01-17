
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
		if (bValue == bEnabled) return;

		bEnabled = bValue;

		if (bEnabled)
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
		return bEnabled;
	}

	void BGraphicsDriverBase::StartGameFrameRenderingThread()
	{
		m_renderGameFrameThread = std::thread([this]()
			{
				while (true)
				{
					RenderGameFrame();
				}
			});
	}

	void BGraphicsDriverBase::StopGameFrameRenderingThread()
	{
		m_renderGameFrameThread.~thread();
	}

	void BGraphicsDriverBase::SwapFrames()
	{
		if (true)
		{
			SwapGameFrame();
			bGameFrameReady = false;
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

		bGameFrameReady = true;
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