#pragma once

#include "GraphicsDriverBase.h"

namespace bgl
{
	class BViewport;
	class BRenderStage;
}

namespace bgl
{
	enum class BERenderSpeed
	{
		Normal,
		Fast,
		VeryFast
	};
	
	enum class BERenderThreadMode
	{
		SingleThread,
		MultiThread,
		HyperThread
	};

	enum class BEIntrinsicsMode
	{
		Off,
		AVX
	};

	struct BFTriangleScanParams
	{
		// Ray direction
		BVec3f orig, dir;
		// Triangle intersection
		float t, u, v;
		// Viewport to render
		BViewport* viewport = nullptr;
		// Pixel location and final color
		uint32_t px, py, rgb;
		// Type of render output
		BERenderOutputType renderType = BERenderOutputType::UvColor;
		// Max distance to render on pixel depth output mode
		double depthDist;
		// Resolution scale to render
		BERenderSpeed renderSpeed = BERenderSpeed::Fast;
		// Hit any geometry
		bool bHit = false;
	};

	class BGraphicsDriverGeneric : public BGraphicsDriverBase
	{
		class BGenericPlatformWindow* m_cachedPlatformWindowPtr = nullptr;

		static void ScanTriangles_Sequential( BArray< BTriangle< float > >& compTris, BFTriangleScanParams& p );
		static void ScanTriangles_SIMD( BTriangle< BArray< float > >& compTris, BFTriangleScanParams& p );

		static void PaintPixelWithShader( BFTriangleScanParams& p );

	public:
		BGraphicsDriverGeneric();
		~BGraphicsDriverGeneric();

		static void RenderLines( BRenderStage* renderStage, BViewport* viewport, const uint32_t renderThreadIndex );
		static void Draw3DLine( BViewport* viewport, const BLine< BVec3f >& line, const Color32Bit color );
		static void Draw2DLine( BViewport* viewport, const BLine< BPixelPos >& line, const Color32Bit color );
		static void PaintPixel(
			BViewport* viewportPtr,
			const BERenderSpeed renderSpeed,
			const uint32_t i,
			const uint32_t j,
			const uint32_t rgb );

		void SwapFrames() override;
		static void composeFinalFrame(
			Color32Bit* colorPtr,
			Color32Bit* wireframePtr,
			Color32Bit* destBuffer,
			const Color32Bit* bufferEnd );
		void RenderGameFrame() override;
		void SwapUIFrame() override;
		void SwapGameFrame() override;
		void Delay( const uint32_t&& ms ) override;
		void Delay( const uint32_t& ms ) override;
	};
} // namespace bgl
