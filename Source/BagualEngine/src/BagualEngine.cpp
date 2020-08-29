
#include "Bagual.pch.h"

#include "BagualEngine.h"
#include "Camera.h"
#include "CameraManager.h"
#include "EngineTests.h"
#include "Module.h"
#include "Types.h"
#include "Settings.h"
#include "GraphicsPlatform.h"
#include "PlatformGeneric.h"

//#include <SDL.h>

namespace bgl
{
	std::unique_ptr<Engine> Engine::instance(nullptr);

	void Engine::Init()
	{
		graphicsPlatform = std::make_unique<BGraphicsPlatform>();
		platform = std::make_unique<BPlatformGeneric>();
		engineState = EBEngineState::Initializing;
		modules = std::make_unique<BArray<std::shared_ptr<BIModule>>>();
	}

	void Engine::LoadData()
	{
		
	}

	void Engine::RegisterModules()
	{
		
		if (modules)
		{
			//Testing
			modules->Add(std::make_shared<BEngineTest_DrawRandomLines>());

			BArray<std::shared_ptr<BIModule>>* moduleArray = modules.get();
			std::shared_ptr<BIModule> mod = (*moduleArray)[0];
			mod->Init();
		}
	}

	void Engine::Term()
	{
		
	}

	void Engine::ProcessInput()
	{
		/*SDL_Event ev;

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
		}*/
	}

	void Engine::MainLoop()
	{
		while (engineState != EBEngineState::Quitting)
		{
			ProcessInput();
			ModulesLoop();

			if (engineState != EBEngineState::Paused)
			{
				TickWindows();

				if (platform->GetWindows().Size() <= 0)
				{
					SetState(EBEngineState::Quitting);
					continue;
				}

				if (graphicsPlatform)
				{
					graphicsPlatform->RenderFrame();
				}
				
			}
			
			//graphicsPlatform->Delay(1);
		}
	}

	void Engine::TickWindows()
	{
		// Ticking windows to check if need to be destroyed
		auto& windows = platform->GetWindows();

		for (uint32 i = 0; i < windows.Size(); i++)
		{
			if (windows[i]->Tick() == false)
			{
				windows.erase(windows.begin() + i);
				i--;
			}
		}
	}

	void Engine::ModulesLoop()
	{
		for (size_t i = 0; i < modules->Size(); i++)
		{
			(*modules)[i]->Tick();
		}
	}

	void Engine::SetState(EBEngineState newState)
	{
		if (engineState != newState)
		{
			engineState = newState;
			// TODO: Broadcast new engine state through an event
		}
	}

	Engine::Engine() : engineState(EBEngineState::None)
	{
		Settings::width = 320;
		Settings::height = 240;
	}

	Engine& Engine::Instance()
	{
		if (!instance) instance = std::make_unique<Engine>();
		return *instance.get();
	}

	void Engine::Peleia()
	{
		Init();
		LoadData();
		RegisterModules();
		MainLoop();
		Term();
	}

	std::unique_ptr<BGraphicsPlatform>& Engine::GraphicsPlatform()
	{
		return Instance().graphicsPlatform;
	}

	std::unique_ptr<BPlatformBase>& Engine::Platform()
	{
		return Instance().platform;
	}

}


