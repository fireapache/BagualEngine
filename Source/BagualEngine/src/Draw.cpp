
#include "Bagual.pch.h"

#include "Draw.h"
#include "Camera.h"
#include "Canvas.h"
#include "Viewport.h"

namespace bgl
{
	
	bool BDraw::IsLineOnScreen(BCamera* camera, BLine<BPixelPos>& line)
	{
		if (camera == nullptr) return false;

		auto viewport = camera->GetViewport();
		auto canvas = viewport->GetCanvas();

		const BBox<BPixelPos>& viewportBounds = viewport->GetBounds();

		bool p1In = viewportBounds.IsIn(line.p1);
		bool p2In = viewportBounds.IsIn(line.p2);

		if (p1In && p2In) return true;

		bool isValid = true;
		bool isIn;
		BPixelPos inters[4];
		int32 p1Dist = INT_MAX;
		int32 p2Dist = INT_MAX;
		int32 dist;
		BPixelPos* newP1 = nullptr;
		BPixelPos* newP2 = nullptr;

		auto viewEdges = viewport->GetLimits();

		for (int32 i = 0; i < 4; i++)
		{
			if (LinesIntersection(line, viewEdges.GetEdges()[i], inters[i]))
			{
				isIn = viewportBounds.IsIn(inters[i]);

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

	void DrawLineLow(BCamera* camera, const BLine<BPixelPos>& line)
	{
		if (camera == nullptr) return;
		
		auto viewport = camera->GetViewport();

		if (viewport == nullptr) return;

		auto canvas = viewport->GetCanvas();

		if (canvas == nullptr) return;

		auto& screen = canvas->GetColorBuffer();
		const int32 width = canvas->GetWidth();
		int32 dx = line.p2.x - line.p1.x;
		int32 dy = line.p2.y - line.p1.y;
		int32 yi = 1;

		if (dy < 0)
		{
			yi = -1;
			dy = -dy;
		}

		int32 D = 2 * dy - dx;
		int32 y = line.p1.y;

		for (int32 x = line.p1.x; x <= line.p2.x; x++)
		{
			screen[x + width * y] = 0x0000FF;

			if (D > 0)
			{
				y = y + yi;
				D = D - 2 * dx;
			}

			D = D + 2 * dy;
		}
	}
	
	void DrawLineHigh(BCamera* camera, const BLine<BPixelPos>& line)
	{
		if (camera == nullptr) return;

		auto viewport = camera->GetViewport();

		if (viewport == nullptr) return;

		auto canvas = viewport->GetCanvas();

		if (canvas == nullptr) return;

		auto& screen = canvas->GetColorBuffer();
		const int32 width = canvas->GetWidth();
		int32 dx = line.p2.x - line.p1.x;
		int32 dy = line.p2.y - line.p1.y;
		int32 xi = 1;

		if (dx < 0)
		{
			xi = -1;
			dx = -dx;
		}

		int32 D = 2 * dx - dy;
		int32 x = line.p1.x;

		for (int32 y = line.p1.y; y <= line.p2.y; y++)
		{
			screen[x + width * y] = 0x0000FF;

			if (D > 0)
			{
				x = x + xi;
				D = D - 2 * dy;
			}

			D = D + 2 * dx;
		}
	}

	void BDraw::DrawLine(BCamera* camera, const BLine<BPixelPos> line)
	{
		if (camera == nullptr) return;

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

	void BDraw::DrawLine(BCamera* camera, const BPixelPos& p1, const BPixelPos& p2)
	{
		DrawLine(camera, BLine<BPixelPos>(p1, p2));
	}

	bool BDraw::RayTriangleIntersect(
		const BVec3f orig, const BVec3f dir, 
		const BVec3f v0, const BVec3f v1, const BVec3f v2, 
		float& t, float& u, float& v)
	{

#define MOLLER_TRUMBORE 1
#define CULLING 0

#if MOLLER_TRUMBORE
		BVec3f v0v1 = v1 - v0;
		BVec3f v0v2 = v2 - v0;
		BVec3f pvec = CrossProduct<float>(dir, v0v2);
		float det = DotProduct<float>(v0v1, pvec);
#if CULLING
		// if the determinant is negative the triangle is backfacing
		// if the determinant is close to 0, the ray misses the triangle
		if (det < kEpsilon) return false;
#else
		// ray and triangle are parallel if det is close to 0
		if (fabs(det) < kEpsilon) return false;
#endif
		float invDet = 1 / det;

		BVec3f tvec = orig - v0;
		u = DotProduct(tvec, pvec) * invDet;
		if (u < 0 || u > 1) return false;

		BVec3f qvec = CrossProduct(tvec, v0v1);
		v = DotProduct(dir, qvec) * invDet;
		if (v < 0 || u + v > 1) return false;

		t = DotProduct(v0v2, qvec) * invDet;

		return true;
#else
		// compute plane's normal
		BVec3f v0v1 = v1 - v0;
		BVec3f v0v2 = v2 - v0;
		// no need to normalize
		BVec3f N = CrossProduct(v0v1, v0v2); // N
		float area2 = N.Length();

		// Step 1: finding P

		// check if ray and plane are parallel ?
		float NdotRayDirection = DotProduct(N, dir);
		if (fabs(NdotRayDirection) < kEpsilon) // almost 0
			return false; // they are parallel so they don't intersect !

		// compute d parameter using equation 2
		float d = DotProduct(N, v0);

		// compute t (equation 3)
		t = (DotProduct(N, orig) + d) / NdotRayDirection;
		// check if the triangle is in behind the ray
		if (t < 0) return false; // the triangle is behind

		// compute the intersection point using equation 1
		BVec3f P = orig + BVec3f(dir.x * t, dir.y * t, dir.z * t);

		// Step 2: inside-outside test
		BVec3f C; // vector perpendicular to triangle's plane

		// edge 0
		BVec3f edge0 = v1 - v0;
		BVec3f vp0 = P - v0;
		C = CrossProduct(edge0, vp0);
		if (DotProduct(N, C) < 0) return false; // P is on the right side

		// edge 1
		BVec3f edge1 = v2 - v1;
		BVec3f vp1 = P - v1;
		C = CrossProduct(edge1, vp1);
		u = C.Length() / area2;
		if ((DotProduct(N, C)) < 0) return false; // P is on the right side

		// edge 2
		BVec3f edge2 = v0 - v2;
		BVec3f vp2 = P - v2;
		C = CrossProduct(edge2, vp2);
		v = C.Length() / area2;
		if ((DotProduct(N, C)) < 0) return false; // P is on the right side;

		return true; // this ray hits the triangle
#endif
	}

}