#pragma once

#include "Common.h"

namespace bgl
{
	class BCanvas;
	class BCamera;
} // namespace bgl

namespace bgl
{

	class BViewport
	{
		BBox< BPixelPos > m_bounds;
		BBox< BVector2< float > > m_normalSize;
		BCanvas* m_canvas;
		BBoxEdges m_limits;
		BCamera* m_camera;

		void UpdateLimits();

		CanvasDataType m_dummyData;

	public:
		BViewport( BCanvas* canvas );
		BViewport( BCanvas* canvas, const BBox< BVector2< float > > normalizedSize );
		BViewport( BCanvas* canvas, const uint32 width, const uint32 height );
		BViewport( BCanvas* canvas, const uint32 x, const uint32 y, const uint32 width, const uint32 height );
		~BViewport();

		/*	Calculates viewport's position in the canvas based on its
		 *	normalized size if set.
		 *
		 *	Ex.:
		 *
		 *		Canvas Resolution = 320x240
		 *		Viewport's Normalized Size = (0.5, 0.5) (1.0, 1.0)
		 *
		 *		then
		 *
		 *		Viewport Position in Canvas = (160, 120) (320 - 1, 240 - 1)
		 */
		void SetNormalizedSize( const BBox< BVector2< float > > normalizedSize );

		[[nodiscard]] const BBoxEdges& GetLimits() const;

		void SetBounds( const BBox< BPixelPos >& newBounds );

		[[nodiscard]] const BBox< BPixelPos >& GetBounds() const;

		[[nodiscard]] BCanvas* GetCanvas();

		[[nodiscard]] BCamera* GetCamera() const;
		void SetCamera( BCamera* camera );

		[[nodiscard]] BPixelPos GetPosition() const;

		[[nodiscard]] BSize< uint32 > GetSize() const;

		CanvasDataType& operator()( const size_t x, const size_t y );

		DepthDataType GetPixelDepth( const size_t x, const size_t y );
		void SetPixelDepth( size_t x, size_t y, DepthDataType value );
		void ResetPixelDepth();
	};

} // namespace bgl
