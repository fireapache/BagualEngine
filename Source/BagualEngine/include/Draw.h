#pragma once

#include "Common.h"

#include "Camera.h"
#include "Arithmetics.h"
#include "Camera.h"
#include "Canvas.h"
#include "Viewport.h"

namespace bgl
{
	class BDraw
	{

	public:

		static bool RayPlaneIntersection(const BVec3f& planeLoc, BVec3f planeNormal, const BVec3f& rayOrig, const BVec3f& rayDir, BVec3f& result)
		{
			//planeNormal = planeNormal * -1.f;
			const float denom = DotProduct(planeNormal, rayDir);

			if (denom > 1e-6f)
			{
				BVec3f p0l0 = planeLoc - rayOrig;
				const float t = DotProduct(p0l0, planeNormal) / denom;

				if (t >= 0.f)
				{
					result = rayOrig + rayDir * t;
					return true;
				}
			}

			return false;
		}

		// not optimal solution but it works, need optimization
		inline static bool DeProjectPoint(BViewport* viewport, const BVec3f& point, BPixelPos& pixelPos, const bool bInBoundsOnly = false)
		{
			if (viewport == nullptr)
			{
				BGL_LOG("Got null viewport when deprojecting point!");
				return false;
			}

			const BCamera* camera = viewport->GetCamera();

			if (camera == nullptr)
			{
				BGL_LOG("Got null camera when deprojecting point!");
				return false;
			}

			// ray plane intersection parameters
			const float sensorDist = camera->GetSensorDistance();
			const BVec3f cameraLoc = camera->GetLocation();
			const BRotf cameraRot = camera->GetRotation();
			const BVec3f planeNormal = BVec3f(0.f, 0.f, -1.f);
			const BVec3f planeLoc = planeNormal * sensorDist;
			BVec3f rayOrig = point - cameraLoc;
			
			rayOrig = BQuaternion<float>::RotateAroundAxis(-cameraRot.p, BVector3<float>(1.f, 0.f, 0.f), rayOrig);
			rayOrig = BQuaternion<float>::RotateAroundAxis(-cameraRot.y, BVector3<float>(0.f, 1.f, 0.f), rayOrig);
			rayOrig = BQuaternion<float>::RotateAroundAxis(-cameraRot.r, BVector3<float>(0.f, 0.f, 1.f), rayOrig);

			BVec3f rayDir = rayOrig * -1.f;
			rayDir.Normalize();
			BVec3f planeInterPoint;

			const bool validDeproj = RayPlaneIntersection(planeLoc, planeNormal, rayOrig, rayDir, planeInterPoint);

			if (validDeproj)
			{
				BVec2f halfSensorArea = camera->GetSensorArea();
				halfSensorArea.x /= 2.f;
				halfSensorArea.y /= 2.f;
				BVec3f invInterPoint = planeInterPoint;
				invInterPoint.x *= -1.f;

				const float xn = invInterPoint.x / halfSensorArea.x;
				const float yn = invInterPoint.y / halfSensorArea.y;
				
				// final screen coordinates
				const int32 halfViewportWidth = static_cast<int32>(viewport->GetSize().width) / 2;
				const int32 halfViewportHeight = static_cast<int32>(viewport->GetSize().height) / 2;
				const int32 centeredX = static_cast<int32>(xn * static_cast<float>(halfViewportWidth));
				const int32 centeredY = static_cast<int32>(yn * static_cast<float>(halfViewportHeight));
				const int32 x = centeredX + halfViewportWidth + viewport->GetPosition().x;
				const int32 y = centeredY + halfViewportHeight + viewport->GetPosition().y;

				// returning final screen coordinates
				pixelPos = BPixelPos(x, y);

				return true;
			}

			return false;
		}

