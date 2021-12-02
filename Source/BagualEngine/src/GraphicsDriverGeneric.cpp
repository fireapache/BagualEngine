
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

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <obj_parse.h>

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

		RenderGameFrameThread = std::thread([this]()
			{
				while (true)
				{
					RenderGameFrame();
				}
			});

	}

	BGraphicsDriverGeneric::~BGraphicsDriverGeneric()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
	}

	void BGraphicsDriverGeneric::SwapFrames()
	{
		auto& platform = Engine::Platform();
		auto& windows = platform->GetWindows();

		for (auto& window : windows)
		{
			CachedPlatformWindowPtr = static_cast<BGenericPlatformWindow*>(window.get());

			if (CachedPlatformWindowPtr)
			{
				auto glfwWindow = CachedPlatformWindowPtr->GetGLFW_Window();
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
			if (camera == nullptr) continue;
			BGL_ASSERT(camera != nullptr && "Got null camera during render!");

			auto canvas = camera->GetViewport().GetCanvas().lock();

			BGL_ASSERT(canvas != nullptr && "Got null canvas during render!");

			auto window = canvas->GetWindow();

			BGL_ASSERT(window != nullptr && "Got null window during render!");

			auto genericWindow = static_cast<BGenericPlatformWindow*>(window);

			BGL_ASSERT(genericWindow != nullptr && "Got null generic window during render!");

			auto glfwWindow = genericWindow->GetGLFW_Window();

			// Rendering queued 2D lines
			/*auto lines = camera->GetLine2DBuffer();

			std::vector<std::thread> DrawLineThreads;

			for (auto& line : lines)
			{
				DrawLineThreads.push_back(std::thread(
				[&]()
				{
					DrawLine(*camera.get(), line);
				}));
			}

			for (auto& thread : DrawLineThreads)
			{
				thread.join();
			}

			DrawLineThreads.clear();

			camera->ClearLine2DBuffer();*/

			//continue;

			const uint32 tn = 2;

			BTriangle<float> tris[tn];

			tris[0].v0 = BVec3f(-1.f, -1.f, -5.f);
			tris[0].v1 = BVec3f(1.f, -1.f, -5.f);
			tris[0].v2 = BVec3f(0.f, 1.f, -5.f);

			tris[1].v0 = BVec3f(-4.f, -1.f, -6.f);
			tris[1].v1 = BVec3f(-2.f, -1.f, -5.f);
			tris[1].v2 = BVec3f(-3.f, 1.f, -5.f);

			static BArray<BTriangle<float>> meshTris;

			if (meshTris.Size() <= 0)
			{
				objl::Loader objLoader("./assets/basemesh/basemesh.obj");

				if (objLoader.LoadedIndices.size() > 0)
				{
					BTriangle<float> triCache;
					objl::Vertex vert0, vert1, vert2;
					uint32 index0, index1, index2;
					const BVec3f translation(0.f, -2.f, -2.f);

					for (size_t i = 0; i < objLoader.LoadedIndices.size(); i += 3)
					{
						index0 = objLoader.LoadedIndices[i];
						index1 = objLoader.LoadedIndices[i + 1];
						index2 = objLoader.LoadedIndices[i + 2];

						vert0 = objLoader.LoadedVertices[index0];
						vert1 = objLoader.LoadedVertices[index1];
						vert2 = objLoader.LoadedVertices[index2];

						triCache.v0.x = vert0.Position.X + translation.x;
						triCache.v0.y = vert0.Position.Y + translation.y;
						triCache.v0.z = vert0.Position.Z + translation.z;
						triCache.v1.x = vert1.Position.X + translation.x;
						triCache.v1.y = vert1.Position.Y + translation.y;
						triCache.v1.z = vert1.Position.Z + translation.z;
						triCache.v2.x = vert2.Position.X + translation.x;
						triCache.v2.y = vert2.Position.Y + translation.y;
						triCache.v2.z = vert2.Position.Z + translation.z;

						meshTris.Add(triCache);
					}
				}
			}

			auto& viewport = camera->GetViewport();

			const auto width = viewport.GetSize().width;
			const auto height = viewport.GetSize().height;

			float scale = static_cast<float>(tan(deg2rad(camera->GetFOV() * 0.5)));
			float imageAspectRatio = width / (float)height;
			BVector3<float> orig(0.f, 0.f, 0.f);

			for (uint32 j = 0; j < height; ++j)
			{
				for (uint32 i = 0; i < width; ++i)
				{
					float x = (2 * (i + 0.5f) / (float)width - 1) * imageAspectRatio * scale;
					float y = (1 - 2 * (j + 0.5f) / (float)height) * scale;
					BVector3<float> dir(x, y, -1);
					dir.Normalize();
					float t, u, v;

					for (auto tri : meshTris)
					{
						if (RayTriangleIntersect(orig, dir, tri.v0, tri.v1, tri.v2, t, u, v))
						{
							char r = static_cast<char>(255 * std::clamp(u, 0.f, 1.f));
							char g = static_cast<char>(255 * std::clamp(v, 0.f, 1.f));
							char b = static_cast<char>(255 * std::clamp(1 - u - v, 0.f, 1.f));

							uint32 rgb = r;
							rgb = (rgb << 8) + g;
							rgb = (rgb << 8) + b;

							viewport(i, j) = rgb;
						}
					}
				}
			}

		}

		bGameFrameReady = true;

	}

	void BGraphicsDriverGeneric::SwapUIFrame()
	{
		if (CachedPlatformWindowPtr)
		{

#pragma region Rendering ImGui

			auto glfwWindow = CachedPlatformWindowPtr->GetGLFW_Window();

			// Executing GUI tick method if assigned
			{
				auto GuiTickPtr = CachedPlatformWindowPtr->GetGuiTickMethod();

				if (GuiTickPtr)
				{
					// Start the Dear ImGui frame
					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();

					// User code
					GuiTickPtr();

					ImGui::Render();
					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

					auto plat = static_cast<BPlatformGeneric*>(Engine::Instance().Platform().get());
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
		if (CachedPlatformWindowPtr)
		{

#pragma region Rendering Window Canvas to OpenGL

			auto glfwWindow = CachedPlatformWindowPtr->GetGLFW_Window();
			glfwMakeContextCurrent(glfwWindow);
			glClear(GL_COLOR_BUFFER_BIT);

			const GLfloat windowWidth = static_cast<GLfloat>(CachedPlatformWindowPtr->GetCanvas()->GetWidth());
			const GLfloat windowHeight = static_cast<GLfloat>(CachedPlatformWindowPtr->GetCanvas()->GetHeight());

			GLuint& tex = CachedPlatformWindowPtr->GetglTex();

			if (tex == -1)
			{
				glGenTextures(1, &tex);
			}

			glBindTexture(GL_TEXTURE_2D, tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				static_cast<GLsizei>(windowWidth), static_cast<GLsizei>(windowHeight),
				0, GL_RGBA, GL_UNSIGNED_BYTE, CachedPlatformWindowPtr->GetCanvas()->GetBuffer().GetData());
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

}

