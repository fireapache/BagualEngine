
#include "BEngine.h"
#include "BCamera.h"
#include "BLogger.h"

#include <SDL.h>

namespace Bagual::Camera
{

	BCamera::BCamera()
	{
		auto graphics = Bagual::BagualEngine::GraphicsPlatform();

		if (graphics)
		{
			canvas = graphics->CreateCanvas(Settings::width, Settings::height);
		}
		else
		{
			Bagual::Utils::BLogger::Log("Could not create canvas for camera object!");
		}
	}
	
	void Bagual::Camera::BCamera::AddLine2DBuffer(const Bagual::Types::BLine<Bagual::Types::BPixelPos>& line)
	{
		Line2DBuffer.Add(line);
	}

	Bagual::Graphics::BCanvas& BCamera::GetCanvas()
	{
		return *canvas;
	}

	Bagual::Types::BArray<Bagual::Types::BLine<Bagual::Types::BPixelPos>>& BCamera::GetLine2DBuffer()
	{
		return Line2DBuffer;
	}

	void BCamera::ClearLine2DBuffer()
	{
		Line2DBuffer.Clear();
	}

}