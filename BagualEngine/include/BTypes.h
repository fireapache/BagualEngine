#pragma once

#include <cmath>

//using namespace std;

namespace Bagual
{
	namespace Types
	{
		template <typename T>
		class BVector2
		{

		public:

			T x, y;

			BVector2() : x(0), y(0) {}
			BVector2(T x, T y) : x(x), y(y) {}
			BVector2(const BVector2<T> &p) : x(p.x), y(p.y) {}
			~BVector2() {}

			inline const BVector2<T>& operator-(const BVector2<T> &p) const
			{
				return BVector2<T>(x - p.x, y - p.y);
			}

			inline const BVector2<T>& operator+(const BVector2<T> &p) const
			{
				return BVector2<T>(x + p.x, y + p.y);
			}

			inline float operator|(const BVector2<T> &p) const
			{
				float a = static_cast<float>(x - p.x);
				float b = static_cast<float>(y - p.y);

				return sqrt(pow(a, 2.0f) + pow(b, 2.0f));
			}

		};

		template <typename T>
		class BVector3 : public BVector2<T>
		{

		public:

			T z;

			BVector3() : x(0), y(0), z(0) {}
			BVector3(T x, T y) : x(x), y(y), z(0) {}
			BVector3(T x, T y, T z) : x(x), y(y), z(z) {}
			BVector3(BVector3<T> &p) : x(p.x), y(p.y), z(p.z) {}
			~BVector3() {}

			inline const BVector3<T>& operator-(const BVector3<T> &p) const
			{
				return BVector3<T>(x - p.x, y - p.y, z - p.z);
			}

			inline const BVector3<T>& operator+(const BVector3<T> &p) const
			{
				return BVector3<T>(x + p.x, y + p.y, z + p.z);
			}

			inline float operator|(const BVector3<T> &p) const
			{
				float a = static_cast<float>(x - p.x);
				float b = static_cast<float>(y - p.y);
				float c = static_cast<float>(z - p.z);

				return sqrt(pow(a, 2.0f) + pow(b, 2.0f) + pow(c, 2.0f));
			}

		};

		template <typename T>
		class BLine
		{

		public:

			T p1;
			T p2;

			BLine() : p1(T()), p2(T()) {}
			BLine(const T &p1, const T &p2) : p1(p1), p2(p2) {}
			BLine(const BLine<T> &l) : p1(l.p1), p2(l.p2) {}
			~BLine() {}

			const BLine operator-() const
			{
				return BLine(p2, p1);
			}

		};

		typedef BVector2<int> BPixel;

		template <typename T>
		class BBox : public BLine<T>
		{

		public:

			BBox() : p1(T()), p2(T()) {}

			BBox(const T &p1, const T &p2)
			{
				this->p1 = p1;
				this->p2 = p2;
			}

			BBox(const BBox<T> &b) : p1(b.p1), p2(b.p2) {}
			~BBox() {}

			template <typename Y>
			bool IsIn(const BVector2<Y> &v) const
			{
				bool result = true;

				if (v.x < p1.x || v.x > p2.x) result = false;
				else if (v.y < p1.y || v.y > p2.y) result = false;

				return result;
			}

		};
	}
}