#pragma once

#include "Common.h"

#include "Canvas.h"

namespace bgl
{
	static constexpr const uint& BGL_WINDOW_CENTRALIZED = -1;

	struct FWindowSettings
	{
		uint x = BGL_WINDOW_CENTRALIZED;
		uint y = BGL_WINDOW_CENTRALIZED;
		uint width = 320;
		uint height = 240;
		std::string name = "Bagual Engine Window";
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

		BArray<std::shared_ptr<BPlatformWindow>> GetWindows();

	};

}