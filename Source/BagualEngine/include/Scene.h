#pragma once

#include "Types.h"
#include <memory>
#include <mutex>
#include <semaphore>

namespace bgl
{
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
		std::string m_name{ "" };

	public:
		BObject( const char* name = "None" )
		{
			m_name = std::string( name );
		}

		const std::string& getName() const
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
	};

	class BComponent : public BObject
	{
		BTransform< float > m_dummyTransform;

		bool m_bHidden{ false };

	protected:
		BNode* m_owner{ nullptr };
		BModule* m_module{ nullptr };

	public:
		BComponent( BNode* owner = nullptr, const char* name = "None" );
		virtual ~BComponent() = default;

		bool isVisible() const;
		void setOwner( BNode* owner );

		BTransform< float >& getTransform_mutable();
		BTransform< float > getTransform() const;
		BVec3f getLocation() const;
		BRotf getRotation() const;
		BVec3f getScale() const;

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

	public:
		BMeshComponent( BNode* owner = nullptr, const char* name = "None", const char* assetPath = nullptr );
		~BMeshComponent();

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
		BCameraComponent( BNode* owner = nullptr, const char* name = "None", BViewport* viewport = nullptr );
		~BCameraComponent();

		BViewport* getViewport() const;
		void setViewport( BViewport* viewport );

		BCamera* getCamera() const;
	};

	class BNode : public BObject
	{
		friend class BScene;

	protected:
		BNode* m_parent{ nullptr };
		BArray< BNode* > m_children;
		BArray< BComponent* > m_components;
		BModule* m_module{ nullptr };

		BTransform< float > m_transform;

		bool m_bHidden{ false };

	public:
		BNode( BNode* parent = nullptr, const char* name = "None", BModule* owningModule = nullptr );

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

		bool isVisible() const;
		void setHidden( bool bHidden );

		BTransform< float >& getTransform_mutable();
		const BTransform< float > getTransform() const;
		const BVec3f getLocation() const;
		const BRotf getRotation() const;
		const BVec3f getScale() const;

		void setTransform( const BTransform< float >& transform );
		void setLocation( const BVec3f& location );
		void setRotation( const BRotf& rotation );
		void setScale( const BVec3f& scale );

		void setParent( BNode* node );
		void addChild( BNode* node );
		void removeChild( BNode* node );
	};

	class BScene
	{
		std::unique_ptr< BNode > m_sceneRoot;
		BArray< BNode* > m_nodes;
		BArray< BComponent* > m_components;

	public:
		BScene();

		~BScene()
		{
			for( const auto node : m_nodes )
			{
				delete node;
			}

			for( const auto component : m_components )
			{
				delete component;
			}
		}
		
		std::counting_semaphore< 2 > sceneSemaphore{ 2 };

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
			T* newComponent = new T( node, args... );
			m_components.push_back( newComponent );
			node->m_components.add( newComponent );
			return newComponent;
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