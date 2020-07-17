
#include "Bagual.pch.h"

#include "GraphicsDriverGeneric.h"
#include "Camera.h"
#include "CameraManager.h"
#include "Settings.h"
#include "Draw.h"
#include "Viewport.h"
#include "PlatformGeneric.h"

#include <SDL.h>

namespace bgl
{
	
	BGraphicsDriverGeneric::BGraphicsDriverGeneric()
	{
		SDL_Init(SDL_INIT_VIDEO);
	}

	BGraphicsDriverGeneric::~BGraphicsDriverGeneric()
	{
		SDL_Quit();
	}

	void BGraphicsDriverGeneric::RenderFrame()
	{
		BGraphicsDriverBase::RenderFrame();

		auto cameras = BCameraManager::GetCameras();

		// Rendering each camera
		for (auto& camera : cameras)
		{
			BGL_ASSERT(camera != nullptr && "Got null camera during render!");

			auto canvas = camera->GetViewport().GetCanvas().lock();

			BGL_ASSERT(canvas != nullptr && "Got null canvas during render!");

			auto window = canvas->GetWindow();

			BGL_ASSERT(window != nullptr && "Got null window during render!");

			auto genericWindow = static_cast<BGenericPlatformWindow*>(window);

			BGL_ASSERT(genericWindow != nullptr && "Got null generic window during render!");

			auto surface = genericWindow->GetSDL_Surface();

			BGL_ASSERT(surface != nullptr && "Got null SDL surface during render!");

			SDL_LockSurface(surface);

			// Rendering queued 2D lines
			auto lines = camera->GetLine2DBuffer();

			for (auto& line : lines)
			{
				DrawLine(*camera.get(), line);
			}

			camera->ClearLine2DBuffer();

			SDL_UnlockSurface(surface);

			SDL_UpdateWindowSurface(genericWindow->GetSDL_Window());
		}

	}

	void BGraphicsDriverGeneric::Delay(const uint&& ms)
	{
		SDL_Delay(ms);
	}

	void BGraphicsDriverGeneric::Delay(const uint& ms)
	{
		SDL_Delay(ms);
	}

}

