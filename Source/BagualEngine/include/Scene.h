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

	class MeshComponent : public BComponent
	{


	protected:

		BArray<BTriangle<float>> m_triangles;

	public:

		MeshComponent(const char* assetPath, BObject* owner = nullptr, const char* name = "None");

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

		BNode* AddNode(const char* name = "None");
		BNode* AddNode(BNode& parent, const char* name = "None");

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