
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "GraphicsDriverGeneric.h"

#include "BagualEngine.h"
#include "Camera.h"
#include "Draw.h"
#include "GraphicsPlatform.h"
#include "PlatformGeneric.h"
#include "Scene.h"
#include "Settings.h"
#include "ThreadPool.h"
#include "Viewport.h"
#include <imgui.h>
#include <limits>
#include <thread>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <bvh/v2/bvh.h>
#include <bvh/v2/vec.h>
#include <bvh/v2/ray.h>
#include <bvh/v2/node.h>
#include <bvh/v2/thread_pool.h>
#include <bvh/v2/stack.h>
#include <bvh/v2/tri.h>
// clang-format on

namespace bgl
{
	void ErrorCallback( int error, const char* description )
	{
		BGL_LOG( description );
	}

	BGraphicsDriverGeneric::BGraphicsDriverGeneric()
	{
		BGL_ASSERT( glfwInit() && "Could not start GLFW!" );
		glfwSetErrorCallback( ErrorCallback );
	}

	BGraphicsDriverGeneric::~BGraphicsDriverGeneric()
	{
		StopGameFrameRenderingThread();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
	}

	void BGraphicsDriverGeneric::SwapFrames()
	{
		auto& platform = BEngine::Platform();
		auto& windows = platform.getWindows();

		for( auto& window : windows )
		{
			m_cachedPlatformWindowPtr = dynamic_cast< BGenericPlatformWindow* >( window.get() );

			if( m_cachedPlatformWindowPtr )
			{
				auto glfwWindow = m_cachedPlatformWindowPtr->GetGLFW_Window();
				glfwMakeContextCurrent( glfwWindow );
				glClear( GL_COLOR_BUFFER_BIT );
				BGraphicsDriverBase::SwapFrames();
				glfwSwapBuffers( glfwWindow );
			}
		}
	}

	void BGraphicsDriverGeneric::composeFinalFrame(
		Color32Bit* colorPtr,
		Color32Bit* wireframePtr,
		Color32Bit* destBuffer,
		const Color32Bit* bufferEnd )
	{
		while( colorPtr < bufferEnd )
		{
			if( *wireframePtr > 0 /*&& zBufferData*/ )
			{
				*destBuffer = *wireframePtr;
			}
			else
			{
				*destBuffer = *colorPtr;
			}
			wireframePtr++;
			colorPtr++;
			destBuffer++;
			//zBufferData++;
		}
	}

