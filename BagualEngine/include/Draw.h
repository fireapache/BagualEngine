#pragma once

#include "Common.h"

#include "Camera.h"
#include "Arithmetics.h"

namespace bgl
{
	
	/*	Checks is a specific line is on screen, correcting the line's
	 *	properties to make sure it only appears in the visible area.
	 *
	 *	It returns false in case the line completely misses the visible
	 *	area of the camera's viewport.
	 */
	bool IsLineOnScreen(BCamera& camera, BLine<BPixelPos>& line);

	/*	Draw a line on the camera's viewport. */
	void DrawLine(BCamera& camera, const BLine<BPixelPos> &line);

	/*	Draw a line between pixel positions on the camera's viewport */
	void DrawLine(BCamera &camera, const BPixelPos &p1, const BPixelPos &p2);
}
