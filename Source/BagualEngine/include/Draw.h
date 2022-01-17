#pragma once

#include "Common.h"

#include "Camera.h"
#include "Arithmetics.h"

namespace bgl
{
	class BDraw
	{

	public:

		/*	Checks is a specific line is on screen, correcting the line's
		 *	properties to make sure it only appears in the visible area.
		 *
		 *	It returns false in case the line completely misses the visible
		 *	area of the camera's viewport.
		 */
		static bool IsLineOnScreen(BCamera* camera, BLine<BPixelPos>& line);

		/*	Draw a line on the camera's viewport. */
		static void DrawLine(BCamera* camera, const BLine<BPixelPos> line);

		/*	Draw a line between pixel positions on the camera's viewport */
		static void DrawLine(BCamera* camera, const BPixelPos& p1, const BPixelPos& p2);

		// [comment]
		// The main ray-triangle intersection routine.
		// You can test both methods: the geometric method and the Moller-Trumbore algorithm
		// (use the flag -DMOLLER_TRUMBORE when you compile)
		// [/comment]
		static bool RayTriangleIntersect(
			const BVec3f orig, const BVec3f dir,
			const BVec3f v0, const BVec3f v1, const BVec3f v2,
			float& t, float& u, float& v);
	};
}
