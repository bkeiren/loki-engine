#include "core/graphics/Model.h"
#include "core/graphics/Material.h"
#include "core/graphics/Texture.h"
#include "core/graphics/Mesh.h"
#include "core/graphics/Vertex.h"
#include "core/graphics/IndexBuffer.h"
#include "core/graphics/VertexBuffer.h"

#include "AssImp/assimp.hpp"
#include "AssImp/aiPostProcess.h"
#include "AssImp/aiScene.h"

#include "util/json/json.h"

#include "core/renderer/effect/effectmanager.h"

namespace loki
{

namespace graphics
{

Model::Model()	:
	m_Scale(vec3(1.0f, 1.0f, 1.0f)),
	m_UVScale(vec2(1.0f, 1.0f))
{

}

Model::~Model()
{
	m_Meshes.clear();
	m_Materials.clear();
}

Model* Model::Load( const std::string& _LMOFile )
{
	util::JSONDocument* doc = util::JSONDocument::Open(_LMOFile);
	if (!doc)
	{
		LOG(VL_ERROR, "Model::Load: Failed to load LMO file '%s'", _LMOFile.c_str());
		return 0;
	}

	util::JSONValue& root = doc->GetRoot();
	
	util::JSONValue meshValue = root["mesh"];
	util::JSONValue materialsValue = root["materials"];

	std::string MeshFile;
	std::list<std::string> MaterialsList;

	if (meshValue.IsString())
	{
		MeshFile = meshValue.AsString();
	}

	// Read all material strings.
	for (uint32 i = 0; i < materialsValue.Size(); ++i)
	{
		util::JSONValue material = materialsValue[i];
		if (material.IsString())
		{
			MaterialsList.push_back(material.AsString());
		}
	}

	// Close JSON document and set pointer to 0.
	JSON_CLOSE(doc);

	// Istantiate new model.
	Model* mdl = new Model();
	
	// Generate meshes.
	_CreateMeshesFromGeometryFile(MeshFile, mdl->m_Meshes);

	// Generate materials.
	for (std::list<std::string>::iterator it = MaterialsList.begin(); it != MaterialsList.end(); ++it)
	{
		Material* mat = _CreateMaterialFromLMAFile((*it));
		if (mat)
		{
			mdl->m_Materials.push_back(mat);
		}
	}

	return mdl;
}

void Model::SetScale( const vec3& _Scale )
{
	m_Scale = _Scale;
}

void Model::SetUVScale( const vec2& _Scale )
{
	m_UVScale = _Scale;
}

const vec3& Model::GetScale() const
{
	return m_Scale;
}

const vec2& Model::GetUVScale() const
{
	return m_UVScale;
}

void Model::Render( const mat4& _ModelMatrix, const mat4& _ViewMatrix, const mat4& _ProjectionMatrix, float _ZFar, float _ZNear )
{
	for (unsigned int i = 0; i < m_Meshes.size(); ++i)
	{
		graphics::Mesh* mesh = m_Meshes[i];
		const Material* material = m_Materials[i];
		renderer::LkEffect* effect = material->GetEffect();

		if (!effect)
		{
			LOG(VL_ERROR, "Model::_Render: No effect associated with material");
			continue;
		}

		renderer::LkEffectParameter* param = 0;

		// Set the global ambient color.
		// TODO.

#define SETCGPARAM(paramname, value)	{param = effect->GetParameterBySemantic(paramname);if(param){param->Set(value);}}

		SETCGPARAM("LKMODELVIEWPROJ", _ProjectionMatrix * _ViewMatrix * _ModelMatrix);		// Set the model view projection matrix.
		SETCGPARAM("LKMODELMATRIX", _ModelMatrix);			// Set the model matrix.
		SETCGPARAM("LKMODELMATRIXIT", mat3(math::transpose(math::inverse(_ModelMatrix))));		// Set the inverse transpose of the model matrix.	
		SETCGPARAM("LKEYEPOSITION", math::inverse(_ViewMatrix)[3]);			// Set the eye position.
		SETCGPARAM("LKVIEWMATRIX", _ViewMatrix);
		SETCGPARAM("LKMODELSCALE", m_Scale);	// Set the model scale.
		SETCGPARAM("LKUVSCALE", m_UVScale);		// Set the UV scale.
		SETCGPARAM("LKZFAR", _ZFar);	// Set the Z-far value.
		SETCGPARAM("LKZNEAR", _ZNear);	// Set the Z-near value.
		SETCGPARAM("LKMATERIALSHININESS", material->GetShininess());

		const Texture* tex = 0;

		// Set the diffuse texture.
		tex = material->GetTexture(Material::TT_DIFFUSE);
		SETCGPARAM("LKDIFFUSETEX", (tex)?(tex->GetTextureHandle()):((GLuint)0));

		// Set the normal texture.
		tex = material->GetTexture(Material::TT_NORMAL);
		SETCGPARAM("LKNORMALTEX", (tex)?(tex->GetTextureHandle()):((GLuint)0));

		// Set the specular texture.
		tex = material->GetTexture(Material::TT_SPECULAR);
		SETCGPARAM("LKSPECULARTEX", (tex)?(tex->GetTextureHandle()):((GLuint)0));

		// Set the emissive texture.
		tex = material->GetTexture(Material::TT_EMISSIVE);
		SETCGPARAM("LKEMISSIVETEX", (tex)?(tex->GetTextureHandle()):((GLuint)0));

		while (effect->HasNextPass())
		{
			mesh->Draw();
		}
	}
}

const Mesh* Model::GetMesh( unsigned int _Index ) const
{
	if (_Index >= 0 && _Index < m_Meshes.size())
	{
		return m_Meshes[_Index];
	}
	return 0;
}

void Model::_CreateMeshesFromGeometryFile( const std::string& _GeometryFile, Meshes& _Output )
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
		LOG(VL_ERROR, "Model::_CreateMeshFromGeometryFile: Failed to load scene from file '%s':\n%s", _GeometryFile.c_str(), LocalImporter->GetErrorString());
	}

