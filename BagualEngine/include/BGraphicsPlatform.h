#pragma once

#include "BGraphicsDriverInterface.h"
#include "BCanvas.h"
#include "BTypes.h"
#include <memory>

namespace Bagual::Graphics
{
	class BGraphicsPlatform : public BGraphicsDriverInterface
	{
		friend std::unique_ptr<BGraphicsDriverInterface> std::make_unique<BGraphicsDriverInterface>();
		friend std::unique_ptr<Bagual::Types::BArray<std::shared_ptr<BCanvas>>> std::make_unique<Bagual::Types::BArray<std::shared_ptr<BCanvas>>>();

		std::unique_ptr<BGraphicsDriverInterface> driverInstance;

		std::unique_ptr<Bagual::Types::BArray<std::shared_ptr<BCanvas>>> canvases;

	public:

		BGraphicsPlatform();

		virtual void Render();
		virtual void Delay(unsigned int ms);;
		virtual std::weak_ptr<BCanvas> CreateCanvas(unsigned short width, unsigned short height);;

	};
}