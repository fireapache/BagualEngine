#pragma once

#include "BIGraphicsDriver.h"

namespace Bagual::Graphics
{
	class BGraphicsDriverBase : public BIGraphicsDriver
	{

	public:

		virtual void Render();

	};
}