
#include "Bagual.pch.h"

#include "Viewport.h"
#include "Canvas.h"

namespace bgl
{

	void BViewport::UpdateLimits()
	{
		const BPixelPos edge0_0(m_bounds.p1.x, m_bounds.p1.y);
		const BPixelPos edge0_1(m_bounds.p2.x, m_bounds.p1.y);

		m_limits[BEBoxEdge::Top] = BLine<BPixelPos>(edge0_0, edge0_1);

		const BPixelPos edge1_0(m_bounds.p2.x, m_bounds.p2.y);
		const BPixelPos edge1_1(m_bounds.p1.x, m_bounds.p2.y);

		m_limits[BEBoxEdge::Bottom] = BLine<BPixelPos>(edge1_0, edge1_1);

		const BPixelPos edge2_0(m_bounds.p1.x, m_bounds.p2.y);
		const BPixelPos edge2_1(m_bounds.p1.x, m_bounds.p1.y);

		m_limits[BEBoxEdge::Left] = BLine<BPixelPos>(edge2_0, edge2_1);

		const BPixelPos edge3_0(m_bounds.p2.x, m_bounds.p1.y);
		const BPixelPos edge3_1(m_bounds.p2.x, m_bounds.p2.y);

		m_limits[BEBoxEdge::Right] = BLine<BPixelPos>(edge3_0, edge3_1);
	}

	BViewport::BViewport(BCanvas* canvas)
	{
		m_canvas = canvas;

		m_bounds.p1.x = 0;
		m_bounds.p1.y = 0;
		m_bounds.p2.x = canvas->GetWidth() - 1;
		m_bounds.p2.y = canvas->GetHeight() - 1;

		UpdateLimits();
	}

	BViewport::BViewport(BCanvas* canvas, const BBox<BVector2<float>>& normalizedSize)
	{
		m_canvas = canvas;
		SetNormalizedSize(normalizedSize);
	}

	BViewport::BViewport(BCanvas* canvas, const uint32& width, const uint32& height)
	{
		m_canvas = canvas;

		m_bounds.p1.x = 0;
		m_bounds.p1.y = 0;
		m_bounds.p2.x = width - 1;
		m_bounds.p2.y = height - 1;

		UpdateLimits();
	}

	BViewport::BViewport(BCanvas* canvas, const uint32& x, const uint32& y, const uint32& width, const uint32& height)
	{
		m_canvas = canvas;

		m_bounds.p1.x = x;
		m_bounds.p1.y = y;
		m_bounds.p2.x = x + width - 1;
		m_bounds.p2.y = y + height - 1;

		UpdateLimits();
	}

	BViewport::~BViewport()
	{

	}

	void BViewport::SetNormalizedSize(const BBox<BVector2<float>>& normalizedSize)
	{
		BGL_ASSERT(m_canvas != nullptr && "Got null canvas pointer when setting new normalized size to viewport!");

		BSize canvasRes(m_canvas->GetWidth(), m_canvas->GetHeight());

		m_normalSize = normalizedSize;

		m_bounds.p1.x = static_cast<uint32>(m_normalSize.p1.x * static_cast<float>(canvasRes.width));
		m_bounds.p1.y = static_cast<uint32>(m_normalSize.p1.y * static_cast<float>(canvasRes.height));
		m_bounds.p2.x = static_cast<uint32>(m_normalSize.p2.x * static_cast<float>(canvasRes.width)) - 1;
		m_bounds.p2.y = static_cast<uint32>(m_normalSize.p2.y * static_cast<float>(canvasRes.height)) - 1;

		UpdateLimits();
	}

	const BBoxEdges& BViewport::GetLimits() const
	{
		return m_limits;
	}

	BCanvas* BViewport::GetCanvas()
	{
		return m_canvas;
	}

	const bgl::BPixelPos BViewport::GetPosition() const
	{
		return BPixelPos(GetBounds().p1.x, GetBounds().p1.y);
	}

	const bgl::BSize<bgl::uint32> BViewport::GetSize() const
	{
		return BSize<uint32>(GetBounds().p2.x - GetBounds().p1.x, GetBounds().p2.y - GetBounds().p1.y);
	}

	CanvasDataType& BViewport::operator()(size_t x, size_t y)
	{
		if (!m_canvas)
		{
			BGL_LOG("Got null canvas when setting color buffer value!");
			return m_dummyData;
		}

		auto width = m_canvas->GetWidth();
		auto& colorBuffer = m_canvas->GetColorBuffer();
		const uint32 index = (m_bounds.p1.x + (int32)x) + (int32)width * (m_bounds.p1.y + (int32)y);
		const uint32 lastIndex = (int32)colorBuffer.Length() - 1;
		if (index <= lastIndex)
		{
			return colorBuffer[index];
		}
		else
		{
			return colorBuffer[lastIndex];
		}
	}

	DepthDataType BViewport::GetPixelDepth(size_t x, size_t y)
	{
		if (!m_canvas)
		{
			BGL_LOG("Got null canvas when getting pixel depth value!");
			return DepthDataType();
		}

		auto width = m_canvas->GetWidth();
		auto& zBuffer = m_canvas->GetZBuffer();
		return zBuffer[(m_bounds.p1.x + x) + width * (m_bounds.p1.y + y)];
	}

	void BViewport::SetPixelDepth(size_t x, size_t y, DepthDataType value)
	{
		if (!m_canvas)
		{
			BGL_LOG("Got null canvas when setting pixel depth value!");
			return;
		}

		auto width = m_canvas->GetWidth();
		auto& zBuffer = m_canvas->GetZBuffer();
		zBuffer[(m_bounds.p1.x + x) + width * (m_bounds.p1.y + y)] = value;
	}

	void BViewport::ResetPixelDepth()
	{
		if (m_canvas)
		{
			m_canvas->ResetZBuffer();
		}
		else
		{
			BGL_LOG("No ZBuffer canvas to reset!");
		}
	}

	void BViewport::SetBounds(const BBox<BPixelPos>& newBounds)
	{
		m_bounds = newBounds;
		UpdateLimits();
	}

	const BBox<BPixelPos>& BViewport::GetBounds() const
	{
		return m_bounds;
	}

}