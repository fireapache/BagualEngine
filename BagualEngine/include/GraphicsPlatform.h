#pragma once

#include "Common.h"

#include "GraphicsDriverBase.h"

namespace bgl
{
	class BGraphicsPlatform : public BGraphicsDriverInterface
	{
		friend std::unique_ptr<BGraphicsDriverInterface> std::make_unique<BGraphicsDriverInterface>();
		friend std::unique_ptr<BArray<std::shared_ptr<BCanvas>>> std::make_unique<BArray<std::shared_ptr<BCanvas>>>();
		friend std::unique_ptr<BArray<std::shared_ptr<BViewport>>> std::make_unique<BArray<std::shared_ptr<BViewport>>>();

		std::unique_ptr<BGraphicsDriverInterface> driverInstance;

		std::unique_ptr<BArray<std::shared_ptr<BCanvas>>> canvases;
		std::unique_ptr<BArray<std::shared_ptr<BViewport>>> viewports;

	public:

		BGraphicsPlatform();

		virtual void RenderFrame();
		virtual void RenderCamera(std::shared_ptr<BCamera> camera);
		virtual void Delay(unsigned int ms);;
		virtual std::shared_ptr<BCanvas> CreateCanvas(unsigned short width, unsigned short height);
		virtual std::weak_ptr<BViewport> CreateViewport(BCanvas& canvas, const FViewportSettings& viewportSettings);

	};
}