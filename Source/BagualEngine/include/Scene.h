#pragma once

#include "Types.h"

#include <memory>

namespace bgl
{
	class BNode;
	class BViewport;
	class BCamera;
} // namespace bgl

namespace bgl
{
	class BObject
	{
	protected:
		std::string m_name;

	public:
		BObject( const char* name = "None" )
		{
			m_name = std::string( name );
		}

		const std::string& GetName() const
		{
			return m_name;
		}

		void SetName( const std::string& name )
		{
			m_name = name;
		}

		void SetName( const char* name )
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

		bool m_bHidden = false;

	protected:
		BNode* m_owner;

	public:
		BComponent( BNode* owner = nullptr, const char* name = "None" );
		~BComponent();

		const bool IsVisible() const;
		void SetOwner( BNode* owner );

		BTransform< float >& GetTransform_Mutable();
		const BTransform< float > GetTransform() const;
		const BVec3f GetLocation() const;
		const BRotf GetRotation() const;
		const BVec3f GetScale() const;

		void SetTransform( const BTransform< float >& transform );
		void SetLocation( const BVec3f& locations );
		void SetRotation( const BRotf& rotation );
		void SetScale( const BVec3f& scale );
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
		BRawMeshData m_MeshData;

		bool m_ShowWireframe = false;

	public:
		BMeshComponent( BNode* owner = nullptr, const char* name = "None", const char* assetPath = nullptr );
		~BMeshComponent();

		void LoadMesh( const char* assetPath );

		[[nodiscard]] bool getShowWireframe() const;
		bool& getShowWireframe_Mutable();
		void setShowWireframe( const bool bValue );

		BArray< BTriangle< float > >& GetTriangles();
		BTriangle< BArray< float > >& GetTriangles_SIMD();

		[[nodiscard]] const BRawMeshData& getMeshData() const
		{
			return m_MeshData;
		}

		void AddTriangles( const BArray< BTriangle< float > >& triangles );
	};

	class BCameraComponent : public BComponent
	{
		friend std::unique_ptr< BCamera > std::make_unique< BCamera >();

		std::unique_ptr< BCamera > m_camera;

	public:
		BCameraComponent( BNode* owner = nullptr, const char* name = "None", BViewport* viewport = nullptr );
		~BCameraComponent();

		BViewport* GetViewport() const;
		void SetViewport( BViewport* viewport );

		BCamera* GetCamera() const;
	};

	class BNode : public BObject
	{
	protected:
		BNode* m_parent;
		BArray< BNode* > m_childs;
		BArray< std::shared_ptr< BComponent > > m_components;

		BTransform< float > m_transform;

		bool m_bHidden = false;

	public:
		BNode( BNode* parent = nullptr, const char* name = "None" );

		BNode* GetParent();
		BArray< BNode* >& GetChilds();
		BArray< std::shared_ptr< BComponent > >& GetComponents();

		const bool IsVisible() const;
		void SetHidden( const bool bHidden );

		template< class T, typename... P >
		T* CreateComponent( P... args )
		{
			static_assert( std::is_base_of< BComponent, T >::value, "Type should inherit from BComponent" );
			m_components.push_back( std::make_shared< T >( this, args... ) );
			return static_cast< T* >( m_components.back().get() );
		}

		BTransform< float >& GetTransform_Mutable();
		const BTransform< float > GetTransform() const;
		const BVec3f GetLocation() const;
		const BRotf GetRotation() const;
		const BVec3f GetScale() const;

		void SetTransform( const BTransform< float >& transform );
		void SetLocation( const BVec3f& location );
		void SetRotation( const BRotf& rotation );
		void SetScale( const BVec3f& scale );

		void SetParent( BNode* node );
		void AddChild( BNode* node );
		void RemoveChild( BNode* node );
	};

	class BScene
	{
		std::shared_ptr< BNode > m_sceneRoot;
		BArray< std::shared_ptr< BNode > > m_nodes;

	public:
		BScene();

		BArray< BArray< BTriangle< float > >* >& GetMeshComponentTriangles()
		{
			return BMeshComponent::g_meshComponentTriangles;
		}

		BArray< BMeshComponent* >& GetMeshComponents()
		{
			return BMeshComponent::g_meshComponents;
		}

		BNode* CreateNode( const char* name = "None" );
		BNode* CreateNode( BNode& parent, const char* name = "None" );

		BNode* GetRootNode()
		{
			return m_sceneRoot.get();
		}

		BArray< std::shared_ptr< BNode > >& GetNodes()
		{
			return m_nodes;
		}
	};
} // namespace bgl