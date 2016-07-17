#pragma once

#include "BagualTypes.h"


template <typename T>
T CrossProduct(const Vector2<T> &p1, const Vector2<T> &p2)
{
	return (p1.x * p2.y) - (p2.x * p1.y);
}

template <typename T>
bool LinesIntersection(const Line<Vector2<T>> &l1, const Line<Vector2<T>> &l2, Vector2<T> &result)
{
	Vector2<T> auxP1((l1.p1.x) - (l1.p2.x), (l1.p1.y) - (l1.p2.y));
	Vector2<T> auxP2((l2.p1.x) - (l2.p2.x), (l2.p1.y) - (l2.p2.y));

	T div = CrossProduct(auxP1, auxP2);

	if (div == T(0)) return false;

	T auxF1 = CrossProduct(Vector2<T>(l1.p1.x, l1.p1.y), Vector2<T>(l1.p2.x, l1.p2.y));
	T auxF2 = CrossProduct(Vector2<T>(l2.p1.x, l2.p1.y), Vector2<T>(l2.p2.x, l2.p2.y));

	Vector2<T> auxP3(auxF1, (l1.p1.x - l1.p2.x));
	Vector2<T> auxP4(auxF2, (l2.p1.x - l2.p2.x));
	Vector2<T> auxP5(auxF1, (l1.p1.y - l1.p2.y));
	Vector2<T> auxP6(auxF2, (l2.p1.y - l2.p2.y));

	T auxF3 = CrossProduct(auxP3, auxP4);
	T auxF4 = CrossProduct(auxP5, auxP6);

	result = Vector2<T>((auxF3 / div), (auxF4 / div));

	return true;
}

template <typename T>
T Lerp(T x, T y, float a)
{
	return static_cast<T>(x * (1 - a) + y * a);
}
