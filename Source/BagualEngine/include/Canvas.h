#pragma once

#include "Common.h"

namespace bgl
{
	class BPlatformWindow;
}

namespace bgl
{
	class BCanvas
	{
		uint16 width;
		uint16 height;

		BLine<BPixelPos> edges[4];

		std::shared_ptr<BBuffer<CanvasDataType>> buffer;

		inline void UpdateEdges();

		// TODO: Use smart pointer instead
		BPlatformWindow* window;

	public:

		BCanvas(BPlatformWindow* window, uint16 width, uint16 height);
		BCanvas(BPlatformWindow* window, void* pixels, uint8 width, uint8 height);
		//BCanvas(const BCanvas& canvas);

		uint16 GetWidth() const;
		uint16 GetHeight() const;

		const BLine<BPixelPos>* GetEdges()  const;
		const BLine<BPixelPos>& GetEdge(BEBoxEdge edge)  const;

		BBuffer<CanvasDataType>& GetBuffer();

		BPlatformWindow* GetWindow();

		void SetSize(uint16 newWidth, uint16 newHeight);

	};
}