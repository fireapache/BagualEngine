
#include "BCanvas.h"

Bagual::Graphics::BCanvas::BCanvas(unsigned short width /*= 320*/, unsigned short height /*= 240*/) : width(width), height(height)
{
	this->width = width;
	this->height = height;

	this->buffer = std::make_shared<Bagual::Types::BBuffer<unsigned int>>(width * height);

	auto edge0_0 = Bagual::Types::BPixelPos(0, 0);
	auto edge0_1 = Bagual::Types::BPixelPos(width, 0);

	this->edges[0] = Bagual::Types::BLine<Bagual::Types::BPixelPos>(edge0_0, edge0_1);

	auto edge1_0 = Bagual::Types::BPixelPos(width, height);
	auto edge1_1 = Bagual::Types::BPixelPos(0, height);

	this->edges[1] = Bagual::Types::BLine<Bagual::Types::BPixelPos>(edge1_0, edge1_1);

	auto edge2_0 = Bagual::Types::BPixelPos(0, height);
	auto edge2_1 = Bagual::Types::BPixelPos(0, 0);

	this->edges[2] = Bagual::Types::BLine<Bagual::Types::BPixelPos>(edge2_0, edge2_1);

	auto edge3_0 = Bagual::Types::BPixelPos(width, 0);
	auto edge3_1 = Bagual::Types::BPixelPos(width, height);

	this->edges[3] = Bagual::Types::BLine<Bagual::Types::BPixelPos>(edge3_0, edge3_1);
}

unsigned short Bagual::Graphics::BCanvas::GetWidth()
{
	return width;
}

unsigned short Bagual::Graphics::BCanvas::GetHeight()
{
	return height;
}

Bagual::Types::BLine<Bagual::Types::BPixelPos>* Bagual::Graphics::BCanvas::GetEdges()
{
	return edges;
}

Bagual::Types::BLine<Bagual::Types::BPixelPos>& Bagual::Graphics::BCanvas::GetEdge(Bagual::Types::BEBoxEdges edge)
{
	return edges[static_cast<int>(edge)];
}

