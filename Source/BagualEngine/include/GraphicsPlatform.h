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
		virtual void Delay(const uint32&& ms) override;
		virtual void Delay(const uint32& ms) override;
		virtual std::shared_ptr<BViewport> CreateViewport(const std::shared_ptr<BCanvas>& canvas);
		virtual std::shared_ptr<BViewport> CreateViewport(const std::shared_ptr<BCanvas>& canvas, const BBox<BVector2<float>>& normalizedSize);
		virtual std::shared_ptr<BViewport> CreateViewport(const std::shared_ptr<BCanvas>& canvas, const uint32& width, const uint32& height);
		virtual std::shared_ptr<BViewport> CreateViewport(const std::shared_ptr<BCanvas>& canvas, const uint32& x, const uint32& y, const uint32& width, const uint32& height);

	};
}