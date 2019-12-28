
#include "BGraphicsDriverBase.h"

#include "BCamera.h"
#include "BCameraManager.h"

namespace Bagual::GraphicsDriver
{

	void BGraphicsDriverBase::Render()
	{
		for (Bagual::Camera::BCamera* camera : Bagual::Camera::BCameraManager::GetCameras())
		{
			camera->DrawScreen();
		}
	}

}