#pragma once

#include "Common.h"

namespace bgl
{
	class BPlatformWindow;
	class BViewport;
} // namespace bgl

namespace bgl
{
	class BCanvas
	{
		uint16_t width;
		uint16_t height;

		BLine< BPixelPos > edges[ 4 ];

		std::shared_ptr< BBuffer< CanvasDataType > > wireframeBuffer;
		std::shared_ptr< BBuffer< CanvasDataType > > colorBuffer;
		std::shared_ptr< BBuffer< DepthDataType > > zBuffer;
		std::shared_ptr< BBuffer< CanvasDataType > > readyFrameBuffer;

		inline void UpdateEdges();

		BPlatformWindow* window;
		BArray< BViewport* > m_viewports;

	public:
		BCanvas( BPlatformWindow* window, uint16 width, uint16 height );

		[[nodiscard]] uint16_t GetWidth() const;
		[[nodiscard]] uint16_t GetHeight() const;

		void AllocateBuffers();
		void ResetZBuffer();
		void ResetWireframeBuffer();

		[[nodiscard]] const BLine< BPixelPos >* GetEdges() const;
		[[nodiscard]] const BLine< BPixelPos >& GetEdge( BEBoxEdge edge ) const;

		[[nodiscard]] BBuffer< CanvasDataType >& GetColorBuffer() const;
		[[nodiscard]] BBuffer< DepthDataType >& GetZBuffer() const;
		[[nodiscard]] BBuffer< CanvasDataType >& GetWireframeBuffer() const;
		[[nodiscard]] BBuffer< CanvasDataType >& GetReadyFrameBuffer() const;

		[[nodiscard]] BPlatformWindow* GetWindow() const;
		[[nodiscard]] BArray< BViewport* > GetViewports() const;

		void SetSize( const uint16_t newWidth, const uint16_t newHeight );
	};
} // namespace bgl