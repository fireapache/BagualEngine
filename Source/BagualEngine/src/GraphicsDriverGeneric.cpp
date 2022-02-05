
#include "Bagual.pch.h"

#include <thread>

#include "GraphicsDriverGeneric.h"
#include "Camera.h"
#include "CameraManager.h"
#include "Settings.h"
#include "Draw.h"
#include "Viewport.h"
#include "PlatformGeneric.h"
#include "BagualEngine.h"
#include "ThreadPool.h"
#include "Scene.h"
#include "GraphicsPlatform.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <obj_parse.h>
#include <thread>

namespace bgl
{
	void ErrorCallback(int error, const char* description)
	{
		BGL_LOG(description);
	}

	BGraphicsDriverGeneric::BGraphicsDriverGeneric()
	{
		BGL_ASSERT(glfwInit() && "Could not start GLFW!");
		glfwSetErrorCallback(ErrorCallback);
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

		for (auto& window : windows)
		{
			m_cachedPlatformWindowPtr = static_cast<BGenericPlatformWindow*>(window.get());

			if (m_cachedPlatformWindowPtr)
			{
				auto glfwWindow = m_cachedPlatformWindowPtr->GetGLFW_Window();
				glfwMakeContextCurrent(glfwWindow);
				glClear(GL_COLOR_BUFFER_BIT);
				BGraphicsDriverBase::SwapFrames();
				glfwSwapBuffers(glfwWindow);
			}
		}
	}

	void BGraphicsDriverGeneric::RenderGameFrame()
	{
		BGraphicsDriverBase::RenderGameFrame();

		auto viewports = BEngine::GraphicsPlatform().GetViewports();

		// Rendering each viewport
		for (auto viewport : viewports)
		{
#pragma region Asserts

			if (viewport == nullptr)
			{
				BGL_LOG("Got null viewport during render!");
				continue;
			}

			auto camera = viewport->GetCamera();

			if (camera == nullptr)
			{
				BGL_LOG("Got null camera during render!");
				continue;
			}

			auto canvas = viewport->GetCanvas();

			BGL_ASSERT(canvas != nullptr && "Got null canvas during render!");

			auto window = canvas->GetWindow();

			BGL_ASSERT(window != nullptr && "Got null window during render!");

			auto genericWindow = static_cast<BGenericPlatformWindow*>(window);

			BGL_ASSERT(genericWindow != nullptr && "Got null generic window during render!");

			auto glfwWindow = genericWindow->GetGLFW_Window();

#pragma endregion

#pragma region Rendering Geometry Tasks

			viewport->ResetPixelDepth();
			auto renderThreadMode = camera->GetRenderThreadMode();

			const auto processorCount = std::thread::hardware_concurrency() * (renderThreadMode == BERenderThreadMode::HyperThread ? 2 : 1);
			uint32 renderThreadCount = renderThreadMode == BERenderThreadMode::SingleThread ? 1 : processorCount;
			thread_pool renderThreadPool(renderThreadCount);

			for (uint32 t = 0; t < processorCount; t++)
			{
				renderThreadPool.push_task(RenderLines, viewport, t);
			}

#pragma endregion

#pragma region Rendering 2D Line Tasks

			auto& lines = camera->GetLine2DBuffer();

			for (auto& line : lines)
			{
				renderThreadPool.push_task(DrawLine, viewport, line);
			}

			camera->ClearLine2DBuffer();

#pragma endregion

			renderThreadPool.wait_for_tasks();

		}

	}

	void BGraphicsDriverGeneric::DrawLine(BViewport* viewport, BLine<BPixelPos> line)
	{
		BDraw::DrawLine(viewport, line);
	}

