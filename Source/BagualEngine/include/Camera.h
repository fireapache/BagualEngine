#pragma once

#include "Common.h"

#include "PlatformBase.h"

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

	public:

		BCamera();
		BCamera(BViewport* viewport, BCameraComponent* owner = nullptr);

		void SetFOV(const float& newFov);

		const float GetFOV() const;
		float& GetFOV_Mutable();

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