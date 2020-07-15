
#include "GenericPlatform.h"
#include "GenericPlatformWindow.h"

namespace bgl
{
	std::shared_ptr<BPlatformWindowInterface> BGenericPlatform::CreateWindow(const FWindowSettings& settings)
	{
		return nullptr;//std::make_shared<BPlatformWindowInterface>(new BGenericPlatformWindow(settings));
	}
}