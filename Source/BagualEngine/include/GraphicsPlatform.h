#pragma once

#include "Common.h"

#include "GraphicsDriverBase.h"

namespace bgl
{
	class BGraphicsPlatform : public BGraphicsDriverInterface
	{
		friend std::unique_ptr<BGraphicsDriverInterface> std::make_unique<BGraphicsDriverInterface>();
		friend std::unique_ptr<BArray<std::shared_ptr<BCanvas>>> std::make_unique<BArray<std::shared_ptr<BCanvas>>>();
		//friend std::unique_ptr<BViewport> std::make_unique<BViewport>();

		std::unique_ptr<BGraphicsDriverInterface> m_driverInstance;
		BArray<std::shared_ptr<BViewport>> m_viewports;

		BERenderOutputType m_renderOutputType = BERenderOutputType::Depth;

	public:

		BGraphicsPlatform();

		BERenderOutputType GetRenderOutputType() const;
		BERenderOutputType& GetRenderOutputType_Mutable();
		void SetRenderOutputType(BERenderOutputType type);

		virtual void SetEnabled(const bool bValue);
		virtual bool IsEnabled() const;
		virtual void SwapFrames() override;
		virtual void RenderCamera(const BCamera& camera) override;
		virtual void Delay(const uint32&& ms) override;
		virtual void Delay(const uint32& ms) override;
		virtual BViewport* CreateViewport(BCanvas* canvas);
		virtual BViewport* CreateViewport(BCanvas* canvas, const BBox<BVector2<float>>& normalizedSize);
		virtual BViewport* CreateViewport(BCanvas* canvas, const uint32& width, const uint32& height);
		virtual BViewport* CreateViewport(BCanvas* canvas, const uint32& x, const uint32& y, const uint32& width, const uint32& height);

	};
}