	void BGraphicsDriverGeneric::RenderLines(BViewport* viewport, const uint32 renderThreadIndex)
	{
		if (viewport == nullptr)
		{
			BGL_LOG("Got null viewport when rendering lines!");
			return;
		}

		const auto camera = viewport->GetCamera();

		if (camera == nullptr)
		{
			BGL_LOG("Got null camera when rendering lines!");
			return;
		}

		// Calculating camera sensor settings

		const uint32 width = viewport->GetSize().width;
		const uint32 height = viewport->GetSize().height;

		const auto sensorSize = camera->GetSensorSize();
		const BVector2<float> sensorArea(sensorSize.x / 10.f, sensorSize.y / 10.f);
		const float biggerSensorSide = std::max(sensorArea.x, sensorArea.y);
		const float sensorDistance = (biggerSensorSide / 2.f) * (2.f - std::sinf(deg2rad(camera->GetFOV() / 2.f)));

		// Keeping some variables stacked

		BFTriangleScanParams triangleScanParams;
		triangleScanParams.renderType = camera->GetRenderOutputType();
		triangleScanParams.orig = camera->GetLocation();    
		triangleScanParams.depthDist = camera->GetDepthDistance();
		triangleScanParams.renderSpeed = camera->GetRenderSpeed();
		triangleScanParams.viewport = viewport;

		const auto renderThreadMode = camera->GetRenderThreadMode();
		const BVec3f rot = camera->GetRotation();
		const auto intrinsicsMode = camera->GetIntrinsicsMode();

		// Getting render lines of interest

		const auto processorCount = 
			std::thread::hardware_concurrency() * (renderThreadMode == BERenderThreadMode::HyperThread ? 2 : 1);
		const int32 threadCount = processorCount <= 0 ? 1 : processorCount;

		const uint32 lineRange = height / threadCount;
		const uint32 lineStart = renderThreadIndex * lineRange;
		const uint32 lineEnd = (renderThreadIndex + 1 == threadCount) ? height : (renderThreadIndex + 1) * lineRange;

		// Starting line rendering

		const uint32 renderSpeedStep = 
			(triangleScanParams.renderSpeed == BERenderSpeed::Normal ? 1 : (uint32)triangleScanParams.renderSpeed * 2);

		for (uint32 j = lineStart; j < lineEnd; j += renderSpeedStep)
		{
			for (uint32 i = 0; i < width; i += renderSpeedStep)
			{
				triangleScanParams.px = i;
				triangleScanParams.py = j;

				// Getting ray rotation

				const float x = (((float)i / (float)width) - 0.5f) * sensorArea.y;
				const float y = -(((float)j / (float)height) - 0.5f) * sensorArea.x;
				BVec3f& dir = triangleScanParams.dir;
				dir = BVec3f(x, y, sensorDistance);
				dir.Normalize();
				dir = BQuaternion<float>::RotateAroundAxis(rot.x, BVector3<float>(1.f, 0.f, 0.f), dir);
				dir = BQuaternion<float>::RotateAroundAxis(rot.y, BVector3<float>(0.f, 1.f, 0.f), dir);
				dir = BQuaternion<float>::RotateAroundAxis(rot.z, BVector3<float>(0.f, 0.f, 1.f), dir);

				// Getting scene triangles

				auto meshComponents = BEngine::Scene().GetMeshComponents();

				for (auto meshComp : meshComponents)
				{
					// Skipping not visible components
					if (meshComp->IsVisible() == false)
					{
						continue;
					}

					if (intrinsicsMode == BEIntrinsicsMode::Off)
					{
						auto& compTris = meshComp->GetTriangles();
						ScanTriangles_Sequential(compTris, triangleScanParams);
					}
					else if (intrinsicsMode == BEIntrinsicsMode::AVX)
					{
						auto& compTris = meshComp->GetTriangles_SIMD();
						ScanTriangles_SIMD(compTris, triangleScanParams);
					}
				}
			}
		}
	}

	inline void BGraphicsDriverGeneric::ScanTriangles_Sequential(BArray<BTriangle<float>>& compTris, BFTriangleScanParams& p)
	{
		for (auto tri : compTris)
		{
			if (BDraw::RayTriangleIntersect(p.orig, p.dir, tri, p.t, p.u, p.v))
			{
				const double depthZ = p.t * 100.0;
				const double currentDepthZ = p.viewport->GetPixelDepth(p.px, p.py);

				p.rgb = 0x000000;

				if (depthZ < currentDepthZ)
				{
					p.viewport->SetPixelDepth(p.px, p.py, depthZ);

#pragma region Depth Shader
					if (p.renderType == BERenderOutputType::Depth)
					{
						const double calcA = p.depthDist - depthZ;
						const double calcB = std::fmax(calcA, 0.0);
						const double calcC = calcB / p.depthDist;

						const uint32 gray = static_cast<uint32>(255.0 * calcC);

						p.rgb = gray;
						p.rgb = (p.rgb << 8) + gray;
						p.rgb = (p.rgb << 8) + gray;

						PaintPixel(p.viewport, p.renderSpeed, p.px, p.py, p.rgb);
					}
#pragma endregion

#pragma region UV Coloring Shader
					else if (p.renderType == BERenderOutputType::UvColor)
					{
						const char r = static_cast<char>(255 * std::clamp(p.u, 0.f, 1.f));
						const char g = static_cast<char>(255 * std::clamp(p.v, 0.f, 1.f));
						const char b = static_cast<char>(255 * std::clamp(1 - p.u - p.v, 0.f, 1.f));

						p.rgb = r;
						p.rgb = (p.rgb << 8) + g;
						p.rgb = (p.rgb << 8) + b;

						PaintPixel(p.viewport, p.renderSpeed, p.px, p.py, p.rgb);
					}
#pragma endregion
				}

			}
		}
	}

