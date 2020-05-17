#pragma once

#include <memory>

#include "BPlatformBase.h"

namespace Bagual::Platform
{
	class BGenericPlatform : public BPlatformBase
	{
		virtual std::shared_ptr<BPlatformWindowInterface> CreateWindow(const FWindowSettings& settings) override;
	};
}