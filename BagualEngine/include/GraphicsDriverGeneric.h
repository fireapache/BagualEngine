#pragma once

#include "GraphicsDriverBase.h"

namespace bgl
{
	class BGraphicsDriverGeneric : public BGraphicsDriverBase
	{

	public:

		BGraphicsDriverGeneric();
		~BGraphicsDriverGeneric();

		void RenderFrame() override;
		void Delay(const uint&& ms) override;
		void Delay(const uint& ms) override;
		virtual std::shared_ptr<BCanvas> CreateCanvas(const ushort& width, const ushort& height) override;
		virtual std::weak_ptr<BViewport> CreateViewport(BCanvas& canvas, const FViewportSettings& viewportSettings) override;

	};
}



