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
		void Delay(const uint32&& ms) override;
		void Delay(const uint32& ms) override;

	};
}



