#pragma once

#include "BIGraphicsDriver.h"
#include "BCanvas.h"
#include "BTypes.h"
#include <memory>

namespace Bagual::Graphics
{
	class BGraphicsPlatform : Bagual::Graphics::BIGraphicsDriver
	{
		friend std::unique_ptr<BIGraphicsDriver> std::make_unique<BIGraphicsDriver>();
		friend std::unique_ptr<Bagual::Types::BArray<std::shared_ptr<BCanvas>>> std::make_unique<Bagual::Types::BArray<std::shared_ptr<BCanvas>>>();

		std::unique_ptr<BIGraphicsDriver> driverInstance;

		std::unique_ptr<Bagual::Types::BArray<std::shared_ptr<BCanvas>>> canvases;

	public:

		BGraphicsPlatform();

		virtual void Render();
		virtual void Delay(unsigned int ms);;
		virtual std::weak_ptr<BCanvas> CreateCanvas(unsigned short width, unsigned short height);;

	};
}