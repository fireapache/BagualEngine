#pragma once

#include <memory>
#include "BTypes.h"

namespace bgl
{
	class BCanvas
	{
		unsigned short width;
		unsigned short height;

		bgl::BLine<bgl::BPixelPos> edges[4];

		std::shared_ptr<bgl::BBuffer<unsigned int>> buffer;

	public:

		BCanvas(unsigned short width = 320, unsigned short height = 240);

		unsigned short GetWidth();
		unsigned short GetHeight();

		bgl::BLine<bgl::BPixelPos>* GetEdges();
		bgl::BLine<bgl::BPixelPos>& GetEdge(bgl::BEBoxEdges edge);

		bgl::BBuffer<unsigned int>& GetBuffer()
		{
			return *buffer.get();
		}

	};
}