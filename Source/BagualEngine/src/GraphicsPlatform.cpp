
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
	
} // namespace bgl
