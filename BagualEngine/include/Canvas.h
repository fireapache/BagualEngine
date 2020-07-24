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
		ushort width;
		ushort height;

		BLine<BPixelPos> edges[4];

		std::shared_ptr<BBuffer<FrameDataType>> buffer;

		inline void UpdateEdges();

		// TODO: Use smart pointer instead
		BPlatformWindow* window;

	public:

		BCanvas(ushort width, ushort height);
		BCanvas(BPlatformWindow* window, void* pixels, ushort width, ushort height);
		//BCanvas(const BCanvas& canvas);

		ushort GetWidth() const;
		ushort GetHeight() const;

		const BLine<BPixelPos>* GetEdges()  const;
		const BLine<BPixelPos>& GetEdge(BEBoxEdge edge)  const;

		BBuffer<FrameDataType>& GetBuffer();

		BPlatformWindow* GetWindow();

	};
}