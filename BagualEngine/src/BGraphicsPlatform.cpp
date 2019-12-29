#include "BGraphicsPlatform.h"

#ifdef _WIN32
#include "BWindowsGraphicsDriver.h"
#endif // _WIN32

namespace Bagual::Graphics
{

	BGraphicsPlatform::BGraphicsPlatform()
	{
		driverInstance = std::make_unique<

#ifdef _WIN32
			Bagual::Graphics::BWindowsGraphicsDriver
#endif // _WIN32

			>();

		canvases = std::make_unique<Bagual::Types::BArray<std::shared_ptr<BCanvas>>>();
	}

	void BGraphicsPlatform::Render()
	{
		if (driverInstance)
		{
			driverInstance->Render();
		}
	}

	std::weak_ptr<Bagual::Graphics::BCanvas> BGraphicsPlatform::CreateCanvas(unsigned short width, unsigned short height)
	{
		auto canvas = std::make_shared<BCanvas>(width, height);
		canvases->Add(canvas);
		return canvas;
	}

	void BGraphicsPlatform::Delay(unsigned int ms)
	{
		if (driverInstance)
		{
			driverInstance->Delay(ms);
		}
	}


}

