#pragma once

#include "Canvas.h"
#include "Common.h"

#include <functional>

namespace bgl
{
	static constexpr const uint32_t& BGL_WINDOW_CENTRALIZED = -1;

	struct FWindowSettings
	{
		uint32_t x = BGL_WINDOW_CENTRALIZED;
		uint32_t y = BGL_WINDOW_CENTRALIZED;
		uint32_t width = 320;
		uint32_t height = 240;
		std::string Title = "Bagual Engine Window";
	};

	class BPlatformWindow
	{
	protected:
		std::unique_ptr< BCanvas > m_canvas;

		FWindowSettings settings;

		virtual void applySettings();

		virtual void create();

		virtual void destroy();

		BArray< GuiTickFuncType* > m_guiTickFuncs;

	public:
		BPlatformWindow(){};
		BPlatformWindow( const FWindowSettings& windowSettings );

		virtual bool Tick() = 0;

		virtual FWindowSettings getSettings() const;

		virtual void setWindow( const FWindowSettings& newSettings );

		virtual BCanvas* getCanvas();

		void addGuiTickFunc( GuiTickFuncType* func );
		BArray< GuiTickFuncType* >& getGuiTickFuncs();
	};

	class BPlatformBase
	{
	protected:
		BArray< std::shared_ptr< BPlatformWindow > > windows;

	public:
		virtual BPlatformWindow* createWindow() = 0;
		virtual BPlatformWindow* createWindow( const FWindowSettings& settings ) = 0;

		BArray< std::shared_ptr< BPlatformWindow > >& getWindows();
		BPlatformWindow* getWindow( const uint32_t index );
	};

} // namespace bgl