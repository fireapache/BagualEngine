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

	private:



	public:

		uint x = 0;
		uint y = 0;
		uint width = 320;
		uint height = 240;

		FViewportSettings();

		FViewportSettings(const BCanvas& canvas);

		FViewportSettings(uint width, uint height);

		FViewportSettings(uint x, uint y, uint width, uint height);

	};

	class BViewport
	{
		FViewportSettings settings;

		std::weak_ptr<BCanvas> canvas;

		BBoxEdges Limits;

		void UpdateLimits();

	public:

		BViewport(std::weak_ptr<BCanvas>& canvas, const FViewportSettings& settings);

		const BBoxEdges& GetLimits();

		const FViewportSettings& GetSettings();

		FViewportSettings& Set();

		void ApplySettings();

		std::weak_ptr<BCanvas>& GetCanvas();

	};

}

