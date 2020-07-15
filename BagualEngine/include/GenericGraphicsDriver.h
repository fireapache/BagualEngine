#pragma once

#include "GraphicsDriverInterface.h"
#include "GraphicsDriverBase.h"

#include <vector>

#include "Types.h"
#include "CameraManager.h"
#include "Arithmetics.h"

namespace bgl
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



