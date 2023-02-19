
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "Scene.h"

#include "BagualEngine.h"
#include "Camera.h"
#include "CameraManager.h"
#include "Viewport.h"

#include <obj_parse.h>

namespace bgl
{

	BArray< BArray< BTriangle< float > >* > BMeshComponent::g_meshComponentTriangles;
	BArray< BMeshComponent* > BMeshComponent::g_meshComponents;

	BNode::BNode( BNode* parent, const char* name )
	{
		m_parent = parent;

		if( name )
		{
			this->m_name = std::string( name );
		}
	}

	BNode* BNode::GetParent()
	{
		return m_parent;
	}

	BArray< BNode* >& BNode::GetChilds()
	{
		return m_childs;
	}

	BArray< std::shared_ptr< BComponent > >& BNode::GetComponents()
	{
		return m_components;
	}

	const bool BNode::IsVisible() const
	{
		if( m_parent )
		{
			return m_parent->IsVisible() && !m_bHidden;
		}

		return !m_bHidden;
	}

	void BNode::SetHidden( const bool bHidden )
	{
		m_bHidden = bHidden;
	}

	BTransform< float >& BNode::GetTransform_Mutable()
	{
		return m_transform;
	}

	const BTransform< float > BNode::GetTransform() const
	{
		return m_transform;
	}

	const BVec3f BNode::GetLocation() const
	{
		return m_transform.translation;
	}

	const BRotf BNode::GetRotation() const
	{
		return m_transform.rotation;
	}

	const BVec3f BNode::GetScale() const
	{
		return m_transform.scale;
	}

	void BNode::SetTransform( const BTransform< float >& transform )
	{
		m_transform = transform;
	}

	void BNode::SetLocation( const BVec3f& location )
	{
		m_transform.translation = location;
	}

	void BNode::SetRotation( const BRotf& rotation )
	{
		m_transform.rotation = rotation;
	}

	void BNode::SetScale( const BVec3f& scale )
	{
		m_transform.scale = scale;
	}

	void BNode::SetParent( BNode* node )
	{
		if( m_parent )
		{
			m_parent->RemoveChild( this );
		}

		m_parent = node;
		node->AddChild( this );
	}

	void BNode::AddChild( BNode* node )
	{
		m_childs.Add( node );
	}

	void BNode::RemoveChild( BNode* node )
	{
		m_childs.Remove( node );
	}

	BMeshComponent::BMeshComponent(
		class BNode* owner /*= nullptr*/,
		const char* name /*= "None"*/,
		const char* assetPath /*= nullptr*/ )
		: BComponent( owner, name )
	{
		g_meshComponentTriangles.Add( &m_MeshData.triangles );
		g_meshComponents.Add( this );
		if( assetPath )
			LoadMesh( assetPath );
	}

	BMeshComponent::~BMeshComponent()
	{
		g_meshComponentTriangles.Remove( &m_MeshData.triangles );
		g_meshComponents.Remove( this );
	}

	void BMeshComponent::addUniqueEdge( const BLine< BVec3f >& line )
	{
		auto& edges = m_MeshData.edges;
		bool bUnique = true;
		for( auto* curEdge = edges.data(); curEdge < edges.data() + edges.Size(); curEdge++ )
		{
			if( *curEdge == line )
			{
				bUnique = false;
				break;
			}
		}

		if( bUnique )
		{
			edges.emplace_back( line );
		}
	}

	void BMeshComponent::addUniqueTriEdges( const BTriangle< float >& tri )
	{
		auto& edges = m_MeshData.edges;
		addUniqueEdge( BLine< BVec3f >( tri.v0, tri.v1 ) );
		addUniqueEdge( BLine< BVec3f >( tri.v0, tri.v2 ) );
		addUniqueEdge( BLine< BVec3f >( tri.v1, tri.v2 ) );
	}

