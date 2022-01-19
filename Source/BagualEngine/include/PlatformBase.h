#pragma once

#include "Common.h"

#include "Canvas.h"
#include <functional>

namespace bgl
{
	static constexpr const uint32& BGL_WINDOW_CENTRALIZED = -1;

	struct FWindowSettings
	{
		uint32 x = BGL_WINDOW_CENTRALIZED;
		uint32 y = BGL_WINDOW_CENTRALIZED;
		uint32 width = 320;
		uint32 height = 240;
		std::string Title = "Bagual Engine Window";
	};

	class BPlatformWindow
	{

	public:

		typedef std::function<void()> GuiTickFuncType;

	protected:

		std::unique_ptr<BCanvas> m_canvas;

		FWindowSettings settings;

		virtual void ApplySettings();

		virtual void Create();

		virtual void Destroy();

		GuiTickFuncType m_guiTickFunc;

	public:

		BPlatformWindow() {  };
		BPlatformWindow(const FWindowSettings& windowSettings);

		virtual bool Tick() = 0;

		virtual FWindowSettings GetSettings() const;

		virtual void SetWindow(const FWindowSettings& newSettings);

		virtual BCanvas* GetCanvas();

		void SetGuiTickFunc(GuiTickFuncType func);
		GuiTickFuncType GetGuiTickFunc();

	};

	class BPlatformBase
	{


	protected:

		BArray<std::shared_ptr<BPlatformWindow>> windows;

	public:

		virtual BPlatformWindow* CreateWindow() = 0;
		virtual BPlatformWindow* CreateWindow(const FWindowSettings& settings) = 0;

		BArray<std::shared_ptr<BPlatformWindow>>& GetWindows();

	};

}