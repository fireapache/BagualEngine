#pragma once

#include "Types.h"

#include <memory>

namespace bgl
{
	class BObject
	{

	protected:

		std::string m_name;

	public:

		BObject(const char* name = "None")
		{
			m_name = std::string(name);
		}

	};

	class BComponent : public BObject
	{

	protected:

		BObject* m_owner;

	public:

		BComponent(BObject* owner = nullptr, const char* name = "None")
		{
			SetOwner(owner);
			m_name = std::string(name);
		}

		void SetOwner(BObject* owner)
		{
			m_owner = owner;
		}

	};

	class BMeshComponent : public BComponent
	{
		friend class BScene;

		static BArray<BArray<BTriangle<float>>*> g_meshComponentTriangles;

	protected:

		BArray<BTriangle<float>> m_triangles;

	public:

		BMeshComponent(BObject* owner = nullptr, const char* name = "None", const char* assetPath = nullptr);
		~BMeshComponent();

		void LoadMesh(const char* assetPath);

		BArray<BTriangle<float>>& GetTriangles();

	};

	class BNode : public BObject
	{

	protected:

		BNode* m_parent;
		BArray<BNode*> m_childs;
		BArray<std::shared_ptr<BComponent>> m_components;

		BTransform<float> m_transform;

	public:

		BNode(BNode* parent = nullptr, const char* name = "None");

		BNode* GetParent();
		BArray<BNode*>& GetChilds();
		BArray<std::shared_ptr<BComponent>>& GetComponents();

		template<class T, typename... P>
		void CreateComponent(P... args)
		{
			static_assert(std::is_base_of<BComponent, T>::value, 
				"Type should inherit from BComponent");
			m_components.push_back(std::make_shared<T>(this, args...));
		}

		BTransform<float> GetTransform();
		BVec3f GetLocation();
		BVec3f GetRotation();
		BVec3f GetScale();

		void SetParent(BNode* node);
		void AddChild(BNode* node);
		void RemoveChild(BNode* node);

		void SetTransform(const BTransform<float>& transform);
		void SetLocation(const BVec3f& translation);
		void SetRotation(const BVec3f& rotation);
		void SetScale(const BVec3f& scale);

	};

	class BScene
	{
		std::shared_ptr<BNode> m_sceneRoot;
		BArray<std::shared_ptr<BNode>> m_nodes;

	public:

		BScene();

		BArray<BArray<BTriangle<float>>*>& GetMeshComponentTriangles()
		{
			return BMeshComponent::g_meshComponentTriangles;
		}

		BNode* CreateNode(const char* name = "None");
		BNode* CreateNode(BNode& parent, const char* name = "None");

		BNode* GetRootNode()
		{
			return m_sceneRoot.get();
		}

		BArray<std::shared_ptr<BNode>>& GetNodes()
		{
			return m_nodes;
		}

	};
}