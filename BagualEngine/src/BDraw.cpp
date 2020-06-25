
#include "BDraw.h"
#include "BCamera.h"

using namespace Bagual::Math;

namespace Bagual::Draw
{
	
	bool IsLineOnScreen(Bagual::Camera::BCamera& camera, BLine<BPixelPos>& line)
	{
		BBox<BPixelPos> viewport(
			BPixelPos(0, 0),
			BPixelPos(camera.GetCanvas().GetWidth() - 1, camera.GetCanvas().GetHeight() - 1));

		bool p1In = viewport.IsIn(line.p1);
		bool p2In = viewport.IsIn(line.p2);

		if (p1In && p2In) return true;

		bool isValid = true;
		bool isIn;
		BPixelPos inters[4];
		int p1Dist = INT_MAX;
		int p2Dist = INT_MAX;
		int dist;
		BPixelPos* newP1 = nullptr;
		BPixelPos* newP2 = nullptr;

		for (int i = 0; i < 4; i++)
		{
			if (LinesIntersection(line, camera.GetCanvas().GetEdge(static_cast<BEBoxEdges>(i)), inters[i]))
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

	void DrawLineLow(Bagual::Camera::BCamera& camera, const BLine<BPixelPos>& line)
	{
		auto screen = camera.GetCanvas().GetBuffer();
		const unsigned short width = camera.GetCanvas().GetWidth();
		int dx = line.p2.x - line.p1.x;
		int dy = line.p2.y - line.p1.y;
		int yi = 1;

		if (dy < 0)
		{
			yi = -1;
			dy = -dy;
		}

		int D = 2 * dy - dx;
		int y = line.p1.y;

		for (int x = line.p1.x; x <= line.p2.x; x++)
		{
			screen[x + width * y] = 0xFF0000;

			if (D > 0)
			{
				y = y + yi;
				D = D - 2 * dx;
			}

			D = D + 2 * dy;
		}
	}

	void DrawLineHigh(Bagual::Camera::BCamera& camera, const BLine<BPixelPos>& line)
	{
		auto screen = camera.GetCanvas().GetBuffer();
		const int width = camera.GetCanvas().GetWidth();
		int dx = line.p2.x - line.p1.x;
		int dy = line.p2.y - line.p1.y;
		int xi = 1;

		if (dx < 0)
		{
			xi = -1;
			dx = -dx;
		}

		int D = 2 * dx - dy;
		int x = line.p1.x;

		for (int y = line.p1.y; y <= line.p2.y; y++)
		{
			screen[x + width * y] = 0xFF0000;

			if (D > 0)
			{
				x = x + xi;
				D = D - 2 * dy;
			}

			D = D + 2 * dx;
		}
	}

	void DrawLine(Bagual::Camera::BCamera& camera, const BLine<BPixelPos>& line)
	{
		// Implementing Bresenham's algorithm (https://en.wikipedia.org/wiki/Bresenham)

		// Getting a copy so we can change its values
		BLine<BPixelPos> l(line);

		//std::cout << "P1: (" << line.p1.x << "," << line.p1.y << ") P2: (" << line.p2.x << "," << line.p2.y << ")" << std::endl;

		// Checks if line is on screen and clamps it
		if (IsLineOnScreen(camera, l) == false) return;

		//std::cout << "P1: (" << l.p1.x << "," << l.p1.y << ") P2: (" << l.p2.x << "," << l.p2.y << ")" << std::endl;

		if (std::abs(line.p2.y - line.p1.y) < std::abs(line.p2.x - line.p1.x))
		{
			if (line.p1.x > line.p2.x)
			{
				DrawLineLow(camera, -l);
			}
			else
			{
				DrawLineLow(camera, l);
			}
		}
		else
		{
			if (line.p1.y > line.p2.y)
			{
				DrawLineHigh(camera, -l);
			}
			else
			{
				DrawLineHigh(camera, l);
			}
		}

	}

	void DrawLine(Bagual::Camera::BCamera& camera, const BPixelPos& p1, const BPixelPos& p2)
	{
		DrawLine(camera, BLine<BPixelPos>(p1, p2));
	}

}