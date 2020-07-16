#pragma once

#include "Common.h"

namespace bgl
{
	class BCanvas;
	class BCamera;
	class BViewport;
	struct FViewportSettings;
}

namespace bgl
{
	class BGraphicsDriverInterface
	{

	public:

		virtual void RenderFrame() = 0;
		virtual void RenderCamera(const BCamera& camera) = 0;
		virtual void Delay(const uint&& ms) = 0;
		virtual void Delay(const uint& ms) = 0;
		virtual std::shared_ptr<BCanvas> CreateCanvas(const ushort& width, const ushort& height) = 0;
		virtual std::weak_ptr<BViewport> CreateViewport(BCanvas& canvas, const FViewportSettings& viewportSettings) = 0;

	};

	class BGraphicsDriverBase : public BGraphicsDriverInterface
	{

	public:

		virtual void RenderFrame();
		virtual void RenderCamera(const BCamera& camera);

	};
}