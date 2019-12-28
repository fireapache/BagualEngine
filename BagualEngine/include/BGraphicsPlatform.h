#pragma once

#include "BIGraphicsDriver.h"
#include <memory>

namespace Bagual::GraphicsPlatform
{
	class BGraphicsPlatform : Bagual::GraphicsDriver::BIGraphicsDriver
	{
		friend std::unique_ptr<BIGraphicsDriver> std::make_unique<BIGraphicsDriver>();

		std::unique_ptr<BIGraphicsDriver> DriverInstance;

	public:

		BGraphicsPlatform();

		virtual void Render();
		virtual void Delay(unsigned int ms) { }

	};
}