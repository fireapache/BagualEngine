#pragma once

#include "BGraphicsDriverInterface.h"

class BCamera;

namespace bgl
{
	class BGraphicsDriverBase : public BGraphicsDriverInterface
	{

	public:

		virtual void RenderFrame();
		virtual void RenderCamera(std::shared_ptr<bgl::BCamera> camera);

	};
}