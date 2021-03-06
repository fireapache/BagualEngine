
#include "Bagual.pch.h"

#include "Viewport.h"
#include "Canvas.h"

namespace bgl
{

	void BViewport::UpdateLimits()
	{
		const BPixelPos edge0_0(bounds.p1.x, bounds.p1.y);
		const BPixelPos edge0_1(bounds.p2.x, bounds.p1.y);

		Limits[BEBoxEdge::Top] = BLine<BPixelPos>(edge0_0, edge0_1);

		const BPixelPos edge1_0(bounds.p2.x, bounds.p2.y);
		const BPixelPos edge1_1(bounds.p1.x, bounds.p2.y);

		Limits[BEBoxEdge::Bottom] = BLine<BPixelPos>(edge1_0, edge1_1);

		const BPixelPos edge2_0(bounds.p1.x, bounds.p2.y);
		const BPixelPos edge2_1(bounds.p1.x, bounds.p1.y);

		Limits[BEBoxEdge::Left] = BLine<BPixelPos>(edge2_0, edge2_1);

		const BPixelPos edge3_0(bounds.p2.x, bounds.p1.y);
		const BPixelPos edge3_1(bounds.p2.x, bounds.p2.y);

		Limits[BEBoxEdge::Right] = BLine<BPixelPos>(edge3_0, edge3_1);
	}

	BViewport::BViewport(const std::shared_ptr<BCanvas>& canvas)
	{
		this->canvas = canvas;

		bounds.p1.x = 0;
		bounds.p1.y = 0;
		bounds.p2.x = canvas->GetWidth() - 1;
		bounds.p2.y = canvas->GetHeight() - 1;

		UpdateLimits();
	}

	BViewport::BViewport(const std::shared_ptr<BCanvas>& canvas, const BBox<BVector2<float>>& normalizedSize)
	{
		this->canvas = canvas;
		SetNormalizedSize(normalizedSize);
	}

	BViewport::BViewport(const std::shared_ptr<BCanvas>& canvas, const uint32& width, const uint32& height)
	{
		this->canvas = canvas;

		bounds.p1.x = 0;
		bounds.p1.y = 0;
		bounds.p2.x = width - 1;
		bounds.p2.y = height - 1;

		UpdateLimits();
	}

	BViewport::BViewport(const std::shared_ptr<BCanvas>& canvas, const uint32& x, const uint32& y, const uint32& width, const uint32& height)
	{
		this->canvas = canvas;

		bounds.p1.x = x;
		bounds.p1.y = y;
		bounds.p2.x = x + width - 1;
		bounds.p2.y = y + height - 1;

		UpdateLimits();
	}

	void BViewport::SetNormalizedSize(const BBox<BVector2<float>>& normalizedSize)
	{
		auto canvasPtr = canvas.lock();

		BGL_ASSERT(canvasPtr != nullptr && "Got null canvas pointer when setting new normalized size to viewport!");

		BSize canvasRes(canvasPtr->GetWidth(), canvasPtr->GetHeight());

		normalSize = normalizedSize;

		bounds.p1.x = static_cast<uint32>(normalSize.p1.x * static_cast<float>(canvasRes.width));
		bounds.p1.y = static_cast<uint32>(normalSize.p1.y * static_cast<float>(canvasRes.height));
		bounds.p2.x = static_cast<uint32>(normalSize.p2.x * static_cast<float>(canvasRes.width)) - 1;
		bounds.p2.y = static_cast<uint32>(normalSize.p2.y * static_cast<float>(canvasRes.height)) - 1;

		UpdateLimits();
	}

	const BBoxEdges& BViewport::GetLimits() const
	{
		return Limits;
	}

	std::weak_ptr<BCanvas>& BViewport::GetCanvas()
	{
		return canvas;
	}

	const bgl::BPixelPos BViewport::GetPosition() const
	{
		return BPixelPos(GetBounds().p1.x, GetBounds().p1.y);
	}

	const bgl::BSize<bgl::uint32> BViewport::GetSize() const
	{
		return BSize<uint32>(GetBounds().p2.x - GetBounds().p1.x, GetBounds().p2.y - GetBounds().p1.y);
	}

	bgl::CanvasDataType& BViewport::operator()(size_t x, size_t y)
	{
		auto canvasPtr = canvas.lock();
		auto width = canvasPtr->GetWidth();
		auto& buffer = canvasPtr->GetBuffer();
		return buffer[(bounds.p1.x + x) + width * (bounds.p1.y + y)];
	}

	void BViewport::SetBounds(const BBox<BPixelPos>& newBounds)
	{
		bounds = newBounds;
		UpdateLimits();
	}

	const BBox<BPixelPos>& BViewport::GetBounds() const
	{
		return bounds;
	}

}