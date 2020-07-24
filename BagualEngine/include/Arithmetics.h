#pragma once

#include "Common.h"

namespace bgl
{
	template <typename T>
	inline T deg2rad(const T& deg)
	{
		return deg * M_PI / 180;
	}

	template <typename T>
	T CrossProduct(const BVector2<T> &p1, const BVector2<T> &p2)
	{
		return (p1.x * p2.y) - (p2.x * p1.y);
	}

	template <typename T>
	BVector3<T> CrossProduct(const BVector3<T>& p1, const BVector3<T>& p2)
	{
		return BVector3<T>(p1.y * p2.z - p1.z * p2.y, p1.z * p2.x - p1.x * p2.z, p1.x * p2.y - p1.y * p2.x);
	}

	template <typename T>
	T DotProduct(const BVector3<T>& v1, const BVector3<T>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template <typename T>
	bool LinesIntersection(const BLine<BVector2<T>> &l1, const BLine<BVector2<T>> &l2, BVector2<T> &result)
	{
		BVector2<T> auxP1((l1.p1.x) - (l1.p2.x), (l1.p1.y) - (l1.p2.y));
		BVector2<T> auxP2((l2.p1.x) - (l2.p2.x), (l2.p1.y) - (l2.p2.y));

		T div = CrossProduct(auxP1, auxP2);

		if (div == T(0)) return false;

		T auxF1 = CrossProduct(BVector2<T>(l1.p1.x, l1.p1.y), BVector2<T>(l1.p2.x, l1.p2.y));
		T auxF2 = CrossProduct(BVector2<T>(l2.p1.x, l2.p1.y), BVector2<T>(l2.p2.x, l2.p2.y));

		BVector2<T> auxP3(auxF1, (l1.p1.x - l1.p2.x));
		BVector2<T> auxP4(auxF2, (l2.p1.x - l2.p2.x));
		BVector2<T> auxP5(auxF1, (l1.p1.y - l1.p2.y));
		BVector2<T> auxP6(auxF2, (l2.p1.y - l2.p2.y));

		T auxF3 = CrossProduct(auxP3, auxP4);
		T auxF4 = CrossProduct(auxP5, auxP6);

		result = BVector2<T>((auxF3 / div), (auxF4 / div));

		return true;
	}

	template <typename T>
	T Lerp(T x, T y, float a)
	{
		return static_cast<T>(x * (1 - a) + y * a);
	}
}