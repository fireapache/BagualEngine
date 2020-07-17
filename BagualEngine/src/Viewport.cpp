
#include "Bagual.pch.h"

#include "Viewport.h"
#include "Canvas.h"

namespace bgl
{
	FViewportSettings::FViewportSettings()
	{

	}

	FViewportSettings::FViewportSettings(uint width, uint height)
		: x(0), y(0), width(width), height(height)
	{
		
	}

	FViewportSettings::FViewportSettings(uint x, uint y, uint width, uint height)
		: x(x), y(y), width(width), height(height)
	{
		
	}

	FViewportSettings::FViewportSettings(const BCanvas& canvas)
		: x(0), y(0)
	{
		width = canvas.GetWidth();
		height = canvas.GetHeight();
	}

	void BViewport::UpdateLimits()
	{
		const uint& x = settings.x;
		const uint& y = settings.y;
		const uint& width = settings.width;
		const uint& height = settings.height;

		const BPixelPos edge0_0(x, y);
		const BPixelPos edge0_1(width - 1, y);

		Limits[BEBoxEdge::Top] = BLine<BPixelPos>(edge0_0, edge0_1);

		const BPixelPos edge1_0(width - 1, height - 1);
		const BPixelPos edge1_1(x, height - 1);

		Limits[BEBoxEdge::Bottom] = BLine<BPixelPos>(edge1_0, edge1_1);

		const BPixelPos edge2_0(x, height - 1);
		const BPixelPos edge2_1(x, y);

		Limits[BEBoxEdge::Left] = BLine<BPixelPos>(edge2_0, edge2_1);

		const BPixelPos edge3_0(width - 1, y);
		const BPixelPos edge3_1(width - 1, height - 1);

		Limits[BEBoxEdge::Right] = BLine<BPixelPos>(edge3_0, edge3_1);
	}

	BViewport::BViewport(std::weak_ptr<BCanvas>& canvas, const FViewportSettings& settings)
	{
		this->settings = settings;
		this->canvas = canvas;
		UpdateLimits();
	}

	const bgl::BBoxEdges& BViewport::GetLimits()
	{
		return Limits;
	}

	const FViewportSettings& BViewport::GetSettings()
	{
		return settings;
	}

	FViewportSettings& BViewport::Set()
	{
		return settings;
	}

	void BViewport::ApplySettings()
	{
		UpdateLimits();
	}

	std::weak_ptr<bgl::BCanvas>& BViewport::GetCanvas()
	{
		return canvas;
	}

}