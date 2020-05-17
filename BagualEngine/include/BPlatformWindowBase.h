#pragma once

#include "BPlatformWindowInterface.h"

namespace Bagual::Platform
{
	class BPlatformWindowBase : BPlatformWindowInterface
	{
		

	protected:

		FWindowSettings settings;

		virtual void ApplyWindowSettings();

		virtual void InitializeWindow();

		virtual void FinalizeWindow();

	public:

		BPlatformWindowBase() { }
		BPlatformWindowBase(const FWindowSettings& windowSettings);

		virtual FWindowSettings GetWindowSettings() override;

		virtual void SetWindowSettings(const FWindowSettings& newSettings) override;
	};
}