#pragma once

#include "BCamera.h"
#include "BMath.h"
#include "BTypes.h"

namespace Bagual::Draw
{
	bool IsLineOnScreen(Bagual::Camera::BCamera& camera, Bagual::Types::BLine<Bagual::Types::BPixelPos>& line);

	void DrawLine(Bagual::Camera::BCamera& camera, const Bagual::Types::BLine<Bagual::Types::BPixelPos> &line);

	void DrawLine(Bagual::Camera::BCamera &camera, const Bagual::Types::BPixelPos &p1, const Bagual::Types::BPixelPos &p2);
}
