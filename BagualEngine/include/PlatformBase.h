#pragma once

#include "PlatformInterface.h"
#include "Singleton.h"

namespace bgl
{
	class BPlatformBase : public bgl::BSingleton<BPlatformBase>, public BPlatformInterface
	{

	};
}