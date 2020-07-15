#pragma once

#include "Module.h"
#include "Camera.h"
#include <memory>

namespace bgl
{
	class BEngineTestBase : public bgl::BModuleBase
	{
		
	public:

	};

	class BEngineTest_DrawRandomLines : public BEngineTestBase
	{

		std::shared_ptr<bgl::BCamera> camera;

	public:

		void Init() override;
		void Tick() override;
		void Term() override;

	};
}