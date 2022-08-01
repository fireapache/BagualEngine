
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
	std::unique_ptr<BEngine> BEngine::m_instance(nullptr);

	void BEngine::Init()
	{
		// Initializing stuff
		m_engineState = EBEngineState::Initializing;

		m_platform = std::make_unique<BPlatformGeneric>();
		m_graphicsPlatform = std::make_unique<BGraphicsPlatform>();
		m_modules = std::make_unique<BArray<std::shared_ptr<BIModule>>>();
		m_scene = std::make_unique<BScene>();

		RegisterModules();
	}

	void BEngine::LoadData()
	{
		
	}

	void BEngine::RegisterModules()
	{
		m_engineState = EBEngineState::RegisteringModules;

		if (m_modules)
		{
			//m_modules->Add(std::make_shared<BEngineTest_FundamentalRendering>());
			//m_modules->Add(std::make_shared<BEngineTest_RoomRendering>());
			m_modules->Add(std::make_shared< BEngineTest_DeProjection>());

			for (auto& module : *m_modules.get())
			{
				module->Init();
			}
		}

	}

	void BEngine::BeginPlay()
	{
		m_engineState = EBEngineState::BeginPlaying;

		// Starting rendering thread
		m_graphicsPlatform->SetEnabled(true);
	}

	void BEngine::EndPlay()
	{
		m_engineState = EBEngineState::EndPlaying;

		// Stopping rendering thread
		m_graphicsPlatform->SetEnabled(false);
	}

	void BEngine::Term()
	{
		m_engineState = EBEngineState::Quitting;
	}

	void BEngine::ProcessInput()
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

	void BEngine::MainLoop()
	{
		m_engineState = EBEngineState::Ticking;

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

	void BEngine::TickWindows()
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

	void BEngine::ModulesLoop()
	{
		for (size_t i = 0; i < m_modules->Size(); i++)
		{
			(*m_modules)[i]->Tick();
		}
	}

	void BEngine::SetState(EBEngineState newState)
	{
		if (m_engineState != newState)
		{
			m_engineState = newState;
			// TODO: Broadcast new engine state through an event
		}
	}

	BEngine::BEngine() : m_engineState(EBEngineState::None)
	{
		Settings::width = 320;
		Settings::height = 240;
	}

	BEngine& BEngine::Instance()
	{
		if (!m_instance) m_instance = std::make_unique<BEngine>();
		return *m_instance.get();
	}

	void BEngine::Peleia()
	{
		Init();
		LoadData();
		BeginPlay();
		MainLoop();
		EndPlay();
		Term();
	}

	BGraphicsPlatform& BEngine::GraphicsPlatform()
	{
		return *Instance().m_graphicsPlatform.get();
	}

	BPlatformBase& BEngine::Platform()
	{
		return *Instance().m_platform.get();
	}

	BScene& BEngine::Scene()
	{
		return *Instance().m_scene.get();
	}

}