	void BGraphicsDriverGeneric::RenderGameFrame()
	{
		BEngine::Scene().sceneSemaphore.acquire();

		const auto renderStage = BEngine::Scene().getNextRenderStage();

		if( !renderStage )
		{
			BEngine::Scene().sceneSemaphore.release();
			return;
		}

		if( BSettings::isDebugFlagsSet( DBF_ThreadsTick ) )
		{
			std::cout << "frame: " << frameCount << std::endl;
		}

		const auto renderStartTime = std::chrono::system_clock::now();

		const auto viewports = BEngine::GraphicsPlatform().GetViewports();

		// Rendering each viewport
		for( auto viewport : viewports )
		{
#pragma region Asserts

			if( viewport == nullptr )
			{
				BGL_LOG( "Got null viewport during render!" );
				continue;
			}

			auto camera = viewport->GetCamera();

			if( camera == nullptr )
			{
				BGL_LOG( "Got null camera during render!" );
				continue;
			}

			auto canvas = viewport->GetCanvas();

			BGL_ASSERT( canvas != nullptr && "Got null canvas during render!" );

#pragma endregion

			// Getting thread pool ready
			const auto renderThreadMode = camera->GetRenderThreadMode();
			const bool bUseHyperThread = renderThreadMode == BERenderThreadMode::HyperThread;
			const auto processorCount = std::thread::hardware_concurrency() * ( bUseHyperThread ? 2 : 1 );
			uint32_t renderThreadCount = renderThreadMode == BERenderThreadMode::SingleThread ? 1 : processorCount;
			thread_pool renderThreadPool( renderThreadCount );

#pragma region Pushing Geometry Tasks

			canvas->resetZBuffer();

			for( uint32_t t = 0; t < processorCount; t++ )
			{
				renderThreadPool.push_task( RenderLines, renderStage, viewport, t );
			}

#pragma endregion

#pragma region Pushing 2D and 3D Lines Tasks

			canvas->resetWireframeBuffer();

			const auto edgesData = renderStage->edges.data();
			const auto edgesLength = renderStage->edges.size();

			for( auto* edgePtr = edgesData; edgePtr < edgesData + edgesLength; edgePtr++ )
			{
				renderThreadPool.push_task( Draw3DLine, viewport, edgePtr->line3D, edgePtr->color );
			}

			auto& lines3D = camera->GetLine3DBuffer();

			for( auto& line3D : lines3D )
			{
				renderThreadPool.push_task( Draw3DLine, viewport, line3D, BSettings::lineColor );
			}

			camera->ClearLine3DBuffer();

			auto& lines2D = camera->GetLine2DBuffer();

			for( auto& line2D : lines2D )
			{
				renderThreadPool.push_task( Draw2DLine, viewport, line2D, BSettings::lineColor );
			}

			camera->ClearLine2DBuffer();

#pragma endregion

			// syncing all threads
			renderThreadPool.wait_for_tasks();

			auto& wireframeBuffer = canvas->getWireframeBuffer();
			Color32Bit* wireframdeBufferData = wireframeBuffer.GetData();
			auto& colorBuffer = canvas->getColorBuffer();
			Color32Bit* colorBufferData = colorBuffer.GetData();
			auto& readyFrameBuffer = canvas->getReadyFrameBuffer();
			Color32Bit* readyFrameBufferData = readyFrameBuffer.GetData();

			// TODO: make wireframe work with zbuffer
			//auto& zBuffer = canvas->GetZBuffer();
			//DepthDataType* zBufferData = zBuffer.GetData();

			const size_t bufferTotalSize = colorBuffer.Length();
			const size_t threadPixelLength = bufferTotalSize / renderThreadCount;

			for( size_t i = 0; i < renderThreadCount; i++ )
			{
				size_t memOffset = threadPixelLength * i;
				Color32Bit* colorPtr = colorBufferData + memOffset;
				Color32Bit* wireframePtr = wireframdeBufferData + memOffset;
				Color32Bit* destBuffer = readyFrameBufferData + memOffset;
				const Color32Bit* colorBufferEnd = colorBufferData + bufferTotalSize;
				renderThreadPool.push_task( composeFinalFrame, colorPtr, wireframePtr, destBuffer, colorBufferEnd );
			}

			// syncing all threads one last time to have final composed frame
			renderThreadPool.wait_for_tasks();
		}

		const auto renderEndTime = std::chrono::system_clock::now();
		const auto renderDuration = renderEndTime - renderStartTime;

		lastRenderTime = static_cast< double >( renderDuration.count() );

		BEngine::Scene().sceneSemaphore.release();
		frameCount++;
	}

	void BGraphicsDriverGeneric::Draw3DLine( BViewport* viewport, const BLine< BVec3f >& line, const Color32Bit color )
	{
		BDraw::DrawLine( viewport, line, color );
	}

	void BGraphicsDriverGeneric::Draw2DLine( BViewport* viewport, const BLine< BPixelPos >& line, const Color32Bit color )
	{
		BDraw::DrawLine( viewport, line, color );
	}

