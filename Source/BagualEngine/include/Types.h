#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include "Logger.h"
#include "Assertions.h"
#include "Definitions.h"

namespace bgl
{
	template <typename T>
	class BArray : public std::vector<T>
	{
		std::vector<T> stdContainer;

	public:

		void ZeroAll()
		{
			std::fill(begin(), end(), T(0));
		}

		void Reserve(size_t size)
		{
			reserve(size);
		}

		void Resize(size_t size)
		{
			resize(size);
		}
		
		T& operator[](size_t index)
		{
			assert(index < Size());
			return at(index);
		}

		const T& operator[](size_t index) const
		{
			assert(index < Size());
			return at(index);
		}

		size_t Size() const
		{
			return size();
		}

		void Add(const T& item)
		{
			push_back(item);
		}

		void Add(const BArray<T>& list)
		{
			insert(end(), list.begin(), list.end());
		}

		void Remove(const T& item)
		{
			auto result = std::remove(begin(), end(), item);
		}

		void MoveAdd(const BArray<T>& list)
		{
			splice(end(), list);
		}

		void Clear()
		{
			clear();
		}

	};

	enum class BEBoxEdge : uint32
	{
		Top = 0,
		Bottom,
		Left,
		Right
	};

	class BString : std::string
	{

	};

	template <typename T>
	class BSize
	{

	public:

		T width, height;

		BSize() : width(0), height(0) {}
		BSize(const T& width, const T& height) : width(width), height(height) {}
		BSize(const BSize<T>& p) : width(p.width), height(p.height) {}
		~BSize() {}

	};

	/*template <typename T>
	class BSize3D : public BSize<T>
	{

	public:

		T depth;

		BSize3D() : width(0), height(0), depth(0) {}
		BSize3D(const T& width, const T& height, const T& depth) : width(width), height(height), depth(depth) {}
		BSize3D(const BSize3D<T>& p) : width(p.width), height(p.height), depth(p.depth) {}
		~BSize3D() {}

	};*/

	template <typename T>
	class BVector2
	{

	public:

		T x, y;

		BVector2() : x(0), y(0) {}
		BVector2(const T& x, const T& y) : x(x), y(y) {}
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

		template <typename T>
		T Norm() const
		{
			return x * x + y * y;
		}

		template <typename T>
		BVector2<T>& Normalize()
		{
			T n = Norm<T>();

			if (n > 0)
			{
				T factor = 1 / sqrt<T>(n);
				x *= factor, y *= factor;
			}

			return *this;
		}
	};

	template <typename T>
	class BVector3 : public BVector2<T>
	{

	public:

		T z;

		BVector3()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

		BVector3(T x, T y)
		{
			this->x = x;
			this->y = y;
			this->z = 0;
		}

