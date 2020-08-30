
#include "Bagual.pch.h"

#include "PlatformBase.h"
#include "Assertions.h"

namespace bgl
{

	BPlatformWindow::BPlatformWindow(const FWindowSettings& windowSettings)
	{
		settings = windowSettings;
		Create();
	}

	FWindowSettings BPlatformWindow::GetSettings() const
	{
		return settings;
	}

	void BPlatformWindow::SetWindow(const FWindowSettings& newSettings)
	{
		settings = newSettings;
		ApplySettings();
	}

	const std::shared_ptr<BCanvas>& BPlatformWindow::GetCanvas()
	{
		return canvas;
	}

	void BPlatformWindow::SetGuiTickMethod(GuiTickMethod ptr)
	{
		BGL_ASSERT(ptr != nullptr && "Got null GUI_Tick_Function parameter!");
		GuiTickPtr = ptr;
	}

	bgl::BPlatformWindow::GuiTickMethod BPlatformWindow::GetGuiTickMethod()
	{
		return GuiTickPtr;
	}

	void BPlatformWindow::ApplySettings()
	{
		BGL_NEED_IMPLEMENTATION
	}

	void BPlatformWindow::Create()
	{
		BGL_NEED_IMPLEMENTATION
	}

	void BPlatformWindow::Destroy()
	{
		BGL_NEED_IMPLEMENTATION
	}

	BArray<std::shared_ptr<BPlatformWindow>>& BPlatformBase::GetWindows()
	{
		return windows;
	}

}