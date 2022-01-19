
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
		m_driverInstance = std::make_unique<BGraphicsDriverGeneric>();
		m_viewports = BArray<std::shared_ptr<BViewport>>();
	}

	void BGraphicsPlatform::SetEnabled(const bool bValue)
	{
		m_driverInstance->SetEnabled(bValue);
	}

	bool BGraphicsPlatform::IsEnabled() const
	{
		return m_driverInstance->IsEnabled();
	}

	void BGraphicsPlatform::SwapFrames()
	{
		if (m_driverInstance)
		{
			m_driverInstance->SwapFrames();
		}
	}

	BViewport* BGraphicsPlatform::CreateViewport(BCanvas* canvas)
	{
		m_viewports.Add(std::make_shared<BViewport>(canvas));
		auto viewport = m_viewports.back().get();
		BGL_ASSERT(viewport != nullptr && "Failed creating viewport!");
		return viewport;
	}

	BViewport* BGraphicsPlatform::CreateViewport(BCanvas* canvas, const BBox<BVector2<float>>& normalizedSize)
	{
		m_viewports.Add(std::make_shared<BViewport>(canvas, normalizedSize));
		auto viewport = m_viewports.back().get();
		BGL_ASSERT(viewport != nullptr && "Failed creating viewport!");
		return viewport;
	}

	BViewport* BGraphicsPlatform::CreateViewport(BCanvas* canvas, const uint32& width, const uint32& height)
	{
		m_viewports.Add(std::make_shared<BViewport>(canvas, width, height));
		auto viewport = m_viewports.back().get();
		BGL_ASSERT(viewport != nullptr && "Failed creating viewport!");
		return viewport;
	}

	BViewport* BGraphicsPlatform::CreateViewport(BCanvas* canvas, const uint32& x, const uint32& y, const uint32& width, const uint32& height)
	{
		m_viewports.Add(std::make_shared<BViewport>(canvas, x, y, width, height));
		auto viewport = m_viewports.back().get();
		BGL_ASSERT(viewport != nullptr && "Failed creating viewport!");
		return viewport;
	}

	void BGraphicsPlatform::Delay(const uint32&& ms)
	{
		if (m_driverInstance)
		{
			m_driverInstance->Delay(ms);
		}
	}

	void BGraphicsPlatform::Delay(const uint32& ms)
	{
		if (m_driverInstance)
		{
			m_driverInstance->Delay(ms);
		}
	}

	void BGraphicsPlatform::RenderCamera(const BCamera& camera)
	{
		if (m_driverInstance)
		{
			m_driverInstance->RenderCamera(camera);
		}
	}

}

