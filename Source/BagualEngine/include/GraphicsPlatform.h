#pragma once

#include "Common.h"
#include "GraphicsDriverBase.h"

namespace bgl
{
	class BGraphicsPlatform : public BGraphicsDriverInterface
	{
		friend std::unique_ptr< BGraphicsDriverInterface > std::make_unique< BGraphicsDriverInterface >();
		friend std::unique_ptr< BArray< std::shared_ptr< BCanvas > > > std::make_unique< BArray< std::shared_ptr< BCanvas > > >();
		//friend std::unique_ptr<BViewport> std::make_unique<BViewport>();

		std::unique_ptr< BGraphicsDriverInterface > m_driverInstance;
		BArray< std::shared_ptr< BViewport > > m_viewports;
		BArray< BViewport* > m_viewportsRaw;

		template< typename... P >
		BViewport* RegisterViewport( P&... args )
		{
			auto viewport = std::make_shared< BViewport >( args... );
			BGL_ASSERT( viewport != nullptr && "Failed creating viewport!" );
			m_viewports.add( viewport );
			auto viewportRaw = viewport.get();
			m_viewportsRaw.add( viewportRaw );
			return viewportRaw;
		}

	public:
		BGraphicsPlatform();

		BERenderOutputType& GetRenderOutputType_Mutable();
		void SetRenderOutputType( BERenderOutputType type );

		const BArray< BViewport* > GetViewports() const;

		virtual void SetEnabled( const bool bValue );
		virtual bool IsEnabled() const;
		virtual void SwapFrames() override;
		virtual void RenderCamera( const BCamera& camera ) override;
		virtual void Delay( const uint32_t&& ms ) override;
		virtual void Delay( const uint32_t& ms ) override;
		virtual BViewport* CreateViewport( BCanvas* canvas );
		virtual BViewport* CreateViewport( BCanvas* canvas, const BBox< BVector2< float > > normalizedSize );
		virtual BViewport* CreateViewport( BCanvas* canvas, const uint32_t width, const uint32_t height );
		virtual BViewport* CreateViewport(
			BCanvas* canvas,
			const uint32_t x,
			const uint32_t y,
			const uint32_t width,
			const uint32_t height );
	};
} // namespace bgl