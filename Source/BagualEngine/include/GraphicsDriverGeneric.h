#pragma once

#include "GraphicsDriverBase.h"

namespace bgl
{
	class BViewport;
}

namespace bgl
{
	enum class BERenderSpeed
	{
		Normal,
		Fast,
		VeryFast
	};

	enum class BESceneSetup
	{
		Empty,
		WithObjects,
		ObjectsCharacter
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
		BViewport* viewport;
		// Pixel location and final color
		uint32 px, py, rgb;
		// Type of render output
		BERenderOutputType renderType;
		// Max distance to render on pixel depth output mode
		double depthDist;
		// Resolution scale to render
		BERenderSpeed renderSpeed;
	};

	class BGraphicsDriverGeneric : public BGraphicsDriverBase
	{
		class BGenericPlatformWindow* m_cachedPlatformWindowPtr = nullptr;

		static void ScanTriangles_Sequential(BArray<BTriangle<float>>& compTris, BFTriangleScanParams& p);
		static void ScanTriangles_SIMD(BTriangle<BArray<float>>& compTris, BFTriangleScanParams& p);

		static void PaintPixelWithShader(BFTriangleScanParams& p);

	public:

		BGraphicsDriverGeneric();
		~BGraphicsDriverGeneric();

		static void RenderLines(BViewport* viewport, const uint32 renderThreadIndex);
		static void DrawLine(BViewport* viewport, BLine<BPixelPos> line);
		static void PaintPixel(BViewport* viewportPtr, BERenderSpeed renderSpeed, uint32 i, uint32 j, uint32 rgb);

		void SwapFrames() override;
		void RenderGameFrame() override;
		void SwapUIFrame() override;
		void SwapGameFrame() override;
		void Delay(const uint32&& ms) override;
		void Delay(const uint32& ms) override;

	};
}



