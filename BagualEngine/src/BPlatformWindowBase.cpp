
#include "BPlatformWindowBase.h"
#include "BAssertions.h"

namespace Bagual::Platform
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
		NEED_IMPLEMENTATION
	}

	void BPlatformWindowBase::InitializeWindow()
	{
		NEED_IMPLEMENTATION
	}

	void BPlatformWindowBase::FinalizeWindow()
	{
		NEED_IMPLEMENTATION
	}

}