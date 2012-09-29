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
public:
	~Model();

	static Model* Load( const std::string& _LMOFile );

private:
	Model();

	static void _CreateMeshesFromGeometryFile( const std::string& _GeometryFile, Meshes& _Output );
	static Material* _CreateMaterialFromLMAFile( const std::string& _LMAFile );

	Meshes m_Meshes;
	Materials m_Materials;
};

}

}

#endif