		BVector3(T x, T y, T z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		BVector3(const BVector3<T> &p)
		{
			this->x = p.x;
			this->y = p.y;
			this->z = p.z;
		}

		~BVector3() {}

		inline const BVector3<T> operator-(const BVector3<T> &p) const
		{
			return BVector3<T>(this->x - p.x, this->y - p.y, this->z - p.z);
		}

		inline const BVector3<T> operator+(const BVector3<T> &p) const
		{
			return BVector3<T>(this->x + p.x, this->y + p.y, this->z + p.z);
		}

		inline const BVector3<T> operator*(const BVector3<T>& p) const
		{
			return BVector3<T>(this->x * p.x, this->y * p.y, this->z * p.z);
		}

		inline const BVector3<T> operator*(const T& p) const
		{
			return BVector3<T>(this->x * p, this->y * p, this->z * p);
		}

		inline const BVector3<T> operator/(const BVector3<T>& p) const
		{
			return BVector3<T>(this->x / p.x, this->y / p.y, this->z / p.z);
		}

		inline const BVector3<T> operator/(const T& p) const
		{
			return BVector3<T>(this->x / p, this->y / p, this->z / p);
		}

		inline float operator|(const BVector3<T> &p) const
		{
			float a = static_cast<float>(this->x - p.x);
			float b = static_cast<float>(this->y - p.y);
			float c = static_cast<float>(this->z - p.z);

			return sqrt(a*a + b*b + c*c);
		}

		inline T Dot (const BVector3<T>& p) const
		{
			return (this->x * p.x + this->y * p.y + this->z * p.z);
		}

		inline BVector3<T> Cross(const BVector3<T>& p) const
		{
			BVector3<T> Result;

			Result.x = this->y * p.z - this->z * p.y;
			Result.y = this->z * p.x - this->x * p.z;
			Result.z = this->x * p.y - this->y * p.x;

			return Result;
		}

		inline void operator+=(const BVector3<T>& p)
		{
			this->x += p.x;
			this->y += p.y;
			this->z += p.z;
		}

		inline void operator-=(const BVector3<T>& p)
		{
			this->x -= p.x;
			this->y -= p.y;
			this->z -= p.z;
		}

		inline void operator*=(const T& p)
		{
			*this = *this * p;
		}

		inline void operator*=(const BVector3<T>& p)
		{
			*this = *this * p;
		}

		inline void operator/=(const T& p)
		{
			*this = *this / p;
		}

		inline void operator/=(const BVector3<T>& p)
		{
			*this = *this / p;
		}

		T Norm() const
		{
			return this->x * this->x + this->y * this->y + this->z * this->z;
		}

		T Length() const
		{
			return sqrt(Norm());
		}

		BVector3<T> Normalize()
		{
			T n = Norm();

			if (n > 0)
			{
				T div = sqrt(n);
				this->x /= div, this->y /= div, this->z /= div;
			}

			return *this;
		}

	};

	typedef BVector3<float> BVec3f;

	template <typename T>
	class BTransform
	{


	public:

		BVec3f translation;
		BVec3f rotation;
		BVec3f scale;

		BTransform() = default;

		BTransform(BVec3f&& inTranslation, BVec3f&& inRotation, BVec3f&& inScale)
			: translation(inTranslation), rotation(inRotation), scale(inScale)
		{

		}

		BTransform(BVec3f& inTranslation, BVec3f& inRotation, BVec3f& inScale)
			: translation(inTranslation), rotation(inRotation), scale(inScale)
		{

		}

		BTransform(BVec3f&& inTranslation, BVec3f&& inRotation)
			: translation(inTranslation), rotation(inRotation)
		{

		}

		BTransform(BVec3f& inTranslation, BVec3f& inRotation)
			: translation(inTranslation), rotation(inRotation)
		{

		}

		BTransform(BVec3f&& inTranslation)
			: translation(inTranslation)
		{

		}

		BTransform(BVec3f& inTranslation)
			: translation(inTranslation)
		{

		}

	};

	template <typename T>
	class BTriangle
	{

	public:

#ifdef TRIANGLE_SOURCE
		uint32 meshID;
#endif

		BVector3<T> v0, v1, v2;

