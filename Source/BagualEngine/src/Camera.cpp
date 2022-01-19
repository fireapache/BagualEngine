
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

	BERenderSpeed& BCamera::GetRenderSpeed_Mutable()
	{
		return m_renderSpeed;
	}

	const BERenderSpeed BCamera::GetRenderSpeed()
	{
		return m_renderSpeed;
	}

	void BCamera::SetRenderSpeed(const BERenderSpeed renderSpeed)
	{
		m_renderSpeed = renderSpeed;
	}

	BERenderThreadMode& BCamera::GetRenderThreadMode_Mutable()
	{
		return m_renderThreadMode;
	}

	const BERenderThreadMode BCamera::GetRenderThreadMode()
	{
		return m_renderThreadMode;
	}

	void BCamera::SetRenderThreadMode(const BERenderThreadMode renderThreadMode)
	{
		m_renderThreadMode = renderThreadMode;
	}

	BVec2f& BCamera::GetSensorSize_Mutable()
	{
		return m_sensorSize;
	}

	const BVec2f BCamera::GetSensorSize()
	{
		return m_sensorSize;
	}

	void BCamera::SetSensorSize(const BVec2f sensorSize)
	{
		m_sensorSize = sensorSize;
	}

	const BERenderOutputType BCamera::GetRenderOutputType() const
	{
		return m_renderOutputType;
	}

	BERenderOutputType& BCamera::GetRenderOutputType_Mutable()
	{
		return m_renderOutputType;
	}

	void BCamera::SetRenderOutputType(const BERenderOutputType type)
	{
		m_renderOutputType = type;
	}

	const float BCamera::GetDepthDistance() const
	{
		return m_depthDist;
	}

	float& BCamera::GetDepthDistance_Mutable()
	{
		return m_depthDist;
	}

	void BCamera::SetDepthDistance(const float depth)
	{
		m_depthDist = depth;
	}

	void BCamera::SetFOV(const float fov)
	{
		m_fov = fov;
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