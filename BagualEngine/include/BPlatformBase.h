#pragma once

#include "BPlatformInterface.h"
#include "BSingleton.h"

namespace bgl
{
	class BPlatformBase : public bgl::BSingleton<BPlatformBase>, public BPlatformInterface
	{

	};
}