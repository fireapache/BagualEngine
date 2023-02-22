
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "GraphicsDriverGeneric.h"

#include "BagualEngine.h"
#include "Camera.h"
#include "CameraManager.h"
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
#include <obj_parse.h>
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
		auto& windows = platform.GetWindows();

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

	void BGraphicsDriverGeneric::composeFinalFrame( Color32Bit* colorPtr, Color32Bit* wireframePtr, Color32Bit* destBuffer, const Color32Bit* bufferEnd )
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
		BGraphicsDriverBase::RenderGameFrame();

		auto viewports = BEngine::GraphicsPlatform().GetViewports();

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

			auto window = canvas->GetWindow();

			BGL_ASSERT( window != nullptr && "Got null window during render!" );

			auto genericWindow = static_cast< BGenericPlatformWindow* >( window );

			BGL_ASSERT( genericWindow != nullptr && "Got null generic window during render!" );

			auto glfwWindow = genericWindow->GetGLFW_Window();

#pragma endregion

			// Getting thread pool ready
			const auto renderThreadMode = camera->GetRenderThreadMode();
			const bool bUseHyperThread = renderThreadMode == BERenderThreadMode::HyperThread;
			const auto processorCount = std::thread::hardware_concurrency() * ( bUseHyperThread ? 2 : 1 );
			uint32_t renderThreadCount = renderThreadMode == BERenderThreadMode::SingleThread ? 1 : processorCount;
			thread_pool renderThreadPool( renderThreadCount );

#pragma region Pushing Geometry Tasks

			{
				canvas->ResetZBuffer();

				for( uint32_t t = 0; t < processorCount; t++ )
				{
					renderThreadPool.push_task( RenderLines, viewport, t );
				}
			}

#pragma endregion
			
