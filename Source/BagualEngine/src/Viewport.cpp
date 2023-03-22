
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "Viewport.h"

#include "Canvas.h"

namespace bgl
{

	void BViewport::UpdateLimits()
	{
		const BPixelPos edge0_0( m_bounds.p1.x, m_bounds.p1.y );
		const BPixelPos edge0_1( m_bounds.p2.x, m_bounds.p1.y );

		m_limits[ BEBoxEdge::Top ] = BLine< BPixelPos >( edge0_0, edge0_1 );

		const BPixelPos edge1_0( m_bounds.p2.x, m_bounds.p2.y );
		const BPixelPos edge1_1( m_bounds.p1.x, m_bounds.p2.y );

		m_limits[ BEBoxEdge::Bottom ] = BLine< BPixelPos >( edge1_0, edge1_1 );

		const BPixelPos edge2_0( m_bounds.p1.x, m_bounds.p2.y );
		const BPixelPos edge2_1( m_bounds.p1.x, m_bounds.p1.y );

		m_limits[ BEBoxEdge::Left ] = BLine< BPixelPos >( edge2_0, edge2_1 );

		const BPixelPos edge3_0( m_bounds.p2.x, m_bounds.p1.y );
		const BPixelPos edge3_1( m_bounds.p2.x, m_bounds.p2.y );

		m_limits[ BEBoxEdge::Right ] = BLine< BPixelPos >( edge3_0, edge3_1 );
	}

	BViewport::BViewport( BCanvas* canvas )
	{
		if( canvas )
		{
			canvas->getViewports().add( this );
		}

		m_canvas = canvas;

		m_bounds.p1.x = 0;
		m_bounds.p1.y = 0;
		m_bounds.p2.x = canvas->getWidth() - 1;
		m_bounds.p2.y = canvas->getHeight() - 1;

		UpdateLimits();
	}

	BViewport::BViewport( BCanvas* canvas, const BBox< BVector2< float > > normalizedSize )
	{
		if( canvas )
		{
			canvas->getViewports().add( this );
		}

		m_canvas = canvas;
		SetNormalizedSize( normalizedSize );
	}

	BViewport::BViewport( BCanvas* canvas, const uint32_t width, const uint32_t height )
	{
		if( canvas )
		{
			canvas->getViewports().add( this );
		}

		m_canvas = canvas;

		m_bounds.p1.x = 0;
		m_bounds.p1.y = 0;
		m_bounds.p2.x = width - 1;
		m_bounds.p2.y = height - 1;

		UpdateLimits();
	}

	BViewport::BViewport( BCanvas* canvas, const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height )
	{
		if( canvas )
		{
			canvas->getViewports().add( this );
		}

		m_canvas = canvas;

		m_bounds.p1.x = x;
		m_bounds.p1.y = y;
		m_bounds.p2.x = x + width - 1;
		m_bounds.p2.y = y + height - 1;

		UpdateLimits();
	}

	BViewport::~BViewport()
	{
		if( m_canvas )
		{
			m_canvas->getViewports().remove( this );
		}
	}

	void BViewport::SetNormalizedSize( const BBox< BVector2< float > > normalizedSize )
	{
		BGL_ASSERT( m_canvas != nullptr && "Got null canvas pointer when setting new normalized size to viewport!" );

		BSize canvasRes( m_canvas->getWidth(), m_canvas->getHeight() );

		m_normalSize = normalizedSize;

		m_bounds.p1.x = static_cast< uint32_t >( m_normalSize.p1.x * static_cast< float >( canvasRes.width ) );
		m_bounds.p1.y = static_cast< uint32_t >( m_normalSize.p1.y * static_cast< float >( canvasRes.height ) );
		m_bounds.p2.x = static_cast< uint32_t >( m_normalSize.p2.x * static_cast< float >( canvasRes.width ) ) - 1;
		m_bounds.p2.y = static_cast< uint32_t >( m_normalSize.p2.y * static_cast< float >( canvasRes.height ) ) - 1;

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

	BCamera* BViewport::GetCamera() const
	{
		return m_camera;
	}

	void BViewport::SetCamera( BCamera* camera )
	{
		m_camera = camera;
	}

	BPixelPos BViewport::GetPosition() const
	{
		const auto bounds = GetBounds();
		return { bounds.p1.x, bounds.p1.y };
	}

	BSize< uint32_t > BViewport::GetSize() const
	{
		const auto bounds = GetBounds();
		return { bounds.p2.x - bounds.p1.x + static_cast< uint32_t >( 1 ),
				 bounds.p2.y - bounds.p1.y + static_cast< uint32_t >( 1 ) };
	}

	Color32Bit& BViewport::operator()( const size_t x, const size_t y )
	{
		if( !m_canvas )
		{
			BGL_LOG( "Got null canvas when setting color buffer value!" );
			return m_dummyData;
		}

		const uint16_t width = m_canvas->getWidth();
		auto& colorBuffer = m_canvas->getColorBuffer();
		const size_t index = ( m_bounds.p1.x + x ) + width * ( m_bounds.p1.y + y );
		const size_t lastIndex = colorBuffer.Length() - 1;
		if( index <= lastIndex )
		{
			return colorBuffer[ index ];
		}
		else
		{
			return colorBuffer[ lastIndex ];
		}
	}

	DepthDataType BViewport::GetPixelDepth( const size_t x, const size_t y )
	{
		if( !m_canvas )
		{
			BGL_LOG( "Got null canvas when getting pixel depth value!" );
			return DepthDataType();
		}

		auto width = m_canvas->getWidth();
		auto& zBuffer = m_canvas->getZBuffer();
		return zBuffer[ ( m_bounds.p1.x + x ) + width * ( m_bounds.p1.y + y ) ];
	}

	void BViewport::SetPixelDepth( size_t x, size_t y, DepthDataType value )
	{
		if( !m_canvas )
		{
			BGL_LOG( "Got null canvas when setting pixel depth value!" );
			return;
		}

		const auto width = m_canvas->getWidth();
		auto& zBuffer = m_canvas->getZBuffer();
		zBuffer[ ( m_bounds.p1.x + x ) + width * ( m_bounds.p1.y + y ) ] = value;
	}

	void BViewport::ResetPixelDepth()
	{
		if( m_canvas )
		{
			m_canvas->resetZBuffer();
		}
		else
		{
			BGL_LOG( "No ZBuffer canvas to reset!" );
		}
	}

	void BViewport::SetBounds( const BBox< BPixelPos >& newBounds )
	{
		m_bounds = newBounds;
		UpdateLimits();
	}

	const BBox< BPixelPos >& BViewport::GetBounds() const
	{
		return m_bounds;
	}

} // namespace bgl