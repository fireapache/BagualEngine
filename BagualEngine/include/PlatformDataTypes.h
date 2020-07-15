#pragma once

#include <string>

namespace bgl
{
	struct FWindowSettings
	{
		unsigned int width = 320;
		unsigned int height = 240;
		std::string name = "Bagual Engine Window";
	};
}