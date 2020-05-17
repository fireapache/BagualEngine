
#include "BCanvas.h"

Bagual::Graphics::BCanvas::BCanvas(unsigned short width /*= 320*/, unsigned short height /*= 240*/) : width(width), height(height)
{
	buffer = std::make_shared<Bagual::Utils::BBuffer<unsigned int>>(width * height);
}

