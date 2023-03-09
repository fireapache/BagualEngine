#pragma once

#include <functional>
#include <stdint.h>
#include <limits>

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

	enum class EBAxis : uint8_t
	{
		X = 0,
		Y = 1,
		Z = 2,
		DIMENSIONS = 3
	};

} // namespace bgl