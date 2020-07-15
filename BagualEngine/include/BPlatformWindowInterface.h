#pragma once

#include "BPlatformDataTypes.h"

namespace bgl
{
	class BPlatformWindowInterface
	{

	public:

		virtual FWindowSettings GetWindowSettings() = 0;

		virtual void SetWindowSettings(const FWindowSettings& newSettings) = 0;

	};
}