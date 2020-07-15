#pragma once

#include "Common.h"

namespace bgl
{
	class BCanvas;
}

namespace bgl
{
	struct FViewportSettings
	{
		unsigned int x;
		unsigned int y;
		unsigned int width;
		unsigned int height;
		std::weak_ptr<BCanvas> canvas;

		FViewportSettings();

		FViewportSettings(std::weak_ptr<BCanvas> canvas);

		FViewportSettings(std::weak_ptr<BCanvas> canvas, unsigned int width, unsigned int height);

		FViewportSettings(std::weak_ptr<BCanvas> canvas, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	};

	class BViewport
	{
		FViewportSettings settings;

	public:

		BViewport(const FViewportSettings& settings);

		FViewportSettings& Set();

		void ApplyViewportSettings();
	};

}

