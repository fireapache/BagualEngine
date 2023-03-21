#pragma once

#include "Assertions.h"
#include "Definitions.h"
#include "Logger.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <immintrin.h>
#include <string>
#include <vector>

namespace bgl
{
	template< class T, class aloc = std::allocator< T > >
	class BArray : public std::vector< T, aloc >
	{
		//std::vector<T, aloc> stdContainer;

		typedef std::vector< T, aloc > m_vec;

	public:
		void ZeroAll()
		{
			std::fill( m_vec::begin(), m_vec::end(), T( 0 ) );
		}
		
		T& operator[]( size_t index )
		{
			//assert( index < Size() );
			return m_vec::at( index );
		}

		const T& operator[]( size_t index ) const
		{
			assert( index < m_vec::size() );
			return m_vec::at( index );
		}

		[[nodiscard]] T* first()
		{
			if( m_vec::size() > 0 )
			{
				return &( m_vec::at( 0 ) );
			}
			else
			{
				return nullptr;
			}
		}
		
		void add( const T& item )
		{
			m_vec::push_back( item );
		}

		void add( const BArray< T >& list )
		{
			m_vec::insert( m_vec::end(), list.begin(), list.end() );
		}
		
		void remove( const T& item )
		{
			auto found = std::find( m_vec::begin(), m_vec::end(), item );
			if( found != m_vec::end() )
			{
				m_vec::erase( found );
			}
		}

		void moveAdd( const BArray< T >& list )
		{
			splice( m_vec::end(), list );
		}
		
	};

	class BColor
	{
	public:
		float value[ 4 ] = { 1.f, 0.f, 1.f, 1.f };

		[[nodiscard]] Color32Bit getRGB() const
		{
			Color32Bit result = static_cast< Color32Bit >( 255.f * value[ 2 ] );
			Color32Bit cur = static_cast< Color32Bit >( 255.f * value[ 1 ] );
			result = ( result << 8 ) | cur;
			cur = static_cast< Color32Bit >( 255.f * value[ 0 ] );
			result = ( result << 8 ) | cur;
			return result;
		}

		[[nodiscard]] Color32Bit getRGBA() const
		{
			return ( getRGB() << 8 ) | static_cast< uint32_t >( 255.f * value[ 3 ] );
		}
	};

	// Based on the implementation from the following link,
	// and with fixes mentioned in the same thread:
	// https://codereview.stackexchange.com/questions/164675/c-class-for-aligning-objects-on-the-stack
	template< size_t Align, typename T >
	class BStackAligned
	{
	public:
		using value_type = T;
		using aligned_type = BStackAligned< Align, T >;

		BStackAligned()
		{
			ptr_ = aligned_position();
		}

		BStackAligned( const aligned_type& other )
		{
			*ptr_ = *other.ptr_;
		}

		BStackAligned( aligned_type&& other )
		{
			*ptr_ = std::move( *other.ptr_ );
		}

		aligned_type& operator=( const aligned_type& other )
		{
			*ptr_ = *other.ptr_;
			return *this;
		}

		template< typename... Args >
		BStackAligned( Args&&... args )
		{
			ptr_ = aligned_position();
			new( ptr_ ) T( std::forward< Args >( args )... );
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
			return reinterpret_cast< T* >( ( reinterpret_cast< std::uintptr_t >( &storage_ ) + Align - 1 ) & ~( Align - 1 ) );
		}

		uint8_t storage_[ Align - 1 + sizeof( T ) ];
		T* ptr_;
	};

	enum class BEBoxEdge : uint32_t
	{
		Top = 0,
		Bottom,
		Left,
		Right
	};

	class BString : std::string
	{
	};

	template< typename T >
	class BSize
	{
	public:
		T width, height;

		BSize()
			: width( 0 )
			, height( 0 )
		{
		}
		BSize( const T& width, const T& height )
			: width( width )
			, height( height )
		{
		}
		BSize( const BSize< T >& p )
			: width( p.width )
			, height( p.height )
		{
		}
		~BSize()
		{
		}
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

	template< typename T >
	class BVector2
	{
	public:
		T x, y;

		BVector2()
			: x( T() )
			, y( T() )
		{
		}
		BVector2( const T& x, const T& y )
			: x( x )
			, y( y )
		{
		}
		BVector2( const BVector2< T >& p )
			: x( p.x )
			, y( p.y )
		{
		}
		~BVector2()
		{
		}

