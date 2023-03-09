
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "Camera.h"

#include "Arithmetics.h"
#include "BagualEngine.h"
#include "CameraManager.h"
#include "Canvas.h"
#include "GraphicsPlatform.h"
#include "Logger.h"
#include "PlatformBase.h"
#include "Scene.h"
#include "Viewport.h"

namespace bgl
{

	BCamera::BCamera()
	{
		BGL_ASSERT( false && "Trying to build an empty camera!" );
	}

	BCamera::BCamera( BViewport* viewport, BCameraComponent* owner )
	{
		SetViewport( viewport );
		m_owner = owner;
	}

	BEIntrinsicsMode& BCamera::GetIntrinsicsMode_Mutable()
	{
		return m_intrinsicsMode;
	}

	const BEIntrinsicsMode BCamera::GetIntrinsicsMode() const
	{
		return m_intrinsicsMode;
	}

	void BCamera::SetIntrinsicsMode( const BEIntrinsicsMode intrinsicsMode )
	{
		m_intrinsicsMode = intrinsicsMode;
	}

	BERenderSpeed& BCamera::GetRenderSpeed_Mutable()
	{
		return m_renderSpeed;
	}

	const BERenderSpeed BCamera::GetRenderSpeed() const
	{
		return m_renderSpeed;
	}

	void BCamera::SetRenderSpeed( const BERenderSpeed renderSpeed )
	{
		m_renderSpeed = renderSpeed;
	}

	BERenderThreadMode& BCamera::GetRenderThreadMode_Mutable()
	{
		return m_renderThreadMode;
	}

	const BERenderThreadMode BCamera::GetRenderThreadMode() const
	{
		return m_renderThreadMode;
	}

	void BCamera::SetRenderThreadMode( const BERenderThreadMode renderThreadMode )
	{
		m_renderThreadMode = renderThreadMode;
	}

	BVec2f& BCamera::GetSensorSize_Mutable()
	{
		return m_sensorSize;
	}

	const BVec2f BCamera::GetSensorSize() const
	{
		return m_sensorSize;
	}

	void BCamera::SetSensorSize( const BVec2f& sensorSize )
	{
		m_sensorSize = sensorSize;
	}

	const BVec2f BCamera::GetSensorArea() const
	{
		const auto sensorSize = GetSensorSize();
		return BVector2< float >( sensorSize.x / 10.f, sensorSize.y / 10.f );
	}

	const float BCamera::GetSensorDistance() const
	{
		const auto sensorArea = GetSensorArea();
		const float biggestSensorSide = std::max( sensorArea.x, sensorArea.y );
		return ( biggestSensorSide / 2.f ) * ( 2.f - std::sinf( deg2rad( GetFOV() / 2.f ) ) );
	}

	const BERenderOutputType BCamera::GetRenderOutputType() const
	{
		return m_renderOutputType;
	}

	BERenderOutputType& BCamera::GetRenderOutputType_Mutable()
	{
		return m_renderOutputType;
	}

	void BCamera::SetRenderOutputType( const BERenderOutputType type )
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

	void BCamera::SetDepthDistance( const float depth )
	{
		m_depthDist = depth;
	}

	void BCamera::SetFOV( const float fov )
	{
		m_fov = fov;
	}

	BViewport* BCamera::GetViewport()
	{
		BViewport* result = nullptr;

		auto viewports = BEngine::GraphicsPlatform().GetViewports();

		// Checking if any viewport points to this camera
		for( auto viewport : viewports )
		{
			if( viewport->GetCamera() == this )
			{
				result = viewport;
				break;
			}
		}

		return result;
	}

	void BCamera::SetViewport( BViewport* viewport )
	{
		if( viewport )
		{
			viewport->SetCamera( this );
		}
	}

	void BCamera::AddLine2DBuffer( const BLine< BPixelPos >& line )
	{
		m_line2DBuffer.Add( line );
	}

	BArray< BLine< BPixelPos > >& BCamera::GetLine2DBuffer()
	{
		return m_line2DBuffer;
	}

	void BCamera::ClearLine2DBuffer()
	{
		m_line2DBuffer.Clear();
	}

	void BCamera::AddLine3DBuffer( const BLine< BVec3f >& line )
	{
		m_line3DBuffer.Add( line );
	}

	BArray< BLine< BVec3f > >& BCamera::GetLine3DBuffer()
	{
		return m_line3DBuffer;
	}

	void BCamera::ClearLine3DBuffer()
	{
		m_line3DBuffer.Clear();
	}

	BTransform< float >& BCamera::GetTransform_Mutable()
	{
		if( m_owner )
		{
			return m_owner->getTransform_mutable();
		}

		return m_detachedTransform;
	}

	const BTransform< float > BCamera::GetTransform() const
	{
		if( m_owner )
		{
			return m_owner->getTransform();
		}

		return m_detachedTransform;
	}

	const BVec3f BCamera::GetLocation() const
	{
		return GetTransform().translation;
	}

	const BRotf BCamera::GetRotation() const
	{
		return GetTransform().rotation;
	}

	const BVec3f BCamera::GetScale() const
	{
		return GetTransform().scale;
	}

	void BCamera::SetTransform( const BTransform< float >& transform )
	{
		GetTransform_Mutable() = transform;
	}

	void BCamera::SetLocation( const BVec3f& location )
	{
		GetTransform_Mutable().translation = location;
	}

	void BCamera::SetRotation( const BRotf& rotation )
	{
		GetTransform_Mutable().rotation = rotation;
	}

	void BCamera::SetScale( const BVec3f& scale )
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

} // namespace bgl