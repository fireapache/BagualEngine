#pragma once

#include "BGraphicsDriverInterface.h"
#include "BGraphicsDriverBase.h"

#include <vector>

#include "BTypes.h"
#include "BCameraManager.h"
#include "BMath.h"

namespace Bagual::Graphics
{
	class BWindowsGraphicsDriver : public BGraphicsDriverBase
	{

	public:

		BWindowsGraphicsDriver();
		~BWindowsGraphicsDriver();

		void Render() override;
		void Delay(unsigned int ms) override;
		virtual std::weak_ptr<BCanvas> CreateCanvas(unsigned short width, unsigned short height) override;

	};
}



