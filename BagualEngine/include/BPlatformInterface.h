#pragma once

#include <memory>

#include "BPlatformWindowInterface.h"

namespace Bagual::Platform
{
	class BPlatformInterface
	{

	public:

		virtual std::shared_ptr<BPlatformWindowInterface> CreateWindow(const FWindowSettings& settings) = 0;
	};
}