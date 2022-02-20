#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <immintrin.h>
#include "Logger.h"
#include "Assertions.h"
#include "Definitions.h"

namespace bgl
{
	template <class T, class aloc = std::allocator<T>>
	class BArray : public std::vector<T, aloc>
	{
		//std::vector<T, aloc> stdContainer;

	public:

		inline void ZeroAll()
		{
			std::fill(begin(), end(), T(0));
		}

		inline void Reserve(size_t size)
		{
			reserve(size);
		}

		inline void Resize(size_t size)
		{
			resize(size);
		}
		
		inline T& operator[](size_t index)
		{
			assert(index < Size());
			return at(index);
		}

		inline const T& operator[](size_t index) const
		{
			assert(index < Size());
			return at(index);
		}

		inline size_t Size() const
		{
			return size();
		}

		inline void Add(const T& item)
		{
			push_back(item);
		}

		inline void Add(const BArray<T>& list)
		{
			insert(end(), list.begin(), list.end());
		}

		inline void Remove(const T& item)
		{
			auto result = std::remove(begin(), end(), item);
		}

		inline void MoveAdd(const BArray<T>& list)
		{
			splice(end(), list);
		}

		inline void Clear()
		{
			clear();
		}

	};

	// Based on the implementation from the following link,
	// and with fixes mentioned in the same thread:
	// https://codereview.stackexchange.com/questions/164675/c-class-for-aligning-objects-on-the-stack
	template<size_t Align, typename T>
	class BStackAligned
	{
	public:
		using value_type = T;
		using aligned_type = BStackAligned<Align, T>;

		BStackAligned()
		{
			ptr_ = aligned_position();
		}

		BStackAligned(const aligned_type& other)
		{
			*ptr_ = *other.ptr_;
		}

		BStackAligned(aligned_type&& other)
		{
			*ptr_ = std::move(*other.ptr_);
		}

		aligned_type& operator=(const aligned_type& other)
		{
			*ptr_ = *other.ptr_;
			return *this;
		}

		template<typename... Args>
		BStackAligned(Args&&... args)
		{
			ptr_ = aligned_position();
			new (ptr_) T(std::forward<Args>(args)...);
		}

		~BStackAligned()
		{
			ptr_->~T();
		}

		T* get() const
		{
			return ptr_;
		}

		T* operator&() const
		{
			return get();
		}

		T& operator*() const
		{
			return ptr_;
		}

	private:
		T* aligned_position() const noexcept
		{
			return reinterpret_cast<T*>(
				(reinterpret_cast<std::uintptr_t>(&storage_) + Align - 1) & ~(Align - 1));
		}

		uint8_t storage_[Align - 1 + sizeof(T)];
		T* ptr_;

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

		inline BSize() : width(0), height(0) {}
		inline BSize(const T& width, const T& height) : width(width), height(height) {}
		inline BSize(const BSize<T>& p) : width(p.width), height(p.height) {}
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

		inline BVector2() : x(T()), y(T()) {}
		inline BVector2(const T& x, const T& y) : x(x), y(y) {}
		inline BVector2(const BVector2<T> &p) : x(p.x), y(p.y) {}
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

		inline T Norm() const
		{
			return x * x + y * y;
		}

		inline BVector2<T>& Normalize()
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

	typedef BVector2<float> BVec2f;

	template <typename T>
	class BVector3
	{

	public:

		T x, y, z;

		inline BVector3()
		{
			this->x = T();
			this->y = T();
			this->z = T();
		}

		inline BVector3(T x, T y)
		{
			this->x = x;
			this->y = y;
			this->z = T();
		}

		inline BVector3(T x, T y, T z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		inline BVector3(const BVector3<T> &p)
		{
			this->x = p.x;
			this->y = p.y;
			this->z = p.z;
		}

		~BVector3() {}

		inline BVector3<T>& Sum(const BVector3<T>& p)
		{
			x = x + p.x;
			y = y + p.y;
			z = z + p.z;
			return *this;
		}

		inline BVector3<T>& Subtract(const BVector3<T>& p)
		{
			x = x - p.x;
			y = y - p.y;
			z = z - p.z;
			return *this;
		}

		inline BVector3<T>& Divide(const BVector3<T>& p)
		{
			x = x / p.x;
			y = y / p.y;
			z = z / p.z;
			return *this;
		}

		inline BVector3<T>& Multiply(const BVector3<T>& p)
		{
			x = x * p.x;
			y = y * p.y;
			z = z * p.z;
			return *this;
		}

		inline T DotProduct(const BVector3<T>& p)
		{
			return x * p.x + y * p.y + z * p.z;
		}

		inline BVector3<T>& CrossProduct(const BVector3<T>& p)
		{
			T xTemp = y * p.z - z * p.y;
			T yTemp = z * p.x - x * p.z;
			z = x * p.y - y * p.x;
			x = xTemp;
			y = yTemp;
			return *this;
		}

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

		inline T Norm() const
		{
			return this->x * this->x + this->y * this->y + this->z * this->z;
		}

		inline T Length() const
		{
			return sqrt(Norm());
		}

		inline BVector3<T> Normalize()
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

		inline BTransform(BVec3f&& inTranslation, BVec3f&& inRotation, BVec3f&& inScale)
			: translation(inTranslation), rotation(inRotation), scale(inScale)
		{

		}

		inline BTransform(BVec3f& inTranslation, BVec3f& inRotation, BVec3f& inScale)
			: translation(inTranslation), rotation(inRotation), scale(inScale)
		{

		}

		inline BTransform(BVec3f&& inTranslation, BVec3f&& inRotation)
			: translation(inTranslation), rotation(inRotation)
		{

		}

		inline BTransform(BVec3f& inTranslation, BVec3f& inRotation)
			: translation(inTranslation), rotation(inRotation)
		{

		}

		inline BTransform(BVec3f&& inTranslation)
			: translation(inTranslation)
		{

		}

		inline BTransform(BVec3f& inTranslation)
			: translation(inTranslation)
		{

		}

	};

