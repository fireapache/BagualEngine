
#include "Bagual.pch.h"

#include "Viewport.h"
#include "Canvas.h"

namespace bgl
{

	void BViewport::UpdateLimits()
	{
		const BPixelPos edge0_0(position.x, position.y);
		const BPixelPos edge0_1(size.width - 1, position.y);

		Limits[BEBoxEdge::Top] = BLine<BPixelPos>(edge0_0, edge0_1);

		const BPixelPos edge1_0(size.width - 1, size.height - 1);
		const BPixelPos edge1_1(position.x, size.height - 1);

		Limits[BEBoxEdge::Bottom] = BLine<BPixelPos>(edge1_0, edge1_1);

		const BPixelPos edge2_0(position.x, size.height - 1);
		const BPixelPos edge2_1(position.x, position.y);

		Limits[BEBoxEdge::Left] = BLine<BPixelPos>(edge2_0, edge2_1);

		const BPixelPos edge3_0(size.width - 1, position.y);
		const BPixelPos edge3_1(size.width - 1, size.height - 1);

		Limits[BEBoxEdge::Right] = BLine<BPixelPos>(edge3_0, edge3_1);
	}

	BViewport::BViewport(const std::shared_ptr<BCanvas>& canvas)
	{
		this->canvas = canvas;

		position.x = 0;
		position.y = 0;
		size.width = canvas->GetWidth();
		size.height = canvas->GetHeight();

		UpdateLimits();
	}

	BViewport::BViewport(const std::shared_ptr<BCanvas>& canvas, const BBox<BVector2<float>>& normalizedSize)
	{
		this->canvas = canvas;
		SetNormalizedSize(normalizedSize);
	}

	BViewport::BViewport(const std::shared_ptr<BCanvas>& canvas, const uint& width, const uint& height)
	{
		this->canvas = canvas;

		position.x = 0;
		position.y = 0;
		size.width = width;
		size.height = height;

		UpdateLimits();
	}

	BViewport::BViewport(const std::shared_ptr<BCanvas>& canvas, const uint& x, const uint& y, const uint& width, const uint& height)
	{
		this->canvas = canvas;

		position.x = x;
		position.y = y;
		size.width = width;
		size.height = height;

		UpdateLimits();
	}

	void BViewport::SetNormalizedSize(const BBox<BVector2<float>>& normalizedSize)
	{
		auto canvasPtr = canvas.lock();

		BGL_ASSERT(canvasPtr != nullptr && "Got null canvas pointer when setting new normalized size to viewport!");

		BSize canvasRes(canvasPtr->GetWidth(), canvasPtr->GetHeight());

		normalSize = normalizedSize;

		position.x = static_cast<uint>(normalSize.p1.x * static_cast<float>(canvasRes.width));
		position.y = static_cast<uint>(normalSize.p1.y * static_cast<float>(canvasRes.height));
		size.width = static_cast<uint>(normalSize.p2.x * static_cast<float>(canvasRes.width));
		size.height = static_cast<uint>(normalSize.p2.y * static_cast<float>(canvasRes.height));

		UpdateLimits();
	}

	const BBoxEdges& BViewport::GetLimits() const
	{
		return Limits;
	}

	const BVector2<uint>& BViewport::GetPosition() const
	{
		return position;
	}

	std::weak_ptr<BCanvas>& BViewport::GetCanvas()
	{
		return canvas;
	}

	const BSize<uint>& BViewport::GetSize() const
	{
		return size;
	}

	void BViewport::SetPosition(const BVector2<uint>& pos)
	{
		position = pos;
		UpdateLimits();
	}

	void BViewport::SetSize(const BSize<uint>& size)
	{
		this->size = size;
		UpdateLimits();
	}

	void BViewport::SetBounds(const BVector2<uint>& pos, const BSize<uint>& size)
	{
		position = pos;
		this->size = size;
		UpdateLimits();
	}

	BBox<BPixelPos> BViewport::GetBounds() const
	{
		return BBox<BPixelPos>(
			BPixelPos(position.x, position.y),
			BPixelPos(size.width - 1, size.height - 1));
	}

}