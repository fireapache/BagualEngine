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

		std::shared_ptr< BBuffer< Color32Bit > > wireframeBuffer;
		std::shared_ptr< BBuffer< Color32Bit > > colorBuffer;
		std::shared_ptr< BBuffer< DepthDataType > > zBuffer;
		std::shared_ptr< BBuffer< Color32Bit > > readyFrameBuffer;

		inline void UpdateEdges();

		BPlatformWindow* window;
		BArray< BViewport* > m_viewports;

	public:
		BCanvas( BPlatformWindow* window, uint16_t width, uint16_t height );

		[[nodiscard]] uint16_t getWidth() const;
		[[nodiscard]] uint16_t getHeight() const;

		void allocateBuffers();
		void resetZBuffer();
		void resetWireframeBuffer();

		[[nodiscard]] const BLine< BPixelPos >* getEdges() const;
		[[nodiscard]] const BLine< BPixelPos >& getEdge( BEBoxEdge edge ) const;

		[[nodiscard]] BBuffer< Color32Bit >& getColorBuffer() const;
		[[nodiscard]] BBuffer< DepthDataType >& getZBuffer() const;
		[[nodiscard]] BBuffer< Color32Bit >& getWireframeBuffer() const;
		[[nodiscard]] BBuffer< Color32Bit >& getReadyFrameBuffer() const;

		[[nodiscard]] BPlatformWindow* getWindow() const;
		[[nodiscard]] BArray< BViewport* >& getViewports();

		void setSize( const uint16_t newWidth, const uint16_t newHeight );
	};
} // namespace bgl