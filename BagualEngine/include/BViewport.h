#pragma once

#include <memory>
#include "BCanvas.h"

namespace bgl
{
	struct FViewportSettings
	{
		unsigned int x;
		unsigned int y;
		unsigned int width;
		unsigned int height;
		std::weak_ptr<BCanvas> canvas;

		FViewportSettings()
		{
			x = 0;
			y = 0;
			width = 320;
			height = 240;
		}

		FViewportSettings(std::weak_ptr<BCanvas> canvas)
		{
			x = 0;
			y = 0;
			width = 320;
			height = 240;
			this->canvas = canvas;
		}

		FViewportSettings(std::weak_ptr<BCanvas> canvas, unsigned int width, unsigned int height)
		{
			x = 0;
			y = 0;
			this->width = width;
			this->height = height;
			this->canvas = canvas;
		}

		FViewportSettings(std::weak_ptr<BCanvas> canvas, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			this->canvas = canvas;
		}
	};

	class BViewport
	{
		FViewportSettings settings;

	public:

		BViewport(FViewportSettings settings)
		{
			this->settings = settings;
		}

		FViewportSettings& Set()
		{
			return settings;
		}

		void ApplyViewportSettings()
		{

		}
	};

}