		const BVector2< T >& operator-( const BVector2< T >& p ) const
		{
			return BVector2< T >( x - p.x, y - p.y );
		}

		const BVector2< T >& operator+( const BVector2< T >& p ) const
		{
			return BVector2< T >( x + p.x, y + p.y );
		}

		float operator|( const BVector2< T >& p ) const
		{
			const float a = static_cast< float >( x - p.x );
			const float b = static_cast< float >( y - p.y );

			return sqrt( pow( a, 2.0f ) + pow( b, 2.0f ) );
		}

		T norm() const
		{
			return x * x + y * y;
		}

		BVector2< T >& normalize()
		{
			T n = norm();

			if( n > 0 )
			{
				T factor = 1 / sqrt< T >( n );
				x *= factor, y *= factor;
			}

			return *this;
		}
	};

	typedef BVector2< float > BVec2f;
	typedef BVector2< int32_t > BVec2i;
	typedef BVector2< uint32_t > BVec2u;

	template< typename T >
	class BVector3
	{
	public:
		T x, y, z;

		BVector3()
		{
			this->x = T();
			this->y = T();
			this->z = T();
		}

		BVector3( T x, bool bNormalize = false )
		{
			this->x = x;
			this->y = T();
			this->z = T();
			if( bNormalize )
				normalize();
		}

		BVector3( T x, T y, bool bNormalize = false )
		{
			this->x = x;
			this->y = y;
			this->z = T();
			if( bNormalize )
				normalize();
		}

		BVector3( T x, T y, T z, bool bNormalize = false )
		{
			this->x = x;
			this->y = y;
			this->z = z;
			if( bNormalize )
				normalize();
		}

		BVector3( const BVector3< T >& p, bool bNormalize = false )
		{
			this->x = p.x;
			this->y = p.y;
			this->z = p.z;
			if( bNormalize )
				normalize();
		}

		~BVector3() = default;

		BVector3< T >& Sum( const BVector3< T >& p )
		{
			x = x + p.x;
			y = y + p.y;
			z = z + p.z;
			return *this;
		}

		BVector3< T >& Subtract( const BVector3< T >& p )
		{
			x = x - p.x;
			y = y - p.y;
			z = z - p.z;
			return *this;
		}

		BVector3< T >& Divide( const BVector3< T >& p )
		{
			x = x / p.x;
			y = y / p.y;
			z = z / p.z;
			return *this;
		}

		BVector3< T >& Multiply( const BVector3< T >& p )
		{
			x = x * p.x;
			y = y * p.y;
			z = z * p.z;
			return *this;
		}

		[[nodiscard]] T DotProduct( const BVector3< T >& p )
		{
			return x * p.x + y * p.y + z * p.z;
		}

		BVector3< T >& CrossProduct( const BVector3< T >& p )
		{
			T xTemp = y * p.z - z * p.y;
			T yTemp = z * p.x - x * p.z;
			z = x * p.y - y * p.x;
			x = xTemp;
			y = yTemp;
			return *this;
		}

		BVector3< T > operator-( const BVector3< T >& p ) const
		{
			return BVector3< T >( this->x - p.x, this->y - p.y, this->z - p.z );
		}

		BVector3< T > operator+( const BVector3< T >& p ) const
		{
			return BVector3< T >( this->x + p.x, this->y + p.y, this->z + p.z );
		}

		BVector3< T > operator*( const BVector3< T >& p ) const
		{
			return BVector3< T >( this->x * p.x, this->y * p.y, this->z * p.z );
		}

		BVector3< T > operator*( const T& p ) const
		{
			return BVector3< T >( this->x * p, this->y * p, this->z * p );
		}

		BVector3< T > operator/( const BVector3< T >& p ) const
		{
			return BVector3< T >( this->x / p.x, this->y / p.y, this->z / p.z );
		}

		BVector3< T > operator/( const T& p ) const
		{
			return BVector3< T >( this->x / p, this->y / p, this->z / p );
		}

		float operator|( const BVector3< T >& p ) const
		{
			float a = static_cast< float >( this->x - p.x );
			float b = static_cast< float >( this->y - p.y );
			float c = static_cast< float >( this->z - p.z );

			return sqrt( a * a + b * b + c * c );
		}

