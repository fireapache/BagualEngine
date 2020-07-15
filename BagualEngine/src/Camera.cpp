
#include "Engine.h"
#include "Camera.h"
#include "Logger.h"

#include <SDL.h>

namespace bgl
{

	BCamera::BCamera()
	{
		auto graphics = bgl::Engine::GraphicsPlatform();

		if (graphics)
		{
			canvas = graphics->CreateCanvas(Settings::width, Settings::height);
		}
		else
		{
			bgl::BLogger::Log("Could not create canvas for camera object!");
		}
	}
	
	void bgl::BCamera::AddLine2DBuffer(const bgl::BLine<bgl::BPixelPos>& line)
	{
		Line2DBuffer.Add(line);
	}

	bgl::BCanvas& BCamera::GetCanvas()
	{
		return *canvas;
	}

	bgl::BArray<bgl::BLine<bgl::BPixelPos>>& BCamera::GetLine2DBuffer()
	{
		return Line2DBuffer;
	}

	void BCamera::ClearLine2DBuffer()
	{
		Line2DBuffer.Clear();
	}

}