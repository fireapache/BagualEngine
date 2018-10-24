#pragma once

#include "BRenderer.h"
#include "BSettings.h"

namespace Bagual
{

	class BagualEngine
	{
		bool toQuit;
		bool pause;

		void Init();
		void LoadData();
		void Term();
		void ProcessInput();
		void Loop();

		static BagualEngine *instance;

		BagualEngine() : toQuit(false)
		{
			BagualSettings::width = 320;
			BagualSettings::height = 240;
		}

	public:

		BagualEngine(const BagualEngine&) {}
		void operator=(BagualEngine&) {}

		static BagualEngine* Instance()
		{
			if (!instance) instance = new BagualEngine();
			return instance;
		}

		void Peleia();

	};

}