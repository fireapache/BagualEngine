#pragma once

#include "Common.h"

namespace bgl
{
	class BCanvas
	{
		ushort width;
		ushort height;

		BLine<BPixelPos> edges[4];

		std::shared_ptr<BBuffer<uint>> buffer;

	public:

		BCanvas(ushort width = 320, ushort height = 240);

		ushort GetWidth();
		ushort GetHeight();

		BLine<BPixelPos>* GetEdges();
		BLine<BPixelPos>& GetEdge(BEBoxEdges edge);

		BBuffer<uint>& GetBuffer()
		{
			return *buffer.get();
		}

	};
}