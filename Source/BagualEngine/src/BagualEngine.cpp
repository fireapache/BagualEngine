
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
#include "Scene.h"

namespace bgl
{
	std::unique_ptr<Engine> Engine::m_instance(nullptr);

	void Engine::Init()
	{
		m_graphicsPlatform = std::make_unique<BGraphicsPlatform>();
		m_platform = std::make_unique<BPlatformGeneric>();
		m_engineState = EBEngineState::Initializing;
		m_modules = std::make_unique<BArray<std::shared_ptr<BIModule>>>();
		m_scene = std::make_unique<BScene>();
	}

	void Engine::LoadData()
	{
		
	}

	void Engine::RegisterModules()
	{
		
		if (m_modules)
		{
			m_modules->Add(std::make_shared<BEngineTest_BaseRendering>());

			BArray<std::shared_ptr<BIModule>>* moduleArray = m_modules.get();
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
					m_engineState = EBEngineState::Quitting;
					break;
				case 'p':
					m_engineState = m_engineState == EBEngineState::Paused ? EBEngineState::Ticking : EBEngineState::Paused;
				default: break;
				}
			}
		}*/
	}

	void Engine::MainLoop()
	{
		while (m_engineState != EBEngineState::Quitting)
		{
			ProcessInput();
			ModulesLoop();

			if (m_engineState != EBEngineState::Paused)
			{
				TickWindows();

				if (m_platform->GetWindows().Size() <= 0)
				{
					SetState(EBEngineState::Quitting);
					continue;
				}

				if (m_graphicsPlatform)
				{
					m_graphicsPlatform->SwapFrames();
				}
				
			}
			
			//m_graphicsPlatform->Delay(1);
		}
	}

	void Engine::TickWindows()
	{
		// Ticking windows to check if need to be destroyed
		auto& windows = m_platform->GetWindows();

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
		for (size_t i = 0; i < m_modules->Size(); i++)
		{
			(*m_modules)[i]->Tick();
		}
	}

	void Engine::SetState(EBEngineState newState)
	{
		if (m_engineState != newState)
		{
			m_engineState = newState;
			// TODO: Broadcast new engine state through an event
		}
	}

	Engine::Engine() : m_engineState(EBEngineState::None)
	{
		Settings::width = 320;
		Settings::height = 240;
	}

	Engine& Engine::Instance()
	{
		if (!m_instance) m_instance = std::make_unique<Engine>();
		return *m_instance.get();
	}

	void Engine::Peleia()
	{
		Init();
		LoadData();
		RegisterModules();
		MainLoop();
		Term();
	}

	BGraphicsPlatform& Engine::GraphicsPlatform()
	{
		return *Instance().m_graphicsPlatform.get();
	}

	BPlatformBase& Engine::Platform()
	{
		return *Instance().m_platform.get();
	}

	BScene& Engine::Scene()
	{
		return *Instance().m_scene.get();
	}

}


