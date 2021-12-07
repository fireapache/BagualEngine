
#include "Bagual.pch.h"

#include "Canvas.h"
#include "Settings.h"
#include "PlatformBase.h"

namespace bgl
{

	BCanvas::BCanvas(BPlatformWindow* window, uint16 width, uint16 height)
		: width(width), height(height)
	{
		this->colorBuffer = std::make_shared<BBuffer<CanvasDataType>>(static_cast<size_t>(width) * height);
		ResetZBuffer();

		this->window = window;

		UpdateEdges();

	}

	BCanvas::BCanvas(BPlatformWindow* window, void* pixels, uint8 width, uint8 height)
		: width(width), height(height)
	{
		this->colorBuffer = std::make_shared<BBuffer<CanvasDataType>>(static_cast<CanvasDataType*>(pixels), static_cast<size_t>(width) * height);
		ResetZBuffer();

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

	uint16 BCanvas::GetWidth() const
	{
		return width;
	}

	uint16 BCanvas::GetHeight() const
	{
		return height;
	}

	inline void BCanvas::ResetZBuffer()
	{
		this->zBuffer = std::make_shared<BBuffer<DepthDataType>>(static_cast<size_t>(width) * height);
	}

	const BLine<BPixelPos>* BCanvas::GetEdges() const
	{
		return edges;
	}

	const BLine<BPixelPos>& BCanvas::GetEdge(BEBoxEdge edge) const
	{
		return edges[static_cast<int>(edge)];
	}

	BBuffer<CanvasDataType>& BCanvas::GetColorBuffer()
	{
		return *colorBuffer.get();
	}

	BBuffer<DepthDataType>& BCanvas::GetZBuffer()
	{
		return *zBuffer.get();
	}

	BPlatformWindow* BCanvas::GetWindow()
	{
		return window;
	}

	void BCanvas::SetSize(uint16 newWidth, uint16 newHeight)
	{
		width = newWidth;
		height = newHeight;
		uint32 newLength = static_cast<uint32>(width) * static_cast<uint32>(height);
		colorBuffer->SetLength(newLength);
	}

}

