#pragma once

#include "BCameraSettings.h"
#include "BMath.h"

namespace Bagual::Draw
{
	bool IsLineOnScreen(Bagual::Camera::BCameraSettings &cameraSettings, BLine<BPixelPos> &line);

	void DrawLine(Bagual::Camera::BCameraSettings &cameraSettings, const BLine<BPixelPos> &line);

	void DrawLine(Bagual::Camera::BCameraSettings &cameraSettings, const BPixelPos &p1, const BPixelPos &p2);
}
