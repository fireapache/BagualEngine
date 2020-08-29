#pragma once

#include "Common.h"

#include "PlatformBase.h"

#include "GLFW/glfw3.h"

namespace bgl
{
	class BPlatformGeneric : public BPlatformBase
	{

	public:

		virtual std::shared_ptr<BPlatformWindow> CreateWindow() override;
		virtual std::shared_ptr<BPlatformWindow> CreateWindow(const FWindowSettings& settings) override;
	};

	class BGenericPlatformWindow : public BPlatformWindow
	{

	private:

		GLFWwindow* glfwWindow = nullptr;

	protected:

		virtual void ApplySettings() override;

		virtual void Create() override;

		virtual void Destroy() override;

	public:

		BGenericPlatformWindow() = delete;
		BGenericPlatformWindow(const FWindowSettings& windowSettings);
		~BGenericPlatformWindow();

		virtual bool Tick() override;

		GLFWwindow* GetGLFW_Window();

	};
}