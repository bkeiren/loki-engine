#include "core/graphics/Model.h"

namespace loki
{

namespace graphics
{

Model::Model()
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
	if (!doc->IsOpen())
	{
		JSON_CLOSE(doc);
		LOG(VL_ERROR, "Model::Load: Failed to load LMO file '%s'", _LMOFile.c_str());
		return 0;
	}

	util::JSONValue& root = doc->GetRoot();
	
	util::JSONValue meshValue = root["meshes"];
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
		JSONValue material = materialsValue[i];
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
			mdl->m_Meshes.push_back(mat);
		}
	}
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
		graphics::IndexBufferObject* ibo = 0;
		graphics::VertexBufferObject* vbo = 0;

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
		ibo = graphics::IndexBufferObject::Create(Indices, NumIndices);
		if (!ibo/*m_SubMeshes[i]->_CreateIndexBuffer(Indices, NumIndices)*/)
		{
			LOG(VL_ERROR, "Model::_CreateMeshesFromGeometryFile: Failed to instantiate IndexBufferObject class");
			assert("Model::_CreateMeshesFromGeometryFile: Failed to instantiate IndexBufferObject class" && 0);
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
		vbo = graphics::VertexBufferObject::Create((graphics::Vertex*)Vertices, NumVerts);
		if(!vbo/*m_SubMeshes[i]->_CreateVertexBuffer(Vertices, NumVerts)*/)
		{
			LOG(VL_ERROR, "Model::_CreateMeshesFromGeometryFile: Failed to instantiate VertexBufferObject class");
			assert("Model::_CreateMeshesFromGeometryFile: Failed to instantiate VertexBufferObject class" && 0);
		}

		graphics::Mesh* mesh = graphics::Mesh::Create(ibo, vbo);
		if (!mesh)
		{
			LOG(VL_ERROR, "Model::_CreateMeshesFromGeometryFile: Failed to instantiate Mesh class");
		}
		else
		{
			m_Meshes.push_back(mesh);
		}

		// No need to delete Vertices or Indices because we transferred ownership to the submesh.
		// The submesh will take care of deleting the data at destruction.
	}
}

Material* Model::_CreateMaterialFromLMAFile( const std::string& _LMAFile )
{
	util::JSONDocument* doc = util::JSONDocument::Open(_LMAFile);
	if (!doc->IsOpen())
	{
		JSON_CLOSE(doc);
		LOG(VL_ERROR, "Model::_CreateMaterialFromLMAFile: Failed to open LMA file '%s'", _LMAFile.c_str());
		return 0;
	}

	util::JSONValue& root = doc->GetRoot();
	
	util::JSONValue diffuseTexString = root["diffuse"];
	util::JSONValue specularTexString = root["specular"];
	util::JSONValue normalTexString = root["normal"];
	util::JSONValue effectString = root["effect"];

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
	if (effectString.IsString())
	{
		mtl->SetEffect(LkEffectManager.CreateEffectFromFile(effectString.AsString(), EffectName));
	}

	JSON_CLOSE(doc);

	return mtl;
}

}

}