		/*	Checks is a specific line is on screen, correcting the line's
		 *	properties to make sure it only appears in the visible area.
		 *
		 *	It returns false in case the line completely misses the visible
		 *	area of the camera's viewport.
		 */
		inline static bool ClampViewportLine(BViewport* viewport, BLine<BPixelPos>& line)
		{
			if (viewport == nullptr)
			{
				BGL_LOG("Got null viewport when clamping viewport line!");
				return false;
			}

			auto canvas = viewport->GetCanvas();

			if (canvas == nullptr)
			{
				BGL_LOG("Got null canvas when clamping viewport line!");
				return false;
			}

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

		inline static void DrawLineLow(BViewport* viewport, const BLine<BPixelPos>& line)
		{
			if (viewport == nullptr)
			{
				BGL_LOG("Got null viewport when drawing line low!");
				return;
			}

			auto canvas = viewport->GetCanvas();

			if (canvas == nullptr)
			{
				BGL_LOG("Got null canvas when drawing line low!");
				return;
			}

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

		inline static void DrawLineHigh(BViewport* viewport, const BLine<BPixelPos>& line)
		{
			if (viewport == nullptr)
			{
				BGL_LOG("Got null viewport when drawing line high!");
				return;
			}

			auto canvas = viewport->GetCanvas();

			if (canvas == nullptr)
			{
				BGL_LOG("Got null canvas when drawing line high!");
				return;
			}

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

		/*	Draw a line on viewport. */
		inline static void DrawLine(BViewport* viewport, const BLine<BPixelPos> line)
		{
			if (viewport == nullptr)
			{
				BGL_LOG("Got null viewport when drawing viewport line!");
				return;
			}

			// Implementing Bresenham's algorithm (https://en.wikipedia.org/wiki/Bresenham)

			// Getting a copy so we can change its values
			BLine<BPixelPos> l(line);

			//std::cout << "P1: (" << line.p1.x << "," << line.p1.y << ") P2: (" << line.p2.x << "," << line.p2.y << ")" << std::endl;

			// Checks if line is on screen and clamps it
			if (ClampViewportLine(viewport, l) == false) return;

			//std::cout << "P1: (" << l.p1.x << "," << l.p1.y << ") P2: (" << l.p2.x << "," << l.p2.y << ")" << std::endl;

			if (std::abs(line.p2.y - line.p1.y) < std::abs(line.p2.x - line.p1.x))
			{
				if (line.p1.x > line.p2.x)
				{
					DrawLineLow(viewport, -l);
				}
				else
				{
					DrawLineLow(viewport, l);
				}
			}
			else
			{
				if (line.p1.y > line.p2.y)
				{
					DrawLineHigh(viewport, -l);
				}
				else
				{
					DrawLineHigh(viewport, l);
				}
			}

		}

		/*	Draw a line between pixel positions on viewport */
		inline static void DrawLine(BViewport* viewport, const BPixelPos& p1, const BPixelPos& p2)
		{
			DrawLine(viewport, BLine<BPixelPos>(p1, p2));
		}

		// [comment]
		// The main ray-triangle intersection routine.
		// You can test both methods: the geometric 
		// method and the Moller-Trumbore algorithm
		// [/comment]
		inline static bool RayTriangleIntersect(
			const BVec3f& origin, const BVec3f& dir,
			const BTriangle<float>& triangle,
			float& t, float& u, float& v)
		{
			BTriangle<float> tri = triangle;
			BVec3f dir1 = dir, dir2 = dir, orig = origin;
			const BVec3f& v0v1 = tri.v1.Subtract(tri.v0);
			const BVec3f& v0v2 = tri.v2.Subtract(tri.v0);
			const BVec3f& pvec = dir1.CrossProduct(v0v2);
			const float det = DotProduct<float>(v0v1, pvec);

			// if the determinant is negative the triangle is backfacing
			// if the determinant is close to 0, the ray misses the triangle
			if (det < kEpsilon) return false;

			const float invDet = 1.f / det;

			BVec3f& tvec = orig.Subtract(tri.v0);
			u = DotProduct(tvec, pvec) * invDet;
			if (u < 0 || u > 1) return false;

			BVec3f& qvec = tvec.CrossProduct(v0v1);
			v = DotProduct(dir, qvec) * invDet;
			if (v < 0 || u + v > 1) return false;

			t = DotProduct(v0v2, qvec) * invDet;

			return true;
		}
	};
}
