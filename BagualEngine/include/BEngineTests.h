#pragma once

#include "BModule.h"
#include "BCamera.h"
#include <memory>

namespace Bagual::Tests
{
	class BEngineTestBase : public Bagual::Modules::BModuleBase
	{
		
	public:

	};

	class BEngineTest_DrawRandomLines : public BEngineTestBase
	{

		std::shared_ptr<Bagual::Camera::BCamera> camera;

	public:

		void Init() override;
		void Tick() override;
		void Term() override;

	};
}