#pragma once

#include <memory>

#include "PlatformBase.h"

#include <SDL.h>

namespace bgl
{
	class BGenericPlatform : public BPlatformBase
	{
		virtual std::shared_ptr<BPlatformWindowInterface> CreateWindow(const FWindowSettings& settings) override;
	};

	class BGenericPlatformWindow : public BPlatformWindowBase
	{
		SDL_Window* sdlWindow = nullptr;

	protected:

		virtual void ApplyWindowSettings() override;

		virtual void InitializeWindow() override;

		virtual void FinalizeWindow() override;

	public:

		BGenericPlatformWindow() = delete;
		BGenericPlatformWindow(const FWindowSettings& windowSettings);
		~BGenericPlatformWindow();

	};
}