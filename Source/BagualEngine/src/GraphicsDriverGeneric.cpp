
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

	BVector3<float> BGraphicsDriverGeneric::camOrig = BVec3f(0.f, 1.640f, -4.f);
	BVector3<float> BGraphicsDriverGeneric::camRot = BVec3f(0.f, 0.f, 0.f);
	double BGraphicsDriverGeneric::minZ = 200.0;
	double BGraphicsDriverGeneric::maxZ = 900.0;
	int32 BGraphicsDriverGeneric::i = 0;
	int32 BGraphicsDriverGeneric::j = 0;
	BViewport* BGraphicsDriverGeneric::cachedViewport = nullptr;
	BVector2<float> BGraphicsDriverGeneric::sensorSize = BVec3f(24.f, 36.f);
	BCamera* BGraphicsDriverGeneric::cachedCamera = nullptr;
	BERenderSpeed BGraphicsDriverGeneric::RenderSpeed = BERenderSpeed::VeryFast;

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

			auto guiTick = []()
			{
				IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

				ImGuiViewport* main_viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkPos().x + 650, main_viewport->GetWorkPos().y + 20), ImGuiCond_FirstUseEver);
				ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

				ImGuiWindowFlags window_flags = 0;
				if (!ImGui::Begin("Bagual Game", nullptr, window_flags))
				{
					ImGui::End();
					return;
				}

				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

				if (ImGui::Button("Restart Rendering"))
				{
#define RENDERSPEED (RenderSpeed == BERenderSpeed::Normal ? 1 : (int32)RenderSpeed * 2)

					i = 0; j = 0;

					if (cachedViewport)
					{
						cachedViewport->ResetPixelDepth();
						cachedViewport->GetCanvas().lock()->GetColorBuffer().SetBufferValue(0);
					}
				}

				ImGui::InputFloat3("Camera Position", reinterpret_cast<float*>(&camOrig));
				const float rotRange = 20.f;
				ImGui::SliderFloat3("Camera Rotation", reinterpret_cast<float*>(&camRot), -rotRange, rotRange);
				ImGui::InputDouble("MinZ", &minZ);
				ImGui::InputDouble("MaxZ", &maxZ);
				ImGui::InputFloat2("Sensor Size", reinterpret_cast<float*>(&sensorSize));

				const char* items[] = { "Normal", "Fast", "Very Fast" };
				ImGui::Combo("Render Speed", reinterpret_cast<int*>(&RenderSpeed), items, IM_ARRAYSIZE(items));

				if (cachedCamera)
				{
					ImGui::InputFloat("Camera FOV", &cachedCamera->GetFOV_Mutable());
				}

				ImGui::End();
			};

			// Gui update procedure
			window->SetGuiTickMethod(guiTick);

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
				objl::Loader charObjLoader("./assets/basemesh/basemesh.obj");
				objl::Loader mapObjLoader("./assets/basemap/basemap.obj");

				BTriangle<float> triCache;
				objl::Vertex vert0, vert1, vert2;
				uint32 index0, index1, index2;

				/*for (size_t i = 0; i < charObjLoader.LoadedIndices.size(); i += 3)
				{
					index0 = charObjLoader.LoadedIndices[i];
					index1 = charObjLoader.LoadedIndices[i + 1];
					index2 = charObjLoader.LoadedIndices[i + 2];

					vert0 = charObjLoader.LoadedVertices[index0];
					vert1 = charObjLoader.LoadedVertices[index1];
					vert2 = charObjLoader.LoadedVertices[index2];

					triCache.v0.x = vert0.Position.X;
					triCache.v0.y = vert0.Position.Y;
					triCache.v0.z = vert0.Position.Z;
					triCache.v1.x = vert1.Position.X;
					triCache.v1.y = vert1.Position.Y;
					triCache.v1.z = vert1.Position.Z;
					triCache.v2.x = vert2.Position.X;
					triCache.v2.y = vert2.Position.Y;
					triCache.v2.z = vert2.Position.Z;

					meshTris.Add(triCache);
				}*/

				for (size_t i = 0; i < mapObjLoader.LoadedIndices.size(); i += 3)
				{
					index0 = mapObjLoader.LoadedIndices[i];
					index1 = mapObjLoader.LoadedIndices[i + 1];
					index2 = mapObjLoader.LoadedIndices[i + 2];

					vert0 = mapObjLoader.LoadedVertices[index0];
					vert1 = mapObjLoader.LoadedVertices[index1];
					vert2 = mapObjLoader.LoadedVertices[index2];

					triCache.v0.x = vert0.Position.X;
					triCache.v0.y = vert0.Position.Y;
					triCache.v0.z = vert0.Position.Z;
					triCache.v1.x = vert1.Position.X;
					triCache.v1.y = vert1.Position.Y;
					triCache.v1.z = vert1.Position.Z;
					triCache.v2.x = vert2.Position.X;
					triCache.v2.y = vert2.Position.Y;
					triCache.v2.z = vert2.Position.Z;

					meshTris.Add(triCache);
				}
			}

			auto& viewport = camera->GetViewport();

			const int32 width = (int32)viewport.GetSize().width;
			const int32 height = (int32)viewport.GetSize().height;

			viewport.ResetPixelDepth();
			cachedViewport = &viewport;
			cachedCamera = camera.get();

			for (j = 0; j < height; j += RENDERSPEED)
			{
				for (i = 0; i < width; i += RENDERSPEED)
				{
					BVector2<float> sensorArea(sensorSize.x / 10.f, sensorSize.y / 10.f);
					float biggerSensorSide = std::max(sensorArea.x, sensorArea.y);
					float sensorDistance = (biggerSensorSide / 2.f) * (2.f - std::sinf(deg2rad(camera->GetFOV() / 2.f)));
					double zRange = maxZ - minZ;

					float x = (((float)i / (float)width) - 0.5f) * sensorArea.y;
					float y = -(((float)j / (float)height) - 0.5f) * sensorArea.x;
					BVector3<float> dir(x, y, sensorDistance);
					dir.Normalize();
					dir = BQuaternion<float>::RotateAroundAxis(camRot.x, BVector3<float>(1.f, 0.f, 0.f), dir);
					dir = BQuaternion<float>::RotateAroundAxis(camRot.y, BVector3<float>(0.f, 1.f, 0.f), dir);
					dir = BQuaternion<float>::RotateAroundAxis(camRot.z, BVector3<float>(0.f, 0.f, 1.f), dir);
					float t, u, v;
					uint32 rgb = 0x000000;

					for (auto tri : meshTris)
					{
						if (RayTriangleIntersect(camOrig, dir, tri.v0, tri.v1, tri.v2, t, u, v))
						{
							char r = static_cast<char>(255 * std::clamp(u, 0.f, 1.f));
							char g = static_cast<char>(255 * std::clamp(v, 0.f, 1.f));
							char b = static_cast<char>(255 * std::clamp(1 - u - v, 0.f, 1.f));

							BVec3f surfacePoint = tri.GetPointOnSurface(u, v);
							const double depthZ = (camOrig | surfacePoint) * 100.0;

							rgb = 0x000000;

							if (viewport.GetPixelDepth(i, j) < depthZ)
							{
								viewport.SetPixelDepth(i, j, depthZ);

								const double calcA = std::clamp(depthZ - minZ, 0.0, zRange);
								const double calcB = 1 - calcA / zRange;

								const uint32 gray = static_cast<uint32>(255.0 * calcB);

								rgb = gray;
								rgb = (rgb << 8) + gray;
								rgb = (rgb << 8) + gray;

								viewport(i, j) = rgb;

								if (RenderSpeed > BERenderSpeed::Normal)
								{
									viewport(i + 1, j) = rgb;
									viewport(i, j + 1) = rgb;
									viewport(i + 1, j + 1) = rgb;
								}

								if (RenderSpeed > BERenderSpeed::Fast)
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
							/*else
							{
								rgb = r;
								rgb = (rgb << 8) + g;
								rgb = (rgb << 8) + b;
							}*/

							
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

			GLfloat windowWidth = static_cast<GLfloat>(CachedPlatformWindowPtr->GetCanvas()->GetWidth());
			GLfloat windowHeight = static_cast<GLfloat>(CachedPlatformWindowPtr->GetCanvas()->GetHeight());

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
				0, GL_RGBA, GL_UNSIGNED_BYTE, CachedPlatformWindowPtr->GetCanvas()->GetColorBuffer().GetData());
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

