#pragma once

#include "Common.h"

#include "Module.h"
#include "Camera.h"

namespace bgl
{
	class BEngineTestBase : public BModuleBase
	{
		
	public:

	};

	class BEngineTest_DrawRandomLines : public BEngineTestBase
	{

		std::shared_ptr<BCamera> camera;

	public:

		void Init() override;
		void Tick() override;
		void Term() override;

	};
}