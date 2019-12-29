#pragma once

#include <memory>
#include "BBuffer.h"
#include "BTypes.h"

namespace Bagual::Graphics
{
	class BCanvas
	{
		unsigned short width;
		unsigned short height;

		Bagual::Types::BLine<Bagual::Types::BPixelPos> screenEdges[4];

		std::shared_ptr<Bagual::Utils::BBuffer<unsigned int>> buffer;

	public:

		BCanvas(unsigned short width = 320, unsigned short height = 240) : width(width), height(height)
		{
			buffer = std::make_shared<Bagual::Utils::BBuffer<unsigned int>>(width * height);
		}

	};
}