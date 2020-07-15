#include "GraphicsPlatform.h"

#ifdef _WIN32
#include "GraphicsDriverGeneric.h"
#endif // _WIN32

#include "Canvas.h"
#include "Viewport.h"

namespace bgl
{
	
	BGraphicsPlatform::BGraphicsPlatform()
	{
		driverInstance = std::make_unique<

#ifdef _WIN32
			BGraphicsDriverGeneric
#endif // _WIN32

			>();

		canvases = std::make_unique<BArray<std::shared_ptr<BCanvas>>>();
	}

	void BGraphicsPlatform::RenderFrame()
	{
		if (driverInstance)
		{
			driverInstance->RenderFrame();
		}
	}

	std::shared_ptr<BCanvas> BGraphicsPlatform::CreateCanvas(unsigned short width, unsigned short height)
	{
		auto canvas = std::make_shared<BCanvas>(width, height);
		canvases->Add(canvas);
		return canvas;
	}

	std::weak_ptr<BViewport> BGraphicsPlatform::CreateViewport(BCanvas& canvas, const FViewportSettings& viewportSettings)
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

	void BGraphicsPlatform::RenderCamera(std::shared_ptr<BCamera> camera)
	{
		if (driverInstance)
		{
			driverInstance->RenderCamera(camera);
		}
	}

}

