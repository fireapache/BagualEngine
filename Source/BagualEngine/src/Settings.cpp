
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "Settings.h"

namespace bgl
{
	int BSettings::width = 320;
	int BSettings::height = 240;
	void* BSettings::window = nullptr;
	void* BSettings::surface = nullptr;
	void* BSettings::pixels = nullptr;
	BLine< BPixelPos > BSettings::screenEdges[ 4 ];
	uint32_t BSettings::lineColor = 0x000000FF;
} // namespace bgl
