#pragma once

#include "Common.h"

#include "Singleton.h"

namespace bgl
{
	struct FWindowSettings
	{
		unsigned int width = 320;
		unsigned int height = 240;
		std::string name = "Bagual Engine Window";
	};

	class BPlatformWindowInterface
	{

	public:

		virtual FWindowSettings GetWindowSettings() = 0;

		virtual void SetWindowSettings(const FWindowSettings& newSettings) = 0;

	};

	class BPlatformInterface
	{

	public:

		virtual std::shared_ptr<BPlatformWindowInterface> CreateWindow(const FWindowSettings& settings) = 0;
	};

	class BPlatformBase : public BSingleton<BPlatformBase>, public BPlatformInterface
	{

	};

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