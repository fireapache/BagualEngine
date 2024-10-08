
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "Scene.h"

#include "BagualEngine.h"
#include "Camera.h"
#include "CameraManager.h"
#include "GraphicsPlatform.h"
#include "Module.h"
#include "Viewport.h"
#include <obj_parse.h>

#include <bvh/v2/bvh.h>
#include <bvh/v2/default_builder.h>
#include <bvh/v2/thread_pool.h>
#include <bvh/v2/tri.h>

namespace bgl
{

	BArray< BArray< BTriangle< float > >* > BMeshComponent::g_meshComponentTriangles;
	BArray< BMeshComponent* > BMeshComponent::g_meshComponents;

	BNode::BNode( BScene* scene, BNode* parent, const char* name, BModule* owningModule )
		: BSceneObject( scene, name, owningModule )
	{
		m_parent = parent;

		if( m_module )
		{
			m_module->getNodes().add( this );
		}

		if( BSettings::isDebugFlagsSet( DBF_SceneConstruction ) )
		{
			std::cout << "BNode: " << getName() << std::endl;
		}
	}

	BNode::~BNode()
	{
		if( m_module )
		{
			m_module->getNodes().remove( this );
		}

		if( BSettings::isDebugFlagsSet( DBF_SceneConstruction ) )
		{
			std::cout << "~BNode: " << getName() << std::endl;
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
				allChildren.add( node->getChildren( true ) );
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
				allComponents.add( childNode->getComponents() );
			}

			return allComponents;
		}

