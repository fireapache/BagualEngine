
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

#include <GLFW/glfw3.h>

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
		glfwTerminate();
	}

	void BGraphicsDriverGeneric::RenderFrame()
	{
		BGraphicsDriverBase::RenderFrame();

		auto cameras = BCameraManager::GetCameras();

		// Rendering each camera
		for (auto& camera : cameras)
		{
			BGL_ASSERT(camera != nullptr && "Got null camera during render!");

			auto canvas = camera->GetViewport().GetCanvas().lock();

			BGL_ASSERT(canvas != nullptr && "Got null canvas during render!");

			auto window = canvas->GetWindow();

			BGL_ASSERT(window != nullptr && "Got null window during render!");

			auto genericWindow = static_cast<BGenericPlatformWindow*>(window);

			BGL_ASSERT(genericWindow != nullptr && "Got null generic window during render!");

			// Rendering queued 2D lines
			auto lines = camera->GetLine2DBuffer();

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

			camera->ClearLine2DBuffer();

			//continue;

			const uint32_t tn = 2;

			BTriangle<float> tris[tn];

			tris[0].v0 = BVec3f(-1.f, -1.f, -5.f);
			tris[0].v1 = BVec3f(1.f, -1.f, -5.f);
			tris[0].v2 = BVec3f(0.f, 1.f, -5.f);

			tris[1].v0 = BVec3f(-4.f, -1.f, -6.f);
			tris[1].v1 = BVec3f(-2.f, -1.f, -5.f);
			tris[1].v2 = BVec3f(-3.f, 1.f, -5.f);

			auto& viewport = camera->GetViewport();

			const auto width = viewport.GetSize().width;
			const auto height = viewport.GetSize().height;

			float scale = static_cast<float>(tan(deg2rad(camera->GetFOV() * 0.5)));
			float imageAspectRatio = width / (float)height;
			BVector3<float> orig(0.f, 0.f, 0.f);

			for (uint32_t j = 0; j < height; ++j)
			{
				for (uint32_t i = 0; i < width; ++i)
				{
					float x = (2 * (i + 0.5f) / (float)width - 1) * imageAspectRatio * scale;
					float y = (1 - 2 * (j + 0.5f) / (float)height) * scale;
					BVector3<float> dir(x, y, -1);
					dir.Normalize();
					float t, u, v;

					for (uint32_t tri = 0; tri < tn; ++tri)
					{
						if (RayTriangleIntersect(orig, dir, tris[tri].v0, tris[tri].v1, tris[tri].v2, t, u, v))
						{
							//*pix = u * cols[0] + v * cols[1] + (1 - u - v) * cols[2];
							// uncomment this line if you want to visualize the row barycentric coordinates
							// char r = (char)(255 * scratch::utils::clamp(0, 1, framebuffer[i].x));
							// char g = (char)(255 * scratch::utils::clamp(0, 1, framebuffer[i].y));
							// char b = (char)(255 * scratch::utils::clamp(0, 1, framebuffer[i].z));
							
							char r = static_cast<char>(255 * std::clamp(u, 0.f, 1.f));
							char g = static_cast<char>(255 * std::clamp(v, 0.f, 1.f));
							char b = static_cast<char>(255 * std::clamp(1 - u - v, 0.f, 1.f));

							uint rgb = r;
							// 0x0000RR

							rgb = (rgb << 8) + g;
							// 0x00RR00
							// 0x00RRGG

							rgb = (rgb << 8) + b;
							// 0xRRGG00
							// 0xRRGGBB

							//pix[i + width * j] = rgb;
							viewport(i, j) = rgb;
						}
					}
				}
			}

			
		}

		auto& platform = Engine::Platform();
		auto& windows = platform->GetWindows();

		for (auto& window : windows)
		{
			auto genericWindow = static_cast<BGenericPlatformWindow*>(window.get());

			if (genericWindow)
			{
				//SDL_UpdateWindowSurface(genericWindow->GetSDL_Window());
			}
			else
			{
				//BGL_LOG("Could not get generic window!");
			}
		}

		

	}

	void BGraphicsDriverGeneric::Delay(const uint&& ms)
	{
		//SDL_Delay(ms);
	}

	void BGraphicsDriverGeneric::Delay(const uint& ms)
	{
		//SDL_Delay(ms);
	}

}

