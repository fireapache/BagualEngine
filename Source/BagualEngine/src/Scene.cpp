
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "Scene.h"

#include "BagualEngine.h"
#include "Camera.h"
#include "CameraManager.h"
#include "Module.h"
#include "Viewport.h"
#include <obj_parse.h>

namespace bgl
{

	BArray< BArray< BTriangle< float > >* > BMeshComponent::g_meshComponentTriangles;
	BArray< BMeshComponent* > BMeshComponent::g_meshComponents;

	BNode::BNode( BNode* parent, const char* name, BModule* owningModule )
	{
		m_parent = parent;
		m_module = owningModule;

		if( name )
		{
			this->m_name = std::string( name );
		}
	}

	BNode* BNode::getParent()
	{
		return m_parent;
	}

	BArray< BNode* > BNode::getChildren( const bool recursive ) const
	{
		if( recursive )
		{
			BArray< BNode* > allChildren{ m_children };
			for( const auto& node : m_children )
			{
				allChildren.Add( node->getChildren( true ) );
			}
			return allChildren;
		}

		return m_children;
	}

	BArray< BComponent* > BNode::getComponents( bool recursive ) const
	{
		if( recursive )
		{
			const BArray< BNode* > childNodes = getChildren( true );
			BArray< BComponent* > allComponents = m_components;

			for( auto& childNode : childNodes )
			{
				allComponents.Add( childNode->getComponents() );
			}

			return allComponents;
		}

		return m_components;
	}

	bool BNode::hasChildren() const
	{
		return m_children.Size() > 0;
	}

	bool BNode::isVisible() const
	{
		const bool bModuleHidden = m_module ? m_module->isHidden() : false;
		const bool bHidden = !m_bHidden && !bModuleHidden;

		if( m_parent )
		{
			return m_parent->isVisible() && bHidden;
		}

		return bHidden;
	}

	void BNode::setHidden( bool bHidden )
	{
		m_bHidden = bHidden;
	}

	BTransform< float >& BNode::getTransform_mutable()
	{
		return m_transform;
	}

	const BTransform< float > BNode::getTransform() const
	{
		return m_transform;
	}

	const BVec3f BNode::getLocation() const
	{
		return m_transform.translation;
	}

	const BRotf BNode::getRotation() const
	{
		return m_transform.rotation;
	}

	const BVec3f BNode::getScale() const
	{
		return m_transform.scale;
	}

	void BNode::setTransform( const BTransform< float >& transform )
	{
		m_transform = transform;
	}

	void BNode::setLocation( const BVec3f& location )
	{
		m_transform.translation = location;
	}

	void BNode::setRotation( const BRotf& rotation )
	{
		m_transform.rotation = rotation;
	}

	void BNode::setScale( const BVec3f& scale )
	{
		m_transform.scale = scale;
	}

	void BNode::setParent( BNode* node )
	{
		if( m_parent )
		{
			m_parent->removeChild( this );
		}

		m_parent = node;

		if( m_parent )
		{
			node->addChild( this );
		}
	}

	void BNode::addChild( BNode* node )
	{
		m_children.Add( node );
	}

	void BNode::removeChild( BNode* node )
	{
		m_children.Remove( node );
	}

	BMeshComponent::BMeshComponent(
		class BNode* owner /*= nullptr*/,
		const char* name /*= "None"*/,
		const char* assetPath /*= nullptr*/ )
		: BComponent( owner, name )
	{
		g_meshComponentTriangles.Add( &m_meshData.triangles );
		g_meshComponents.Add( this );
		if( assetPath )
			LoadMesh( assetPath );
	}

	BMeshComponent::~BMeshComponent()
	{
		g_meshComponentTriangles.Remove( &m_meshData.triangles );
		g_meshComponents.Remove( this );
	}

	void BMeshComponent::addUniqueEdge( const BLine< BVec3f >& line )
	{
		auto& edges = m_meshData.edges;
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
		auto& edges = m_meshData.edges;
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

			m_meshData.triangles.Add( triCache );

			addUniqueTriEdges( triCache );

			m_meshData.triangles_SIMD.v0.x.Add( triCache.v0.x );
			m_meshData.triangles_SIMD.v0.y.Add( triCache.v0.y );
			m_meshData.triangles_SIMD.v0.z.Add( triCache.v0.z );
			m_meshData.triangles_SIMD.v1.x.Add( triCache.v1.x );
			m_meshData.triangles_SIMD.v1.y.Add( triCache.v1.y );
			m_meshData.triangles_SIMD.v1.z.Add( triCache.v1.z );
			m_meshData.triangles_SIMD.v2.x.Add( triCache.v2.x );
			m_meshData.triangles_SIMD.v2.y.Add( triCache.v2.y );
			m_meshData.triangles_SIMD.v2.z.Add( triCache.v2.z );
		}

		const size_t simdFillerCount = ( 8 - m_meshData.triangles_SIMD.v0.x.Size() % 8 ) % 8;

