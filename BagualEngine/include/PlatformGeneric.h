#pragma once

#include "Common.h"

#include "PlatformBase.h"

#include <SDL.h>

namespace bgl
{
	class BPlatformGeneric : public BPlatformBase
	{

	public:

		virtual std::shared_ptr<BPlatformWindow> CreateWindow() override;
		virtual std::shared_ptr<BPlatformWindow> CreateWindow(const FWindowSettings& settings) override;
	};

	class BGenericPlatformWindow : public BPlatformWindow
	{
		SDL_Window* sdlWindow = nullptr;
		SDL_Surface* sdlSurface = nullptr;

	protected:

		virtual void ApplySettings() override;

		virtual void Create() override;

		virtual void Destroy() override;

	public:

		BGenericPlatformWindow() = delete;
		BGenericPlatformWindow(const FWindowSettings& windowSettings);
		~BGenericPlatformWindow();

		SDL_Window* GetSDL_Window();
		SDL_Surface* GetSDL_Surface();
	};
}