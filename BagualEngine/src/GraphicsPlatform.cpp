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

	std::shared_ptr<BCanvas> BGraphicsPlatform::CreateCanvas(const ushort& width, const ushort& height)
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

	void BGraphicsPlatform::Delay(const uint&& ms)
	{
		if (driverInstance)
		{
			driverInstance->Delay(ms);
		}
	}

	void BGraphicsPlatform::Delay(const uint& ms)
	{
		if (driverInstance)
		{
			driverInstance->Delay(ms);
		}
	}

	void BGraphicsPlatform::RenderCamera(const BCamera& camera)
	{
		if (driverInstance)
		{
			driverInstance->RenderCamera(camera);
		}
	}

}

