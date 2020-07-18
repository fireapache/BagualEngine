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
		FWindowSettings windowSettings;

		std::shared_ptr<BCamera> camera1;
		std::shared_ptr<BCamera> camera2;

	public:

		void Init() override;
		void Tick() override;

		void DrawCameraLine(std::shared_ptr<BCamera>& camera);

		void Term() override;

	};
}