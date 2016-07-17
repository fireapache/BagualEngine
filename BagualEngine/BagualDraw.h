#pragma once

#include "BagualCameraSettings.h"
#include "BagualMath.h"

bool IsLineOnScreen(CameraSettings &camera, Line<Pixel> &line);

void DrawLine(CameraSettings &camera, const Line<Pixel> &line);

void DrawLine(CameraSettings &camera, const Pixel &p1, const Pixel &p2);