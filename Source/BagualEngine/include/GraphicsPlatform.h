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
		std::unique_ptr<BArray<std::shared_ptr<BViewport>>> viewports;

	public:

		BGraphicsPlatform();

		virtual void RenderFrame() override;
		virtual void RenderCamera(const BCamera& camera) override;
		virtual void Delay(const uint&& ms) override;
		virtual void Delay(const uint& ms) override;
		virtual std::shared_ptr<BViewport> CreateViewport(const std::shared_ptr<BCanvas>& canvas);
		virtual std::shared_ptr<BViewport> CreateViewport(const std::shared_ptr<BCanvas>& canvas, const BBox<BVector2<float>>& normalizedSize);
		virtual std::shared_ptr<BViewport> CreateViewport(const std::shared_ptr<BCanvas>& canvas, const uint& width, const uint& height);
		virtual std::shared_ptr<BViewport> CreateViewport(const std::shared_ptr<BCanvas>& canvas, const uint& x, const uint& y, const uint& width, const uint& height);

	};
}