	template <typename T>
	class BTriangle
	{

	public:

		BVector3<T> v0, v1, v2;

		BTriangle() = default;
		inline BTriangle(const BVector3<T>&& v0, const BVector3<T>&& v1, const BVector3<T>&& v2)
		{
			this->v0 = v0;
			this->v1 = v1;
			this->v2 = v2;
		}
		inline BTriangle(const BVector3<T>& v0, const BVector3<T>& v1, const BVector3<T>& v2)
		{
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

		inline BLine() : p1(T()), p2(T()) {}
		inline BLine(const T &p1, const T &p2) : p1(p1), p2(p2) {}
		inline BLine(const BLine<T> &l) : p1(l.p1), p2(l.p2) {}
		~BLine() {}

		inline const BLine operator-() const
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

		inline BBoxEdges(const EdgeType& topEdge, const EdgeType& bottomEdge, const EdgeType& LeftEdge, const EdgeType& rightEdge)
		{
			edges[(uint32)BEBoxEdge::Top] = topEdge;
			edges[(uint32)BEBoxEdge::Bottom] = bottomEdge;
			edges[(uint32)BEBoxEdge::Left] = LeftEdge;
			edges[(uint32)BEBoxEdge::Right] = rightEdge;
		}

		inline BBoxEdges(const BBoxEdges& otherBox)
		{
			memcpy(edges, otherBox.GetEdges(), sizeof(EdgeType) * 4);
		}

		inline const EdgeType* GetEdges() const
		{
			return edges;
		}

		inline EdgeType& operator[](const BEBoxEdge&& index)
		{
			return edges[(uint32)index];
		}

		inline EdgeType& operator[](uint32 index)
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

		inline BBox(const T &p1, const T &p2)
		{
			this->p1 = p1;
			this->p2 = p2;
		}

		inline BBox(const BBox<T> &b)
		{
			this->p1 = b.p1;
			this->p2 = b.p2;
		}

		~BBox() {}

		template <typename Y>
		inline bool IsIn(const BVector2<Y> &v) const
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

		inline BBuffer(size_t length = 0)
		{
			BGL_ASSERT(length > 0 && "Can't start buffer with length <= 0!");

			_memBlock = new T[length];
			_length = length;
			_allocLength = length;
		}

		inline BBuffer(size_t length, T defaultValue)
		{
			BGL_ASSERT(length > 0 && "Can't start buffer with length <= 0!");

			_memBlock = new T[length];
			_length = length;
			_allocLength = length;

			SetBufferValue(defaultValue);
		}

		inline BBuffer(T* data, size_t length)
		{
			BGL_ASSERT(length > 0 && "Can't start buffer with length <= 0!");

			_memBlock = data;
			_length = length;
			_allocLength = length;
		}

		inline BBuffer(T* data, size_t length, T defaultValue)
		{
			BBuffer(data, length);
			SetBufferValue(defaultValue);
		}

		inline T& operator[](size_t index)
		{
			BGL_ASSERT(index < Length() && "Out of range!");
			return _memBlock[index];
		}

		inline const T& operator[](size_t index) const
		{
			BGL_ASSERT(index < Length() && "Out of range!");
			return _memBlock[index];
		}

		inline size_t Length()
		{
			return _length;
		}

		inline T* GetData()
		{
			return _memBlock;
		}

		inline void Allocate(size_t allocLength, bool bMove = false)
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

		inline void SetLength(size_t newLength, bool bMove = false)
		{
			BGL_ASSERT(newLength > 0 && "Got negative newLength!");

			if (newLength > _allocLength)
			{
				Allocate(newLength, bMove);
			}

			_length = newLength;
		}

		inline void SetBufferValue(T value)
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

		inline BQuaternion<T>() : s(0), v(VecType()) { }

		inline BQuaternion<T>(const T scalar, const BVector3<T>& vec)
		{
			this->s = scalar;
			this->v = vec;
		}

		~BQuaternion<T>() { }

		inline BQuaternion<T>(const BQuaternion& value)
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

#pragma region AVX Specialization

	inline const __m256 operator-(const __m256& p1, const __m256 p2)
	{
		return _mm256_sub_ps(p1, p2);
	}

	inline const __m256 operator+(const __m256& p1, const __m256 p2)
	{
		return _mm256_add_ps(p1, p2);
	}

	inline const __m256 operator*(const __m256& p1, const __m256 p2)
	{
		return _mm256_mul_ps(p1, p2);
	}

	inline const __m256 operator/(const __m256& p1, const __m256 p2)
	{
		return _mm256_div_ps(p1, p2);
	}

#pragma endregion

}