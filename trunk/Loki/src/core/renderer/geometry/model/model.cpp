#include "core/renderer/geometry/model/model.h"
#include "core/renderer/geometry/mesh/mesh.h"
//#include "core/renderer/geometry/submesh/submesh.h"
#include "core/renderer/geometry/vertex/vertex.h"
//#include "core/resourcemanager/meshmanager.h"
#include "core/resourcemanager/texturemanager.h"
#include "core/renderer/texture/texture.h"
#include "core/renderer/material/material.h"

#include "core/renderer/effect/effectmanager.h"

#include <AssImp//assimp.hpp>
#include <AssImp//aiMesh.h>
#include <AssImp//aiScene.h>
#include <AssImp//aiPostProcess.h>

#include "cg/cgGL.h"

#include "core/graphics/Mesh.h"

using namespace loki;
using namespace loki::renderer;

LkModel::LkModel( const char* _File )	:
	LkResource(_File),
	m_Mesh(0),
	m_UVScale(vec2(1.0f, 1.0f)),
	m_Scale(vec3(1.0f, 1.0f, 1.0f))
{
	assert(_File != NULL);
	m_Filename = std::string(_File);

	_Load();

// 	if (_DiffuseMap != 0)
// 		m_DiffuseMap = g_TextureManager->GetResource(_DiffuseMap);
// 	if (_NormalMap != 0)
// 		m_NormalMap = g_TextureManager->GetResource(_NormalMap);
// 	if (_SpecularMap != 0)
// 		m_SpecularMap = g_TextureManager->GetResource(_SpecularMap);
// 	if (_EmissiveMap != 0)
// 		m_EmissiveMap = g_TextureManager->GetResource(_EmissiveMap);
}

LkModel::~LkModel()
{
// 	g_TextureManager->ReleaseResource(&m_DiffuseMap);
// 	g_TextureManager->ReleaseResource(&m_NormalMap);
// 	g_TextureManager->ReleaseResource(&m_SpecularMap);
// 	g_TextureManager->ReleaseResource(&m_EmissiveMap);
}

void LkModel::SetUVScale( const vec2& _Scale )
{
	m_UVScale = _Scale;
}

const vec2& LkModel::GetUVScale() const
{
	return m_UVScale;
}

void LkModel::SetScale( const vec3& _Scale )
{
	m_Scale = _Scale;
}

void LkModel::SetScaleX( float _ScaleX )
{
	m_Scale.x = _ScaleX;
}

void LkModel::SetScaleY( float _ScaleY )
{
	m_Scale.y = _ScaleY;
}

void LkModel::SetScaleZ( float _ScaleZ )
{
	m_Scale.z = _ScaleZ;
}

const vec3& LkModel::GetScale() const
{
	return m_Scale;
}

float LkModel::GetScaleX() const
{
	return m_Scale.x;
}

float LkModel::GetScaleY() const
{
	return m_Scale.y;
}

float LkModel::GetScaleZ() const
{
	return m_Scale.z;
}

LkMaterial* LkModel::GetMaterial( unsigned int _Index )
{
	assert(_Index < m_NumMaterials);
	return m_Materials[_Index];
}

void LkModel::SetMaterial( LkMaterial* _Material, unsigned int _Index, bool _DeleteOldMaterial /*= true*/ )
{
	assert(_Index < m_NumMaterials);
	assert(_Material != NULL);

	if (_DeleteOldMaterial)
	{
		delete m_Materials[_Index];
	}
	m_Materials[_Index] = _Material;
}

const LkMesh* LkModel::GetMesh() const
{
	return m_Mesh;
}

void LkModel::_Load()
{
	static Assimp::Importer* LocalImporter = new Assimp::Importer();
	const aiScene* LocalScene = LocalImporter->ReadFile(m_Filename.c_str(),	
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
		LOG(VL_ERROR, "Model::_Load: Failed to load scene from file '%s':\n%s", m_Filename.c_str(), LocalImporter->GetErrorString());
	}

	_LoadMesh(LocalScene);
	_LoadMaterials(LocalScene);

	//delete LocalImporter;
}

void LkModel::_LoadMesh( const aiScene* _aiScene )
{
	m_Mesh = new LkMesh(_aiScene);
}

