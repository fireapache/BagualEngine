#pragma once

#include <functional>
#include <stdint.h>
#include <limits>

#define BGL_OPTIMIZATION_OFF __pragma( optimize( "", off ) )
#define BGL_OPTIMIZATION_ON __pragma( optimize( "", on ) )

namespace bgl
{

	constexpr float kInfinity = std::numeric_limits< float >::max();
	constexpr float kEpsilon = static_cast< float >( 1e-8 );
	constexpr double dPi = 3.14159265358979323846;
	constexpr float fPi = static_cast< float >( dPi );
	
	typedef uint32_t Color32Bit;
	typedef double DepthDataType;
	typedef std::function< void() > GuiTickFuncType;

	constexpr auto DEFAULT_CANVAS_VALUE = static_cast< Color32Bit >( 0 );
	constexpr auto DEFAULT_DEPTH_VALUE = static_cast< DepthDataType >( 9999999999999 );

	enum class BERenderOutputType : uint8_t
	{
		Depth,
		UvColor
	};
	
	enum EEulerOrder
	{
		ORDER_XYZ,
		ORDER_YZX,
		ORDER_ZXY,
		ORDER_ZYX,
		ORDER_YXZ,
		ORDER_XZY
	};

	enum class EBAxis : uint8_t
	{
		X = 0,
		Y = 1,
		Z = 2,
		DIMENSIONS = 3
	};

	enum DebugFlags
	{
		DBF_None = 0,
		DBF_SceneConstruction = 1,
		DBF_ThreadsTick = 1 << 1
	};

	enum ECameraRotationMethod
	{
		None,
		Naive,
		Quaternion,
		Matrix,
		glm
	};

	constexpr ECameraRotationMethod DefaultCameraRotationMethod{ Naive };

} // namespace bgl