		[[nodiscard]] T Dot( const BVector3< T >& p ) const
		{
			return ( this->x * p.x + this->y * p.y + this->z * p.z );
		}

		[[nodiscard]] BVector3< T > Cross( const BVector3< T >& p ) const
		{
			BVector3< T > Result;

			Result.x = this->y * p.z - this->z * p.y;
			Result.y = this->z * p.x - this->x * p.z;
			Result.z = this->x * p.y - this->y * p.x;

			return Result;
		}

		void operator+=( const BVector3< T >& p )
		{
			this->x += p.x;
			this->y += p.y;
			this->z += p.z;
		}

		void operator-=( const BVector3< T >& p )
		{
			this->x -= p.x;
			this->y -= p.y;
			this->z -= p.z;
		}

		void operator*=( const T& p )
		{
			*this = *this * p;
		}

		void operator*=( const BVector3< T >& p )
		{
			*this = *this * p;
		}

		void operator/=( const T& p )
		{
			*this = *this / p;
		}

		void operator/=( const BVector3< T >& p )
		{
			*this = *this / p;
		}

		bool operator==( const BVector3< T >& p )
		{
			return x == p.x && y == p.y && z == p.z;
		}

		[[nodiscard]] T norm() const
		{
			return this->x * this->x + this->y * this->y + this->z * this->z;
		}

		[[nodiscard]] T length() const
		{
			return sqrt( norm() );
		}

		void normalize()
		{
			T n = norm();

			if( n > 0 )
			{
				T div = sqrt( n );
				this->x /= div, this->y /= div, this->z /= div;
			}
		}

		static BVector3< T > up()
		{
			return BVector3< T >( T( 0 ), T( 1 ), T( 0 ) );
		}
		static BVector3< T > right()
		{
			return BVector3< T >( T( 1 ), T( 0 ), T( 0 ) );
		}
		static BVector3< T > forward()
		{
			return BVector3< T >( T( 0 ), T( 0 ), T( 1 ) );
		}
	};

	typedef BVector3< float > BVec3f;

	template< typename T >
	class BQuaternion
	{
		typedef BVector3< T > VecType;

	public:
		T s;
		VecType v;

		BQuaternion< T >()
			: s( 0 )
			, v( VecType() )
		{
		}

		BQuaternion< T >( const T scalar, const BVector3< T >& vec )
		{
			this->s = scalar;
			this->v = vec;
		}

		~BQuaternion< T >()
		{
		}

		BQuaternion< T >( const BQuaternion& value )
		{
			this->s = value.s;
			this->v = value.v;
		}

		BQuaternion< T >& operator=( const BQuaternion& value )
		{
			this->s = value.s;
			this->v = value.v;
			return *this;
		}

		void operator+=( const BQuaternion& q )
		{
			s += q.s;
			v += q.v;
		}

		BQuaternion< T > operator+( const BQuaternion& q ) const
		{
			const T scalar = s + q.s;
			const VecType imaginary = v + q.v;
			return BQuaternion< T >( scalar, imaginary );
		}

		void operator-=( const BQuaternion& q )
		{
			s -= q.s;
			v -= q.v;
		}

		BQuaternion< T > operator-( const BQuaternion< T >& q ) const
		{
			const T scalar = s - q.s;
			const VecType imaginary = v - q.v;
			return BQuaternion< T >( scalar, imaginary );
		}

		void operator*=( const BQuaternion< T >& q )
		{
			( *this ) = multiply( q );
		}

		BQuaternion< T > operator*( const BQuaternion< T >& q ) const
		{
			return multiply( q );
		}

		BQuaternion< T > multiply( const BQuaternion< T >& q ) const
		{
			const T scalar = s * q.s - v.Dot( q.v );
			const VecType imaginary = q.v * s + v * q.s + v.Cross( q.v );
			return BQuaternion< T >( scalar, imaginary );
		}

		void operator*=( const T value )
		{
			s *= value;
			v *= value;
		}

		BQuaternion< T > operator*( const T value ) const
		{
			const T scalar = s * value;
			const VecType imaginary = v * value;
			return BQuaternion< T >( scalar, imaginary );
		}

