#include "BGraphicsPlatform.h"

#ifdef _WIN32
#include "BWindowsGraphicsDriver.h"
#endif // _WIN32

namespace Bagual::GraphicsPlatform
{

	BGraphicsPlatform::BGraphicsPlatform()
	{
		DriverInstance = std::make_unique<

#ifdef _WIN32
			Bagual::GraphicsDriver::BWindowsGraphicsDriver
#endif // _WIN32

			>();
	}

	void BGraphicsPlatform::Render()
	{
		if (DriverInstance)
		{
			DriverInstance->Render();
		}
	}

	

}