	void BGraphicsDriverGeneric::RenderLines( BRenderStage* renderStage, BViewport* viewport, const uint32_t renderThreadIndex )
	{
		if( viewport == nullptr )
		{
			BGL_LOG( "Got null viewport when rendering lines!" );
			return;
		}

		const auto camera = viewport->GetCamera();

		if( camera == nullptr )
		{
			BGL_LOG( "Got null camera when rendering lines!" );
			return;
		}

		// camera sensor settings

		const uint32_t width = viewport->GetSize().width;
		const uint32_t height = viewport->GetSize().height;

		const auto sensorArea = camera->GetSensorArea();
		const float sensorDistance = camera->GetSensorDistance();

		// keeping some variables stacked

		BFTriangleScanParams triangleScanParams;
		triangleScanParams.renderType = camera->GetRenderOutputType();
		triangleScanParams.orig = camera->GetLocation();
		triangleScanParams.depthDist = camera->GetDepthDistance();
		triangleScanParams.renderSpeed = camera->GetRenderSpeed();
		triangleScanParams.viewport = viewport;

		// camera setup
		const auto renderThreadMode = camera->GetRenderThreadMode();
		const auto cameraRotator = camera->GetRotator();
		const auto cameraMatrix = BMatrix3x3::fromEuler( cameraRotator, ORDER_XYZ );
		const BSIMDQuaternion cameraRotation{ cameraMatrix.toQuaternion() };
		const auto renderMode = camera->GetRenderMode();
		const auto cameraRotationMethod = BEngine::GraphicsPlatform().getGraphicsDriver()->GetCameraRotationMethod_Mutator();

		// getting render lines of interest

		const auto processorCount
			= std::thread::hardware_concurrency() * ( renderThreadMode == BERenderThreadMode::HyperThread ? 2 : 1 );
		const uint32_t threadCount = processorCount <= 0 ? 1 : processorCount;

		const uint32_t lineRange = height / threadCount;
		const uint32_t lineStart = renderThreadIndex * lineRange;
		const uint32_t lineEnd = ( renderThreadIndex + 1 == threadCount ) ? height : ( renderThreadIndex + 1 ) * lineRange;

		// starting line rendering

		const bool bRenderNormalSpeed = triangleScanParams.renderSpeed == BERenderSpeed::Normal;
		const uint32_t renderSpeed = static_cast< uint32_t >( triangleScanParams.renderSpeed ) * 2;
		const uint32_t renderSpeedStep = ( bRenderNormalSpeed ? 1 : renderSpeed );

		for( uint32_t j = lineStart; j < lineEnd; j += renderSpeedStep )
		{
			for( uint32_t i = 0; i < width; i += renderSpeedStep )
			{
				triangleScanParams.px = i;
				triangleScanParams.py = j;
				triangleScanParams.bHit = false;
				triangleScanParams.t = std::numeric_limits< float >::max();

				// Getting ray rotation
				const float unitX = static_cast< float >( i ) / static_cast< float >( width - 1 ) - 0.5f;
				const float unitY = static_cast< float >( j ) / static_cast< float >( height - 1 ) - 0.5f;

				BVec3f rayDirection( sensorArea.x * unitX, -sensorArea.y * unitY, sensorDistance );
				rayDirection.normalize();

				BVec3f vUp( BVec3f::up() );
				BVec3f vRight( BVec3f::right() );
				BVec3f vForward( BVec3f::forward() );
				
				switch( cameraRotationMethod )
				{
				case Naive:

					// yaw
					rayDirection = BQuatf::rotateVector( cameraRotator.y, vUp, rayDirection );

					// pitch
					vRight = BQuatf::rotateVector( cameraRotator.y, vUp, vRight );
					rayDirection = BQuatf::rotateVector( cameraRotator.p, vRight, rayDirection );

					// roll
					vForward = BQuatf::rotateVector( cameraRotator.y, BVec3f::up(), vForward );
					vForward = BQuatf::rotateVector( cameraRotator.p, BVec3f::right(), vForward );
					rayDirection = BQuatf::rotateVector( cameraRotator.r, vForward, rayDirection );

					break;

				case Quaternion:

					BSIMDQuaternion rayRotation{ rayDirection };
					rayRotation.multiply( cameraRotation );
					rayDirection = rayRotation.v();

					break;
				}
				
				triangleScanParams.dir = rayDirection;

				// Getting scene triangles

				switch( renderMode )
				{
				case BERenderMode::Sequential:
					ScanTriangles_Sequential( renderStage, triangleScanParams );
					break;
				case BERenderMode::SIMD:
					ScanTriangles_SIMD( renderStage, triangleScanParams );
					break;
				case BERenderMode::BVH:
					ScanTriangles_BVH( renderStage, triangleScanParams );
					break;
				case BERenderMode::Embree:
					ScanTriangles_Embree( renderStage, triangleScanParams );
					break;
				}
				
				if( triangleScanParams.bHit )
				{
					PaintPixelWithShader( triangleScanParams );
				}
				else
				{
					auto& p = triangleScanParams;
					PaintPixel( p.viewport, p.renderSpeed, p.px, p.py, 0x00 );
				}
			}
		}
	}

