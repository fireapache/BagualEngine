#pragma once

#include <cmath>

using namespace std;

//class Pixel
//{
//
//public:
//
//	int x;
//	int y;
//	int colour;
//
//	Pixel()							: x(0), y(0), colour(0) {}
//	Pixel(int x, int y)				: x(x), y(y) {}
//	Pixel(int x, int y, int colour) : x(x), y(y), colour(colour) {}
//	Pixel(const Pixel &p)			: x(p.x), y(p.y), colour(p.colour) {}
//	~Pixel() {}
//
//	inline const Pixel& operator-(const Pixel &p) const
//	{
//		return Pixel(x - p.x, y - p.y);
//	}
//
//	inline const Pixel& operator+(const Pixel &p) const
//	{
//		return Pixel(x + p.x, y + p.y);
//	}
//
//	inline float operator|(const Pixel &p) const
//	{
//		return sqrt(pow(float(x - p.x), 2.0f) + pow(float(y - p.y), 2.0f));
//	}
//
//};

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

	Line(const T &p1, const T &p2)	: p1(p1), p2(p2) {}
	Line(const Line<T> &l)			: p1(l.p1), p2(l.p2) {}
	~Line() {}

};

typedef Vector2<int> Pixel;
