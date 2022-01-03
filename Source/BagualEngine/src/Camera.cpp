
#include "Bagual.pch.h"

#include "Camera.h"
#include "Viewport.h"
#include "BagualEngine.h"
#include "Logger.h"
#include "PlatformBase.h"
#include "Canvas.h"

namespace bgl
{

	BCamera::BCamera()
	{
		BGL_ASSERT(false && "Trying to build an empty camera!");
	}
	
	BCamera::BCamera(const std::shared_ptr<BViewport>& viewport)
	{
		this->viewport = viewport;
	}

	void BCamera::SetFOV(const float& newFov)
	{
		fov = newFov;
	}

	void BCamera::AddLine2DBuffer(const BLine<BPixelPos>& line)
	{
		Line2DBuffer.Add(line);
	}

	BViewport& BCamera::GetViewport()
	{
		return *viewport;
	}

	BArray<BLine<BPixelPos>>& BCamera::GetLine2DBuffer()
	{
		return Line2DBuffer;
	}

	void BCamera::ClearLine2DBuffer()
	{
		Line2DBuffer.Clear();
	}

	const float& BCamera::GetFOV() const
	{
		return fov;
	}

	float& BCamera::GetFOV_Mutable()
	{
		return fov;
	}

}