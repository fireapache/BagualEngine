#pragma once

#include "Common.h"
#include "PlatformBase.h"

struct ImGuiIO;
struct GLFWwindow;

namespace bgl
{
	class BPlatformGeneric : public BPlatformBase
	{
	private:
		ImGuiIO* imguiConfig;

	public:
		BPlatformGeneric();
		~BPlatformGeneric();

		virtual BPlatformWindow* CreateWindow() override;
		virtual BPlatformWindow* CreateWindow( const FWindowSettings& settings ) override;

		ImGuiIO* GetImguiConfig();
	};

	class BGenericPlatformWindow : public BPlatformWindow
	{
	private:
		GLFWwindow* glfwWindow = nullptr;

		uint32_t glTex = UINT32_MAX;

	protected:
		virtual void ApplySettings() override;

		virtual void Create() override;

		virtual void Destroy() override;

	public:
		BGenericPlatformWindow() = delete;
		BGenericPlatformWindow( const FWindowSettings& windowSettings );
		~BGenericPlatformWindow();

		virtual bool Tick() override;

		GLFWwindow* GetGLFW_Window();

		uint32_t& GetglTex();

		void SetglTex( uint32_t index );
	};
} // namespace bgl