#pragma once

#include "Common.h"

#include "PlatformBase.h"
#include "GraphicsDriverGeneric.h"

namespace bgl
{
	class BViewport;
	class BPlatformWindow;
	class BCameraComponent;
}

namespace bgl
{
	class BCamera
	{
		BViewport* m_viewport = nullptr;
		BArray<BLine<BPixelPos>> m_line2DBuffer;
		float m_fov = 90.f;
		BCameraComponent* m_owner = nullptr;
		BTransform<float> m_detachedTransform;
		BERenderOutputType m_renderOutputType = BERenderOutputType::Depth;
		float m_depthDist = 1000.f;
		BVec2f m_sensorSize = BVec3f(24.f, 36.f);
		BERenderSpeed m_renderSpeed = BERenderSpeed::VeryFast;
		BERenderThreadMode m_renderThreadMode = BERenderThreadMode::MultiThread;

	public:

		BCamera();
		BCamera(BViewport* viewport, BCameraComponent* owner = nullptr);

		BERenderSpeed& GetRenderSpeed_Mutable();
		const BERenderSpeed GetRenderSpeed();
		void SetRenderSpeed(const BERenderSpeed renderSpeed);

		BERenderThreadMode& GetRenderThreadMode_Mutable();
		const BERenderThreadMode GetRenderThreadMode();
		void SetRenderThreadMode(const BERenderThreadMode renderThreadMode);

		BVec2f& GetSensorSize_Mutable();
		const BVec2f GetSensorSize();
		void SetSensorSize(const BVec2f sensorSize);

		const BERenderOutputType GetRenderOutputType() const;
		BERenderOutputType& GetRenderOutputType_Mutable();
		void SetRenderOutputType(const BERenderOutputType type);

		const float GetDepthDistance() const;
		float& GetDepthDistance_Mutable();
		void SetDepthDistance(const float depth);

		const float GetFOV() const;
		float& GetFOV_Mutable();
		void SetFOV(const float fov);

		// Line to be render on top of everything during camera render loop in the Renderer
		void AddLine2DBuffer(const BLine<BPixelPos>& line);

		BViewport* GetViewport();
		void SetViewport(BViewport* viewport);

		BArray<BLine<BPixelPos>>& GetLine2DBuffer();

		void ClearLine2DBuffer();

		BTransform<float>& GetTransform_Mutable();
		const BTransform<float> GetTransform() const;
		const BVec3f GetLocation() const;
		const BVec3f GetRotation() const;
		const BVec3f GetScale() const;

		void SetTransform(const BTransform<float>& transform);
		void SetLocation(const BVec3f& translation);
		void SetRotation(const BVec3f& rotation);
		void SetScale(const BVec3f& scale);

	};
}