void LkModel::_LoadMaterials( const aiScene* _aiScene )
{
	m_NumMaterials = m_Mesh->GetNumSubMeshes();
	m_Materials = new LkMaterial*[m_NumMaterials];

	unsigned int NumMeshes = m_Mesh->GetNumSubMeshes();
	for (unsigned int i = 0; i < NumMeshes; ++i)
	{
		const unsigned int PathLength = 128;
		char DiffusePath[PathLength] = "\0";
		char NormalPath[PathLength] = "\0";
		char SpecularPath[PathLength] = "\0";
		char EmissivePath[PathLength] = "\0";

		aiMesh** m_tempMeshArray = _aiScene->mMeshes;
		aiMaterial** m_tempMaterialArray = _aiScene->mMaterials;

		aiMaterial* tempMat = m_tempMaterialArray[m_tempMeshArray[i]->mMaterialIndex];
		aiString tempTexStr;
		if (tempMat->GetTexture(aiTextureType_DIFFUSE, 0, &tempTexStr) == aiReturn_SUCCESS)
		{
			memcpy_s((void*)DiffusePath, sizeof(char) * PathLength, (void*)tempTexStr.data, tempTexStr.length);
		}
#define FIX_ASSIMP_NORMALMAP_BUG
#ifdef FIX_ASSIMP_NORMALMAP_BUG
		if (tempMat->GetTexture(aiTextureType_HEIGHT, 0, &tempTexStr) == aiReturn_SUCCESS)
#else
		if (tempMat->GetTexture(aiTextureType_NORMALS, 0, &tempTexStr) == aiReturn_SUCCESS)
#endif
		{
			memcpy_s((void*)NormalPath, sizeof(char) * PathLength, (void*)tempTexStr.data, tempTexStr.length);
		}
		if (tempMat->GetTexture(aiTextureType_SPECULAR, 0, &tempTexStr) == aiReturn_SUCCESS)
		{
			memcpy_s((void*)SpecularPath, sizeof(char) * PathLength, (void*)tempTexStr.data, tempTexStr.length);
		}
		if (tempMat->GetTexture(aiTextureType_EMISSIVE, 0, &tempTexStr) == aiReturn_SUCCESS)
		{
			memcpy_s((void*)EmissivePath, sizeof(char) * PathLength, (void*)tempTexStr.data, tempTexStr.length);
		}

//#define USE_DEFAULT_IF_NO_TEXTURE_PRESENT	// If this is defined, models can not have no texture
											// associated to any texture slot. This can help with testing models
											// without having to assign textures to it, however it will remove the ability
											// to ignore having normal or specular maps assigned to a model.

#ifdef USE_DEFAULT_IF_NO_TEXTURE_PRESENT
	#define PATH_HELPER(a)			(a)
#else
	#define PATH_HELPER(a)			((a[0] == '\0')?(0):(a))
#endif
		m_Materials[i] = new LkMaterial(0,
										PATH_HELPER(DiffusePath), 
										PATH_HELPER(NormalPath), 
										PATH_HELPER(SpecularPath), 
										PATH_HELPER(EmissivePath)	);
	}
}

void LkModel::_Render( const mat4& _ModelMatrix, const mat4& _ViewMatrix, const mat4& _ProjectionMatrix, float _ZFar, float _ZNear )
{
	// If no mesh exists, stop.
	if (!m_Mesh)
	{
		return;
	}

	for (unsigned int i = 0; i < m_Mesh->GetNumSubMeshes(); ++i)
	{
		graphics::Mesh* submesh = m_Mesh->m_SubMeshes[i];
		const LkMaterial* material = GetMaterial(i);
		LkEffect* effect = material->GetEffect();

		if (!effect)
		{
			LOG(VL_ERROR, "Model::_Render: No effect associated with material");
			continue;
		}

		LkEffectParameter* param = 0;

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
		
		const LkTexture* tex = 0;

		// Set the diffuse texture.
		tex = material->GetDiffuse();
		SETCGPARAM("LKDIFFUSETEX", (tex)?(tex->m_OpenGLTextureID):((GLuint)0));

		// Set the normal texture.
		tex = material->GetNormal();
		SETCGPARAM("LKNORMALTEX", (tex)?(tex->m_OpenGLTextureID):((GLuint)0));

		// Set the specular texture.
		tex = material->GetSpecular();
		SETCGPARAM("LKSPECULARTEX", (tex)?(tex->m_OpenGLTextureID):((GLuint)0));

		// Set the emissive texture.
		tex = material->GetEmissive();
		SETCGPARAM("LKEMISSIVETEX", (tex)?(tex->m_OpenGLTextureID):((GLuint)0));

		// Set the environment texture.
		// 			tex = material->GetEmissive();
		// 			if (tex)
		// 			{
		// 				SETCGPARAM("LKENVTEX", tex->m_OpenGLTextureID);
		// 			}

		while (effect->HasNextPass())
		{
			submesh->Draw();
		}
	}

// 	glDisable(GL_CULL_FACE);
// 	glMatrixMode(GL_PROJECTION);
// 	glLoadMatrixf(math::value_ptr(_ProjectionMatrix));
// 	glMatrixMode(GL_MODELVIEW);
// 	glLoadMatrixf(math::value_ptr(_ViewMatrix));
// 	glBegin(GL_TRIANGLES);
// 	glVertex3f(0.0f, 0.0f, -2.0f);
// 	glVertex3f(1.0f, 0.0f, -2.0f);
// 	glVertex3f(1.0f, 1.0f, -2.0f);
// 
// 	glVertex3f(0.0f, 0.0f, 2.0f);
// 	glVertex3f(1.0f, 0.0f, 2.0f);
// 	glVertex3f(1.0f, 1.0f, 2.0f);
// 	glEnd();
}