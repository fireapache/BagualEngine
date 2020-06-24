#pragma once

#include "BGraphicsDriverInterface.h"
#include "BGraphicsDriverBase.h"

#include <vector>

#include "BTypes.h"
#include "BCameraManager.h"
#include "BMath.h"

namespace Bagual::Graphics
{
	class BGenericGraphicsDriver : public BGraphicsDriverBase
	{

	public:

		BGenericGraphicsDriver();
		~BGenericGraphicsDriver();

		void RenderFrame() override;
		void Delay(unsigned int ms) override;
		virtual std::shared_ptr<BCanvas> CreateCanvas(unsigned short width, unsigned short height) override;
		virtual std::weak_ptr<BViewport> CreateViewport(BCanvas& canvas, FViewportSettings viewportSettings) override;

	};
}



