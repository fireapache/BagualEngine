
#include "Viewport.h"
#include "Canvas.h"

namespace bgl
{

	FViewportSettings::FViewportSettings()
	{
		x = 0;
		y = 0;
		width = 320;
		height = 240;
	}

	FViewportSettings::FViewportSettings(std::weak_ptr<BCanvas> canvas, unsigned int width, unsigned int height)
	{
		x = 0;
		y = 0;
		this->width = width;
		this->height = height;
		this->canvas = canvas;
	}

	FViewportSettings::FViewportSettings(std::weak_ptr<BCanvas> canvas)
	{
		x = 0;
		y = 0;
		width = 320;
		height = 240;
		this->canvas = canvas;
	}

	FViewportSettings::FViewportSettings(std::weak_ptr<BCanvas> canvas, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->canvas = canvas;
	}

	BViewport::BViewport(const FViewportSettings& settings)
	{
		this->settings = settings;
	}

	FViewportSettings& BViewport::Set()
	{
		return settings;
	}

	void BViewport::ApplyViewportSettings()
	{

	}

}