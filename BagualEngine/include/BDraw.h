#pragma once

#include "BCameraSettings.h"
#include "BMath.h"

namespace Bagual::Draw
{
	bool IsLineOnScreen(Bagual::Camera::BCameraSettings &camera, BLine<BPixel> &line);

	void DrawLine(Bagual::Camera::BCameraSettings &camera, const BLine<BPixel> &line);

	void DrawLine(Bagual::Camera::BCameraSettings &camera, const BPixel &p1, const BPixel &p2);
}