		return m_components;
	}

	bool BNode::hasChildren() const
	{
		return m_children.size() > 0;
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
		if( m_bHidden != bHidden )
		{
			m_bHidden = bHidden;
			setSceneDirty();
		}
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
		m_children.add( node );
	}

	void BNode::removeChild( BNode* node )
	{
		m_children.remove( node );
	}

	BMeshComponent::BMeshComponent( BScene* scene, BNode* owner, BModule* module, const char* name, const char* assetPath )
		: BComponent( scene, owner, module, name )
		, rtcGeometry( nullptr )
		, rtxGeomID( RTC_INVALID_GEOMETRY_ID )
		, m_showWireframe( false )
	{
		m_scene = scene;
		g_meshComponentTriangles.add( &m_meshData.triangles );
		g_meshComponents.add( this );
		if( assetPath )
		{
			LoadMesh( assetPath );
			setSceneDirty();
		}
	}

	BMeshComponent::~BMeshComponent()
	{
		g_meshComponentTriangles.remove( &m_meshData.triangles );
		g_meshComponents.remove( this );
		rtcReleaseGeometry( rtcGeometry );
		setSceneDirty();
	}

	void BMeshComponent::addUniqueEdge( const BLine< BVec3f >& line )
	{
		auto& edges = m_meshData.edges;
		bool bUnique = true;
		for( auto* curEdge = edges.data(); curEdge < edges.data() + edges.size(); curEdge++ )
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

		const objl::Loader triLoader( assetPath );

		rtcGeometry = rtcNewGeometry( BScene::rtcDevice, RTC_GEOMETRY_TYPE_TRIANGLE );
		auto rtcVertices = 
			static_cast< float* >( rtcSetNewGeometryBuffer(
				rtcGeometry,
				RTC_BUFFER_TYPE_VERTEX,
				0,
				RTC_FORMAT_FLOAT3,
				3 * sizeof( float ),
				triLoader.LoadedVertices.size() ) );

		for( size_t i = 0; i < triLoader.LoadedVertices.size(); ++i )
		{
			float* x = rtcVertices + ( 3 * i );
			//float* y = x + 1;
			//float* z = x + 2;
			const auto& loadedVertex = triLoader.LoadedVertices[ i ];
			//*x = loadedVertex.Position.X;
			//*y = loadedVertex.Position.Y;
			//*z = loadedVertex.Position.Z;
			memcpy( x, &( loadedVertex.Position.X ), 3 * sizeof( float ) );
		}

		unsigned* rtcIndices = 
			static_cast< unsigned* >( rtcSetNewGeometryBuffer(
				rtcGeometry,
				RTC_BUFFER_TYPE_INDEX, 
				0,
				RTC_FORMAT_UINT3,
				3 * sizeof( unsigned ),
				triLoader.LoadedIndices.size() ) );

		memcpy( rtcIndices, triLoader.LoadedIndices.data(), triLoader.LoadedIndices.size() * sizeof( unsigned ) );

		rtcCommitGeometry( rtcGeometry );
		rtcAttachGeometry( m_scene->rtcScene, rtcGeometry );

		for( size_t i = 0; i < triLoader.LoadedIndices.size(); i += 3 )
		{
			const uint32_t index0 = triLoader.LoadedIndices[ i ];
			const uint32_t index1 = triLoader.LoadedIndices[ i + 1 ];
			const uint32_t index2 = triLoader.LoadedIndices[ i + 2 ];

			const objl::Vertex vert0 = triLoader.LoadedVertices[ index0 ];
			const objl::Vertex vert1 = triLoader.LoadedVertices[ index1 ];
			const objl::Vertex vert2 = triLoader.LoadedVertices[ index2 ];

			BTriangle< float > triCache;
			triCache.v0.x = vert0.Position.X;
			triCache.v0.y = vert0.Position.Y;
			triCache.v0.z = vert0.Position.Z;
			triCache.v1.x = vert1.Position.X;
			triCache.v1.y = vert1.Position.Y;
			triCache.v1.z = vert1.Position.Z;
			triCache.v2.x = vert2.Position.X;
			triCache.v2.y = vert2.Position.Y;
			triCache.v2.z = vert2.Position.Z;

			m_meshData.triangles.add( triCache );

			addUniqueTriEdges( triCache );

			m_meshData.triangles_SIMD.v0.x.add( triCache.v0.x );
			m_meshData.triangles_SIMD.v0.y.add( triCache.v0.y );
			m_meshData.triangles_SIMD.v0.z.add( triCache.v0.z );
			m_meshData.triangles_SIMD.v1.x.add( triCache.v1.x );
			m_meshData.triangles_SIMD.v1.y.add( triCache.v1.y );
			m_meshData.triangles_SIMD.v1.z.add( triCache.v1.z );
			m_meshData.triangles_SIMD.v2.x.add( triCache.v2.x );
			m_meshData.triangles_SIMD.v2.y.add( triCache.v2.y );
			m_meshData.triangles_SIMD.v2.z.add( triCache.v2.z );
		}

		const size_t simdFillerCount = ( 8 - m_meshData.triangles_SIMD.v0.x.size() % 8 ) % 8;

		for( size_t i = 0; i < simdFillerCount; i++ )
		{
			constexpr float dummy = 0.f;
			m_meshData.triangles_SIMD.v0.x.add( dummy );
			m_meshData.triangles_SIMD.v0.y.add( dummy );
			m_meshData.triangles_SIMD.v0.z.add( dummy );
			m_meshData.triangles_SIMD.v1.x.add( dummy );
			m_meshData.triangles_SIMD.v1.y.add( dummy );
			m_meshData.triangles_SIMD.v1.z.add( dummy );
			m_meshData.triangles_SIMD.v2.x.add( dummy );
			m_meshData.triangles_SIMD.v2.y.add( dummy );
			m_meshData.triangles_SIMD.v2.z.add( dummy );
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
		if( m_showWireframe != bValue )
		{
			m_showWireframe = bValue;
			setSceneDirty();
		}
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
		m_meshData.triangles.add( triangles );

		for( auto& tri : triangles )
		{
			addUniqueTriEdges( tri );

			m_meshData.triangles_SIMD.v0.x.add( tri.v0.x );
			m_meshData.triangles_SIMD.v0.y.add( tri.v0.y );
			m_meshData.triangles_SIMD.v0.z.add( tri.v0.z );

			m_meshData.triangles_SIMD.v1.x.add( tri.v1.x );
			m_meshData.triangles_SIMD.v1.y.add( tri.v1.y );
			m_meshData.triangles_SIMD.v1.z.add( tri.v1.z );

			m_meshData.triangles_SIMD.v2.x.add( tri.v2.x );
			m_meshData.triangles_SIMD.v2.y.add( tri.v2.y );
			m_meshData.triangles_SIMD.v2.z.add( tri.v2.z );
		}

		rtcGeometry = rtcNewGeometry( BScene::rtcDevice, RTC_GEOMETRY_TYPE_TRIANGLE );
		auto rtcVertices = 
			static_cast< float* >( rtcSetNewGeometryBuffer(
				rtcGeometry,
				RTC_BUFFER_TYPE_VERTEX,
				0,
				RTC_FORMAT_FLOAT3,
				3 * sizeof( float ),
				m_meshData.triangles.size() * 3 ) );

		unsigned* rtcIndices = 
			static_cast< unsigned* >( rtcSetNewGeometryBuffer(
				rtcGeometry,
				RTC_BUFFER_TYPE_INDEX, 
				0,
				RTC_FORMAT_UINT3,
				3 * sizeof( unsigned ),
				m_meshData.triangles.size() * 3 ) );

		for( unsigned i = 0; i < m_meshData.triangles.size(); ++i )
		{
			float* x = rtcVertices + ( 9 * i );
			const auto& loadedVertex = m_meshData.triangles[ i ];
			memcpy( x, &( loadedVertex.v0.x ), 9 * sizeof( float ) );
			auto indexPtr = rtcIndices + 9 * i;
			unsigned indexes[ 9 ] = { 9 * i, 9 * i + 1, 9 * i + 2, 9 * i + 3, 9 * i + 4, 9 * i + 5, 9 * i + 6, 9 * i + 7, 9 * i + 8 };
			memcpy( indexPtr, indexes, 9 * sizeof( float ) );
		}
		
		rtcCommitGeometry( rtcGeometry );
		rtcAttachGeometry( m_scene->rtcScene, rtcGeometry );
		setSceneDirty();
	}

	BScene::BScene()
	{
		m_sceneRoot = std::make_unique< BNode >( this, nullptr, "Scene Root" );

		if( !rtcDevice )
		{
			rtcDevice = rtcNewDevice( nullptr );
		}

		rtcScene = rtcNewScene( rtcDevice );
	}

	BScene::~BScene()
	{
		for( const auto node : m_nodes )
		{
			delete node;
		}

		for( const auto component : m_components )
		{
			delete component;
		}

		rtcReleaseScene( rtcScene );
	}

	RTCDevice BScene::rtcDevice = nullptr;

	void BScene::update()
	{
		// deleting unused render stages
		const auto [ first, last ] = std::ranges::remove_if(
			renderStages,
			[]( const BRenderStage* rs ) { return rs->state == BRenderStage::State::Old; } );
		renderStages.erase( first, last );

		if( m_bDirty )
		{
			auto* renderStage = new BRenderStage();
			renderStages.add( renderStage );

			auto& tris = renderStage->triangles;
			auto& trisSIMD = renderStage->triangles_SIMD;
			auto& edges = renderStage->edges;
			renderStage->rtcScene = rtcScene;
			
			for( const auto meshComp : BMeshComponent::g_meshComponents )
			{
				if( !meshComp )
				{
					continue;
				}

				if( !meshComp->isVisible() )
				{
					continue;
				}

				auto& compTris = meshComp->getTriangles();
				auto& compTrisSIMD = meshComp->getTriangles_SIMD();
				auto& compEdges = meshComp->getMeshData().edges;

				tris.insert( tris.end(), compTris.begin(), compTris.end() );

				trisSIMD.v0.x.insert( trisSIMD.v0.x.end(), compTrisSIMD.v0.x.begin(), compTrisSIMD.v0.x.end() );
				trisSIMD.v0.y.insert( trisSIMD.v0.y.end(), compTrisSIMD.v0.y.begin(), compTrisSIMD.v0.y.end() );
				trisSIMD.v0.z.insert( trisSIMD.v0.z.end(), compTrisSIMD.v0.z.begin(), compTrisSIMD.v0.z.end() );

				trisSIMD.v1.x.insert( trisSIMD.v1.x.end(), compTrisSIMD.v1.x.begin(), compTrisSIMD.v1.x.end() );
				trisSIMD.v1.y.insert( trisSIMD.v1.y.end(), compTrisSIMD.v1.y.begin(), compTrisSIMD.v1.y.end() );
				trisSIMD.v1.z.insert( trisSIMD.v1.z.end(), compTrisSIMD.v1.z.begin(), compTrisSIMD.v1.z.end() );

				trisSIMD.v2.x.insert( trisSIMD.v2.x.end(), compTrisSIMD.v2.x.begin(), compTrisSIMD.v2.x.end() );
				trisSIMD.v2.y.insert( trisSIMD.v2.y.end(), compTrisSIMD.v2.y.begin(), compTrisSIMD.v2.y.end() );
				trisSIMD.v2.z.insert( trisSIMD.v2.z.end(), compTrisSIMD.v2.z.begin(), compTrisSIMD.v2.z.end() );

				if( !meshComp->getShowWireframe() )
				{
					continue;
				}

				for( auto& compEdge : compEdges )
				{
					edges.emplace_back( BRenderStage::EdgeData{ compEdge, meshComp->getColor().getRGB() } );
				}
			}
			
#pragma region ========== BVH V2 Code ==========
			if( !tris.empty() )
			{
				using Scalar = float;
				using Vec3 = bvh::v2::Vec< Scalar, 3 >;
				using Box = bvh::v2::BBox< Scalar, 3 >;
				using Tri = bvh::v2::Tri< Scalar, 3 >;
				using Node = bvh::v2::Node< Scalar, 3 >;
				using Bvh = bvh::v2::Bvh< Node >;
				using Ray = bvh::v2::Ray< Scalar, 3 >;

				using PrecomputedTri = bvh::v2::PrecomputedTri<Scalar>;

				// This is the original data, which may come in some other data type/structure.
				std::vector< Tri > bvhTris;

				for( auto& tri : tris )
				{
					bvhTris.emplace_back(
						Tri{
							Vec3{ tri.v0.x, tri.v0.y, tri.v0.z },
							Vec3{ tri.v1.x, tri.v1.y, tri.v1.z },
							Vec3{ tri.v2.x, tri.v2.y, tri.v2.z }
						} );
				}

				bvh::v2::ThreadPool thread_pool;
				bvh::v2::ParallelExecutor executor( thread_pool );

				// Get triangle centers and bounding boxes (required for BVH builder)
				std::vector< Box > boxes( bvhTris.size() );
				std::vector< Vec3 > centers( bvhTris.size() );
				executor.for_each(
					0,
					bvhTris.size(),
					[ & ]( size_t begin, size_t end )
					{
						for( size_t i = begin; i < end; ++i )
						{
							boxes[ i ] = bvhTris[ i ].get_bbox();
							centers[ i ] = bvhTris[ i ].get_center();
						}
					} );

				typename bvh::v2::DefaultBuilder< Node >::Config config;
				config.quality = bvh::v2::DefaultBuilder< Node >::Quality::High;
				renderStage->bvh = bvh::v2::DefaultBuilder< Node >::build( thread_pool, boxes, centers, config );

				// Permuting the primitive data allows to remove indirections during traversal, which makes it faster.
				const bool should_permute = bPermuteTris;

				if( bPrecomputeTris )
				{
					// This precomputes some data to speed up traversal further.
					renderStage->bvh.precomputed_tris.reserve( bvhTris.size() );
					executor.for_each(
						0,
						bvhTris.size(),
						[ & ]( size_t begin, size_t end )
						{
							for( size_t i = begin; i < end; ++i )
							{
								auto j = should_permute ? renderStage->bvh.prim_ids[ i ] : i;
								renderStage->bvh.precomputed_tris[ i ] = bvhTris[ j ];
							}
						} );
					renderStage->bvh.bPermuted = should_permute;
					renderStage->bvh.bPrecomputed = true;
				}
			}
#pragma endregion ========== BVH V2 Code ==========

#pragma region ========== Embree Code ==========

			rtcCommitScene( rtcScene );

#pragma endregion ========== Embree Code ==========

			m_bDirty = false;
		}
	}

	BRenderStage* BScene::getNextRenderStage()
	{
		BRenderStage* selectedRenderStage{ nullptr };

		// finds the latest New
		for( uint32_t i = 0; i < renderStages.size(); ++i )
		{
			if( renderStages[ i ]->state == BRenderStage::State::New )
			{
				selectedRenderStage = renderStages[ i ];
			}
		}

		// lets stick with the last Claimed
		if( !selectedRenderStage )
		{
			for( uint32_t i = 0; i < renderStages.size(); ++i )
			{
				if( renderStages[ i ]->state == BRenderStage::State::Claimed )
				{
					selectedRenderStage = renderStages[ i ];
				}
			}
		}
		else
		{
			// ok we have the latest New, lets stage it
			selectedRenderStage->state = BRenderStage::State::Staged;

			// marking all others as Old
			for( uint32_t i = 0; i < renderStages.size(); ++i )
			{
				if( renderStages[ i ]->state != BRenderStage::State::Staged )
				{
					renderStages[ i ]->state = BRenderStage::State::Old;
				}
			}

			// finally claiming the one selected
			selectedRenderStage->state = BRenderStage::State::Claimed;
		}
		
		return selectedRenderStage;
	}

	BNode* BScene::addNode( const char* name /*= "None"*/ )
	{
		return addNode( m_sceneRoot.get(), name );
	}

	BNode* BScene::addNode( BNode* parent, const char* name /*= "None"*/ )
	{
		if( parent == nullptr )
		{
			return nullptr;
		}

		BModule* moduleContext = BEngine::Instance().getModuleContext();
		BNode* newNodePtr = new BNode( this, parent, name, moduleContext );
		m_nodes.push_back( newNodePtr );
		parent->addChild( newNodePtr );
		if( moduleContext )
		{
			moduleContext->getNodes().add( newNodePtr );
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
				allComponents.add( childNode->getComponents() );
				m_nodes.remove( childNode );
				delete childNode;
			}

			for( const auto component : allComponents )
			{
				m_components.remove( component );
				delete component;
			}

			m_nodes.remove( node );
		}
	}

	void BSceneObject::setSceneDirty() const
	{
		if( m_scene )
		{
			m_scene->setSceneDirty( true );
		}
	}

	BComponent::BComponent( BScene* scene, BNode* owner, BModule* module, const char* name )
		: BSceneObject( scene, name, module )
	{
		setOwner( owner );

		if( BSettings::isDebugFlagsSet( DBF_SceneConstruction ) )
		{
			std::cout << "BComponent: " << getName() << std::endl;
		}
	}

	bool BComponent::isVisible() const
	{
		if( m_module && m_module->isHidden() )
		{
			return false;
		}

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

	BTransform< float > BComponent::getTransform() const
	{
		if( m_owner )
		{
			return m_owner->getTransform();
		}

		return BTransform< float >();
	}

	BVec3f BComponent::getLocation() const
	{
		return getTransform().translation;
	}

	BRotf BComponent::getRotation() const
	{
		return getTransform().rotation;
	}

	BVec3f BComponent::getScale() const
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

	BCameraComponent::BCameraComponent( BScene* scene, BNode* owner, BModule* module, const char* name, BViewport* viewport )
		: BComponent( scene, owner, module, name )
	{
		m_camera = new BCamera( viewport, this );
		BCameraManager::AddCamera( m_camera );
	}

	BCameraComponent::~BCameraComponent()
	{
		BCameraManager::RemoveCamera( m_camera );
		delete m_camera;
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
		return m_camera;
	}

} // namespace bgl