	void BMeshComponent::LoadMesh( const char* assetPath )
	{
		if( assetPath == nullptr )
		{
			BGL_LOG( "Got null asset path when loading mesh!" );
			return;
		}

		BTriangle< float > triCache;
		objl::Vertex vert0, vert1, vert2;
		uint32_t index0, index1, index2;

		objl::Loader triLoader( assetPath );

		for( size_t i = 0; i < triLoader.LoadedIndices.size(); i += 3 )
		{
			index0 = triLoader.LoadedIndices[ i ];
			index1 = triLoader.LoadedIndices[ i + 1 ];
			index2 = triLoader.LoadedIndices[ i + 2 ];

			vert0 = triLoader.LoadedVertices[ index0 ];
			vert1 = triLoader.LoadedVertices[ index1 ];
			vert2 = triLoader.LoadedVertices[ index2 ];

			triCache.v0.x = vert0.Position.X;
			triCache.v0.y = vert0.Position.Y;
			triCache.v0.z = vert0.Position.Z;
			triCache.v1.x = vert1.Position.X;
			triCache.v1.y = vert1.Position.Y;
			triCache.v1.z = vert1.Position.Z;
			triCache.v2.x = vert2.Position.X;
			triCache.v2.y = vert2.Position.Y;
			triCache.v2.z = vert2.Position.Z;

			m_MeshData.triangles.Add( triCache );

			addUniqueTriEdges( triCache );

			m_MeshData.triangles_SIMD.v0.x.Add( triCache.v0.x );
			m_MeshData.triangles_SIMD.v0.y.Add( triCache.v0.y );
			m_MeshData.triangles_SIMD.v0.z.Add( triCache.v0.z );
			m_MeshData.triangles_SIMD.v1.x.Add( triCache.v1.x );
			m_MeshData.triangles_SIMD.v1.y.Add( triCache.v1.y );
			m_MeshData.triangles_SIMD.v1.z.Add( triCache.v1.z );
			m_MeshData.triangles_SIMD.v2.x.Add( triCache.v2.x );
			m_MeshData.triangles_SIMD.v2.y.Add( triCache.v2.y );
			m_MeshData.triangles_SIMD.v2.z.Add( triCache.v2.z );
		}

		const size_t simdFillerCount = ( 8 - m_MeshData.triangles_SIMD.v0.x.Size() % 8 ) % 8;

		for( size_t i = 0; i < simdFillerCount; i++ )
		{
			constexpr float dummy = 0.f;
			m_MeshData.triangles_SIMD.v0.x.Add( dummy );
			m_MeshData.triangles_SIMD.v0.y.Add( dummy );
			m_MeshData.triangles_SIMD.v0.z.Add( dummy );
			m_MeshData.triangles_SIMD.v1.x.Add( dummy );
			m_MeshData.triangles_SIMD.v1.y.Add( dummy );
			m_MeshData.triangles_SIMD.v1.z.Add( dummy );
			m_MeshData.triangles_SIMD.v2.x.Add( dummy );
			m_MeshData.triangles_SIMD.v2.y.Add( dummy );
			m_MeshData.triangles_SIMD.v2.z.Add( dummy );
		}
	}

	bool BMeshComponent::getShowWireframe() const
	{
		return m_ShowWireframe;
	}

	bool& BMeshComponent::getShowWireframe_Mutable()
	{
		return m_ShowWireframe;
	}

	void BMeshComponent::setShowWireframe( const bool bValue )
	{
		m_ShowWireframe = bValue;
	}

	bgl::BArray< bgl::BTriangle< float > >& BMeshComponent::GetTriangles()
	{
		return m_MeshData.triangles;
	}

	BTriangle< BArray< float > >& BMeshComponent::GetTriangles_SIMD()
	{
		return m_MeshData.triangles_SIMD;
	}

