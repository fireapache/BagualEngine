
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "BagualEngine.h"

#include "EngineModules.h"
#include "EngineTests.h"
#include "GraphicsPlatform.h"
#include "Module.h"
#include "PlatformGeneric.h"
#include "Scene.h"
#include "Settings.h"
#include "Types.h"

namespace bgl
{
	std::unique_ptr< BEngine > BEngine::m_instance( nullptr );

	void BEngine::Init()
	{
		// Initializing stuff
		m_engineState = EEngineState::Initializing;

		m_platform = std::make_unique< BPlatformGeneric >();
		m_graphicsPlatform = std::make_unique< BGraphicsPlatform >();
		m_modules = std::make_unique< BArray< std::shared_ptr< BModule > > >();
		m_scene = std::make_unique< BScene >();

		RegisterModules();
	}

	void BEngine::LoadData()
	{
	}

	void BEngine::RegisterModules()
	{
		m_engineState = EEngineState::RegisteringModules;

		if( m_modules )
		{
			m_modules->Add( std::make_shared< BModuleManager >() );

			m_modules->Add( std::make_shared< BEngineTest_WindowAndScene >() );
			m_modules->Add( std::make_shared< BEngineTest_FundamentalRendering >() );
			m_modules->Add( std::make_shared< BEngineTest_RoomRendering >() );
			m_modules->Add( std::make_shared< BEngineTest_CubeProjection >() );
			m_modules->Add( std::make_shared< BEngineTest_AABBTests >() );

			for( auto& module : *m_modules )
			{
				m_moduleContext = module.get();

				if( module->autoInit() )
				{
					module->init();
				}
			}

			m_moduleContext = nullptr;
		}
	}

	void BEngine::BeginPlay()
	{
		m_engineState = EEngineState::BeginPlaying;

		// Starting rendering thread
		m_graphicsPlatform->SetEnabled( true );
	}

	void BEngine::EndPlay()
	{
		m_engineState = EEngineState::EndPlaying;

		// Stopping rendering thread
		m_graphicsPlatform->SetEnabled( false );
	}

	void BEngine::Term()
	{
		m_engineState = EEngineState::Quitting;
	}

	void BEngine::ProcessInput()
	{
		m_mainLoopState = EMainLoopState::Input;

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
		m_engineState = EEngineState::MainLoop;

		while( m_engineState != EEngineState::Quitting )
		{
			m_mainLoopState = EMainLoopState::None;

			ProcessInput();
			ModulesLoop();
			TickWindows();

			m_mainLoopState = EMainLoopState::Graphics;

			if( m_engineState != EEngineState::Paused )
			{
				if( m_platform->getWindows().Size() <= 0 )
				{
					SetState( EEngineState::Quitting );
					continue;
				}

				if( m_graphicsPlatform )
				{
					m_graphicsPlatform->SwapFrames();
				}
			}

			//m_graphicsPlatform->Delay(1);
		}
	}

	void BEngine::TickWindows()
	{
		m_mainLoopState = EMainLoopState::Windows;

		// Ticking windows to check if need to be destroyed
		auto& windows = m_platform->getWindows();

		for( uint32_t i = 0; i < windows.Size(); i++ )
		{
			if( windows[ i ]->Tick() == false )
			{
				windows.erase( windows.begin() + i );
				i--;
			}
		}
	}

	void BEngine::ModulesLoop()
	{
		m_mainLoopState = EMainLoopState::Modules;

		for( const auto& module : *m_modules )
		{
			const bool bInitialized = module->initialized();

			if( !bInitialized && module->pendingTasks.bInitialize )
			{
				m_moduleContext = module.get();
				module->init();
			}

			m_moduleContext = nullptr;

			if( !module->isHidden() )
			{
				module->tick();
			}
		}
	}

	void BEngine::SetState( EEngineState newState )
	{
		if( m_engineState != newState )
		{
			m_engineState = newState;
			// TODO: Broadcast new engine state through an event
		}
	}

	BEngine::BEngine()
		: m_engineState( EEngineState::None )
	{
		BSettings::width = 320;
		BSettings::height = 240;
	}

	BEngine& BEngine::Instance()
	{
		if( !m_instance )
			m_instance = std::make_unique< BEngine >();
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

	BModule* BEngine::getModuleContext()
	{
		return m_moduleContext;
	}

	BArray< std::shared_ptr< BModule > >& BEngine::getModules() const
	{
		return *m_modules;
	}

	void BEngine::registerGuiTickFunc( GuiTickFuncType* func )
	{
		const auto found = std::find( m_guiTickFuncs.begin(), m_guiTickFuncs.end(), func );
		if( found == m_guiTickFuncs.end() )
		{
			m_guiTickFuncs.Add( func );
		}
	}

	BArray< GuiTickFuncType* >& BEngine::getGuiTickFuncs()
	{
		return m_guiTickFuncs;
	}

} // namespace bgl
