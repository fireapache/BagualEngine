#pragma once

#include "Common.h"

namespace bgl
{
	class BCanvas;
}

namespace bgl
{
	class BCamera
	{
		std::shared_ptr<BCanvas> canvas;

		BArray<BLine<BPixelPos>> Line2DBuffer;

	public:

		BCamera();

		// Line to be render on top of everything during camera render loop in the Renderer
		void AddLine2DBuffer(const BLine<BPixelPos>& line);

		BCanvas& GetCanvas();

		BArray<BLine<BPixelPos>>& GetLine2DBuffer();

		void ClearLine2DBuffer();
	};
}