	void BMeshComponent::AddTriangles( const BArray< BTriangle< float > >& triangles )
	{
		m_MeshData.triangles.Add( triangles );

		for( auto& tri : triangles )
		{
			addUniqueTriEdges( tri );

			m_MeshData.triangles_SIMD.v0.x.Add( tri.v0.x );
			m_MeshData.triangles_SIMD.v0.y.Add( tri.v0.y );
			m_MeshData.triangles_SIMD.v0.z.Add( tri.v0.z );

			m_MeshData.triangles_SIMD.v1.x.Add( tri.v1.x );
			m_MeshData.triangles_SIMD.v1.y.Add( tri.v1.y );
			m_MeshData.triangles_SIMD.v1.z.Add( tri.v1.z );

			m_MeshData.triangles_SIMD.v2.x.Add( tri.v2.x );
			m_MeshData.triangles_SIMD.v2.y.Add( tri.v2.y );
			m_MeshData.triangles_SIMD.v2.z.Add( tri.v2.z );
		}
	}

	BScene::BScene()
	{
		m_sceneRoot = std::make_unique< BNode >( nullptr, "Scene Root Actor" );
	}

	BNode* BScene::CreateNode( const char* name /*= "None"*/ )
	{
		m_nodes.push_back( std::make_shared< BNode >( m_sceneRoot.get(), name ) );
		BNode* rawPtr = m_nodes.back().get();
		m_sceneRoot->AddChild( rawPtr );
		return rawPtr;
	}

	BNode* BScene::CreateNode( BNode& parent, const char* name /*= "None"*/ )
	{
		m_nodes.push_back( std::make_shared< BNode >( &parent, name ) );
		BNode* rawPtr = m_nodes.back().get();
		m_sceneRoot->AddChild( rawPtr );
		return rawPtr;
	}

	BComponent::BComponent( BNode* owner, const char* name )
	{
		SetOwner( owner );
		m_name = std::string( name );
	}

	BComponent::~BComponent()
	{
	}

	const bool BComponent::IsVisible() const
	{
		if( m_owner )
		{
			return m_owner->IsVisible() && !m_bHidden;
		}

		return !m_bHidden;
	}

	void BComponent::SetOwner( BNode* owner )
	{
		m_owner = owner;
	}

	BTransform< float >& BComponent::GetTransform_Mutable()
	{
		if( m_owner )
		{
			return m_owner->GetTransform_Mutable();
		}
		else
		{
			BGL_LOG( "Got null owner when returning mutable transform!" );
		}

		return m_dummyTransform;
	}

	const BTransform< float > BComponent::GetTransform() const
	{
		if( m_owner )
		{
			return m_owner->GetTransform();
		}

		return BTransform< float >();
	}

	const BVec3f BComponent::GetLocation() const
	{
		return GetTransform().translation;
	}

	const BRotf BComponent::GetRotation() const
	{
		return GetTransform().rotation;
	}

	const BVec3f BComponent::GetScale() const
	{
		return GetTransform().scale;
	}

	void BComponent::SetTransform( const BTransform< float >& transform )
	{
		GetTransform_Mutable() = transform;
	}

	void BComponent::SetLocation( const BVec3f& location )
	{
		GetTransform_Mutable().translation = location;
	}

	void BComponent::SetRotation( const BRotf& rotation )
	{
		GetTransform_Mutable().rotation = rotation;
	}

	void BComponent::SetScale( const BVec3f& scale )
	{
		GetTransform_Mutable().scale = scale;
	}

	BCameraComponent::BCameraComponent( BNode* owner, const char* name, BViewport* viewport )
		: BComponent( owner, name )
	{
		m_camera = std::make_unique< BCamera >( viewport, this );
		BCameraManager::AddCamera( m_camera.get() );
	}

	BCameraComponent::~BCameraComponent()
	{
		BCameraManager::RemoveCamera( m_camera.get() );
	}

	BViewport* BCameraComponent::GetViewport() const
	{
		if( m_camera )
		{
			return m_camera->GetViewport();
		}

		return nullptr;
	}

	void BCameraComponent::SetViewport( BViewport* viewport )
	{
		if( m_camera )
		{
			m_camera->SetViewport( viewport );
		}
	}

	BCamera* BCameraComponent::GetCamera() const
	{
		return m_camera.get();
	}

} // namespace bgl