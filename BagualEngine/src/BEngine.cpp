
#include "BEngine.h"
#include "BCamera.h"
#include "BCameraManager.h"
#include "BEngineTests.h"
#include "BModule.h"
#include "BTypes.h"


namespace Bagual
{
	std::unique_ptr<BagualEngine> BagualEngine::instance(nullptr);

	void BagualEngine::Init()
	{
		graphicsPlatform = std::make_unique<Bagual::Graphics::BGraphicsPlatform>();
		toQuit = false;
		pause = false;
		modules = std::make_unique<Bagual::Types::BArray<std::shared_ptr<Bagual::Modules::BIModule>>>();
	}

	void BagualEngine::LoadData()
	{
		
	}

	void BagualEngine::RegisterModules()
	{
		
		if (modules)
		{
			//Testing
			modules->Add(std::make_shared<Bagual::Tests::BEngineTest_DrawRandomLines>());

			Bagual::Types::BArray<std::shared_ptr<Bagual::Modules::BIModule>>* moduleArray = modules.get();
			std::shared_ptr<Bagual::Modules::BIModule> mod = (*moduleArray)[0];
			mod->Init();
		}
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

	void BagualEngine::MainLoop()
	{
		while (!toQuit)
		{
			ProcessInput();
			ModulesLoop();

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

	void BagualEngine::ModulesLoop()
	{
		for (size_t i = 0; i < modules->Size(); i++)
		{
			(*modules)[i]->Tick();
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
		RegisterModules();
		MainLoop();
		Term();
	}

	Bagual::Graphics::BGraphicsPlatform* BagualEngine::GraphicsPlatform()
	{
		return Instance().graphicsPlatform.get();
	}

}