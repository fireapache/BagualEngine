#pragma once

#include "Assertions.h"
#include "Definitions.h"
#include "Logger.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <emmintrin.h>
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

		// Converts euler vector into direction vector
		// Note: doesn't use roll
		BVector3< T > direction()
		{
			BVector3< T > direction;

			// Convert angles from degrees to radians
			//const T phi = z * T( dPi ) / T( 180.0 );
			const T theta = y * T( dPi ) / T( 180 );
			const T psi = x * T( dPi ) / T( 180 );

			// Convert Euler angles to direction vector
			direction.x = cos( theta ) * cos( psi );
			direction.y = cos( theta ) * sin( psi );
			direction.z = sin( theta );

			return direction;
		}
	};

	typedef BVector3< float > BVec3f;

	class BSIMDQuaternion
	{
	public:
		__m128 data{};

		BSIMDQuaternion( const BVector3< float >& vec )
		{
			const std::array values{ 0.0f, vec.x, vec.y, vec.z };
			data = _mm_loadu_ps( values.data() );
		}

		BSIMDQuaternion( const float scalar, const BVector3< float >& vec )
		{
			const std::array values{ scalar, vec.x, vec.y, vec.z };
			data = _mm_loadu_ps( values.data() );
		}

		BSIMDQuaternion( const float w, const float x, const float y, const float z )
		{
			const std::array values{ w, x, y, z };
			data = _mm_loadu_ps( values.data() );
		}

		[[nodiscard]] float s() const
		{
			std::array< float, 4 > result{};
			_mm_storeu_ps( result.data(), data );
			return result[ 0 ];
		}

		[[nodiscard]] BVec3f v() const
		{
			std::array< float, 4 > result{};
			_mm_storeu_ps( result.data(), data );
			BVec3f vec;
			memcpy( &( vec.x ), result.data() + 1, sizeof( float ) * 3 );
			return vec;
		}

		// based on https://stackoverflow.com/a/22219399
		void multiply( const BSIMDQuaternion& q )
		{
			__m128 a = data;
			__m128 b = q.data;
			__m128 a1123 = _mm_shuffle_ps( a, a, 0xE5 );
			__m128 a2231 = _mm_shuffle_ps( a, a, 0x7A );
			__m128 b1000 = _mm_shuffle_ps( b, b, 0x01 );
			__m128 b2312 = _mm_shuffle_ps( b, b, 0x9E );
			__m128 t1 = _mm_mul_ps( a1123, b1000 );
			__m128 t2 = _mm_mul_ps( a2231, b2312 );
			__m128 t12 = _mm_add_ps( t1, t2 );
			const __m128i mask = _mm_set_epi32( 0, 0, 0, 0x80000000 );
			__m128 t12m = _mm_xor_ps( t12, _mm_castsi128_ps( mask ) ); // flip sign bits
			__m128 a3312 = _mm_shuffle_ps( a, a, 0x9F );
			__m128 b3231 = _mm_shuffle_ps( b, b, 0x7B );
			__m128 a0000 = _mm_shuffle_ps( a, a, 0x00 );
			__m128 t3 = _mm_mul_ps( a3312, b3231 );
			__m128 t0 = _mm_mul_ps( a0000, b );
			__m128 t03 = _mm_sub_ps( t0, t3 );
			data = _mm_add_ps( t03, t12m );
		}

		[[nodiscard]] BVec3f toVec3f() const
		{
			std::array< float, 4 > simdData{};
			_mm_storeu_ps( simdData.data(), data );
			const float& x = simdData[ 1 ];
			const float& y = simdData[ 2 ];
			const float& z = simdData[ 3 ];
			const float& w = simdData[ 0 ];
			// clang-format off
			return
			{
				2.0f * ( x * z + w * y ),
				2.0f * ( y * z - w * x ),
				1.0f - 2.0f * ( x * x + y * y )
			};
			// clang-format on
		}
	};

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

		BQuaternion< T >( const BVector3< T >& vec )
			: s( 0 )
			, v( vec )
		{
		}

		BQuaternion< T >( const T scalar, const BVector3< T >& vec )
			: s( scalar )
			, v( vec )
		{
		}

		~BQuaternion< T >()
		{
		}

		BQuaternion< T >( const BQuaternion& value )
			: s( value.s )
			, v( value.v )
		{
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

		[[nodiscard]] BQuaternion< T > multiply( const BQuaternion< T >& q ) const
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

		BQuaternion< T > conjugate() const
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

		static void rotateVector( const BQuaternion< T >& q, VecType& v )
		{
			// From Eigen
			BQuaternion< T > p( 0, v );
			v = ( q * p * q.conjugate() ).v;
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
	};

	typedef BRotator< float > BRotf;

	class BMatrix3x3
	{
	public:
		float data[ 9 ];

		const float* operator[]( size_t&& i ) const
		{
			return data + i * 3;
		}

		float* operator[]( size_t&& i )
		{
			return data + i * 3;
		}

		[[nodiscard]] float get( size_t i, size_t j ) const
		{
			return *( ( data + i * 3 ) + j );
		}

		void rotateVector( BVec3f& vec ) const
		{
			size_t j = 0;
			for( size_t i = 0; i < 3; i++ )
			{
				const __m128 row = _mm_loadu_ps( ( data + j ) );
				const __m128 vector = _mm_loadu_ps( &( vec.x ) );
				__m128 mul = _mm_mul_ps( row, vector );
				mul = _mm_add_ps( mul, _mm_movehl_ps( mul, mul ) );
				mul = _mm_add_ss( mul, _mm_shuffle_ps( mul, mul, 1 ) );
				_mm_store_ss( &( vec.x ) + i, mul );
				j += 3;
			}
		}

		// Based on https://stackoverflow.com/a/1569893
		static BMatrix3x3 fromEuler( const BRotf& rotator, EEulerOrder eulerOrder )
		{
			// Convert Euler Angles passed in a vector of Radians
			// into a rotation matrix.  The individual Euler Angles are
			// processed in the order requested.
			BMatrix3x3 Mx{};

			const float Sx = sinf( rotator.p * fPi / 180.0f );
			const float Sy = sinf( -rotator.y * fPi / 180.0f );
			const float Sz = sinf( rotator.r * fPi / 180.0f );
			const float Cx = cosf( rotator.p * fPi / 180.0f );
			const float Cy = cosf( -rotator.y * fPi / 180.0f );
			const float Cz = cosf( rotator.r * fPi / 180.0f );

			switch( eulerOrder )
			{
			case ORDER_XYZ:
				Mx[ 0 ][ 0 ] = Cy * Cz;
				Mx[ 0 ][ 1 ] = -Cy * Sz;
				Mx[ 0 ][ 2 ] = Sy;
				Mx[ 1 ][ 0 ] = Cz * Sx * Sy + Cx * Sz;
				Mx[ 1 ][ 1 ] = Cx * Cz - Sx * Sy * Sz;
				Mx[ 1 ][ 2 ] = -Cy * Sx;
				Mx[ 2 ][ 0 ] = -Cx * Cz * Sy + Sx * Sz;
				Mx[ 2 ][ 1 ] = Cz * Sx + Cx * Sy * Sz;
				Mx[ 2 ][ 2 ] = Cx * Cy;
				break;

			case ORDER_YZX:
				Mx[ 0 ][ 0 ] = Cy * Cz;
				Mx[ 0 ][ 1 ] = Sx * Sy - Cx * Cy * Sz;
				Mx[ 0 ][ 2 ] = Cx * Sy + Cy * Sx * Sz;
				Mx[ 1 ][ 0 ] = Sz;
				Mx[ 1 ][ 1 ] = Cx * Cz;
				Mx[ 1 ][ 2 ] = -Cz * Sx;
				Mx[ 2 ][ 0 ] = -Cz * Sy;
				Mx[ 2 ][ 1 ] = Cy * Sx + Cx * Sy * Sz;
				Mx[ 2 ][ 2 ] = Cx * Cy - Sx * Sy * Sz;
				break;

			case ORDER_ZXY:
				Mx[ 0 ][ 0 ] = Cy * Cz - Sx * Sy * Sz;
				Mx[ 0 ][ 1 ] = -Cx * Sz;
				Mx[ 0 ][ 2 ] = Cz * Sy + Cy * Sx * Sz;
				Mx[ 1 ][ 0 ] = Cz * Sx * Sy + Cy * Sz;
				Mx[ 1 ][ 1 ] = Cx * Cz;
				Mx[ 1 ][ 2 ] = -Cy * Cz * Sx + Sy * Sz;
				Mx[ 2 ][ 0 ] = -Cx * Sy;
				Mx[ 2 ][ 1 ] = Sx;
				Mx[ 2 ][ 2 ] = Cx * Cy;
				break;

			case ORDER_ZYX:
				Mx[ 0 ][ 0 ] = Cy * Cz;
				Mx[ 0 ][ 1 ] = Cz * Sx * Sy - Cx * Sz;
				Mx[ 0 ][ 2 ] = Cx * Cz * Sy + Sx * Sz;
				Mx[ 1 ][ 0 ] = Cy * Sz;
				Mx[ 1 ][ 1 ] = Cx * Cz + Sx * Sy * Sz;
				Mx[ 1 ][ 2 ] = -Cz * Sx + Cx * Sy * Sz;
				Mx[ 2 ][ 0 ] = -Sy;
				Mx[ 2 ][ 1 ] = Cy * Sx;
				Mx[ 2 ][ 2 ] = Cx * Cy;
				break;

			case ORDER_YXZ:
				Mx[ 0 ][ 0 ] = Cy * Cz + Sx * Sy * Sz;
				Mx[ 0 ][ 1 ] = Cz * Sx * Sy - Cy * Sz;
				Mx[ 0 ][ 2 ] = Cx * Sy;
				Mx[ 1 ][ 0 ] = Cx * Sz;
				Mx[ 1 ][ 1 ] = Cx * Cz;
				Mx[ 1 ][ 2 ] = -Sx;
				Mx[ 2 ][ 0 ] = -Cz * Sy + Cy * Sx * Sz;
				Mx[ 2 ][ 1 ] = Cy * Cz * Sx + Sy * Sz;
				Mx[ 2 ][ 2 ] = Cx * Cy;
				break;

			case ORDER_XZY:
				Mx[ 0 ][ 0 ] = Cy * Cz;
				Mx[ 0 ][ 1 ] = -Sz;
				Mx[ 0 ][ 2 ] = Cz * Sy;
				Mx[ 1 ][ 0 ] = Sx * Sy + Cx * Cy * Sz;
				Mx[ 1 ][ 1 ] = Cx * Cz;
				Mx[ 1 ][ 2 ] = -Cy * Sx + Cx * Sy * Sz;
				Mx[ 2 ][ 0 ] = -Cx * Sy + Cy * Sx * Sz;
				Mx[ 2 ][ 1 ] = Cz * Sx;
				Mx[ 2 ][ 2 ] = Cx * Cy + Sx * Sy * Sz;
				break;
			}
			return ( Mx );
		}

		[[nodiscard]] BSIMDQuaternion toQuaternion() const
		{
			// Ref: http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
			float w, x, y, z;
			const BMatrix3x3& m = *this;
			const float trace = m[ 0 ][ 0 ] + m[ 1 ][ 1 ] + m[ 2 ][ 2 ];
			if( trace > 0 )
			{
				const float s = 0.5f / sqrt( trace + 1.0f );
				w = 0.25f / s;
				x = ( m[ 2 ][ 1 ] - m[ 1 ][ 2 ] ) * s;
				y = ( m[ 0 ][ 2 ] - m[ 2 ][ 0 ] ) * s;
				z = ( m[ 1 ][ 0 ] - m[ 0 ][ 1 ] ) * s;
			}
			else
			{
				if( m[ 0 ][ 0 ] > m[ 1 ][ 1 ] && m[ 0 ][ 0 ] > m[ 2 ][ 2 ] )
				{
					const float s = 2.0f * sqrt( 1.0f + m[ 0 ][ 0 ] - m[ 1 ][ 1 ] - m[ 2 ][ 2 ] );
					w = ( m[ 2 ][ 1 ] - m[ 1 ][ 2 ] ) / s;
					x = 0.25f * s;
					y = ( m[ 0 ][ 1 ] + m[ 1 ][ 0 ] ) / s;
					z = ( m[ 0 ][ 2 ] + m[ 2 ][ 0 ] ) / s;
				}
				else if( m[ 1 ][ 1 ] > m[ 2 ][ 2 ] )
				{
					const float s = 2.0f * sqrt( 1.0f + m[ 1 ][ 1 ] - m[ 0 ][ 0 ] - m[ 2 ][ 2 ] );
					w = ( m[ 0 ][ 2 ] - m[ 2 ][ 0 ] ) / s;
					x = ( m[ 0 ][ 1 ] + m[ 1 ][ 0 ] ) / s;
					y = 0.25f * s;
					z = ( m[ 1 ][ 2 ] + m[ 2 ][ 1 ] ) / s;
				}
				else
				{
					const float s = 2.0f * sqrt( 1.0f + m[ 2 ][ 2 ] - m[ 0 ][ 0 ] - m[ 1 ][ 1 ] );
					w = ( m[ 1 ][ 0 ] - m[ 0 ][ 1 ] ) / s;
					x = ( m[ 0 ][ 2 ] + m[ 2 ][ 0 ] ) / s;
					y = ( m[ 1 ][ 2 ] + m[ 2 ][ 1 ] ) / s;
					z = 0.25f * s;
				}
			}
			return BSIMDQuaternion( w, x, y, z );
		}
	};

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

		BBoxEdges( const EdgeType& topEdge, const EdgeType& bottomEdge, const EdgeType& LeftEdge, const EdgeType& rightEdge )
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