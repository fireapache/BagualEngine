#pragma once

#include "Core.h"

#define vxs(x0,y0, x1,y1)    ((x0)*(y1) - (x1)*(y0))   // vxs: Vector cross product
// Intersect: Calculate the point of intersection between two lines.
#define Intersect(x1,y1, x2,y2, x3,y3, x4,y4) ((struct xy) { \
    vxs(vxs(x1,y1, x2,y2), (x1)-(x2), vxs(x3,y3, x4,y4), (x3)-(x4)) / vxs((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)), \
    vxs(vxs(x1,y1, x2,y2), (y1)-(y2), vxs(x3,y3, x4,y4), (y3)-(y4)) / vxs((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)) })


template <typename T>
T CrossProduct(const Vector2<T> &p1, const Vector2<T> &p2)
{
	return (p1.x * p2.y) - (p2.x * p1.y);
}

template <typename T>
Vector2<T> LinesIntersection(const Line<Vector2<T>> &l1, const Line<Vector2<T>> &l2)
{
	Vector2<T> auxP1((l1.p1.x) - (l1.p2.x), (l1.p1.y) - (l1.p2.y));
	Vector2<T> auxP2((l2.p1.x) - (l2.p2.x), (l2.p1.y) - (l2.p2.y));

	T div = CrossProduct(auxP1, auxP2);

	if (div == T(0)) return Vector2<T>(T(0), T(0)); // TODO: Add exception...

	T auxF1 = CrossProduct(Vector2<T>(l1.p1.x, l1.p1.y), Vector2<T>(l1.p2.x, l1.p2.y));
	T auxF2 = CrossProduct(Vector2<T>(l2.p1.x, l2.p1.y), Vector2<T>(l2.p2.x, l2.p2.y));

	Vector2<T> auxP3(auxF1, (l1.p1.x - l1.p2.x));
	Vector2<T> auxP4(auxF2, (l2.p1.x - l2.p2.x));
	Vector2<T> auxP5(auxF1, (l1.p1.y - l1.p2.y));
	Vector2<T> auxP6(auxF2, (l2.p1.y - l2.p2.y));

	T auxF3 = CrossProduct(auxP3, auxP4);
	T auxF4 = CrossProduct(auxP5, auxP6);

	return Vector2<T>((auxF3 / div), (auxF4 / div));
}

template <typename T>
T Lerp(T x, T y, float a)
{
	return static_cast<int>(x * (1 - a) + y * a);
}