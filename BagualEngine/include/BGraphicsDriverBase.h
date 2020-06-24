#pragma once

#include "BGraphicsDriverInterface.h"

class BCamera;

namespace Bagual::Graphics
{
	class BGraphicsDriverBase : public BGraphicsDriverInterface
	{

	public:

		virtual void RenderFrame();
		virtual void RenderCamera(std::shared_ptr<Bagual::Camera::BCamera> camera);

	};
}