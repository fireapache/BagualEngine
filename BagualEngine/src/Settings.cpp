
#include "Bagual.pch.h"

#include "Settings.h"

namespace bgl
{
	int Settings::width = 320;
	int Settings::height = 240;
	void* Settings::window = nullptr;
	void* Settings::surface = nullptr;
	void* Settings::pixels = nullptr;
	BLine<BPixelPos> Settings::screenEdges[4];
}
