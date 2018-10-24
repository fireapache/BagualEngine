#include "BEngine.h"

using namespace Bagual::Camera;
using namespace Bagual::Renderer;

namespace Bagual
{

	BagualEngine* BagualEngine::instance = nullptr;

	void BagualEngine::Init()
	{
		SDL_Window *window;
		toQuit = false;

		SDL_Init(SDL_INIT_VIDEO);

		window = SDL_CreateWindow("Bagual Engine",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			BagualSettings::width,
			BagualSettings::height,
			SDL_WINDOW_SHOWN);

		BagualSettings::window = window;

		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	void BagualEngine::LoadData()
	{
		BCameraManager::AddCamera(new BCamera());
	}

	void BagualEngine::Term()
	{
		SDL_Quit();
	}

	void BagualEngine::ProcessInput()
	{
		SDL_Event ev;

		while (SDL_PollEvent(&ev))
		{
			if (SDL_KEYDOWN || SDL_KEYUP)
			{
				switch (ev.key.keysym.sym)
				{
				case 'q':
					toQuit = true;
				default: break;
				}
			}
		}
	}

	void BagualEngine::Loop()
	{
		while (!toQuit)
		{
			ProcessInput();
			BRenderer::Render();
			SDL_Delay(10);
		}
	}

	void BagualEngine::Peleia()
	{
		Init();
		LoadData();
		Loop();
		Term();
	}

}