	inline void BGraphicsDriverGeneric::ScanTriangles_Sequential( BRenderStage* renderStage, BFTriangleScanParams& p )
	{
		BArray< BTriangle< float > >& compTris = renderStage->triangles;

		BFTriangleScanParams lp = p;

		for( auto tri : compTris )
		{
			if( BDraw::RayTriangleIntersect( lp.orig, lp.dir, tri, lp.t, lp.u, lp.v ) )
			{
				if( lp.t < p.t )
				{
					p = lp;
					p.bHit = true;
				}
			}
		}
	}

#pragma region AVX Specialization

	__m256 operator-( const __m256& p1, const __m256 p2 )
	{
		return _mm256_sub_ps( p1, p2 );
	}

	__m256 operator+( const __m256& p1, const __m256 p2 )
	{
		return _mm256_add_ps( p1, p2 );
	}

	__m256 operator*( const __m256& p1, const __m256 p2 )
	{
		return _mm256_mul_ps( p1, p2 );
	}

	__m256 operator/( const __m256& p1, const __m256 p2 )
	{
		return _mm256_div_ps( p1, p2 );
	}

#pragma endregion

	inline void BGraphicsDriverGeneric::ScanTriangles_SIMD( BRenderStage* renderStage, BFTriangleScanParams& p )
	{
		BTriangle< BArray< float > >& compTris = renderStage->triangles_SIMD;

		const size_t triCount = compTris.v0.x.size();
		const size_t notSimdTriCount = triCount % 8;

		// Stacking data and variables

		BTriangle< float* > triData;
		triData.v0.x = compTris.v0.x.data();
		triData.v0.y = compTris.v0.y.data();
		triData.v0.z = compTris.v0.z.data();

		triData.v1.x = compTris.v1.x.data();
		triData.v1.y = compTris.v1.y.data();
		triData.v1.z = compTris.v1.z.data();

		triData.v2.x = compTris.v2.x.data();
		triData.v2.y = compTris.v2.y.data();
		triData.v2.z = compTris.v2.z.data();

		BTriangle< __m256 > tri;
		BVector3< __m256 > orig, dir1, dir2, pvec;
		BVector3< __m256 > tvec, qvec;
		__m256 u, v, t, uv, culling, det, invDet, uvgrt1;
		__m256 fail1, fail2, uless0, ugrt1, vless0, invalidHit;

		__m256 pixelDepth = _mm256_set1_ps( std::numeric_limits< float >::max() );
		__m256 validDepth;
		__m256i canCopy;

		// Getting aligned floats for efficient output of SIMD
		constexpr size_t dataAlignment = 32;
		constexpr size_t floatCount = 8;

		struct finalPixelInfo
		{
			float t[ floatCount ];
			float u[ floatCount ];
			float v[ floatCount ];
		};

		BStackAligned< dataAlignment, finalPixelInfo > finalPixel;

		_mm256_store_ps( finalPixel.get()->t, _mm256_set1_ps( std::numeric_limits< float >::max() ) );

		// Core loop

		for( size_t i = 0; i < triCount; i += 8 )
		{
			// @TODO: make these loads aligned!
			tri.v0.x = _mm256_loadu_ps( triData.v0.x + i );
			tri.v0.y = _mm256_loadu_ps( triData.v0.y + i );
			tri.v0.z = _mm256_loadu_ps( triData.v0.z + i );

			tri.v1.x = _mm256_loadu_ps( triData.v1.x + i );
			tri.v1.y = _mm256_loadu_ps( triData.v1.y + i );
			tri.v1.z = _mm256_loadu_ps( triData.v1.z + i );

			tri.v2.x = _mm256_loadu_ps( triData.v2.x + i );
			tri.v2.y = _mm256_loadu_ps( triData.v2.y + i );
			tri.v2.z = _mm256_loadu_ps( triData.v2.z + i );

			pixelDepth = _mm256_load_ps( finalPixel.get()->t );

			orig.x = _mm256_set1_ps( p.orig.x );
			orig.y = _mm256_set1_ps( p.orig.y );
			orig.z = _mm256_set1_ps( p.orig.z );

			dir1.x = _mm256_set1_ps( p.dir.x );
			dir1.y = _mm256_set1_ps( p.dir.y );
			dir1.z = _mm256_set1_ps( p.dir.z );

			dir2 = dir1;

			// ==================================================================
			// === starting vectorization of BDraw::RayTriangleIntersect(...) ===
			// ==================================================================

			BVector3< __m256 >& v0v1 = tri.v1.Subtract( tri.v0 );
			BVector3< __m256 >& v0v2 = tri.v2.Subtract( tri.v0 );
			BVector3< __m256 >& pvec = dir1.CrossProduct( v0v2 );

			det = DotProduct( v0v1, pvec );

			// Checking if triangle is backfacing (true means it should be discarded)
			culling = _mm256_cmp_ps( det, _mm256_set1_ps( kEpsilon ), 1 );

			// invDet = 1 / det;
			invDet = _mm256_div_ps( _mm256_set1_ps( 1.f ), det );

			BVector3< __m256 >& tvec = orig.Subtract( tri.v0 );

			u = DotProduct( tvec, pvec ) * invDet;

			// if (u < 0 || u > 1) return false;
			uless0 = _mm256_cmp_ps( u, _mm256_set1_ps( 0.f ), 1 );
			ugrt1 = _mm256_cmp_ps( u, _mm256_set1_ps( 1.f ), 14 );
			fail1 = _mm256_or_ps( uless0, ugrt1 );

			BVector3< __m256 >& qvec = tvec.CrossProduct( v0v1 );
			v = dir2.DotProduct( qvec ) * invDet;

			// if (v < 0 || u + v > 1) return false;
			vless0 = _mm256_cmp_ps( v, _mm256_set1_ps( 0.f ), 1 );
			uv = u + v;
			uvgrt1 = _mm256_cmp_ps( uv, _mm256_set1_ps( 1.f ), 14 );
			fail2 = _mm256_or_ps( vless0, uvgrt1 );

			t = DotProduct( v0v2, qvec ) * invDet;

			invalidHit = _mm256_or_ps( _mm256_or_ps( culling, fail1 ), fail2 );
			validDepth = _mm256_cmp_ps( t, pixelDepth, 1 );
			canCopy = _mm256_castps_si256( _mm256_andnot_ps( invalidHit, validDepth ) );

			auto adrT = finalPixel.get()->t;
			auto adrU = finalPixel.get()->u;
			auto adrV = finalPixel.get()->v;

			_mm256_maskstore_ps( adrT, canCopy, t );
			_mm256_maskstore_ps( adrU, canCopy, u );
			_mm256_maskstore_ps( adrV, canCopy, v );
		}

		for( size_t i = 0; i < 8; i++ )
		{
			const float currentDist = finalPixel.get()->t[ i ];

			if( currentDist < p.t )
			{
				p.t = currentDist;
				p.u = finalPixel.get()->u[ i ];
				p.v = finalPixel.get()->v[ i ];
				p.bHit = true;
			}
		}
	}

