#pragma once

#include "Common.h"

#include "PlatformBase.h"

namespace bgl
{
	class BViewport;
	class BPlatformWindow;
}

namespace bgl
{
	class BCamera
	{
		std::shared_ptr<BViewport> viewport;

		BArray<BLine<BPixelPos>> Line2DBuffer;

	public:

		BCamera();
		BCamera(const std::shared_ptr<BViewport>& viewport);

		// Line to be render on top of everything during camera render loop in the Renderer
		void AddLine2DBuffer(const BLine<BPixelPos>& line);

		BViewport& GetViewport();

		BArray<BLine<BPixelPos>>& GetLine2DBuffer();

		void ClearLine2DBuffer();
	};
}