#pragma once

#include <memory>
#include "BGraphicsPlatform.h"
#include "BModule.h"
#include "BTypes.h"

namespace Bagual
{
	class BagualEngine
	{

#pragma region std::unique_ptr "permissions"
		friend std::unique_ptr<BagualEngine> std::make_unique<BagualEngine>();
		friend std::unique_ptr<Bagual::Graphics::BGraphicsPlatform> std::make_unique<Bagual::Graphics::BGraphicsPlatform>();
		friend std::unique_ptr<Bagual::Types::BArray<std::shared_ptr<Bagual::Modules::BIModule>>> std::make_unique<Bagual::Types::BArray<std::shared_ptr<Bagual::Modules::BIModule>>>();
#pragma endregion

		static std::unique_ptr<BagualEngine> instance;
		std::unique_ptr<Bagual::Graphics::BGraphicsPlatform> graphicsPlatform;
		std::unique_ptr<Bagual::Types::BArray< std::shared_ptr<Bagual::Modules::BIModule>>> modules;

		bool toQuit;
		bool pause;

		void Init();
		void LoadData();
		void RegisterModules();
		void Term();
		void ProcessInput();
		void MainLoop();
		void ModulesLoop();

		BagualEngine();

	public:

		BagualEngine(const BagualEngine&) = delete;
		BagualEngine& operator=(const BagualEngine&) = delete;

		static BagualEngine& Instance();

		void Peleia();

		static Bagual::Graphics::BGraphicsPlatform* GraphicsPlatform();

	};

}