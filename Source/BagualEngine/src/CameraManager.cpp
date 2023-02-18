
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "CameraManager.h"

#include "Camera.h"
#include "Viewport.h"

namespace bgl
{
	BArray< BCamera* > BCameraManager::m_cameras = BArray< BCamera* >();

	void BCameraManager::AddCamera( BCamera* camera )
	{
		m_cameras.Add( camera );
	}

	void BCameraManager::RemoveCamera( BCamera* camera )
	{
		m_cameras.Remove( camera );
	}

	BArray< BCamera* > BCameraManager::GetCameras()
	{
		return m_cameras;
	}

} // namespace bgl