	_Output.clear();


	unsigned int NumMeshes = LocalScene->mNumMeshes;
	aiMesh** m_tempMeshArray = LocalScene->mMeshes;

	for (unsigned int i = 0; i < NumMeshes; ++i)
	{       
		graphics::IndexBuffer* ibo = 0;
		graphics::VertexBuffer* vbo = 0;

		int NumFaces = m_tempMeshArray[i]->mNumFaces;
		int NumVerts = m_tempMeshArray[i]->mNumVertices;
		int NumIndices = NumFaces * 3;
		Vertex* Vertices = new Vertex[NumVerts];
		unsigned int* Indices = new unsigned int[NumIndices];

		for (int j = 0; j < NumFaces; ++j)
		{
			Indices[j * 3] = m_tempMeshArray[i]->mFaces[j].mIndices[0];
			Indices[j * 3 + 1] = m_tempMeshArray[i]->mFaces[j].mIndices[1];
			Indices[j * 3 + 2] = m_tempMeshArray[i]->mFaces[j].mIndices[2];
		}
		ibo = graphics::IndexBuffer::Create(Indices, NumIndices);
		if (!ibo/*m_SubMeshes[i]->_CreateIndexBuffer(Indices, NumIndices)*/)
		{
			LOG(VL_ERROR, "Model::_CreateMeshesFromGeometryFile: Failed to instantiate IndexBuffer class");
			assert("Model::_CreateMeshesFromGeometryFile: Failed to instantiate IndexBuffer class" && 0);
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
		vbo = graphics::VertexBuffer::Create(Vertices, NumVerts);
		if(!vbo/*m_SubMeshes[i]->_CreateVertexBuffer(Vertices, NumVerts)*/)
		{
			LOG(VL_ERROR, "Model::_CreateMeshesFromGeometryFile: Failed to instantiate VertexBuffer class");
			assert("Model::_CreateMeshesFromGeometryFile: Failed to instantiate VertexBuffer class" && 0);
		}

		graphics::Mesh* mesh = graphics::Mesh::Create(ibo, vbo);
		if (!mesh)
		{
			LOG(VL_ERROR, "Model::_CreateMeshesFromGeometryFile: Failed to instantiate Mesh class");
		}
		else
		{
			_Output.push_back(mesh);
		}

		// No need to delete Vertices or Indices because we transferred ownership to the submesh.
		// The submesh will take care of deleting the data at destruction.
	}
}

Material* Model::_CreateMaterialFromLMAFile( const std::string& _LMAFile )
{
	util::JSONDocument* doc = util::JSONDocument::Open(_LMAFile);
	if (!doc)
	{
		LOG(VL_ERROR, "Model::_CreateMaterialFromLMAFile: Failed to open LMA file '%s'", _LMAFile.c_str());
		return 0;
	}

	util::JSONValue& root = doc->GetRoot();
	
	util::JSONValue diffuseTexString = root["diffuse"];
	util::JSONValue specularTexString = root["specular"];
	util::JSONValue normalTexString = root["normal"];
	util::JSONValue emissiveTexString = root["emissive"];
	util::JSONValue effectString = root["effect"];
	util::JSONValue shininessString = root["shininess"];

	Material* mtl = new Material();

	if (diffuseTexString.IsString())
	{
		mtl->SetTexture(Material::TT_DIFFUSE, Texture::Load(diffuseTexString.AsString()));
	}
	if (specularTexString.IsString())
	{
		mtl->SetTexture(Material::TT_SPECULAR, Texture::Load(specularTexString.AsString()));
	}
	if (normalTexString.IsString())
	{
		mtl->SetTexture(Material::TT_NORMAL, Texture::Load(normalTexString.AsString()));
	}
	if (emissiveTexString.IsString())
	{
		mtl->SetTexture(Material::TT_EMISSIVE, Texture::Load(emissiveTexString.AsString()));
	}
	if (effectString.IsString())
	{
		std::stringstream ss;
		ss << _LMAFile << (rand()%1024);

		mtl->SetEffect(renderer::g_EffectManager->CreateEffectFromFile(effectString.AsString(), ss.str()));
	}
	if (shininessString.IsDouble())
	{
		mtl->SetShininess((float)shininessString.AsDouble());
	}

	JSON_CLOSE(doc);

	return mtl;
}

}

}