
#include "BEngine.h"
#include "BCamera.h"
#include "BCameraManager.h"


namespace Bagual
{
	std::unique_ptr<BagualEngine> BagualEngine::instance(nullptr);

	void BagualEngine::Init()
	{
		graphicsPlatform = std::make_unique<Bagual::GraphicsPlatform::BGraphicsPlatform>();
		toQuit = false;
		pause = false;
	}

	void BagualEngine::LoadData()
	{
		Bagual::Camera::BCameraManager::AddCamera(new Bagual::Camera::BCamera());
	}

	void BagualEngine::Term()
	{
		
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
				if (graphicsPlatform)
				{
					graphicsPlatform->Render();
				}
				
				//pause = true;
			}
			
			graphicsPlatform->Delay(1);
		}
	}

	BagualEngine::BagualEngine() : toQuit(false), pause(false)
	{
		BagualSettings::width = 320;
		BagualSettings::height = 240;
	}

	Bagual::BagualEngine& BagualEngine::Instance()
	{
		if (!instance) instance = std::make_unique<BagualEngine>();
		return *instance.get();
	}

	void BagualEngine::Peleia()
	{
		Init();
		LoadData();
		Loop();
		Term();
	}

}