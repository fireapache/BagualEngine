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
	class BArray
	{
		std::vector<T> stdContainer;

	public:

		void ZeroAll()
		{
			std::fill(stdContainer.begin(), stdContainer.end(), T(0));
		}

		void Reserve(size_t size)
		{
			stdContainer.reserve(size);
		}

		void Resize(size_t size)
		{
			stdContainer.resize(size);
		}
		
		T& operator[](size_t index)
		{
			assert(index < Size());
			return stdContainer[index];
		}

		const T& operator[](size_t index) const
		{
			assert(index < Size());
			return stdContainer[index];
		}

		size_t Size()
		{
			return stdContainer.size();
		}

		void Add(const T& item)
		{
			stdContainer.push_back(item);
		}

		void Add(const BArray<T>& list)
		{
			stdContainer.insert(stdContainer.end(), list.begin(), list.end());
		}

		void Remove(const T& item)
		{
			auto result = std::remove(stdContainer.begin(), stdContainer.end(), item);
		}

		void MoveAdd(const BArray<T>& list)
		{
			stdContainer.splice(stdContainer.end(), list);
		}

		void Clear()
		{
			stdContainer.clear();
		}

#pragma region Standard C++ for each loop support

		class iterator
		{
		public:
			typedef iterator self_type;
			typedef T value_type;
			typedef T& reference;
			typedef T* pointer;
			typedef std::forward_iterator_tag iterator_category;
			typedef int difference_type;
			iterator(pointer ptr) : ptr_(ptr) { }
			self_type operator++() { self_type i = *this; ptr_++; return i; }
			self_type operator++(int junk) { ptr_++; return *this; }
			reference operator*() { return *ptr_; }
			pointer operator->() { return ptr_; }
			bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
			bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
		private:
			pointer ptr_;
		};

		class const_iterator
		{
		public:
			typedef const_iterator self_type;
			typedef T value_type;
			typedef T& reference;
			typedef T* pointer;
			typedef int difference_type;
			typedef std::forward_iterator_tag iterator_category;
			const_iterator(pointer ptr) : ptr_(ptr) { }
			self_type operator++() { self_type i = *this; ptr_++; return i; }
			self_type operator++(int junk) { ptr_++; return *this; }
			const reference operator*() { return *ptr_; }
			const pointer operator->() { return ptr_; }
			bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
			bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
		private:
			pointer ptr_;
		};

		iterator begin()
		{
			return iterator(&stdContainer[0]);
		}

		const_iterator begin() const
		{
			return const_iterator(&stdContainer[0]);
		}

		iterator end()
		{
			return iterator(&stdContainer[0] + Size());
		}

		const_iterator end() const
		{
			return const_iterator(&stdContainer[0] + Size());
		}

#pragma endregion

	};

	enum class BEBoxEdge : uint
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

		inline const BVector3<T>& operator+(const BVector3<T> &p) const
		{
			return BVector3<T>(this->x + p.x, this->y + p.y, this->z + p.z);
		}

		inline float operator|(const BVector3<T> &p) const
		{
			float a = static_cast<float>(this->x - p.x);
			float b = static_cast<float>(this->y - p.y);
			float c = static_cast<float>(this->z - p.z);

			return sqrt(pow(a, 2.0f) + pow(b, 2.0f) + pow(c, 2.0f));
		}

		T Norm() const
		{
			return this->x * this->x + this->y * this->y + this->z * this->z;
		}

		BVector3<T> Normalize()
		{
			T n = Norm();

			if (n > 0)
			{
				T factor = 1 / sqrt(n);
				this->x *= factor, this->y *= factor, this->z *= factor;
			}

			return *this;
		}

	};

	typedef BVector3<float> BVec3f;

	template <typename T>
	class BTriangle
	{

	public:

		BVector3<T> v0, v1, v2;

		BTriangle() {};

		BTriangle(const BVector3<T>& v0, const BVector3<T>& v1, const BVector3<T>& v2)
		{
			this->v0 = v0;
			this->v1 = v1;
			this->v2 = v2;
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

	typedef BVector2<int> BPixelPos;
	
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
			edges[(uint)BEBoxEdge::Top] = topEdge;
			edges[(uint)BEBoxEdge::Bottom] = bottomEdge;
			edges[(uint)BEBoxEdge::Left] = LeftEdge;
			edges[(uint)BEBoxEdge::Right] = rightEdge;
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
			return edges[(uint)index];
		}

		EdgeType& operator[](uint index)
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

		T* _data;

		size_t _length;

	public:

		BBuffer(size_t length = 0)
		{
			BGL_ASSERT(length > 0 && "Can't start buffer with length <= 0!");

			_data = new T[length];
		}

		BBuffer(T* data, size_t length)
		{
			BGL_ASSERT(length > 0 && "Can't start buffer with length <= 0!");

			_data = data;
			_length = length;
		}

		T& operator[](size_t index)
		{
			BGL_ASSERT(index < Length() && "Out of range!");
			return _data[index];
		}

		const T& operator[](size_t index) const
		{
			BGL_ASSERT(index < Length() && "Out of range!");
			return _data[index];
		}

		size_t Length()
		{
			return _length;
		}

	};
}