		T norm()
		{
			const T scalar = s * s;
			const VecType imaginary = v * v;
			return std::sqrt( scalar + imaginary.x + imaginary.y + imaginary.z );
		}

		void normalize()
		{
			if( norm() != 0 )
			{
				const T normValue = 1.f / norm();
				s *= normValue;
				v *= normValue;
			}
		}

		void convertToUnitNormQuaternion()
		{
			const T angle = s * fPi / 180.f;
			v.normalize();
			s = cosf( angle * 0.5f );
			v *= sinf( angle * 0.5f );
		}

		BQuaternion< T > conjugate()
		{
			const T scalar = s;
			const VecType imaginary = v * ( -1.f );
			return BQuaternion< T >( scalar, imaginary );
		}

		BQuaternion< T > inverse()
		{
			T absoluteValue = norm();
			absoluteValue *= absoluteValue;
			absoluteValue = 1 / absoluteValue;
			BQuaternion< T > conjugateValue = conjugate();
			const T scalar = conjugateValue.s * absoluteValue;
			const VecType imaginary = conjugateValue.v * absoluteValue;
			return BQuaternion< T >( scalar, imaginary );
		}

		static VecType rotateVector( T uAngle, const VecType&& uAxis, const VecType& uVector )
		{
			return rotateVector( uAngle, uAxis, uVector );
		}

		static VecType rotateVector( T uAngle, const VecType& uAxis, const VecType& uVector )
		{
			//convert our vector to a pure quaternion
			BQuaternion< T > p( 0, uVector );

			//normalize the axis
			//uAxis.Normalize();

			//create the real quaternion
			BQuaternion< T > q( uAngle, uAxis );

			//convert quaternion to unit norm quaternion
			q.convertToUnitNormQuaternion();

			//Get the inverse of the quaternion
			BQuaternion< T > qInverse = q.inverse();

			//rotate the quaternion
			BQuaternion< T > rotatedVector = q * p * qInverse;

			//return the vector part of the quaternion
			return rotatedVector.v;
		}
	};

	typedef BQuaternion< float > BQuatf;
	typedef BQuaternion< double > BQuatd;

	template< typename T >
	class BRotator
	{
	public:
		T p, y, r;

		BRotator()
		{
			this->p = T();
			this->y = T();
			this->r = T();
		}

		BRotator( T p, T y )
		{
			this->p = p;
			this->y = y;
			this->r = T();
		}

		BRotator( T p, T y, T r )
		{
			this->p = p;
			this->y = y;
			this->r = r;
		}

		BRotator( const BRotator< T >& p )
		{
			this->p = p.p;
			this->y = p.y;
			this->r = p.r;
		}

		~BRotator()
		{
		}

		BVector3< T > Direction() const
		{
			BVector3< T > dir( T( 0 ), T( 0 ), T( 1 ) );

			dir = BQuaternion< T >::RotateAroundAxis( p, BVector3< T >( 1.f, 0.f, 0.f ), dir );
			dir = BQuaternion< T >::RotateAroundAxis( y, BVector3< T >( 0.f, 1.f, 0.f ), dir );
			dir = BQuaternion< T >::RotateAroundAxis( r, BVector3< T >( 0.f, 0.f, 1.f ), dir );

			return dir;
		}
	};

	typedef BRotator< float > BRotf;

	template< typename T >
	class BTransform
	{
	public:
		BVec3f translation;
		BRotf rotation;
		BVec3f scale;

		BTransform() = default;

		BTransform( BVec3f&& inTranslation, BRotf&& inRotation, BVec3f&& inScale )
			: translation( inTranslation )
			, rotation( inRotation )
			, scale( inScale )
		{
		}

		BTransform( BVec3f& inTranslation, BRotf& inRotation, BVec3f& inScale )
			: translation( inTranslation )
			, rotation( inRotation )
			, scale( inScale )
		{
		}

		BTransform( BVec3f&& inTranslation, BRotf&& inRotation )
			: translation( inTranslation )
			, rotation( inRotation )
		{
		}

		BTransform( BVec3f& inTranslation, BRotf& inRotation )
			: translation( inTranslation )
			, rotation( inRotation )
		{
		}

		BTransform( BVec3f&& inTranslation )
			: translation( inTranslation )
		{
		}

