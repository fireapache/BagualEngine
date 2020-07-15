#pragma once

#include <memory>

#include "BPlatformBase.h"

namespace bgl
{
	class BGenericPlatform : public BPlatformBase
	{
		virtual std::shared_ptr<BPlatformWindowInterface> CreateWindow(const FWindowSettings& settings) override;
	};
}