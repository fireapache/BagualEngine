#pragma once

#include "Common.h"
#include "GraphicsDriverGeneric.h"
#include "PlatformBase.h"

namespace bgl
{
	class BViewport;
	class BPlatformWindow;
	class BCameraComponent;
} // namespace bgl

namespace bgl
{
	class BCamera
	{
		BArray< BLine< BPixelPos > > m_line2DBuffer;
		BArray< BLine< BVec3f > > m_line3DBuffer;
		float m_fov = 90.f;
		BCameraComponent* m_owner = nullptr;
		BTransform< float > m_detachedTransform;
		BERenderOutputType m_renderOutputType = BERenderOutputType::Depth;
		float m_depthDist = 1000.f;
		BVec2f m_sensorSize = BVec2f( 36.f, 24.f );
		BERenderSpeed m_renderSpeed = BERenderSpeed::VeryFast;
		BERenderThreadMode m_renderThreadMode = BERenderThreadMode::MultiThread;
		BERenderMode m_renderMode = BERenderMode::Sequential;

	public:
		BCamera();
		BCamera( BViewport* viewport, BCameraComponent* owner = nullptr );

		BERenderMode& GetRenderMode_Mutable();
		const BERenderMode GetRenderMode() const;
		void SetRenderMode( const BERenderMode renderMode );

		BERenderSpeed& GetRenderSpeed_Mutable();
		const BERenderSpeed GetRenderSpeed() const;
		void SetRenderSpeed( const BERenderSpeed renderSpeed );

		BERenderThreadMode& GetRenderThreadMode_Mutable();
		const BERenderThreadMode GetRenderThreadMode() const;
		void SetRenderThreadMode( const BERenderThreadMode renderThreadMode );

		BVec2f& GetSensorSize_Mutable();
		const BVec2f GetSensorSize() const;
		void SetSensorSize( const BVec2f& sensorSize );
		const BVec2f GetSensorArea() const;
		const float GetSensorDistance() const;

		const BERenderOutputType GetRenderOutputType() const;
		BERenderOutputType& GetRenderOutputType_Mutable();
		void SetRenderOutputType( const BERenderOutputType type );

		const float GetDepthDistance() const;
		float& GetDepthDistance_Mutable();
		void SetDepthDistance( const float depth );

		const float GetFOV() const;
		float& GetFOV_Mutable();
		void SetFOV( const float fov );
		
		BViewport* GetViewport();
		void SetViewport( BViewport* viewport );

		void AddLine2DBuffer( const BLine< BPixelPos >& line );
		BArray< BLine< BPixelPos > >& GetLine2DBuffer();
		void ClearLine2DBuffer();

		void AddLine3DBuffer( const BLine< BVec3f >& line );
		BArray< BLine< BVec3f > >& GetLine3DBuffer();
		void ClearLine3DBuffer();

		BTransform< float >& GetTransform_Mutable();
		const BTransform< float > GetTransform() const;
		const BVec3f GetLocation() const;
		const BRotf GetRotator() const;
		const BVec3f GetScale() const;

		void SetTransform( const BTransform< float >& transform );
		void SetLocation( const BVec3f& translation );
		void SetRotation( const BRotf& rotation );
		void SetScale( const BVec3f& scale );
	};
} // namespace bgl