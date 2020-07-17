
#include "Bagual.pch.h"

#include "Canvas.h"
#include "Settings.h"
#include "PlatformBase.h"

namespace bgl
{

	BCanvas::BCanvas(ushort width /*= 320*/, ushort height /*= 240*/)
		: width(width), height(height)
	{
		this->buffer = std::make_shared<BBuffer<uint>>(static_cast<size_t>(width * height));

		UpdateEdges();

	}

	BCanvas::BCanvas(BPlatformWindow* window, void* pixels, ushort width, ushort height)
		: width(width), height(height)
	{
		this->buffer = std::make_shared<BBuffer<uint>>(static_cast<uint*>(pixels), static_cast<size_t>(width * height));

		this->window = window;

		UpdateEdges();
	}

	void BCanvas::UpdateEdges()
	{
		const BPixelPos edge0_0(0, 0);
		const BPixelPos edge0_1(width - 1, 0);

		this->edges[0] = BLine<BPixelPos>(edge0_0, edge0_1);

		const BPixelPos edge1_0(width - 1, height - 1);
		const BPixelPos edge1_1(0, height - 1);

		this->edges[1] = BLine<BPixelPos>(edge1_0, edge1_1);

		const BPixelPos edge2_0(0, height - 1);
		const BPixelPos edge2_1(0, 0);

		this->edges[2] = BLine<BPixelPos>(edge2_0, edge2_1);

		const BPixelPos edge3_0(width - 1, 0);
		const BPixelPos edge3_1(width - 1, height - 1);

		this->edges[3] = BLine<BPixelPos>(edge3_0, edge3_1);
	}

	ushort BCanvas::GetWidth() const
	{
		return width;
	}

	ushort BCanvas::GetHeight() const
	{
		return height;
	}

	const BLine<BPixelPos>* BCanvas::GetEdges() const
	{
		return edges;
	}

	const BLine<BPixelPos>& BCanvas::GetEdge(BEBoxEdge edge) const
	{
		return edges[static_cast<int>(edge)];
	}

	BBuffer<uint>& BCanvas::GetBuffer()
	{
		return *buffer.get();
	}

	BPlatformWindow* BCanvas::GetWindow()
	{
		return window;
	}

}

