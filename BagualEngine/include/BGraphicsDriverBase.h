#pragma once

#include "BIGraphicsDriver.h"

namespace Bagual::GraphicsDriver
{
	class BGraphicsDriverBase : public BIGraphicsDriver
	{

	public:

		virtual void Render();

	};
}