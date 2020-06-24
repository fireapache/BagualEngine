#pragma once

#include <memory>
#include "BTypes.h"

namespace Bagual::Graphics
{
	class BCanvas
	{
		unsigned short width;
		unsigned short height;

		Bagual::Types::BLine<Bagual::Types::BPixelPos> edges[4];

		std::shared_ptr<Bagual::Types::BBuffer<unsigned int>> buffer;

	public:

		BCanvas(unsigned short width = 320, unsigned short height = 240);

		unsigned short GetWidth();
		unsigned short GetHeight();

		Bagual::Types::BLine<Bagual::Types::BPixelPos>* GetEdges();
		Bagual::Types::BLine<Bagual::Types::BPixelPos>& GetEdge(Bagual::Types::BEBoxEdges edge);

		Bagual::Types::BBuffer<unsigned int>* GetBuffer()
		{
			return buffer.get();
		}

	};
}