#pragma once

#include "Common.h"

#include "Canvas.h"

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


	protected:

		std::shared_ptr<BCanvas> canvas;

		FWindowSettings settings;

		virtual void ApplySettings();

		virtual void Create();

		virtual void Destroy();

	public:

		BPlatformWindow() {  };
		BPlatformWindow(const FWindowSettings& windowSettings);

		virtual bool Tick() = 0;

		virtual FWindowSettings GetSettings() const;

		virtual void SetWindow(const FWindowSettings& newSettings);

		virtual const std::shared_ptr<BCanvas>& GetCanvas();

	};

	class BPlatformBase
	{


	protected:

		BArray<std::shared_ptr<BPlatformWindow>> windows;

	public:

		virtual std::shared_ptr<BPlatformWindow> CreateWindow() = 0;
		virtual std::shared_ptr<BPlatformWindow> CreateWindow(const FWindowSettings& settings) = 0;

		BArray<std::shared_ptr<BPlatformWindow>>& GetWindows();

	};

}