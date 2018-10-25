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
		pause = false;

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
			if (ev.key.type == SDL_KEYDOWN)
			{
				switch (ev.key.keysym.sym)
				{
				case 'q':
					toQuit = true;
					break;
				case 'p':
					pause = !pause;
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

			if (!pause)
			{
				BRenderer::Render();
				//pause = true;
			}
			
			SDL_Delay(1);
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