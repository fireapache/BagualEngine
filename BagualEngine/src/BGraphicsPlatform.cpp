#include "BGraphicsPlatform.h"

#ifdef _WIN32
#include "BGenericGraphicsDriver.h"
#endif // _WIN32

namespace Bagual::Graphics
{

	BGraphicsPlatform::BGraphicsPlatform()
	{
		driverInstance = std::make_unique<

#ifdef _WIN32
			Bagual::Graphics::BGenericGraphicsDriver
#endif // _WIN32

			>();

		canvases = std::make_unique<Bagual::Types::BArray<std::shared_ptr<BCanvas>>>();
	}

	void BGraphicsPlatform::RenderFrame()
	{
		if (driverInstance)
		{
			driverInstance->RenderFrame();
		}
	}

	std::shared_ptr<Bagual::Graphics::BCanvas> BGraphicsPlatform::CreateCanvas(unsigned short width, unsigned short height)
	{
		auto canvas = std::make_shared<BCanvas>(width, height);
		canvases->Add(canvas);
		return canvas;
	}

	std::weak_ptr<BViewport> BGraphicsPlatform::CreateViewport(BCanvas& canvas, FViewportSettings viewportSettings)
	{
		std::shared_ptr<BViewport> viewport = std::make_shared<BViewport>(viewportSettings);

		viewports->Add(viewport);

		return viewport;
	}

	void BGraphicsPlatform::Delay(unsigned int ms)
	{
		if (driverInstance)
		{
			driverInstance->Delay(ms);
		}
	}

	void BGraphicsPlatform::RenderCamera(std::shared_ptr<Bagual::Camera::BCamera> camera)
	{
		if (driverInstance)
		{
			driverInstance->RenderCamera(camera);
		}
	}

}