		BTriangle() {};

#ifdef TRIANGLE_SOURCE
		BTriangle(const BVector3<T>& v0, const BVector3<T>& v1, const BVector3<T>& v2, uint32 sourceMeshID = 0)
		{
			meshID = sourceMeshID;
#else
		BTriangle(const BVector3<T>& v0, const BVector3<T>& v1, const BVector3<T>& v2)
		{
#endif
			this->v0 = v0;
			this->v1 = v1;
			this->v2 = v2;
		}

		inline BVector3<T> GetPointOnSurface(const float u, const float v)
		{
			const BVector3<T> v0v1 = v1 - v0;
			const BVector3<T> v0v2 = v2 - v0;
			return v0 + v0v1 * u + v0v2 * v;
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

	typedef BVector2<int32> BPixelPos;
	
	class BBoxEdges
	{
		typedef BLine<BPixelPos> EdgeType;

		EdgeType edges[4];

	public:

		BBoxEdges()
		{
			//BGL_LOG("Creating an empty BBoxEdges object!");
		}

		BBoxEdges(const EdgeType& topEdge, const EdgeType& bottomEdge, const EdgeType& LeftEdge, const EdgeType& rightEdge)
		{
			edges[(uint32)BEBoxEdge::Top] = topEdge;
			edges[(uint32)BEBoxEdge::Bottom] = bottomEdge;
			edges[(uint32)BEBoxEdge::Left] = LeftEdge;
			edges[(uint32)BEBoxEdge::Right] = rightEdge;
		}

		BBoxEdges(const BBoxEdges& otherBox)
		{
			memcpy(edges, otherBox.GetEdges(), sizeof(EdgeType) * 4);
		}

		const EdgeType* GetEdges() const
		{
			return edges;
		}

		EdgeType& operator[](const BEBoxEdge&& index)
		{
			return edges[(uint32)index];
		}

		EdgeType& operator[](uint32 index)
		{
			return edges[index];
		}


	};

	template <typename T>
	class BBox : public BLine<T>
	{

	public:

		BBox()
		{
			this->p1 = T();
			this->p2 = T();
		}

		BBox(const T &p1, const T &p2)
		{
			this->p1 = p1;
			this->p2 = p2;
		}

		BBox(const BBox<T> &b)
		{
			this->p1 = b.p1;
			this->p2 = b.p2;
		}

		~BBox() {}

		template <typename Y>
		bool IsIn(const BVector2<Y> &v) const
		{
			bool result = true;

			if (v.x < this->p1.x || v.x > this->p2.x) result = false;
			else if (v.y < this->p1.y || v.y > this->p2.y) result = false;

			return result;
		}

	};

	template<typename T>
	class BBuffer
	{

		T* _memBlock;

		size_t _length;
		size_t _allocLength;

	public:

		BBuffer(size_t length = 0)
		{
			BGL_ASSERT(length > 0 && "Can't start buffer with length <= 0!");

			_memBlock = new T[length];
			_length = length;
			_allocLength = length;
		}

		BBuffer(size_t length, T defaultValue)
		{
			BGL_ASSERT(length > 0 && "Can't start buffer with length <= 0!");

			_memBlock = new T[length];
			_length = length;
			_allocLength = length;

			SetBufferValue(defaultValue);
		}

		BBuffer(T* data, size_t length)
		{
			BGL_ASSERT(length > 0 && "Can't start buffer with length <= 0!");

			_memBlock = data;
			_length = length;
			_allocLength = length;
		}

		BBuffer(T* data, size_t length, T defaultValue)
		{
			BBuffer(data, length);
			SetBufferValue(defaultValue);
		}

		T& operator[](size_t index)
		{
			BGL_ASSERT(index < Length() && "Out of range!");
			return _memBlock[index];
		}

		const T& operator[](size_t index) const
		{
			BGL_ASSERT(index < Length() && "Out of range!");
			return _memBlock[index];
		}

		size_t Length()
		{
			return _length;
		}

		T* GetData()
		{
			return _memBlock;
		}

		void Allocate(size_t allocLength, bool bMove = false)
		{
			BGL_ASSERT(allocLength > 0 && "Got negative allocLength!");

			if (allocLength > _allocLength)
			{
				BGL_LOG("Reallocating Buffer!");

				T* newMemBlock = new T[allocLength];

				if (bMove)
				{
					memcpy(newMemBlock, _memBlock, _allocLength);
				}

				_allocLength = allocLength;
				delete[] _memBlock;

				_memBlock = newMemBlock;
			}
		}

		void SetLength(size_t newLength, bool bMove = false)
		{
			BGL_ASSERT(newLength > 0 && "Got negative newLength!");

			if (newLength > _allocLength)
			{
				Allocate(newLength, bMove);
			}

			_length = newLength;
		}

		void SetBufferValue(T value)
		{
			std::fill_n(_memBlock, _length, value);
		}

	};

	template <typename T>
	class BQuaternion
	{
		typedef BVector3<T> VecType;

	public:

		T s;
		VecType v;

		BQuaternion<T>() : s(0), v(VecType()) { }

		BQuaternion<T>(const T scalar, const BVector3<T>& vec)
		{
			this->s = scalar;
			this->v = vec;
		}

		~BQuaternion<T>() { }

		BQuaternion<T>(const BQuaternion& value)
		{
			this->s = value.s;
			this->v = value.v;
		}

		inline BQuaternion<T>& operator=(const BQuaternion& value)
		{
			this->s = value.s;
			this->v = value.v;
			return *this;
		}

		inline void operator+=(const BQuaternion& q)
		{
			s += q.s;
			v += q.v;
		}

		inline BQuaternion<T> operator+(const BQuaternion& q) const
		{
			const T scalar = s + q.s;
			const VecType imaginary = v + q.v;
			return BQuaternion<T>(scalar, imaginary);
		}

		inline void operator-=(const BQuaternion& q)
		{
			s -= q.s;
			v -= q.v;
		}

		inline BQuaternion<T> operator-(const BQuaternion<T>& q) const
		{
			const T scalar = s - q.s;
			const VecType imaginary = v - q.v;
			return BQuaternion<T>(scalar, imaginary);
		}

		inline void operator*=(const BQuaternion<T>& q)
		{
			(*this) = multiply(q);
		}

		inline BQuaternion<T> operator*(const BQuaternion<T>& q) const
		{
			return multiply(q);
		}

		inline BQuaternion<T> multiply(const BQuaternion<T>& q) const
		{
			const T scalar = s * q.s - v.Dot(q.v);
			const VecType imaginary = q.v * s + v * q.s + v.Cross(q.v);
			return BQuaternion<T>(scalar, imaginary);
		}

		inline void operator*=(const T value)
		{
			s *= value;
			v *= value;
		}

		inline BQuaternion<T> operator*(const T value) const
		{
			const T scalar = s * value;
			const VecType imaginary = v * value;
			return BQuaternion<T>(scalar, imaginary);
		}

		inline T norm()
		{
			const T scalar = s * s;
			const VecType imaginary = v * v;
			return std::sqrt(scalar + imaginary.x + imaginary.y + imaginary.z);
		}

		inline void normalize()
		{
			if (norm() != 0)
			{
				const T normValue = 1.f / norm();
				s *= normValue;
				v *= normValue;
			}
		}

		inline void convertToUnitNormQuaternion()
		{
			const T angle = s * fPi / 180.f;
			v.Normalize();
			s = cosf(angle * 0.5f);
			v *= sinf(angle * 0.5f);
		}

		inline BQuaternion<T> conjugate()
		{
			const T scalar = s;
			const VecType imaginary = v * (-1.f);
			return BQuaternion<T>(scalar, imaginary);
		}

		inline BQuaternion<T> inverse()
		{
			T absoluteValue = norm();
			absoluteValue *= absoluteValue;
			absoluteValue = 1 / absoluteValue;
			BQuaternion<T> conjugateValue = conjugate();
			const T scalar = conjugateValue.s * absoluteValue;
			const VecType imaginary = conjugateValue.v * absoluteValue;
			return BQuaternion<T>(scalar, imaginary);
		}

		static inline VecType RotateAroundAxis(T uAngle, VecType&& uAxis, VecType& uVector)
		{
			return RotateAroundAxis(uAngle, uAxis, uVector);
		}

		static inline VecType RotateAroundAxis(T uAngle, VecType& uAxis, VecType& uVector)
		{
			//convert our vector to a pure quaternion
			BQuaternion<T> p(0, uVector);

			//normalize the axis
			uAxis.Normalize();

			//create the real quaternion
			BQuaternion<T> q(uAngle, uAxis);

			//convert quaternion to unit norm quaternion
			q.convertToUnitNormQuaternion();

			//Get the inverse of the quaternion
			BQuaternion<T> qInverse = q.inverse();

			//rotate the quaternion
			BQuaternion<T> rotatedVector = q * p * qInverse;

			//return the vector part of the quaternion
			return rotatedVector.v;
		}

	};
}