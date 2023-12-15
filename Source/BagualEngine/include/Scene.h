#pragma once

#include "BagualEngine.h"
#include "Module.h"
#include "Settings.h"
#include "Types.h"
#include <bvh/v2/bvh.h>
#include <memory>
#include <mutex>
#include <semaphore>
#include <embree4/rtcore.h>

namespace bgl
{
	class BScene;
	class BNode;
	class BViewport;
	class BCamera;
	class BModule;
} // namespace bgl

namespace bgl
{
	class BObject
	{
	protected:
		std::string m_name;

		BModule* m_module{ nullptr };

	public:
		BObject( const char* name = "None", BModule* module = nullptr )
			: m_module( module )
		{
			m_name = std::string( name );
		}

		[[nodiscard]] const std::string& getName() const
		{
			return m_name;
		}

		void setName( const std::string& name )
		{
			m_name = name;
		}

		void setName( const char* name )
		{
			if( name )
			{
				m_name = std::string( name );
			}
		}

		BModule* getModule() const
		{
			return m_module;
		}
	};

	class BSceneObject : public BObject
	{
		friend class BModule;

	protected:
		BScene* m_scene{ nullptr };

		void setSceneDirty() const;

	public:
		BSceneObject( BScene* scene, const char* name = "None", BModule* module = nullptr )
			: BObject( name, module )
			, m_scene( scene )
		{
		}
	};

	class BComponent : public BSceneObject
	{
		BTransform< float > m_dummyTransform;

		bool m_bHidden{ false };

	protected:
		BNode* m_owner{ nullptr };

	public:
		BComponent() = delete;
		BComponent( BScene* scene, BNode* owner, BModule* module = nullptr, const char* name = "None" );
		virtual ~BComponent()
		{
			if( m_module )
			{
				m_module->getComponents().remove( this );
			}

			if( BSettings::isDebugFlagsSet( DBF_SceneConstruction ) )
			{
				std::cout << "~BComponent: " << getName() << std::endl;
			}
		}

		[[nodiscard]] bool isVisible() const;
		void setOwner( BNode* owner );

		[[nodiscard]] BTransform< float >& getTransform_mutable();
		[[nodiscard]] BTransform< float > getTransform() const;
		[[nodiscard]] BVec3f getLocation() const;
		[[nodiscard]] BRotf getRotation() const;
		[[nodiscard]] BVec3f getScale() const;

		void setTransform( const BTransform< float >& transform );
		void setLocation( const BVec3f& locations );
		void setRotation( const BRotf& rotation );
		void setScale( const BVec3f& scale );
	};

	class BRawMeshData
	{
	public:
		BArray< BTriangle< float > > triangles;
		BTriangle< BArray< float > > triangles_SIMD;
		BArray< BLine< BVec3f > > edges;
	};

	class BMeshComponent : public BComponent
	{
		friend class BScene;
		friend class BGraphicsDriverGeneric;

		static BArray< BArray< BTriangle< float > >* > g_meshComponentTriangles;
		static BArray< BMeshComponent* > g_meshComponents;

	protected:
		void addUniqueTriEdges( const BTriangle< float >& tri );
		void addUniqueEdge( const BLine< BVec3f >& line );

		//BRawMeshData m_sourceMeshData;
		BRawMeshData m_meshData;

		bool m_showWireframe{ false };

		BColor color;

		RTCGeometry rtcGeometry;
		unsigned rtxGeomID = RTC_INVALID_GEOMETRY_ID;

	public:
		BMeshComponent(
			BScene* scene,
			BNode* owner,
			BModule* module,
			const char* name = "None",
			const char* assetPath = nullptr );
		~BMeshComponent() override;

		void LoadMesh( const char* assetPath );

		[[nodiscard]] bool getShowWireframe() const;
		bool& getShowWireframe_Mutable();
		void setShowWireframe( const bool bValue );

		[[nodiscard]] const BColor& getColor() const
		{
			return color;
		}
		BColor& getColorMutable()
		{
			return color;
		}
		void setColor( const BColor& newColor )
		{
			this->color = newColor;
		}

		BArray< BTriangle< float > >& getTriangles();
		BTriangle< BArray< float > >& getTriangles_SIMD();

		[[nodiscard]] const BRawMeshData& getMeshData() const
		{
			return m_meshData;
		}

		void addTriangles( const BArray< BTriangle< float > >& triangles );
	};

	class BCameraComponent : public BComponent
	{
		BCamera* m_camera;

	public:
		BCameraComponent(
			BScene* scene,
			BNode* owner,
			BModule* module = nullptr,
			const char* name = "None",
			BViewport* viewport = nullptr );
		~BCameraComponent();

