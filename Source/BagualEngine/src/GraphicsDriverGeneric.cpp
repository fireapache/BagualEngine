
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

		auto cameras = BCameraManager::GetCameras();

		// Rendering each camera
		for (auto& camera : cameras)
		{
#pragma region Asserts

			if (camera == nullptr) continue;
			BGL_ASSERT(camera != nullptr && "Got null camera during render!");

			auto canvas = camera->GetViewport()->GetCanvas();

			BGL_ASSERT(canvas != nullptr && "Got null canvas during render!");

			auto window = canvas->GetWindow();

			BGL_ASSERT(window != nullptr && "Got null window during render!");

			auto genericWindow = static_cast<BGenericPlatformWindow*>(window);

			BGL_ASSERT(genericWindow != nullptr && "Got null generic window during render!");

			auto glfwWindow = genericWindow->GetGLFW_Window();

#pragma endregion

#pragma region Rendering Geometry Tasks

			auto viewport = camera->GetViewport();

			viewport->ResetPixelDepth();
			auto renderThreadMode = camera->GetRenderThreadMode();

			const auto processorCount = std::thread::hardware_concurrency() * (renderThreadMode == BERenderThreadMode::HyperThread ? 2 : 1);
			uint32 renderThreadCount = renderThreadMode == BERenderThreadMode::SingleThread ? 1 : processorCount;
			thread_pool renderThreadPool(renderThreadCount);

			for (uint32 t = 0; t < processorCount; t++)
			{
				renderThreadPool.push_task(RenderLines, camera, t);
			}

#pragma endregion

#pragma region Rendering 2D Line Tasks

			auto& lines = camera->GetLine2DBuffer();

			for (auto& line : lines)
			{
				renderThreadPool.push_task(DrawLine, camera, line);
			}

			camera->ClearLine2DBuffer();

#pragma endregion

			renderThreadPool.wait_for_tasks();

		}

	}

	void BGraphicsDriverGeneric::DrawLine(BCamera* camera, BLine<BPixelPos> line)
	{
		BDraw::DrawLine(camera, line);
	}

	void BGraphicsDriverGeneric::RenderLines(BCamera* camera, const uint32 renderThreadIndex)
	{
		auto viewport = camera->GetViewport();

		// Calculating camera sensor settings

		const uint32 width = viewport->GetSize().width;
		const uint32 height = viewport->GetSize().height;

		const auto sensorSize = camera->GetSensorSize();
		const BVector2<float> sensorArea(sensorSize.x / 10.f, sensorSize.y / 10.f);
		const float biggerSensorSide = std::max(sensorArea.x, sensorArea.y);
		const float sensorDistance = (biggerSensorSide / 2.f) * (2.f - std::sinf(deg2rad(camera->GetFOV() / 2.f)));

		// Keeping some variables stacked

		float t, u, v;
		uint32 rgb = 0x000000;
		const auto renderType = camera->GetRenderOutputType();
		const BVec3f orig = camera->GetLocation();
		const BVec3f rot = camera->GetRotation();         
		const double depthDist = camera->GetDepthDistance();
		const auto renderThreadMode = camera->GetRenderThreadMode();
		const auto renderSpeed = camera->GetRenderSpeed();

		// Getting render lines of interest

		const auto processorCount = std::thread::hardware_concurrency() * (renderThreadMode == BERenderThreadMode::HyperThread ? 2 : 1);
		const int32 threadCount = processorCount <= 0 ? 1 : processorCount;

		const uint32 lineRange = height / threadCount;
		const uint32 lineStart = renderThreadIndex * lineRange;
		const uint32 lineEnd = (renderThreadIndex + 1 == threadCount) ? height : (renderThreadIndex + 1) * lineRange;

		// Starting line rendering

		const uint32 renderSpeedStep = (renderSpeed == BERenderSpeed::Normal ? 1 : (uint32)renderSpeed * 2);

		for (uint32 j = lineStart; j < lineEnd; j += renderSpeedStep)
		{
			for (uint32 i = 0; i < width; i += renderSpeedStep)
			{
				// Getting ray rotation

				const float x = (((float)i / (float)width) - 0.5f) * sensorArea.y;
				const float y = -(((float)j / (float)height) - 0.5f) * sensorArea.x;
				BVector3<float> dir(x, y, sensorDistance);
				dir.Normalize();
				dir = BQuaternion<float>::RotateAroundAxis(rot.x, BVector3<float>(1.f, 0.f, 0.f), dir);
				dir = BQuaternion<float>::RotateAroundAxis(rot.y, BVector3<float>(0.f, 1.f, 0.f), dir);
				dir = BQuaternion<float>::RotateAroundAxis(rot.z, BVector3<float>(0.f, 0.f, 1.f), dir);

				// Getting scene triangles

				auto meshComponentTris = BEngine::Scene().GetMeshComponentTriangles();

				for (auto objTris : meshComponentTris)
				{
					for (auto tri : *objTris)
					{
						if (BDraw::RayTriangleIntersect(orig, dir, tri.v0, tri.v1, tri.v2, t, u, v))
						{
							BVec3f surfacePoint = tri.GetPointOnSurface(u, v);
							const double depthZ = (orig | surfacePoint) * 100.0;
							const double currentDepthZ = viewport->GetPixelDepth(i, j);

							rgb = 0x000000;

							if (depthZ < currentDepthZ)
							{
								viewport->SetPixelDepth(i, j, depthZ);

#pragma region Depth Shader
								if (renderType == BERenderOutputType::Depth)
								{
									const double calcA = depthDist - depthZ;
									const double calcB = std::fmax(calcA, 0.0);
									const double calcC = calcB / depthDist;

									const uint32 gray = static_cast<uint32>(255.0 * calcC);

									rgb = gray;
									rgb = (rgb << 8) + gray;
									rgb = (rgb << 8) + gray;

									PaintPixel(viewport, renderSpeed, i, j, rgb);
								}
#pragma endregion

#pragma region UV Coloring Shader
								else if (renderType == BERenderOutputType::UvColor)
								{
									const char r = static_cast<char>(255 * std::clamp(u, 0.f, 1.f));
									const char g = static_cast<char>(255 * std::clamp(v, 0.f, 1.f));
									const char b = static_cast<char>(255 * std::clamp(1 - u - v, 0.f, 1.f));

									rgb = r;
									rgb = (rgb << 8) + g;
									rgb = (rgb << 8) + b;

									PaintPixel(viewport, renderSpeed, i, j, rgb);
								}
#pragma endregion
							}

						}
					}
				}
			}
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

