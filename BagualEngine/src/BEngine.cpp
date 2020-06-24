
#include "BEngine.h"
#include "BCamera.h"
#include "BCameraManager.h"
#include "BEngineTests.h"
#include "BModule.h"
#include "BTypes.h"

#include <SDL.h>

namespace Bagual
{
	std::unique_ptr<BagualEngine> BagualEngine::instance(nullptr);

	void BagualEngine::Init()
	{
		graphicsPlatform = std::make_unique<Bagual::Graphics::BGraphicsPlatform>();
		engineState = EBEngineState::Initializing;
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
					engineState = EBEngineState::Quitting;
					break;
				case 'p':
					engineState = engineState == EBEngineState::Paused ? EBEngineState::Ticking : EBEngineState::Paused;
				default: break;
				}
			}
		}
	}

	void BagualEngine::MainLoop()
	{
		while (engineState != EBEngineState::Quitting)
		{
			ProcessInput();
			ModulesLoop();

			if (engineState != EBEngineState::Paused)
			{
				if (graphicsPlatform)
				{
					graphicsPlatform->RenderFrame();
				}
				
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

	BagualEngine::BagualEngine() : engineState(EBEngineState::None)
	{
		Settings::width = 320;
		Settings::height = 240;
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