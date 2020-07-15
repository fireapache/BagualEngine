#pragma once

#include "Camera.h"
#include "Arithmetics.h"
#include "Types.h"

namespace bgl
{
	bool IsLineOnScreen(BCamera& camera, BLine<BPixelPos>& line);

	void DrawLine(BCamera& camera, const BLine<BPixelPos> &line);

	void DrawLine(BCamera &camera, const BPixelPos &p1, const BPixelPos &p2);
}