	inline void BGraphicsDriverGeneric::ScanTriangles_SIMD(BTriangle<BArray<float>>& compTris, BFTriangleScanParams& p)
	{
		const size_t trisNum = compTris.v0.x.Size();
		const size_t toKeep = trisNum % 8;

		// Stacking data and variables

		BTriangle<float*> triData;
		triData.v0.x = compTris.v0.x.data();
		triData.v0.y = compTris.v0.y.data();
		triData.v0.z = compTris.v0.z.data();

		triData.v1.x = compTris.v1.x.data();
		triData.v1.y = compTris.v1.y.data();
		triData.v1.z = compTris.v1.z.data();

		triData.v2.x = compTris.v2.x.data();
		triData.v2.y = compTris.v2.y.data();
		triData.v2.z = compTris.v2.z.data();

		BTriangle<__m256> tri;
		BVector3<__m256> orig, dir, v0v1, v0v2, pvec;
		BVector3<__m256> tvec, qvec;
		__m256 u, v, t, uv, culling, det, invDet, uvgrt1;
		__m256 fail1, fail2, uless0, ugrt1, vless0, validHit;

		// Core loop

		for (size_t i = 0; i < trisNum; i += 8)
		{
			tri.v0.x = _mm256_load_ps(triData.v0.x + i);
			tri.v0.y = _mm256_load_ps(triData.v0.y + i);
			tri.v0.z = _mm256_load_ps(triData.v0.z + i);

			tri.v1.x = _mm256_load_ps(triData.v1.x + i);
			tri.v1.y = _mm256_load_ps(triData.v1.y + i);
			tri.v1.z = _mm256_load_ps(triData.v1.z + i);

			tri.v2.x = _mm256_load_ps(triData.v2.x + i);
			tri.v2.y = _mm256_load_ps(triData.v2.y + i);
			tri.v2.z = _mm256_load_ps(triData.v2.z + i);

			orig.x = _mm256_set1_ps(p.orig.x);
			orig.y = _mm256_set1_ps(p.orig.y);
			orig.z = _mm256_set1_ps(p.orig.z);

			dir.x = _mm256_set1_ps(p.dir.x);
			dir.y = _mm256_set1_ps(p.dir.y);
			dir.z = _mm256_set1_ps(p.dir.z);

			// ==================================================================
			// === starting vectorization of BDraw::RayTriangleIntersect(...) ===
			// ==================================================================

			v0v1 = tri.v1 - tri.v0;
			v0v2 = tri.v2 - tri.v0;
			pvec = CrossProduct(dir, v0v2);
			det = DotProduct(v0v1, pvec);

			// Checking if triangle is backfacing (true means it should be discarded)
			culling = _mm256_cmp_ps(det, _mm256_set1_ps(kEpsilon), 1);

			// invDet = 1 / det;
			invDet = _mm256_div_ps(_mm256_set1_ps(1.f), det);

			tvec = orig - tri.v0;
			u = DotProduct(tvec, pvec) * invDet;

			// if (u < 0 || u > 1) return false;
			uless0 = _mm256_cmp_ps(u, _mm256_set1_ps(0.f), 1);
			ugrt1 = _mm256_cmp_ps(u, _mm256_set1_ps(1.f), 14);
			fail1 = _mm256_or_ps(uless0, ugrt1);

			qvec = CrossProduct(tvec, v0v1);
			v = DotProduct(dir, qvec) * invDet;

			// if (v < 0 || u + v > 1) return false;
			vless0 = _mm256_cmp_ps(v, _mm256_set1_ps(0.f), 1);
			uv = u + v;
			uvgrt1 = _mm256_cmp_ps(uv, _mm256_set1_ps(1.f), 14);
			fail2 = _mm256_or_ps(vless0, uvgrt1);
			
			t = DotProduct(v0v2, qvec) * invDet;

			validHit = _mm256_or_ps(_mm256_or_ps(culling, fail1), fail2);

		}

	}

