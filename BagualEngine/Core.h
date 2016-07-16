#pragma once

#include <cmath>

using namespace std;

template <typename T>
class Vector2
{
	
public:

	T x, y;

	Vector2()						: x(0), y(0) {}
	Vector2(T x, T y)				: x(x), y(y) {}
	Vector2(const Vector2<T> &p)	: x(p.x), y(p.y) {}
	~Vector2() {}

	inline const Vector2<T>& operator-(const Vector2<T> &p) const
	{
		return Vector2<T>(x - p.x, y - p.y);
	}

	inline const Vector2<T>& operator+(const Vector2<T> &p) const
	{
		return Vector2<T>(x + p.x, y + p.y);
	}

	inline float operator|(const Vector2<T> &p) const
	{
		float a = static_cast<float>(x - p.x);
		float b = static_cast<float>(y - p.y);

		return sqrt(pow(a, 2.0f) + pow(b, 2.0f));
	}

};

template <typename T>
class Vector3 : public Vector2<T>
{

public:

	T z;

	Vector3()				: x(0), y(0), z(0) {}
	Vector3(T x, T y)		: x(x), y(y), z(0) {}
	Vector3(T x, T y, T z)	: x(x), y(y), z(z) {}
	Vector3(Vector3<T> &p)	: x(p.x), y(p.y), z(p.z) {}
	~Vector3() {}

	inline const Vector3<T>& operator-(const Vector3<T> &p) const
	{
		return Vector3<T>(x - p.x, y - p.y, z - p.z);
	}

	inline const Vector3<T>& operator+(const Vector3<T> &p) const
	{
		return Vector3<T>(x + p.x, y + p.y, z + p.z);
	}

	inline float operator|(const Vector3<T> &p) const
	{
		float a = static_cast<float>(x - p.x);
		float b = static_cast<float>(y - p.y);
		float c = static_cast<float>(z - p.z);

		return sqrt(pow(a, 2.0f) + pow(b, 2.0f) + pow(c, 2.0f));
	}

};

template <typename T>
class Line
{
	
public:

	T p1;
	T p2;

	Line()							: p1(T()), p2(T()) {}
	Line(const T &p1, const T &p2)	: p1(p1), p2(p2) {}
	Line(const Line<T> &l)			: p1(l.p1), p2(l.p2) {}
	~Line() {}

};

typedef Vector2<int> Pixel;

template <typename T>
class Box : public Line<T>
{

public:

	Box()							: p1(T()), p2(T()) {}

	Box(const T &p1, const T &p2)
	{
		this->p1 = p1;
		this->p2 = p2;
	}

	Box(const Box<T> &b)			: p1(b.p1), p2(b.p2) {}
	~Box() {}

	template <typename Y>
	bool IsIn(const Vector2<Y> &v) const
	{
		bool result = true;

		if (v.x < p1.x || v.x > p2.x) result = false;
		else if (v.y < p1.y || v.y > p2.y) result = false;

		return result;
	}

};
