#pragma once

#include "Common.h"

namespace bgl
{
	class BViewport;
}

namespace bgl
{
	class BCamera;
}

namespace bgl
{
	class BCameraManager
	{
		static BArray< BCamera* > m_cameras;

	public:
		static void AddCamera( BCamera* camera );
		static void RemoveCamera( BCamera* camera );

		static BArray< BCamera* > GetCameras();
	};
} // namespace bgl
