#pragma once

#include "Camera.h"
#include "Arithmetics.h"
#include "Types.h"

namespace bgl
{
	bool IsLineOnScreen(bgl::BCamera& camera, bgl::BLine<bgl::BPixelPos>& line);

	void DrawLine(bgl::BCamera& camera, const bgl::BLine<bgl::BPixelPos> &line);

	void DrawLine(bgl::BCamera &camera, const bgl::BPixelPos &p1, const bgl::BPixelPos &p2);
}
