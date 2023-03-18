
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "Settings.h"

namespace bgl
{
	int BSettings::width = 320;
	int BSettings::height = 240;
	double BSettings::simulationFrequency{ 1.0 / 30.0 };
	double BSettings::renderingFrequency{ 1.0 / 60.0 };
	uint32_t BSettings::lineColor = 0x000000FF;
} // namespace bgl
