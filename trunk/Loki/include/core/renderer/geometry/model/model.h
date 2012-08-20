#pragma once

#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <GLEW\\glew.h>
#include "core/renderer/glslshader/glslshader.h"
#include "core/resourcemanager/resource/resource.h"
#include "core/resourcemanager/resourcemanager.h"

struct aiScene;

namespace loki
{

namespace renderer
{

class LkMesh;
class LkTexture;
class LkMaterial;

class LkModel	: public LkResource
{
	friend class ::loki::LkResourceManager<LkModel*>;
	friend class LkRenderer;
public:
	LkModel( const char* _File );
	virtual ~LkModel();

	void SetUVScale( const glm::vec2& _Scale );
	const glm::vec2& GetUVScale() const;
	void SetScale( const glm::vec3& _Scale );
	void SetScaleX( float _ScaleX );
	void SetScaleY( float _ScaleY );
	void SetScaleZ( float _ScaleZ );
	const glm::vec3& GetScale() const;
	float GetScaleX() const;
	float GetScaleY() const;
	float GetScaleZ() const;
	LkMaterial* GetMaterial( unsigned int _Index );
	void SetMaterial( LkMaterial* _Material, unsigned int _Index, bool _DeleteOldMaterial = true );
	
	const LkMesh* GetMesh() const;
private:
	void _Load();
	void _LoadMesh( const aiScene* _aiScene );
	void _LoadMaterials( const aiScene* _aiScene );
	void _Render( const glm::mat4& _ModelMatrix, const glm::mat4& _ViewMatrix, const glm::mat4& _ProjectionMatrix, float _ZFar, float _ZNear );

	std::string m_Filename;

	LkMaterial** m_Materials;
	unsigned int m_NumMaterials;

	LkMesh* m_Mesh;

	glm::vec2 m_UVScale;
	glm::vec3 m_Scale;
};

}

}

#endif