	inline void BGraphicsDriverGeneric::ScanTriangles_BVH( BRenderStage* renderStage, BFTriangleScanParams& p )
	{
		using Scalar = float;
		using Vec3 = bvh::v2::Vec< Scalar, 3 >;
		using Box = bvh::v2::BBox< Scalar, 3 >;
		using Tri = bvh::v2::Tri< Scalar, 3 >;
		using Node = bvh::v2::Node< Scalar, 3 >;
		using Bvh = bvh::v2::Bvh< Node >;
		using Ray = bvh::v2::Ray< Scalar, 3 >;

		const Vec3 rayOrigin{ p.orig.x, p.orig.y, p.orig.z };
		const Vec3 rayDir = { p.dir.x, p.dir.y, p.dir.z };

		auto ray = bvh::v2::Ray< float, 3 >{
			rayOrigin, // Ray origin
			rayDir,	   // Ray direction
			0.,		   // Minimum intersection distance
			100.	   // Maximum intersection distance
		};

		static constexpr size_t invalid_id = std::numeric_limits< size_t >::max();
		static constexpr size_t stack_size = 64;
		static constexpr bool use_robust_traversal = false;

		auto prim_id = invalid_id;
		Scalar u, v;

		if( !renderStage->bvh.nodes.empty() )
		{
			// Traverse the BVH and get the u, v coordinates of the closest intersection.
			bvh::v2::SmallStack< Bvh::Index, stack_size > stack;
			renderStage->bvh.intersect< false, use_robust_traversal >(
				ray,
				renderStage->bvh.get_root().index,
				stack,
				[ & ]( size_t begin, size_t end )
				{
					for( size_t i = begin; i < end; ++i )
					{
						size_t j = renderStage->bvh.bPermuted ? i : renderStage->bvh.prim_ids[ i ];
						if( auto hit = renderStage->bvh.precomputed_tris[ j ].intersect( ray ) )
						{
							prim_id = i;
							std::tie( u, v ) = *hit;
						}
					}
					return prim_id != invalid_id;
				} );
		}

		if( prim_id != invalid_id )
		{
			p.bHit = true;
			p.u = u;
			p.v = v;
			p.t = ray.tmax;
		}
		else
		{
			p.bHit = false;
		}
	}