		BTransform( BVec3f& inTranslation )
			: translation( inTranslation )
		{
		}
	};

	template< typename T >
	class BTriangle
	{
	public:
		BVector3< T > v0, v1, v2;

		BTriangle() = default;
		BTriangle( const BVector3< T >&& v0, const BVector3< T >&& v1, const BVector3< T >&& v2 )
		{
			this->v0 = v0;
			this->v1 = v1;
			this->v2 = v2;
		}
		BTriangle( const BVector3< T >& v0, const BVector3< T >& v1, const BVector3< T >& v2 )
		{
			this->v0 = v0;
			this->v1 = v1;
			this->v2 = v2;
		}

		BVector3< T > GetPointOnSurface( const float u, const float v )
		{
			const BVector3< T > v0v1 = v1 - v0;
			const BVector3< T > v0v2 = v2 - v0;
			return v0 + v0v1 * u + v0v2 * v;
		}

		BTriangle< T >& Sum( const BTriangle< T >& p )
		{
			v0.Sum( p.v0 );
			v1.Sum( p.v1 );
			v2.Sum( p.v2 );
		}

		BTriangle< T >& Subtract( const BTriangle< T >& p )
		{
			v0.Subtract( p.v0 );
			v1.Subtract( p.v1 );
			v2.Subtract( p.v2 );
		}

		BTriangle< T >& Multiply( const BTriangle< T >& p )
		{
			v0.Multiply( p.v0 );
			v1.Multiply( p.v1 );
			v2.Multiply( p.v2 );
		}

		BTriangle< T >& Divide( const BTriangle< T >& p )
		{
			v0.Divide( p.v0 );
			v1.Divide( p.v1 );
			v2.Divide( p.v2 );
		}
	};

	template< typename T >
	class BCube
	{
		
	public:
		BVector3< T > origin;
		T width, height, depth;
		T halfWidth, halfHeight, halfDepth;
		T min[ static_cast< int >( EBAxis::DIMENSIONS ) ];
		T max[ static_cast< int >( EBAxis::DIMENSIONS ) ];

		BCube()
			: origin{ T{ 0 }, T{ 0 }, T{ 0 } }
			, width{ T{ 2 } }
			, height{ T{ 2 } }
			, depth{ T{ 2 } }
			, halfWidth{ T{ 1 } }
			, halfHeight{ T{ 1 } }
			, halfDepth{ T{ 1 } }
			, min{ T{ -1 }, T{ -1 }, T{ -1 } }
			, max{ T{ 1 }, T{ 1 }, T{ 1 } }
		{
		}

		BCube( const BVector3< T >& origin, const T width, const T height, const T depth )
			: origin{ origin }
			, width{ width }
			, height{ height }
			, depth{ depth }
		{
			halfWidth = width / T{ 2 };
			halfHeight = height / T{ 2 };
			halfDepth = depth / T{ 2 };
			constexpr int x = static_cast< int >( EBAxis::X );
			constexpr int y = static_cast< int >( EBAxis::Y );
			constexpr int z = static_cast< int >( EBAxis::Z );
			min[ x ] = origin.x - halfWidth;
			min[ y ] = origin.y - halfHeight;
			min[ z ] = origin.z - halfDepth;
			max[ x ] = origin.x + halfWidth;
			max[ y ] = origin.y + halfHeight;
			max[ z ] = origin.z + halfDepth;
		}
	};

	typedef BCube< float > BCubef;

	template< typename T >
	class BLine
	{
	public:
		T p1;
		T p2;

		BLine()
			: p1( T() )
			, p2( T() )
		{
		}
		BLine( const T& p1, const T& p2 )
			: p1( p1 )
			, p2( p2 )
		{
		}
		BLine( const BLine< T >& l )
			: p1( l.p1 )
			, p2( l.p2 )
		{
		}
		~BLine()
		{
		}

		const BLine operator-() const
		{
			return BLine( p2, p1 );
		}

		bool operator==( const BLine< T >& val )
		{
			return p1 == val.p1 && p2 == val.p2;
		}
	};

	typedef BVector2< int32_t > BPixelPos;

	class BBoxEdges
	{
		typedef BLine< BPixelPos > EdgeType;

		EdgeType edges[ 4 ];

