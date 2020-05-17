#pragma once

#include "BGraphicsDriverInterface.h"

namespace Bagual::Graphics
{
	class BGraphicsDriverBase : public BGraphicsDriverInterface
	{

	public:

		virtual void Render();

	};
}