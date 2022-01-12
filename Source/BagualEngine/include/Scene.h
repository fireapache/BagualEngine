#pragma once

#include "Types.h"

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

		std::shared_ptr<BObject> m_owner;

	public:

		BComponent(BObject* owner = nullptr, const char* name = "None")
		{
			if (owner)
			{
				m_owner = std::make_shared<BObject>(*owner);
			}
			
			m_name = std::string(name);
		}

		void SetOwner(BObject* owner)
		{
			if (owner)
			{
				m_owner = std::make_shared<BObject>(*owner);
			}
		}

	};

	class MeshComponent : public BComponent
	{


	protected:

		BArray<BTriangle<float>> m_triangles;

	public:

		MeshComponent() = default;
		MeshComponent(const char* assetPath, uint32 meshID = 0);

		void LoadMesh(const char* assetPath, uint32 meshID = 0);

		BArray<BTriangle<float>>& GetTriangles();

	};

	class BNode : public BObject
	{

	protected:

		std::shared_ptr<BNode> m_parent;
		BArray<std::shared_ptr<BNode>> m_childs;
		BArray<std::shared_ptr<BComponent>> m_components;

		BTransform<float> m_transform;

	public:

		BNode(BNode* parent = nullptr, const char* name = "None");

		std::shared_ptr<BNode> GetParent();
		BArray<std::shared_ptr<BNode>>& GetChilds();
		BArray<std::shared_ptr<BComponent>>& GetComponents();

		BTransform<float> GetTransform();
		BVec3f GetLocation();
		BVec3f GetRotation();
		BVec3f GetScale();

		void SetParent(std::shared_ptr<BNode> node);
		void AddChild(std::shared_ptr<BNode> node);
		void RemoveChild(std::shared_ptr<BNode> node);

		void SetTransform(const BTransform<float>& transform);
		void SetLocation(const BVec3f& translation);
		void SetRotation(const BVec3f& rotation);
		void SetScale(const BVec3f& scale);

	};

	class BScene
	{
		std::shared_ptr<BNode> m_sceneRoot;

	public:

		BScene();

		std::shared_ptr<BNode> AddNode(const char* name = "None");
		std::shared_ptr<BNode> AddNode(BNode& parent, const char* name = "None");

	};
}