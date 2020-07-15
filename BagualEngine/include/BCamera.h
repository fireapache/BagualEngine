#pragma once

#include "BTypes.h"
#include "BSettings.h"
#include "BGraphicsPlatform.h"
#include "BTypes.h"

namespace bgl
{
	class BCamera
	{
		std::shared_ptr<bgl::BCanvas> canvas;

		bgl::BArray<bgl::BLine<bgl::BPixelPos>> Line2DBuffer;

	public:

		BCamera();

		// Line to be render on top of everything during camera render loop in the Renderer
		void AddLine2DBuffer(const bgl::BLine<bgl::BPixelPos>& line);

		bgl::BCanvas& GetCanvas();

		bgl::BArray<bgl::BLine<bgl::BPixelPos>>& GetLine2DBuffer();

		void ClearLine2DBuffer();
	};
}