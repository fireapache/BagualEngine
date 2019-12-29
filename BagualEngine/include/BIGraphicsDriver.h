#pragma once

#include <memory>
#include "BCanvas.h"

namespace Bagual::Graphics
{
	class BIGraphicsDriver
	{

	public:

		virtual void Render() = 0;
		virtual void Delay(unsigned int ms) = 0;
		virtual std::weak_ptr<BCanvas> CreateCanvas(unsigned short width, unsigned short height) = 0;

	};
}