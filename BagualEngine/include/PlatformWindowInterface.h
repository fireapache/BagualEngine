#pragma once

#include "PlatformDataTypes.h"

namespace bgl
{
	class BPlatformWindowInterface
	{

	public:

		virtual FWindowSettings GetWindowSettings() = 0;

		virtual void SetWindowSettings(const FWindowSettings& newSettings) = 0;

	};
}