#pragma once

#include <memory>
#include "GraphicsPlatform.h"
#include "Module.h"
#include "Types.h"

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
		friend std::unique_ptr<bgl::BGraphicsPlatform> std::make_unique<bgl::BGraphicsPlatform>();
		friend std::unique_ptr<bgl::BArray<std::shared_ptr<bgl::BIModule>>> std::make_unique<bgl::BArray<std::shared_ptr<bgl::BIModule>>>();
#pragma endregion

		static std::unique_ptr<Engine> instance;
		std::unique_ptr<bgl::BGraphicsPlatform> graphicsPlatform;
		std::unique_ptr<bgl::BArray< std::shared_ptr<bgl::BIModule>>> modules;

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

		static bgl::BGraphicsPlatform* GraphicsPlatform();

	};

}