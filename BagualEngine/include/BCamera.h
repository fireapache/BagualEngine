#pragma once

#include "BTypes.h"
#include "BSettings.h"
#include "BGraphicsPlatform.h"
#include "BTypes.h"

namespace Bagual::Camera
{
	class BCamera
	{
		std::shared_ptr<Bagual::Graphics::BCanvas> canvas;

		Bagual::Types::BArray<Bagual::Types::BLine<Bagual::Types::BPixelPos>> Line2DBuffer;

	public:

		BCamera();

		// Line to be render on top of everything during camera render loop in the Renderer
		void AddLine2DBuffer(const Bagual::Types::BLine<Bagual::Types::BPixelPos>& line);

		Bagual::Graphics::BCanvas& GetCanvas();

		Bagual::Types::BArray<Bagual::Types::BLine<Bagual::Types::BPixelPos>>& GetLine2DBuffer();

		void ClearLine2DBuffer();
	};
}