	public:
		BBoxEdges()
		{
			//BGL_LOG("Creating an empty BBoxEdges object!");
		}

		BBoxEdges(
			const EdgeType& topEdge,
			const EdgeType& bottomEdge,
			const EdgeType& LeftEdge,
			const EdgeType& rightEdge )
		{
			edges[ ( uint32_t )BEBoxEdge::Top ] = topEdge;
			edges[ ( uint32_t )BEBoxEdge::Bottom ] = bottomEdge;
			edges[ ( uint32_t )BEBoxEdge::Left ] = LeftEdge;
			edges[ ( uint32_t )BEBoxEdge::Right ] = rightEdge;
		}

		BBoxEdges( const BBoxEdges& otherBox )
		{
			memcpy( edges, otherBox.GetEdges(), sizeof( EdgeType ) * 4 );
		}

		[[nodiscard]] const EdgeType* GetEdges() const
		{
			return edges;
		}

		EdgeType& operator[]( const BEBoxEdge&& index )
		{
			return edges[ ( uint32_t )index ];
		}

		EdgeType& operator[]( uint32_t index )
		{
			return edges[ index ];
		}
	};

	template< typename T >
	class BBox : public BLine< T >
	{
	public:
		BBox()
		{
			this->p1 = T();
			this->p2 = T();
		}

		BBox( const T& p1, const T& p2 )
		{
			this->p1 = p1;
			this->p2 = p2;
		}

		BBox( const BBox< T >& b )
		{
			this->p1 = b.p1;
			this->p2 = b.p2;
		}

		~BBox()
		{
		}

		template< typename Y >
		[[nodiscard]] bool IsIn( const BVector2< Y >& v ) const
		{
			const bool bInRangeX = v.x >= this->p1.x && v.x <= this->p2.x;
			const bool bInRangeY = v.y >= this->p1.y && v.y <= this->p2.y;
			return bInRangeX && bInRangeY;
		}
	};

	template< typename T >
	class BBuffer
	{
		T* _memBlock;

		size_t _length;
		size_t _allocLength;

	public:
		BBuffer( size_t length = 0 )
		{
			BGL_ASSERT( length > 0 && "Can't start buffer with length <= 0!" );

			_memBlock = new T[ length ];
			_length = length;
			_allocLength = length;
		}

		BBuffer( size_t length, T defaultValue )
		{
			BGL_ASSERT( length > 0 && "Can't start buffer with length <= 0!" );

			_memBlock = new T[ length ];
			_length = length;
			_allocLength = length;

			SetBufferValue( defaultValue );
		}

		BBuffer( T* data, size_t length )
		{
			BGL_ASSERT( length > 0 && "Can't start buffer with length <= 0!" );

			_memBlock = data;
			_length = length;
			_allocLength = length;
		}

		BBuffer( T* data, size_t length, T defaultValue )
		{
			BBuffer( data, length );
			SetBufferValue( defaultValue );
		}

		T& operator[]( size_t index )
		{
			BGL_ASSERT( index < Length() && "Out of range!" );
			return _memBlock[ index ];
		}

		const T& operator[]( size_t index ) const
		{
			BGL_ASSERT( index < Length() && "Out of range!" );
			return _memBlock[ index ];
		}

		[[nodiscard]] size_t Length() const
		{
			return _length;
		}

		T* GetData()
		{
			return _memBlock;
		}

		void Allocate( size_t allocLength, bool bMove = false )
		{
			BGL_ASSERT( allocLength > 0 && "Got negative allocLength!" );

			if( allocLength > _allocLength )
			{
				BGL_LOG( "Reallocating Buffer!" );

				T* newMemBlock = new T[ allocLength ];

				if( bMove )
				{
					memcpy( newMemBlock, _memBlock, _allocLength );
				}

				_allocLength = allocLength;
				delete[] _memBlock;

				_memBlock = newMemBlock;
			}
		}

		void SetLength( size_t newLength, bool bMove = false )
		{
			BGL_ASSERT( newLength > 0 && "Got negative newLength!" );

			if( newLength > _allocLength )
			{
				Allocate( newLength, bMove );
			}

			_length = newLength;
		}

		void SetBufferValue( T value )
		{
			std::fill_n( _memBlock, _length, value );
		}
	};

} // namespace bgl