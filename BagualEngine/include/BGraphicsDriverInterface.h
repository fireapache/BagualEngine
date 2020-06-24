#pragma once

#include <memory>
#include "BCanvas.h"
#include "BViewport.h"

namespace Bagual::Camera
{
	class BCamera;
}


namespace Bagual::Graphics
{
	class BGraphicsDriverInterface
	{

	public:

		virtual void RenderFrame() = 0;
		virtual void RenderCamera(std::shared_ptr<Bagual::Camera::BCamera> camera) = 0;
		virtual void Delay(unsigned int ms) = 0;
		virtual std::shared_ptr<BCanvas> CreateCanvas(unsigned short width, unsigned short height) = 0;
		virtual std::weak_ptr<BViewport> CreateViewport(BCanvas& canvas, FViewportSettings viewportSettings) = 0;

	};
}