#pragma once

#include "Common.h"

namespace bgl
{
	class BCanvas;
}

namespace bgl
{
	
	class BViewport
	{
		BBox<BPixelPos> m_bounds;
		BBox<BVector2<float>> m_normalSize;
		BCanvas* m_canvas;
		BBoxEdges m_limits;

		void UpdateLimits();

		CanvasDataType m_dummyData;

	public:

		BViewport(BCanvas* canvas);
		BViewport(BCanvas* canvas, const BBox<BVector2<float>>& normalizedSize);
		BViewport(BCanvas* canvas, const uint32& width, const uint32& height);
		BViewport(BCanvas* canvas, const uint32& x, const uint32& y, const uint32& width, const uint32& height);
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
		void SetNormalizedSize(const BBox<BVector2<float>>& normalizedSize);

		const BBoxEdges& GetLimits() const;

		void SetBounds(const BBox<BPixelPos>& newBounds);

		const BBox<BPixelPos>& GetBounds() const;

		BCanvas* GetCanvas();

		const BPixelPos GetPosition() const;

		const BSize<uint32> GetSize() const;

		CanvasDataType& operator()(size_t x, size_t y);
		
		DepthDataType GetPixelDepth(size_t x, size_t y);
		void SetPixelDepth(size_t x, size_t y, DepthDataType value);
		void ResetPixelDepth();

	};

}

