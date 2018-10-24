#pragma once

#include "BCameraSettings.h"
#include "BMath.h"

using namespace Bagual::Camera;
using namespace Bagual::Types;

namespace Bagual
{
	namespace Draw
	{
		bool IsLineOnScreen(BCameraSettings &camera, BLine<BPixel> &line);

		void DrawLine(BCameraSettings &camera, const BLine<BPixel> &line);

		void DrawLine(BCameraSettings &camera, const BPixel &p1, const BPixel &p2);
	}
}
