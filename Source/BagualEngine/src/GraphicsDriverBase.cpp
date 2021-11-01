
#include "Bagual.pch.h"

#include "GraphicsDriverBase.h"

#include "CameraManager.h"
#include <thread>

namespace bgl
{

	void BGraphicsDriverBase::StartGameFrameRenderingThread()
	{
		
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