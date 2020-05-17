#pragma once

#include "BPlatformInterface.h"
#include "BSingleton.h"

namespace Bagual::Platform
{
	class BPlatformBase : public Bagual::Utils::BSingleton<BPlatformBase>, public BPlatformInterface
	{

	};
}