#pragma once

#include <memory>
#include "BRenderer.h"
#include "BSettings.h"

namespace Bagual
{

	class BagualEngine
	{
		friend std::unique_ptr<BagualEngine> std::make_unique<BagualEngine>();

		bool toQuit;
		bool pause;

		void Init();
		void LoadData();
		void Term();
		void ProcessInput();
		void Loop();

		static std::unique_ptr<BagualEngine> instance;

		BagualEngine();

	public:

		BagualEngine(const BagualEngine&) = delete;
		BagualEngine& operator=(const BagualEngine&) = delete;

		static BagualEngine& Instance();

		void Peleia();

	};

}