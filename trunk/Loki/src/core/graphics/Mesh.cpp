#include "core/graphics/Mesh.h"
#include "core/graphics/SubMesh.h"
#include "core/graphics/Vertex.h"
#include "core/graphics/IndexBuffer.h"
#include "core/graphics/VertexBufferObject.h"

#include "AssImp/assimp.hpp"
#include "AssImp/aiPostProcess.h"
#include "AssImp/aiScene.h"

namespace loki
{

namespace graphics
{

Mesh::Meshes Mesh::m_Meshes;

Mesh::Mesh( const SubMeshes& _SubMeshes )	:
	m_SubMeshes(_SubMeshes)
{

}

Mesh::Mesh()
{
	ILLEGAL_CTOR_ERROR("Mesh")
}

Mesh::~Mesh()
{

}

Mesh* Mesh::LoadMesh( const std::string& _GeometryFile )
{
	Mesh* mesh = _FindMesh(_GeometryFile);
	if (!mesh)
	{
		mesh = _CreateMeshFromGeometryFile(_GeometryFile);
	}
	return mesh;
}

const SubMesh* Mesh::GetSubMesh( int32 _Index ) const
{
	assert(_Index >= 0 && (uint32)_Index < m_SubMeshes.size());
	return m_SubMeshes[_Index];
}

uint32 Mesh::GetSubMeshCount() const
{
	return m_SubMeshes.size();
}

Mesh* Mesh::_CreateMeshFromGeometryFile( const std::string& _GeometryFile )
{
	static Assimp::Importer* LocalImporter = new Assimp::Importer();
	const aiScene* LocalScene = LocalImporter->ReadFile(_GeometryFile.c_str(),	
																				aiProcess_JoinIdenticalVertices		|
																				aiProcess_GenNormals                |
																				aiProcess_CalcTangentSpace          |
																				aiProcess_GenUVCoords               |
																				aiProcess_SortByPType               |
																				aiProcess_Triangulate               |
																				aiProcess_OptimizeMeshes            |
																				aiProcess_FindInvalidData           |
																				/*aiProcess_FlipUVs                   |*/
																				/*aiProcess_FlipWindingOrder          |*/
																				aiProcess_ImproveCacheLocality      );

	if (!LocalScene)
	{
		LOG(VL_ERROR, "Mesh::_CreateMeshFromGeometryFile: Failed to load scene from file '%s':\n%s", _GeometryFile.c_str(), LocalImporter->GetErrorString());
	}

	uint32 NumMeshes = LocalScene->mNumMeshes;
	aiMesh** m_tempMeshArray = LocalScene->mMeshes;

	SubMeshes Output;

	for (uint32 i = 0; i < NumMeshes; ++i)
	{
		graphics::IndexBuffer* ibo = 0;
		graphics::VertexBufferObject* vbo = 0;

		int32 NumFaces = m_tempMeshArray[i]->mNumFaces;
		int32 NumVerts = m_tempMeshArray[i]->mNumVertices;
		int32 NumIndices = NumFaces * 3;
		Vertex* Vertices = new Vertex[NumVerts];
		uint32* Indices = new uint32[NumIndices];

		for (int32 j = 0; j < NumFaces; ++j)
		{
			Indices[j * 3] = m_tempMeshArray[i]->mFaces[j].mIndices[0];
			Indices[j * 3 + 1] = m_tempMeshArray[i]->mFaces[j].mIndices[1];
			Indices[j * 3 + 2] = m_tempMeshArray[i]->mFaces[j].mIndices[2];
		}
		ibo = graphics::IndexBuffer::Create(Indices, NumIndices);
		if (!ibo/*m_SubMeshes[i]->_CreateIndexBuffer(Indices, NumIndices)*/)
		{
			LOG(VL_ERROR, "Mesh::_CreateMeshesFromGeometryFile: Failed to instantiate IndexBuffer class");
			assert("Mesh::_CreateMeshesFromGeometryFile: Failed to instantiate IndexBuffer class" && 0);
		}
		for (int32 y = 0; y < NumVerts; ++y)
		{
			if (m_tempMeshArray[i]->mVertices)			Vertices[y].pos			= vec3(m_tempMeshArray[i]->mVertices[y].x,			m_tempMeshArray[i]->mVertices[y].y,		m_tempMeshArray[i]->mVertices[y].z);
			if (m_tempMeshArray[i]->mNormals)			Vertices[y].normal		= vec3(m_tempMeshArray[i]->mNormals[y].x,			m_tempMeshArray[i]->mNormals[y].y,		m_tempMeshArray[i]->mNormals[y].z);
			if (m_tempMeshArray[i]->mBitangents)		Vertices[y].bitangent	= vec3(m_tempMeshArray[i]->mBitangents[y].x,		m_tempMeshArray[i]->mBitangents[y].y,	m_tempMeshArray[i]->mBitangents[y].z);

			// IMPORTANT NOTE: The tangent is negated because apparently, that's what is required when using Cg. If this negation is not performed,
			// certain faces will have incorrect TBN matrices and will not be properly shaded.
			if (m_tempMeshArray[i]->mTangents)			Vertices[y].tangent		= vec3(m_tempMeshArray[i]->mTangents[y].x,			m_tempMeshArray[i]->mTangents[y].y,		m_tempMeshArray[i]->mTangents[y].z);
			if (m_tempMeshArray[i]->mTextureCoords[0])	Vertices[y].uv			= vec2(m_tempMeshArray[i]->mTextureCoords[0][y].x, m_tempMeshArray[i]->mTextureCoords[0][y].y);

			// 			if (math::leftHanded(Vertices[i].tangent, Vertices[i].bitangent, Vertices[i].normal))
			// 			{
			// 				Vertices[i].tangent *= -1.0f;
			// 			}
		}
		vbo = graphics::VertexBufferObject::Create(Vertices, NumVerts);
		if(!vbo/*m_SubMeshes[i]->_CreateVertexBufferObject(Vertices, NumVerts)*/)
		{
			LOG(VL_ERROR, "Mesh::_CreateMeshesFromGeometryFile: Failed to instantiate VertexBufferObject class");
			assert("Mesh::_CreateMeshesFromGeometryFile: Failed to instantiate VertexBufferObject class" && 0);
		}

		graphics::SubMesh* submesh = graphics::SubMesh::Create(ibo, vbo);
		if (!submesh)
		{
			LOG(VL_ERROR, "Mesh::_CreateMeshesFromGeometryFile: Failed to instantiate SubMesh class");
		}
		else
		{
			Output.push_back(submesh);
		}

		// No need to delete Vertices or Indices because we transferred ownership to the submesh.
		// The submesh will take care of deleting the data at destruction.
	}

	Mesh* mesh = new Mesh(Output);
	m_Meshes[_GeometryFile] = mesh;
	return mesh;
}

Mesh* Mesh::_FindMesh( const std::string& _MeshFile )
{
	MeshesConstIter it = m_Meshes.find(_MeshFile);
	if (it == m_Meshes.end())
	{
		return 0;
	}
	return (*it).second;
}

}

}
