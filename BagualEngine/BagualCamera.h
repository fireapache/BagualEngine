#pragma once

#include <SDL2\SDL.h>

#include "BagualTypes.h"
#include "BagualSettings.h"
#include "BagualCameraSettings.h"
#include "BagualDraw.h"


class Camera
{
	CameraSettings settings;

public:

	void DrawScreen();

	void SetupScreen();

};

