#pragma once

#include "BTypes.h"
#include "BSettings.h"
#include "BCameraSettings.h"
#include "BDraw.h"
#include "BGraphicsPlatform.h"

namespace Bagual::Camera
{
	class BCamera
	{
		BCameraSettings settings;

		std::weak_ptr<Bagual::Graphics::BCanvas> canvas;

	public:

		BCamera();

		void DrawScreen();

		void SetupScreen();

	};
}