		[[nodiscard]] BViewport* getViewport() const;
		void setViewport( BViewport* viewport );

		[[nodiscard]] BCamera* getCamera() const;
	};

	class BNode : public BSceneObject
	{
		friend class BScene;

	protected:
		BNode* m_parent{ nullptr };
		BArray< BNode* > m_children;
		BArray< BComponent* > m_components;

		BTransform< float > m_transform;

		bool m_bHidden{ false };

	public:
		BNode( BScene* scene, BNode* parent, const char* name, BModule* owningModule = nullptr );
		~BNode();

		BNode* getParent();
		[[nodiscard]] BArray< BNode* > getChildren( bool recursive = false ) const;
		[[nodiscard]] BArray< BComponent* > getComponents( bool recursive = false ) const;

		[[nodiscard]] bool hasChildren() const;

		[[nodiscard]] BComponent* getComponent( const char* componentName ) const
		{
			for( const auto component : m_components )
			{
				if( std::strcmp( componentName, component->getName().c_str() ) == 0 )
				{
					return component;
				}
			}

			return nullptr;
		}

		[[nodiscard]] bool isVisible() const;
		void setHidden( bool bHidden );

		BTransform< float >& getTransform_mutable();
		[[nodiscard]] const BTransform< float > getTransform() const;
		[[nodiscard]] const BVec3f getLocation() const;
		[[nodiscard]] const BRotf getRotation() const;
		[[nodiscard]] const BVec3f getScale() const;

		void setTransform( const BTransform< float >& transform );
		void setLocation( const BVec3f& location );
		void setRotation( const BRotf& rotation );
		void setScale( const BVec3f& scale );

		void setParent( BNode* node );
		void addChild( BNode* node );
		void removeChild( BNode* node );
	};

	class BRenderStage
	{
	public:
		BArray< BTriangle< float > > triangles;
		BTriangle< BArray< float > > triangles_SIMD;
		bvh::v2::Bvh< bvh::v2::Node< float, 3 > > bvh;
		RTCScene rtcScene;

		class EdgeData
		{
		public:
			BLine< BVec3f > line3D;
			Color32Bit color{ BSettings::lineColor };
		};

		BArray< EdgeData > edges;

		enum class State : uint8_t
		{
			New,
			Staged,
			Claimed,
			Old
		};

		State state{ State::New };
	};

	class BScene
	{
		friend class BSceneObject;

		std::unique_ptr< BNode > m_sceneRoot;
		BArray< BNode* > m_nodes;
		BArray< BComponent* > m_components;

		bool m_bDirty{ false };

	public:
		BScene();

		~BScene();

		static RTCDevice rtcDevice;
		RTCScene rtcScene = nullptr;

		void update();

		BRenderStage* getNextRenderStage();

		BArray< BRenderStage* > renderStages;

		std::counting_semaphore< 2 > sceneSemaphore{ 2 };

		bool bPermuteTris{ true };
		bool bPrecomputeTris{ true };

		static BArray< BArray< BTriangle< float > >* >& getMeshComponentTriangles()
		{
			return BMeshComponent::g_meshComponentTriangles;
		}

		static BArray< BMeshComponent* >& getMeshComponents()
		{
			return BMeshComponent::g_meshComponents;
		}

		template< class T, typename... P >
		T* addComponent( BNode* node, P... args )
		{
			static_assert( std::is_base_of< BComponent, T >::value, "Type should inherit from BComponent" );
			auto* moduleContext = BEngine::Instance().getModuleContext();
			T* newComponent = new T( this, node, moduleContext, args... );
			m_components.push_back( newComponent );
			node->m_components.add( newComponent );
			BComponent* comp = dynamic_cast< BComponent* >( newComponent );
			if( comp )
			{
				if( BModule* module = comp->getModule() )
				{
					module->getComponents().add( comp );
				}
			}
			return newComponent;
		}

		void setSceneDirty( bool bValue )
		{
			m_bDirty = bValue;
		}

		BNode* addNode( const char* name = "None" );
		BNode* addNode( BNode& parent, const char* name = "None" );

		void deleteNode( BNode* node );

		BNode* getRootNode() const
		{
			return m_sceneRoot.get();
		}

		BArray< BNode* >& getNodes()
		{
			return m_nodes;
		}

		BNode* getNode( const char* nodeName ) const
		{
			for( BNode* node : m_nodes )
			{
				if( std::strcmp( nodeName, node->getName().c_str() ) == 0 )
				{
					return node;
				}
			}

			return nullptr;
		}
	};
} // namespace bgl