		for( size_t i = 0; i < simdFillerCount; i++ )
		{
			constexpr float dummy = 0.f;
			m_meshData.triangles_SIMD.v0.x.Add( dummy );
			m_meshData.triangles_SIMD.v0.y.Add( dummy );
			m_meshData.triangles_SIMD.v0.z.Add( dummy );
			m_meshData.triangles_SIMD.v1.x.Add( dummy );
			m_meshData.triangles_SIMD.v1.y.Add( dummy );
			m_meshData.triangles_SIMD.v1.z.Add( dummy );
			m_meshData.triangles_SIMD.v2.x.Add( dummy );
			m_meshData.triangles_SIMD.v2.y.Add( dummy );
			m_meshData.triangles_SIMD.v2.z.Add( dummy );
		}
	}

	bool BMeshComponent::getShowWireframe() const
	{
		return m_showWireframe;
	}

	bool& BMeshComponent::getShowWireframe_Mutable()
	{
		return m_showWireframe;
	}

	void BMeshComponent::setShowWireframe( const bool bValue )
	{
		m_showWireframe = bValue;
	}

	bgl::BArray< bgl::BTriangle< float > >& BMeshComponent::getTriangles()
	{
		return m_meshData.triangles;
	}

	BTriangle< BArray< float > >& BMeshComponent::getTriangles_SIMD()
	{
		return m_meshData.triangles_SIMD;
	}

	void BMeshComponent::addTriangles( const BArray< BTriangle< float > >& triangles )
	{
		m_meshData.triangles.Add( triangles );

		for( auto& tri : triangles )
		{
			addUniqueTriEdges( tri );

			m_meshData.triangles_SIMD.v0.x.Add( tri.v0.x );
			m_meshData.triangles_SIMD.v0.y.Add( tri.v0.y );
			m_meshData.triangles_SIMD.v0.z.Add( tri.v0.z );

			m_meshData.triangles_SIMD.v1.x.Add( tri.v1.x );
			m_meshData.triangles_SIMD.v1.y.Add( tri.v1.y );
			m_meshData.triangles_SIMD.v1.z.Add( tri.v1.z );

			m_meshData.triangles_SIMD.v2.x.Add( tri.v2.x );
			m_meshData.triangles_SIMD.v2.y.Add( tri.v2.y );
			m_meshData.triangles_SIMD.v2.z.Add( tri.v2.z );
		}
	}

	BScene::BScene()
	{
		m_sceneRoot = std::make_unique< BNode >( nullptr, "Scene Root" );
	}

	BNode* BScene::addNode( const char* name /*= "None"*/ )
	{
		return addNode( *m_sceneRoot, name );
	}

	BNode* BScene::addNode( BNode& parent, const char* name /*= "None"*/ )
	{
		BModule* moduleContext = BEngine::Instance().getModuleContext();
		BNode* newNodePtr = new BNode( &parent, name, moduleContext );
		m_nodes.push_back( newNodePtr );
		parent.addChild( newNodePtr );
		if( moduleContext )
		{
			moduleContext->getNodes().Add( newNodePtr );
		}
		return newNodePtr;
	}

	void BScene::deleteNode( BNode* node )
	{
		if( node )
		{
			if( BNode* parentNode = node->getParent() )
			{
				parentNode->removeChild( node );
			}

			node->setParent( nullptr );

			BArray< BComponent* > allComponents = node->getComponents();

			const auto childNodes = node->getChildren( true );

			for( const auto childNode : childNodes )
			{
				allComponents.Add( childNode->getComponents() );
				m_nodes.Remove( childNode );
				delete childNode;
			}

			for( const auto component : allComponents )
			{
				m_components.Remove( component );
				delete component;
			}

			m_nodes.Remove( node );
		}
	}

	BComponent::BComponent( BNode* owner, const char* name )
	{
		setOwner( owner );
		m_name = std::string( name );
	}
	
	const bool BComponent::isVisible() const
	{
		if( m_owner )
		{
			return m_owner->isVisible() && !m_bHidden;
		}

		return !m_bHidden;
	}

	void BComponent::setOwner( BNode* owner )
	{
		m_owner = owner;
	}

	BTransform< float >& BComponent::getTransform_mutable()
	{
		if( m_owner )
		{
			return m_owner->getTransform_mutable();
		}
		else
		{
			BGL_LOG( "Got null owner when returning mutable transform!" );
		}

		return m_dummyTransform;
	}

	const BTransform< float > BComponent::getTransform() const
	{
		if( m_owner )
		{
			return m_owner->getTransform();
		}

		return BTransform< float >();
	}

	const BVec3f BComponent::getLocation() const
	{
		return getTransform().translation;
	}

	const BRotf BComponent::getRotation() const
	{
		return getTransform().rotation;
	}

	const BVec3f BComponent::getScale() const
	{
		return getTransform().scale;
	}

	void BComponent::setTransform( const BTransform< float >& transform )
	{
		getTransform_mutable() = transform;
	}

	void BComponent::setLocation( const BVec3f& location )
	{
		getTransform_mutable().translation = location;
	}

	void BComponent::setRotation( const BRotf& rotation )
	{
		getTransform_mutable().rotation = rotation;
	}

	void BComponent::setScale( const BVec3f& scale )
	{
		getTransform_mutable().scale = scale;
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

	BViewport* BCameraComponent::getViewport() const
	{
		if( m_camera )
		{
			return m_camera->GetViewport();
		}

		return nullptr;
	}

	void BCameraComponent::setViewport( BViewport* viewport )
	{
		if( m_camera )
		{
			m_camera->SetViewport( viewport );
		}
	}

	BCamera* BCameraComponent::getCamera() const
	{
		return m_camera.get();
	}

} // namespace bgl