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
		void Delay(unsigned int ms) override;
		virtual std::shared_ptr<BCanvas> CreateCanvas(unsigned short width, unsigned short height) override;
		virtual std::weak_ptr<BViewport> CreateViewport(BCanvas& canvas, const FViewportSettings& viewportSettings) override;

	};
}



