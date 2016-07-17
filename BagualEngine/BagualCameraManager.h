#pragma once

#include <vector>

#include "BagualCamera.h"


class CameraManager
{

	static vector<Camera*> cameras;

public:

	static void AddCamera(Camera *camera);

	static void RemoveCamera(Camera *camera);

	static vector<Camera*>& GetCameras() { return cameras; }

};

