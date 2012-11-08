#pragma once

#ifndef MODEL_H
#define MODEL_H

namespace loki
{

namespace graphics
{

class Mesh;
class Material;

class Model
{
	CONTAINER_MACRO_VECTOR(Mesh*, Meshes);
	CONTAINER_MACRO_VECTOR(Material*, Materials);
	CONTAINER_MACRO_VECTOR(int, MaterialIndices);
public:
	~Model();

	static Model* Load( const std::string& _LMOFile );

	void SetUVScale( const vec2& _Scale );
	const vec2& GetUVScale() const;

	void Render( const mat4& _ModelMatrix, const mat4& _ViewMatrix, const mat4& _ProjectionMatrix, f32 _ZFar, f32 _ZNear );

	const Mesh* GetMesh( uint32 _Index ) const;
private:
	Model();

	static void _CreateMeshesFromGeometryFile( const std::string& _GeometryFile, Meshes& _Output );
	static Material* _CreateMaterialFromLMAFile( const std::string& _LMAFile );

	Meshes m_Meshes;
	MaterialIndices m_MaterialIndices;
	Materials m_Materials;

	vec2 m_UVScale;
};

}

}

#endif