	inline void BGraphicsDriverGeneric::ScanTriangles_Embree( BRenderStage* renderStage, BFTriangleScanParams& p )
	{
		RTCRayHit rayhit;
		rayhit.ray.org_x = p.orig.x;
		rayhit.ray.org_y = p.orig.y;
		rayhit.ray.org_z = p.orig.z;
		rayhit.ray.dir_x = p.dir.x;
		rayhit.ray.dir_y = p.dir.y;
		rayhit.ray.dir_z = p.dir.z;
		rayhit.ray.tnear = 0;
		rayhit.ray.tfar = std::numeric_limits< float >::infinity();
		rayhit.ray.mask = -1;
		rayhit.ray.flags = 0;
		rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
		rayhit.hit.instID[ 0 ] = RTC_INVALID_GEOMETRY_ID;

		rtcIntersect1( renderStage->rtcScene, &rayhit );

		if( rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID )
		{
			p.bHit = true;
			p.t = rayhit.ray.tfar;
			p.u = rayhit.hit.u;
			p.v = rayhit.hit.v;
		}
	}

	inline void BGraphicsDriverGeneric::PaintPixelWithShader( BFTriangleScanParams& p )
	{
		const double depthZ = static_cast< double >( p.t ) * 100.0;
		const double currentDepthZ = p.viewport->GetPixelDepth( p.px, p.py );

		p.rgb = 0x000000;

		if( depthZ < currentDepthZ )
		{
			p.viewport->SetPixelDepth( p.px, p.py, depthZ );

#pragma region Depth Shader
			if( p.renderType == BERenderOutputType::Depth )
			{
				const double calcA = p.depthDist - depthZ;
				const double calcB = std::fmax( calcA, 0.0 );
				const double calcC = calcB / p.depthDist;

				const uint32_t gray = static_cast< uint32_t >( 255.0 * calcC );

				p.rgb = gray;
				p.rgb = ( p.rgb << 8 ) | gray;
				p.rgb = ( p.rgb << 8 ) | gray;

				PaintPixel( p.viewport, p.renderSpeed, p.px, p.py, p.rgb );
			}
#pragma endregion

#pragma region UV Coloring Shader
			else if( p.renderType == BERenderOutputType::UvColor )
			{
				const uint32_t r = static_cast< uint32_t >( 255.f * std::clamp( p.u, 0.f, 1.f ) );
				const uint32_t g = static_cast< uint32_t >( 255.f * std::clamp( p.v, 0.f, 1.f ) );
				const uint32_t b = static_cast< uint32_t >( 255.f * std::clamp( 1 - p.u - p.v, 0.f, 1.f ) );

				p.rgb = r;
				p.rgb = ( p.rgb << 8 ) | g;
				p.rgb = ( p.rgb << 8 ) | b;

				PaintPixel( p.viewport, p.renderSpeed, p.px, p.py, p.rgb );
			}
#pragma endregion
		}
	}

