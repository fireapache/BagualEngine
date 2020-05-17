
#include "BDraw.h"

using namespace Bagual::Math;

namespace Bagual::Draw
{
	
	bool IsLineOnScreen(Bagual::Camera::BCameraSettings& cameraSettings, BLine<BPixelPos>& line)
	{
		BBox<BPixelPos> viewport(
			BPixelPos(0, 0),
			BPixelPos(cameraSettings.GetWidth() - 1, cameraSettings.GetHeight() - 1));

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
			if (LinesIntersection(line, cameraSettings.GetScreenEdge(i), inters[i]))
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

	void DrawLineLow(Bagual::Camera::BCameraSettings& camera, const BLine<BPixelPos>& line)
	{
		int* screen = camera.GetScreen();
		const int width = camera.GetWidth();
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

	void DrawLineHigh(Bagual::Camera::BCameraSettings& camera, const BLine<BPixelPos>& line)
	{
		int* screen = camera.GetScreen();
		const int width = camera.GetWidth();
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

	void DrawLine(Bagual::Camera::BCameraSettings& cameraSettings, const BLine<BPixelPos>& line)
	{
		// Implementing Bresenham's algorithm (https://en.wikipedia.org/wiki/Bresenham)

		// Getting a copy so we can change its values
		BLine<BPixelPos> l(line);

		//std::cout << "P1: (" << line.p1.x << "," << line.p1.y << ") P2: (" << line.p2.x << "," << line.p2.y << ")" << std::endl;

		// Checks if line is on screen and clamps it
		if (IsLineOnScreen(cameraSettings, l) == false) return;

		//std::cout << "P1: (" << l.p1.x << "," << l.p1.y << ") P2: (" << l.p2.x << "," << l.p2.y << ")" << std::endl;

		if (std::abs(line.p2.y - line.p1.y) < std::abs(line.p2.x - line.p1.x))
		{
			if (line.p1.x > line.p2.x)
			{
				DrawLineLow(cameraSettings, -l);
			}
			else
			{
				DrawLineLow(cameraSettings, l);
			}
		}
		else
		{
			if (line.p1.y > line.p2.y)
			{
				DrawLineHigh(cameraSettings, -l);
			}
			else
			{
				DrawLineHigh(cameraSettings, l);
			}
		}

	}

	void DrawLine(Bagual::Camera::BCameraSettings& cameraSettings, const BPixelPos& p1, const BPixelPos& p2)
	{
		DrawLine(cameraSettings, BLine<BPixelPos>(p1, p2));
	}

}