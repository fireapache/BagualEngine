#include "BDraw.h"

using namespace Bagual::Math;

namespace Bagual
{
	namespace Draw
	{

		bool IsLineOnScreen(BCameraSettings &camera, BLine<BPixel> &line)
		{
			BBox<BPixel> viewport(
				BPixel(0, 0),
				BPixel(camera.GetWidth() - 1, camera.GetHeight() - 1));

			bool p1In = viewport.IsIn(line.p1);
			bool p2In = viewport.IsIn(line.p2);

			if (p1In && p2In) return true;

			bool isValid = true;
			bool isIn;
			BPixel inters[4];
			int p1Dist = INT_MAX;
			int p2Dist = INT_MAX;
			int dist;
			BPixel *newP1 = nullptr;
			BPixel *newP2 = nullptr;

			for (int i = 0; i < 4; i++)
			{
				if (LinesIntersection(line, camera.GetScreenEdge(i), inters[i]))
				{
					isIn = viewport.IsIn(inters[i]);

					if (!p1In && isIn)
					{
						dist = static_cast<int>(line.p1 | inters[i]);

						if (dist < p1Dist)
						{
							p1Dist = dist;
							newP1 = &inters[i];
						}
					}

					if (!p2In && isIn)
					{
						dist = static_cast<int>(line.p2 | inters[i]);

						if (dist < p2Dist)
						{
							p2Dist = dist;
							newP2 = &inters[i];
						}
					}
				}
			}

			if (!p1In && newP1) line.p1 = *newP1;
			else isValid = false;

			if (!p2In && newP2) line.p2 = *newP2;
			else isValid = false;

			return isValid;
		}

		void DrawLine(BCameraSettings &camera, const BLine<BPixel> &line)
		{
			int x, y;
			int *screen = camera.GetScreen();
			const int width = camera.GetWidth();
			BLine<BPixel> l(line);

			if (IsLineOnScreen(camera, l) == false) return;

			const float dist = l.p1 | l.p2;
			const float step = 1.0f / dist;

			for (float a = 0.0f; a <= 1.0f; a += step)
			{
				x = Lerp(l.p1.x, l.p2.x, a);
				y = Lerp(l.p1.y, l.p2.y, a);
				screen[x + width * y] = 0xFF0000;
			}

		}

		void DrawLine(BCameraSettings &camera, const BPixel &p1, const BPixel &p2)
		{
			DrawLine(camera, BLine<BPixel>(p1, p2));
		}

	}
}