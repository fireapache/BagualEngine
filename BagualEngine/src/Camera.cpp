
#include "Camera.h"
#include "Engine.h"
#include "Logger.h"
#include "Settings.h"
#include "GraphicsPlatform.h"

#include <SDL.h>

namespace bgl
{

	BCamera::BCamera()
	{
		auto graphics = Engine::GraphicsPlatform();

		if (graphics)
		{
			canvas = graphics->CreateCanvas(Settings::width, Settings::height);
		}
		else
		{
			BLogger::Log("Could not create canvas for camera object due to null Graphics Platform!");
		}
	}
	
	void BCamera::AddLine2DBuffer(const BLine<BPixelPos>& line)
	{
		Line2DBuffer.Add(line);
	}

	BCanvas& BCamera::GetCanvas()
	{
		return *canvas;
	}

	BArray<BLine<BPixelPos>>& BCamera::GetLine2DBuffer()
	{
		return Line2DBuffer;
	}

	void BCamera::ClearLine2DBuffer()
	{
		Line2DBuffer.Clear();
	}

}