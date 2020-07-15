
#include "Canvas.h"
#include "Settings.h"

namespace bgl
{

	BCanvas::BCanvas(unsigned short width /*= 320*/, unsigned short height /*= 240*/) : width(width), height(height)
	{
		this->width = Settings::width;
		this->height = Settings::height;

		this->buffer = std::make_shared<BBuffer<unsigned int>>(static_cast<unsigned int*>(Settings::pixels), Settings::width * Settings::height);

		auto edge0_0 = BPixelPos(0, 0);
		auto edge0_1 = BPixelPos(width - 1, 0);

		this->edges[0] = BLine<BPixelPos>(edge0_0, edge0_1);

		auto edge1_0 = BPixelPos(width - 1, height - 1);
		auto edge1_1 = BPixelPos(0, height - 1);

		this->edges[1] = BLine<BPixelPos>(edge1_0, edge1_1);

		auto edge2_0 = BPixelPos(0, height - 1);
		auto edge2_1 = BPixelPos(0, 0);

		this->edges[2] = BLine<BPixelPos>(edge2_0, edge2_1);

		auto edge3_0 = BPixelPos(width - 1, 0);
		auto edge3_1 = BPixelPos(width - 1, height - 1);

		this->edges[3] = BLine<BPixelPos>(edge3_0, edge3_1);
	}

	unsigned short BCanvas::GetWidth()
	{
		return width;
	}

	unsigned short BCanvas::GetHeight()
	{
		return height;
	}

	BLine<BPixelPos>* BCanvas::GetEdges()
	{
		return edges;
	}

	BLine<BPixelPos>& BCanvas::GetEdge(BEBoxEdges edge)
	{
		return edges[static_cast<int>(edge)];
	}

}

