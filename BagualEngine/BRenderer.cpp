#include "BRenderer.h"

using namespace Bagual::Camera;

namespace Bagual
{
	namespace Renderer
	{
		void BRenderer::Render()
		{
			for (BCamera *camera : BCameraManager::GetCameras())
			{
				camera->DrawScreen();
			}

			SDL_UpdateWindowSurface(BagualSettings::window);
		}
	}
}

