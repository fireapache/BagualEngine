#pragma once

#include <vector>
#include <SDL2\SDL.h>

#include "BagualTypes.h"
#include "BagualCameraManager.h"
#include "BagualMath.h"


class Renderer
{
	//static Renderer *instance;

	//Renderer() {}

public:

	/*Renderer(const Renderer&) {}
	void operator=(Renderer&) {}

	static Renderer* Instance()
	{
		if (!instance) instance = new Renderer();
		return instance;
	}*/

	static void Render();

};

