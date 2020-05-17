
#include "BGenericPlatform.h"
#include "BGenericPlatformWindow.h"

namespace Bagual::Platform
{
	std::shared_ptr<BPlatformWindowInterface> BGenericPlatform::CreateWindow(const FWindowSettings& settings)
	{
		return nullptr;//std::make_shared<BPlatformWindowInterface>(new BGenericPlatformWindow(settings));
	}
}