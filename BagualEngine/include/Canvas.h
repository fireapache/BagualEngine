#pragma once

#include <memory>

#include "Types.h"

namespace bgl
{
	class BCanvas
	{
		unsigned short width;
		unsigned short height;

		BLine<BPixelPos> edges[4];

		std::shared_ptr<BBuffer<unsigned int>> buffer;

	public:

		BCanvas(unsigned short width = 320, unsigned short height = 240);

		unsigned short GetWidth();
		unsigned short GetHeight();

		BLine<BPixelPos>* GetEdges();
		BLine<BPixelPos>& GetEdge(BEBoxEdges edge);

		BBuffer<unsigned int>& GetBuffer()
		{
			return *buffer.get();
		}

	};
}