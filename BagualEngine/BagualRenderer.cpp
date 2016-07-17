#include "BagualRenderer.h"


void Renderer::Render()
{
	for (Camera *camera : CameraManager::GetCameras())
	{
		camera->DrawScreen();
	}

	SDL_UpdateWindowSurface(BagualSettings::window);
}