	void BGraphicsDriverGeneric::SwapUIFrame()
	{
		// Updating ImGui frames if assigned
		auto guiTickFuncs = BEngine::Instance().getGuiTickFuncs();

		if( guiTickFuncs.size() > 0 )
		{
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// User code
			for( const auto guiTickFunc : guiTickFuncs )
			{
				( *guiTickFunc )();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

			auto plat = static_cast< BPlatformGeneric* >( &BEngine::Instance().Platform() );
			ImGuiIO& io = *plat->GetImguiConfig();

			// Update and Render additional Platform Windows
			// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
			//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
			if( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent( backup_current_context );
			}
		}
	}

	void BGraphicsDriverGeneric::SwapGameFrame()
	{
		if( m_cachedPlatformWindowPtr && m_cachedPlatformWindowPtr->getCanvas() )
		{
#pragma region Rendering Window Canvas to OpenGL

			const auto canvas = m_cachedPlatformWindowPtr->getCanvas();

			auto glfwWindow = m_cachedPlatformWindowPtr->GetGLFW_Window();
			glfwMakeContextCurrent( glfwWindow );

			const GLsizei windowWidth = canvas->getWidth();
			const GLsizei windowHeight = canvas->getHeight();
			
			const auto pixels = canvas->getReadyFrameBuffer().GetData();
			
			if( pixels )
			{
				/* draw pixels to screen */
				glRasterPos2i( -1, 1 );
				glPixelZoom( 1.0f, -1.0f );
				glDrawPixels( windowWidth, windowHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
				m_bGameFrameReady = false;
			}

#pragma endregion
		}
		else
		{
			//BGL_LOG("Could not get generic window!");
		}
	}

	void BGraphicsDriverGeneric::Delay( const uint32_t&& ms )
	{
		//SDL_Delay(ms);
	}

	void BGraphicsDriverGeneric::Delay( const uint32_t& ms )
	{
		//SDL_Delay(ms);
	}

	void BGraphicsDriverGeneric::PaintPixel(
		BViewport* viewportPtr,
		const BERenderSpeed renderSpeed,
		const uint32_t i,
		const uint32_t j,
		const uint32_t rgb )
	{
		if( viewportPtr == nullptr )
			return;

		BViewport& viewport = *viewportPtr;
		viewport( i, j ) = rgb;

		if( renderSpeed > BERenderSpeed::Normal )
		{
			viewport( i + 1, j ) = rgb;
			viewport( i, j + 1 ) = rgb;
			viewport( i + 1, j + 1 ) = rgb;
		}

		if( renderSpeed > BERenderSpeed::Fast )
		{
			viewport( i + 2, j ) = rgb;
			viewport( i + 3, j ) = rgb;
			viewport( i + 2, j + 1 ) = rgb;
			viewport( i + 3, j + 1 ) = rgb;

			viewport( i, j + 2 ) = rgb;
			viewport( i, j + 3 ) = rgb;
			viewport( i + 1, j + 2 ) = rgb;
			viewport( i + 1, j + 3 ) = rgb;

			viewport( i + 2, j + 2 ) = rgb;
			viewport( i + 2, j + 3 ) = rgb;
			viewport( i + 3, j + 2 ) = rgb;
			viewport( i + 3, j + 3 ) = rgb;
		}
	}

} // namespace bgl
