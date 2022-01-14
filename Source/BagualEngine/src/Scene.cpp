
#include "Bagual.pch.h"

#include "Scene.h"

#include <obj_parse.h>

namespace bgl
{

	BNode::BNode(BNode* parent, const char* name)
	{
		m_parent = parent;

		if (name)
		{
			this->m_name = std::string(name);
		}

	}

	BNode* BNode::GetParent()
	{
		return m_parent;
	}

	BArray<BNode*>& BNode::GetChilds()
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

	void BNode::SetParent(BNode* node)
	{
		if (m_parent)
		{
			m_parent->RemoveChild(this);
		}

		m_parent = node;
		node->AddChild(this);
	}

	void BNode::AddChild(BNode* node)
	{
		m_childs.Add(node);
	}

	void BNode::RemoveChild(BNode* node)
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

	MeshComponent::MeshComponent(const char* assetPath)
	{
		LoadMesh(assetPath);
	}

	void MeshComponent::LoadMesh(const char* assetPath)
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

			m_triangles.Add(triCache);
		}
	}

	bgl::BArray<bgl::BTriangle<float>>& MeshComponent::GetTriangles()
	{
		return m_triangles;
	}

	BScene::BScene()
	{
		m_sceneRoot = std::make_unique<BNode>(nullptr, "Scene Root Actor");
	}

	BNode* BScene::AddNode(const char* name /*= "None"*/)
	{
		m_nodes.push_back(std::make_shared<BNode>(m_sceneRoot.get(), name));
		BNode* rawPtr = m_nodes.back().get();
		m_sceneRoot->AddChild(rawPtr);
		return rawPtr;
	}

	BNode* BScene::AddNode(BNode& parent, const char* name /*= "None"*/)
	{
		m_nodes.push_back(std::make_shared<BNode>(&parent, name));
		BNode* rawPtr = m_nodes.back().get();
		m_sceneRoot->AddChild(rawPtr);
		return rawPtr;
	}

}