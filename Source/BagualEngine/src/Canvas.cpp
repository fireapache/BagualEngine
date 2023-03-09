
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "Canvas.h"

#include "PlatformBase.h"

namespace bgl
{

	BCanvas::BCanvas( BPlatformWindow* window, uint16_t width, uint16_t height )
		: width( width )
		, height( height )
	{
		allocateBuffers();
		this->window = window;
		UpdateEdges();
	}

	inline void BCanvas::UpdateEdges()
	{
		const BPixelPos edge0_0( 0, 0 );
		const BPixelPos edge0_1( width - 1, 0 );

		this->edges[ 0 ] = BLine< BPixelPos >( edge0_0, edge0_1 );

		const BPixelPos edge1_0( width - 1, height - 1 );
		const BPixelPos edge1_1( 0, height - 1 );

		this->edges[ 1 ] = BLine< BPixelPos >( edge1_0, edge1_1 );

		const BPixelPos edge2_0( 0, height - 1 );
		const BPixelPos edge2_1( 0, 0 );

		this->edges[ 2 ] = BLine< BPixelPos >( edge2_0, edge2_1 );

		const BPixelPos edge3_0( width - 1, 0 );
		const BPixelPos edge3_1( width - 1, height - 1 );

		this->edges[ 3 ] = BLine< BPixelPos >( edge3_0, edge3_1 );
	}

	uint16_t BCanvas::getWidth() const
	{
		return width;
	}

	uint16_t BCanvas::getHeight() const
	{
		return height;
	}

	void BCanvas::allocateBuffers()
	{
		const size_t frameBufferLength = static_cast< size_t >( width ) * static_cast< size_t >( height );
		this->colorBuffer = std::make_shared< BBuffer< Color32Bit > >( frameBufferLength, DEFAULT_CANVAS_VALUE );
		this->zBuffer = std::make_shared< BBuffer< DepthDataType > >( frameBufferLength, DEFAULT_DEPTH_VALUE );
		this->wireframeBuffer = std::make_shared< BBuffer< Color32Bit > >( frameBufferLength, DEFAULT_CANVAS_VALUE );
		this->readyFrameBuffer = std::make_shared< BBuffer< Color32Bit > >( frameBufferLength, DEFAULT_CANVAS_VALUE );
	}

	void BCanvas::resetZBuffer()
	{
		this->zBuffer->SetBufferValue( DEFAULT_DEPTH_VALUE );
	}

	void BCanvas::resetWireframeBuffer()
	{
		this->wireframeBuffer->SetBufferValue( DEFAULT_CANVAS_VALUE );
	}

	const BLine< BPixelPos >* BCanvas::getEdges() const
	{
		return edges;
	}

	const BLine< BPixelPos >& BCanvas::getEdge( BEBoxEdge edge ) const
	{
		return edges[ static_cast< int32_t >( edge ) ];
	}

	BBuffer< Color32Bit >& BCanvas::getColorBuffer() const
	{
		return *colorBuffer;
	}

	BBuffer< DepthDataType >& BCanvas::getZBuffer() const
	{
		return *zBuffer;
	}

	BBuffer< Color32Bit >& BCanvas::getWireframeBuffer() const
	{
		return *wireframeBuffer;
	}

	BBuffer< Color32Bit >& BCanvas::getReadyFrameBuffer() const
	{
		return *readyFrameBuffer;
	}

	BPlatformWindow* BCanvas::getWindow() const
	{
		return window;
	}

	BArray< BViewport* > BCanvas::getViewports() const
	{
		return m_viewports;
	}

	void BCanvas::setSize( const uint16_t newWidth, const uint16_t newHeight )
	{
		width = newWidth;
		height = newHeight;
		const uint32_t newLength = static_cast< uint32_t >( width ) * static_cast< uint32_t >( height );
		colorBuffer->SetLength( newLength );
	}

} // namespace bgl
