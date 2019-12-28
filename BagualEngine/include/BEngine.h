#pragma once

#include <memory>
#include "BGraphicsPlatform.h"
#include "BSettings.h"

namespace Bagual
{
	class BagualEngine
	{
		friend std::unique_ptr<BagualEngine> std::make_unique<BagualEngine>();
		friend std::unique_ptr<Bagual::GraphicsPlatform::BGraphicsPlatform> std::make_unique<Bagual::GraphicsPlatform::BGraphicsPlatform>();
		
		static std::unique_ptr<BagualEngine> instance;

		std::unique_ptr<Bagual::GraphicsPlatform::BGraphicsPlatform> graphicsPlatform;

		bool toQuit;
		bool pause;

		void Init();
		void LoadData();
		void Term();
		void ProcessInput();
		void Loop();

		BagualEngine();

	public:

		BagualEngine(const BagualEngine&) = delete;
		BagualEngine& operator=(const BagualEngine&) = delete;

		static BagualEngine& Instance();

		void Peleia();

	};

}