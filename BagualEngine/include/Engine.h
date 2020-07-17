#pragma once

#include "Common.h"

namespace bgl
{
	class BGraphicsPlatform;
	class BPlatformBase;
	class BIModule;
}

namespace bgl
{
	class Engine
	{

#pragma region Private Types
		enum class EBEngineState
		{
			None,
			Initializing,
			Ticking,
			Paused,
			Quitting
		};
#pragma endregion

#pragma region std::unique_ptr "permissions"
		friend std::unique_ptr<Engine> std::make_unique<Engine>();
		friend std::unique_ptr<BGraphicsPlatform> std::make_unique<BGraphicsPlatform>();
		friend std::unique_ptr<BPlatformBase> std::make_unique<BPlatformBase>();
		friend std::unique_ptr<BArray<std::shared_ptr<BIModule>>> std::make_unique<BArray<std::shared_ptr<BIModule>>>();
#pragma endregion

		static std::unique_ptr<Engine> instance;
		std::unique_ptr<BGraphicsPlatform> graphicsPlatform;
		std::unique_ptr<BPlatformBase> platform;
		std::unique_ptr<BArray< std::shared_ptr<BIModule>>> modules;

		EBEngineState engineState;

		void Init();
		void LoadData();
		void RegisterModules();
		void Term();
		void ProcessInput();
		void MainLoop();
		void ModulesLoop();

		Engine();

	public:

		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		static Engine& Instance();

		void Peleia();

		static std::unique_ptr<BGraphicsPlatform>& GraphicsPlatform();

		static std::unique_ptr<BPlatformBase>& Platform();

	};

}