	void BGraphicsDriverGeneric::SwapUIFrame()
	{
		if (m_cachedPlatformWindowPtr)
		{

#pragma region Rendering ImGui

			auto glfwWindow = m_cachedPlatformWindowPtr->GetGLFW_Window();

			// Executing GUI tick func if assigned
			{
				auto guiTickFunc = m_cachedPlatformWindowPtr->GetGuiTickFunc();

				if (guiTickFunc)
				{
					// Start the Dear ImGui frame
					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();

					// User code
					//GuiTickPtr();
					guiTickFunc();

					ImGui::Render();
					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

					auto plat = static_cast<BPlatformGeneric*>(&BEngine::Instance().Platform());
					ImGuiIO& io = *plat->GetImguiConfig();

					// Update and Render additional Platform Windows
					// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
					//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
					if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
					{
						GLFWwindow* backup_current_context = glfwGetCurrentContext();
						ImGui::UpdatePlatformWindows();
						ImGui::RenderPlatformWindowsDefault();
						glfwMakeContextCurrent(backup_current_context);
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
		if (m_cachedPlatformWindowPtr)
		{

#pragma region Rendering Window Canvas to OpenGL

			auto glfwWindow = m_cachedPlatformWindowPtr->GetGLFW_Window();
			glfwMakeContextCurrent(glfwWindow);
			glClear(GL_COLOR_BUFFER_BIT);

			GLfloat windowWidth = static_cast<GLfloat>(m_cachedPlatformWindowPtr->GetCanvas()->GetWidth());
			GLfloat windowHeight = static_cast<GLfloat>(m_cachedPlatformWindowPtr->GetCanvas()->GetHeight());

			GLuint& tex = m_cachedPlatformWindowPtr->GetglTex();

			if (tex == -1)
			{
				glGenTextures(1, &tex);
			}

			glBindTexture(GL_TEXTURE_2D, tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				static_cast<GLsizei>(windowWidth), static_cast<GLsizei>(windowHeight),
				0, GL_RGBA, GL_UNSIGNED_BYTE, m_cachedPlatformWindowPtr->GetCanvas()->GetColorBuffer().GetData());
			glBindTexture(GL_TEXTURE_2D, 0);

			// Match projection to window resolution
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0, windowWidth, 0, windowHeight, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

			// Clear and draw quad with texture
			glClear(GL_COLOR_BUFFER_BIT);
			glBindTexture(GL_TEXTURE_2D, tex);
			glEnable(GL_TEXTURE_2D);
			glBegin(GL_QUADS);
			glTexCoord2f(1, 1); glVertex3f(windowWidth, 0, 0);
			glTexCoord2f(1, 0); glVertex3f(windowWidth, windowHeight, 0);
			glTexCoord2f(0, 0); glVertex3f(0, windowHeight, 0);
			glTexCoord2f(0, 1); glVertex3f(0, 0, 0);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFlush();

#pragma endregion

		}
		else
		{
			//BGL_LOG("Could not get generic window!");
		}
	}

	void BGraphicsDriverGeneric::Delay(const uint32&& ms)
	{
		//SDL_Delay(ms);
	}

	void BGraphicsDriverGeneric::Delay(const uint32& ms)
	{
		//SDL_Delay(ms);
	}

	void BGraphicsDriverGeneric::PaintPixel(BViewport* viewportPtr, BERenderSpeed renderSpeed, uint32 i, uint32 j, uint32 rgb)
	{
		if (viewportPtr == nullptr) return;

		BViewport& viewport = *viewportPtr;
		viewport(i, j) = rgb;

		if (renderSpeed > BERenderSpeed::Normal)
		{
			viewport(i + 1, j) = rgb;
			viewport(i, j + 1) = rgb;
			viewport(i + 1, j + 1) = rgb;
		}

		if (renderSpeed > BERenderSpeed::Fast)
		{
			viewport(i + 2, j) = rgb;
			viewport(i + 3, j) = rgb;
			viewport(i + 2, j + 1) = rgb;
			viewport(i + 3, j + 1) = rgb;

			viewport(i, j + 2) = rgb;
			viewport(i, j + 3) = rgb;
			viewport(i + 1, j + 2) = rgb;
			viewport(i + 1, j + 3) = rgb;

			viewport(i + 2, j + 2) = rgb;
			viewport(i + 2, j + 3) = rgb;
			viewport(i + 3, j + 2) = rgb;
			viewport(i + 3, j + 3) = rgb;
		}
	}

}

