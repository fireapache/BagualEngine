
#include "Bagual.pch.h"

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

			BGraphicsDriverGeneric

			>();

		viewports = std::make_unique<BArray<std::shared_ptr<BViewport>>>();
	}

	void BGraphicsPlatform::SwapFrames()
	{
		if (driverInstance)
		{
			driverInstance->SwapFrames();
		}
	}

	std::shared_ptr<BViewport> BGraphicsPlatform::CreateViewport(const std::shared_ptr<BCanvas>& canvas)
	{
		auto viewport = std::make_shared<BViewport>(canvas);

		BGL_ASSERT(viewport != nullptr && "Failed creating viewport!");

		viewports->Add(viewport);

		return viewport;
	}

	std::shared_ptr<BViewport> BGraphicsPlatform::CreateViewport(const std::shared_ptr<BCanvas>& canvas, const BBox<BVector2<float>>& normalizedSize)
	{
		auto viewport = std::make_shared<BViewport>(canvas, normalizedSize);

		BGL_ASSERT(viewport != nullptr && "Failed creating viewport!");

		viewports->Add(viewport);

		return viewport;
	}

	std::shared_ptr<BViewport> BGraphicsPlatform::CreateViewport(const std::shared_ptr<BCanvas>& canvas, const uint32& width, const uint32& height)
	{
		auto viewport = std::make_shared<BViewport>(canvas, width, height);

		BGL_ASSERT(viewport != nullptr && "Failed creating viewport!");

		viewports->Add(viewport);

		return viewport;
	}

	std::shared_ptr<BViewport> BGraphicsPlatform::CreateViewport(const std::shared_ptr<BCanvas>& canvas, const uint32& x, const uint32& y, const uint32& width, const uint32& height)
	{
		auto viewport = std::make_shared<BViewport>(canvas, x, y, width, height);

		BGL_ASSERT(viewport != nullptr && "Failed creating viewport!");

		viewports->Add(viewport);

		return viewport;
	}

	void BGraphicsPlatform::Delay(const uint32&& ms)
	{
		if (driverInstance)
		{
			driverInstance->Delay(ms);
		}
	}

	void BGraphicsPlatform::Delay(const uint32& ms)
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

