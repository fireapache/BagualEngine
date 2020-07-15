
#include "BPlatformWindowBase.h"
#include "BAssertions.h"

namespace bgl
{

	BPlatformWindowBase::BPlatformWindowBase(const FWindowSettings& windowSettings)
	{
		settings = windowSettings;
		InitializeWindow();
	}

	FWindowSettings BPlatformWindowBase::GetWindowSettings()
	{
		return settings;
	}

	void BPlatformWindowBase::SetWindowSettings(const FWindowSettings& newSettings)
	{
		settings = newSettings;
		ApplyWindowSettings();
	}

	void BPlatformWindowBase::ApplyWindowSettings()
	{
		BGL_NEED_IMPLEMENTATION
	}

	void BPlatformWindowBase::InitializeWindow()
	{
		BGL_NEED_IMPLEMENTATION
	}

	void BPlatformWindowBase::FinalizeWindow()
	{
		BGL_NEED_IMPLEMENTATION
	}

}