#pragma region Pushing 2D and 3D Lines Tasks

			{
				canvas->ResetWireframeBuffer();

				for( const auto meshComponent : BMeshComponent::g_meshComponents )
				{
					if( meshComponent == nullptr || meshComponent->getShowWireframe() == false )
						continue;

					const Color32Bit lineColor = meshComponent->getColor().getRGB();
					auto& edges = meshComponent->getMeshData().edges;

					for( auto* edge = edges.data(); edge < edges.data() + edges.size(); edge++ )
					{
						renderThreadPool.push_task( Draw3DLine, viewport, *edge, lineColor );
					}
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
			}

#pragma endregion

			// syncing all threads
			renderThreadPool.wait_for_tasks();

			auto& wireframeBuffer = canvas->GetWireframeBuffer();
			Color32Bit* wireframdeBufferData = wireframeBuffer.GetData();
			auto& colorBuffer = canvas->GetColorBuffer();
			Color32Bit* colorBufferData = colorBuffer.GetData();
			auto& readyFrameBuffer = canvas->GetReadyFrameBuffer();
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
	}

	void BGraphicsDriverGeneric::Draw3DLine( BViewport* viewport, const BLine< BVec3f >& line, const Color32Bit color )
	{
		BDraw::DrawLine( viewport, line, color );
	}

	void BGraphicsDriverGeneric::Draw2DLine( BViewport* viewport, const BLine< BPixelPos >& line, const Color32Bit color )
	{
		BDraw::DrawLine( viewport, line, color );
	}

	void BGraphicsDriverGeneric::RenderLines( BViewport* viewport, const uint32_t renderThreadIndex )
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

		// Calculating camera sensor settings

		const uint32_t width = viewport->GetSize().width;
		const uint32_t height = viewport->GetSize().height;

		const auto sensorArea = camera->GetSensorArea();
		const float sensorDistance = camera->GetSensorDistance();

		// Keeping some variables stacked

		BFTriangleScanParams triangleScanParams;
		triangleScanParams.renderType = camera->GetRenderOutputType();
		triangleScanParams.orig = camera->GetLocation();
		triangleScanParams.depthDist = camera->GetDepthDistance();
		triangleScanParams.renderSpeed = camera->GetRenderSpeed();
		triangleScanParams.viewport = viewport;

		const auto renderThreadMode = camera->GetRenderThreadMode();
		const BRotf rRot = camera->GetRotation();
		const auto intrinsicsMode = camera->GetIntrinsicsMode();

		// Getting render lines of interest

		const auto processorCount
			= std::thread::hardware_concurrency() * ( renderThreadMode == BERenderThreadMode::HyperThread ? 2 : 1 );
		const uint32_t threadCount = processorCount <= 0 ? 1 : processorCount;

		const uint32_t lineRange = height / threadCount;
		const uint32_t lineStart = renderThreadIndex * lineRange;
		const uint32_t lineEnd = ( renderThreadIndex + 1 == threadCount ) ? height : ( renderThreadIndex + 1 ) * lineRange;

		// Starting line rendering

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

				BVec3f vRayDir( sensorArea.x * unitX, -sensorArea.y * unitY, sensorDistance );
				vRayDir.Normalize();

				BVec3f vUp( BVec3f::up() );
				BVec3f vRight( BVec3f::right() );
				BVec3f vForward( BVec3f::forward() );

				// yaw
				vRayDir = BQuatf::rotateVector( rRot.y, vUp, vRayDir );

				// pitch
				vRight = BQuatf::rotateVector( rRot.y, vUp, vRight );
				vRayDir = BQuatf::rotateVector( rRot.p, vRight, vRayDir );

				// roll
				vForward = BQuatf::rotateVector( rRot.y, BVec3f::up(), vForward );
				vForward = BQuatf::rotateVector( rRot.p, BVec3f::right(), vForward );
				vRayDir = BQuatf::rotateVector( rRot.r, vForward, vRayDir );

				triangleScanParams.dir = vRayDir;

				// Getting scene triangles

				auto meshComponents = BEngine::Scene().GetMeshComponents();

				for( const auto meshComp : meshComponents )
				{
					// Skipping not visible components
					if( meshComp->IsVisible() == false )
					{
						continue;
					}

					if( intrinsicsMode == BEIntrinsicsMode::Off )
					{
						auto& compTris = meshComp->GetTriangles();
						ScanTriangles_Sequential( compTris, triangleScanParams );
					}
					else if( intrinsicsMode == BEIntrinsicsMode::AVX )
					{
						auto& compTris = meshComp->GetTriangles_SIMD();
						ScanTriangles_SIMD( compTris, triangleScanParams );
					}
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

	inline void BGraphicsDriverGeneric::ScanTriangles_Sequential(
		BArray< BTriangle< float > >& compTris,
		BFTriangleScanParams& p )
	{
		// Local copy of ray tracing parameters
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

	inline void BGraphicsDriverGeneric::ScanTriangles_SIMD( BTriangle< BArray< float > >& compTris, BFTriangleScanParams& p )
	{
		const size_t triCount = compTris.v0.x.Size();
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
		if( m_cachedPlatformWindowPtr )
		{
#pragma region Rendering ImGui

			auto glfwWindow = m_cachedPlatformWindowPtr->GetGLFW_Window();

			// Executing GUI tick func if assigned
			{
				auto guiTickFunc = m_cachedPlatformWindowPtr->GetGuiTickFunc();

				if( guiTickFunc )
				{
					// Start the Dear ImGui frame
					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();

					// User code
					//GuiTickPtr();
					guiTickFunc();

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

#pragma endregion
		}
		else
		{
			//BGL_LOG("Could not get generic window!");
		}
	}

	void BGraphicsDriverGeneric::SwapGameFrame()
	{
		if( m_cachedPlatformWindowPtr )
		{
#pragma region Rendering Window Canvas to OpenGL

			auto glfwWindow = m_cachedPlatformWindowPtr->GetGLFW_Window();
			glfwMakeContextCurrent( glfwWindow );
			glClear( GL_COLOR_BUFFER_BIT );

			const GLfloat windowWidth = m_cachedPlatformWindowPtr->GetCanvas()->GetWidth();
			const GLfloat windowHeight = m_cachedPlatformWindowPtr->GetCanvas()->GetHeight();

			GLuint& tex = m_cachedPlatformWindowPtr->GetglTex();

			if( !glIsTexture( tex ) )
			{
				glGenTextures( 1, &tex );
				m_cachedPlatformWindowPtr->SetglTex( tex );
			}

			if( m_bGameFrameReady )
			{
				glBindTexture( GL_TEXTURE_2D, tex );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RGBA,
					static_cast< GLsizei >( windowWidth ),
					static_cast< GLsizei >( windowHeight ),
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					m_cachedPlatformWindowPtr->GetCanvas()->GetReadyFrameBuffer().GetData() );
				glBindTexture( GL_TEXTURE_2D, 0 );
				m_bGameFrameReady = false;
			}

			// Match projection to window resolution
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			glLoadIdentity();
			glOrtho( 0, windowWidth, 0, windowHeight, -1, 1 );
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();

			// Clear and draw quad with texture
			glClear( GL_COLOR_BUFFER_BIT );
			glBindTexture( GL_TEXTURE_2D, tex );
			glEnable( GL_TEXTURE_2D );
			glBegin( GL_QUADS );
			glTexCoord2f( 1, 1 );
			glVertex3f( windowWidth, 0, 0 );
			glTexCoord2f( 1, 0 );
			glVertex3f( windowWidth, windowHeight, 0 );
			glTexCoord2f( 0, 0 );
			glVertex3f( 0, windowHeight, 0 );
			glTexCoord2f( 0, 1 );
			glVertex3f( 0, 0, 0 );
			glEnd();
			glDisable( GL_TEXTURE_2D );
			glBindTexture( GL_TEXTURE_2D, 0 );
			glFlush();

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
