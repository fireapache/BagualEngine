
#include "Bagual.pch.h"

#include "Camera.h"
#include "Viewport.h"
#include "BagualEngine.h"
#include "Logger.h"
#include "PlatformBase.h"
#include "Canvas.h"
#include "Scene.h"

namespace bgl
{

	BCamera::BCamera()
	{
		BGL_ASSERT(false && "Trying to build an empty camera!");
	}
	
	BCamera::BCamera(BViewport* viewport, BCameraComponent* owner)
	{
		m_viewport = viewport;
		m_owner = owner;
	}

	void BCamera::SetFOV(const float& newFov)
	{
		m_fov = newFov;
	}

	void BCamera::AddLine2DBuffer(const BLine<BPixelPos>& line)
	{
		m_line2DBuffer.Add(line);
	}

	BViewport* BCamera::GetViewport()
	{
		return m_viewport;
	}

	void BCamera::SetViewport(BViewport* viewport)
	{
		m_viewport = viewport;
	}

	BArray<BLine<BPixelPos>>& BCamera::GetLine2DBuffer()
	{
		return m_line2DBuffer;
	}

	void BCamera::ClearLine2DBuffer()
	{
		m_line2DBuffer.Clear();
	}

	BTransform<float>& BCamera::GetTransform_Mutable()
	{
		if (m_owner)
		{
			return m_owner->GetTransform_Mutable();
		}

		return m_detachedTransform;
	}

	const BTransform<float> BCamera::GetTransform() const
	{
		if (m_owner)
		{
			return m_owner->GetTransform();
		}

		return m_detachedTransform;
	}

	const BVec3f BCamera::GetLocation() const
	{
		return GetTransform().translation;
	}

	const BVec3f BCamera::GetRotation() const
	{
		return GetTransform().rotation;
	}

	const BVec3f BCamera::GetScale() const
	{
		return GetTransform().scale;
	}

	void BCamera::SetTransform(const BTransform<float>& transform)
	{
		GetTransform_Mutable() = transform;
	}

	void BCamera::SetLocation(const BVec3f& location)
	{
		GetTransform_Mutable().translation = location;
	}

	void BCamera::SetRotation(const BVec3f& rotation)
	{
		GetTransform_Mutable().rotation = rotation;
	}

	void BCamera::SetScale(const BVec3f& scale)
	{
		GetTransform_Mutable().scale = scale;
	}

	const float BCamera::GetFOV() const
	{
		return m_fov;
	}

	float& BCamera::GetFOV_Mutable()
	{
		return m_fov;
	}

}