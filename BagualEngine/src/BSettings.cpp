#include "BSettings.h"

namespace Bagual
{
	int Settings::width = 320;
	int Settings::height = 240;
	void* Settings::window = nullptr;
	void* Settings::surface = nullptr;
	void* Settings::pixels = nullptr;
	Bagual::Types::BLine<Bagual::Types::BPixelPos> Settings::screenEdges[4];
}
