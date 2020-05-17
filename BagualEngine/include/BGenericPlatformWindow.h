#pragma once

#include <memory>

#include "BPlatformWindowBase.h"

#include <SDL.h>

namespace Bagual::Platform
{
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