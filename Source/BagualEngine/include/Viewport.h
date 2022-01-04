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
		BBox<BPixelPos> bounds;

		BBox<BVector2<float>> normalSize;

		std::weak_ptr<BCanvas> canvas;

		BBoxEdges Limits;

		void UpdateLimits();

	public:

		BViewport(const std::shared_ptr<BCanvas>& canvas);
		BViewport(const std::shared_ptr<BCanvas>& canvas, const BBox<BVector2<float>>& normalizedSize);
		BViewport(const std::shared_ptr<BCanvas>& canvas, const uint32& width, const uint32& height);
		BViewport(const std::shared_ptr<BCanvas>& canvas, const uint32& x, const uint32& y, const uint32& width, const uint32& height);

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

		std::weak_ptr<BCanvas>& GetCanvas();

		const BPixelPos GetPosition() const;

		const BSize<uint32> GetSize() const;

		CanvasDataType& operator()(size_t x, size_t y);
		
		DepthDataType GetPixelDepth(size_t x, size_t y);
		void SetPixelDepth(size_t x, size_t y, DepthDataType value);
		void ResetPixelDepth();

#ifdef TRIANGLE_SOURCE
		uint32 GetMeshSource(size_t x, size_t y);
		void SetMeshSource(size_t x, size_t y, uint32 meshID);
#endif

	};

}

