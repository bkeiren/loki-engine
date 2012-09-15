#include "core/renderer/geometry/submesh/submesh.h"
#include "core/renderer/geometry/mesh/mesh.h"
#include "core/renderer/geometry/vertex/vertex.h"

#include <AssImp//assimp.h>
#include <AssImp//assimp.hpp>
#include <AssImp//aiMesh.h>
#include <AssImp//aiScene.h>
#include <AssImp//aiPostProcess.h>

namespace loki
{

namespace renderer
{

LkMesh::LkMesh( const aiScene* _aiScene )	:
	m_NumSubMeshes(0),
	m_SubMeshes(0)
{
	unsigned int NumMeshes = _aiScene->mNumMeshes;
	m_SubMeshes = new LkSubMesh*[NumMeshes];
	m_NumSubMeshes = NumMeshes;

	aiMesh** m_tempMeshArray = _aiScene->mMeshes;

	for (unsigned int i = 0; i < NumMeshes; ++i)
	{       
		m_SubMeshes[i] = new LkSubMesh();

		int NumFaces = m_tempMeshArray[i]->mNumFaces;
		int NumVerts = m_tempMeshArray[i]->mNumVertices;
		int NumIndices = NumFaces * 3;
		LkVertex* Vertices = new LkVertex[NumVerts];
		unsigned int* Indices = new unsigned int[NumIndices];

		for (int j = 0; j < NumFaces; ++j)
		{
			Indices[j * 3] = m_tempMeshArray[i]->mFaces[j].mIndices[0];
			Indices[j * 3 + 1] = m_tempMeshArray[i]->mFaces[j].mIndices[1];
			Indices[j * 3 + 2] = m_tempMeshArray[i]->mFaces[j].mIndices[2];
		}
		if (!m_SubMeshes[i]->_CreateIndexBuffer(Indices, NumIndices))
		{
			LOG(VL_ERROR, "Mesh::Mesh: SubMesh::_CreateIndexBuffer failed");
			assert("Mesh::Mesh: SubMesh::_CreateIndexBuffer failed" && 0);
		}
		for (int y = 0; y < NumVerts; ++y)
		{
			if (m_tempMeshArray[i]->mVertices)			Vertices[y].pos			= vec3(m_tempMeshArray[i]->mVertices[y].x,			m_tempMeshArray[i]->mVertices[y].y,		m_tempMeshArray[i]->mVertices[y].z);
			if (m_tempMeshArray[i]->mNormals)			Vertices[y].normal		= vec3(m_tempMeshArray[i]->mNormals[y].x,			m_tempMeshArray[i]->mNormals[y].y,		m_tempMeshArray[i]->mNormals[y].z);
			if (m_tempMeshArray[i]->mBitangents)		Vertices[y].binormal	= vec3(m_tempMeshArray[i]->mBitangents[y].x,		m_tempMeshArray[i]->mBitangents[y].y,	m_tempMeshArray[i]->mBitangents[y].z);
		
			// IMPORTANT NOTE: The tangent is negated because apparently, that's what is required when using Cg. If this negation is not performed,
			// certain faces will have incorrect TBN matrices and will not be properly shaded.
			if (m_tempMeshArray[i]->mTangents)			Vertices[y].tangent		= -vec3(m_tempMeshArray[i]->mTangents[y].x,			m_tempMeshArray[i]->mTangents[y].y,		m_tempMeshArray[i]->mTangents[y].z);
			if (m_tempMeshArray[i]->mTextureCoords[0])	Vertices[y].uv			= vec2(m_tempMeshArray[i]->mTextureCoords[0][y].x, m_tempMeshArray[i]->mTextureCoords[0][y].y);
		}
		if(!m_SubMeshes[i]->_CreateVertexBuffer(Vertices, NumVerts))
		{
			LOG(VL_ERROR, "Mesh::Mesh: SubMesh::_CreateVertexBuffer failed");
			assert("Mesh::Mesh: SubMesh::_CreateVertexBuffer failed" && 0);
		}

		// No need to delete Vertices or Indices because we transferred ownership to the submesh.
		// The submesh will take care of deleting the data at destruction.
	}
}

LkMesh::LkMesh()
{
	ILLEGAL_CTOR_ERROR("Mesh");
}

LkMesh::~LkMesh()
{
	for (unsigned int i = 0; i < m_NumSubMeshes; ++i)
	{
		delete m_SubMeshes[i];
	}
	delete[] m_SubMeshes;
}

unsigned int LkMesh::GetNumSubMeshes() const
{
	return m_NumSubMeshes;
}

const LkSubMesh* LkMesh::GetSubMesh( unsigned int _Index ) const
{
	assert(_Index >= 0 && _Index < m_NumSubMeshes);
	return m_SubMeshes[_Index];
}

}

}
