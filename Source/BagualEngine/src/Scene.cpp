
#include "Bagual.pch.h"

#include "Scene.h"

#include <obj_parse.h>

namespace bgl
{

	BNode::BNode(BNode* parent, const char* name)
	{
		if (parent)
		{
			m_parent = std::shared_ptr<BNode>(parent);
		}

		if (name)
		{
			this->m_name = std::string(m_name);
		}

	}

	std::shared_ptr<BNode> BNode::GetParent()
	{
		return m_parent;
	}

	BArray<std::shared_ptr<BNode>>& BNode::GetChilds()
	{
		return m_childs;
	}

	BArray<std::shared_ptr<BComponent>>& BNode::GetComponents()
	{
		return m_components;
	}

	bgl::BTransform<float> BNode::GetTransform()
	{
		return m_transform;
	}

	bgl::BVec3f BNode::GetLocation()
	{
		return m_transform.translation;
	}

	bgl::BVec3f BNode::GetRotation()
	{
		return m_transform.rotation;
	}

	bgl::BVec3f BNode::GetScale()
	{
		return m_transform.scale;
	}

	void BNode::SetParent(std::shared_ptr<BNode> node)
	{
		if (m_parent)
		{
			m_parent->RemoveChild(std::shared_ptr<BNode>(this));
		}

		m_parent = node;
		node->AddChild(std::shared_ptr<BNode>(this));
	}

	void BNode::AddChild(std::shared_ptr<BNode> node)
	{
		m_childs.Add(node);
	}

	void BNode::RemoveChild(std::shared_ptr<BNode> node)
	{
		m_childs.Remove(node);
	}

	void BNode::SetTransform(const BTransform<float>& transform)
	{
		m_transform = transform;
	}

	void BNode::SetLocation(const BVec3f& translation)
	{
		m_transform.translation = translation;
	}

	void BNode::SetRotation(const BVec3f& rotation)
	{
		m_transform.rotation = rotation;
	}

	void BNode::SetScale(const BVec3f& scale)
	{
		m_transform.scale = scale;
	}

	MeshComponent::MeshComponent(const char* assetPath, uint32 meshID /*= 0*/)
	{
		LoadMesh(assetPath, meshID);
	}

	void MeshComponent::LoadMesh(const char* assetPath, uint32 meshID /*= 0*/)
	{
		BTriangle<float> triCache;
		objl::Vertex vert0, vert1, vert2;
		uint32 index0, index1, index2;

		objl::Loader triLoader(assetPath);

		for (size_t i = 0; i < triLoader.LoadedIndices.size(); i += 3)
		{
			index0 = triLoader.LoadedIndices[i];
			index1 = triLoader.LoadedIndices[i + 1];
			index2 = triLoader.LoadedIndices[i + 2];

			vert0 = triLoader.LoadedVertices[index0];
			vert1 = triLoader.LoadedVertices[index1];
			vert2 = triLoader.LoadedVertices[index2];

			triCache.v0.x = vert0.Position.X;
			triCache.v0.y = vert0.Position.Y;
			triCache.v0.z = vert0.Position.Z;
			triCache.v1.x = vert1.Position.X;
			triCache.v1.y = vert1.Position.Y;
			triCache.v1.z = vert1.Position.Z;
			triCache.v2.x = vert2.Position.X;
			triCache.v2.y = vert2.Position.Y;
			triCache.v2.z = vert2.Position.Z;

#ifdef TRIANGLE_SOURCE
			triCache.meshID = meshID;
#endif

			m_triangles.Add(triCache);
		}
	}

	bgl::BArray<bgl::BTriangle<float>>& MeshComponent::GetTriangles()
	{
		return m_triangles;
	}

	BScene::BScene()
	{
		m_sceneRoot = std::make_shared<BNode>(nullptr, "Scene Root Actor");
	}

	std::shared_ptr<BNode> BScene::AddNode(const char* name /*= "None"*/)
	{
		std::shared_ptr<BNode> newActor = std::make_shared<BNode>(m_sceneRoot.get(), name);
		m_sceneRoot->AddChild(newActor);
		return newActor;
	}

	std::shared_ptr<BNode> BScene::AddNode(BNode& parent, const char* name /*= "None"*/)
	{
		std::shared_ptr<BNode> newActor = std::make_shared<BNode>(&parent, name);
		parent.AddChild(newActor);
		return newActor;
	}

}