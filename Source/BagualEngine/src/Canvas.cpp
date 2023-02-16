
#include "Bagual.pch.h"

#include "Canvas.h"
#include "Settings.h"
#include "PlatformBase.h"

namespace bgl
{

	BCanvas::BCanvas(BPlatformWindow* window, uint16 width, uint16 height)
		: width(width), height(height)
	{
		AllocateBuffers();
		this->window = window;
		UpdateEdges();
	}
	
	inline void BCanvas::UpdateEdges()
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

	void BCanvas::AllocateBuffers()
	{
		const size_t frameBufferLength = static_cast<size_t>(width) * static_cast<size_t>(height);
		this->colorBuffer = std::make_shared<BBuffer<CanvasDataType>>(frameBufferLength, static_cast<CanvasDataType>(0));
		this->zBuffer = std::make_shared<BBuffer<DepthDataType>>(frameBufferLength, static_cast<DepthDataType>(9999999999999));
		this->wireframeBuffer = std::make_shared<BBuffer<CanvasDataType>>(frameBufferLength, static_cast<CanvasDataType>(0));
		this->readyFrameBuffer = std::make_shared<BBuffer<CanvasDataType>>(frameBufferLength, static_cast<CanvasDataType>(0));
	}

	void BCanvas::ResetZBuffer()
	{
		this->zBuffer->SetBufferValue(static_cast<DepthDataType>(999999999999));
	}

	void BCanvas::ResetWireframeBuffer()
	{
		this->wireframeBuffer->SetBufferValue(static_cast<CanvasDataType>(0));
	}

	const BLine<BPixelPos>* BCanvas::GetEdges() const
	{
		return edges;
	}

	const BLine<BPixelPos>& BCanvas::GetEdge(BEBoxEdge edge) const
	{
		return edges[static_cast<int32_t>(edge)];
	}

	BBuffer<CanvasDataType>& BCanvas::GetColorBuffer() const
	{
		return *colorBuffer;
	}

	BBuffer<DepthDataType>& BCanvas::GetZBuffer() const
	{
		return *zBuffer;
	}

	BBuffer<CanvasDataType>& BCanvas::GetWireframeBuffer() const
	{
		return *wireframeBuffer;
	}

	BBuffer<CanvasDataType>& BCanvas::GetReadyFrameBuffer() const
	{
		return *readyFrameBuffer;
	}

	BPlatformWindow* BCanvas::GetWindow() const
	{
		return window;
	}

	inline BArray<BViewport*> BCanvas::GetViewports() const
	{
		return m_viewports;
	}

	void BCanvas::SetSize(uint16 newWidth, uint16 newHeight)
	{
		width = newWidth;
		height = newHeight;
		const uint32 newLength = static_cast<uint32>(width) * static_cast<uint32>(height);
		colorBuffer->SetLength(newLength);
	}

}

