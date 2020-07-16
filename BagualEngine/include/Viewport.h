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
		uint x;
		uint y;
		uint width;
		uint height;

		std::weak_ptr<BCanvas> canvas;

		FViewportSettings();

		FViewportSettings(std::weak_ptr<BCanvas> canvas);

		FViewportSettings(std::weak_ptr<BCanvas> canvas, uint width, uint height);

		FViewportSettings(std::weak_ptr<BCanvas> canvas, uint x, uint y, uint width, uint height);
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

