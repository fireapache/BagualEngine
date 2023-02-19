
// clang-format off
#include "Bagual.pch.h"
// clang-format on

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
		m_driverInstance = std::make_unique< BGraphicsDriverGeneric >();
		m_viewports = BArray< std::shared_ptr< BViewport > >();
	}

	const BArray< BViewport* > BGraphicsPlatform::GetViewports() const
	{
		return m_viewportsRaw;
	}

	void BGraphicsPlatform::SetEnabled( const bool bValue )
	{
		m_driverInstance->SetEnabled( bValue );
	}

	bool BGraphicsPlatform::IsEnabled() const
	{
		return m_driverInstance->IsEnabled();
	}

	void BGraphicsPlatform::SwapFrames()
	{
		if( m_driverInstance )
		{
			m_driverInstance->SwapFrames();
		}
	}

	BViewport* BGraphicsPlatform::CreateViewport( BCanvas* canvas )
	{
		return RegisterViewport( canvas );
	}

	BViewport* BGraphicsPlatform::CreateViewport( BCanvas* canvas, const BBox< BVector2< float > > normalizedSize )
	{
		return RegisterViewport( canvas, normalizedSize );
	}

	BViewport* BGraphicsPlatform::CreateViewport( BCanvas* canvas, const uint32_t width, const uint32_t height )
	{
		return RegisterViewport( canvas, width, height );
	}

	BViewport* BGraphicsPlatform::CreateViewport(
		BCanvas* canvas,
		const uint32_t x,
		const uint32_t y,
		const uint32_t width,
		const uint32_t height )
	{
		return RegisterViewport( canvas, x, y, width, height );
	}

	void BGraphicsPlatform::Delay( const uint32_t&& ms )
	{
		if( m_driverInstance )
		{
			m_driverInstance->Delay( ms );
		}
	}

	void BGraphicsPlatform::Delay( const uint32_t& ms )
	{
		if( m_driverInstance )
		{
			m_driverInstance->Delay( ms );
		}
	}

	void BGraphicsPlatform::RenderCamera( const BCamera& camera )
	{
		if( m_driverInstance )
		{
			m_driverInstance->RenderCamera( camera );
		}
	}

} // namespace bgl
