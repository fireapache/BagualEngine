
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "PlatformBase.h"

#include "Assertions.h"

namespace bgl
{

	BPlatformWindow::BPlatformWindow( const FWindowSettings& windowSettings )
	{
		settings = windowSettings;
		create();
	}

	FWindowSettings BPlatformWindow::getSettings() const
	{
		return settings;
	}

	void BPlatformWindow::setWindow( const FWindowSettings& newSettings )
	{
		settings = newSettings;
		applySettings();
	}

	BCanvas* BPlatformWindow::getCanvas()
	{
		return m_canvas.get();
	}
	
	void BPlatformWindow::applySettings()
	{
		BGL_NEED_IMPLEMENTATION
	}

	void BPlatformWindow::create()
	{
		BGL_NEED_IMPLEMENTATION
	}

	void BPlatformWindow::destroy()
	{
		BGL_NEED_IMPLEMENTATION
	}

	BArray< std::shared_ptr< BPlatformWindow > >& BPlatformBase::getWindows()
	{
		return windows;
	}

	BPlatformWindow* BPlatformBase::getWindow( const uint32_t index )
	{
		if( windows.Size() < index )
		{
			return windows[ index ].get();
		}

		return nullptr;
	}
} // namespace bgl