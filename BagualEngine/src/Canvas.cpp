
#include "Canvas.h"
#include "Settings.h"

bgl::BCanvas::BCanvas(unsigned short width /*= 320*/, unsigned short height /*= 240*/) : width(width), height(height)
{
	this->width = bgl::Settings::width;
	this->height = bgl::Settings::height;

	this->buffer = std::make_shared<bgl::BBuffer<unsigned int>>(static_cast<unsigned int*>(bgl::Settings::pixels), bgl::Settings::width * bgl::Settings::height);
	
	auto edge0_0 = bgl::BPixelPos(0, 0);
	auto edge0_1 = bgl::BPixelPos(width - 1, 0);

	this->edges[0] = bgl::BLine<bgl::BPixelPos>(edge0_0, edge0_1);

	auto edge1_0 = bgl::BPixelPos(width - 1, height - 1);
	auto edge1_1 = bgl::BPixelPos(0, height - 1);

	this->edges[1] = bgl::BLine<bgl::BPixelPos>(edge1_0, edge1_1);

	auto edge2_0 = bgl::BPixelPos(0, height - 1);
	auto edge2_1 = bgl::BPixelPos(0, 0);

	this->edges[2] = bgl::BLine<bgl::BPixelPos>(edge2_0, edge2_1);

	auto edge3_0 = bgl::BPixelPos(width - 1, 0);
	auto edge3_1 = bgl::BPixelPos(width - 1, height - 1);

	this->edges[3] = bgl::BLine<bgl::BPixelPos>(edge3_0, edge3_1);
}

unsigned short bgl::BCanvas::GetWidth()
{
	return width;
}

unsigned short bgl::BCanvas::GetHeight()
{
	return height;
}

bgl::BLine<bgl::BPixelPos>* bgl::BCanvas::GetEdges()
{
	return edges;
}

bgl::BLine<bgl::BPixelPos>& bgl::BCanvas::GetEdge(bgl::BEBoxEdges edge)
{
	return edges[static_cast<int>(edge)];
}

