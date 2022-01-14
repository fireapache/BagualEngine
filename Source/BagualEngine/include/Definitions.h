#pragma once

namespace bgl
{

	constexpr float kInfinity = std::numeric_limits<float>::max();
	constexpr float kEpsilon = static_cast<float>(1e-8);
	constexpr double dPi = 3.14159265358979323846;
	constexpr float fPi = static_cast<float>(dPi);

	typedef int32_t int32;
	typedef int16_t int16;
	typedef int8_t int8;

	typedef uint32_t uint32;
	typedef uint16_t uint16;
	typedef uint8_t uint8;

	typedef uint32 CanvasDataType;
	typedef double DepthDataType;


}