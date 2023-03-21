#pragma once

#include "Common.h"
#include "GraphicsDriverBase.h"

namespace bgl
{
	class BGraphicsPlatform
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
		
		[[nodiscard]] const BArray< BViewport* >& GetViewports() const
		{
			return m_viewportsRaw;
		}

		[[nodiscard]] BGraphicsDriverInterface* getGraphicsDriver() const
		{
			return m_driverInstance.get();
		}

		void SetEnabled( bool bValue )
		{
			m_driverInstance->SetEnabled( bValue );
		}

		bool IsEnabled() const
		{
			return m_driverInstance->IsEnabled();
		}

		BViewport* CreateViewport( BCanvas* canvas );
		BViewport* CreateViewport( BCanvas* canvas, BBox< BVector2< float > > normalizedSize );
		BViewport* CreateViewport( BCanvas* canvas, uint32_t width, uint32_t height );
		BViewport* CreateViewport( BCanvas* canvas, uint32_t x, uint32_t y, uint32_t width, uint32_t height );
	};
} // namespace bgl