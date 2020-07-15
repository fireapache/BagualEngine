#pragma once

#include <memory>
#include "BCanvas.h"
#include "BViewport.h"

namespace bgl
{
	class BCamera;
}


namespace bgl
{
	class BGraphicsDriverInterface
	{

	public:

		virtual void RenderFrame() = 0;
		virtual void RenderCamera(std::shared_ptr<bgl::BCamera> camera) = 0;
		virtual void Delay(unsigned int ms) = 0;
		virtual std::shared_ptr<BCanvas> CreateCanvas(unsigned short width, unsigned short height) = 0;
		virtual std::weak_ptr<BViewport> CreateViewport(BCanvas& canvas, FViewportSettings viewportSettings) = 0;

	};
}