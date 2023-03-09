#pragma once

#include "Common.h"

namespace bgl
{
	class BGraphicsPlatform;
	class BPlatformBase;
	class BModule;
	class BScene;
} // namespace bgl

namespace bgl
{
	class BEngine
	{
#pragma region Private Types
		enum class EBEngineState : uint8_t
		{
			None,
			Initializing,
			RegisteringModules,
			BeginPlaying,
			Ticking,
			Paused,
			EndPlaying,
			EndingModules,
			Quitting
		};
#pragma endregion

#pragma region std::unique_ptr "permissions"
		friend std::unique_ptr< BEngine > std::make_unique< BEngine >();
		friend std::unique_ptr< BGraphicsPlatform > std::make_unique< BGraphicsPlatform >();
		friend std::unique_ptr< BPlatformBase > std::make_unique< BPlatformBase >();
		friend std::unique_ptr< BArray< std::shared_ptr< BModule > > > std::make_unique<
			BArray< std::shared_ptr< BModule > > >();
		friend std::unique_ptr< BScene > std::make_unique< BScene >();
#pragma endregion

		static std::unique_ptr< BEngine > m_instance;
		std::unique_ptr< BGraphicsPlatform > m_graphicsPlatform;
		std::unique_ptr< BPlatformBase > m_platform;
		std::unique_ptr< BArray< std::shared_ptr< BModule > > > m_modules;
		std::unique_ptr< BScene > m_scene;

		EBEngineState m_engineState;

		BModule* m_moduleContext = nullptr;

		void Init();
		void LoadData();
		void RegisterModules();
		void BeginPlay();
		void EndPlay();
		void Term();
		void ProcessInput();
		void MainLoop();

		void TickWindows();

		void ModulesLoop();

		void SetState( EBEngineState newState );

		BEngine();

	public:
		BEngine( const BEngine& ) = delete;
		BEngine& operator=( const BEngine& ) = delete;

		static BEngine& Instance();

		void Peleia();

		static BGraphicsPlatform& GraphicsPlatform();
		static BPlatformBase& Platform();
		static BScene& Scene();
		BModule* getModuleContext();
	};

} // namespace bgl
