
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

	BCanvas* BPlatformWindow::GetCanvas()
	{
		return m_canvas.get();
	}

	void BPlatformWindow::SetGuiTickFunc(GuiTickFuncType func)
	{
		m_guiTickFunc = func;
	}

	BPlatformWindow::GuiTickFuncType BPlatformWindow::GetGuiTickFunc()
	{
		return m_guiTickFunc;
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